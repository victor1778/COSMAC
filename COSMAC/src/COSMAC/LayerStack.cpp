#include "cpch.h"
#include "LayerStack.h"

namespace COSMAC
{
    // Constructor for the LayerStack class.
    LayerStack::LayerStack()
    {

    }

    // Destructor for the LayerStack class.
    LayerStack::~LayerStack()
    {
        // Deallocate memory for each layer in the layers list.
        for (Layer *layer : m_Layers)
            delete layer;
    }

    // Add a new layer to the layer stack before the current insertion point.
    void LayerStack::PushLayer(Layer *layer)
    {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
    }

    // Add an overlay layer to the top of the layer stack.
    void LayerStack::PushOverlay(Layer *overlay)
    {
        m_Layers.emplace_back(overlay);
    }

    // Remove a layer from the layer stack.
    void LayerStack::PopLayer(Layer *layer)
    {
        // Find the specified layer in the layers list.
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            // Erase the layer from the layers list.
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    // Remove an overlay layer from the layer stack.
    void LayerStack::PopOverlay(Layer *overlay)
    {
        // Find the specified overlay layer in the layers list.
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            // Erase the overlay layer from the layers list.
            m_Layers.erase(it);
        }
    }
}
