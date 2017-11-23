#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "MCDFile.h"

namespace py = pybind11;

#ifdef OMETIFF_SUPPORT_ENABLED

namespace mcd {

    namespace py {

        void writeOMETIFFUncompressed(const MCDFileAcquisitionData &acquisitionData, const std::string &destPath) {
            acquisitionData.writeOMETIFF(destPath);
        }

    }

}

#endif //OMETIFF_SUPPORT_ENABLED

PYBIND11_MODULE(mcdpy, m) {
    m.doc() = "MCD file parser";

    py::class_<mcd::MCDFile>(m, "MCDFile")
            .def(py::init<const std::string &>(), py::arg("path"))
            .def("readMetadata", &mcd::MCDFile::readMetadata, "Loads the MCD metadata into memory")
            .def("saveSlideImage", &mcd::MCDFile::saveSlideImage, "Writes the picture of the specified slide to a file", py::arg("slide"), py::arg("destPath"))
            .def("savePanoramaImage", &mcd::MCDFile::savePanoramaImage, "Writes the picture of the specified panorama to a file", py::arg("panorama"), py::arg("destPath"))
            .def("saveAcquisitionImage", &mcd::MCDFile::saveAcquisitionImage, "Writes the before/ablation acquisition pictures", py::arg("acquisition"), py::arg("destPath"), py::arg("type"))
            .def("readAcquisitionData", &mcd::MCDFile::readAcquisitionData, "Loads the data for the specified acquisition into memory", py::arg("acquisition"));

    py::enum_<mcd::MCDFile::AcquisitionImageType>(m, "AcquisitionImageType")
            .value("BEFORE", mcd::MCDFile::AcquisitionImageType::BEFORE)
            .value("AFTER", mcd::MCDFile::AcquisitionImageType::AFTER);

    py::class_<mcd::MCDFileMetadata>(m, "MCDFileMetadata")
            .def_property_readonly("schemaXML", &mcd::MCDFileMetadata::getSchemaXML, "The raw metadata XML")
            .def_property_readonly("slides", &mcd::MCDFileMetadata::getSlides, "All slides defined in the metadata")
            .def_property_readonly("panoramas", &mcd::MCDFileMetadata::getPanoramas, "All panoramas defined in the metadata")
            .def_property_readonly("regions", &mcd::MCDFileMetadata::getRegions, "All regions defined in the metadata")
            .def_property_readonly("acquisitions", &mcd::MCDFileMetadata::getAcquisitions, "All acquisitions defined in the metadata")
            .def_property_readonly("channels", &mcd::MCDFileMetadata::getChannels, "All channels defined in the metadata");

    py::class_<mcd::data::MetadataBase, std::shared_ptr<mcd::data::MetadataBase>>(m, "MetadataBase")
            .def_property_readonly("properties", &mcd::data::MetadataBase::getProperties, "Key-value map of all available properties");

    py::class_<mcd::data::Slide, std::shared_ptr<mcd::data::Slide>, mcd::data::MetadataBase>(m, "Slide")
            .def_property_readonly("panoramas", &mcd::data::Slide::getPanoramas, "A list of all panoramas of this slide");

    py::class_<mcd::data::Panorama, std::shared_ptr<mcd::data::Panorama>, mcd::data::MetadataBase>(m, "Panorama")
            .def_property_readonly("slide", &mcd::data::Panorama::getSlide, "The slide this panorama belongs to")
            .def_property_readonly("regions", &mcd::data::Panorama::getRegions, "A list of all regions of this panorama");

    py::class_<mcd::data::Region, std::shared_ptr<mcd::data::Region>, mcd::data::MetadataBase>(m, "Region")
            .def_property_readonly("panorama", &mcd::data::Region::getPanorama, "The panorama this region belongs to")
            .def_property_readonly("acquisitions", &mcd::data::Region::getAcquisitions, "A list of all acquisitions of this region");

    py::class_<mcd::data::Acquisition, std::shared_ptr<mcd::data::Acquisition>, mcd::data::MetadataBase>(m, "Acquisition")
            .def_property_readonly("region", &mcd::data::Acquisition::getRegion, "The region this acquisition belongs to")
            .def_property_readonly("channels", &mcd::data::Acquisition::getChannels, "A list of all channels of this region");

    py::class_<mcd::data::Channel, std::shared_ptr<mcd::data::Channel>, mcd::data::MetadataBase>(m, "Channel")
            .def_property_readonly("acquisition", &mcd::data::Channel::getAcquisition, "The acquisition this channel belongs to");

    py::class_<mcd::MCDFileAcquisitionData>(m, "MCDFileAcquisitionData")
#ifdef OMETIFF_SUPPORT_ENABLED
            .def("writeOMETIFF", &mcd::py::writeOMETIFFUncompressed, "Exports the binary data of this acquisition as a multipage OME-TIFF (float, 32-bit) file", py::arg("destPath"))
            .def("writeOMETIFFCompressed", &mcd::MCDFileAcquisitionData::writeOMETIFF, "Exports the binary data of this acquisition as a compressed multipage OME-TIFF (float, 32-bit) file", py::arg("destPath"), py::arg("compression"))
#endif //OMETIFF_SUPPORT_ENABLED
            .def_property_readonly("channelData", &mcd::MCDFileAcquisitionData::getChannelData, "A list of per-channel data recordings (same ordering as in Acquisition.channels is guaranteed)");

    py::class_<mcd::data::ChannelData, std::shared_ptr<mcd::data::ChannelData>>(m, "ChannelData")
            .def_property_readonly("channel", &mcd::data::ChannelData::getChannel, "The channel this data belongs to")
            .def_property_readonly("sizeX", &mcd::data::ChannelData::getSizeX, "The width of the channel image in pixels")
            .def_property_readonly("sizeY", &mcd::data::ChannelData::getSizeY, "The height of the channel image in pixels")
            .def_property_readonly("data", &mcd::data::ChannelData::getData, "Access to the in-memory data (in row-major order)");

}
