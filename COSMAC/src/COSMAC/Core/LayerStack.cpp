#include "cpch.h"
#include "COSMAC/Core/LayerStack.h"

namespace COSMAC
{
    // Destructor for the LayerStack class.
    LayerStack::~LayerStack()
    {
        // Deallocate memory for each layer in the layers list.
        for (Layer* layer : m_Layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    // Add a new layer to the layer stack before the current insertion point.
    void LayerStack::PushLayer(Layer *layer)
    {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
        layer->OnAttach();
    }

    // Add an overlay layer to the top of the layer stack.
    void LayerStack::PushOverlay(Layer *overlay)
    {
        m_Layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    // Remove a layer from the layer stack.
    void LayerStack::PopLayer(Layer *layer)
    {
        // Find the specified layer in the layers list.
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.begin() + m_LayerInsertIndex)
        {
            // Erase the layer from the layers list.
            layer->OnDetach();
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    // Remove an overlay layer from the layer stack.
    void LayerStack::PopOverlay(Layer *overlay)
    {
        // Find the specified overlay layer in the layers list.
        auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            // Erase the overlay layer from the layers list.
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    }
}
