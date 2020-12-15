using namespace std;

class Event;

class Layer
{
public:
    Layer(const string &name = "Layer");
    virtual ~Layer() = default;

    virtual void on_attach() { }
    virtual void on_detach() { }
    virtual void on_update(double ts) { }
    virtual void on_event(Event &event) { }
    virtual void on_imgui_render() { }

    const std::string &get_name() const
    {
        return debug_name;
    }

protected:
    std::string debug_name;
};
