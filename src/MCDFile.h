#ifndef MCD_MCDFILE_H
#define MCD_MCDFILE_H

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "MCDFileMetadata.h"
#include "MCDFileAcquisitionData.h"
#include "MCDFileMalformedException.h"
#include "MCDFileIOException.h"

#define IMAGE_START_OFFSET 161
#define COPY_BUFFER_SIZE 8192
#define SEARCH_BUFFER_SIZE 8192
#define MCDSCHEMA_START "<MCDSchema"
#define MCDSCHEMA_END "</MCDSchema>"

namespace mcd {

    class MCDFile {

    private:

        const std::string path;

        static std::vector<char> toUTF16(const std::string &s);

        static std::string toUTF8(const std::vector<char> &vec);

        template<class T>
        static bool
        searchVectorBackwards(const std::vector<T> &vec, const std::vector<T> &pattern, std::size_t *foundPos);

        static bool searchFileBackwards(std::ifstream &file, const std::string &pattern, std::streamoff *foundPos,
                                        std::streamoff startPos = 0);

        static void
        copyFile(std::ifstream &file, std::streamoff startPos, std::streamoff endPos, const std::string &destPath);

    public:

        enum class AcquisitionImageType {
            BEFORE,
            AFTER
        };

        explicit MCDFile(const std::string &path);

        MCDFileMetadata readMetadata() const;

        void saveSlideImage(const std::shared_ptr<data::Slide> &slide, const std::string &destPath) const;

        void savePanoramaImage(const std::shared_ptr<data::Panorama> &panorama, const std::string &destPath) const;

        void saveAcquisitionImage(const std::shared_ptr<data::Acquisition> &acquisition, const std::string &destPath,
                                  const AcquisitionImageType &type) const;

        MCDFileAcquisitionData readAcquisitionData(const std::shared_ptr<data::Acquisition> &acquisition) const;

    };

}

#endif //MCD_MCDFILE_H
