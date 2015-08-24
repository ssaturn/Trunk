#pragma once


#include <vector>

#include "State.h"

namespace sst
{
	// state와 bt를 혼합하여 사용 상태를 유지하기 위해서 사용한다.
	// eval의 범위를 줄이고 재사용성으로 고려
	class HSBT
	{
	public:
		class Owner
		{
		public:
			Owner() : m_hsbt(nullptr) {}
			virtual ~Owner() {}

		private:
			HSBT*	m_hsbt;
		};

	public:
		HSBT();
		virtual ~HSBT();

		bool Init(Owner* owner);
		void OnTick(int tick);
		void Tran(int state);

		void Finish();
		State* CurrentState() { return m_current; }
		State* GetRoot() { return &m_root; }
		Owner* GetOwner() { return m_owner; }
		State* GetState(int stateId);

	private:
		Owner* m_owner;
		State  m_root;
		State* m_current;

	}; // HSBT
}