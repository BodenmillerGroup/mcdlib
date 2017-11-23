#ifndef MCD_ACQUISITION_H
#define MCD_ACQUISITION_H

#include <algorithm>
#include <memory>
#include <vector>

#include "MetadataBase.h"
#include "Channel.h"

#define ACQUISITION_ID "ID"
#define ACQUISITION_MAX_X "MaxX"
#define ACQUISITION_MAX_Y "MaxY"
#define ACQUISITION_REGION_ID "AcquisitionROIID"
#define ACQUISITION_ORDER_NUMBER "OrderNumber"
#define ACQUISITION_DATA_START_OFFSET "DataStartOffset"
#define ACQUISITION_DATA_END_OFFSET "DataEndOffset"
#define ACQUISITION_BEFORE_ABLATION_IMAGE_START_OFFSET "BeforeAblationImageStartOffset"
#define ACQUISITION_BEFORE_ABLATION_IMAGE_END_OFFSET "BeforeAblationImageEndOffset"
#define ACQUISITION_AFTER_ABLATION_IMAGE_START_OFFSET "AfterAblationImageStartOffset"
#define ACQUISITION_AFTER_ABLATION_IMAGE_END_OFFSET "AfterAblationImageEndOffset"

namespace mcd {

    namespace data {

        class Region;

        class Acquisition : public MetadataBase {

        private:

            std::shared_ptr<Region> region;
            std::vector<std::shared_ptr<Channel>> sortedChannels;

        public:

            using MetadataBase::MetadataBase;

            bool isValid() const override;

            int32_t getID() const override;

            uint32_t getMaxX() const;

            uint32_t getMaxY() const;

            int32_t getRegionID() const;

            int32_t getOrderNumber() const;

            uint64_t getDataStartOffset() const;

            uint64_t getDataEndOffset() const;

            uint64_t getBeforeAblationImageStartOffset() const;

            uint64_t getBeforeAblationImageEndOffset() const;

            uint64_t getAfterAblationImageStartOffset() const;

            uint64_t getAfterAblationImageEndOffset() const;

            const std::shared_ptr<Region> &getRegion() const;

            void setRegion(const std::shared_ptr<Region> &region);

            void addChannel(const std::shared_ptr<Channel> &channel);

            const std::vector<std::shared_ptr<Channel>> &getChannels() const;

        };

    }

}

#endif //MCD_ACQUISITION_H
