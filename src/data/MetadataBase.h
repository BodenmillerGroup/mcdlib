#ifndef MCD_METADATABASE_H
#define MCD_METADATABASE_H

#include <map>
#include <string>

namespace mcd {

    namespace data {

        class MetadataBase {

        protected:

            std::map<std::string, std::string> properties;

        public:

            explicit MetadataBase(std::map<std::string, std::string> properties);

            virtual bool isValid() const = 0;

            virtual int32_t getID() const = 0;

            bool hasProperty(const std::string &key) const;

            const std::string &getProperty(const std::string &key) const;

            const std::string &getPropertyOrDefault(const std::string &key, const std::string &defaultValue) const;

            const std::map<std::string, std::string> &getProperties() const;

        };

    }

}

#endif //MCD_METADATABASE_H
