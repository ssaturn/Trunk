#include "stdafx.h"
#include "State.h"
#include <assert.h>

namespace sst
{
	State::State()
		: m_id(0)
		, m_parent(nullptr)
		, m_owner(nullptr)
		, m_targetId(0)
	{

	}

	State::~State()
	{
		Finish();
	}

	bool State::Init(HSBT* owner, int id, State* parent)
	{
		assert(owner != 0);

		m_owner = owner;
		m_id = id;
		m_parent = parent;
		m_targetId = 0;

		return init();
	}

	void State::Finish()
	{
		for (auto& child : m_children)
		{
			child->Finish();
			delete child;
		}

		m_children.clear();

		finish();
	}

	bool State::HasState(int id) const
	{
		if (id == m_id)
		{
			return true;
		}

		for (auto& child : m_children)
		{
			if (child->HasState(id))
			{
				return true;
			}
		}

		return false;
	}

	State* State::GetState(int id)
	{
		if (id == m_id)
		{
			return this;
		}

		if (!HasState(id))
		{
			return nullptr;
		}

		for (auto& child : m_children)
		{
			assert(child != nullptr);

			if (child->HasState(id))
			{
				return child->GetState(id);
			}
		}

		return nullptr;
	}

}