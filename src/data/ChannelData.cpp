#include "ChannelData.h"

using namespace mcd::data;

ChannelData::ChannelData(const std::shared_ptr<Channel> &channel)
        : channel(channel), sizeX(channel->getAcquisition()->getMaxX()), sizeY(channel->getAcquisition()->getMaxY()),
          data(channel->getAcquisition()->getMaxX() * channel->getAcquisition()->getMaxY()) {
}

const std::shared_ptr<Channel> &ChannelData::getChannel() const {
    return channel;
}

std::size_t ChannelData::getSizeX() const {
    return sizeX;
}

std::size_t ChannelData::getSizeY() const {
    return sizeY;
}

float ChannelData::getPixel(uint32_t x, uint32_t y) const {
    return data[y * sizeX + x];
}

void ChannelData::setPixel(uint32_t x, uint32_t y, float val) {
    data[y * sizeX + x] = val;
}

const std::vector<float> &ChannelData::getData() const {
    return data;
}
