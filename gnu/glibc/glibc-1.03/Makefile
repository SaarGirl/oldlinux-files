# Copyright (C) 1991, 1992 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.

# You should have received a copy of the GNU Library General Public
# License along with the GNU C Library; see the file COPYING.LIB.  If
# not, write to the Free Software Foundation, Inc., 675 Mass Ave,
# Cambridge, MA 02139, USA.

#
#	Master Makefile for the GNU C library
#
ifneq (,)
This makefile requires GNU Make.
endif


# This is the default target; it makes everything except the tests.
.PHONY: all
all: lib headers others # doc

include Makeconfig

include $(objpfx)sysd-dirs
define \n


endef
+sysdep-subdirs := $(subst $(\n), ,$(+sysdep-subdirs))

# These are the subdirectories containing the library source.
+ansi_dirs	:= assert ctype locale math setjmp \
		   signal stdio stdlib malloc string time
+posix_dirs	:= dirent grp pwd posix io termios
+other_dirs	:= resource socket misc gnulib
subdirs		:= $(strip $(+ansi_dirs) $(+posix_dirs) $(+other_dirs) \
			   $(sysdep-subdirs))


# All initialization source files.
+subdir_inits	:= $(wildcard $(foreach dir,$(subdirs),$(dir)/init-$(dir).c))
# All subdirectories containing initialization source files.
+init_subdirs	:= $(patsubst %/,%,$(dir $(+subdir_inits)))


# These are the targets that are made by making them in each subdirectory.
+subdir_targets	:= subdir_lib objects objs others mostlyclean subdir_clean\
		   tests subdir_lint.out subdir_TAGS subdir_headers	\
		   subdir_echo-headers subdir_echo-distinfo subdir_install

headers := features.h errno.h errnos.h limits.h $(stddef.h)	\
	   float.h fl.h

headers: subdir_headers

echo-headers: subdir_echo-headers

ifdef +gnu-stabs
# We're using the GNU linker, so we use the set-element init function.
+init := set-init
else
# We're not using the GNU linker, so we use the munch init function.
+init := munch-init

$(objpfx)munch-init.c: munch.awk munch-tmpl.c $(+subdir_inits)
	awk -f $< subdirs='$(+init_subdirs)' $(word 2,$^) > $@-tmp
	mv $@-tmp $@
endif

aux	:= sysdep $(+init) version start


# What to install.
install-lib = libc.a crt0.o Mcrt1.o


include Makerules

# Install from subdirectories too.
install: subdir_install

# `crt0' is the traditional name for the startup code,
# so that's what we install start.o as.
$(objpfx)crt0.o: $(objpfx)start.o
	-rm -f $@
	ln $< $@

$(objpfx)Mcrt1.o:
	touch $@

lib: subdir_lib


$(objpfx)sysd-dirs: $(+sysdir_pfx)sysdirs
	(echo define sysdep-subdirs;					      \
	 for dir in `cat $<`; do					      \
	   if [ -r $(sysdep_dir)/$$dir/Subdirs ]; then			      \
	     cat $(sysdep_dir)/$$dir/Subdirs;				      \
	   else true;							      \
	   fi;								      \
	 done;								      \
	 echo endef) > $@-tmp
	 mv $@-tmp $@

# This makes the Info file of the documentation from the Texinfo source.
.PHONY: doc
doc:
	$(MAKE) -C manual

# This makes all the subdirectory targets.
.PHONY: $(+subdir_targets)
$(+subdir_targets):
	@lose=nil;			\
	 for dir in $(subdirs); do	\
	 $(+cmdecho) $(MAKE) -C $$dir $@;\
	 $(MAKE) -C $$dir $@ || lose=t;	\
	 done;				\
	 test $$lose = nil

# This clobbers everything that can be regenerated.
.PHONY: clean realclean
realclean: clean
clean:
	-rm -f $(objects) $(+depfiles) munch-init.c
	-rm -f $(libc.a) dist.tar lint.out core TAGS
	-rm -f -r ansi/ trad/
# This is done this way rather than having `subdir_clean' be a
# dependency of this target so that libc.a will be removed before the
# subdirectories are dealt with and so they won't try to remove object
# files from it when it's going to be removed anyway.
	@$(MAKE) subdir_clean

parent_TAGS: $(+allsources)
	$(etags) $^

.PHONY: echo_subdirs
echo_subdirs:;@echo '$(subdirs)'

.PHONY: echo-distinfo parent_echo-distinfo
echo-distinfo: parent_echo-distinfo subdir_echo-distinfo
parent_echo-distinfo:
	@echo $(addprefix +header+,$(headers)) \
	      $(addprefix +nodist+,$(dont_distribute))

# Make the distribution tarfile.

distribute  := README INSTALL NOTES Q+A COPYING.LIB ChangeLog	\
	       Makefile Makeconfig Makerules Rules Make-dist	\
	       ansidecl.m4 ansidecl ansidecl.h			\
	       configure find-sysdirs				\
	       munch-tmpl.c munch.awk gnu-stabs.h sysdep.h

distribute := $(strip $(distribute))

.PHONY: dist
dist: Make-dist
	$(MAKE) -f $< no_deps=t $(Make-dist-args)
