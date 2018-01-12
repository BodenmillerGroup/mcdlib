# mcdlib

MCD file parser library

## Features
* Full metadata access
* Slide image extraction
* Panorama image extraction
* Acquisition image extraction
* Binary data access (in-memory)
* OME-TIFF export (optional compression)
* Unit test environment
* Python 3 bindings

## Prerequisites

* **pugixml** <br />
http://www.pugixml.org <br />
Bundled with the source (version 1.8) <br />
Header-only library, no additional setup required

* **OME Files** (optional, for OME-TIFF support) <br />
http://www.openmicroscopy.org/site/products/ome-files-cpp <br />
Required components: OME Common, OME Model, OME Files <br />
Build and install with ``cmake & make & sudo make install`` <br />
Tested with OME Common 5.4.2, OME Model 5.5.6 and OME Files 0.4.0

* **Google Test** (optional, for unit test support) <br />
https://github.com/google/googletest <br />
Configured as a Git submodule, no additional setup required

* **pybind11** (optional, for Python support) <br />
https://github.com/pybind/pybind11 <br />
Configured as a Git submodule, no additional setup required

## Installation

```bash
git clone --recursive https://github.com/BodenmillerGroup/mcdlib.git
mkdir mcdlib/cmake-build-release
cd mcdlib/cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
sudo ldconfig
```

Note: if you experience troubles while building the library (more specifically, when linking mcdlib to the OME libraries), this likely happens because of outdated/broken CMake files provided by the Open Microscopy Environment (OME) team.

## Usage

In C++11, just include the headers:

```C++
#include <string>

#define OMETIFF_SUPPORT_ENABLED
#include <mcdlib/MCDFile.h>

int main(int argc, char *argv[]) {
    mcd::MCDFile mcdFile("/path/to/file");

    // full metadata access
    auto meta = mcdFile.readMetadata();
    auto firstSlide = meta.getSlides()[0];
    const std::string schemaXML = meta.getSchemaXML();
    auto swVersion = firstSlide->getProperty("SwVersion");

    // panorama image extraction
    auto firstPanorama = firstSlide->getPanoramas()[0];
    mcdFile.savePanoramaImage(firstPanorama, "/path/to/file");

    // slide image extraction
    auto firstRegion = firstPanorama->getRegions()[0];
    mcdFile.saveSlideImage(firstSlide, "/path/to/file");

    // before/after acquisition image extraction
    auto firstAcquisition = firstRegion->getAcquisitions()[0];
    mcdFile.saveAcquisitionImage(firstAcquisition, "/path/to/file", mcd::MCDFile::AcquisitionImageType::BEFORE);

    // acquisition data export to OME-TIFF
    std::string compression = "LZW";
    auto data = mcdFile.readAcquisitionData(firstAcquisition);
    data.writeOMETIFF("/path/to/file", &compression);

    // in-memory acquisition data access
    auto firstChannel = firstAcquisition->getChannels()[0];
    auto firstChannelData = data.findChannelData(firstChannel);
    auto firstChannelDataRaw = firstChannelData->getData();

    return 0;
}
```

In Python 3, just import the module:

```python3
import mcdpy
help(mcdpy.MCDFile)
```

## License

Copyright 2017 Bodenmiller Lab

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this project except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
