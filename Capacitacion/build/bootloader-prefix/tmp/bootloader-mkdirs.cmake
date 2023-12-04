# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/ESP32/esp-idf/components/bootloader/subproject"
  "D:/CapacitacionESP32/Capacitacion/build/bootloader"
  "D:/CapacitacionESP32/Capacitacion/build/bootloader-prefix"
  "D:/CapacitacionESP32/Capacitacion/build/bootloader-prefix/tmp"
  "D:/CapacitacionESP32/Capacitacion/build/bootloader-prefix/src/bootloader-stamp"
  "D:/CapacitacionESP32/Capacitacion/build/bootloader-prefix/src"
  "D:/CapacitacionESP32/Capacitacion/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/CapacitacionESP32/Capacitacion/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/CapacitacionESP32/Capacitacion/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
