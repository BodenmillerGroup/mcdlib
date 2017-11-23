#ifndef MCD_CHANNELDATA_H
#define MCD_CHANNELDATA_H

#include <memory>
#include <vector>

#include "Acquisition.h"
#include "Channel.h"

namespace mcd {

    namespace data {

        class ChannelData {

        private:

            const std::shared_ptr<Channel> channel;
            const std::size_t sizeX;
            const std::size_t sizeY;
            std::vector<float> data;

        public:

            explicit ChannelData(const std::shared_ptr<Channel> &channel);

            const std::shared_ptr<Channel> &getChannel() const;

            std::size_t getSizeX() const;

            std::size_t getSizeY() const;

            float getPixel(uint32_t x, uint32_t y) const;

            void setPixel(uint32_t x, uint32_t y, float val);

            const std::vector<float> &getData() const;

        };

    }

}

#endif //MCD_CHANNELDATA_H
