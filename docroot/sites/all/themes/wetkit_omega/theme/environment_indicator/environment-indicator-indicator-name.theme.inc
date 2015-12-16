<?php
/**
 * @file
 * environment-indicator-indicator-name.theme.inc
 */


/**
 * Theme function for the indicator name.
 */
function wetkit_omega_environment_indicator_indicator_name($variables) {
  $output = '<div class="environment-indicator-name">' . $variables['name'] . '</div>';
  $environments = environment_indicator_get_all(TRUE);
  if (!empty($environments)) {
    $destination = drupal_get_destination();
    foreach ($environments as $environment) {
      $items[] = array(
        'data' => l(t('Open in %name', array('%name' => $environment->name)), 'http://' . $environment->regexurl . '/' . $destination['destination'], array('html' => TRUE)),
        'class' => array('environment-switcher'),
      );
    }
    $switcher = theme('item_list', array(
      'items' => $items,
      'attributes' => array(
        'class' => array('environment-switcher-container'),
      ),
    ));
    $output .= '<div class="item-list">' . $switcher . '</div>';
  }
  return $output;
}
