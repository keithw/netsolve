#include <stdexcept>

#include "flow.hh"

using namespace std;

Flow Flow::add_hop( const string & hop ) const
{
  Flow ret = *this;
  ret.route_ += "->" + hop;
  return ret;
}

Flow Flow::scale( const double delivery_proportion ) const
{
  if ( delivery_proportion < 0 or delivery_proportion > 1 ) {
    throw invalid_argument( "delivery proportion = " + to_string( delivery_proportion ) );
  }

  return Flow( source_, destination_, rate_ * delivery_proportion );
}
