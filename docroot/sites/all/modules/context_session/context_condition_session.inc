<?php
/**
 * @file
 * Provide context_condition class implmentation for session.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

class context_condition_session extends context_condition {

  /**
   * Provide list of session variable keys as requested on settings page.
   */
  function condition_values() {
    return variable_get('context_session', array());
  }

  /**
   * Check to see if the requested session keys are current available.
   */
  function execute(array $session) {
    $keys = variable_get('context_session', array());
    foreach ($keys as $key) {
      if (isset($session[$key])) {
        foreach ($this->get_contexts($key) as $context) {
          $this->condition_met($context, $key);
        }
      }
    }
  }
}
