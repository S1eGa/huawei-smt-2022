#include <stdexcept>
#include <string>

struct AntisymmetryException : std::logic_error {
    AntisymmetryException(const std::string &what_arg) : std::logic_error(what_arg) {}
};