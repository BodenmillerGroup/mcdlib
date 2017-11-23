#ifndef MCD_SLIDE_H
#define MCD_SLIDE_H

#include <memory>
#include <vector>

#include "MetadataBase.h"
#include "Panorama.h"

#define SLIDE_ID "ID"
#define SLIDE_IMAGE_START_OFFSET "ImageStartOffset"
#define SLIDE_IMAGE_END_OFFSET "ImageEndOffset"

namespace mcd {

    namespace data {

        class Slide : public MetadataBase {

        private:

            std::vector<std::shared_ptr<Panorama>> panoramas;

        public:

            using MetadataBase::MetadataBase;

            bool isValid() const override;

            int32_t getID() const override;

            uint64_t getImageStartOffset() const;

            uint64_t getImageEndOffset() const;

            void addPanorama(const std::shared_ptr<Panorama> &panorama);

            const std::vector<std::shared_ptr<Panorama>> &getPanoramas() const;

        };

    }

}

#endif //MCD_SLIDE_H
