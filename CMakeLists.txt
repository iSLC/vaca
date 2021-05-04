# Vaca - Visual Application Components Abstraction
# Copyright (c) 2005-2010 David Capello, Jie Zhang
#
# This file is distributed under the terms of the MIT license,
# please read LICENSE.txt for more information.

# The newest CMake version can be easily acquired on Windows.
cmake_minimum_required(VERSION 3.17)

project(vaca)

# The newest C++ version can be easily acquired on Windows.
# Unlike linux, you just grab an updated compiler.
# Either through MinGW (msys2) or the latest MSVC.
set(CMAKE_CXX_STANDARD 17)

# Is this library being built directly or added as a subdirectory?
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(VACA_IS_MASTER TRUE)
else()
    set(VACA_IS_MASTER FALSE)
endif()

# Whether to build a shared or static library
option(VACA_BUILD_SHARED "Build shared libraries" OFF)
# Whether to build examples using themes
option(VACA_BUILD_THEMES "Build examples using WinXP themes" ON)

# Do not build tests and examples if added as a subdirectory
if(VACA_IS_MASTER)
    option(VACA_BUILD_EXAMPLES "Build examples" ON)
    option(VACA_BUILD_TESTS "Build tests" ON)
else(VACA_IS_MASTER)
    option(VACA_BUILD_EXAMPLES "Build examples" OFF)
    option(VACA_BUILD_TESTS "Build tests" OFF)
endif(VACA_IS_MASTER)

# Allow the user to select the desired windows version
set(VACA_WINNT "WINXP" CACHE STRING "Desired Windows version. Windows 2000, XP, Vista, 7, 8, 8.1, 10")
set_property(CACHE VACA_WINNT PROPERTY STRINGS "WIN2K" "WINXP" "VISTA" "WIN7" "WIN8" "WINBLUE" "WIN10")
# Is Windows 2000?
if(VACA_WINNT STREQUAL "WIN2K")
    set(VACA_WIN32_WINNT "0x0500")
# Is Windows XP?
elseif(VACA_WINNT STREQUAL "WINXP")
    set(VACA_WIN32_WINNT "0x0501")
# Is Windows Vista?
elseif(VACA_WINNT STREQUAL "VISTA")
    set(VACA_WIN32_WINNT "0x0600")
# Is Windows 7?
elseif(VACA_WINNT STREQUAL "WIN7")
    set(VACA_WIN32_WINNT "0x0601")
# Is Windows 8?
elseif(VACA_WINNT STREQUAL "WIN8")
    set(VACA_WIN32_WINNT "0x0602")
# Is Windows 8.1?
elseif(VACA_WINNT STREQUAL "WINBLUE")
    set(VACA_WIN32_WINNT "0x0603")
# Is Windows 10?
elseif(VACA_WINNT STREQUAL "WIN10")
    set(VACA_WIN32_WINNT "0x0A00")
# Fallback to XP
else()
    set(VACA_WIN32_WINNT "0x0501")
    message(WARNING "Unknown windows version. Falling back to default: XP")
endif()

# Allow the user to select the desired internet explorer version
set(VACA_IE "IE60" CACHE STRING "Desired IE version. Internet Explorer 5.0, 5.01, 5.5, 6.0, 6.0 SP1, 6.0 SP2, 7.0, 8.0, 9.0, 10.0, 11.0")
set_property(CACHE VACA_IE PROPERTY STRINGS "IE50" "IE51" "IE55" "IE60" "IE61" "IE63" "IE70" "IE80" "IE90" "IE100" "IE110")
# Is Internet Explorer 5.0, 5.0a, 5.0b?
if(VACA_IE STREQUAL "IE50")
    set(VACA_WIN32_IE "0x0500")
# Is Internet Explorer 5.01?
elseif(VACA_IE STREQUAL "IE51")
    set(VACA_WIN32_IE "0x0501")
# Is Internet Explorer 5.5?
elseif(VACA_IE STREQUAL "IE55")
    set(VACA_WIN32_IE "0x0550")
# Is Internet Explorer 6.0?
elseif(VACA_IE STREQUAL "IE60")
    set(VACA_WIN32_IE "0x0600")
# Is Internet Explorer 6.0 SP1?
elseif(VACA_IE STREQUAL "IE61")
    set(VACA_WIN32_IE "0x0601")
# Is Internet Explorer 6.0 SP2?
elseif(VACA_IE STREQUAL "IE63")
    set(VACA_WIN32_IE "0x0603")
# Is Internet Explorer 7.0?
elseif(VACA_IE STREQUAL "IE70")
    set(VACA_WIN32_IE "0x0700")
# Is Internet Explorer 8.0?
elseif(VACA_IE STREQUAL "IE80")
    set(VACA_WIN32_IE "0x0800")
# Is Internet Explorer 9.0?
elseif(VACA_IE STREQUAL "IE90")
    set(VACA_WIN32_IE "0x0900")
# Is Internet Explorer 10.0 or 11.0?
elseif(VACA_IE STREQUAL "IE100" OR VACA_IE STREQUAL "IE110")
    set(VACA_WIN32_IE "0x0A00")
# Fallback to Internet Explorer 6.0
else()
    set(VACA_WIN32_IE "0x0600")
    message(WARNING "Unknown IE version. Falling back to default: Internet Explorer 6.0")
