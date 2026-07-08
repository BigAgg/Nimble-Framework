#include <app/eventmanager.h>
#include <cassert>
#include <functional>

void EventRegistry::RegisterEvent(Event function) {
  mRegistry.push_back(std::move(function));
}

void EventRegistry::TriggerEvents () {
  for (auto &func : mRegistry)
    func();
  mRegistry.clear();
}
