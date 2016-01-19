<?php

/**
 * Returns HTML for primary and secondary local tasks.
 *
 * @param $variables
 *   An associative array containing:
 *     - primary: (optional) An array of local tasks (tabs).
 *     - secondary: (optional) An array of local tasks (tabs).
 *
 * @ingroup themeable
 * @see menu_local_tasks()
 */
function twbs_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $values = array();
    foreach ($variables['primary'] as $link) {
      $key = isset($link['#active']) ? 'active' : '';
      $value = $link['#link'] + $link['#link']['localized_options'];
      $values['links'][$key] = $value;
    }
    $values['attributes'] = array(
      'class' => array('primary'),
    );
    $values['heading'] = array(
      'text' => t('Primary tabs'),
      'level' => 'h2',
      'class' => array('sr-only'),
    );
    $output .= theme('twbs_bootstrap_nav', $values);
  }

  if (!empty($variables['secondary'])) {
    $values = array();
    foreach ($variables['secondary'] as $link) {
      $key = isset($link['#active']) ? 'active' : '';
      $value = $link['#link'] + $link['#link']['localized_options'];
      $values['links'][$key] = $value;
    }
    $values['attributes'] = array(
      'class' => array('secondary', 'nav-pills'),
    );
    $values['heading'] = array(
      'text' => t('Secondary tabs'),
      'level' => 'h2',
      'class' => array('sr-only'),
    );
    $output .= theme('twbs_bootstrap_nav', $values);
  }

  return $output;
}
