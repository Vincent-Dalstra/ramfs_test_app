# ramfs_test_app

esp-idf project that runs the Unit-Tests in the [ramfs](https://github.com/Vincent-Dalstra/ramfs) component.

ramfs is included as a submodule in 'dev_managed_components/ramfs'. This means you can work on ramfs from within that folder, commit changes, and push to github (change the origin url first!).


## To run the tests:

The esp-idf is required, along with a an ESP32 development board to run the tests. An ESP32-S3 with at least 4MB of SPIRAM and 2MB of PSRAM is recommended.

First, ensure submodules are present:

`git submodule update --init --recursive`

Build and run tests, and monitor the output.

`idf.py -p /dev/ttyACM0 flash monitor`

## Interesting files & folders

- dev_managed_components/
  - see [dev_managed_components/README.txt]
- main/idf_component.yml
  - tells the component manager where to find 'ramfs'
  - further explained in [dev_managed_components/README.txt]
 
- pre-commit-config.yaml
  - Format code before committing it. (much easier than doing it yourself!).
  - You need to install it first (very simple): https://pre-commit.com
- /tools/ci/astyle-rules.yml
  - Used by the pre-commit hook.
  - copied from esp-idf. I personally like the format.

- CMakeLists.txt
  - Contains the magic spells and incantations to run the unit tests.
  - Also generates the dependency graph. Useful in large projects.

- Doxyfile
  - Setup to produce call-graphs (and called-by graphs) for each function.
  - Useful for debugging.
- Makefile
  - Shortcut to make and view the doxygen output.

- sdkconfig.defaults
  - Turns on PSRAM, if it's available.
  - Heap poisoning will sometimes catch certain bugs early.
  - Enable colored output from Unity
