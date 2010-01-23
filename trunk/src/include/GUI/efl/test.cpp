#include <stdlib.h>
#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Edje.h>

//The pointer to a canvas wrapper
Ecore_Evas *ecore_evas = NULL;
//The pointer to an Evas canvas
Evas *evas = NULL;
//The pointer to a given Edje object
Evas_Object *edje = NULL;
//Width and height for resizing Evas/Edje objects
Evas_Coord width, height;

int main() 
{
    //Control that the libraries are properly initialized
    if (!ecore_init()) 
    {
	printf("ecore init failed\n");
	return EXIT_FAILURE;
    }
    if (!ecore_evas_init()) 
    {
	printf("ecore_evas init failed\n");
        return EXIT_FAILURE;
    }
    if (!edje_init()) 
    {
	printf("edje init failed\n");
	return EXIT_FAILURE;
    }

    //Check the canvas wrapper (800x600 X11 window) is created correctly
    ecore_evas = ecore_evas_software_x11_new(NULL, 0, 0, 0, 800, 600);
    if (!ecore_evas) 
    {
	printf("edje canvas wrapper failed\n");
	return EXIT_FAILURE;
    }
    
    //We set some window attributes and make the wrapper visible
    ecore_evas_title_set(ecore_evas, "Example Application");
    ecore_evas_name_class_set(ecore_evas, "testapp", "Testapp");
    ecore_evas_show(ecore_evas);
    
    //Get the pointer to the canvas and add tan object
    evas = ecore_evas_get(ecore_evas);
    edje = edje_object_add(evas);
    edje_object_file_set(edje, "testfile.edj", "testgroup");
    
    //Setting the object and canvas to the minimal size of the object
    evas_object_move(edje, 0, 0);
    edje_object_size_min_get(edje, &width, &height);
    evas_object_resize(edje, width, height);
    ecore_evas_resize(ecore_evas, width, height);
    evas_object_show(edje);
    
    //Starting the main application loop
    ecore_main_loop_begin();
    
    //Once the window is closed (ending the loop) we clean up our mess
    ecore_evas_shutdown();
    ecore_shutdown();
    edje_shutdown();
}
                                                                                                             