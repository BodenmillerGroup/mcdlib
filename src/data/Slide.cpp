#include "Slide.h"

using namespace mcd::data;

bool Slide::isValid() const {
    return hasProperty(SLIDE_ID);
}

int32_t Slide::getID() const {
    return std::stoi(getProperty(SLIDE_ID));
}

uint64_t Slide::getImageStartOffset() const {
    return std::stoull(getProperty(SLIDE_IMAGE_START_OFFSET));
}

uint64_t Slide::getImageEndOffset() const {
    return std::stoull(getProperty(SLIDE_IMAGE_END_OFFSET));
}

void Slide::addPanorama(const std::shared_ptr<Panorama> &panorama) {
    panoramas.push_back(panorama);
}

const std::vector<std::shared_ptr<Panorama>> &Slide::getPanoramas() const {
    return panoramas;
}
