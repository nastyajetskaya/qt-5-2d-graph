QMAKE_CXXFLAGS += -Werror -fsanitize=address,undefined
QMAKE_LFLAGS=-fsanitize=address,undefined

HEADERS       = window.h \
    functions.h
SOURCES       = main.cpp \
                method1.cpp \
                method2.cpp \
                window.cpp
QT += widgets        
