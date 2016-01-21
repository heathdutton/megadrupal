<?php

/**
 * Expose subsite blueprint as a context condition.
 */
class dominion_context_condition_blueprint extends context_condition {
  /**
   * Provide values for condition configuration.
   */
  function condition_values() {
    return db_select('dominion_blueprint', 'b')
      ->fields('b', array('blueprint', 'name'))
      ->orderBy('b.is_default', 'desc')
      ->orderBy('b.name', 'asc')
      ->execute()
      ->fetchAllKeyed();
  }
  
  /**
   * Validate condition for all contexts.
   */
  function execute() {
    if ($this->condition_used() && ($subsite = dominion_get_info())) {
      foreach ($this->get_contexts() as $context) {
        $blueprints = $this->fetch_from_context($context, 'values');
        if (!empty($blueprints[$subsite->blueprint])) {
          $this->condition_met($context);
        }
      }
    }
  }
}
