#pragma once

#include <string>
#include <vector>
#include <functional>

using Event = std::function<void()>;

class EventRegistry {
public:
  static EventRegistry& Get () {
    static EventRegistry instance;
    return instance;
  }

  void RegisterEvent(Event function);
  void TriggerEvents();

private:
  std::vector<Event> mRegistry;

public:
  EventRegistry(const EventRegistry &) = delete;
  EventRegistry &operator=(const EventRegistry &) = delete;
  EventRegistry(EventRegistry &&) = delete;
  EventRegistry &operator=(EventRegistry &&) = delete;

private:
  EventRegistry() = default;
	~EventRegistry() = default;
};
