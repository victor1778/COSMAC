#include "cpch.h"
#include "COSMAC/Core/Layer.h"

namespace COSMAC
{

	Layer::Layer(const std::string &debugName)
		: m_DebugName(debugName)
	{
	}

	Layer::~Layer()
	{
	}
}