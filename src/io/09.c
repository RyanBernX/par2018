#include <rpc/rpc.h>
#include <cmath>
#include <iostream>

int main(int argc, char * argv[])
{
  FILE * fp = fopen("tmp.dat", "rb");
  XDR xdrs;
  xdrstdio_create(&xdrs, fp, XDR_DECODE);
  for (u_int i = 0;i < 1024;++ i) {
    double buffer;
    xdr_double(&xdrs, &buffer);
    if (fabs(buffer - i*i) > 1.0e-15) {
      std::cout << i << "-th read is incorrect!" 
                << std::endl;
    }
  }
  xdr_destroy(&xdrs);
  fclose(fp);

  return 0;
}
