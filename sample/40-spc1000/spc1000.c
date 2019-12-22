/*
    spc1000.c
    SPC1000 Spectrum 48/128 emulator.
    - wait states when accessing contended memory are not emulated
    - video decoding works with scanline accuracy, not cycle accuracy
    - no tape or disc emulation
*/
#include "common.h"
#define CHIPS_IMPL
#ifdef _L
#undef _L
#endif
#ifdef _C
#undef _C
#endif
#ifdef _B
#undef _B
#endif

#include "chips/z80.h"
#include "chips/mc6847.h"
#include "chips/beeper.h"
#include "chips/ay38910.h"
#include "chips/kbd.h"
#include "chips/clk.h"
#include "chips/mem.h"
#include "systems/spc1000.h"
#include "roms/spc1000-roms.h"

/* imports from spc1000-ui.cc */
#ifdef CHIPS_USE_UI
#include "ui.h"
void spc1000ui_init(spc1000_t* spc1000);
void spc1000ui_discard(void);
void spc1000ui_draw(void);
void spc1000ui_exec(spc1000_t* spc1000, uint32_t frame_time_us);
//static const int ui_extra_height = 16;
#else
//static const int ui_extra_height = 0;
#endif

static spc1000_t spc1000;
int getTapePos()
{
	if (!spc1000.tapeMotor)
		return -1;
	return (100 * spc1000.tape_pos) / spc1000.tape_size;
}
/* sokol-app entry, configure application callbacks and window */
static void app_init(void);
static void app_frame(void);
static void app_input(const sapp_event*);
static void app_cleanup(void);

sapp_desc sokol_main(int argc, char* argv[]) {
    sargs_setup(&(sargs_desc){ .argc=argc, .argv=argv });
    //printf("sargs_setup...ok\n");
    // fflush(stdout);
    return (sapp_desc) {
        .init_cb = app_init,
        .frame_cb = app_frame,
        .event_cb = app_input,
        .cleanup_cb = app_cleanup,
//        .width = 3 * spc1000_std_display_width(),
//        .height = 3 * spc1000_std_display_height() + ui_extra_height,
        .window_title = "SPC-1000 Samsung Electronics 1982",
//        .ios_keyboard_resizes_canvas = true
    };
}

/* audio-streaming callback */
static void push_audio(const float* samples, int num_samples, void* user_data) {
    saudio_push(samples, num_samples);
}

/* get spc1000_desc_t struct for given SPC1000 type and joystick type */
spc1000_desc_t spc1000_desc(spc1000_type_t type, spc1000_joystick_type_t joy_type) {
    return (spc1000_desc_t){
        .type = type,
        .joystick_type = joy_type,
        .pixel_buffer = gfx_framebuffer(),
        .pixel_buffer_size = gfx_framebuffer_size(),
        .audio_cb = push_audio,
        .audio_sample_rate = saudio_sample_rate(),
        .rom_spc1000 = dump_spcall_rom,
        .rom_spc1000_size = sizeof(dump_spcall_rom),
        .tap_spc1000 = dump_demo_tap,
        .tap_spc1000_size = sizeof(dump_demo_tap)
        };
}

uint8_t tape_bytes[20*1024*1024];
#include <stdio.h>
/* one-time application init */
void app_init() {
    gfx_init(&(gfx_desc_t){
        #ifdef CHIPS_USE_UI
        .draw_extra_cb = ui_draw,
        #endif
    });
    keybuf_init(6);
    clock_init();
    saudio_setup(&(saudio_desc){0});
    fs_init();
    spc1000_type_t type = SPC1000;
    if (sargs_exists("type")) {
        if (sargs_equals("type", "spc1000")) {
            type = SPC1000;
        }
    }
    spc1000_joystick_type_t joy_type = SPC1K_JOYSTICKTYPE_NONE;
    spc1000_desc_t desc = spc1000_desc(type, joy_type);
    spc1000_init(&spc1000, &desc);
    #ifdef CHIPS_USE_UI
    spc1000ui_init(&spc1000);
    #endif
    bool delay_input = false;
    if (sargs_exists("file")) {

        if (!fs_load_file(sargs_value("file"))) {
            gfx_flash_error();
            delay_input = true;
        }
		else
		{
//			uint8_t* tape_ptr = (uint8_t*)fs_ptr();
            spc1000_insert_tape(&spc1000, fs_ptr(), fs_size());
		}
    } else {
        spc1000_insert_tape(&spc1000, desc.tap_spc1000, desc.tap_spc1000_size); 
    }
    if (!delay_input) {
        if (sargs_exists("input")) {
            keybuf_put(sargs_value("input"));
        }
    }
}

/* per frame stuff, tick the emulator, handle input, decode and draw emulator display */
void app_frame() {
    #if CHIPS_USE_UI
        spc1000ui_exec(&spc1000, (int)(clock_frame_time()*spc1000.speed));
    #else
        spc1000_exec(&spc1000, (int)(clock_frame_time()*spc1000.speed));
    #endif
    gfx_draw(spc1000_display_width(&spc1000), spc1000_display_height(&spc1000));
    const uint32_t load_delay_frames = 60;
    static bool completed = false;
    if (fs_ptr() && clock_frame_count() > load_delay_frames) {
        bool load_success = false;
        if (fs_ext("txt") || fs_ext("bas")) {
            load_success = true;
            keybuf_put((const char*)fs_ptr());
        }
        else {
            //load_success = spc1000_tapeload(&spc1000, fs_ptr(), fs_size());
            //printf("dat=%s\n", fs_ptr());
        }

        if (load_success) {
            if (clock_frame_count() > (load_delay_frames + 10)) {
                gfx_flash_success();
            }  
        }
        else {
            gfx_flash_error();
        }
        fs_free();
    }
    if (completed == false && clock_frame_count() > (load_delay_frames + 10)) {
        //keybuf_put("load\n");
        completed = true;
    } 
	if (sargs_exists("input")) {
		keybuf_put(sargs_value("input"));
	}
    uint8_t key_code;
    if (0 != (key_code = keybuf_get())) {
        spc1000_key_down(&spc1000, key_code);
        spc1000_key_up(&spc1000, key_code);
    }
}

