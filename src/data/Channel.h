#ifndef MCD_CHANNEL_H
#define MCD_CHANNEL_H

#include <memory>

#include "MetadataBase.h"

#define CHANNEL_ID "ID"
#define CHANNEL_ORDER_NUMBER "OrderNumber"
#define CHANNEL_ACQUISITION_ID "AcquisitionID"

namespace mcd {

    namespace data {

        class Acquisition;

        class Channel : public MetadataBase {

        private:

            std::shared_ptr<Acquisition> acquisition;

        public:

            using MetadataBase::MetadataBase;

            bool isValid() const override;

            int32_t getID() const override;

            int32_t getOrderNumber() const;

            int32_t getAcquisitionID() const;

            const std::shared_ptr<Acquisition> &getAcquisition() const;

            void setAcquisition(const std::shared_ptr<Acquisition> &acquisition);

        };

    }

}

#endif //MCD_CHANNEL_H
