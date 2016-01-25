<?php

/**
 * @file
 * Forms for creating, editing, and deleting products.
 */


/**
 * Form callback: create or edit a product.
 *
 * @param $product_key
 *   The product object to edit or for a create form an empty product object
 *     with only a product key type defined.
 */
function commerce_product_key_product_form($form, &$form_state, $product_key) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product_key') . '/includes/commerce_product_key.forms.inc';

  // Add the default field elements.
  $form['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Product code'),
    '#description' => t('Supply a code for this product key.'),
    '#default_value' => $product_key->code,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#weight' => -10,
  );

  //only add extra field setting on the edit.
  if (!empty($product_key->product_key_id)) {
    $form['status'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#description' => t('Disabled product keys are not visible to users.'),
      '#options' => array(
        '1' => t('Active'),
        '0' => t('Disabled'),
      ),
      '#default_value' => $product_key->status,
      '#required' => TRUE,
      '#weight' => -1,
    );

    $form['advanced'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced'),
      // Make the fieldset collapsible.
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => 49, //keep these items at the bottom
    );
    $form['advanced']['description'] = array(
      '#type' => 'item',
      '#title' => t('Remove Association'),
      '#description' => t('To remove a product key association and make it available for others to purchase one again. Make both <em>Order ID</em> and <em>Owner UID</em> <strong>blank</strong>. This can be handy when you want to assign a key but not let the user see it until the payment has cleared. To force an association with out a paper trail assign the relative IDs, this is useful when a product code may not have worked and you need to assign another. If you want a paper trail create a new order and process it under <em>COD</em>.'),
      '#weight' => -10,
    );
    $form['advanced']['order_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Order ID'),
      '#description' => t('Leave <strong>blank to remove</strong> the order association.'),
      '#default_value' => $product_key->order_id,
      '#maxlength' => 10,
      '#required' => FALSE,
      '#weight' => -3,
    );
    $form['advanced']['key_owner_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Owner UID'),
      '#description' => t('Leave <strong>blank to remove</strong> the user association.'),
      '#default_value' => $product_key->key_owner_id,
      '#maxlength' => 10,
      '#required' => FALSE,
      '#weight' => -2,
    );

    $form['software'] = array(
      '#type' => 'fieldset',
      '#title' => t('Software Key Settings'),
      // Make the fieldset collapsible.
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 50, //keep these items at the bottom
    );
    $form['software']['description'] = array(
      '#type' => 'item',
      '#title' => t('These settings are for software keys.'),
      '#description' => t('These settings only apply to software keys that require a server to active and revoke keys. If you are not using the product key module for this simply ignore this section.'),
      '#weight' => 0,
    );
    $form['software']['revoked'] = array(
      '#type' => 'radios',
      '#title' => t('Revoked'),
      '#description' => t('If a license is revoked, any attempt to activate it the software will be denied. This may not shut down any activated programs, but will prevent the product key from being used to activate any more products.'),
      '#options' => array(
        '1' => t('Revoked'),
        '0' => t('Active'),
      ),
      '#default_value' => $product_key->revoked,
      '#required' => TRUE,
      '#weight' => 5,
    );
  }

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // Add the field related form elements.
  $form_state['commerce_product_key'] = $product_key;
  field_attach_form('commerce_product_key', $product_key, $form, $form_state);

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save sequence'),
    '#submit' => $submit + array('commerce_product_key_product_key_form_submit'),
    '#weight' => 1,
  );
  if (empty($form['code']['#default_value'])) {
    $form['actions']['save_continue'] = array(
      '#type' => 'submit',
      '#value' => t('Save and add another'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/product-keys'),
      '#submit' => $submit + array('commerce_product_key_product_key_form_submit'),
      '#weight' => 2,
    );
  }
  else {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete product key'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/product-keys'),
      '#submit' => $submit + array('commerce_product_key_product_key_delete_form_submit'),
      '#weight' => 2,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'commerce_product_key_product_key_form_validate';

  return $form;
}

/**
 * Validation callback for commerce_product_key_product_form().
 */
function commerce_product_key_product_key_form_validate($form, &$form_state) {
  $product_key = $form_state['commerce_product_key'];

  $code = $form_state['values']['code'];

  // Ensure the proposed code is unique.
  if (!commerce_product_key_validate_code_unique($code, $product_key->product_key_id)) {
    form_set_error('code', t('This code is already in use and must be unique. Please supply another value.'));
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('commerce_product_key', $product_key, $form, $form_state);
}

/**
 * Submit callback for commerce_product_key_product_key_form().
 */
function commerce_product_key_product_key_form_submit($form, &$form_state) {
  global $user;

  $product_key = &$form_state['commerce_product_key'];

  // Save default parameters back into the $product_key object.
  $product_key->code = $form_state['values']['code'];

  //as we have conditional items only on the edit check they exist
  if (array_key_exists('status', $form_state['values'])) {
    $product_key->status = $form_state['values']['status'];
  }
  if (array_key_exists('revoked', $form_state['values'])) {
    $product_key->revoked = $form_state['values']['revoked'];
  }

  if (array_key_exists('order_id', $form_state['values'])) {
    if ($form_state['values']['order_id'] == '') {
      $product_key->order_id = NULL;
    }
    else {
      $product_key->order_id = $form_state['values']['order_id'];
    }
  }
  if (array_key_exists('key_owner_id', $form_state['values'])) {
    if ($form_state['values']['key_owner_id'] == '') {
      $product_key->key_owner_id = NULL;
    }
    else {
      $product_key->key_owner_id = $form_state['values']['key_owner_id'];
    }
  }


  // Set the product's uid if it's being created at this time.
  if (empty($product_key->product_key_id)) {
    $product_key->uid = $user->uid;
  }

  // Notify field widgets.
  field_attach_submit('commerce_product_key', $product_key, $form, $form_state);

  // Save the product key.
  commerce_product_key_save($product_key);


  drupal_set_message(t('Product key saved.'));

  // Redirect based on the button clicked.
  if ($form_state['clicked_button']['#parents'][0] != 'save_continue') {
    $form_state['redirect'] = 'admin/commerce/product-keys';
  }
}

/**
 * Form callback: confirmation form for deleting a product key.
 *
 * @param $product_key
 *   The product key object to be deleted.
 *
 * @see confirm_form()
 */
function commerce_product_key_product_key_delete_form($form, &$form_state, $product_key) {
  $form_state['commerce_product_key'] = $product_key;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product_key') . '/includes/commerce_product_key.forms.inc';

  $form['#submit'][] = 'commerce_product_key_product_key_delete_form_submit';

  $content = commerce_product_key_build_content($product_key);

  $form = confirm_form($form,
    t('Are you sure you want to delete %code?', array('%code' => $product_key->code)),
    '',
    drupal_render($content) . '<p>' . t('Deleting this product key cannot be undone.', array('@code' => $product_key->code)) . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_product_key_product_key_delete_form().
 */
function commerce_product_key_product_key_delete_form_submit($form, &$form_state) {
  $product_key = $form_state['commerce_product_key'];

  if (commerce_product_key_delete($product_key->product_key_id)) {
    drupal_set_message(t('%code has been deleted.', array('%code' => $product_key->code)));
    watchdog('commerce_product_key', 'Deleted product key %code.', array('%code' => $product_key->code), WATCHDOG_NOTICE);
  }
  else {
    drupal_set_message(t('%code could not be deleted.', array('%code' => $product_key->code)), 'error');
  }
  $form_state['redirect'] = 'admin/commerce/product-keys';
}