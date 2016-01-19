<?php
/**
 * @file
 * Advanced theme settings.
 *
 * Complete documentation for this file is available online
 * https://www.drupal.org/node/177868
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function pointnet_red_sun_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL) {
  // Work-around for a core bug affecting admin themes. See issue #943212.
  if (isset($form_id)) {
    return;
  }
}
