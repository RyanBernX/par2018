#include <rpc/rpc.h>
#include <cmath>
#include <iostream>

int main(int argc, char * argv[])
{
  FILE * fp = fopen("tmp.dat", "wb");
  XDR xdrs;
  xdrstdio_create(&xdrs, fp, XDR_ENCODE);
  for (u_int i = 0;i < 1024;++ i) {
    double buffer = i*i;
    xdr_double(&xdrs, &buffer);
  }
  xdr_destroy(&xdrs);
  fclose(fp);

  return 0;
}
