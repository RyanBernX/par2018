#include <stdio.h>
#include <fftw3.h>

#define N 8
#define nthreads 4

int main()
{
	int i;
	fftw_init_threads();
	fftw_complex *in, *out;
	fftw_plan p, p1;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

	fftw_plan_with_nthreads( nthreads );
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	p1 = fftw_plan_dft_1d(N, out, in, FFTW_BACKWARD, FFTW_ESTIMATE);

	if ((in == NULL) || (out == NULL))
	{
		printf("Error: insufficient available memory\n");
	}
	else
	{
		for (i = 0; i < N; i++)
		{
			in[i][0] = i + 1;
			in[i][1] = 0;
		}
	}

	fftw_execute(p);

	for (i = 0; i < N; i++)
	{
		printf("%f, %fi\n", in[i][0], in[i][1]);
	}
	printf("\n");

	for (i = 0; i < N; i++)
	{
		printf("%f, %fi\n", out[i][0], out[i][1]);
	}


	fftw_execute(p1);

	fftw_destroy_plan(p);
	fftw_destroy_plan(p1);

	printf("\n");

	for (i = 0; i < N; i++)
	{
		printf("%f, %fi\n", in[i][0], in[i][1]);
	}
	printf("\n");

	for (i = 0; i < N; i++)
	{
		printf("%f, %fi\n", out[i][0], out[i][1]);
	}

	if ( in != NULL) fftw_free(in);
	if ( in != NULL) fftw_free(out);

	return 0;
}



