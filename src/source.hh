#pragma once

class Source
{
private:
  double rate_;

public:
  Source( const double rate )
    : rate_( rate )
  {}

  double rate() const { return rate_; }
  void set_rate( const double rate ) { rate_ = rate; }
};
