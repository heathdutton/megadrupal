<?php

/**
 * @file
 * Contains the administrative functions of the prev_next_views module.
 *
 * @ingroup print
 */

/**
 * Menu callback for the prev_next_views module settings form.
 *
 * @ingroup forms
 */
function prev_next_views_settings() {
  $options = array();
  foreach (views_get_all_views() as $key => $view) {
    $options[$key] = $view->name;
  }

  $form['prev_next_views_enabled'] = array(
    '#type' => 'select',
    '#title' => t('Enable the Prev/Next API for the following views'),
    '#default_value' => variable_get('prev_next_views_enabled', array()),
    '#options' => $options,
    '#multiple' => TRUE,
  );

  return system_settings_form($form);
}
