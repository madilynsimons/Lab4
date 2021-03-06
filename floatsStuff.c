#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <float.h>

#define NORM 0
#define DNORM 1
#define SPEC 2
#define BIAS 127


/*
    Declare a "typedef struct {} flt;" to contain data for a float
    The data should include:
        An integer for the sign
            (1 for positive, -1 for negative)
        An integer for the exponent value
            (should be bit value to integer minus BIAS or -126 for denormalized)
        A float to contain the value of the mantissa
            (Actual float value extracted from the binary value)
        An integer to contain the mode using the defines above
            (NORM, DNORM and SPEC)
    Example after processing: -15.375 = 1 10000010 11101100000000000000000
        sign = -1
        exp = 3
        man = 0.9218750000
        mode = NORM
*/

typedef struct{
   int sign;
   int exponent;
   float mantissa;
   int mode;
} flt;

/*
    Write a function get_flt_bits_int to return an integer with the
    bits copied from a float.
    Example:
        for f = -15.375,
        the bits of int n = 11000001011101100000000000000000
    Look at the slides and code from the float lectures and use the
    indirection trick.  This can easily be done in one line of code.
    The function should accept a float and return an int.
*/
int get_flt_bits_int(float f)
{
	int n = *(int*)&f;
	return n;
}


/*
    Write a function that returns the sign of a float as a char.
    You should call get_flt_bits_int to get the bits in an int
    and return '1' if the sign is negative else return '0'.  The
    function should accept a float and return a string.
*/
char get_flt_sign_char(float f)
{
	int to_int = get_flt_bits_int(f);
	if(to_int & INT_MIN) return '1';
	else return '0';
}


/*
    Write a function that returns the sign of a float as an integer.
    You should call get_flt_bits_int to get the bits in an int
    and return -1 if the sign is negative else return 1.  The function
    should accept a float and return an int.
*/
int get_flt_sign_val(float f)
{
	int to_int = get_flt_bits_int(f);
	if(to_int & INT_MIN) return -1;
	else return 1;
}


/*
    Write a function to return a string containing the
    actual binary value of the exponent of a float in a
    char array.  You should call get_flt_bits_int to get
    the bits in an int and return the string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are "10000010"
    The function should accept a float and return a string.
*/
char* get_flt_exp_str(float f)
{
	int to_int = get_flt_bits_int(f);

	char *exponent = (char*) malloc((8+1)*sizeof(char));
	int i;

	to_int = to_int >> 23;

	for(i = 7; i >= 0; i--)
	{
		exponent[i] = (to_int & 1) + '0';
		to_int = to_int >> 1;
	}
	exponent[8] = '\0';

	return exponent;
}



/*
    Write a function to return an integer containing the
    actual integer value of the exponent of a float.  You
    should call get_flt_bits_int to get the bits in an int
    and return the int with the exponent value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are 10000010
            the actual value of the exponent is 3
    The function should accept a float and return an int.
*/
int get_flt_exp_val(float f)
{
	int to_int = get_flt_bits_int(f);

	int exponent = -1 * BIAS;
	int i;
	int multiplier = 1;

	to_int = to_int >> 23;

	for(i = 0; i < 8; i++)
	{
		exponent += (to_int & 1) * multiplier;
		to_int = to_int >> 1;
		multiplier *= 2;
	}

	if(exponent == 128) exponent = 255; // specialized
	return exponent; // normalized or denormalized or zero
}


/*
    Write a function to return an integer containing the
    mode of the exponent of a float.  You should call
    get_flt_exp_val to get the bits in an int and return
    the int with the mode value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are 10000010
            the mode is NORM
    The function should accept a float and return an int.
*/
int get_flt_exp_mode(float f)
{	// if all 1s, specialized
	// if all 0s, denormalized
	// else normalized

	int exp = get_flt_exp_val(f);

	if(exp == -127)
	{
		if(f == 0) return SPEC;
		return DNORM;
	}else if(exp == 255)
	{
		return SPEC;
	}else{
		return NORM;
	}
}



/*
    Write a function to return a string containing the
    actual binary value of the mantissa of a float in a
    char array.  You should call get_flt_bits_int to get
    the bits in an int and return the string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the mantissa bits are "11101100000000000000000"
    The function should accept a float and return a string.
*/
char* get_flt_man_str(float f)
{
	int to_int = get_flt_bits_int(f);

	char *mantissa = (char*) malloc((23+1)*sizeof(char));
	int i;

	for(i = 22; i >= 0; i--)
	{
		mantissa[i] = (to_int & 1) + '0';
		to_int = to_int >> 1;
	}
	mantissa[23] = '\0';

	return mantissa;
}



/*
    Write a function to return a float containing the
    actual float value of the mantissa of a float.  You
    should call get_flt_bits_int to get the bits in an int
    and return the int with the mantissa value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the mantissa bits are 11101100000000000000000
            the actual value of the mantissa is 0.9218750000
    The function should accept a float and return an int.
*/
float get_flt_man_val(float f)
{
	float out = 0;
	int i;
	int to_int = get_flt_bits_int(f);
	float divider = 8388608;

	for(i = 0; i < 23; i++)
	{
		if(to_int & 1) out += (1.0 / divider);
		to_int = to_int >> 1;
		divider /= 2;
	}

	return out;
}



