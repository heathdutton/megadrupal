<?php
/**
 * @file
 * Defines Rules conditions for checking if a line item's product has a
 * specified taxonomy term and for calculating the total quantity of
 * products in an order that has a specified taxonomy term.
 */
 
/**
 * Implements hook_rules_conditions_info()
 */
function commerce_conditions_rules_condition_info() {
  // Define the condition for checking if a line item has a product with a
  // specified term
  $conditions['commerce_conditions_rules_condition_has_term'] = array(
    'group' => t('Commerce Line Item'),
    'label' => t('Line item product has term'),
    'parameter' => array(
      'line_item' => array(
        'type' => 'commerce_line_item', 
        'label' => t('Commere Line Item'),
      ),
      'field_name' => array(
        'type' => 'text',
        'label' => t('Term Reference Field'),
        'description' => t('The machine-name of the expected product\'s term reference field'),
        'options list' => 'commerce_conditions_term_fields_options_list',
        'restriction' => 'input',
      ),
      'term_id' => array(
        'type' => 'integer',
        'label' => t('Taxonomy Term'),
        'description' => t('The term being checked for'),
        'restriction' => 'input',
      ),
    ),
  );
  
  // Define the condition for checking if a line item has a product with
  // specified terms
  $conditions['commerce_conditions_rules_condition_has_terms'] = array(
    'group' => t('Commerce Line Item'),
    'label' => t('Line item product has terms'),
    'parameter' => array(
      'line_item' => array(
        'type' => 'commerce_line_item', 
        'label' => t('Commere Line Item'),
      ),
      'field_name' => array(
        'type' => 'text',
        'label' => t('Term Reference Field'),
        'description' => t('The machine-name of the expected product\'s term reference field'),
        'options list' => 'commerce_conditions_term_fields_options_list',
        'restriction' => 'input',
      ),
      'term_ids' => array(
        'type' => 'list<integer>',
        'label' => t('Taxonomy Terms'),
        'description' => t('The terms being checked for'),
        'options list' => 'commerce_conditions_empty_options_list',
        'restriction' => 'input',
      ),
      'require_all' => array(
        'type' => 'boolean',
        'label' => t('Require All'),
        'description' => t('If checked the condition will only return TRUE if
          the product has all of the selected terms.'),
        'restriction' => 'input'
      ),
    ),
  );
  
  // Define the condition for comparing the quantity of products with a
  // specified term in an order to a value
  $conditions['commerce_conditions_compare_termed_product_quantity'] = array(
    'group' => t('Commerce Order'),
    'label' => t('Total product with term quantity comparison'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'description' => t('The order whose product line item quantities should be totalled. If the specified order does not exist, the comparison will act as if it is against a quantity of 0.'),
      ),
      'field_name' => array(
        'type' => 'text',
        'label' => t('Term Reference Field'),
        'description' => t('The machine-name of the expected product\'s term reference field'),
        'options list' => 'commerce_conditions_term_fields_options_list',
        'restriction' => 'input',
      ),
      'term_id' => array(
        'type' => 'integer',
        'label' => t('Taxonomy Term'),
        'description' => t('The term being checked for'),
        'restriction' => 'input',
      ),
      'operator' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The comparison operator to use against the total number of products matching the term on the order.'),
        'default value' => '>=',
        'options list' => 'commerce_numeric_comparison_operator_options_list',
        'restriction' => 'input',
      ),
      'value' => array(
        'type' => 'integer',
        'label' => t('Quantity'),
        'default value' => 1,
        'description' => t('The value to compare against the total quantity of products matching the term on the order.'),
      ),
    ),
  );
  
  $conditions['commerce_conditions_compare_total_product_type_quantity'] = array(
    'group' => t('Commerce Order'),
    'label' => t('Total product of type quantity comparison'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'description' => t('The order whose product line item quantities should be totalled. If the specified order does not exist, the comparison will act as if it is against a quantity of 0.'),
      ),
      'product_type' => array(
        'type' => 'text',
        'label' => t('Product Type'),
        'description' => t('The type of the product to look for on the order.'),
        'options list' => 'commerce_product_type_options_list',
        'restriction' => 'input',
      ),
      'exclude' => array(
        'type' => 'boolean',
        'label' => t('Exlude'),
        'description' => t('If checked the 
          quantity of all products on the order excluding the product type 
          specified above will be calculated.'),
        'restriction' => 'input'
      ),
      'operator' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The comparison operator to use against the total number of products on the order.'),
        'default value' => '>=',
        'options list' => 'commerce_numeric_comparison_operator_options_list',
        'restriction' => 'input',
      ),
      'value' => array(
        'type' => 'integer',
        'label' => t('Quantity'),
        'default value' => 1,
        'description' => t('The value to compare against the total quantity of products on the order.'),
      ),
    ),
  );

  return $conditions;
}

