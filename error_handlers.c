#include "allegro_common.h"
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "error_handlers.h"

// Error handlers global variable definitions
char temp_error_buf[256];

void fatal_error(const char *msg)
{
   char temp_buf[512];

   if (datafile != NULL)
      unload_datafile(datafile);

   sprintf(temp_buf, "\nERROR: %s", msg);
   strcat(temp_buf, "\n\nPlease contact ScanTool.net via our website at http://www.ScanTool.net/support");
   strcat(temp_buf, "\nInclude the following information:");
   strcat(temp_buf, "\n\t- Exact error message");
   strcat(temp_buf, "\n\t- CPU type/speed, i.e. \"Pentium 100Mhz\"");
   strcat(temp_buf, "\n\t- OS, i.e. \"Windows 95\"");
   strcat(temp_buf, "\n\t- Total amount of RAM installed, i.e. \"4Mb\"\n\n");
   scantool_message(temp_buf);
   
   exit(EXIT_FAILURE);
}
