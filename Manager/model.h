#include <vector>
#include "base.h"
#include "../Model/model.h"

namespace Manager {
    namespace {
        typedef ListEntry<Graphics::Model> ModelEntry;
    }
    class Model {
    private:
        std::vector<ModelEntry> models;
    public:
        Model();
        ~Model();
        unsigned addEntry(ModelEntry entry);
        unsigned addEntry(std::string name, Graphics::Model model);
        bool     dropEntry(unsigned index);
    };
}
