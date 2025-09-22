ifdef MINGDIR
   CC = $(MINGDIR)/bin/mingw32-gcc
else
   CC = gcc
endif

ifdef DEBUGMODE
   CFLAGS = -g -Wall
   AL_LIBS = -lalld
   DEFINES = -DDEBUG
else
ifdef RELEASE
   CFLAGS = -Wall -O3 -fexpensive-optimizations -s
   AL_LIBS = -lalleg
else
   CC = gcc
CFLAGS = -O -Wall
   AL_LIBS = -lalleg
endif
endif

ifdef MINGDIR
   ifdef STATICLINK
      LIBS = $(AL_LIBS)_s -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound
      DEFINES += -DALLEGRO_STATICLINK
   else
      LIBS = $(AL_LIBS)
   endif
   WINDRES = windres -I rc -O coff
   CFLAGS += -mwindows -I$(MINGDIR)/include
   OBJ = listports.o get_port_names.o
   EXT = .exe
else
   # Create minimal build without Allegro for now
   LIBS = 
   # Add include path for Allegro headers (for definitions only)
   CFLAGS += -I./build_tools/include
   EXT = .exe
endif


ifndef NOWERROR
   CFLAGS += -Werror
endif

ifdef DEFINES
   CFLAGS += $(DEFINES)
endif

# Original objects
OBJ += main.o main_menu.o serial.o options.o sensors.o trouble_code_reader.o custom_gui.o error_handlers.o about.o reset.o

# Allegro implementation (single point of allegro.h inclusion)
OBJ += allegro_impl.o

# Enhanced diagnostic objects - temporarily disabled for core build
# OBJ += main_menu_enhanced.o ecu_programming.o advanced_diagnostics.o realtime_charts.o enhanced_communication.o

BIN = ScanTool.exe

ifdef MINGDIR
endif

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LIBS)

ifdef MINGDIR
release:
	make RELEASE=1 STATICLINK=1
else
release:
	make RELEASE=1
endif

all: $(BIN)

clean:
	rm -f $(OBJ)

veryclean: clean
	rm -f $(BIN)

scantool.res: scantool.rc scantool.ico
	windres -O coff -o scantool.res -i scantool.rc

main.o: main.c globals.h main_menu.h error_handlers.h options.h serial.h version.h
	$(CC) $(CFLAGS) -c main.c

main_menu.o: main_menu.c globals.h about.h trouble_code_reader.h sensors.h options.h serial.h custom_gui.h main_menu.h
	$(CC) $(CFLAGS) -c main_menu.c

serial.o: serial.c globals.h serial.h
	$(CC) $(CFLAGS) -c serial.c

options.o: options.c globals.h custom_gui.h serial.h options.h
	$(CC) $(CFLAGS) -c options.c

sensors.o: sensors.c globals.h serial.h options.h error_handlers.h sensors.h custom_gui.h
	$(CC) $(CFLAGS) -c sensors.c

trouble_code_reader.o: trouble_code_reader.c globals.h serial.h options.h custom_gui.h error_handlers.h trouble_code_reader.h
	$(CC) $(CFLAGS) -c trouble_code_reader.c

custom_gui.o: custom_gui.c globals.h custom_gui.h
	$(CC) $(CFLAGS) -c custom_gui.c

error_handlers.o: error_handlers.c globals.h error_handlers.h
	$(CC) $(CFLAGS) -c error_handlers.c

about.o: about.c globals.h custom_gui.h serial.h sensors.h options.h version.h about.h
	$(CC) $(CFLAGS) -c about.c

reset.o: reset.c globals.h custom_gui.h main_menu.h serial.h reset.h
	$(CC) $(CFLAGS) -c reset.c

listports.o: listports.c listports.h
	$(CC) $(CFLAGS) -c listports.c

get_port_names.o: get_port_names.c listports.h get_port_names.h
	$(CC) $(CFLAGS) -c get_port_names.c

# Special rule for allegro_impl.c - needs ALLEGRO_IMPL_C define and relaxed warnings
allegro_impl.o: allegro_impl.c allegro_common.h
	$(CC) -O -Wall -DALLEGRO_IMPL_C -IC:/MinGW/include -c allegro_impl.c
