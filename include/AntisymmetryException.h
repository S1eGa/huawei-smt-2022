#include <stdexcept>
#include <string>

struct AntisymmetryException : std::logic_error
{
private:
    std::vector<int32_t> errorElements;

public:
    AntisymmetryException(const std::vector<int32_t> errorElements) : std::logic_error("Given order is not satisfy antisymmetry properties"), errorElements(errorElements) {}

    std::vector<int32_t> getErrorElements() const
    {
        return errorElements;
    }
};