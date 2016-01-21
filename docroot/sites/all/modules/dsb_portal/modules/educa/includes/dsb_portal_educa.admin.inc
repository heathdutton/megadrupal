<?php

/**
 * @file
 * Module administration callbacks.
 */

/**
 * Module settings form callback.
 */
function dsb_portal_educa_admin_settings_form($form, $form_state) {
  // Make sure dependencies are loaded before configuring the module.
  if (
    !dsb_portal_check_libraries(TRUE) ||
    !dsb_portal_check_module_dependency(TRUE)
  ) {
    drupal_set_message(t("Please correct the dependency errors before configuring dsb Portal Standard Curriculum."), 'error');
    return array();
  }

  $form['dsb_portal_educa_json_url'] = array(
    '#type' => 'textfield',
    '#title' => t("Official Standard Curriculum JSON download URL"),
    '#description' => t("The URL at which the official standard (educa) curriculum definition can be downloaded (JSON)."),
    '#default_value' => variable_get('dsb_portal_educa_json_url', 'http://ontology.biblio.educa.ch/json/educa_standard_curriculum'),
  );

  return system_settings_form($form);
}
