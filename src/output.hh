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

  double delivery_rate( const Multiplex & multiplex ) const;
};
