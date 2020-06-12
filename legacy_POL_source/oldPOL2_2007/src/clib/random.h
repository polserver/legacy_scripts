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

#ifndef _RANDOM_H_
#define _RANDOM_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int 	gaussian_noise_toggle;
extern float 	gaussian_noise_uniform1, gaussian_noise_uniform2;
extern float	gaussian_noise_temp;

/* random.c */

float random_float ( float f );

int random_int ( int i );

void set_seed ( unsigned long s );

unsigned long get_seed (void);

float park_miller_randomizer (void);

long PMrand ( long *s );

float gaussian_noise (   float mean,    float sigma );

#ifdef __cplusplus
}
#endif

#endif
