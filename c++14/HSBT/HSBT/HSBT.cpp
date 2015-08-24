#include "stdafx.h"
#include "HSBT.h"
#include <assert.h>

namespace sst
{
	HSBT::HSBT()
		: m_owner(nullptr)
		, m_root()
		, m_current(nullptr)
	{

	}

	HSBT::~HSBT()
	{
		Finish();
	}

	bool HSBT::Init(Owner* owner)
	{
		assert(owner);

		m_owner = owner;
		m_current = &m_root;
		m_current->Init(this, State::STATE_ROOT, 0);

		return true;
	}

	void HSBT::OnTick(int tick)
	{
		State* state = m_current;

		while (state != 0)
		{
			state = state->OnTick(tick);
		}
	}

	State* HSBT::GetState(int stateId)
	{
		return m_root.GetState(stateId);
	}

	void HSBT::Tran(int state)
	{
		if (m_current == nullptr)
		{
			return;
		}

		State::Path path;

		bool rc = m_current->GetTransitionPathTo(state, path);
		if (!rc)
		{
			return;
		}

		for (auto& node : path)
		{
			assert(node.state != nullptr);

			bool found = false;

			switch (node.code)
			{
			case State::Node::ENTER:
				{
					node.state->OnEnter();
				}
				break;
			case State::Node::EXIT:
				{
					node.state->OnExit();
				}
				break;
			case State::Node::TARGET:
				{
					found = true;

					m_current = node.state;
					node.state->OnEnter();
				}
				break;
			default:
				break;
			}
		}
	}

	void HSBT::Finish()
	{
		m_root.Finish();
	}

	


}