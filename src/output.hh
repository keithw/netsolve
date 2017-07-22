#pragma once

class Multiplex;

class Output
{
private:
  double capacity_;

public:
  Output( const double capacity )
    : capacity_( capacity )
  {}

  double capacity() const { return capacity_; }
  double delivery_proportion( const Multiplex & multiplex ) const;
  double output_rate( const Multiplex & multiplex ) const;
};
