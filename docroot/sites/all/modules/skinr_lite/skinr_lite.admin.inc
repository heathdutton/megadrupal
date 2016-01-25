<?php

/**
 * @file
 *
 */

/**
 * Page callback function for enabling the skinr lite theme chooser
 */
function skinr_lite_enable_chooser() {
  variable_set('skinr_lite_chooser_enabled', TRUE);
  drupal_set_message(t('The theme option chooser is enabled. You can view it by clicking the eye icon on the right.'));
  drupal_goto();
}
