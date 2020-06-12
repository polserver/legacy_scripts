/*
SGPC: Simple Genetic Programming in C
(c) 1993 by Walter Alden Tackett and Aviram Carmi
 
 This code and documentation is copyrighted and is not in the public domain.
 All rights reserved. 
 
 - This notice may not be removed or altered.
 
 - You may not try to make money by distributing the package or by using the
   process that the code creates.
 
 - You may not distribute modified versions without clearly documenting your
   changes and notifying the principal author.
 
 - The origin of this software must not be misrepresented, either by
   explicit claim or by omission.  Since few users ever read sources,
   credits must appear in the documentation.
 
 - Altered versions must be plainly marked as such, and must not be
   misrepresented as being the original software.  Since few users ever read
   sources, credits must appear in the documentation.
 
 - The authors are not responsible for the consequences of use of this 
   software, no matter how awful, even if they arise from flaws in it.
 
If you make changes to the code, or have suggestions for changes,
let us know!  (gpc@ipld01.hac.com)
*/

/*
 *
 * $Log: random.cpp,v $
 * Revision 1.1  2003/01/02 16:45:48  syzygy
 * Changed C files to CPP
 *
 * Revision 1.1  2002/10/10 10:00:28  syzygy
 * Transferred from VSS
 *
 * Revision 1.1  2002/10/09 14:31:29  syzygy
 * transferred from VSS
 *
 * Revision 2.9  1993/04/22  07:39:12  gpc-avc
 * Removed old log messages
 *
 * Revision 2.8  1993/04/14  04:53:06  gpc-avc
 * Finished modes for checkpoining
 *
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include <math.h>
#include "random.h"

int 	gaussian_noise_toggle;
float 	gaussian_noise_uniform1, gaussian_noise_uniform2;
float	gaussian_noise_temp;


#define M_PI 3.14159265358979323846 

/* definitions for the random number generator (seed * GEN % MOD) */
#define GEN 16807
#define MOD 2147483647
#define QUO 127773
#define RES 2836

static long	seed;

void set_seed(
  unsigned long s
  )
{
  seed = (long)s;
}

unsigned long get_seed(void)
{
  return seed;
}

float park_miller_randomizer(void)
{
  float retval;

  retval = (float) (((double) PMrand(&seed)) / ((double) MOD));
  return retval;
}

/* Park-Miller "minimal standard" pseudo random number generator */
/* Implementation by Jan Jannink (c) 1992 */

long PMrand(
	long *s
	)
{
  long   tmp;

  tmp = (*s >> 16) * GEN;
  *s = (*s & 65535) * GEN + ((tmp & 32767) << 16) + (tmp >> 15);
  return (*s -= (*s < 0 ? MOD: 0));
}

float gaussian_noise(
  float mean,
  float sigma
  )
{
  float		gauss;

  if (gaussian_noise_toggle) {
    gaussian_noise_uniform1 = (random_float(1.0));
    gaussian_noise_uniform2 = (random_float(1.0));
    gaussian_noise_temp = (float)sqrt(-2.0*log((double)gaussian_noise_uniform1));
    gauss = gaussian_noise_temp * 
      (float)cos((double)(2.0*M_PI*gaussian_noise_uniform2));
  }
  else {
    gauss = gaussian_noise_temp * 
      (float)sin((double)(2.0*M_PI*gaussian_noise_uniform2));
  }
  gaussian_noise_toggle = ! gaussian_noise_toggle;
  return mean + (sigma * gauss);
}

float random_float( float f )   // 0 <=  random_float() < f 
{
  float	retval;

  retval = (float)(f*(1.0-park_miller_randomizer()));
  return retval;
}


int random_int( int i ) //  0 <= random_int() < i 
{
  int	retval;

  retval = (int) (i * random_float( 1.0 ));
  if (retval >= i)
  {
        fprintf( stderr, "Random number range exceeded, blech (i=%d, retval=%d)\n", i, retval );
        
        retval = i-1;
  }
  return retval;
}

