<?php
/**
 * @file
 */

/**
 * Implementation of hook_checkout_init().
 */
function ec_store_fields_checkout_init(&$txn) {
  $txn->fields = array();
  $txn->fields_used = array();
}

/**
 * Implementation of hook_checkout_form().
 */
function ec_store_fields_checkout_form(&$form, &$form_state) {
  ctools_include('export');
  $txn =& $form_state['txn'];
  
  $fields = ec_store_get_all_fields();
  $form['ec_store'] = array(
    'fields' => array(
      '#tree' => TRUE,
    ),
  );

  foreach ($fields as $name => $field) {
    if ($plugin = ec_store_get_field_plugins($field->field_type)) {
      if (isset($plugin['render']) && function_exists($plugin['render'])) {
        $element = $plugin['render']($form, $form_state, $field);
        $element['#default_value'] = isset($txn->fields[$name]) ? $txn->fields[$name] : NULL;
        $form['ec_store']['fields'][$field->name] = $element;
      }
    }
  }
}

function ec_store_fields_checkout_post_validate(&$form, &$form_state) {
  ctools_include('export');
  $txn =& $form_state['txn'];
  
  $fields = ec_store_get_all_fields();
  
  foreach ($fields as $name => $field) {
    if (!empty($txn->fields[$name]) && !in_array($name, $txn->fields_used) && $field->field_error) {
      form_set_error('fields][' . $name, $field->field_error);
    }
  }
}

function ec_store_fields_checkout_update(&$form, &$form_state) {
  $txn =& $form_state['txn'];
  
  $txn->fields = isset($form_state['values']['fields']) ? $form_state['values']['fields'] : array();
}
