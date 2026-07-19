#pragma once

#include <cstdio>
#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>

template<typename Signature>
class FunctionRegistry;

template<typename Ret, typename... Args>
class FunctionRegistry<Ret(Args...)>
{
public:
  using FunctionType = std::function<Ret(Args...)>;

  /** Registers fn under label. Overwrites (and logs) an existing entry. */
  void Register(const std::string& label, FunctionType fn)
  {
    if (Contains(label))
      std::fprintf(stderr, "[FunctionRegistry] overwriting \"%s\"\n", label.c_str());
    m_functions[label] = std::move(fn);
  }

  /** Removes the entry under label, if one exists. */
  void Unregister(const std::string& label)
  {
    m_functions.erase(label);
  }

  /** True if a callable is currently registered under label. */
  bool Contains(const std::string& label) const
  {
    return m_functions.find(label) != m_functions.end();
  }

  /**
   * Invokes the callable registered under label.
   * Logs and returns a default-constructed Ret (nothing, for void) if
   * label isn't registered - never throws, safe to call speculatively.
   */
  Ret Call(const std::string& label, Args... args) const
  {
    auto it = m_functions.find(label);
    if (it == m_functions.end())
    {
      std::fprintf(stderr, "[FunctionRegistry] no function registered under \"%s\"\n", label.c_str());
      if constexpr (!std::is_void_v<Ret>)
        return Ret{};
      else
        return;
    }
    return it->second(args...);
  }

  /** Read-only access for enumeration, e.g. listing commands in a UI. */
  const std::unordered_map<std::string, FunctionType>& GetAll() const
  {
    return m_functions;
  }

private:
  std::unordered_map<std::string, FunctionType> m_functions;
};

// What you need today: label -> void() action, same call shape as your
// EventManager callbacks, just invoked on demand instead of every frame.
using ActionRegistry = FunctionRegistry<void()>;
