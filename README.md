# mcdlib

MCD file parser library

## Features
* Full metadata access
* Slide image extraction
* Panorama image extraction
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

This is a C++11 example of the full functionality of the library:

```C++
#include <mcdlib/MCDFile.h>
```

For interactive/scripting usage, this is a Python 3 example:

```python3
import mcdpy
mcd = mcdpy.MCDFile('/path/to/file')

# full metadata access
meta = mcd.readMetadata()
first_slide = meta.slides[0]
print(first_slide.properties['SwVersion'])
print(meta.schemaXML)

# panorama image extraction
first_panorama = first_slide.panoramas[0]
mcd.savePanoramaImage(first_slide, '/path/to/file')

# slide image extraction
first_region = first_panorama.regions[0]
mcd.saveSlideImage(first_region, '/path/to/file')

# before/after acquisition image extraction
first_acquisition = first_region.acquisitions[0]
mcd.saveAcquisitionImage(first_acquisition, '/path/to/file', mcdpy.AcquisitionImageType.BEFORE)
mcd.saveAcquisitionImage(first_acquisition, '/path/to/file', mcdpy.AcquisitionImageType.AFTER)

# acquisition data export to OME-TIFF
data = mcd.readAcquisitionData(first_acquisition)
data.writeOMETIFF('/path/to/file.ome.tiff')
data.writeOMETIFFCompressed('/path/to/file.ome.tiff', 'LZW')

# in-memory acquisition data access
first_channel = first_acquisition.channels[0]
channel_data = data.findChannelData(first_channel)
raw_channel_data = channel_data.data
```

At any time, a brief documentation is available using Python's built-in help functionality.

## License

Copyright 2017 Jonas Windhager

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this project except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