void keydown(int key_code)
{
	spc1000_key_down(&spc1000, key_code);	
}
void keyup(int key_code)
{
	spc1000_key_up(&spc1000, key_code);	
}

/* keyboard input handling */
#ifdef SDL2
#include <SDL.h>
void sdl_keyinput(const SDL_Event *event) {
	int c = 0;
	switch (event->key.keysym.sym)
	{
		case SDLK_SPACE:       	c = 0x20; break;
		case SDLK_LEFT:         c = 0x08; break;
		case SDLK_RIGHT:        c = 0x09; break;
		case SDLK_DOWN:         c = 0x0A; break;
		case SDLK_UP:           c = 0x0B; break;
		case SDLK_RETURN:        c = 0x0D; break;
		case SDLK_BACKSPACE:    c = 0x0C; break;
		case SDLK_ESCAPE:       c = 0x07; break;
		case SDLK_RCTRL: 
		case SDLK_LCTRL: 		c = 0x0F; break; 
		case SDLK_F1:           c = 0xF1; break;
		case SDLK_F2:           c = 0xF2; break;
		case SDLK_F3:           c = 0xF3; break;
		case SDLK_F4:           c = 0xF4; break;
		case SDLK_F5:           c = 0xF5; break;
		case SDLK_END:          c = 0xF6; break;
		case SDLK_CAPSLOCK:		c = 0xF7; break;
		case SDLK_RALT:
		case SDLK_LALT:			c = 0xF8; break;
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:		c = 0x0E; break;
	}
	if (event->key.keysym.sym > 0x20 && event->key.keysym.sym < 0x7f)
		c = event->key.keysym.sym;
	if (c) {
		if (event->type == SDL_KEYDOWN) {
			spc1000_key_down(&spc1000, c);
		}
		else {
			spc1000_key_up(&spc1000, c);
		}
	}	
}
#endif
void app_input(const sapp_event* event) {
    #ifdef CHIPS_USE_UI
    if (ui_input(event)) {
        /* input was handled by UI */
        return;
    }
    #endif
    switch (event->type) {
        int c;
#if 0
        case SAPP_EVENTTYPE_CHAR:
            c = (int) event->char_code;
            if ((c > 0x20) && (c < 0x7F)) {
                spc1000_key_down(&spc1000, c);
                spc1000_key_up(&spc1000, c);
            }
            break;
#endif
        case SAPP_EVENTTYPE_KEY_DOWN:
        case SAPP_EVENTTYPE_KEY_UP:
            switch (event->key_code) {
                case SAPP_KEYCODE_SPACE:        c = 0x20; break;
                case SAPP_KEYCODE_LEFT:         c = 0x08; break;
                case SAPP_KEYCODE_RIGHT:        c = 0x09; break;
                case SAPP_KEYCODE_DOWN:         c = 0x0A; break;
                case SAPP_KEYCODE_UP:           c = 0x0B; break;
                case SAPP_KEYCODE_ENTER:        c = 0x0D; break;
                case SAPP_KEYCODE_BACKSPACE:    c = 0x0C; break;
                case SAPP_KEYCODE_ESCAPE:       c = 0x07; break;
                case SAPP_KEYCODE_RIGHT_CONTROL: 
                case SAPP_KEYCODE_LEFT_CONTROL: c = 0x0F; break; 
                case SAPP_KEYCODE_F1:           c = 0xF1; break;
                case SAPP_KEYCODE_F2:           c = 0xF2; break;
                case SAPP_KEYCODE_F3:           c = 0xF3; break;
                case SAPP_KEYCODE_F4:           c = 0xF4; break;
                case SAPP_KEYCODE_F5:           c = 0xF5; break;
                case SAPP_KEYCODE_END:          c = 0xF6; break;
				case SAPP_KEYCODE_CAPS_LOCK:	c = 0xF7; break;
				case SAPP_KEYCODE_RIGHT_ALT:
				case SAPP_KEYCODE_LEFT_ALT:		c = 0xF8; break;
				case SAPP_KEYCODE_RIGHT_SHIFT:
				case SAPP_KEYCODE_LEFT_SHIFT:	c = 0x0E; break;

                default:                        c = 0; break;
            }
			if (event->key_code > 0x20 && event->key_code < 0x7f)
				c = event->key_code;
            if (c) {
                if (event->type == SAPP_EVENTTYPE_KEY_DOWN) {
                    spc1000_key_down(&spc1000, c);
                }
                else {
                    spc1000_key_up(&spc1000, c);
                }
            }
            break;
        case SAPP_EVENTTYPE_TOUCHES_BEGAN:
//            sapp_show_keyboard(true);
            break;
        default:
            break;
    }
}

/* application cleanup callback */
void app_cleanup() {
    spc1000_discard(&spc1000);
    #ifdef CHIPS_USE_UI
    spc1000ui_discard();
    #endif
    saudio_shutdown();
    gfx_shutdown();
    sargs_shutdown();
}
