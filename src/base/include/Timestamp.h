#pragma once

#include <boost/operators.hpp>

#include "Macros.h"
#include "Types.h"

namespace zhy {
class Timestamp : public boost::equality_comparable<Timestamp>,
                  public boost::less_than_comparable<Timestamp> {
 public:
  Timestamp();
  explicit Timestamp(int64_t microSecondsSinceEpoch);
  ~Timestamp();

  void Swap(Timestamp& rhs) {
    std::swap(micro_seconds_since_epoch_, rhs.micro_seconds_since_epoch_);
  }
  bool Valid() const { return micro_seconds_since_epoch_ > 0; }
  std::string ToString() const;
  std::string ToFormattedString(bool showMicroseconds = true) const;

  int64_t MicroSecondsSinceEpoch() const { return micro_seconds_since_epoch_; }
  time_t SecondsSinceEpoch() const {
    return static_cast<time_t>(micro_seconds_since_epoch_ /
                               kMicroSecondsPerSecond);
  }

  static Timestamp Now();
  static Timestamp InValid() { return Timestamp(); }
  static Timestamp FromUnixTime(time_t t) { return FromUnixTime(t, 0); }
  static Timestamp FromUnixTime(time_t t, int microSeconds) {
    return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond +
                     microSeconds);
  }
  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t micro_seconds_since_epoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs) {
  return lhs.MicroSecondsSinceEpoch() < rhs.MicroSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs) {
  return lhs.MicroSecondsSinceEpoch() == rhs.MicroSecondsSinceEpoch();
}

inline double TimeDifference(Timestamp high, Timestamp low) {
  int64_t diff = high.MicroSecondsSinceEpoch() - low.MicroSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp AddTime(Timestamp timestamp, double seconds) {
  int64_t delta =
      static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.MicroSecondsSinceEpoch() + delta);
}
}  // namespace zhy
