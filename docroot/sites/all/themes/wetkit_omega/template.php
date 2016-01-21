<?php

/**
 * @file
 * Template overrides as well as (pre-)process and alter hooks for the
 * WetKit Omega theme.
 */

/**
 * Implements hook_theme().
 */
function wetkit_omega_theme() {
  $info['links__menu_menu_wet_footer'] = array(
    'render element' => 'element',
    'theme path' => drupal_get_path('theme', 'wetkit_omega'),
  );
  $info['links__menu_menu_wet_header'] = array(
    'render element' => 'element',
    'theme path' => drupal_get_path('theme', 'wetkit_omega'),
  );
  $info['links__menu_menu_wet_terms'] = array(
    'render element' => 'element',
    'theme path' => drupal_get_path('theme', 'wetkit_omega'),
  );

  return $info;
}
