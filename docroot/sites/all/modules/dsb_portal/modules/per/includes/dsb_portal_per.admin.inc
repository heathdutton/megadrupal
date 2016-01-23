<?php

/**
 * @file
 *
 */

/**
 * Module settings form callback.
 */
function dsb_portal_per_admin_settings_form($form, $form_state) {
  // Make sure dependencies are loaded before configuring the module.
  if (
    !dsb_portal_check_libraries(TRUE) ||
    !dsb_portal_check_module_dependency(TRUE)
  ) {
    drupal_set_message(t("Please correct the dependency errors before configuring dsb Portal PER."), 'error');
    return array();
  }

  $form['dsb_portal_per_search_alter'] = array(
    '#type' => 'checkbox',
    '#title' => t("Activate PER objective code filter in search query"),
    '#description' => t("If a user types an objective code in the full-text search, this will activate the PER objective code filter. If this is not activated, the full-text will simply be handled by the REST API, but it may not recognize the code as such."),
    '#default_value' => variable_get('dsb_portal_per_search_alter', FALSE),
  );

  $form['facets'] = array(
    '#type' => 'fieldset',
    '#title' => t("PER facets"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['facets']['dsb_portal_per_facets'] = array(
    '#type' => 'checkboxes',
    '#title' => t("Activate facets"),
    '#description' => t("The national catalog has different facets related to the PER curriculum that can be activated for search. These will be displayed as a series of checkboxes on the search form."),
    '#options' => array(
      'per_full' => t("PER full curriculum", array(), array('context' => 'dsb_portal_per:search')),
      'perCycle' => t("PER cycle", array(), array('context' => 'dsb_portal_per:search')),
      'perObjectiveCode' => t("PER objective code", array(), array('context' => 'dsb_portal_per:search')),
      'perDomain' => t("PER domain", array(), array('context' => 'dsb_portal_per:search')),
      'perDiscipline' => t("PER discipline", array(), array('context' => 'dsb_portal_per:search')),
    ),
    '#default_value' => variable_get('dsb_portal_per_facets', array('per_full' => 'per_full')),
  );

  $form['site'] = array(
    '#type' => 'fieldset',
    '#title' => t("PER official site"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['site']['dsb_portal_per_site_url'] = array(
    '#type' => 'textfield',
    '#title' => t("Official PER site URL"),
    '#description' => t("The URL at which the PER can be visited."),
    '#default_value' => variable_get('dsb_portal_per_site_url', 'http://www.plandetudes.ch'),
  );

  $form['site']['dsb_portal_per_code_url'] = array(
    '#type' => 'textfield',
    '#title' => t("Official PER site URL for a specific code"),
    '#description' => t("The URL where to find more information about a specific PER objective code. Use the [CODE] token to specify where the objective code should be placed in the URL. For example: <em>http://www.plandetudes.ch/web/guest/[CODE]</em>."),
    '#default_value' => variable_get('dsb_portal_per_code_url', 'http://www.plandetudes.ch/web/guest/[CODE]'),
  );

  $form['site']['dsb_portal_per_xml_url'] = array(
    '#type' => 'textfield',
    '#title' => t("Official PER XML download URL"),
    '#description' => t("The URL at which the official PER curriculum definition can be downloaded (XML)."),
    '#default_value' => variable_get('dsb_portal_per_xml_url', 'http://ontology.biblio.educa.ch/curriculum/latest.xml'),
  );

  return system_settings_form($form);
}
