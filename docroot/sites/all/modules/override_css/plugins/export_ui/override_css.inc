<?php
/**
 * @file
 * CTools Export UI plugin for Configuration Builder pages.
 */

/**
 * CTools Export UI required function for plugin definition
 */
function override_css_override_css_ctools_export_ui() {
  return array(
    'schema' => 'override_css',
    'access' => 'override css admin',

    'menu' => array(
      'menu prefix' => 'admin/appearance',
      'menu item' => 'override_css',
      'menu title' => 'Override css',
      'menu description' => 'Administer Override css.',
    ),

    'title singular' => t('selector group'),
    'title singular proper' => t('Selector group'),
    'title plural' => t('selector groups'),
    'title plural proper' => t('Selector groups'),

    'handler' => 'override_css_ui',

    'form' => array(
      'settings' => 'override_css_export_ui_form',
      'submit' => 'override_css_export_ui_form_submit',
    ),

    'export' => array(
      'admin_title' => 'title',
      'admin_description' => 'description',
    ),
  );
}

/**
 * Override css settings form.
 */
function override_css_export_ui_form(&$form, &$form_state) {
  $item = $form_state['item'];

  // Basic information fieldset.
  $form['info']['#title'] = t('Basic information');
  $form['info']['#type'] = 'fieldset';
  $form['info']['#collapsible'] = TRUE;
  $form['info']['#collapsed'] = $form_state['op'] != 'add';

  $form['bundle'] = array(
    '#type' => 'textfield',
    '#title' => t('Bundle'),
    '#default_value' => $item->bundle,
    '#description' => t('Bundle for css selector groups.'),
    '#required' => FALSE,
    '#access' => variable_get('override_css_bundle_enabled', FALSE),
  );

  $form['selectors'] = array(
    '#type' => 'textarea',
    '#title' => t('Selectors'),
    '#default_value' => $item->selectors,
    '#description' => t('Selectors of this group, one per line.'),
    '#required' => TRUE,
  );

  $form['properties'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Properties'),
    '#options' => override_css_properties_list(TRUE),
    '#default_value' => (array) $item->properties,
    '#required' => TRUE,
  );

  $form['properties_values'] = array(
    '#type' => 'value',
    '#value' => isset($item->properties_values) ? $item->properties_values : array(),
  );
}

function override_css_export_ui_form_submit(&$form, &$form_state) {
  override_css_store_css();
}