/*
    Write a function to return a string containing the
    actual binary value of a float in a char array.  You
    should call get_flt_sign_char, get_flt_exp_str and
    get_flt_man_str to get the bits in an char and two
    strings and return the concatenated string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            The sign is '1'
            the exponent is "10000010"
            and the mantissa bits are "11101100000000000000000"
            The string should be formatted as:
                "1 10000010 11101100000000000000000" to clearly
                separate the 3 parts.
    The function should accept a float and return a string.
*/
char* get_flt_bits_str(float f)
{
	char *binary = (char*) malloc((32+2+1)*sizeof(char));

	char  sig = get_flt_sign_char(f);
	char *exp = get_flt_exp_str(f);
	char *man = get_flt_man_str(f);

	int bin_it = 0;
	binary[bin_it++] = sig;
	binary[bin_it++] = ' ';

	int i;
	for(i = 0; exp[i] != '\0'; i++)
	{
		binary[bin_it++] = exp[i];
	}

	binary[bin_it++] = ' ';

	for(i = 0; man[i] != '\0'; i++)
	{
		binary[bin_it++] = man[i];
	}

	binary[bin_it] = '\0';

	free(exp);
	free(man);

	return binary;

}



/*
    Write a function to separate the parts of a float
    into a flt struct as described above.  You should
    call get_flt_sign_val, get_flt_exp_mode,
    get_flt_exp_val and get_flt_man_val.
    Hint:  make sure to set exponent to -126 for
    DNORM mode.
*/
flt get_flt_val_flt(float f)
{
	flt f_struct;
	f_struct.sign = get_flt_sign_val(f);
    f_struct.exponent = get_flt_exp_val(f);
    f_struct.mantissa = get_flt_man_val(f);
    f_struct.mode = get_flt_exp_mode(f);
	return f_struct;
}



/*
    Write a function to print a flt struct to screen.
    It should accept a flt struct and return nothing.
    Hint: Use if statement to print mode.
*/
void print_flt(flt f)
{
	printf("sign = %d\n", f.sign);
	printf("exp = %d\n", f.exponent);
	printf("man = %f\n", f.mantissa);
	printf("mode = ");
	switch(f.mode){
		case NORM:
			printf("normalized\n");
		break;
		case DNORM:
			printf("denormalized\n");
		break;
		case SPEC:
			printf("specialized\n");
		break;
	}
	return;
}



/*
    Write a function to get the actual float value back
    out of a flt struct.
    Hints:
        The float value produced will depend on the mode.
        To set a float to infinity use the math library constant INFINITY
        To set a float to not-a-number use the math library constant NAN
        Check the slides and text for conditions for NORN, DNORM and SPEC
        You need to return (sign) * M * 2^e
*/
float get_flt_bits_val(flt f_struct)
{
	float f;
	int sign = f_struct.sign;
	int exponent = f_struct.exponent;
	float mantissa = f_struct.mantissa;

	switch(f_struct.mode)
	{
		case SPEC:
		// infinity, negative infinity, zero, or not a number
			if(exponent == -127)
			{
				if(sign) return -0.0;
				else return 0.0;
			}
			if(mantissa == 0)
			{
				if(sign)
				{
					// negative infinity
					return -1 * INFINITY;
				}
				else
				{
					// infinity
					return INFINITY;
				}
			}else
			{
				if(sign) return (0.0/0.0); //-nan
				return NAN; //nan
			}

		break;
		case DNORM:
		    f = sign * mantissa * pow(2, exponent);
		break;
		case NORM:
			f = sign * (1+mantissa) * pow(2, exponent);
		break;
	}

	return f;
}



/*
    Write a main function that calls an prints results for
    each function when completed.
    get_flt_sign_char
    get_flt_sign_val

    get_flt_exp_str
    get_flt_exp_val

    get_flt_man_str
    get_flt_man_val

    get_flt_bits_str

    get_flt_val_flt
    print_flt

    get_flt_bits_val
*/
int main(){


	float f = -15.375000;

	printf("f = %f\n\n", f);

	char sig = get_flt_sign_char(f);
	int s = get_flt_sign_val(f);

	printf("sig = %c\n", sig);
	printf("s = %d\n\n", s);

	char* exp = get_flt_exp_str(f);
	int e = get_flt_exp_val(f);

	printf("exp = %s\n", exp);
	printf("e = %d\n\n", e);
	free(exp);

	char* man = get_flt_man_str(f);
	float m = get_flt_man_val(f);

	printf("man = %s\n", man);
	printf("m = %f\n\n", m);
	free(man);

	char* bits = get_flt_bits_str(f);

	printf("bits = %s\n\n", bits);
	free(bits);

	flt f_struct = get_flt_val_flt(f);
	print_flt(f_struct);
	printf("\n");

	float ff = get_flt_bits_val(f_struct);
	printf("ff = %f\n", ff);
    return 0;
}
