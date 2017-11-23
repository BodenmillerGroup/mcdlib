#include "MCDFileAcquisitionData.h"

using namespace mcd;

MCDFileAcquisitionData::MCDFileAcquisitionData(const std::string &mcdPath,
                                               const std::shared_ptr<data::Acquisition> &acquisition)
        : mcdFilePath(mcdPath), acquisition(acquisition) {
}

MCDFileAcquisitionData MCDFileAcquisitionData::loadFromMCD(const std::string &mcdFilePath,
                                                           const std::shared_ptr<data::Acquisition> &acquisition) {
    // check data offsets
    const auto numChannels = acquisition->getChannels().size();
    const auto numPixelsPerChannel = acquisition->getMaxX() * acquisition->getMaxY();
    const auto expectedDataLength = numChannels * numPixelsPerChannel * sizeof(float);
    uint64_t dataLength = 0;
    if (acquisition->getDataStartOffset() <= acquisition->getDataEndOffset()) {
        // attention: data end offset is pointing to the last byte of the data block!
        dataLength = acquisition->getDataEndOffset() - acquisition->getDataStartOffset() + 1;
        if (dataLength < expectedDataLength) {
            std::cerr << "WARNING: Attempting to correct for missing binary data"
                      << " (metadata: " << dataLength << " B, expected: " << expectedDataLength << " B)" << std::endl;
        } else if (dataLength > expectedDataLength) {
            std::cerr << "WARNING: Attempting to correct for corrupted binary data"
                      << " (metadata: " << dataLength << " B, expected: " << expectedDataLength << " B)" << std::endl;
            dataLength = expectedDataLength;
        }
    } else {
        std::cerr << "WARNING: Invalid metadata (data start offset larger than data end offset)" << std::endl;
    }
    // open file
    std::vector<float> rowBuffer(numChannels);
    std::ifstream file(mcdFilePath, std::ios_base::binary);
    if (!file) {
        throw MCDFileIOException("Could not open file " + mcdFilePath);
    }
    // check file size
    file.seekg(0, std::ios_base::end);
    const auto fileSize = file.tellg();
    if (acquisition->getDataStartOffset() + dataLength > fileSize) {
        uint64_t availableDataLength = 0;
        if (acquisition->getDataStartOffset() < fileSize) {
            availableDataLength = static_cast<uint64_t>(fileSize) - acquisition->getDataStartOffset();
        }
        std::cerr << "WARNING: Attempting to correct for missing binary data "
                  << " (available: " << availableDataLength << "B, expected: " << dataLength << "B)" << std::endl;
        dataLength = availableDataLength;
    }
    // initialize channels
    MCDFileAcquisitionData acquisitionData(mcdFilePath, acquisition);
    for (const std::shared_ptr<data::Channel> &channel : acquisition->getChannels()) {
        acquisitionData.sortedChannelData.push_back(std::make_shared<data::ChannelData>(channel));
    }
    // read data
    const auto rowLength = numChannels * sizeof(float);
    file.seekg(acquisition->getDataStartOffset(), std::ios_base::beg);
    for (uint64_t dataPos = 0; dataPos < dataLength; dataPos += rowLength) {
        auto readLength = rowLength;
        if (dataPos + readLength > dataLength) {
            readLength = dataLength - dataPos;
        }
        //TODO check endianness when reading binary data
        file.read(reinterpret_cast<char *>(rowBuffer.data()), readLength);
        const auto x = static_cast<uint32_t>(rowBuffer[CHANNEL_X]);
        const auto y = static_cast<uint32_t>(rowBuffer[CHANNEL_Y]);
        for (std::size_t channelNum = 0; channelNum < numChannels; ++channelNum) {
            acquisitionData.sortedChannelData[channelNum]->setPixel(x, y, rowBuffer[channelNum]);
        }
    }
    // return result
    return std::move(acquisitionData);
}

const std::vector<std::shared_ptr<data::ChannelData>> &MCDFileAcquisitionData::getChannelData() const {
    return sortedChannelData;
}

