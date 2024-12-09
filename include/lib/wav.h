#pragma once

#include <cstdint>
#include <vector>
#include <istream>
#include <fstream>
#include <iostream>


///
/// https://github.com/Signalsmith-Audio/reverb-example-code/blob/2798498231eac8c31e8d75e7c1ddea1e4c794f18/wav.h
///
template<bool littleEndian=true>
struct BigEndian {
  static uint32_t read16(std::istream& in) {
    unsigned char a[2];
    in.read((char*)a, sizeof(a));
    return ((uint32_t)a[0]) + ((uint32_t)a[1])*256;
  }
  static uint32_t read32(std::istream& in) {
    unsigned char a[4];
    in.read((char*)a, sizeof(a));
    return ((uint32_t)a[0]&0xff) + ((uint32_t)a[1])*256 + ((uint32_t)a[2])*65536 + ((uint32_t)a[3])*256*65536;
  }

  static void write16(std::ostream& out, uint16_t value) {
    char a[2] = {(char)(value>>0), (char)(value>>8)};
    out.write(a, sizeof(a));
  }
  static void write32(std::ostream& out, uint32_t value) {
    char a[4] = {(char)(value>>0), (char)(value>>8), (char)(value>>16), (char)(value>>24)};
    out.write(a, sizeof(a));
  }
};

class Wav : BigEndian<true> {
  // Little-endian versions of text values
  uint32_t value_RIFF = 0x46464952;
  uint32_t value_WAVE = 0x45564157;
  uint32_t value_fmt = 0x20746d66;
  uint32_t value_data = 0x61746164;

  using BigEndian<true>::read16;
  using BigEndian<true>::read32;
  using BigEndian<true>::write16;
  using BigEndian<true>::write32;

public:
  struct Result {
    enum class Code {
      OK = 0,
      IO_ERROR,
      FORMAT_ERROR,
      UNSUPPORTED,
      WEIRD_CONFIG
    };
    Code code = Code::OK;
    std::string reason;

    Result(Code code, std::string reason="") : code(code), reason(reason) {};
    Result(const Result &other) : code(other.code), reason(other.reason) {};
    Result & operator=(const Result &other) {
      // preserve previous error value
      if (code == Code::OK) {
        code = other.code;
        reason = other.reason;
      }
      return *this;
    }
    // Used to neatly test for success
    explicit operator bool () const {
      return code == Code::OK;
    };
  };

  unsigned int sampleRate = 48000;
  unsigned int channels = 1;
  std::vector<float> samples;
  Result result = Result(Result::Code::OK);

  Wav() = default;

  enum class Format {
    PCM=1
  };
  bool formatIsValid(uint16_t format, uint16_t bits) {
    if (format == (uint16_t)Format::PCM) {
      if (bits == 16) {
        return true;
      }
    }
    return false;
  }

  Result write(std::string filename, Format format=Format::PCM) {
    if (channels == 0 || channels > 65535) return result = Result(Result::Code::WEIRD_CONFIG, "Invalid channel count");
    if (sampleRate <= 0 || sampleRate > 0xFFFFFFFFu) return result = Result(Result::Code::WEIRD_CONFIG, "Invalid sample rate");

    std::ofstream file;
    file.open(filename, std::ios::binary);
    if (!file.is_open()) return result = Result(Result::Code::IO_ERROR, "Failed to open file: " + filename);

    int bytesPerSample;
    switch (format) {
    case Format::PCM:
      bytesPerSample = 2;
      break;
    }

    // File size - 44 bytes is RIFF header, "fmt" block, and "data" block header
    unsigned int dataLength = samples.size()*bytesPerSample;
    unsigned int fileLength = 44 + dataLength;

    // RIFF chunk
    write32(file, value_RIFF);
    write32(file, fileLength - 8); // File length, excluding the RIFF header
    write32(file, value_WAVE);
    // "fmt " block
    write32(file, value_fmt);
    write32(file, 16); // block length
    write16(file, (uint16_t)format);
    write16(file, channels);
    write32(file, sampleRate);
    unsigned int expectedBytesPerSecond = sampleRate*channels*bytesPerSample;
    write32(file, expectedBytesPerSecond);
    write16(file, channels*bytesPerSample); // Bytes per frame
    write16(file, bytesPerSample*8); // bist per sample

    // "data" block
    write32(file, value_data);
    write32(file, dataLength);
    switch (format) {
    case Format::PCM:
      for (unsigned int i = 0; i < samples.size(); i++) {
        float value = samples[i]*32768;
        if (value > 32767) value = 32767;
        if (value <= -32768) value = -32768;
        if (value < 0) value += 65536;
        write16(file, (uint16_t)value);
      }
      break;
    }
    return result = Result(Result::Code::OK);
  }
};
