#include <stdexcept>

#include "parking_lot.hh"

using namespace std;

tuple<float, float, float> ParkingLot::throughputs( const float A_rate,
						       const float B_rate,
						       const float C_rate )
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

tuple<float, float, float> ParkingLot::throughputs_shortcut( const float A_rate,
								const float B_rate,
								const float C_rate ) const
{
  const float x_transiting_input = A_rate + C_rate;
  const float x_output = min( x_transiting_input, x.output_capacity() );
  const float x_delivery_proportion = x_output / x_transiting_input;

  const float A_throughput = A_rate * x_delivery_proportion;

  const float C_rate_after_x = C_rate * x_delivery_proportion;

  const float y_transiting_input = C_rate_after_x + B_rate;
  const float y_output = min( y_transiting_input, y.output_capacity() );
  const float y_delivery_proportion = y_output / y_transiting_input;

  const float B_throughput = B_rate * y_delivery_proportion;
  const float C_throughput = C_rate_after_x * y_delivery_proportion;

  return { A_throughput, B_throughput, C_throughput };
}

tuple<float, float, float> ParkingLot::throughputs_fast( const float A_rate,
							    const float B_rate,
							    const float C_rate,
							    const unsigned int i )
{
  const auto fast_answer = throughputs_shortcut( A_rate, B_rate, C_rate );

  if ( i % 73 == 0 ) {
    const auto slow_answer = throughputs( A_rate, B_rate, C_rate );
    audit_count_++;
    if ( fast_answer != slow_answer ) {
      throw logic_error( "fast and slow throughput calculations disagree" );
    }
  }

  return fast_answer;
}
