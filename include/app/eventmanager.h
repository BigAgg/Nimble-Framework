#pragma once

using Event = void(*)();

void RegisterEvent(Event function);
void TriggerEvents();

