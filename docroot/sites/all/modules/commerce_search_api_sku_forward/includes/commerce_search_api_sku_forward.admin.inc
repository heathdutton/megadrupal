<?php
/**
 * @file
 * Module administrative functionality is defined in this file.
 */

/**
 * Administrative form for commerce amazon fulfillment.
 *
 * @return mixed
 *   Returns form object
 */
function commerce_search_api_sku_forward_admin() {
  // Initiate form array.
  $form = array();

  // Description of form for users.
  $form['description'] = array(
    '#markup' => t('Adjust settings here to control exactly which search indexes
are influenced by SKU forwarding, along with configuring relevant node types and
product reference fields.'),
  );

  // Fieldset for amazon credentials.
  $form['search_indexes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search Indexes'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Conditional collapsing of search_indexes fieldset.
  if (variable_get('commerce_search_api_sku_forward_indexes')) {
    $form['search_indexes']['#collapsed'] = TRUE;
  }

  // Select options for search indexes.
  $form['search_indexes']['search_indexes'] = array(
    '#title' => t('Select relevant search indexes.'),
    '#description' => t("The following search indexes will have SKU comparison on them.
 If you do not see your index here, it must not be enabled in the Search API administration."),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_search_api_sku_forward_indexes'),
    '#type' => 'select',
    '#options' => commerce_search_api_sku_forward_get_indexes(),
    '#multiple' => TRUE,
  );

  // Fieldset for node and field options.
  $form['node_and_field_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node and Field Settings'),
    '#weight' => 8,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Select options for which nodes to check against.
  $form['node_and_field_settings']['nodes'] = array(
    '#title' => t('Select relevant node types.'),
    '#description' => t("The following node types will be compared for SKU forwarding."),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_search_api_sku_forward_nodes'),
    '#type' => 'select',
    '#options' => commerce_search_api_sku_forward_get_node_types(),
    '#multiple' => TRUE,
  );

  // Fieldset for field values.
  $form['node_and_field_settings']['field_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field Settings'),
    '#weight' => 4,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('Here is where you enter the field name that is used for product referencing, and the
column in which the Product SKU (ID) is stored. It is a presumption that you will be
using the same field on all nodes for products. A work around to allow for multiple fields does not currently exist.'),
  );

  $form['node_and_field_settings']['field_settings']['field_machine_name'] = array(
    '#title' => t('Enter field machine name.'),
    '#description' => t("This is the field machine name that will be used for product forwarding ."),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_search_api_sku_forward_field_machine_name', 'field_product'),
    '#type' => 'textfield',
  );

  $form['node_and_field_settings']['field_settings']['field_db_column'] = array(
    '#title' => t('Enter field DB Column.'),
    '#description' => t("This is the column which will be referenced. See the DB structure for this."),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_search_api_sku_forward_db_column', 'product_id'),
    '#type' => 'textfield',
  );

  /*
  // Select options for which fields to check against.
  $form['node_and_field_settings']['fields'] = array(
    '#title' => t('Select relevant fields.'),
    '#description' => t("The following fields will be compared for SKU forwarding."),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_search_api_sku_forward_fields'),
    '#type' => 'select',
    '#options' => commerce_search_api_sku_forward_get_fields(),
    '#multiple' => TRUE,
  );
  */

  // Validation and submission handlers.
  $form['#validate'][] = 'commerce_search_api_sku_forward_admin_validate';
  $form['#submit'][] = 'commerce_search_api_sku_forward_admin_submit';

  // Create a system settings form.
  return system_settings_form($form);
}

/**
 * Helper function that returns node types for form select usage.
 *
 * @return array
 *   Associative array of node types.
 */
function commerce_search_api_sku_forward_get_node_types() {
  // Initialize array.
  $node_types_select = array();

  // Take loaded node types and map properly for form select usage.
  foreach (node_type_get_types() as $node_type => $node_object) {
    $node_types_select[$node_type] = $node_type;
  }

  return $node_types_select;
}

/**
 * Returns an associative array of all enabled search indexes.
 *
 * @return array
 *   Associative array of search indexes that are enabled.
 */
function commerce_search_api_sku_forward_get_indexes() {
  // Initialize array to loop over.
  $indexes = array();
  // Loop through all indexes.
  foreach (search_api_index_load_multiple(FALSE) as $index) {
    // If enabled, add to array of indexes to affect.
    if ($index->enabled) {
      $indexes[$index->machine_name] = $index->name;
    }
  }
  // Returns all enabled indexes.
  return $indexes;
}

/**
 * Returns an associative array of all possible product reference fields.
 *
 * THIS IS CURRENTLY NOT BEING USED! It is left around for possible later
 * usage in the 7.x-2.x branch.
 *
 * @return array
 *   An associative array of product reference fields.
 */
function commerce_search_api_sku_forward_get_fields(){
  // Get all fields.
  $fields = array();
  // Loop through all fields in the system.
  foreach (field_info_fields() as $field) {
    // Check if it is a product reference field, and has no entity type
    // (making it node specific).
    if ($field['type'] == 'commerce_product_reference'&& empty($field['entity_types'])) {
      // Add field to the list of fields to return.
      $fields[$field['field_name']] = $field['field_name'];
    }
  }
  return $fields;
}

/**
 * Admin form validation function.
 *
 * Currently this is just a placeholder for later form validation that may
 * occur.
 *
 * @param array $form
 *   Form object.
 *
 * @param array $form_state
 *   Form state object
 */
function commerce_search_api_sku_forward_admin_validate($form, &$form_state) {
  // Validation can occur in here.
}

/**
 * Admin form submission handler.
 *
 * @param object $form
 *   Form object.
 *
 * @param array $form_state
 *   Form state object
 */
function commerce_search_api_sku_forward_admin_submit($form, &$form_state){
  // Set Values for indexes, nodes, fields machine name and field db column.
  variable_set('commerce_search_api_sku_forward_indexes', $form_state['input']['search_indexes']);
  variable_set('commerce_search_api_sku_forward_nodes', $form_state['input']['nodes']);
  //variable_set('commerce_search_api_sku_forward_fields', $form_state['input']['fields']);
  variable_set('commerce_search_api_sku_forward_field_machine_name', $form_state['input']['field_machine_name']);
  variable_set('commerce_search_api_sku_forward_db_column', $form_state['input']['field_db_column']);
}
