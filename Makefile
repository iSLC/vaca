default: all

# ----------------------------------------------------------------------
# Configuration
# ----------------------------------------------------------------------

# DEBUG = 1
# STATIC = 1
# PROFILE = 1
# UNICODE = 1

WIN32_IMPL = 1
# ALLEG_IMPL = 1

ifdef MEMORY_LEAK_DETECTOR
ifdef STATIC
MEMORY_LEAK_DETECTOR = 1
endif
endif

# ----------------------------------------------------------------------
# Extensions
# ----------------------------------------------------------------------

OBJ = .o
LIB = .a
EXE = .exe

# ----------------------------------------------------------------------
# Programs
# ----------------------------------------------------------------------

AR	= ar
GXX	= g++
RM	= rm
CP	= cp
RMDIR	= rmdir
MKDIR	= mkdir
WINDRES = windres

# ----------------------------------------------------------------------
# Sources & Examples
# ----------------------------------------------------------------------

LIB_SOURCES = Anchor AnchorLayout Application BandedDockArea		\
	      BasicDockArea Bix BoxConstraint BoxLayout Brush Button	\
	      ButtonBase CancelableEvent CheckBox ClientLayout		\
	      Clipboard CloseEvent Color ColorDialog ComboBox		\
	      CommonDialog Component Condition Constraint Cursor	\
	      CustomButton CustomLabel Debug Dialog DockArea DockBar	\
	      DockFrame DropFilesEvent Edit Event FileDialog		\
	      FindTextDialog Font FontDialog Frame Graphics GroupBox	\
	      Icon Image ImageList KeyEvent Keys Label Layout		\
	      LinkLabel ListBox ListView Mdi Menu MenuItemEvent		\
	      MouseEvent Mutex Panel Pen Point ProgressBar RadioButton	\
	      ReBar Rect Region ResourceId Scintilla Size Slider	\
	      SpinButton Spinner String Style System Tab Thread		\
	      TimePoint Timer ToggleButton ToolBar TreeNode TreeView	\
	      TreeViewEvent Vaca Widget WidgetClass

EXAMPLES = AnchorLayouts AutoCompletion Bixes BouncingBalls BoxLayouts	\
	   Buttons ColoredButton ComboBoxes Commands DataGrids		\
	   EditableListBox Edits EyeDropper FontMetrics FreeOfLayout	\
	   Hashing HelloWorld Images Labels LikeScript Maths		\
	   MenuResource MiniExplorer PensBrushes Primitives		\
	   ProgressBars Regions Scribble Sliders Spinners StdCommands	\
	   Sudoku SystemImageList Tabs TextEditor Threads Timers	\
	   ToolBars Trees

TESTS = \
	test_bind \
	test_point \
	test_rect \
	test_signal \
	test_size \
	test_string

# EXAMPLES = 

# ----------------------------------------------------------------------
# Flags
# ----------------------------------------------------------------------

CXXFLAGS = -Iinclude -Iscintilla/include -Ithird_party -W -Wall -Wno-unused
DLLFLAGS =
LFLAGS = -mwindows -Lobj
LIBS =

# Win32
ifdef WIN32_IMPL
  CXXFLAGS += -DVACA_WIN32_IMPL -DWINVER=0x0500 -D_WIN32_IE=0x0500 -D_WIN32_WINNT=0x0500
  ifdef UNICODE
    CXXFLAGS += -DUNICODE -D_UNICODE
  endif

  LIBS += -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool \
	  -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 \
	  -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lwsock32 \
	  -lodbc32 -lcomctl32 -lsecur32 -lmsimg32
endif

# Allegro
ifdef ALLEG_IMPL
  CXXFLAGS += -DVACA_ALLEG_IMPL
  LIBS += -lalleg
endif

EXE_LIBS = $(LIBS)

# ----------------------------------------------------------------------
# STATIC or SHARED?
# ----------------------------------------------------------------------

ifdef STATIC

  VACA_LIB_FLAGS = -DVACA_STATIC
  VACA_EXE_FLAGS = -DVACA_STATIC

  ifdef DEBUG
    SUFFIX = _ds
  else
    SUFFIX = _s
  endif

else

  VACA_LIB_FLAGS = -DVACA_SRC
  VACA_EXE_FLAGS = 

  ifdef DEBUG
    SUFFIX = _d
  endif

endif

# ----------------------------------------------------------------------
# PROFILE?
# ----------------------------------------------------------------------

