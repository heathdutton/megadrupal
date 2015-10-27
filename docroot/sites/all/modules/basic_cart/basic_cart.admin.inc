<?php

/**
 * @file
 * Basic cart admin settings forms.
 */

/**
 * Callback for the admin configuration page.
 */
function basic_cart_admin_content_type() {
  module_load_include('inc', 'basic_cart', 'basic_cart.cart');
  $node_types = node_type_get_types();
  if (empty($node_types)) {
    return NULL;
  }

  $options = array();
  foreach ($node_types as $node_type => $type) {
    if ($node_type == 'order' && module_exists('basic_cart_order')) {
      continue;
    }
    $options[$node_type] = check_plain($type->name);
  }
  $default_value = array();
  
  foreach (basic_cart_product_types() as $product_type) {
    if (isset($options[$product_type])){
      $default_value[$product_type] = $product_type;
    }
  }

  $form['content_type'] = array(
    '#title' => t('Content type selection'),
    '#type' => 'fieldset',
    '#description' => t('Please select the content types for which you wish to have the "Add to cart" option.'),
  );

  $form['content_type']['basic_cart_content_types'] = array(
    '#title' => t('Content types'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $default_value,
  );

  $form['currency'] = array(
    '#title' => t('Currency and price'),
    '#type' => 'fieldset',
    '#description' => t('Please select the currency in which the prices will be calculated.'),
  );

  $form['currency']['basic_cart_currency'] = array(
    '#title' => t('Currency'),
    '#type' => 'textfield',
    '#description' => t("Please choose the currency."),
    '#default_value' => variable_get('basic_cart_currency'),
  );

  $form['currency']['basic_cart_price_format'] = array(
    '#title' => t('Price format'),
    '#type' => 'select',
    '#options' => _basic_cart_price_format(),
    '#description' => t("Please choose the format in which the price will be shown."),
    '#default_value' => variable_get('basic_cart_price_format'),
  );
  
  $form['vat'] = array(
    '#title' => t('VAT'),
    '#type' => 'fieldset',
  );

  $form['vat']['basic_cart_vat_state'] = array(
    '#title' => t('Check if you want to apply the VAT tax on the total amount in the checkout process.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('basic_cart_vat_state', FALSE),
  );

  $form['vat']['basic_cart_vat_value'] = array(
    '#title' => t('VAT value'),
    '#type' => 'textfield',
    '#description' => t("Please enter VAT value."),
    '#field_suffix' => '%',
    '#size' => 10,
    '#default_value' => variable_get('basic_cart_vat_value', ''),
  );
  
  $form['redirect'] = array(
    '#title' => t('Redirect user after adding an item to the shopping cart'),
    '#type' => 'fieldset',
  );
  
  $form['redirect']['basic_cart_redirect_user_after_add_to_cart'] = array(
    '#title' => t('Add to cart redirect'),
    '#type' => 'textfield',
    '#description' => t("Enter the page you wish to redirect the customer to when an item is added to the cart, or &lt;none&gt; for no redirect."),
    '#default_value' => variable_get('basic_cart_redirect_user_after_add_to_cart'),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}

/**
 * Callback validation function for the settings page.
 */
function basic_cart_admin_content_type_validate($form, &$form_state) {
  $vat_is_enabled = (int) $form_state['values']['basic_cart_vat_state'];
  if (!empty ($vat_is_enabled) && $vat_is_enabled) {
    $vat_value = (float) $form_state['values']['basic_cart_vat_value'];
    if ($vat_value <= 0) {
      form_set_error('basic_cart_vat_value', t('Please enter a valid figure for the VAT.'));
    }
  }
}

/**
 * Callback submit function for the settings page.
 */
function basic_cart_admin_content_type_submit($form, &$form_state) {
  // Remove internal Form API values.
  unset($form_state['values']['form_id'], $form_state['values']['form_token'], $form_state['values']['form_build_id'], $form_state['values']['op']);

  $product_types = array();
  
  // Setting up the price field for the selected content types.
  $content_types = $form_state['values']['basic_cart_content_types'];
  
  // Unset to prevent 'double' save
  unset($form_state['values']['basic_cart_content_types']);
  
  if (!empty($content_types) && is_array($content_types)) {
    foreach (basic_cart_get_fields() as $field_name => $field_) {
      // Check to see if the field already exists.
      $field = field_info_field($field_name);
      // If the field does not exist then create it.
      if (empty($field)) {
        $field = array(
          'field_name' => $field_name,
          'type' => $field_['type'],
          'entity_types' => array('node'),
        );
        field_create_field($field);
      }
    }
    
    foreach ($content_types as $type => $checked) {
      
      // If a node type is checked, then create the price field.
      if ($checked) {
        // Save content_type as a product.
        $product_types[$type] = $type;
        
        foreach (basic_cart_get_fields() as $field_name => $field_) {
          // Foreach checked content type, we must assign the price field to the content type.
          $instance = field_info_instance('node', $field_name, $type);

          if (empty($instance)) {
            $instance = array(
              'field_name' => $field_name,
              'label' => $field_['title'],
              'description' => $field_['description'],
              'entity_type' => 'node',
              'bundle' => $type,
            );
            // It doesn't exist. Create it.
            field_create_instance($instance);
          }
        }
      }
      // If not, then delete the instance.
      else {
        foreach (basic_cart_get_fields() as $field_name => $field_) {
          $instance = field_info_instance('node', $field_name, $type);
          if (!empty($instance)) {
            field_delete_instance($instance);
          }
        }
      }
    }
  }

  variable_set('basic_cart_content_types', $product_types);
  
  // Set VAT to nothing if the checkbox is unchecked.
  if (empty($form_state['values']['basic_cart_vat_state'])) {
    $form_state['values']['basic_cart_vat_value'] = '';
    // Check to see if the VAT instance exists and if so, delete it.
    if (module_exists('basic_cart_order')) {
      $instance = field_info_instance('node', 'vat', 'order');
      if (!empty($instance)) {
        field_delete_instance($instance);
      }
    }
  }
  else {
    // If the enable VAT checkbox is checked and if basic_cart_order is enabled,
    // then create the vat field for the Order content type.
    if (module_exists('basic_cart_order')) {
      // Check to see if the vat field already exists.
      $vat = field_info_field('vat');
      // If the vat field does not exist then create it.
      if (empty($vat)) {
        $vat = array(
          'field_name' => 'vat',
          'type' => 'number_decimal',
          'entity_types' => array('node'),
        );
        field_create_field($vat);

        // Assign the vat field to the Order content type.
        $instance = field_info_instance('node', 'vat', 'order');
        if (empty($instance)) {
          $instance = array(
            'field_name' => 'vat',
            'label' => t('VAT'),
            'description' => t('The VAT tax.'),
            'entity_type' => 'node',
            'bundle' => 'order',
          );
          // It doesn't exist. Create it.
          field_create_instance($instance);
        }
      }
    }
  }
  
  // Save other variables.
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }
  
  drupal_set_message(t('The configuration options have been saved.'));
}
