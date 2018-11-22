#include <stdio.h>
#include <fftw3.h>

#define N 16
#define nthreads 4

void print_fftw_complex_array(fftw_complex *data, int numel);

int main()
{
    int i;
    fftw_complex *in, *out;
    fftw_plan p, p1;

    fftw_init_threads();
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    fftw_plan_with_nthreads(nthreads);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    p1 = fftw_plan_dft_1d(N, out, in, FFTW_BACKWARD, FFTW_ESTIMATE);

    if ((in == NULL) || (out == NULL))
    {
        printf("Error: insufficient available memory\n");
        return 1;
    }
    
    for (i = 0; i < N; i++)
    {
        in[i][0] = i + 1;
        in[i][1] = 0;
    }

    fftw_execute(p);

    print_fftw_complex_array(in, N);
    printf("\n");
    print_fftw_complex_array(out, N);

    fftw_execute(p1);

    fftw_destroy_plan(p);
    fftw_destroy_plan(p1);

    printf("\n");

    print_fftw_complex_array(in, N);

    fftw_free(in);
    fftw_free(out);

    return 0;
}

void print_fftw_complex_array(fftw_complex *data, int numel){
    for (int i = 0; i < numel; ++i)
        printf("(%f,%f)\n", data[i][0], data[i][1]);
}
