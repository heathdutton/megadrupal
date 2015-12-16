<?php

/**
 * @file
 * Internationalization (i18n) module - Commerce product handling.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function i18n_commerce_product_form_commerce_product_ui_product_form_alter(&$form, &$form_state) {
  global $language;

  // Pre-fill fields from translation source.
  $commerce_product = $form['#entity'];
  if (isset($_GET['target']) && isset($_GET['translation'])) {
    $commerce_product->language = $_GET['target'];
    if ($source_product = commerce_product_load($_GET['translation'])) {
      foreach (element_children($form) as $field_name) {
        if (!empty($source_product->{$field_name})) {
          if (!is_array($source_product->{$field_name})) {
            $form[$field_name]['#default_value'] = $source_product->{$field_name};
          }
          elseif (!empty($source_product->{$field_name}[LANGUAGE_NONE][0])) {
            foreach ($source_product->{$field_name}[LANGUAGE_NONE][0] as $key => $value) {
              if (isset($form[$field_name][LANGUAGE_NONE][0][$key])) {
                if ($key == 'amount') {
                  $value = number_format($value / 100, 2);
                }
                $form[$field_name][LANGUAGE_NONE][0][$key]['#default_value'] = $value;
              }
            }
          }
        }
      }
    }
  }

  // Make field language editable on commerce product form.
  if (!empty($form['language']) && $form['language']['#type'] != 'select') {
    $languages = language_list();
    foreach ($languages as $langcode => $lang) {
      if (!$lang->enabled) {
        unset($languages[$langcode]);
      }
      else {
        $languages[$langcode] = $lang->name;
      }
    }
    $form['language'] = array(
      '#type' => 'select',
      '#title' => t('Language'),
      '#options' => $languages,
      '#disabled' => !empty($commerce_product->language),
      '#weight' => 1,
      '#default_value' => !empty($commerce_product->language) ? $commerce_product->language : $language->language,
    );
  }

  $tproduct_id = isset($_GET['translation']) ? $_GET['translation'] : NULL;
  if (!empty($form['#entity']->tproduct_id)) {
    $tproduct_id = $form['#entity']->tproduct_id;
  }
  // Add tproduct_id field.
  $form['tproduct_id'] = array(
    '#type' => 'value',
    '#value' => $tproduct_id,
  );

  // Override validation handler.
  $pos = array_search('commerce_product_product_form_validate', $form['#validate']);
  if ($pos !== FALSE) {
    $form['#validate'][$pos] = 'i18n_commerce_product_product_form_validate';
  }
  // Override submit handler.
  $pos = array_search('commerce_product_product_form_submit', $form['actions']['submit']['#submit']);
  if ($pos !== FALSE) {
    $form['#validate'][$pos] = 'i18n_commerce_product_product_form_submit';
  }
}

/**
 * Implements hook_form_alter().
 */
function i18n_commerce_product_form_alter(&$form, &$form_state, $form_id) {
  if (preg_match('/^commerce_cart_add_to_cart_form_.*/', $form_id)) {
    $pos = array_search('commerce_cart_add_to_cart_form_submit', $form['#submit']);
    if ($pos !== FALSE) {
      $form['#submit'][$pos] = 'i18n_commerce_product_commerce_cart_add_to_cart_form_submit';
    }
  }
}

/**
 * Validation handler for commerce_product_product_form().
 */
