<?php

/**
 * Returns HTML for a system settings form.
 *
 * By default this does not alter the appearance of a form at all,
 * but is provided as a convenience for themers.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function twbs_system_settings_form($variables) {
  return drupal_render_children($variables['form']);
}