/**
 * Alters the form for commerce_conditions_rules_condition_has_term
 * so that we can require the term reference field be selected first so that
 * the term_id list can be populated.
 */ 
function commerce_conditions_rules_condition_has_term_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  if (!empty($options['init']) && !isset($form_state['rules_element_step'])) {
    unset($form['parameter']['term_id']);
    $form_state['rules_element_step'] = 1;
    $form['negate']['#access'] = FALSE;
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
      '#limit_validation_errors' => array(array('parameter', 'list')),
      '#submit' => array('rules_form_submit_rebuild'),
    );
  }
  else {
    // Change the list parameter to be not editable any more.
    $form['parameter']['field_name']['settings']['#access'] = FALSE;
    $form['parameter']['field_name']['info'] = array(
      '#prefix' => '<p>',
      '#markup' => t('<strong>Selected term field:</strong> %selector', array('%selector' => $element->settings['field_name'])),
      '#suffix' => '</p>',
    );
    $form['parameter']['term_id']['settings']['term_id']['#type'] = 'select';
    $form['parameter']['term_id']['settings']['term_id']['#options'] = commerce_conditions_get_terms_list($element->settings['field_name']);
  }
}

/**
 * Alters the form for commerce_conditions_rules_condition_has_terms
 * so that we can require the term reference field be selected first so that
 * the term_ids list can be populated.
 */ 
function commerce_conditions_rules_condition_has_terms_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  if (!empty($options['init']) && !isset($form_state['rules_element_step'])) {
    unset($form['parameter']['term_ids'], $form['parameter']['require_all']);
    $form_state['rules_element_step'] = 1;
    $form['negate']['#access'] = FALSE;
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
      '#limit_validation_errors' => array(array('parameter', 'list')),
      '#submit' => array('rules_form_submit_rebuild'),
    );
  }
  else {
    // Change the list parameter to be not editable any more.
    $form['parameter']['field_name']['settings']['#access'] = FALSE;
    $form['parameter']['field_name']['info'] = array(
      '#prefix' => '<p>',
      '#markup' => t('<strong>Selected term field:</strong> %selector', array('%selector' => $element->settings['field_name'])),
      '#suffix' => '</p>',
    );
    $form['parameter']['term_ids']['settings']['term_ids']['#options'] = commerce_conditions_get_terms_list($element->settings['field_name']);
  }
}

/**
 * Alters the form for 
 * commerce_conditions_compare_termed_product_quantity so that we
 * can require the term reference field be selected first so that the term_id
 * list can be populated.
 */
function commerce_conditions_compare_termed_product_quantity_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  if (!empty($options['init']) && !isset($form_state['rules_element_step'])) {
    unset($form['parameter']['term_id'], $form['parameter']['operator'], $form['parameter']['value']);
    $form_state['rules_element_step'] = 1;
    $form['negate']['#access'] = FALSE;
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
      '#limit_validation_errors' => array(array('parameter', 'list')),
      '#submit' => array('rules_form_submit_rebuild'),
    );
  }
  else {
    // Change the list parameter to be not editable any more.
    $form['parameter']['field_name']['settings']['#access'] = FALSE;
    $form['parameter']['field_name']['info'] = array(
      '#prefix' => '<p>',
      '#markup' => t('<strong>Selected term field:</strong> %selector', array('%selector' => $element->settings['field_name'])),
      '#suffix' => '</p>',
    );
    $form['parameter']['term_id']['settings']['term_id']['#type'] = 'select';
    $form['parameter']['term_id']['settings']['term_id']['#options'] = commerce_conditions_get_terms_list($element->settings['field_name']);
  }
}
