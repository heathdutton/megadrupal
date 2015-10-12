Finite State Machine
--------------------
A machine is a device which accepts some inputs, possibly produces some output,
and has some memory sorting information on the overall results of all previous
inputs.

The condition of the machine at a particular instance is called a state of the
machine. New input changes the state, may be to another state, and may be to
the same state. The effect of a new input depends on the present state the
machine is in.

If the total number of possible states is finite, the machine is called finite
state machine, eg. a computer.

More information about State Machines can be found here: http://en.wikipedia.org/wiki/Finite-state_machine

Event Execution Summary
-----------------------
1. Check the event's guard condition.  Stop here if FALSE.
2. Event::before_transition
3. CurrentState::on_exit
4. Update state in machine
5. NewState::on_enter
6. Event::after_transition


Structure of a State Machine
----------------------------
A StateMachine is made up of States and Events.
States are the status of the item at a certain point in time.
Events describe how the machine can move between states.


State Flow
-----------------------
State Flow is the base implementation of State Machine class. It provides a base workflow as a base plugin.

Custom plugins should use State Flow as a model when:

Adding new states
Adding new events


Integration
-----------------------
State Flow provides the following optional integration with other modules:

Rules: Hook a condition into a event transition
Views: Exposes defined states, revision information, and timestamps


Comparisons to Workbench
-----------------------
Workbench is powerful, workflow-related module to manage content. The direct comparison of State Machine is to Workbench Moderation.

The main difference between State Machine and Workbench is that State Machine is extendable and exportable through plugins. Workbench is mainly administered through the user interface and is currently not exportable.

For developers, State Machine events and states can be defined through extending the base class. A basic workflow interface within Drupal's default revision architecture is provided by State Flow.

The user interaction of State Machine is purposefully simplistic. For those who are looking for a more robust solution that can be configured through the Drupal administration area should look at using Workbench.
