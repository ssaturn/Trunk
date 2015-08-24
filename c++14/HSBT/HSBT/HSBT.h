#pragma once


#include <vector>

#include "State.h"

namespace sst
{
	// state�� bt�� ȥ���Ͽ� ��� ���¸� �����ϱ� ���ؼ� ����Ѵ�.
	// eval�� ������ ���̰� ���뼺���� ���
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