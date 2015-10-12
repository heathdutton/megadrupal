<?php

/**
 * @file
 * Context condition "is_mobile".
 */

/**
 * Provide a context condition to reacto on mobile devices detected by browscap.
 */
class browscap_context_context_condition_is_mobile extends context_condition {

  /**
   * Define values used in this condition.
   */
  function condition_values() {
    return array(
      'not_mobile' => t('Is no mobile device'),
      'mobile' => t('Is mobile device'),
    );
  }

  /**
   * Executes this plugin.
   */
  function execute() {
    if (function_exists('browscap_get_browser')) {

      $browser = browscap_get_browser();

      if (isset($browser['ismobiledevice']) && $browser['ismobiledevice'] == 'true') {
        $type = 'mobile';
      }
      else {
        $type = 'not_mobile';
      }

      foreach ($this->get_contexts($type) as $context) {
        $this->condition_met($context, $type);
      }
    }
  }

}
