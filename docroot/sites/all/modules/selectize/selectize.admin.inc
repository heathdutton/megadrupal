<?php

/**
 * Configuration form for Selectize.js.
 * @param $form
 * @param $form_state
 * @return mixed
 */
function selectize_admin_settings($form, $form_state) {
  $form['selectize_elements'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS Classes'),
    '#description' => t('Apply Selectize.js to the classes above for particular select elements.'),
    '#default_value' => variable_get('selectize_elements', NULL)
  );

  $form['selectize_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply to all'),
    '#description' => t('Apply Selectize.js to all select elements on the site.'),
    '#default_value' => variable_get('selectize_all', FALSE)
  );

  $form['selectize_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply in admin areas'),
    '#description' => t('Apply Selectize.js while in admin areas. Unselect this to ensure compatibility with the backend.'),
    '#default_value' => variable_get('selectize_admin', FALSE)
  );

  return system_settings_form($form);
}