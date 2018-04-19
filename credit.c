#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    long long n;

     do
    {
        printf("Number: ");
        n = get_long_long();
    }
    while ( n < 0);

    char n_str[16];
    sprintf(n_str, "%lld", n);
    int length = strlen(n_str);
    long long nd[length+1];
    int checksum = 0;
    long long pow_i;
    int i;

    if ( length !=15 && length !=16 && length !=13)
    {
        printf("INVALID\n");
        return 0;
    }
    else
    {
        for (i = 1, pow_i = 10; (i <= length) || (pow_i < pow (10, i)); i++, pow_i = pow_i * 10)
        {
            nd[i] = (n % pow_i - n % (pow_i / 10)) / (pow_i / 10);
        }

        for (i = 1; i <= length; i++)
        {
            if ( i%2 == 0)
            {
                //multiply each of the underlined digits by 2
                if ( 2*nd[i] > 10)
                {
                    checksum += + 1 + (2*nd[i] % 10);
                }
                else if ( 2*nd[i] == 10)
                {
                    checksum += + 1;
                }
                //sum of the digits that weren’t multiplied by 2
                else
                {
                    checksum += + 2*nd[i];
                }
            }
            else
            {
                checksum += + nd[i];
            }
        }
        //printf("checksum: %i\n", checksum);

        // the last digit in that sum must be  0
        if ( checksum % 10 == 0 )
        {
            // AMEX 15 digits; start with 34,37
            if ( (length == 15) && (nd[length]==3) && (nd[length-1]==4 || nd[length-1] ==7))
            {
                printf("AMEX\n");
            }
            // MASTERCARD 16 digits; start with 51-55
            else if ( (length == 16) && (((nd[length]==5) && ( 0 < nd[length-1] || nd[length-1] < 6 )) || ((nd[length-1]==2) && ( nd[length-1]==2 ))))
            {
                printf("MASTERCARD\n");
            }
            // VISA 13 and 16 digits; start with 4
            else if ( ((length == 16) || (length == 13)) && (nd[length]==4) )
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }
        }
        else
        {
            printf("INVALID\n");
            return 0;
        }
    }
}
