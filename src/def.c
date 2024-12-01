#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "def.h"

/* 
    Functions to define custom routes;
    simply create pages (html files) and define the names
    in the export_routesfunction's argument array;

    e.g. char* routes[] = ["index", "about", "bio"];
*/

char* define_routes(const char* routes[], int count)
{
    printf("Placeholder\n");
}

char* export_routes()
{   
    // Amount of routes
    const char* routes[] = {"index", "about"};
    int array_size = sizeof(routes)/sizeof(routes[0]);
    define_routes(routes, array_size);
}