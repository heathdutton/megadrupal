<?php
/**
 * @file
 * Context condition plugin for Context Pantheon.
 */

/**
 * Expose the current Pantheon server environment as a context condition.
 */
class context_condition_pantheon_environment extends context_condition {
  /**
   * Define allowed condition values.
   */
  function condition_values() {
    return array(
      CONTEXT_PANTHEON_LOCAL_KEY => t('Local development (non-Pantheon)'),
      CONTEXT_PANTHEON_DEV_KEY => t('Pantheon dev environment'),
      CONTEXT_PANTHEON_TEST_KEY => t('Pantheon test environment'),
      CONTEXT_PANTHEON_LIVE_KEY => t('Pantheon live environment'),
    );
  }

  /**
   * Execute.
   */
  function execute() {
    foreach ($this->get_contexts() as $context) {
      $environment_match = FALSE;
      foreach((array) $context->conditions['pantheon_environment']['values'] as $key => $value) {
        // Local environment has no Pantheon server variable.
        if (!defined('PANTHEON_ENVIRONMENT')) {
          if ($key === CONTEXT_PANTHEON_LOCAL_KEY) {
            $environment_match = TRUE;
          }
        }
        else {
          if ($key === PANTHEON_ENVIRONMENT) {
            $environment_match = TRUE;
          }
        }
      }

      // If we have an environment match, let Context know.
      if ($environment_match) {
        $this->condition_met($context);
      }
    }
  }

}