ifdef PROFILE
  CXXFLAGS += -pg
  LFLAGS += -pg
  DLLFLAGS += -pg

  ifdef SUFFIX
    SUFFIX := $(SUFFIX)p
  else
    SUFFIX := _p
  endif
endif

# ----------------------------------------------------------------------
# UNICODE
# ----------------------------------------------------------------------

ifdef UNICODE
  ifdef SUFFIX
    SUFFIX := $(SUFFIX)u
  else
    SUFFIX := _u
  endif
endif

# ----------------------------------------------------------------------
# DEBUG or NDEBUG?
# ----------------------------------------------------------------------

ifdef DEBUG
  VACA_LIB_FLAGS += -D_DEBUG
  VACA_EXE_FLAGS += -D_DEBUG
  CXXFLAGS += -g
  LFLAGS += -g
  DLLFLAGS += -g
else
#   VACA_LIB_FLAGS += -DNDEBUG
#   VACA_EXE_FLAGS += -DNDEBUG
  CXXFLAGS += -s
  LFLAGS += -s
  DLLFLAGS += -s
endif

# ----------------------------------------------------------------------
# Memory leak detector
# ----------------------------------------------------------------------

ifdef MEMORY_LEAK_DETECTOR
#   VACA_LIB_FLAGS += -D_DEBUG
#   VACA_EXE_FLAGS += -D_DEBUG
  CXXFLAGS += -Ithird_party/nvwa -DMEMORY_LEAK_DETECTOR
