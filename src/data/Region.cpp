#include "Region.h"

using namespace mcd::data;

bool Region::isValid() const {
    return hasProperty(REGION_PANORAMA_ID);
}

int32_t Region::getID() const {
    return std::stoi(getProperty(REGION_ID));
}

int32_t Region::getPanoramaID() const {
    return std::stoi(getProperty(REGION_PANORAMA_ID));
}

const std::shared_ptr<Panorama> &Region::getPanorama() const {
    return panorama;
}

void Region::setPanorama(const std::shared_ptr<Panorama> &panorama) {
    this->panorama = panorama;
}

void Region::addAcquisition(const std::shared_ptr<Acquisition> &acquisition) {
    auto it = std::lower_bound(sortedAcquisitions.begin(), sortedAcquisitions.end(), acquisition,
                               [](const std::shared_ptr<Acquisition> &a, const std::shared_ptr<Acquisition> &b) {
                                   return a->getOrderNumber() < b->getOrderNumber();
                               });
    sortedAcquisitions.insert(it, acquisition);
}

const std::vector<std::shared_ptr<Acquisition>> &Region::getAcquisitions() const {
    return sortedAcquisitions;
}
