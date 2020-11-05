#include "LayerStack.h"
#include "Layer.h"


LayerStack::~LayerStack()
{
    for (Layer *layer : layers)
    {
        layer->on_detach();
        delete layer;
    }
}

void LayerStack::push_layer(Layer *layer)
{
    layers.emplace(layers.begin() + layer_insert_index, layer);
    layer_insert_index++;
}

void LayerStack::push_overlay(Layer *overlay)
{
    layers.emplace_back(overlay);
}

void LayerStack::pop_layer(Layer *layer)
{
    auto it = find(layers.begin(), layers.begin() + layer_insert_index,
                        layer);
    if (it != layers.begin() + layer_insert_index)
    {
        layer->on_detach();
        layers.erase(it);
        layer_insert_index--;
    }
}

void LayerStack::pop_overlay(Layer *overlay)
{
    auto it = find(layers.begin() + layer_insert_index, layers.end(),
                        overlay);
    if (it != layers.end())
    {
        overlay->on_detach();
        layers.erase(it);
    }
}
