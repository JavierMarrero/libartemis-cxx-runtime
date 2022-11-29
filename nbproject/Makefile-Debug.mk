#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/sources/Arch/Windows/DllMain.o \
	${OBJECTDIR}/sources/Core/Exception.o \
	${OBJECTDIR}/sources/Core/IllegalStateException.o \
	${OBJECTDIR}/sources/Core/IndexOutOfBoundsException.o \
	${OBJECTDIR}/sources/Core/NullPointerException.o \
	${OBJECTDIR}/sources/Core/Object.o \
	${OBJECTDIR}/sources/Core/ReferenceCounted.o \
	${OBJECTDIR}/sources/Core/String.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/axf/core/array.o \
	${TESTDIR}/tests/axf/core/memory/smart_references.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wall
CXXFLAGS=-Wall

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libartemis-cxx.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libartemis-cxx.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libartemis-cxx.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared

${OBJECTDIR}/sources/Arch/Windows/DllMain.o: sources/Arch/Windows/DllMain.cpp
	${MKDIR} -p ${OBJECTDIR}/sources/Arch/Windows
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Arch/Windows/DllMain.o sources/Arch/Windows/DllMain.cpp

${OBJECTDIR}/sources/Core/Exception.o: sources/Core/Exception.cpp
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/Exception.o sources/Core/Exception.cpp

${OBJECTDIR}/sources/Core/IllegalStateException.o: sources/Core/IllegalStateException.cpp
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/IllegalStateException.o sources/Core/IllegalStateException.cpp

${OBJECTDIR}/sources/Core/IndexOutOfBoundsException.o: sources/Core/IndexOutOfBoundsException.cpp
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/IndexOutOfBoundsException.o sources/Core/IndexOutOfBoundsException.cpp

${OBJECTDIR}/sources/Core/NullPointerException.o: sources/Core/NullPointerException.cpp
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/NullPointerException.o sources/Core/NullPointerException.cpp

${OBJECTDIR}/sources/Core/Object.o: sources/Core/Object.cpp
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/Object.o sources/Core/Object.cpp

${OBJECTDIR}/sources/Core/ReferenceCounted.o: sources/Core/ReferenceCounted.cpp
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/ReferenceCounted.o sources/Core/ReferenceCounted.cpp

${OBJECTDIR}/sources/Core/String.o: sources/Core/String.cpp
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/String.o sources/Core/String.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/axf/core/array.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS}   

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/axf/core/memory/smart_references.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   


${TESTDIR}/tests/axf/core/array.o: tests/axf/core/array.cpp 
	${MKDIR} -p ${TESTDIR}/tests/axf/core
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -Iincludes -I. -std=c++98 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/axf/core/array.o tests/axf/core/array.cpp


${TESTDIR}/tests/axf/core/memory/smart_references.o: tests/axf/core/memory/smart_references.cpp 
	${MKDIR} -p ${TESTDIR}/tests/axf/core/memory
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -Iincludes -I. -std=c++98 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/axf/core/memory/smart_references.o tests/axf/core/memory/smart_references.cpp


${OBJECTDIR}/sources/Arch/Windows/DllMain_nomain.o: ${OBJECTDIR}/sources/Arch/Windows/DllMain.o sources/Arch/Windows/DllMain.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources/Arch/Windows
	@NMOUTPUT=`${NM} ${OBJECTDIR}/sources/Arch/Windows/DllMain.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Arch/Windows/DllMain_nomain.o sources/Arch/Windows/DllMain.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/sources/Arch/Windows/DllMain.o ${OBJECTDIR}/sources/Arch/Windows/DllMain_nomain.o;\
	fi

${OBJECTDIR}/sources/Core/Exception_nomain.o: ${OBJECTDIR}/sources/Core/Exception.o sources/Core/Exception.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/sources/Core/Exception.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/Exception_nomain.o sources/Core/Exception.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/sources/Core/Exception.o ${OBJECTDIR}/sources/Core/Exception_nomain.o;\
	fi

${OBJECTDIR}/sources/Core/IllegalStateException_nomain.o: ${OBJECTDIR}/sources/Core/IllegalStateException.o sources/Core/IllegalStateException.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/sources/Core/IllegalStateException.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/IllegalStateException_nomain.o sources/Core/IllegalStateException.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/sources/Core/IllegalStateException.o ${OBJECTDIR}/sources/Core/IllegalStateException_nomain.o;\
	fi

${OBJECTDIR}/sources/Core/IndexOutOfBoundsException_nomain.o: ${OBJECTDIR}/sources/Core/IndexOutOfBoundsException.o sources/Core/IndexOutOfBoundsException.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/sources/Core/IndexOutOfBoundsException.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/IndexOutOfBoundsException_nomain.o sources/Core/IndexOutOfBoundsException.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/sources/Core/IndexOutOfBoundsException.o ${OBJECTDIR}/sources/Core/IndexOutOfBoundsException_nomain.o;\
	fi

${OBJECTDIR}/sources/Core/NullPointerException_nomain.o: ${OBJECTDIR}/sources/Core/NullPointerException.o sources/Core/NullPointerException.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/sources/Core/NullPointerException.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/NullPointerException_nomain.o sources/Core/NullPointerException.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/sources/Core/NullPointerException.o ${OBJECTDIR}/sources/Core/NullPointerException_nomain.o;\
	fi

${OBJECTDIR}/sources/Core/Object_nomain.o: ${OBJECTDIR}/sources/Core/Object.o sources/Core/Object.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/sources/Core/Object.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/Object_nomain.o sources/Core/Object.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/sources/Core/Object.o ${OBJECTDIR}/sources/Core/Object_nomain.o;\
	fi

${OBJECTDIR}/sources/Core/ReferenceCounted_nomain.o: ${OBJECTDIR}/sources/Core/ReferenceCounted.o sources/Core/ReferenceCounted.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/sources/Core/ReferenceCounted.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/ReferenceCounted_nomain.o sources/Core/ReferenceCounted.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/sources/Core/ReferenceCounted.o ${OBJECTDIR}/sources/Core/ReferenceCounted_nomain.o;\
	fi

${OBJECTDIR}/sources/Core/String_nomain.o: ${OBJECTDIR}/sources/Core/String.o sources/Core/String.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/sources/Core/String.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Werror -Iincludes -std=c++98  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sources/Core/String_nomain.o sources/Core/String.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/sources/Core/String.o ${OBJECTDIR}/sources/Core/String_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
