#include <cerrno>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

enum struct Result {
  FullRead = 0,
  PartialRead_Error = 1,
  PartialRead_EOF = 2,
  InvalidFilePtr = 3
};

// Type cannot be ignored as a return value
template <typename Type>
struct [[nodiscard]] ReadResult {
  std::vector<Type> elements;

  Result result;
  std::string error_string;
};

// strongly typed file handle based on unique_ptr
// to ensure that we cannot accidentally leak a FILE *
using FileHandle = std::unique_ptr<std::FILE, decltype(&std::fclose)>;

// concept to make sure we only try to create trivially_copyable
// objects with our reader
template <typename Type>
concept trivially_copyable = std::is_trivially_copyable_v<Type>;

// type safe read from the file handle
// No c-cast necessary
// No possibility of leaking data
// Errors handled and reported
template <trivially_copyable Type>
ReadResult<Type> read(std::size_t count, FileHandle &fh) {
  if (fh.get() == nullptr) {
    return ReadResult<Type>{
        .elements{}, .result = Result::InvalidFilePtr, .error_string{}};
  }

  // construct proper type to get NRVO
  ReadResult<Type> results{.elements{count},
                           .result = Result::FullRead,
                           .error_string = ""};  // default initialize?

  const auto read =
      std::fread(results.elements.data(), sizeof(Type), count, fh.get());
  results.elements.resize(read);  // right-size results object

  // if read != count then we know something
  // didn't go quite right.
  if (read != count) {
    // it's possible we got a partial read
    if (std::ferror(fh.get()) != 0) {
      results.error_string = std::strerror(errno);
      results.result = Result::PartialRead_Error;
    } else if (std::feof(fh.get())) {
      results.result = Result::PartialRead_EOF;
    }
  }

  // NRVO basically guaranteed here
  return results;
}

// Type safe string mode wrapper that requires
// compile-time initialization
struct ModeString {
  explicit consteval ModeString(const char *str) noexcept
   : mode(str) {}

  const char *mode;
};

// Return a FileHandle that knows how to close the open file
// when the object goes out of scope
FileHandle my_fopen(const std::filesystem::path &file, ModeString mode)
{
  return FileHandle(std::fopen(file.c_str(), mode.mode), &std::fclose);
}

// delete potentially problematic implicit conversions
auto my_fopen(const auto & ...) = delete;

// Example thing to read
struct MyData {
  int x;
  float f;
};

int main() {
  // open file
  auto f = my_fopen(std::filesystem::path("/dev/random"),
                    ModeString("r"));

  // read some set of elements
  const auto results = read<MyData>(10, f);

  // Debug print what happened
  std::cout << std::to_underlying(results.result) << " " << results.error_string << '\n';

  for (const auto &elem : results.elements) {
    std::cout << elem.x << ',' << elem.f << '\n';
  }
}