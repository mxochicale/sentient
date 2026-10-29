#include "imu_utils.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <numeric>

bool parseArgs(int argc, char* argv[], CliOptions& out, std::string& error) {
    // Collect positional arguments, consuming any --verbose flags.
    // positional[0] is the program name (unused for parsing).
    std::vector<const char*> positional;
    positional.reserve(static_cast<size_t>(argc));
    positional.push_back(argv[0]);
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--verbose") == 0) {
            out.verbose = true;
        } else {
            positional.push_back(argv[i]);
        }
    }

    auto parsePositiveInt = [&](const char* s, const char* label, int& dst) -> bool {
        char* endptr = nullptr;
        long val = std::strtol(s, &endptr, 10);
        if (endptr == s || *endptr != '\0' || val <= 0) {
            error = std::string("Invalid ") + label + ": " + s;
            return false;
        }
        dst = static_cast<int>(val);
        return true;
    };

    if (positional.size() > 1) {
        out.macAddress = positional[1];
    }
    if (positional.size() > 2) {
        if (!parsePositiveInt(positional[2], "sampling rate", out.samplingRate)) {
            return false;
        }
    }
    if (positional.size() > 3) {
        if (!parsePositiveInt(positional[3], "baud rate", out.baudRate)) {
            return false;
        }
    }
    return true;
}

TimingStats computeTimingStats(const std::vector<double>& intervalsMs, int samples) {
    TimingStats s;
    s.samples = samples;
    if (samples <= 1 || intervalsMs.empty()) {
        return s;
    }

    double totalMs = std::accumulate(intervalsMs.begin(), intervalsMs.end(), 0.0);
    double minMs   = *std::min_element(intervalsMs.begin(), intervalsMs.end());
    double maxMs   = *std::max_element(intervalsMs.begin(), intervalsMs.end());
    double meanMs  = totalMs / static_cast<double>(intervalsMs.size());

    // RMS jitter around the mean interval
    double sq = 0.0;
    for (double v : intervalsMs) {
        double d = v - meanMs;
        sq += d * d;
    }
    double rmsJitterMs = std::sqrt(sq / static_cast<double>(intervalsMs.size()));

    s.totalMs     = totalMs;
    s.observedHz  = (samples - 1) * 1000.0 / totalMs;
    s.meanMs      = meanMs;
    s.minMs       = minMs;
    s.maxMs       = maxMs;
    s.rmsJitterMs = rmsJitterMs;
    return s;
}