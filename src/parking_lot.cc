#include "parking_lot.hh"

using namespace std;

tuple<double, double, double> ParkingLot::throughputs( const double A_rate,
						       const double B_rate,
						       const double C_rate )
{
  A.set_rate( A_rate );
  B.set_rate( B_rate );
  C.set_rate( C_rate );

  x.add_flow( A );
  y.add_flow( B );
  x.add_flow( C );

  x.connect( y );
  y.connect( z );

  return { y.terminal_flow( "A" ).rate(),
           z.terminal_flow( "B" ).rate(),
           z.terminal_flow( "C" ).rate() };
}
