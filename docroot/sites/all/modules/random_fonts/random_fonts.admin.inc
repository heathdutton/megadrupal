<?php

/**
 * @file
 * Admin interface for the Random Fonts (random_fonts) module.
 */

/**
 * Implements hook_form().
 *
 * Configuration form for Random Fonts module.
 *
 * @ingroup forms
 */
function random_fonts_configure() {
  $form = array();
  $form['random_fonts_selectors'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Target selectors'),
    '#rows'          => 3,
    '#default_value' => variable_get('random_fonts_selectors', 'p,ul,ol,:header'),
    '#required'      => TRUE,
    '#description'   => t('A comma-separated list of HTML tag names, CSS classes, IDs and such; for advanced options, see !link.', array('!link' => l(t('jQuery documentation'), 'http://api.jquery.com/category/selectors/'))),
  );
  $form['random_fonts_count'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Number of fonts to display simultaneously'),
    '#size'             => 5,
    '#default_value'    => variable_get('random_fonts_count', 10),
    '#required'         => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );
  $form['random_fonts_restricted'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Limit font display to users with %permission permission', array('%permission' => 'administer @font-your-face')),
    '#default_value' => variable_get('random_fonts_restricted', TRUE),
  );
  $form['random_fonts_noadmin'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Exclude admin paths and toolbar'),
    '#default_value' => variable_get('random_fonts_noadmin', TRUE),
  );
  return system_settings_form($form);
}
