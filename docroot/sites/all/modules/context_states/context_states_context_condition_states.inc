<?php
/**
 * @file
 * Expose States value(s) as context conditions
 */

class context_states_context_condition_states extends context_condition {
  /**
   * Condition values.
   */
  function condition_values() {
    $values = array();
    $states = preg_split('/[\r\n]+/', variable_get('context_states_defined_states', ''), -1, PREG_SPLIT_NO_EMPTY);
    foreach ($states as $setstate) {
      $state_var = strtolower(str_replace(" ", "_", $setstate));
      $values[$state_var] = check_plain($setstate);
    }
    return $values;
  }

  /**
   * Condition execution.
   */
  function execute() {
    $states = preg_split('/[\r\n]+/', variable_get('context_states_defined_states', ''), -1, PREG_SPLIT_NO_EMPTY);
    foreach ($states as $key => &$value) {
      $state_var = strtolower(str_replace(" ", "_", $value));
      $state = variable_get('context_states_states_' . $key);
      foreach ($this->get_contexts($state_var) as $context) {
        if ($state == 1) {
          $this->condition_met($context, $value);
        }
      }
    }
  }
}
