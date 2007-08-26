CC=g++
CFLAGS=-Wall -Werror -Wno-sign-compare -Wno-switch -Wno-switch-enum -Wno-non-virtual-dtor -O2 -c -g `sdl-config --cflags` -I/usr/include/GL -I/usr/local/include/boost-1_34 -I/System/Library/Frameworks/OpenGL.framework/Headers -I/sw/include -I/usr/include/qt4
LDFLAGS=-O2 `sdl-config --libs` -lGL -lGLU -lSDL_image -lSDL_ttf -L/System/Library/Frameworks/OpenGL.framework/Libraries -lboost_regex
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o) xml.o
EDITOR_OBJECTS=editor/main.o editor/editpartydialog.o editor/editormainwindow.o editor/editorglwidget.o editor/terrainhandler.o editor/editwmldialog.o gamemap.o map_avatar.o wml_node.o wml_parser.o wml_utils.o wml_writer.o camera.o tile.o base_terrain.o model.o string_utils.o tile_logic.o parse3ds.o parseark.o parsedae.o terrain_feature.o material.o particle.o particle_emitter.o particle_system.o formula.o formula_tokenizer.o font.o texture.o filesystem.o raster.o surface_cache.o surface.o sdl_algo.o variant.o xml.o
EXECUTABLE=game

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm -f *.o editor/*.o edit game editor/*.moc editor/ui_*.hpp

cleanvi:
	rm .*swp editor/.*swp

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

editor/ui_mainwindow.hpp: editor/mainwindow.ui
	cd editor ; uic mainwindow.ui > ui_mainwindow.hpp

editor/ui_editpartydialog.hpp: editor/editpartydialog.ui
	cd editor ; uic editpartydialog.ui > ui_editpartydialog.hpp

editor/ui_editwml.hpp: editor/editwml.ui
	cd editor ; uic editwml.ui > ui_editwml.hpp

editor/editorglwidget.moc: editor/editorglwidget.hpp
	cd editor ; moc editorglwidget.hpp > editorglwidget.moc

editor/editormainwindow.moc: editor/editormainwindow.hpp
	cd editor ; moc editormainwindow.hpp > editormainwindow.moc

editor/terrainhandler.moc: editor/terrainhandler.hpp
	cd editor ; moc terrainhandler.hpp > terrainhandler.moc

editor/editpartydialog.moc: editor/editpartydialog.hpp
	cd editor ; moc editpartydialog.hpp > editpartydialog.moc

editor/editwmldialog.moc: editor/editwmldialog.hpp
	cd editor ; moc editwmldialog.hpp > editwmldialog.moc

edit: editor/ui_mainwindow.hpp editor/ui_editpartydialog.hpp editor/ui_editwml.hpp editor/editorglwidget.moc editor/editormainwindow.moc editor/terrainhandler.moc editor/editpartydialog.moc editor/editwmldialog.moc $(EDITOR_OBJECTS)
	$(CC) $(LDFLAGS) $(EDITOR_OBJECTS) -L/Library/Frameworks/QtCore.framework/ -L/Library/Frameworks/QtGui.framework -L/Library/Frameworks/QtOpenGL.framework -lQtCore -lQtGui -lQtOpenGL -o edit

formula_test: formula.cpp formula_tokenizer.cpp variant.cpp
	g++ -g formula.cpp formula_tokenizer.cpp variant.cpp -DUNIT_TEST_FORMULA $(LDFLAGS) -o formula_test

xml_test: xml/xml.c
	gcc xml/xml.c -g -DUNIT_TEST_XML $(LDFLAGS) -o xml_test

xml.o: xml/xml.c
	gcc -c xml/xml.c -o xml.o

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
