#include "Acquisition.h"

using namespace mcd::data;

bool Acquisition::isValid() const {
    return hasProperty(ACQUISITION_REGION_ID) && hasProperty(ACQUISITION_ORDER_NUMBER);
}

int32_t Acquisition::getID() const {
    return std::stoi(getProperty(ACQUISITION_ID));
}

uint32_t Acquisition::getMaxX() const {
    return (uint32_t) std::stoul(getProperty(ACQUISITION_MAX_X));
}

uint32_t Acquisition::getMaxY() const {
    return (uint32_t) std::stoul(getProperty(ACQUISITION_MAX_Y));
}

int32_t Acquisition::getRegionID() const {
    return std::stoi(getProperty(ACQUISITION_REGION_ID));
}

int32_t Acquisition::getOrderNumber() const {
    return std::stoi(getProperty(ACQUISITION_ORDER_NUMBER));
}

uint64_t Acquisition::getDataStartOffset() const {
    return std::stoull(getProperty(ACQUISITION_DATA_START_OFFSET));
}

uint64_t Acquisition::getDataEndOffset() const {
    return std::stoull(getProperty(ACQUISITION_DATA_END_OFFSET));
}

uint64_t Acquisition::getBeforeAblationImageStartOffset() const {
    return std::stoull(getProperty(ACQUISITION_BEFORE_ABLATION_IMAGE_START_OFFSET));
}

uint64_t Acquisition::getBeforeAblationImageEndOffset() const {
    return std::stoull(getProperty(ACQUISITION_BEFORE_ABLATION_IMAGE_END_OFFSET));
}

uint64_t Acquisition::getAfterAblationImageStartOffset() const {
    return std::stoull(getProperty(ACQUISITION_AFTER_ABLATION_IMAGE_START_OFFSET));
}

uint64_t Acquisition::getAfterAblationImageEndOffset() const {
    return std::stoull(getProperty(ACQUISITION_AFTER_ABLATION_IMAGE_END_OFFSET));
}

const std::shared_ptr<Region> &Acquisition::getRegion() const {
    return region;
}

void Acquisition::setRegion(const std::shared_ptr<Region> &region) {
    this->region = region;
}

void Acquisition::addChannel(const std::shared_ptr<Channel> &channel) {
    auto it = std::lower_bound(sortedChannels.begin(), sortedChannels.end(), channel,
                               [](const std::shared_ptr<Channel> &a, const std::shared_ptr<Channel> &b) {
                                   return a->getOrderNumber() < b->getOrderNumber();
                               });
    sortedChannels.insert(it, channel);
}

const std::vector<std::shared_ptr<Channel>> &Acquisition::getChannels() const {
    return sortedChannels;
}
