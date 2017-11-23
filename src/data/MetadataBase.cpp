#include "MetadataBase.h"

using namespace mcd::data;

MetadataBase::MetadataBase(std::map<std::string, std::string> properties)
        : properties(std::move(properties)) {
}

bool MetadataBase::hasProperty(const std::string &key) const {
    return properties.count(key) > 0;
}

const std::string &MetadataBase::getProperty(const std::string &key) const {
    return properties.at(key);
}

const std::string &MetadataBase::getPropertyOrDefault(const std::string &key, const std::string &defaultValue) const {
    if (hasProperty(key)) {
        return getProperty(key);
    }
    return defaultValue;
}

const std::map<std::string, std::string> &MetadataBase::getProperties() const {
    return properties;
}
