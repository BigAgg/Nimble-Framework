#include <app/eventmanager.h>
#include <cassert>
#include <utility>
#include <vector>
#include <utils/logging.h>

class EventRegistry {
public:
  static EventRegistry& Get () {
    static EventRegistry instance;
    return instance;
  }

  std::vector<Event> registry;

public:
  EventRegistry(const EventRegistry &) = delete;
  EventRegistry &operator=(const EventRegistry &) = delete;
  EventRegistry(EventRegistry &&) = delete;
  EventRegistry &operator=(EventRegistry &&) = delete;

private:
  EventRegistry() = default;
	~EventRegistry() = default;
};

void RegisterEvent(Event function) {
  if (!function) {
    LOG_ERROR("Invalid function pointer!");
    return;
  }
  auto& registry = EventRegistry::Get();
  registry.registry.push_back(std::move(function));
}

void TriggerEvents () {
  auto& registry = EventRegistry::Get();
  for (auto &func : registry.registry)
    func();
  registry.registry.clear();
}
