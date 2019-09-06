#pragma once
#include <vector>
#include <string>

// Parses argument char array. Usually passed from command line
namespace base {
class CmdLineParser {
public:
    // Arguments are typical of the main function
    // argc - argument count, argv - pointer to 2d array of char
    CmdLineParser(uint32_t argc,  char* argv[]);
    // Return next token after the option that is passed
    // Returns an empty string if no option found or no tokens after option 
    std::string GetOption(std::string_view option) const;
    // Checks weather given option(token) exists
    bool OptionExists(std::string_view token) const;
private:
    using TokenVector = std::vector <std::string>;
    TokenVector GetTokens(uint32_t argc, char* argv[]) const;
    // All arguments in string vector
    const TokenVector tokens_;
};
}; // base