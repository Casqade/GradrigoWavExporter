#include <Gradrigo/Gradrigo.hpp>

#include <lib/wav.h>

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>


class CmdArgsParser
{
public:
  enum class ErrorCodes
  {
    IsNotAnOption = 1,
    UnknownOption,
    UndefinedOptionHandler,
    OptionHandlerFailure,
  };

  struct ErrorCategory : std::error_category
  {
    const char* name() const noexcept override;
    std::string message(int ev) const override;

  };

  using OptionHandler = std::function <std::error_code( const std::string& value )>;


  CmdArgsParser() = default;

  void addOption( const std::string& option, const OptionHandler& );

  std::error_code parseArgument( const std::string& ) const;


private:
  std::map <std::string, OptionHandler> mOptions {};
};

const static CmdArgsParser::ErrorCategory errorCategory {};


const char* CmdArgsParser::ErrorCategory::name() const noexcept
{
  return "CmdArgsParser";
}

std::string
CmdArgsParser::ErrorCategory::message(
  int errorCode ) const
{
  return {};
}

std::error_code
make_error_code(
  CmdArgsParser::ErrorCodes errorCode )
{
  return { static_cast <int> (errorCode), errorCategory};
}

namespace std
{
template <>
struct is_error_code_enum <CmdArgsParser::ErrorCodes> : true_type {};
}


void
CmdArgsParser::addOption(
  const std::string& option,
  const OptionHandler& handler )
{
  mOptions[option] = handler;
}

std::error_code
CmdArgsParser::parseArgument(
  const std::string& argument ) const
{
  if ( mOptions.count(argument) > 0 )
  {
    auto& handler = mOptions.at(argument);

    if ( handler == nullptr )
      return ErrorCodes::UndefinedOptionHandler;

    return handler({});
  }

  for ( size_t i {}; i < argument.size(); ++i )
  {
    if ( argument[i] == '=' )
    {
      const auto option = argument.substr(0, i);
      const auto value = argument.substr(i + 1);

      if ( mOptions.count(option) == 0 )
      {
        if ( argument[0] == '-' )
          return ErrorCodes::UnknownOption;

        if ( argument.size() >= 2 &&
             argument[0] == '-' && argument[1] == '-' )
          return ErrorCodes::UnknownOption;


        return ErrorCodes::IsNotAnOption;
      }


      auto& handler = mOptions.at(option);

      if ( handler == nullptr )
        return ErrorCodes::UndefinedOptionHandler;

      return handler(value);
    }
  }

  return ErrorCodes::IsNotAnOption;
}

void
printHelp()
{
std::cout <<
R"help(GradrigoWavExporter - exports Gradrigo boxes as .wav files

NOTE: Boxes with infinite loops are NOT available for export!

Usage: GradrigoWavExporter [options] [<boxes>]

Examples:
  GradrigoWavExporter --script=myScript.gr -sr=44100 box1 box2


Options:
  -h --help             Show this help

  -l                    List available boxes
  --list

  -d=FILENAME           Override path to Gradrigo DLL
  --dll=FILENAME          Default path is 'gradrigo.dll'

  -s=FILENAME           Specify path to Gradrigo script
  --script=FILENAME

  -sr=NUMBER            Override sample rate.
  --sample-rate=NUMBER    Default sample rate is 44100

  -mem=BYTES            Specify memory limit for samples buffer
  --memory-limit=BYTES    Default memory limit is 10 MiB

)help";
}

void
printHelpHint()
{
  std::cout << "Try 'GradrigoWavExporter --help' for more information\n";
}


