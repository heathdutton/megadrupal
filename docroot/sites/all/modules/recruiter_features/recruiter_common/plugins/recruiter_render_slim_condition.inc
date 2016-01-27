<?php

/**
 * @file
 * Context calculation for job rendering.
 */

/**
 * Context calculation (slim parameter).
 */
class recruiter_render_slim_condition extends context_condition {

  function condition_values() {
    return array(1 => t('True'));
  }

  function execute() {
    if (isset($_GET['render']) && $_GET['render'] == 'slim') {
      foreach ($this->get_contexts() as $context) {
        $this->condition_met($context, TRUE);
      }
    }
  }
}
