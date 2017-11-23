#include "MCDFileMetadata.h"

using namespace mcd;

MCDFileMetadata::MCDFileMetadata(const std::string &schemaXML)
        : schemaXML(schemaXML) {
}

std::map<std::string, std::string> MCDFileMetadata::parseNode(const pugi::xml_node &node) {
    std::map<std::string, std::string> props;
    for (const auto &childNode : node.children()) {
        props[childNode.name()] = childNode.text().get();
    }
    return std::move(props);
}

template<class T>
std::shared_ptr<T> MCDFileMetadata::findByID(const std::vector<std::shared_ptr<T>> &vec, int32_t id) {
    for (const std::shared_ptr<T> &x : vec) {
        if (x->getID() == id) {
            return x;
        }
    }
    return nullptr;
}

MCDFileMetadata MCDFileMetadata::parseXML(const std::string &schemaXML) {
    pugi::xml_document schemaDoc;
    pugi::xml_parse_result schemaParseResult = schemaDoc.load(schemaXML.c_str());
    if (!schemaParseResult) {
        throw MCDFileMalformedException("Could not parse XML: " + std::string(schemaParseResult.description()));
    }
    // Slides
    MCDFileMetadata metaData(schemaXML);
    for (const pugi::xpath_node &node : schemaDoc.select_nodes(SLIDE_XPATH)) {
        std::shared_ptr<data::Slide> slide = std::make_shared<data::Slide>(parseNode(node.node()));
        if (slide->isValid()) {
            metaData.slides.push_back(std::move(slide));
        }
    }
    // Panoramas
    for (const pugi::xpath_node &node : schemaDoc.select_nodes(PANORAMA_XPATH)) {
        std::shared_ptr<data::Panorama> panorama = std::make_shared<data::Panorama>(parseNode(node.node()));
        if (panorama->isValid()) {
            std::shared_ptr<data::Slide> panoramaSlide = metaData.findByID(metaData.slides, panorama->getSlideID());
            if (panoramaSlide != nullptr) {
                panorama->setSlide(panoramaSlide);
                panoramaSlide->addPanorama(panorama);
            }
            metaData.panoramas.push_back(std::move(panorama));
        }
    }
    // Regions
    for (const pugi::xpath_node &node : schemaDoc.select_nodes(REGION_XPATH)) {
        std::shared_ptr<data::Region> region = std::make_shared<data::Region>(parseNode(node.node()));
        if (region->isValid()) {
            std::shared_ptr<data::Panorama> regionPanorama = metaData.findByID(metaData.panoramas,
                                                                               region->getPanoramaID());
            if (regionPanorama != nullptr) {
                region->setPanorama(regionPanorama);
                regionPanorama->addRegion(region);
            }
            //TODO initialize points
            metaData.regions.push_back(std::move(region));
        }
    }
    // Acquisitions
    for (const pugi::xpath_node &node : schemaDoc.select_nodes(ACQUISITION_XPATH)) {
        std::shared_ptr<data::Acquisition> acquisition = std::make_shared<data::Acquisition>(parseNode(node.node()));
        if (acquisition->isValid()) {
            std::shared_ptr<data::Region> acquisitionRegion = metaData.findByID(metaData.regions,
                                                                                acquisition->getRegionID());
            if (acquisitionRegion != nullptr) {
                acquisition->setRegion(acquisitionRegion);
                acquisitionRegion->addAcquisition(acquisition);
            }
            metaData.acquisitions.push_back(std::move(acquisition));
        }
    }
    // Channels
    for (const pugi::xpath_node &node : schemaDoc.select_nodes(CHANNEL_XPATH)) {
        std::shared_ptr<data::Channel> channel = std::make_shared<data::Channel>(parseNode(node.node()));
        if (channel->isValid()) {
            std::shared_ptr<data::Acquisition> channelAcquisition = metaData.findByID(metaData.acquisitions,
                                                                                      channel->getAcquisitionID());
            if (channelAcquisition != nullptr) {
                channel->setAcquisition(channelAcquisition);
                channelAcquisition->addChannel(channel);
            }
            metaData.channels.push_back(std::move(channel));
        }
    }
    return std::move(metaData);
}

const std::string &MCDFileMetadata::getSchemaXML() const {
    return schemaXML;
}

const std::vector<std::shared_ptr<data::Slide>> &MCDFileMetadata::getSlides() const {
    return slides;
}

const std::vector<std::shared_ptr<data::Panorama>> &MCDFileMetadata::getPanoramas() const {
    return panoramas;
}

const std::vector<std::shared_ptr<data::Region>> &MCDFileMetadata::getRegions() const {
    return regions;
}

const std::vector<std::shared_ptr<data::Acquisition>> &MCDFileMetadata::getAcquisitions() const {
    return acquisitions;
}

const std::vector<std::shared_ptr<data::Channel>> &MCDFileMetadata::getChannels() const {
    return channels;
}