endif()

# Should we create a shared or static library?
if(VACA_BUILD_SHARED)
    add_library(vaca SHARED source/Wg.cpp)
    target_compile_definitions(vaca PUBLIC VACA_SHARED)
else(VACA_BUILD_SHARED)
    add_library(vaca STATIC source/Wg.cpp)
    target_compile_definitions(vaca PUBLIC VACA_STATIC)
endif(VACA_BUILD_SHARED)

# Library header files
FILE(GLOB VACA_PUBLIC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
FILE(GLOB VACA_PRIVATE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
# Add the headers to the library
target_sources(vaca PUBLIC ${VACA_PUBLIC_HEADERS})
target_sources(vaca PRIVATE ${VACA_PRIVATE_HEADERS})

# Library source files
target_sources(vaca PRIVATE
    source/Anchor.cpp
    source/AnchorLayout.cpp
    source/Application.cpp
    source/BandedDockArea.cpp
    source/BasicDockArea.cpp
    source/Bix.cpp
    source/BoxConstraint.cpp
    source/BoxLayout.cpp
    source/Brush.cpp
    source/Button.cpp
    source/ButtonBase.cpp
    source/CancelableEvent.cpp
    source/CheckBox.cpp
    source/ClientLayout.cpp
    source/Clipboard.cpp
    source/CloseEvent.cpp
    source/Color.cpp
    source/ColorDialog.cpp
    source/ComboBox.cpp
    source/Command.cpp
    source/CommandEvent.cpp
    source/CommonDialog.cpp
    source/Component.cpp
    source/ConditionVariable.cpp
    source/Constraint.cpp
    source/ConsumableEvent.cpp
    source/Cursor.cpp
    source/CustomButton.cpp
    source/CustomLabel.cpp
    source/Debug.cpp
    source/Dialog.cpp
    source/DockArea.cpp
    source/DockBar.cpp
    source/DockFrame.cpp
    source/DropFilesEvent.cpp
    source/Event.cpp
    source/Exception.cpp
    source/FileDialog.cpp
    source/FindFiles.cpp
    source/FindTextDialog.cpp
    source/FocusEvent.cpp
    source/Font.cpp
    source/FontDialog.cpp
    source/Frame.cpp
    source/Graphics.cpp
    source/GraphicsPath.cpp
    source/GroupBox.cpp
    source/HttpRequest.cpp
    source/Icon.cpp
    source/Image.cpp
    source/ImageList.cpp
    source/KeyEvent.cpp
    source/Keys.cpp
    source/Label.cpp
    source/Layout.cpp
    source/LayoutEvent.cpp
    source/LinkLabel.cpp
    source/ListBox.cpp
    source/ListColumn.cpp
    source/ListItem.cpp
    source/ListView.cpp
    source/Mdi.cpp
    source/Menu.cpp
    source/MenuItemEvent.cpp
    source/Message.cpp
    source/MouseEvent.cpp
    source/MsgBox.cpp
    source/Mutex.cpp
    source/PaintEvent.cpp
    source/Pen.cpp
    source/Point.cpp
    source/PreferredSizeEvent.cpp
    source/ProgressBar.cpp
    source/Property.cpp
    source/RadioButton.cpp
    source/ReBar.cpp
    source/Rect.cpp
    source/Referenceable.cpp
    source/Region.cpp
    source/ResizeEvent.cpp
    source/ResourceId.cpp
    source/RichEdit.cpp
    #source/Scintilla.cpp
    source/ScrollEvent.cpp
    source/ScrollInfo.cpp
    source/ScrollableWidget.cpp
    source/Separator.cpp
    source/SetCursorEvent.cpp
    source/Size.cpp
    source/Slider.cpp
    source/SpinButton.cpp
    source/Spinner.cpp
    source/SplitBar.cpp
    source/StatusBar.cpp
    source/String.cpp
    source/Style.cpp
    source/Styles.cpp
    source/System.cpp
    source/Tab.cpp
    source/TextEdit.cpp
    source/Thread.cpp
    source/TimePoint.cpp
    source/Timer.cpp
    source/ToggleButton.cpp
    source/ToolBar.cpp
    source/TreeNode.cpp
    source/TreeView.cpp
    source/TreeViewEvent.cpp
    source/Widget.cpp
    source/WidgetClass.cpp
)

# Include folders
target_include_directories(vaca PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(vaca PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Platform dependent source files
if(WIN32 OR MINGW)
    target_sources(vaca PRIVATE source/Win32/Win32.cpp)
endif()

# Platform dependent libraries
if(WIN32 OR MINGW)
    target_link_libraries(vaca PUBLIC user32 shell32 comctl32 comdlg32 gdi32 msimg32 winmm advapi32 ole32 shlwapi vfw32 wininet)
endif()

# Platform dependent definitions
if(WIN32 OR MINGW)
    # Windows version
    target_compile_definitions(vaca PUBLIC WINVER=${VACA_WIN32_WINNT} _WIN32_WINNT=${VACA_WIN32_WINNT})
    # Internet Explorer version
    target_compile_definitions(vaca PUBLIC _WIN32_IE=${VACA_WIN32_IE})
    # Enable unicode
    target_compile_definitions(vaca PUBLIC UNICODE _UNICODE)
endif()
