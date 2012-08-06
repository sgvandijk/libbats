#include "humanoidagent.ih"

bool HumanoidAgent::s_die = false;

sigc::signal<void> HumanoidAgent::think_start_signal;
sigc::signal<void> HumanoidAgent::think_end_signal;

