#include "Sampler.h"

Engine::Rendering::Sampler::Sampler(const std::shared_ptr<Direct3D>& d3d)
	:m_direct3d(d3d)
{
	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	if (FAILED(m_hresult = m_direct3d->GetDevice()->CreateSamplerState(&sampler_desc, &m_sampler)))
	{
		
	}
}

void Engine::Rendering::Sampler::Bind(UINT slot) const
{
	m_direct3d->GetContext()->PSSetSamplers(slot, 1u, m_sampler.GetAddressOf());
}
