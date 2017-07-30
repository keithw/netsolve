#pragma once

#include <unordered_map>

#include "flow.hh"

class Router
{
private:
  std::unordered_map<std::string, Flow> inputs_ {};
  double output_capacity_;

public:
  Router( const double output_capacity )
    : output_capacity_( output_capacity )
  {}

  double output_capacity() const { return output_capacity_; }

  double total_input() const;
  double delivery_proportion() const;
  double output_rate() const;
};
