cmake_minimum_required(VERSION 3.21)

project(Smartcarpentry VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

option(ENABLE_OPENCV "Enable OpenCV integration" OFF)

# Qt modules used by the current project.
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql Charts Multimedia PrintSupport Network SerialPort)

qt_standard_project_setup()

set(PROJECT_SOURCES
    aigenerationdashboard.cpp
    aiassistantservice.cpp
    airesponse.cpp
    bois.cpp
    connsql.cpp
    employee.cpp
    fabrication.cpp
    faceid.cpp
    generatedpiececard.cpp
    login.cpp
    main.cpp
    modele.cpp
    modelecostestimator.cpp
    modelpreviewwidget.cpp
    nestingengine.cpp
    nestingwidget.cpp
    panel.cpp
    phase.cpp
    piecerect.cpp
    piece.cpp
    smartcarpentry.cpp
    arduinomanager.cpp
    arduinohandler.cpp
    woodexpert.cpp
)

set(PROJECT_HEADERS
    aigenerationdashboard.h
    aiassistantservice.h
    airesponse.h
    bois.h
    connsql.h
    employee.h
    fabrication.h
    faceid.h
    generatedpiececard.h
    login.h
    modele.h
    modelecostestimator.h
    modelpreviewwidget.h
    nestingengine.h
    nestingwidget.h
    panel.h
    phase.h
    piecerect.h
    piece.h
    smartcarpentry.h
    arduinomanager.h
    arduinohandler.h
    woodexpert.h
)

set(PROJECT_FORMS
    faceid.ui
    login.ui
    smartcarpentry.ui
)

set(PROJECT_RESOURCES
    resources.qrc
)

qt_add_executable(Smartcarpentry
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_FORMS}
    ${PROJECT_RESOURCES}
)

target_link_libraries(Smartcarpentry PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Charts
    Qt6::Multimedia
    Qt6::PrintSupport
    Qt6::Network
    Qt6::SerialPort
)

if(ENABLE_OPENCV)
    set(OPENCV_ROOT "${CMAKE_SOURCE_DIR}/libraries/opencv/release")
    set(OPENCV_INCLUDE_DIR "${OPENCV_ROOT}/install/include")

    if(NOT EXISTS "${OPENCV_INCLUDE_DIR}")
        message(FATAL_ERROR "OpenCV include directory not found: ${OPENCV_INCLUDE_DIR}")
    endif()

    set(OpenCV_INCLUDE_DIRS
        "${OPENCV_INCLUDE_DIR}"
        "${OPENCV_INCLUDE_DIR}/opencv2"
    )

    set(OpenCV_LIBS
        "${OPENCV_ROOT}/lib/libopencv_highgui320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_objdetect320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_videoio320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_imgcodecs320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_imgproc320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_video320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_photo320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_ml320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_shape320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_superres320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_features2d320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_calib3d320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_stitching320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_videostab320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_flann320.dll.a"
        "${OPENCV_ROOT}/lib/libopencv_core320.dll.a"
    )

    target_include_directories(Smartcarpentry PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(Smartcarpentry PRIVATE
        "-Wl,--start-group"
        ${OpenCV_LIBS}
        "-Wl,--end-group"
    )
    target_compile_definitions(Smartcarpentry PRIVATE SC_WITH_OPENCV=1)

    message(STATUS "OpenCV enabled from: ${OPENCV_ROOT}")
    message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
endif()

if(WIN32)
    set_target_properties(Smartcarpentry PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

install(TARGETS Smartcarpentry
    RUNTIME DESTINATION bin
)
