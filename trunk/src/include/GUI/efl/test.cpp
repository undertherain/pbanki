#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <libintl.h>
#include <Evas.h>
#include <Ecore.h>
#include <Ecore_X.h>
#include <Ecore_File.h>
#include <Ecore_Evas.h>
#include <Efreet_Mime.h>
#include <Edje.h>


Evas *evas;
Evas_Object *orig_image;
Evas_Object *image;
bool dither = false;

Eina_List *filelist;
Eina_List *cur_file;


int winwidth = 600;
int winheight = 800;


void quit();


void exit_all(void *param) { ecore_main_loop_quit(); }

void quit() { ecore_main_loop_quit(); }

static void die(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

static void main_win_resize_handler(Ecore_Evas* main_win)
{
    int w, h;
    Evas *canvas = ecore_evas_get(main_win);
    evas_output_size_get(canvas, &w, &h);

    winwidth = w;
    winheight = h;

    Evas_Object *bg = evas_object_name_find(canvas, "background");
    evas_object_resize(bg, w, h);
    evas_object_image_load_size_set(orig_image, w, h);

    //render_cur_image();

    Evas_Object* rr = evas_object_name_find(evas, "help-window");
    evas_object_resize(rr, w, h);
}

static void main_win_delete_handler(Ecore_Evas* main_win)
{
    ecore_main_loop_quit();
}

static void page_updated_handler(Evas_Object* tb,
        int cur_page,
        int total_pages,
        const char* header,
        void* param)
{
    Evas_Object* rr = evas_object_name_find(evas, "help-window");
    //choicebox_aux_edje_footer_handler(rr, "footer", cur_page, total_pages);
}


int main(int argc, char *argv[])
{
//    if (argc == 1) {
//        fprintf(stderr,"no image file given!\n");
//        return 1;
//    }

    Ecore_Evas *ee;
    Evas_Object *bg; //background

    /* initialize efl libraries */
    if(!evas_init())
        die("Unable to initialize Evas\n");
    if(!ecore_init())
        die("Unable to initialize Ecore\n");
    if(!ecore_evas_init())
        die("Unable to initialize Evas\n");
    if(!edje_init())
        die("Unable to initialize Edje\n");

//    setlocale(LC_ALL, "");
//    textdomain("ohoh");

    ecore_x_io_error_handler_set(exit_all, NULL);

    ee = ecore_evas_software_x11_new(0, 0, 0, 0, 600, 800);

    ecore_evas_borderless_set(ee, 0);
    ecore_evas_shaped_set(ee, 0);
    ecore_evas_title_set(ee, "mindcraft");
    ecore_evas_show(ee);

    ecore_evas_callback_resize_set(ee, main_win_resize_handler);
    ecore_evas_callback_delete_request_set(ee, main_win_delete_handler);

    /* get a pointer our new Evas canvas */
    evas = ecore_evas_get(ee);

    /* create our white background */
    bg = evas_object_rectangle_add(evas);
    evas_object_color_set(bg, 255, 255, 255, 255);
    evas_object_move(bg, 0, 0);
    evas_object_resize(bg, 600, 800);
    evas_object_name_set(bg, "background");
    evas_object_focus_set(bg, 1);
//    evas_object_event_callback_add(bg, EVAS_CALLBACK_KEY_UP, &key_handler, NULL);
    evas_object_show(bg);

    orig_image = evas_object_image_filled_add(evas);
    evas_object_name_set(orig_image, "orig_image");
    image = evas_object_image_filled_add(evas);
    evas_object_name_set(image, "image");
    evas_object_image_smooth_scale_set(image, EINA_TRUE);
    evas_object_image_smooth_scale_set(orig_image, EINA_TRUE);
    evas_object_image_load_size_set(orig_image, 600, 800);

//    read_keymap(operations);
//    init_filelist(argv[1]);
//    render_cur_image();

    /* start the main event loop */
    ecore_main_loop_begin();

    evas_object_del(image);
    evas_object_del(bg);

    edje_shutdown();
    ecore_evas_shutdown();
    ecore_shutdown();
    evas_shutdown();

    return 0;
}
