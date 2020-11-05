#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

class Layer;

class LayerStack
{
    public:
    LayerStack() = default;
    ~LayerStack();

    void push_layer(Layer *layer);
    void push_overlay(Layer *overlay);
    void pop_layer(Layer *layer);
    void pop_overlay(Layer *overlay);

    vector<Layer *>::iterator begin()
    {
        return layers.begin();
    }
    vector<Layer *>::iterator end()
    {
        return layers.end();
    }
    vector<Layer *>::reverse_iterator rbegin()
    {
        return layers.rbegin();
    }
    vector<Layer *>::reverse_iterator rend()
    {
        return layers.rend();
    }

    vector<Layer *>::const_iterator begin() const
    {
        return layers.begin();
    }
    vector<Layer *>::const_iterator end() const
    {
        return layers.end();
    }
    vector<Layer *>::const_reverse_iterator rbegin() const
    {
        return layers.rbegin();
    }
    vector<Layer *>::const_reverse_iterator rend() const
    {
        return layers.rend();
    }

    private:
    vector<Layer *> layers;
    unsigned int layer_insert_index = 0;
};
