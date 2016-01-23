<?php
// $Id: custompage_context_condition_alias.inc,v 1.1.2.1.2.1 2011/01/19 22:41:10 faris Exp $

/**
 * Expose node views/node forms of specific node types as a context condition.
 */
class custompage_context_condition_alias extends context_condition {

  function condition_values() {
    $values = _custompage_context_get_custompages();
    return $values;
  }

  function execute( $path_key ) {
    if (empty($path_key)) {
      return;
    }

    foreach (context_enabled_contexts() as $context) {
      if ($paths = $this->fetch_from_context($context, 'values')) {
        if (in_array($path_key, $paths)) {
          $this->condition_met($context, $path_key);
        }
      }
    }

  }
}
