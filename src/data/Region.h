#ifndef MCD_REGION_H
#define MCD_REGION_H

#include <memory>
#include <vector>

#include "MetadataBase.h"
#include "Acquisition.h"

#define REGION_ID "ID"
#define REGION_PANORAMA_ID "PanoramaID"

namespace mcd {

    namespace data {

        class Panorama;

        class Region : public MetadataBase {

        private:

            std::shared_ptr<Panorama> panorama;
            std::vector<std::shared_ptr<Acquisition>> sortedAcquisitions;

        public:

            using MetadataBase::MetadataBase;

            bool isValid() const override;

            int32_t getID() const override;

            int32_t getPanoramaID() const;

            const std::shared_ptr<Panorama> &getPanorama() const;

            void setPanorama(const std::shared_ptr<Panorama> &panorama);

            void addAcquisition(const std::shared_ptr<Acquisition> &acquisition);

            const std::vector<std::shared_ptr<Acquisition>> &getAcquisitions() const;

        };

    }

}

#endif //MCD_REGION_H
