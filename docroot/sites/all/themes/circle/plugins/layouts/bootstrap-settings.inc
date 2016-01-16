<?php

/**
 * @file
 * Contains the theme function overrides for forms.
 */

/**
 * Add container class to pane content and add classes to region.
 */
function _circle_bootstrap_layout_settings_form($display, $layout, $layout_settings) {
  $form['use_container'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use .container wrapper'),
    '#default_value' => (isset($layout_settings['use_container'])) ? $layout_settings['use_container'] : 1,
  );
  $form['panel_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Panel additional class'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => isset($layout_settings['panel_class']) ? $layout_settings['panel_class'] : '',
  );
  $form['#collapsible'] = TRUE;
  $form['#collapsed'] = TRUE;

  return $form;
}
