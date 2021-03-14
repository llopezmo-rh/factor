// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>


// find_divisor stores on "divisor" the first divisor of "n" found different to 1.
// If none is found, it will set "divisor" to 1.
// It returns 0 (false) if no divisor different to 1 has been found. Otherwise 1.
// "divisor" must have been initialized before calling the function
int find_divisor(mpz_t divisor, const mpz_t n)
     {
     mpz_t i, last_divisor;
     if (mpz_cmp_ui(n, 2) == 0)
          {       
          mpz_set_ui(divisor, 1);
          return 0;
          }
     if (mpz_even_p(n))
          {
          mpz_set_ui(divisor, 2);
          return 1;
          }     
     mpz_set_ui(divisor, 1);
     mpz_init_set_ui(i, 3);
     mpz_init(last_divisor);
     mpz_cdiv_q_ui(last_divisor, n, 2);
     while (mpz_cmp(i, last_divisor) <= 0)
          {
          if (mpz_divisible_p(n, i))
               {
               mpz_set(divisor, i);
               break;
               }
          else
               {
               mpz_cdiv_q(last_divisor, n, i);
               mpz_add_ui(i, i, 2);
               }
          }
     mpz_clear(i);
     mpz_clear(last_divisor);
     return mpz_cmp_ui(divisor, 1) > 0;
	}
      
int main(int argc, char *argv[])
    {
    // Variable to store the exponent to be printed
    unsigned long long int exp;
    
    // Variable to keep the number whose divisor will be searched for. 
    // Initialized to the CLI argument
    mpz_t dividend;
    
    // When a divisor of "dividend" is found, it is stored on "divisor"
    mpz_t divisor;
    
    // Variable to keep the divisor found on the previous iteraction of the 
    // loop. When a new divisor is found and stored in "divisor", it will be 
    // compared with "previous divisor". If both values are the same, "exp" 
    // will be increased. Otherwise, "previous_divisor" will be printed with 
    // its corresponding exponent ("exp"), the value of "previous_divisor" 
    // will be overwritten with the one of "divisor" and "exp" will be 
    // reinitialized to 1.
    mpz_t previous_divisor;
    
    // This is just a variable to convert mpz_t values to strings to be 
    // printed. Alternatively, the following function is being used too:
    // size_t mpz_out_str (FILE *stream, int base, const mpz_t op)
    char* mpz_string;
    
    // Removing buffer in order to send output to stdout immediately
    setbuf(stdout, NULL);
    
    // Initializing and validating variables
    if (mpz_init_set_str(dividend, argv[1], 10) == -1)
        {
        fprintf(stderr, "Fatal error: \"%s\" does not seem to be a correct "
            "integer value\n", argv[1]);
        return 1;
        }
    if (mpz_cmp_ui(dividend, 1) < 0)
        {
        fprintf(stderr, "Fatal error: only values greater than 0 can be "
            "factored\n");
        return 1;
        }
    // According to GNU MP documentation, output strings on mpz_get_str should 
    // be allocated automatically by the function if it was initilized 
    // previously to NULL, but according to my tests this is not happening. 
    // Therefore, it is being initialized on the line below as the CLI 
    // argument. The factors of a number can never be longer than the number 
    // to be factored
    mpz_string = (char*)malloc(strlen(argv[1]) * sizeof(char));
    mpz_init_set_ui(previous_divisor, 0);
    mpz_init(divisor);
    
    // Printing CLI argument, which will be the number to be factored
    printf("%s = ", argv[1]);
    
    // Loop where factors will be found and printed
    for(;;)
        {
        find_divisor(divisor, dividend);
        if (mpz_cmp_ui(divisor, 1) > 0)
            {
            if (mpz_cmp(previous_divisor, divisor) != 0)
                {
                if (mpz_cmp_ui(previous_divisor, 0) != 0)
                    {
                    mpz_get_str(mpz_string, 10, previous_divisor);
                    if (exp == 1)
                        printf("%s * ", mpz_string);
                    else
                        printf("(%s^%llu) * ", mpz_string, exp);
                    }
                mpz_set(previous_divisor, divisor);
                exp = 1;
                }
            else
                exp++;
            mpz_cdiv_q(dividend, dividend, divisor);
            }
        else
            {
            if (mpz_cmp(dividend, previous_divisor) == 0)
                {
                mpz_get_str(mpz_string, 10, dividend);
                printf("(%s^%llu)", mpz_string, ++exp);
                }
            else
                {
                if (mpz_cmp_ui(previous_divisor, 0) != 0)
                    {       
                    mpz_get_str(mpz_string, 10, previous_divisor);       
                    if (exp == 1)
                        printf("%s * ", mpz_string);
                    else
                        printf("(%s^%llu) * ", mpz_string, exp);
                    }
                mpz_out_str(stdout, 10, dividend);
                }
            printf("\n");
            // As a prime factor has been found, the loop has to be finished
            break;
            }
        }
    }
                    
    

