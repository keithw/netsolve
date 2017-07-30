#include "router.hh"

using namespace std;

double Router::total_input() const
{
  double ret = 0.0;
  for ( const auto & x : inputs_ ) {
    ret += x.second.rate();
  }
  return ret;
}

double Router::delivery_proportion() const
{
  return output_rate() / total_input();
}

double Router::output_rate() const
{
  return min( total_input(), output_capacity_ );
}
