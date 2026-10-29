#pragma once

#include <string>
#include <vector>

struct CliOptions {
    std::string macAddress   = "00:04:3E:53:ED:5B";
    int         samplingRate = 50;
    int         baudRate     = 921600;
    bool        verbose      = false;
};

// Parses command-line arguments into `out`.
// Returns true on success. On failure returns false and sets `error`
// to a human-readable message. Recognized form:
//   program [--verbose] [MAC_ADDRESS] [SAMPLING_RATE] [BAUD_RATE]
bool parseArgs(int argc, char* argv[], CliOptions& out, std::string& error);

struct TimingStats {
    int    samples      = 0;
    double totalMs      = 0.0;
    double observedHz   = 0.0;
    double meanMs       = 0.0;
    double minMs        = 0.0;
    double maxMs        = 0.0;
    double rmsJitterMs  = 0.0;
};

// Computes timing statistics from the wall-clock intervals (in ms)
// between consecutive IMU samples.
//
// Contract: `intervalsMs.size() == samples - 1` for samples > 1.
// If samples <= 1 or intervalsMs is empty, all metrics are left at 0.
TimingStats computeTimingStats(const std::vector<double>& intervalsMs, int samples);