#pragma once

#include "Behavior.h"
#include <vector>

namespace sst
{
	class HSBT;
	class State : public Behavior
	{
	public:
		enum EState
		{
			STATE_ROOT = 1,
			STATE_BEGIN
		};

		struct Node
		{
			enum Code
			{
				NOOP = 1,		// nothing to do
				EXIT,			// exit the state
				ENTER,			// enter the state
				TARGET,			// target state
			};

			Code	code;
			State*	state;
		};

		typedef std::vector<Node> Path;
		typedef std::vector<State*> StateList;

		State();
		virtual ~State();

		bool Init(HSBT* owner, int id, State* parent);
		void OnEnter() {};
		State* OnTick(int tick) { return nullptr; }
		void OnExit() {};
		void Finish();

		bool HasState(int id) const;
		State* GetState(int id);
		int GetId() { return m_id; }
		State* GetParent() { return m_parent; }
		bool GetTransitionPathTo(int target, Path& path) { return true; };

	protected:
		virtual bool init() { return true; }
		virtual void onEnter() {}
		virtual State* onTick() { return nullptr; }
		virtual void onExit() {}
		virtual void finish() {}

	protected:
		int		  		m_id;
		HSBT*	 		m_owner;
		State*    		m_parent;
		StateList 		m_children;
		int				m_targetId;

	};
}
