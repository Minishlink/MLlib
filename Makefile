#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
endif

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

export PATH	:=	$(DEVKITPPC)/bin:$(PATH)

#export LIBML_MAJOR	:= 1
#export LIBML_MINOR	:= 6
#export LIBML_PATCH	:= 0

#---------------------------------------------------------------------------------
PREFIX	:=	powerpc-eabi
#---------------------------------------------------------------------------------

CC		:=	$(PREFIX)-gcc
CXX		:=	$(PREFIX)-g++
AS		:=	$(PREFIX)-as
AR		:=	$(PREFIX)-ar
LD		:=	$(PREFIX)-ld
OBJCOPY		:=	$(PREFIX)-objcopy

BUILD		:=	build

GCC_VERSION	:=	$(shell $(DEVKITPPC)/bin/$(CC) -dumpversion)
DATESTRING	:=	$(shell date +%Y)$(shell date +%m)$(shell date +%d)

#---------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),)
#---------------------------------------------------------------------------------
export BASEDIR		:= $(CURDIR)
export MLDIR		:= $(BASEDIR)/source
export DEPS			:=	$(BASEDIR)/deps
export LIBS			:=	$(BASEDIR)/lib

export INCDIR		:=	$(BASEDIR)/include/

#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------

export LIBDIR		:= $(LIBS)/$(PLATFORM)
export DEPSDIR		:=	$(DEPS)/$(PLATFORM)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------


#---------------------------------------------------------------------------------
MLLIB		:= $(LIBDIR)/source

#---------------------------------------------------------------------------------
INCLUDES	:= -I$(BASEDIR)/include -I$(DEVKITPRO)/libogc/include

MACHDEP		:= -DBIGENDIAN -DGEKKO -mcpu=750 -meabi -msdata=eabi -mhard-float -fmodulo-sched -ffunction-sections -fdata-sections

ifeq ($(PLATFORM),wii)
MACHDEP		+=	-DHW_RVL
endif

#ifeq ($(PLATFORM),cube)
#MACHDEP		+=	-DHW_DOL
#endif

CFLAGS		:= -DGAMECUBE -O2 -Wall $(MACHDEP) -fno-strict-aliasing $(INCLUDES)


#---------------------------------------------------------------------------------
VPATH :=	$(MLDIR)

#---------------------------------------------------------------------------------
MLOBJ		:=	ML_draw.o ML_init.o	\
			ML_input.o ML_modplayer.o ML_mp3player.o \
			ML_time.o ML_video.o ML_system.o ML_sprites.o \
			ML_backgrounds.o ML_images.o \
			PNG.o globals.o MLlib_SplashScreen.png.o

#---------------------------------------------------------------------------------
# Build rules:
#---------------------------------------------------------------------------------
%.o : %.c
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(CC) -MMD -MF $(DEPSDIR)/$*.d $(CFLAGS) -Wa,-mgekko -c $< -o $@

#---------------------------------------------------------------------------------
%.o : %.cpp
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(CC) -MMD -MF $(DEPSDIR)/$*.d $(CFLAGS) -Wa,-mgekko -c $< -o $@
	
#---------------------------------------------------------------------------------
%.png.o	: %.png
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.o : %.S
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(CC) -MMD -MF $(DEPSDIR)/$*.d $(CFLAGS) -D_LANGUAGE_ASSEMBLY -Wa,-mgekko -c $< -o $@

#---------------------------------------------------------------------------------
%.o : %.s
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(AS) -mgekko -Qy $< -o $@

#---------------------------------------------------------------------------------
%.a:
#---------------------------------------------------------------------------------
	$(AR) -rc $@ $^


#---------------------------------------------------------------------------------
all: wii
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
#wii: include/libversion.h
wii:
#---------------------------------------------------------------------------------
	@[ -d $(LIBS)/wii ] || mkdir -p $(LIBS)/wii
	@[ -d $(DEPS)/wii ] || mkdir -p $(DEPS)/wii
	@[ -d wii ] || mkdir -p wii
	@$(MAKE) PLATFORM=wii libs -C wii -f $(CURDIR)/Makefile
	@mv $(LIBS)/wii/source.a $(BASEDIR)/lib/wii/libMLlib.a

#---------------------------------------------------------------------------------
#cube: include/libversion.h
#cube:
#---------------------------------------------------------------------------------
#	@[ -d $(LIBS)/cube ] || mkdir -p $(LIBS)/cube
#	@[ -d $(DEPS)/cube ] || mkdir -p $(DEPS)/cube
#	@[ -d cube ] || mkdir -p cube
#	@$(MAKE) PLATFORM=cube libs -C cube -f $(CURDIR)/Makefile

##---------------------------------------------------------------------------------
#include/libversion.h: Makefile
##---------------------------------------------------------------------------------
#	@echo "#ifndef __LIBVERSION_H__" > $@
#	@echo "#define __LIBVERSION_H__" >> $@
#	@echo >> $@
#	@echo "#define _V_MAJOR_	$(LIBML_MAJOR)" >> $@
#	@echo "#define _V_MINOR_	$(LIBML_MINOR)" >> $@
#	@echo "#define _V_PATCH_	$(LIBML_PATCH)" >> $@
#	@echo >> $@
#	@echo "#define _V_DATE_			__DATE__" >> $@
#	@echo "#define _V_TIME_			__TIME__" >> $@
#	@echo >> $@
#	@echo '#define _V_STRING "libML Release '$(LIBML_MAJOR).$(LIBML_MINOR).$(LIBML_PATCH)'"' >> $@
#	@echo >> $@
#	@echo "#endif // __LIBVERSION_H__" >> $@

#---------------------------------------------------------------------------------
$(MLLIB).a: $(MLOBJ)
#---------------------------------------------------------------------------------
 
.PHONY: libs install dist docs cube wii

##---------------------------------------------------------------------------------
install:
##---------------------------------------------------------------------------------
	@mkdir -p $(DEVKITPRO)/MLlib/lib
	@mkdir -p $(DEVKITPRO)/MLlib/include
	@cp -rv $(INCDIR)*.h $(DEVKITPRO)/MLlib/include/
	@cp -rv $(LIBS)/wii/libMLlib.a $(DEVKITPRO)/MLlib/lib/


#---------------------------------------------------------------------------------
dist:
#---------------------------------------------------------------------------------
	@tar    --exclude=*CVS* --exclude=wii --exclude=cube --exclude=*deps* \
		--exclude=*.bz2 --exclude=*lib/* --exclude=*docs/* \
		-cvjf MLlib-src-$(DATESTRING).tar.bz2 *
	@tar -cvjf MLlib-$(DATESTRING).tar.bz2 include source


LIBRARIES	:=	$(MLLIB).a

#---------------------------------------------------------------------------------
libs: $(LIBRARIES)
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
clean:
#---------------------------------------------------------------------------------
	rm -fr $(MLPATH)/include 
	rm -fr $(MLPATH)/lib
	rm -fr wii
	rm -fr $(DEPS)
	rm -fr $(LIBS)
	rm -f *.map

#---------------------------------------------------------------------------------
docs:
#---------------------------------------------------------------------------------
	doxygen mllib.dox

-include $(DEPSDIR)/*.d
