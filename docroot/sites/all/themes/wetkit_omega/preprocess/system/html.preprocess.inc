<?php
/**
 * @file
 * html.preprocess.inc
 */


/**
 * Implements hook_preprocess_html().
 */
function wetkit_omega_preprocess_html(&$variables, $hook) {
  global $theme_key, $language;

  // Return early, so the maintenance page does not call any of the code below.
  if ($hook != 'html') {
    return;
  }

  // Modify html attributes.
  $variables['html_attributes_array']['lang'][] = $language->language;
  $variables['html_attributes_array']['dir'][] = $language->dir;

  unset($variables['html_attributes_array']['dir']);

  // Add the default body id needed
  // WetKit Layouts may have already set this variable.
  if (empty($variables['wetkit_col_array'])) {
    $variables['wetkit_col_array'] = 'wb-body';
  }

  // Add a body class for the active theme name.
  $variables['classes_array'][] = drupal_html_class($theme_key);

  // Assign skip link variables.
  $variables['wetkit_skip_link_id_1'] = omega_theme_get_setting('wetkit_skip_link_id_1');
  $variables['wetkit_skip_link_text_1'] = t('Skip to main content');
  $variables['wetkit_skip_link_id_2'] = omega_theme_get_setting('wetkit_skip_link_id_2');
  $variables['wetkit_skip_link_text_2'] = t('Skip to footer');

  // Remove Classes.
  $variables['classes_array'] = array_diff($variables['classes_array'], array('site-name-web-experience-toolkit'));
}
