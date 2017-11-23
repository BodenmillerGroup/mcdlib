#include "Panorama.h"

using namespace mcd::data;

bool Panorama::isValid() const {
    return hasProperty(PANORAMA_SLIDE_ID);
}

int32_t Panorama::getID() const {
    return std::stoi(getProperty(PANORAMA_ID));
}

int32_t Panorama::getSlideID() const {
    return std::stoi(getProperty(PANORAMA_SLIDE_ID));
}

uint64_t Panorama::getImageStartOffset() const {
    return std::stoull(getProperty(PANORAMA_IMAGE_START_OFFSET));
}

uint64_t Panorama::getImageEndOffset() const {
    return std::stoull(getProperty(PANORAMA_IMAGE_END_OFFSET));
}

const std::shared_ptr<Slide> &Panorama::getSlide() const {
    return slide;
}

void Panorama::setSlide(const std::shared_ptr<Slide> &slide) {
    this->slide = slide;
}

void Panorama::addRegion(const std::shared_ptr<Region> &region) {
    regions.push_back(region);
}

const std::vector<std::shared_ptr<Region>> &Panorama::getRegions() const {
    return regions;
}
