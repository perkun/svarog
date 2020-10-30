#include "LayerStack.h"

LayerStack::~LayerStack()
{
    for (Layer *layer : layers)
    {
        layer->on_detach();
        delete layer;
    }
}

void LayerStack::PushLayer(Layer *layer)
{
    layers.emplace(layers.begin() + layer_insert_index, layer);
    layer_insert_index++;
}

void LayerStack::PushOverlay(Layer *overlay)
{
    layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer *layer)
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

void LayerStack::PopOverlay(Layer *overlay)
{
    auto it = find(layers.begin() + layer_insert_index, layers.end(),
                        overlay);
    if (it != layers.end())
    {
        overlay->on_detach();
        layers.erase(it);
    }
}
