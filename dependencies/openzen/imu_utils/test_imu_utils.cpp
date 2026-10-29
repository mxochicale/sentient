// test_imu_utils.cpp
#include <cmath>
#include <gtest/gtest.h>
#include "imu_utils.h"

// ---------- Helpers ----------
static std::vector<char*> makeArgv(std::vector<std::string>& storage) {
    std::vector<char*> argv;
    for (auto& s : storage) argv.push_back(s.data());
    return argv;
}

// ---------- parseArgs: defaults ----------
TEST(ParseArgs, DefaultsWhenNoPositional) {
    std::vector<std::string> s{"prog"};
    auto argv = makeArgv(s);
    CliOptions o; std::string err;
    ASSERT_TRUE(parseArgs(static_cast<int>(argv.size()), argv.data(), o, err));
    EXPECT_EQ(o.macAddress, "00:04:3E:53:ED:5B");
    EXPECT_EQ(o.samplingRate, 50);
    EXPECT_EQ(o.baudRate, 921600);
    EXPECT_FALSE(o.verbose);
}

TEST(ParseArgs, MacOnly) {
    std::vector<std::string> s{"prog", "AA:BB:CC:DD:EE:FF"};
    auto argv = makeArgv(s);
    CliOptions o; std::string err;
    ASSERT_TRUE(parseArgs(static_cast<int>(argv.size()), argv.data(), o, err));
    EXPECT_EQ(o.macAddress, "AA:BB:CC:DD:EE:FF");
    EXPECT_EQ(o.samplingRate, 50);   // unchanged default
    EXPECT_EQ(o.baudRate, 921600);
}

TEST(ParseArgs, AllPositional) {
    std::vector<std::string> s{"prog", "11:22:33:44:55:66", "100", "115200"};
    auto argv = makeArgv(s);
    CliOptions o; std::string err;
    ASSERT_TRUE(parseArgs(static_cast<int>(argv.size()), argv.data(), o, err));
    EXPECT_EQ(o.macAddress, "11:22:33:44:55:66");
    EXPECT_EQ(o.samplingRate, 100);
    EXPECT_EQ(o.baudRate, 115200);
}

TEST(ParseArgs, VerboseFlagAnywhere) {
    std::vector<std::string> s{"prog", "--verbose", "AA:BB:CC:DD:EE:FF", "200"};
    auto argv = makeArgv(s);
    CliOptions o; std::string err;
    ASSERT_TRUE(parseArgs(static_cast<int>(argv.size()), argv.data(), o, err));
    EXPECT_TRUE(o.verbose);
    EXPECT_EQ(o.macAddress, "AA:BB:CC:DD:EE:FF");
    EXPECT_EQ(o.samplingRate, 200);
}

// ---------- parseArgs: error cases ----------
TEST(ParseArgs, RejectsNonNumericSamplingRate) {
    std::vector<std::string> s{"prog", "AA", "abc"};
    auto argv = makeArgv(s);
    CliOptions o; std::string err;
    EXPECT_FALSE(parseArgs(static_cast<int>(argv.size()), argv.data(), o, err));
    EXPECT_NE(err.find("sampling rate"), std::string::npos);
}

TEST(ParseArgs, RejectsZeroOrNegativeSamplingRate) {
    for (const char* bad : {"0", "-5"}) {
        std::vector<std::string> s{"prog", "AA", bad};
        auto argv = makeArgv(s);
        CliOptions o; std::string err;
        EXPECT_FALSE(parseArgs(static_cast<int>(argv.size()), argv.data(), o, err))
            << "input: " << bad;
    }
}

TEST(ParseArgs, RejectsTrailingGarbage) {
    std::vector<std::string> s{"prog", "AA", "100x"};
    auto argv = makeArgv(s);
    CliOptions o; std::string err;
    EXPECT_FALSE(parseArgs(static_cast<int>(argv.size()), argv.data(), o, err));
}

TEST(ParseArgs, RejectsBadBaudRate) {
    std::vector<std::string> s{"prog", "AA", "50", "notanumber"};
    auto argv = makeArgv(s);
    CliOptions o; std::string err;
    EXPECT_FALSE(parseArgs(static_cast<int>(argv.size()), argv.data(), o, err));
    EXPECT_NE(err.find("baud rate"), std::string::npos);
}

// ---------- computeTimingStats ----------
TEST(TimingStats, EmptyWhenTooFewSamples) {
    std::vector<double> iv;
    EXPECT_EQ(computeTimingStats(iv, 0).samples, 0);
    EXPECT_EQ(computeTimingStats(iv, 1).samples, 1);
    // still no intervals -> all metrics zero
    auto s = computeTimingStats(iv, 1);
    EXPECT_EQ(s.observedHz, 0.0);
    EXPECT_EQ(s.rmsJitterMs, 0.0);
}

TEST(TimingStats, PerfectPeriodicSamples) {
    // 99 intervals of 20 ms -> 100 samples at 50 Hz
    std::vector<double> iv(99, 20.0);
    auto s = computeTimingStats(iv, 100);
    EXPECT_EQ(s.samples, 100);
    EXPECT_NEAR(s.totalMs, 1980.0, 1e-9);
    EXPECT_NEAR(s.observedHz, 99 * 1000.0 / 1980.0, 1e-9);
    EXPECT_NEAR(s.meanMs, 20.0, 1e-12);
    EXPECT_NEAR(s.minMs, 20.0, 1e-12);
    EXPECT_NEAR(s.maxMs, 20.0, 1e-12);
    EXPECT_NEAR(s.rmsJitterMs, 0.0, 1e-12);
}

TEST(TimingStats, KnownJitter) {
    // intervals: 10, 20, 30 -> mean 20, deviations -10, 0, 10
    // RMS = sqrt((100 + 0 + 100)/3) = sqrt(200/3)
    std::vector<double> iv{10.0, 20.0, 30.0};
    auto s = computeTimingStats(iv, 4);
    EXPECT_NEAR(s.meanMs, 20.0, 1e-12);
    EXPECT_NEAR(s.minMs, 10.0, 1e-12);
    EXPECT_NEAR(s.maxMs, 30.0, 1e-12);
    EXPECT_NEAR(s.rmsJitterMs, std::sqrt(200.0 / 3.0), 1e-12);
    EXPECT_NEAR(s.observedHz, 3 * 1000.0 / 60.0, 1e-9);
}

TEST(TimingStats, ObservedRateDiffersFromRequested) {
    // 9 intervals of 25 ms -> 10 samples in 225 ms -> 40 Hz
    std::vector<double> iv(9, 25.0);
    auto s = computeTimingStats(iv, 10);
    EXPECT_NEAR(s.observedHz, 40.0, 1e-9);
}