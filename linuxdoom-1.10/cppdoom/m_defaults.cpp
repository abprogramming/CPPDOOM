
#include <unordered_map>
#include <string>
#include <variant>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <ctype.h>


#include "doomdef.h"

#include "z_zone.h"

#include "m_swap.h"
#include "m_argv.h"

#include "w_wad.h"

#include "i_system.h"
#include "i_video.h"
#include "v_video.h"

#include "hu_stuff.h"

// State.
#include "doomstat.h"

// Data.
#include "dstrings.h"

#include "m_defaults.h"

//
// DEFAULTS
//
int		usemouse;
int		usejoystick;

extern int	key_right;
extern int	key_left;
extern int	key_up;
extern int	key_down;

extern int	key_strafeleft;
extern int	key_straferight;

extern int	key_fire;
extern int	key_use;
extern int	key_strafe;
extern int	key_speed;

extern int	mousebfire;
extern int	mousebstrafe;
extern int	mousebforward;

extern int	joybfire;
extern int	joybstrafe;
extern int	joybuse;
extern int	joybspeed;

extern int	viewwidth;
extern int	viewheight;

extern int	mouseSensitivity;
extern int	showMessages;

extern int	detailLevel;

extern int	screenblocks;

extern int	showMessages;

// machine-independent sound params
extern	int	numChannels;


// UNIX hack, to be removed.
#ifdef SNDSERV
extern char*	sndserver_filename;
extern int	mb_used;
#endif

#ifdef LINUX
char*		mousetype;
char*		mousedev;
#endif

extern char*	chat_macros[];

CPPDOOM_BEGIN

struct default_t
{
    std::variant<int*, char**> Location;
    std::variant<int, std::string> DefaultValue;
    //int		scantranslate;		// PC scan code hack
   // int		untranslated;		// lousy hack
};

static std::unordered_map<std::string, default_t> Defaults =
{
    {"mouse_sensitivity", {&mouseSensitivity, 5} },
    {"sfx_volume", {&snd_SfxVolume, 8} },
    {"music_volume", {&snd_MusicVolume, 8} },
    {"show_messages", {&showMessages, 1} },
    

#ifdef NORMALUNIX
    {"key_right", {&key_right, KEY_RIGHTARROW} },
    {"key_left", {&key_left, KEY_LEFTARROW} },
    {"key_up", {&key_up, KEY_UPARROW} },
    {"key_down", {&key_down, KEY_DOWNARROW} },
    {"key_strafeleft", {&key_strafeleft, ','} },
    {"key_straferight", {&key_straferight, '.'} },

    {"key_fire", {&key_fire, KEY_RCTRL} },
    {"key_use", {&key_use, ' '} },
    {"key_strafe", {&key_strafe, KEY_RALT} },
    {"key_speed", {&key_speed, KEY_RSHIFT} },

// UNIX hack, to be removed. 
#ifdef SNDSERV
    {"sndserver", { &sndserver_filename, "sndserver"} },
    {"mb_used", { &mb_used, 2} },
#endif
    
#endif

#ifdef LINUX
    {"mousedev", { &mousedev, "/dev/ttyS0"} },
    {"mousetype", { &mousetype, "microsoft"} },
#endif

    {"use_mouse", {&usemouse, 1} },
    {"mouseb_fire", {&mousebfire,0} },
    {"mouseb_strafe", {&mousebstrafe,1} },
    {"mouseb_forward", {&mousebforward,2} },

    {"use_joystick", {&usejoystick, 0} },
    {"joyb_fire", {&joybfire,0} },
    {"joyb_strafe", {&joybstrafe,1} },
    {"joyb_use", {&joybuse,3} },
    {"joyb_speed", {&joybspeed,2} },

    {"screenblocks", {&screenblocks, 9} },
    {"detaillevel", {&detailLevel, 0} },

    {"snd_channels", {&numChannels, 3} },



    {"usegamma", {&usegamma, 0} },

    {"chatmacro0", { &chat_macros[0], HUSTR_CHATMACRO0 } },
    {"chatmacro1", { &chat_macros[1], HUSTR_CHATMACRO1 } },
    {"chatmacro2", { &chat_macros[2], HUSTR_CHATMACRO2 } },
    {"chatmacro3", { &chat_macros[3], HUSTR_CHATMACRO3 } },
    {"chatmacro4", { &chat_macros[4], HUSTR_CHATMACRO4 } },
    {"chatmacro5", { &chat_macros[5], HUSTR_CHATMACRO5 } },
    {"chatmacro6", { &chat_macros[6], HUSTR_CHATMACRO6 } },
    {"chatmacro7", { &chat_macros[7], HUSTR_CHATMACRO7 } },
    {"chatmacro8", { &chat_macros[8], HUSTR_CHATMACRO8 } },
    {"chatmacro9", { &chat_macros[9], HUSTR_CHATMACRO9 } }

};

int	numdefaults;
char*	defaultfile;


//
// M_SaveDefaults
//
void M_SaveDefaults (void)
{
    FILE*	f;
    f = fopen (defaultfile, "w");
    if (!f)
	    return; // can't write the file, but don't complain
		
    for (const auto& p : Defaults)
    {
        const std::string& Name = p.first;
        const default_t& d = p.second;
        if (std::holds_alternative<int*>(d.Location))
        {
            fprintf (f, "%s\t\t%i\n", Name.c_str(), *(std::get<int*>(d.Location)));
        }
        else
        {
            fprintf (f, "%s\t\t\"%s\"\n", Name.c_str(), *(std::get<char**>(d.Location)));
        }
    }
	
    fclose (f);
}


//
// M_LoadDefaults
//
extern byte	scantokey[128];

void M_LoadDefaults (void)
{
    int		i;
    int		len;
    FILE*	f;
    char	def[80];
    char	strparm[100];
    char*	newstring;
    int		parm;
    bool	isstring;

    decltype(default_t::DefaultValue) Value;
    
    for (auto& p : Defaults)
    {
        default_t& d = p.second;
        if (std::holds_alternative<int*>(d.Location))
        {
            *(std::get<int*>(d.Location)) = std::get<int>(d.DefaultValue);
        }
        else
        {

            *(std::get<char**>(d.Location)) = std::get<std::string>(d.DefaultValue).data();
        }
    }

    // read the file in, overriding any set defaults
    f = fopen (defaultfile, "r");
    if (f)
    {
        while (!feof(f))
        {
            isstring = false;
            if (fscanf (f, "%79s %[^\n]\n", def, strparm) == 2)
            {
                if (strparm[0] == '"')
                {
                    // get a string default
                    isstring = true;
                    len = strlen(strparm);
                    newstring = (char *) malloc(len);
                    strparm[len-1] = 0;
                    strcpy(newstring, strparm+1);
                    Value = std::string(newstring);
                }
                else
                {
                    if (strparm[0] == '0' && strparm[1] == 'x')
                        sscanf(strparm+2, "%x", &parm);
                    else
                        sscanf(strparm, "%i", &parm);

                    Value = parm;
                }

                auto it = Defaults.find(std::string(def));
                if (it != Defaults.end())
                {
                    if (std::holds_alternative<int>(Value))
                    {
                        *(std::get<int*>(it->second.Location)) = std::get<int>(Value);
                    }
                    else
                    {
                        *(std::get<char**>(it->second.Location)) = std::get<std::string>(Value).data();
                    }
                }
            }
        }
		
	fclose (f);
    }
}

CPPDOOM_END