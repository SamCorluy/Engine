#pragma once
#include "Observer.h"
#include <memory>
#include <vector>
namespace dae
{
	class Subject final: public BaseComponent
	{
	public:
		Subject(const std::shared_ptr<GameObject>& owner);
		void AddObserver(std::shared_ptr<Observer> observer);
		void RemoveObserver(std::shared_ptr<Observer> observer);

		void StaticUpdate() override;
		void Update() override;
		void Render(const Transform& pos) const override;

		void Notify(Event event, int data);
	private:
		std::vector<std::shared_ptr<Observer>> observers;
	};
}

