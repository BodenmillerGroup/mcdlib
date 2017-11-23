#ifndef MCD_MCDFILEMETADATA_H
#define MCD_MCDFILEMETADATA_H

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "MCDFileMalformedException.h"

#include "data/MetadataBase.h"
#include "data/Slide.h"
#include "data/Panorama.h"
#include "data/Region.h"
#include "data/Acquisition.h"
#include "data/Channel.h"

#include "pugixml/pugixml.hpp"

#define SLIDE_XPATH "/MCDSchema/Slide"
#define PANORAMA_XPATH "/MCDSchema/Panorama"
#define REGION_XPATH "/MCDSchema/AcquisitionROI"
#define ACQUISITION_XPATH "/MCDSchema/Acquisition"
#define CHANNEL_XPATH "/MCDSchema/AcquisitionChannel"

namespace mcd {

    class MCDFileMetadata {

    private:

        const std::string schemaXML;
        std::vector<std::shared_ptr<data::Slide>> slides;
        std::vector<std::shared_ptr<data::Panorama>> panoramas;
        std::vector<std::shared_ptr<data::Region>> regions;
        std::vector<std::shared_ptr<data::Acquisition>> acquisitions;
        std::vector<std::shared_ptr<data::Channel>> channels;

        explicit MCDFileMetadata(const std::string &schemaXML);

        template<class T>
        static std::shared_ptr<T> findByID(const std::vector<std::shared_ptr<T>> &vec, int32_t id);

        static std::map<std::string, std::string> parseNode(const pugi::xml_node &node);

    public:

        static MCDFileMetadata parseXML(const std::string &schemaXML);

        const std::string &getSchemaXML() const;

        const std::vector<std::shared_ptr<data::Slide>> &getSlides() const;

        const std::vector<std::shared_ptr<data::Panorama>> &getPanoramas() const;

        const std::vector<std::shared_ptr<data::Region>> &getRegions() const;

        const std::vector<std::shared_ptr<data::Acquisition>> &getAcquisitions() const;

        const std::vector<std::shared_ptr<data::Channel>> &getChannels() const;

    };

}

#endif //MCD_MCDFILEMETADATA_H
