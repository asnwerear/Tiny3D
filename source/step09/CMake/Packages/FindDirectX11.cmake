
include(FindPkgMacros)
findpkg_begin(DirectX11)

# Find the win10 SDK path.
if ("$ENV{WIN10_SDK_PATH}$ENV{WIN10_SDK_VERSION}" STREQUAL "" )
  get_filename_component(WIN10_SDK_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Microsoft SDKs\\Windows\\v10.0;InstallationFolder]" ABSOLUTE CACHE)
  get_filename_component(TEMP_WIN10_SDK_VERSION "[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Microsoft SDKs\\Windows\\v10.0;ProductVersion]" ABSOLUTE CACHE)
  get_filename_component(WIN10_SDK_VERSION ${TEMP_WIN10_SDK_VERSION} NAME)
elseif(TRUE)
  set (WIN10_SDK_PATH $ENV{WIN10_SDK_PATH})
  set (WIN10_SDK_VERSION $ENV{WIN10_SDK_VERSION})
endif ("$ENV{WIN10_SDK_PATH}$ENV{WIN10_SDK_VERSION}" STREQUAL "" )

# WIN10_SDK_PATH will be something like C:\Program Files (x86)\Windows Kits\10
# WIN10_SDK_VERSION will be something like 10.0.14393 or 10.0.14393.0; we need the
# one that matches the directory name.

if (IS_DIRECTORY "${WIN10_SDK_PATH}/Include/${WIN10_SDK_VERSION}.0")
  set(WIN10_SDK_VERSION "${WIN10_SDK_VERSION}.0")
endif (IS_DIRECTORY "${WIN10_SDK_PATH}/Include/${WIN10_SDK_VERSION}.0")


# Find the d3d12 and dxgi include path, it will typically look something like this.
# C:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\um\d3d11.h
# C:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\shared\dxgi1_4.h
find_path(DirectX11_INCLUDE_DIR_TEMP    # Set variable DirectX11_INCLUDE_DIR
          d3d11.h                # Find a path with d3d11.h
          HINTS "${WIN10_SDK_PATH}/Include/${WIN10_SDK_VERSION}/um"
          DOC "path to WIN10 SDK header files"
          HINTS
          )


find_path(DXGI_INCLUDE_DIR_TEMP    # Set variable DXGI_INCLUDE_DIR
          dxgi1_4.h           # Find a path with dxgi1_4.h
          HINTS "${WIN10_SDK_PATH}/Include/${WIN10_SDK_VERSION}/shared"
          DOC "path to WIN10 SDK header files"
          HINTS
          )
		  

if ("${MSVC_CXX_ARCHITECTURE_ID}" STREQUAL "x64" )
  find_library(DirectX11_LIBRARY_TEMP NAMES d3d11.lib
               HINTS ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION}/um/x64 )
elseif (CMAKE_GENERATOR MATCHES "Visual Studio.*ARM" OR "${MSVC_CXX_ARCHITECTURE_ID}" STREQUAL "ARM")
  find_library(DirectX11_LIBRARY_TEMP NAMES d3d11.lib
               HINTS ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION}/um/arm )
elseif (CMAKE_GENERATOR MATCHES "Visual Studio.*ARM64" OR "${MSVC_CXX_ARCHITECTURE_ID}" STREQUAL "ARM64")
  find_library(DirectX11_LIBRARY_TEMP NAMES d3d11.lib
               HINTS ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION}/um/arm64 )
elseif ("${MSVC_CXX_ARCHITECTURE_ID}" STREQUAL "Win32" )
  find_library(DirectX11_LIBRARY_TEMP NAMES d3d11.lib
               HINTS ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION}/um/x86 )
endif ("${MSVC_CXX_ARCHITECTURE_ID}" STREQUAL "x64" )


if ("${MSVC_CXX_ARCHITECTURE_ID}" STREQUAL "x64" )
  find_library(DXGI_LIBRARY_TEMP NAMES dxgi.lib
               HINTS ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION}/um/x64 )
elseif (CMAKE_GENERATOR MATCHES "Visual Studio.*ARM" OR "${MSVC_CXX_ARCHITECTURE_ID}" STREQUAL "ARM")
  find_library(DXGI_LIBRARY_TEMP NAMES dxgi.lib
               HINTS ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION}/um/arm )
elseif (CMAKE_GENERATOR MATCHES "Visual Studio.*ARM64" OR "${MSVC_CXX_ARCHITECTURE_ID}" STREQUAL "ARM64")
  find_library(DXGI_LIBRARY_TEMP NAMES dxgi.lib
               HINTS ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION}/um/arm64 )
elseif ("${MSVC_CXX_ARCHITECTURE_ID}" STREQUAL "Win32" )
  find_library(DXGI_LIBRARY_TEMP NAMES dxgi.lib
               HINTS ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION}/um/x86 )
endif ("${MSVC_CXX_ARCHITECTURE_ID}" STREQUAL "x64" )

set(DirectX11_LIBRARY ${DirectX11_LIBRARY_TEMP} ${DXGI_LIBRARY_TEMP})
set(DirectX11_INCLUDE_DIR ${DirectX11_INCLUDE_DIR_TEMP} ${DXGI_INCLUDE_DIR_TEMP})

findpkg_finish(DirectX11)


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set D3D11_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(D3D11  DEFAULT_MSG
                                  DirectX11_INCLUDE_DIR DirectX11_LIBRARY)

mark_as_advanced(DirectX11_INCLUDE_DIR DirectX11_LIBRARY)