int
main(
  int argc,
  char* argv[] )
{
  using ErrorCodes = CmdArgsParser::ErrorCodes;


  std::string dllPath = "gradrigo.dll";
  std::string scriptPath {};

  size_t sampleRate = 44100;
  size_t memoryLimit = 10 * 1024 * 1024;

  bool showHelp {};
  bool listBoxes {};


  const auto helpOptionHandler =
  [&showHelp] ( const std::string& value ) -> std::error_code
  {
    showHelp = true;
    return {};
  };

  const auto listBoxesOptionHandler =
  [&listBoxes] ( const std::string& value ) -> std::error_code
  {
    listBoxes = true;
    return {};
  };

  const auto dllPathOptionHandler =
  [&dllPath] ( const std::string& value ) -> std::error_code
  {
    dllPath = value;
    return {};
  };

  const auto scriptPathOptionHandler =
  [&scriptPath] ( const std::string& value ) -> std::error_code
  {
    scriptPath = value;
    return {};
  };

  const auto sampleRateOptionHandler =
  [&sampleRate] ( const std::string& value ) -> std::error_code
  {
    try
    {
      const auto number = std::stoi(value);

      if ( number <= 0 )
        return ErrorCodes::OptionHandlerFailure;

      sampleRate = number;
    }
    catch ( ... )
    {
      return ErrorCodes::OptionHandlerFailure;
    }

    return {};
  };

  const auto memoryLimitOptionHandler =
  [&memoryLimit] ( const std::string& value ) -> std::error_code
  {
    try
    {
      const auto number = std::stoi(value);

      if ( number <= 0 )
        return ErrorCodes::OptionHandlerFailure;

      memoryLimit = number;
    }
    catch ( ... )
    {
      return ErrorCodes::OptionHandlerFailure;
    }

    return {};
  };


  CmdArgsParser argsParser {};

  argsParser.addOption(
    "-h", helpOptionHandler );

  argsParser.addOption(
    "--help", helpOptionHandler );

  argsParser.addOption(
    "-l", listBoxesOptionHandler );

  argsParser.addOption(
    "--list", listBoxesOptionHandler );

  argsParser.addOption(
    "-d", dllPathOptionHandler );

  argsParser.addOption(
    "--dll", dllPathOptionHandler );

  argsParser.addOption(
    "-s", scriptPathOptionHandler );

  argsParser.addOption(
    "--script", scriptPathOptionHandler );

  argsParser.addOption(
    "-sr", sampleRateOptionHandler );

  argsParser.addOption(
    "--sample-rate", sampleRateOptionHandler );

  argsParser.addOption(
    "-mem", memoryLimitOptionHandler );

  argsParser.addOption(
    "--memory-limit", memoryLimitOptionHandler );


  std::vector <std::string> boxesToExport {};

  bool finishedOptionsProcessing {};

  for ( size_t i {1}; i < argc; ++i )
  {
    const std::string argument {argv[i]};

    if ( finishedOptionsProcessing == true )
    {
      boxesToExport.push_back(argument);
      continue;
    }


    const auto result = argsParser.parseArgument(argument);

    switch (static_cast <ErrorCodes> (result.value()))
    {
      case ErrorCodes::IsNotAnOption:
      {
        boxesToExport.push_back(argument);
        finishedOptionsProcessing = true;
        break;
      }

      case ErrorCodes::UnknownOption:
      {
        std::cerr << "Error: Unknown option '" + argument + "'\n";
        printHelpHint();
        return -1;
      }

      case ErrorCodes::UndefinedOptionHandler:
      {
        std::cerr << "Error: Undefined option handler for '" + argument + "'\n";
        return -2;
      }

      case ErrorCodes::OptionHandlerFailure:
      {
        std::cerr << "Error: Failed to handle option '" + argument + "'\n";
        return -3;
      }

      default:
        break;
    }
  }


  if ( showHelp == true )
  {
    printHelp();
    return 0;
  }

  if ( scriptPath.empty() == true )
  {
    std::cerr << "Error: No Gradrigo script specified\n";
    printHelpHint();
    return -4;
  }


  if ( listBoxes == false && boxesToExport.empty() == true )
  {
    std::cerr << "Error: No boxes specified for export\n";
    printHelpHint();
    return -5;
  }


  std::ifstream scriptFile {};

  scriptFile.exceptions(
    std::ifstream::failbit | std::ifstream::badbit );

  try
  {
    scriptFile.open(scriptPath, std::ios::in);
  }
  catch ( const std::system_error& e )
  {
    std::cerr << "Error: Failed to open '" +
                 scriptPath + "': " + std::strerror(errno) + "\n";

    return -6;
  }


  if ( Gradrigo::Load(dllPath.c_str()) == false )
  {
    std::cerr << "Error: Failed to load '" + dllPath + "'\n";
    return -7;
  }


  Gradrigo::Initialize(sampleRate);
  Gradrigo::EnableCompletedVoicePolling(true);


  std::string script {};
  {
    std::stringstream stream {};
    stream << scriptFile.rdbuf();
    scriptFile.close();
    script = stream.str();
  }

  const auto requestId =
    Gradrigo::ParseString(script.c_str());


//  Lazy-initialize Gradrigo
  {
    float buffer;
    Gradrigo::GetBuffer(1, &buffer);
  }


  const auto parseResult = Gradrigo::GetResponseString(requestId);

  if ( parseResult != nullptr )
//    std::cout << "Script parse result (request id " << requestId << "): " << parseResult << "\n";
    ;

  const std::string boxList = Gradrigo::ReportBoxesAsJson();

  if ( listBoxes == true )
    std::cout << "Available boxes:\n" << boxList << "\n";


  const auto sampleSize = sizeof(decltype(Wav::samples)::value_type);
  const auto maxSamples = memoryLimit / sampleSize;

  for ( const auto& boxName : boxesToExport )
  {
    const auto boxJsonEntry =
      R"prefix({ "Name": ")prefix" + boxName + R"postfix(" })postfix";

    if ( boxList.find(boxJsonEntry) == std::string::npos )
    {
      std::cerr << "Error: Box '" + boxName + "' not found in script '" + scriptPath + "'\n";
      continue;
    }


    int processedSamples {};

    Wav file {};
    file.samples.reserve(sampleRate);
    file.sampleRate = sampleRate;


    const auto voiceId =
      Gradrigo::StartVoice(boxName.c_str());

    while ( Gradrigo::PollCompletedVoice() != voiceId )
    {
      if ( file.samples.size() >= maxSamples )
      {
        std::cerr << "Error: Failed to export box '"
                  << boxName << "'. Reached memory limit of "
                  << maxSamples * sampleSize << " bytes. "
                  << "Make sure the voice is NOT an infinite loop "
                  << "or increase memory limit\n";
        printHelpHint();
        return -9;
      }

      file.samples.resize(
        file.samples.size() + 1 );

      processedSamples += Gradrigo::GetBuffer(
        1, file.samples.data() + processedSamples );
    }


    if ( file.samples.empty() == false )
    {
      file.samples.pop_back();

      const auto writeResult = file.write(boxName + ".wav");

      if ( !writeResult )
      {
        std::cerr << "Error: " + writeResult.reason + "\n";
        std::cerr << "Error: Failed to export box '" + boxName + "'\n";
        continue;
      }
    }
  }

  return 0;
}
