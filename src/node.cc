#include "node.hh"

using namespace std;

double Node::total_transiting_inputs() const
{
  double ret = 0.0;
  for ( const auto & x : inputs_ ) {
    if ( x.second.destination() != name_ ) {
      ret += x.second.rate();
    }
  }
  return ret;
}

double Node::delivery_proportion() const
{
  return output_rate() / total_transiting_inputs();
}

double Node::output_rate() const
{
  return min( total_transiting_inputs(), output_capacity_ );
}