std::shared_ptr<data::ChannelData>
MCDFileAcquisitionData::findChannelData(const std::shared_ptr<data::Channel> &channel) const {
    for (const std::shared_ptr<data::ChannelData> &channelData : sortedChannelData) {
        if (channelData->getChannel() == channel) {
            return channelData;
        }
    }
    return nullptr;
}

#ifdef OMETIFF_SUPPORT_ENABLED

void MCDFileAcquisitionData::writeOMETIFF(const std::string &dest, const std::string *compression) const {
    // core metadata
    std::shared_ptr<ome::files::CoreMetadata> coreMeta = std::make_shared<ome::files::CoreMetadata>();
    coreMeta->sizeX = sortedChannelData[0]->getSizeX();
    coreMeta->sizeY = sortedChannelData[0]->getSizeY();
    coreMeta->sizeC = std::vector<ome::files::dimension_size_type>(sortedChannelData.size(), 1);
    coreMeta->sizeZ = 1;
    coreMeta->sizeT = 1;
    coreMeta->dimensionOrder = ome::xml::model::enums::DimensionOrder::XYZTC;
    coreMeta->pixelType = ome::xml::model::enums::PixelType::FLOAT;
    coreMeta->littleEndian = true;
    coreMeta->bitsPerPixel = 32U;
    // xml metadata
    std::shared_ptr<ome::xml::meta::OMEXMLMetadata> xmlMeta = std::make_shared<ome::xml::meta::OMEXMLMetadata>();
    ome::files::fillMetadata(*xmlMeta, {coreMeta});
    xmlMeta->setImageName(mcdFilePath, 0);
    ome::xml::meta::BaseMetadata::index_type channelIndex = 0;
    for (const std::shared_ptr<data::ChannelData> &channelData : sortedChannelData) {
        const std::shared_ptr<data::Channel> &channel = channelData->getChannel();
        xmlMeta->setChannelFluor(channel->getPropertyOrDefault("ChannelName", ""), 0, channelIndex);
        xmlMeta->setChannelName(channel->getPropertyOrDefault("ChannelLabel", ""), 0, channelIndex);
        channelIndex++;
    }
    // writer
    std::shared_ptr<ome::files::FormatWriter> writer = std::make_shared<ome::files::out::OMETIFFWriter>();
    std::shared_ptr<ome::xml::meta::MetadataRetrieve> metaRetrieve(xmlMeta);
    writer->setMetadataRetrieve(metaRetrieve);
    writer->setWriteSequentially(true);
    if (compression != nullptr) {
        writer->setCompression(*compression);
    }
    // buffer
    typedef ome::files::PixelProperties<ome::xml::model::enums::PixelType::FLOAT>::std_type PixelValueType;
    auto buffer = std::make_shared<ome::files::PixelBuffer<PixelValueType>>(
            boost::extents[coreMeta->sizeX][coreMeta->sizeY][1][1][1][1][1][1][1],
            ome::xml::model::enums::PixelType::FLOAT, ome::files::ENDIAN_LITTLE,
            ome::files::PixelBufferBase::make_storage_order(ome::xml::model::enums::DimensionOrder::XYZTC, false));
    ome::files::VariantPixelBuffer vbuffer(buffer);
    // write data
    writer->setId(dest);
    ome::files::dimension_size_type currentChannel = 0;
    ome::files::PixelBufferBase::indices_type currentIdx{};
    for (const std::shared_ptr<data::ChannelData> &channelData : sortedChannelData) {
        //TODO fill OME buffer in a smarter (and faster) way
        for (uint32_t y = 0; y < coreMeta->sizeY; ++y) {
            currentIdx[ome::files::DIM_SPATIAL_Y] = y;
            for (uint32_t x = 0; x < coreMeta->sizeX; ++x) {
                currentIdx[ome::files::DIM_SPATIAL_X] = x;
                buffer->at(currentIdx) = channelData->getPixel(x, y);
            }
        }
        writer->saveBytes(currentChannel, vbuffer);
        currentChannel++;
    }
}

#endif //OMETIFF_SUPPORT_ENABLED