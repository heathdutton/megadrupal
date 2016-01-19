<?php
/**
 * Custom field handler for State Machine Schedule events, displays the event by its label.
 */
class state_flow_schedule_handler_field_state_machine_event extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);

    $state_flow = state_flow_load_state_machine(node_load(NULL, $values->vid));
    $event = $state_flow->get_event($value);
    $options = $event->get_options();

    return $this->sanitize_value($options['label']);
  }
}
