<?php
/**
 * @file
 */

$plugin = array(
  'schema' => 'ec_fields',
  'access' => EC_PERM_MANAGE,
  'menu' => array(
    'menu prefix' => 'admin/config/store',
    'menu item' => 'fields',
    'menu title' => 'Fields',
    'menu description' => 'Create additional fields to be displayed during checkout for use by other modules.',
  ),
  'title singular' => t('field'),
  'title singular proper' => t('Field'),
  'title plural' => t('fields'),
  'title plural proper' => t('Fields'),
  'form' => array(
    'settings' => 'ec_store_fields_settings',
    'validate' => 'ec_store_fields_settings_validate',
    'submit' => 'ec_store_fields_settings_submit',
  ),
);

function ec_store_fields_settings(&$form, &$form_state) {
  ctools_include('dependent');
  
  if (empty($form_state['item']->field_type)) { 
    $plugins = ec_store_get_field_plugins($form_state['item']->field_type);
  }
  else {
    $plugin = ec_store_get_field_plugins($form_state['item']->field_type);
    $plugins = array($plugin['name'] => $plugin);
  }
  if (empty($form_state['item']->field_type)) {
    $options = array();
    foreach ($plugins as $name => $plugin) {
      $options[$name] = $plugin['title'];
    }
    asort($options);
    
    $form['field_type'] = array(
      '#type' => 'select',
      '#title' => t('Type of field'),
      '#options' => array_merge(array('' => t('- Select type -')), $options),
      '#required' => TRUE,
    );
  }
  else {
    $form['field_type'] = array(
      '#type' => 'value',
      '#value' => $form_state['item']->field_type,
    );
  }
  
  $form['field_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $form_state['item']->field_title,
    '#description' => t('Title of the field that will be shown on the checkout'),
    '#required' => TRUE,
  );
  
  foreach ($plugins as $name => $plugin) {
    if (isset($plugin['settings']['form']) && function_exists($plugin['settings']['form'])) {
      $elements = $plugin['settings']['form']($form_state);
      foreach (element_children($elements) as $id) {
        $elements[$id]['#process'] = array('ctools_dependent_process');
        $elements[$id]['#dependency'] = array('edit-field-type' => array($name));
      }
      $form['fields']['#tree'] = TRUE;
      $form['fields'][$name] = $elements;
      $form['fields'][$name]['#prefix'] = '<div class="ec-fields-' . $name . '">';
      $form['fields'][$name]['#suffix'] = '</div>';
    }
  }
  
  $form['field_settings'] = array(
    '#type' => 'value',
    '#value' => '',
  );
  
  $form['field_description'] = array(
    '#type' => 'textarea',
    '#title' => t('description'),
    '#default_value' => $form_state['item']->field_title,
    '#description' => t('Title of the field that will be shown on the checkout'),
  );
  $form['field_error'] = array(
    '#type' => 'textarea',
    '#title' => t('Error'),
    '#default_value' => $form_state['item']->field_error,
    '#description' => t('Error to be displayed when this field has not meet any criteria or been used by other modules.'),
  );
}

function ec_store_fields_settings_validate(&$form, &$form_state) {  
  $plugin = ec_store_get_field_plugins($form_state['values']['field_type']);
  
  if (isset($plugin['settings']['validate']) && function_exists($plugin['settings']['validate'])) {
    $plugin['settings']['validate']($form['fields'][$plugin['name']], $form_state);
  }
  
  form_set_value($form['field_settings'], $form_state['values']['fields'][$plugin['name']], $form_state);
}

function ec_store_fields_settings_submit(&$form, &$form_state) {
  $plugin = ec_store_get_field_plugins($form_state['values']['field_type']);
  
  if (isset($plugin['settings']['submit']) && function_exists($plugin['settings']['submit'])) {
    $plugin['settings']['submit']($form['fields'][$plugin['name']], $form_state);
  }
}