#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Entity.hpp"

class Wasp : public Entity
{
public:
	Wasp(Game* game, const Vec2& position, float orientationDegrees);
	~Wasp() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void DebugRender() const override;

private:
	void InitializeLocalVerts() override;

	Vertex_PCU m_localVerts[WASP_VERTS_NUM];
};