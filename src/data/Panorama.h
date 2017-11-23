#ifndef MCD_PANORAMA_H
#define MCD_PANORAMA_H

#include <memory>
#include <vector>

#include "MetadataBase.h"
#include "Region.h"

#define PANORAMA_ID "ID"
#define PANORAMA_SLIDE_ID "SlideID"
#define PANORAMA_IMAGE_START_OFFSET "ImageStartOffset"
#define PANORAMA_IMAGE_END_OFFSET "ImageEndOffset"

namespace mcd {

    namespace data {

        class Slide;

        class Panorama : public MetadataBase {

        private:

            std::shared_ptr<Slide> slide;
            std::vector<std::shared_ptr<Region>> regions;

        public:

            using MetadataBase::MetadataBase;

            bool isValid() const override;

            int32_t getID() const override;

            int32_t getSlideID() const;

            uint64_t getImageStartOffset() const;

            uint64_t getImageEndOffset() const;

            const std::shared_ptr<Slide> &getSlide() const;

            void setSlide(const std::shared_ptr<Slide> &slide);

            void addRegion(const std::shared_ptr<Region> &region);

            const std::vector<std::shared_ptr<Region>> &getRegions() const;

        };

    }

}

#endif //MCD_PANORAMA_H
