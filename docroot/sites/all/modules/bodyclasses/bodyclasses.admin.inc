<?php

/**
 * @file
 * Admin page callbacks for the Body Classes module.
 */

/**
 * Menu callback for the Body Classes module to display its administration.
 */
function bodyclasses_admin($form, $form_state, $lang = NULL) {
  $form = array();

  // Create the check list of body classes to add.
  $form['bodyclasses'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Body Classes'),
    '#default_value' => variable_get('bodyclasses', array()),
    '#description' => t('Select which classes to inject into the body tag.'),
  );

  // Add the hooks as options.
  $bodyclasses = module_invoke_all('bodyclasses_info');
  foreach ($bodyclasses as $name => $bodyclass) {
    $form['bodyclasses']['#options'][$name] = $bodyclass['name'];
  }

  return system_settings_form($form);
}
