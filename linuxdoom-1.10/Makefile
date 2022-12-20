################################################################
#
# $Id:$
#
# $Log:$
#
CC=  gcc  # gcc or g++
CP=  g++  # gcc or g++

# subdirectory for objects
OBJDIR=linux

# subdirectory for the build target
BINDIR=bin

# separate directory for C++ sources
CPPDOOM=cppdoom

CFLAGS=-m32 -g -Wall -DNORMALUNIX -DLINUX # -DUSEASM 
LDFLAGS=-L/usr/X11R6/lib
LIBS=-lXext -lX11 -lnsl -lm -lstdc++ -L/usr/lib/i386-linux-gnu
INCLUDES=-I. -I./$(CPPDOOM)

# not too sophisticated dependency
OBJS=				\
		$(OBJDIR)/doomdef.o		\
		$(OBJDIR)/doomstat.o		\
		$(OBJDIR)/dstrings.o		\
		$(OBJDIR)/i_system.o		\
		$(OBJDIR)/i_sound.o		\
		$(OBJDIR)/i_video.o		\
		$(OBJDIR)/i_net.o			\
		$(OBJDIR)/tables.o			\
		$(OBJDIR)/f_finale.o		\
		$(OBJDIR)/f_wipe.o 		\
		$(OBJDIR)/d_main.o			\
		$(OBJDIR)/d_net.o			\
		$(OBJDIR)/d_items.o		\
		$(OBJDIR)/g_game.o			\
		$(OBJDIR)/m_menu.o			\
		$(OBJDIR)/m_misc.o			\
		$(OBJDIR)/m_defaults.o			\
		$(OBJDIR)/m_argv.o  		\
		$(OBJDIR)/m_bbox.o			\
		$(OBJDIR)/m_fixed.o		\
		$(OBJDIR)/m_swap.o			\
		$(OBJDIR)/m_cheat.o		\
		$(OBJDIR)/m_random.o		\
		$(OBJDIR)/am_map.o			\
		$(OBJDIR)/p_ceilng.o		\
		$(OBJDIR)/p_doors.o		\
		$(OBJDIR)/p_enemy.o		\
		$(OBJDIR)/p_floor.o		\
		$(OBJDIR)/p_inter.o		\
		$(OBJDIR)/p_lights.o		\
		$(OBJDIR)/p_map.o			\
		$(OBJDIR)/p_maputl.o		\
		$(OBJDIR)/p_plats.o		\
		$(OBJDIR)/p_pspr.o			\
		$(OBJDIR)/p_setup.o		\
		$(OBJDIR)/p_sight.o		\
		$(OBJDIR)/p_spec.o			\
		$(OBJDIR)/p_switch.o		\
		$(OBJDIR)/p_mobj.o			\
		$(OBJDIR)/p_telept.o		\
		$(OBJDIR)/p_tick.o			\
		$(OBJDIR)/p_saveg.o		\
		$(OBJDIR)/p_user.o			\
		$(OBJDIR)/r_bsp.o			\
		$(OBJDIR)/r_data.o			\
		$(OBJDIR)/r_draw.o			\
		$(OBJDIR)/r_main.o			\
		$(OBJDIR)/r_plane.o		\
		$(OBJDIR)/r_segs.o			\
		$(OBJDIR)/r_sky.o			\
		$(OBJDIR)/r_things.o		\
		$(OBJDIR)/w_wad.o			\
		$(OBJDIR)/wi_stuff.o		\
		$(OBJDIR)/v_video.o		\
		$(OBJDIR)/st_lib.o			\
		$(OBJDIR)/st_stuff.o		\
		$(OBJDIR)/hu_stuff.o		\
		$(OBJDIR)/hu_lib.o			\
		$(OBJDIR)/s_sound.o		\
		$(OBJDIR)/z_zone.o			\
		$(OBJDIR)/info.o				\
		$(OBJDIR)/sounds.o

all:	 $(BINDIR)/linuxxdoom

clean:
	rm -f *.o *~ *.flc
	rm -f linux/*
	rm -f bin/linuxxdoom

$(BINDIR)/linuxxdoom:	$(OBJS) $(OBJDIR)/i_main.o
	[ -d $(BINDIR) ] || mkdir $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(OBJDIR)/i_main.o \
	-o $(BINDIR)/linuxxdoom $(LIBS)

$(OBJDIR)/%.o:	%.c
	[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o:	$(CPPDOOM)/%.cpp
	[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
	$(CP) -std=c++17 $(CFLAGS) $(INCLUDES) -c $< -o $@

#############################################################
#
#############################################################
