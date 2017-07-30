#include "node.hh"

using namespace std;

bool Node::flow_transits_node( const Flow & flow ) const
{
  return flow.destination() != name_;
}

float Node::total_transiting_inputs() const
{
  float ret = 0.0;
  for ( const auto & x : inputs_ ) {
    if ( flow_transits_node( x.second ) ) {
      ret += x.second.rate();
    }
  }
  return ret;
}

float Node::delivery_proportion() const
{
  return output_rate() / total_transiting_inputs();
}

float Node::output_rate() const
{
  return min( total_transiting_inputs(), output_capacity_ );
}

void Node::add_flow( const Flow & flow )
{
  inputs_.insert_or_assign( flow.source(), flow.add_hop( name_ ) );
}

void Node::connect( Node & next_hop ) const
{
  const float p = delivery_proportion();

  for ( const auto & x : inputs_ ) {
    if ( flow_transits_node( x.second ) ) {
      next_hop.add_flow( x.second.scale( p ) );
    }
  }
}

const Flow & Node::terminal_flow( const std::string & source ) const
{
  const Flow & ret = inputs_.at( source );

  if ( flow_transits_node( inputs_.at( source ) ) ) {
    throw runtime_error( "Flow from " + source + " does not terminate at node " + name_ );
  }

  return ret;
}
