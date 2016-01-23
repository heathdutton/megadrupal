<?php

/**
 * @file
 * Administration pages and forms callbacks.
 */

/**
 * Module settings form callback.
 */
function dsb_portal_prefilter_ui_admin_settings_form($form, $form_state) {
  // Make sure dependencies are loaded before configuring the module.
  if (
    !dsb_portal_check_libraries(TRUE) ||
    !dsb_portal_check_module_dependency(TRUE)
  ) {
    drupal_set_message(t("Please correct the dependency errors before configuring dsb Portal Prefilter UI."), 'error');
    return array();
  }

  $form['facets'] = array(
    '#type' => 'fieldset',
    '#title' => t("Facets settings"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['facets']['dsb_portal_prefilter_ui_facets'] = array(
    '#type' => 'textarea',
    '#title' => t("Activate facets"),
    '#description' => t("Insert a list of facets to activate, one facet per line. Please refer to the !link for the list of available facets.", array('!link' => l(t("official API documentation"), 'https://dsb-api.educa.ch/latest/doc/#api-Search-GetSearch'))),
    '#default_value' => variable_get('dsb_portal_prefilter_ui_facets', ''),
  );

  $form['facets']['dsb_portal_prefilter_ui_override_other_facets'] = array(
    '#type' => 'checkbox',
    '#title' => t("Override other module facets"),
    '#description' => t("By default, the above facets are added alongside the facets activated by other modules. By checking this box, facets added by other modules are ignored, and only the ones above are used."),
    '#default_value' => variable_get('dsb_portal_prefilter_ui_override_other_facets', 0),
  );

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t("Pre-filter settings"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['filter']['dsb_portal_prefilter_ui_prefilters'] = array(
    '#type' => 'textarea',
    '#title' => t("Activate pre-filters"),
    '#description' => t("Insert a list of filters to activate, one filter per line. Each filter must follow the following syntax: <code>filter_name[filter_value1,filter_value2,...,filter_valueN]</code> Example: <code>keywords[Math,Geometry]</code>. If the filter is available as a checkbox (facet), it will be hidden by default. You may optionally only disable it (grayed out and checked by default) it by appending <code>|1</code> (if omitted, it is treated as <code>|0</code>, which means hidden): <code>filter_name[filter_value1]|1</code>. If you wish for the checkbox to be left alone, append <code>|2</code>. Please refer to the !link for the list of available filters.", array('!link' => l(t("official API documentation"), 'https://dsb-api.educa.ch/latest/doc/#api-Search-GetSearch'))),
    '#default_value' => variable_get('dsb_portal_prefilter_ui_prefilters', ''),
  );

  $form['filter']['dsb_portal_prefilter_ui_show_prefilters'] = array(
    '#type' => 'checkbox',
    '#title' => t("Show pre-filters for administrators"),
    '#description' => t("Useful for debugging. This will show the active pre-filter information for all users that have the <em>see debug info for dsb_portal_prefilter_ui</em> permission."),
    '#default_value' => variable_get('dsb_portal_prefilter_ui_show_prefilters', 0),
  );

  return system_settings_form($form);
}
