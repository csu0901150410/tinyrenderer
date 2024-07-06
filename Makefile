SYSCONF_LINK = g++
CPPFLAGS     =
LDFLAGS      =
LIBS         = -lm
# CFLAGS 		 = -O2 -g

SRCDIR = src
DESTDIR = output
TARGET  = $(DESTDIR)\main.exe

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(DESTDIR)\\%.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

$(DESTDIR)\\%.o: $(SRCDIR)/%.cpp
	@if not exist $(DESTDIR) mkdir $(DESTDIR)
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

clean:
	-del /Q $(OBJECTS)
	-del /Q $(TARGET)
	-del /Q $(DESTDIR)\\*.tga