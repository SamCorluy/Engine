#pragma once
#include "Observer.h"
#define MAX_OBSERVERS 10

namespace dae
{
	class Subject: BaseComponent
	{
	public:
		Subject(const std::shared_ptr<GameObject>& owner);
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

		void StaticUpdate() override;
		void Update() override;
		void Render(const Transform& pos) const override;

		void Notify(Event event, const std::weak_ptr<GameObject>& gameObject);
		void Notify(Event event, int data);
	private:
		Observer* observers[MAX_OBSERVERS];
		int numbObservers;
	};
}

