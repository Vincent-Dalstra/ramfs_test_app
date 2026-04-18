# dev_managed_components

## Purpose

This directory is for external dependencies that are in development.

## Explanation

External dependencies are usually listed in idf_component.yml of the
the component(s) that need them, and are updated automatically.

However, this means we can't make changes to them - they will be
overwritten by the dependency manager. That's really inconvenient
if we want to make bug fixes to the dependency.

## Solution

This was done with esp-idf v5.4.3

(1) Ensure the project's CMakeLists.txt has the following line:

	list(APPEND EXTRA_COMPONENT_DIRS dev_managed_components/)

It must be AFTER the line with cmake_minimum_required(...),
but BEFORE the line with include($ENV{IDF_PATH}/tools/cmake/project.cmake)

(2) Place the component into this directory (as a git submodule, if possible).

(3) Add the dependency to 'main/idf_component.yml'*, with a slightly different format:

e.g. if the dependency would usually be:
```
  jkent/ramfs: '*'
    version: ~0.2.0
```
Replace it with the following:
```
  ramfs:
    override_path:the dependency
      ../dev_managed_components/ramfs
```

Steps (1) ensures the build system can find the component.

Step (3) tells the component manager to ignore any other idf_component.yml
file that lists the dependency.


*Technically this doesn't have to main, but it must be in
one-and-only-one component or there will be a conflict. So
main makes the most sense.

