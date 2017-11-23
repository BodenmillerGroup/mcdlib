#include "Channel.h"

using namespace mcd::data;

bool Channel::isValid() const {
    return hasProperty(CHANNEL_ACQUISITION_ID) && hasProperty(CHANNEL_ORDER_NUMBER);
}

int32_t Channel::getID() const {
    return std::stoi(getProperty(CHANNEL_ID));
}

int32_t Channel::getOrderNumber() const {
    return std::stoi(getProperty(CHANNEL_ORDER_NUMBER));
}

int32_t Channel::getAcquisitionID() const {
    return std::stoi(getProperty(CHANNEL_ACQUISITION_ID));
}

const std::shared_ptr<Acquisition> &Channel::getAcquisition() const {
    return acquisition;
}

void Channel::setAcquisition(const std::shared_ptr<Acquisition> &acquisition) {
    this->acquisition = acquisition;
}
