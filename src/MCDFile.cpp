#include "MCDFile.h"

using namespace mcd;

std::string MCDFile::toUTF8(const std::vector<char> &vec) {
    std::string s(vec.size() / 2, ' ');
    for (std::size_t i = 0; i < s.size(); ++i) {
        s[i] = vec[2 * i];
    }
    return std::move(s);
}

std::vector<char> MCDFile::toUTF16(const std::string &s) {
    std::vector<char> vec;
    vec.reserve(2 * s.length());
    for (const char &c : s) {
        vec.push_back(c);
        vec.push_back(0x00);
    }
    return std::move(vec);
}

template<class T>
bool MCDFile::searchVectorBackwards(const std::vector<T> &vec, const std::vector<T> &pattern, std::size_t *foundPos) {
    std::size_t patternPos = 0;
    std::size_t currentOffset = pattern.size();
    while (currentOffset <= vec.size()) {
        const std::size_t currentPos = vec.size() - currentOffset;
        if (vec[currentPos + patternPos] == pattern[patternPos]) {
            patternPos += 1;
            if (patternPos == pattern.size()) {
                *foundPos = currentPos;
                return true;
            }
        } else {
            patternPos = 0;
            currentOffset++;
        }
    }
    return false;
}

bool MCDFile::searchFileBackwards(std::ifstream &file, const std::string &pattern, std::streamoff *foundPos,
                                  std::streamoff startPos) {
    // convert pattern
    const std::vector<char> patternVector = toUTF16(pattern);
    // prepare buffer
    std::vector<char> searchBuffer(SEARCH_BUFFER_SIZE + patternVector.size());
    char *const cachePtr = searchBuffer.data() + SEARCH_BUFFER_SIZE;
    // determine file size
    file.seekg(0, std::ios_base::end);
    const std::streamoff fileSize = file.tellg();
    // initialize search position
    std::streamoff chunkEndPos = startPos;
    if (startPos <= 0 || startPos >= fileSize) {
        chunkEndPos = fileSize;
    }
    // search file
    while (chunkEndPos >= SEARCH_BUFFER_SIZE) {
        // read chunk
        const std::streamoff chunkStartPos = chunkEndPos - SEARCH_BUFFER_SIZE;
        file.seekg(chunkStartPos, std::ios_base::beg);
        file.read(searchBuffer.data(), SEARCH_BUFFER_SIZE);
        // find in chunk
        std::size_t relativeFoundPos;
        if (MCDFile::searchVectorBackwards(searchBuffer, patternVector, &relativeFoundPos)) {
            *foundPos = chunkStartPos + relativeFoundPos;
            return true;
        }
        // update buffer
        std::copy(searchBuffer.data(), searchBuffer.data() + patternVector.size(), cachePtr);
        chunkEndPos = chunkStartPos;
    }
    // search remainder
    if (chunkEndPos > 0) {
        // update buffer
        char *const remainderCachePtr = searchBuffer.data() + chunkEndPos;
        std::copy(cachePtr, cachePtr + patternVector.size(), remainderCachePtr);
        searchBuffer.resize(chunkEndPos + patternVector.size());
        // read remainder
        file.seekg(0, std::ios_base::beg);
        file.read(searchBuffer.data(), chunkEndPos);
        // find in remainder
        std::size_t relativeFoundPos;
        if (MCDFile::searchVectorBackwards(searchBuffer, patternVector, &relativeFoundPos)) {
            *foundPos = relativeFoundPos;
            return true;
        }
    }
    return false;
}

void
MCDFile::copyFile(std::ifstream &file, std::streamoff startPos, std::streamoff endPos, const std::string &destPath) {
    std::ofstream outFile(destPath, std::ios_base::binary);
    if (!outFile) {
        throw MCDFileIOException("Could not open file " + destPath);
    }
    file.seekg(startPos, std::ios_base::beg);
    char copyBuffer[COPY_BUFFER_SIZE];
    while ((std::streamoff) file.tellg() + COPY_BUFFER_SIZE <= endPos) {
        file.read(copyBuffer, COPY_BUFFER_SIZE);
        outFile.write(copyBuffer, COPY_BUFFER_SIZE);
    }
    auto remaining = endPos - file.tellg();
    if (remaining > 0) {
        file.read(copyBuffer, remaining);
        outFile.write(copyBuffer, remaining);
    }
}

MCDFile::MCDFile(const std::string &path) : path(path) {
}

MCDFileMetadata MCDFile::readMetadata() const {
    // open file
    std::ifstream file(path, std::ios_base::binary);
    if (!file) {
        throw MCDFileIOException("Could not open file " + path);
    }
    // find end tag
    std::streamoff endPos;
    bool endFound = searchFileBackwards(file, MCDSCHEMA_END, &endPos);
    if (!endFound) {
        throw MCDFileMalformedException("Could not find end tag " MCDSCHEMA_END);
    }
    // find start tag
    std::streamoff startPos;
    bool beginFound = searchFileBackwards(file, MCDSCHEMA_START, &startPos, endPos);
    if (!beginFound) {
        throw MCDFileMalformedException("Could not find start tag " MCDSCHEMA_START " ahead of end tag " MCDSCHEMA_END);
    }
    // read xml
    endPos += toUTF16(MCDSCHEMA_END).size();
    std::vector<char> buffer((std::size_t) endPos - startPos);
    file.seekg(startPos, std::ios_base::beg);
    file.read(buffer.data(), buffer.size());
    // parse xml
    return MCDFileMetadata::parseXML(toUTF8(buffer));
}

void MCDFile::saveSlideImage(const std::shared_ptr<data::Slide> &slide, const std::string &destPath) const {
    std::ifstream file(path, std::ios_base::binary);
    if (!file) {
        throw MCDFileIOException("Could not open file " + path);
    }
    const uint64_t startPos = slide->getImageStartOffset() + IMAGE_START_OFFSET;
    copyFile(file, startPos, slide->getImageEndOffset(), destPath);
}

void MCDFile::savePanoramaImage(const std::shared_ptr<data::Panorama> &panorama, const std::string &destPath) const {
    std::ifstream file(path, std::ios_base::binary);
    if (!file) {
        throw MCDFileIOException("Could not open file " + path);
    }
    const uint64_t startPos = panorama->getImageStartOffset() + IMAGE_START_OFFSET;
    copyFile(file, startPos, panorama->getImageEndOffset(), destPath);
}

void MCDFile::saveAcquisitionImage(const std::shared_ptr<data::Acquisition> &acquisition, const std::string &destPath,
                                   const MCDFile::AcquisitionImageType &type) const {
    std::ifstream file(path, std::ios_base::binary);
    if (!file) {
        throw MCDFileIOException("Could not open file " + path);
    }
    uint64_t startPos, endPos;
    switch (type) {
        case AcquisitionImageType::BEFORE:
            startPos = acquisition->getBeforeAblationImageStartOffset() + IMAGE_START_OFFSET;
            endPos = acquisition->getBeforeAblationImageEndOffset();
            break;
        case AcquisitionImageType::AFTER:
            startPos = acquisition->getAfterAblationImageStartOffset() + IMAGE_START_OFFSET;
            endPos = acquisition->getAfterAblationImageEndOffset();
            break;
    }
    copyFile(file, startPos, endPos, destPath);
}

MCDFileAcquisitionData MCDFile::readAcquisitionData(const std::shared_ptr<data::Acquisition> &acquisition) const {
    return MCDFileAcquisitionData::loadFromMCD(path, acquisition);
}
