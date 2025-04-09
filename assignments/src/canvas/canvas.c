#include <malloc.h>
#include "canvas.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

static char *unsigned_int_to_char_array (unsigned int);

Canvas
canvas (unsigned int w, unsigned int h)
{
  Canvas c;
  c.width = w;
  c.height = h;
  c.channels = 3;
  c.pixels = calloc (c.width * c.height * c.channels, sizeof (unsigned int));

  return c;
}

Canvas
write_pixel (Canvas ca, unsigned int x, unsigned int y, Color co)
{

  unsigned int index = (y * ca.width + x) * ca.channels;

  ca.pixels[index] = (unsigned int) (round (co.red * 255));
  ca.pixels[index + 1] = (unsigned int) (round (co.green * 255));
  ca.pixels[index + 2] = (unsigned int) (round (co.blue * 255));


  return ca;
}

Color
pixel_at (Canvas ca, unsigned int x, unsigned int y)
{

  unsigned int index = ca.channels * (y * ca.width + x);
  Color color = {.red = ca.pixels[index],.green = ca.pixels[index + 1],.blue =
      ca.pixels[index + 2] };
  return color;
}

char *
canvas_to_ppm (Canvas ca)
{
  const unsigned int HEADER_MAX_LEN = 100;
  char header[HEADER_MAX_LEN];
  sprintf (header, "P3\n%d %d\n255\n", ca.width, ca.height);

  unsigned int max_pixels_length = ca.width * ca.height * 15 + ca.height * 2;
  char *ppm = calloc (strlen (header) + max_pixels_length, sizeof (char));


  strcpy (ppm, header);

  char line[70] = "";
  int line_length = 0;

  for (int y = 0; y < ca.height; y++)
    {
      for (int x = 0; x < ca.width; x++)
	{
	  int index = y * ca.width * ca.channels + x * ca.channels;
	  char pixel[16];
	  snprintf (pixel, sizeof (pixel), "%u %u %u ", ca.pixels[index],
		    ca.pixels[index + 1], ca.pixels[index + 2]);

	  if (line_length + strlen (pixel) < sizeof (line) - 1)
	    {
	      strcat (line, pixel);
	      line_length += strlen (pixel);
	    }
	  else
	    {
	      line[strlen (line) - 1] = '\n';
	      strcat (ppm, line);
	      line[0] = '\0';
	      strcpy (line, pixel);
	      line_length = strlen (pixel);
	    }
	}
      if (strlen (line) > 0)
	{
	  line[strlen (line) - 1] = '\n';
	  strcat (ppm, line);
	  line[0] = '\0';
	  line_length = 0;
	}
    }

  int len = strlen (ppm);
  if (ppm[len - 1] != '\n')
    {
      ppm[len] = '\n';
      ppm[len + 1] = '\0';
    }

  return ppm;
}