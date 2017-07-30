#pragma once

#include <unordered_map>

#include "flow.hh"

class Node
{
private:
  std::string name_;
  std::unordered_map<std::string, Flow> inputs_ {};
  double output_capacity_;

public:
  Node( const std::string & name,
	  const double output_capacity )
    : name_( name ),
      output_capacity_( output_capacity )
  {}

  double output_capacity() const { return output_capacity_; }

  double total_transiting_inputs() const;
  double delivery_proportion() const;
  double output_rate() const;
};
