#include <cstdio>

#include "output.hh"
#include "multiplex.hh"
#include "source.hh"
#include "parking_lot.hh"

int main()
{
  Multiplex sources { 1 };
  Output link { 10.0 };

  for ( double sending_rate = 0; sending_rate < 30.0; sending_rate += 0.5 ) {
    sources.set_rate( 0, sending_rate );
    printf( "Sending at rate %.2f, output is %.2f (delivering %.0f%% of input).\n",
	    sending_rate, link.output_rate( sources ), 100 * link.delivery_proportion( sources ) );
  }

  return 0;
}
