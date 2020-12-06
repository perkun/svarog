#include "Event/Event.h"

using namespace std;

class Layer
{
public:
    Layer(const string &name = "Layer");
    virtual ~Layer() = default;

    virtual void on_attach() { }
    virtual void on_detach() { }
    virtual void on_update(double ts) { }
    virtual void on_imgui_render() { }
    virtual void on_event(Event &event) { }

    const std::string &GetName() const
    {
        return m_DebugName;
    }

protected:
    std::string m_DebugName;
};
