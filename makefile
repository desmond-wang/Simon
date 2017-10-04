# super simple makefile
# call it using 'make NAME=name_of_code_file_without_extension'
# (assumes a .cpp extension)
CXXFLAGS = -std=c++14 -g -Wall -Wextra -MMD
LDFLAGS = -L/opt/X11/lib -lX11 -lstdc++ ${MAC_OPT}

ifdef ENHANCED
EXEC = a1-enhanced
CXXFLAGS := ${CXXFLAGS} -DENHANCED
else
EXEC = a1-basic
endif

SOURCES = a1-basic.cpp button.cpp controller.cpp text.cpp xwindow.cpp
OBJECTS = ${SOURCES:.cpp=.o}
DEPENDS = ${SOURCES:.cpp=.d}

# Add $(MAC_OPT) to the compile line for macOS 
# (should be ignored by Linux, set to nothing if causing problems)
MAC_OPT = -I/opt/X11/include

.PHONY: run clean

${EXEC}: ${OBJECTS}
	@echo "Compiling..."
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} ${LDFLAGS}

-include ${DEPENDS}

run: ${EXEC}
	@echo "Running..."
	./${EXEC}

clean:
	${RM} ${EXEC} ${OBJECTS} ${DEPENDS}