function i18n_commerce_product_product_form_validate($form, &$form_state) {
  $form_state['commerce_product']->tproduct_id = isset($form_state['values']['tproduct_id']) ? $form_state['values']['tproduct_id'] : NULL;
  $product = $form_state['commerce_product'];

  // TODO: Resolve workflow issues pertaining to token replacement in SKUs.
  // Perform token replacement on the entered SKU.
  // $sku = commerce_product_replace_sku_tokens($form_state['values']['sku'], $product);
  // Until the above is resolved, simply use the SKU as entered with no tokens.
  $sku = $form_state['values']['sku'];

  // If invalid tokens were specified, throw an error.
  if ($sku === FALSE) {
    form_set_error('sku', t('The SKU contains invalid tokens.'));
  }
  else {
    // Ensure the proposed SKU is unique or reused only during product updates.
    $query = new EntityFieldQuery();

    $query
      ->entityCondition('entity_type', 'commerce_product')
      ->propertyCondition('sku', $sku);

    $result = $query->execute();

    if (!empty($result)) {
      $product_id = key($result['commerce_product']);

      if ($product_id != $product->product_id && $product_id != $product->tproduct_id) {
        form_set_error('sku', t('This SKU is <a href="!url">already in use</a> and must be unique. Please supply another value.', array('!url' => url('admin/commerce/products/' . $product_id))));
      }
    }

    // Validate the SKU for invalid characters.
    if (!commerce_product_validate_sku($sku)) {
      form_set_error('sku', t('The SKU %sku contains invalid characters.', array('%sku' => $sku)));
    }

    // Trim leading and trailing whitespace from the SKU.
    form_set_value($form['sku'], trim($sku), $form_state);
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('commerce_product', $product, $form, $form_state);
}

/**
 * Submit callback for commerce_product_product_form().
 */
function i18n_commerce_product_product_form_submit($form, &$form_state) {
  global $user;

  $product = &$form_state['commerce_product'];

  // Save default parameters back into the $product object.
  $product->sku = $form_state['values']['sku'];
  $product->title = $form_state['values']['title'];
  $product->status = $form_state['values']['status'];
  $product->language = $form_state['values']['language'];
  $product->tproduct_id = $form_state['values']['tproduct_id'];

  // Set the product's uid if it's being created at this time.
  if (empty($product->product_id)) {
    $product->uid = $user->uid;
  }

  // Trigger a new revision if the checkbox was enabled or a log message supplied.
  if ((user_access('administer commerce_product entities') && !empty($form_state['values']['revision']))
    || (!user_access('administer commerce_product entities') && !empty($form['change_history']['revision']['#default_value']))
    || !empty($form_state['values']['log'])) {

    $product->revision = TRUE;
    $product->log = $form_state['values']['log'];
  }

  // Notify field widgets.
  field_attach_submit('commerce_product', $product, $form, $form_state);

  // Save the product.
  commerce_product_save($product);

  // Redirect based on the button clicked.
  drupal_set_message(t('Product saved.'));
}

/**
 * Form submit handler: add the selected product to the cart.
 */
function i18n_commerce_product_commerce_cart_add_to_cart_form_submit($form, &$form_state) {
  $product_id = $form_state['values']['product_id'];
  $product = commerce_product_load($product_id);

  // If the line item passed to the function is new...
  if (empty($form_state['line_item']->line_item_id)) {
    // Create the new product line item of the same type.
    $line_item = commerce_product_line_item_new($product, $form_state['values']['quantity'], 0, $form_state['line_item']->data, $form_state['line_item']->type);

    // Allow modules to prepare this as necessary. This hook is defined by the
    // Product Pricing module.
    drupal_alter('commerce_product_calculate_sell_price_line_item', $line_item);

    // Remove line item field values the user didn't have access to modify.
    foreach ($form_state['values']['line_item_fields'] as $field_name => $value) {
      // Note that we're checking the Commerce Cart settings that we inserted
      // into this form element array back when we built the form. This means a
      // module wanting to alter a line item field widget to be available must
      // update both its form element's #access value and the field_access value
      // of the #commerce_cart_settings array.
      if (empty($form['line_item_fields'][$field_name]['#commerce_cart_settings']['field_access'])) {
        unset($form_state['values']['line_item_fields'][$field_name]);
      }
    }

    // Unset the line item field values array if it is now empty.
    if (empty($form_state['values']['line_item_fields'])) {
      unset($form_state['values']['line_item_fields']);
    }

    // Add field data to the line item.
    field_attach_submit('commerce_line_item', $line_item, $form['line_item_fields'], $form_state);

    // Process the unit price through Rules so it reflects the user's actual
    // purchase price.
    rules_invoke_event('commerce_product_calculate_sell_price', $line_item);

    // Only attempt an Add to Cart if the line item has a valid unit price.
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    if (!is_null($line_item_wrapper->commerce_unit_price->value())) {
      // Add the product to the specified shopping cart.
      $form_state['line_item'] = i18n_commerce_product_commerce_cart_product_add(
        $form_state['values']['uid'],
        $line_item,
        isset($line_item->data['context']['add_to_cart_combine']) ? $line_item->data['context']['add_to_cart_combine'] : TRUE
      );
    }
    else {
      drupal_set_message(t('%title could not be added to your cart.', array('%title' => $product->title)), 'error');
    }
  }
}
