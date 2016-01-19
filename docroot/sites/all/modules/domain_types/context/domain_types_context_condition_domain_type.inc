<?php

/**
 * @file
 * Expose current domain as a context condition.
 */

class domain_types_context_condition_domain_type extends context_condition {

  function condition_values() {
    return _domain_types_names();
  }

  function execute($value) {
    foreach ($this->get_contexts($value) as $context) {
      $this->condition_met($context, $value);
    }
  }

}
