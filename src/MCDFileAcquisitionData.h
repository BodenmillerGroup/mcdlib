#ifndef MCD_MCDFILEACQUISITIONDATA_H
#define MCD_MCDFILEACQUISITIONDATA_H

#include <fstream>
#include <memory>
#include <vector>

#include "data/Acquisition.h"
#include "data/ChannelData.h"
#include "MCDFileMalformedException.h"
#include "MCDFileIOException.h"

#ifdef OMETIFF_SUPPORT_ENABLED

#include <ome/files/CoreMetadata.h>
#include <ome/files/MetadataTools.h>
#include <ome/files/VariantPixelBuffer.h>
#include <ome/files/out/OMETIFFWriter.h>

#endif //OMETIFF_SUPPORT_ENABLED

#define CHANNEL_X 0
#define CHANNEL_Y 1

namespace mcd {

    class MCDFileAcquisitionData {

    private:

        const std::string mcdFilePath;
        const std::shared_ptr<data::Acquisition> acquisition;
        std::vector<std::shared_ptr<data::ChannelData>> sortedChannelData;

        MCDFileAcquisitionData(const std::string &mcdPath, const std::shared_ptr<data::Acquisition> &acquisition);

    public:

        static MCDFileAcquisitionData
        loadFromMCD(const std::string &mcdFilePath, const std::shared_ptr<data::Acquisition> &acquisition);

        std::shared_ptr<data::ChannelData> findChannelData(const std::shared_ptr<data::Channel> &channel) const;

        const std::vector<std::shared_ptr<data::ChannelData>> &getChannelData() const;

#ifdef OMETIFF_SUPPORT_ENABLED

        void writeOMETIFF(const std::string &dest, const std::string *compression = nullptr) const;

#endif //OMETIFF_SUPPORT_ENABLED

    };

}

#endif //MCD_MCDFILEACQUISITIONDATA_H