#   LFLAGS += -g
#   DLLFLAGS += -g
  LIB_SOURCES += $(basename $(notdir $(wildcard third_party/nvwa/*.cpp)))
endif

# ----------------------------------------------------------------------
# Files
# ----------------------------------------------------------------------

VACA_LIB = lib/libvaca$(SUFFIX)$(LIB)
ifdef UNICODE
  VACA_DLL = bin/VacaUnicode.dll
else
  VACA_DLL = bin/Vaca.dll
endif

LIB_OBJS = $(addprefix obj/Library., $(addsuffix $(SUFFIX)$(OBJ), $(LIB_SOURCES)))
EXAMPLES_EXE = $(addprefix bin/, $(addsuffix $(SUFFIX)$(EXE), $(EXAMPLES)))
TESTS_EXE = $(addprefix bin/, $(addsuffix $(SUFFIX)$(EXE), $(TESTS)))

EXAMPLES_DIRS_WITH_RC = $(dir $(wildcard examples/*/*.rc))

# ----------------------------------------------------------------------
# Generic Rules
# ----------------------------------------------------------------------

vpath %.cpp $(addprefix examples/, $(EXAMPLES)) tests
vpath %.rc $(EXAMPLES_DIRS_WITH_RC)

obj/Library.%$(SUFFIX)$(OBJ): src/%.cpp
	$(GXX) $(CXXFLAGS) $(VACA_LIB_FLAGS) -o $@ -c $<

obj/Library.%$(SUFFIX)$(OBJ): third_party/nvwa/%.cpp
	$(GXX) $(CXXFLAGS) $(VACA_LIB_FLAGS) -o $@ -c $<

obj/Example.%$(SUFFIX)$(OBJ): %.cpp
	$(GXX) $(CXXFLAGS) $(VACA_EXE_FLAGS) -o $@ -c $<

obj/Example.res: examples/Example.rc
	$(WINDRES) -Iexamples/ -O coff -o $@ -i $<

obj/Example.%.res: %.rc
	$(WINDRES) $(addprefix -I, $(EXAMPLES_DIRS_WITH_RC)) -O coff -o $@ -i $<

obj/Test.%$(SUFFIX)$(OBJ): tests/%.cpp
	$(GXX) $(CXXFLAGS) $(VACA_EXE_FLAGS) -o $@ -c $<

# %.h.gch: %.h
# 	$(GXX) $(CXXFLAGS) -o $@ -c $<

ifdef STATIC
$(VACA_LIB): $(LIB_OBJS)
	$(RM) -f $@
	$(AR) crs $(VACA_LIB) $(LIB_OBJS)
else
$(VACA_LIB): $(LIB_OBJS)
	$(RM) -f $@

$(VACA_DLL): $(VACA_LIB)
	$(GXX) $(DLLFLAGS) -shared -fPIC -o $(VACA_DLL) $(LIB_OBJS) -Wl,--out-implib,$(VACA_LIB) $(LIBS)
endif

# general examples

bin/%$(EXE): obj/Example.%$(OBJ) obj/Example.res $(VACA_LIB)
	$(GXX) $(LFLAGS) -o $@ $^ $(EXE_LIBS)

# special examples

bin/EyeDropper$(SUFFIX)$(EXE): obj/Example.EyeDropper$(SUFFIX)$(OBJ) obj/Example.EyeDropper.res $(VACA_LIB)
	$(GXX) $(LFLAGS) -o $@ $^ $(EXE_LIBS)

bin/Hashing$(SUFFIX)$(EXE): obj/Example.Hashing$(SUFFIX)$(OBJ) obj/Example.md5$(SUFFIX)$(OBJ) obj/Example.sha1$(SUFFIX)$(OBJ) obj/Example.res $(VACA_LIB)
	$(GXX) $(LFLAGS) -o $@ $^ $(EXE_LIBS)

bin/Images$(SUFFIX)$(EXE): obj/Example.Images$(SUFFIX)$(OBJ) obj/Example.Images.res $(VACA_LIB)
	$(GXX) $(LFLAGS) -o $@ $^ $(EXE_LIBS)

bin/MenuResource$(SUFFIX)$(EXE): obj/Example.MenuResource$(SUFFIX)$(OBJ) obj/Example.MenuResource.res $(VACA_LIB)
	$(GXX) $(LFLAGS) -o $@ $^ $(EXE_LIBS)

bin/TextEditor$(SUFFIX)$(EXE): obj/Example.TextEditor$(SUFFIX)$(OBJ) obj/Example.TextEditor.res $(VACA_LIB)
	$(GXX) $(LFLAGS) -o $@ $^ $(EXE_LIBS)

bin/ToolBars$(SUFFIX)$(EXE): obj/Example.ToolBars$(SUFFIX)$(OBJ) obj/Example.ToolBars.res $(VACA_LIB)
	$(GXX) $(LFLAGS) -o $@ $^ $(EXE_LIBS)

bin/Bixes$(SUFFIX)$(EXE): obj/Example.Bixes$(SUFFIX)$(OBJ) obj/Example.Bixes.res $(VACA_LIB)
	$(GXX) $(LFLAGS) -o $@ $^ $(EXE_LIBS)

# tests

bin/%$(EXE): obj/Test.%$(OBJ) $(VACA_LIB)
	$(GXX) $(LFLAGS) -mconsole -o $@ $^ $(EXE_LIBS)

# ----------------------------------------------------------------------
# Rules
# ----------------------------------------------------------------------

all: lib examples tests

lib: $(VACA_DLL)

examples:  $(EXAMPLES_EXE)

tests: $(TESTS_EXE)

runtests:
	@for file in bin/test_*.exe ; do \
		echo Running ./$$file ... ; \
		if ./$$file ; then \
			echo " - OK" ; \
		else \
			echo " - *FAIL*" ; \
		fi ; \
	done

deps:
	$(GXX) -MM $(CXXFLAGS) src/*.cpp | sed -e 's|^\([A-Za-z_0-9]\+\)|obj/Library.\1\$$\(SUFFIX\)|' > .deps
	$(GXX) -MM $(CXXFLAGS) examples/*/*.cpp | sed -e 's|^\([A-Za-z_0-9]\+\)|obj/Example.\1\$$\(SUFFIX\)|' >> .deps

clean:
	-$(RM) -f obj/*$(OBJ) obj/*.res

distclean: clean
	-$(RM) -f lib/*$(LIB) $(VACA_DLL) $(EXAMPLES_EXE)

ifdef MINGW_ROOT
install: $(VACA_LIB) $(VACA_DLL)
	-$(MKDIR) $(MINGW_ROOT)/include/Vaca
	$(CP) $(wildcard include/Vaca/*.hpp) $(MINGW_ROOT)/include/Vaca
	$(CP) $(VACA_LIB) $(MINGW_ROOT)/lib

uninstall:
	@echo $(RM) $(MINGW_ROOT)/$(VACA_LIB)
	@echo $(RM) $(wildcard $(MINGW_ROOT)/include/Vaca/*.hpp)
	@echo $(RMDIR) $(MINGW_ROOT)/include/Vaca
else
install:
	@echo You must to setup your MINGW_ROOT enviroment variable

uninstall:
	@echo You must to setup your MINGW_ROOT enviroment variable
endif

# ----------------------------------------------------------------------
# Dependencies

-include .deps
