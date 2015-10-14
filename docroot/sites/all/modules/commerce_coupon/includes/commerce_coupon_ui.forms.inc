<?php

/**
 * @file
 * Commerce Coupon editing UI.
 */

/**
 * Generates the commerce coupon editing form.
 */
function commerce_coupon_form($form, &$form_state, $coupon, $op = 'edit') {
  // TODO: add a validation method to check, that the code is not a duplicate!
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_coupon') . '/includes/commerce_coupon_ui.forms.inc';

  // Add the field related form elements.
  $form_state['commerce_coupon'] = $coupon;
  $form_state['op'] = $op;
  field_attach_form('commerce_coupon', $coupon, $form, $form_state);

  $form['is_active'] = array(
    '#title' => t('Active'),
    '#type' => 'checkbox',
    '#default_value' => $coupon->is_active,
    '#description' => t('Indicates if the coupon can be used or not.'),
    '#size' => 30,
    '#weight' => 40,
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save coupon'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function commerce_coupon_form_validate(&$form, &$form_state) {
  $coupon = &$form_state['commerce_coupon'];
  $coupon_type = commerce_coupon_type_load($coupon->type);
  // Don't allow to enable a coupon from a coupon type non enabled.
  if (empty($coupon_type->status) && $coupon->is_active == 1) {
    form_set_error('is_active', t('You can\'t enable a coupon of a coupon type that is disabled'));
  }

  if ($form_state['op'] == 'add' || $form_state['op'] == 'edit') {
    $lang_code = field_language('commerce_coupon', $form_state['commerce_coupon'], 'commerce_coupon_code');
    if (isset($form_state['values']['commerce_coupon_code'][$lang_code][0]['value'])) {
      $coupon_code = $form_state['values']['commerce_coupon_code'][$lang_code][0]['value'];
      if (!empty($coupon_code) && commerce_coupon_code_exists($coupon_code)) {
        // Don't invalidate duplicate code if we are simply updating one.
        if ($form_state['op'] == 'add' || (isset($coupon->commerce_coupon_code) &&
          $coupon->commerce_coupon_code[$lang_code][0]['value'] != $coupon_code)) {
          form_set_error('commerce_coupon_code][' . $lang_code, t('This coupon code is already in use.'));
        }
      }
    }
  }

  field_attach_form_validate('commerce_coupon', $form_state['commerce_coupon'], $form, $form_state);
}

/**
 * Form API submit callback for the delete button.
 */
function commerce_coupon_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/coupons/' . $form_state['commerce_coupon']->coupon_id . '/delete';
}

/**
 * Submit callback for commerce_coupon_form().
 */
function commerce_coupon_form_submit($form, &$form_state) {
  $commerce_coupon = &$form_state['commerce_coupon'];
  $commerce_coupon->is_active = $form_state['values']['is_active'];

  // Notify field widgets.
  field_attach_submit('commerce_coupon', $commerce_coupon, $form, $form_state);

  // Save the coupon.
  commerce_coupon_save($commerce_coupon);
  $form_state['redirect'] = 'admin/commerce/coupons';

  // Redirect based on the button clicked.
  drupal_set_message(t('Coupon saved.'));
}

/**
 * Form callback: confirmation form for deleting a coupon.
 *
 * @param $commerce_coupon
 *   The coupon object to be deleted.
 *
 * @see confirm_form()
 */
function commerce_coupon_ui_delete_form($form, &$form_state, $coupon) {
  $form_state['commerce_coupon'] = $coupon;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_coupon') . '/includes/commerce_coupon_ui.forms.inc';
  $form['#submit'][] = 'commerce_coupon_ui_coupon_delete_form_submit';
  $coupon_wrapper = entity_metadata_wrapper('commerce_coupon', $coupon);
  $form = confirm_form($form, t('Are you sure you want to delete the coupon with code %code?', array('%code' => $coupon_wrapper->commerce_coupon_code->value())), 'admin/commerce/coupons', '<p>' . t('Deleting this coupon cannot be undone.') . '</p>', t('Delete'), t('Cancel'), 'confirm');

  // Get the orders referenced by the coupon.
  $query = new EntityFieldQuery();
  $query
    ->entityCondition('entity_type', 'commerce_order', '=')
    ->fieldCondition('commerce_coupon_order_reference', 'target_id', $coupon->coupon_id, '=');
  $result = $query->execute();

  if (!empty($result)) {
    $description = t('This coupon is referenced by an order and therefore cannot be deleted. Disable it instead.');
    $form['description']['#markup'] .= '<p>' . $description . '</p>';
    $form['actions']['submit']['#disabled'] = TRUE;
  }

  return $form;
}

/**
 * Submit callback for commerce_coupon_ui_coupon_delete_form().
 */
function commerce_coupon_ui_coupon_delete_form_submit($form, &$form_state) {
  $coupon = $form_state['commerce_coupon'];
  $coupon_wrapper = entity_metadata_wrapper('commerce_coupon', $coupon);

  if (commerce_coupon_delete($coupon->coupon_id)) {
    drupal_set_message(t('%code has been deleted.', array('%code' => $coupon_wrapper->commerce_coupon_code->value())));
    watchdog('commerce_coupon', 'Deleted coupon %code.', array('%code' => $coupon_wrapper->commerce_coupon_code->value()), WATCHDOG_NOTICE);
  }
  else {
    drupal_set_message(t('%code could not be deleted.', array('%code' => $coupon_wrapper->commerce_coupon_code->value())), 'error');
  }
  $form_state['redirect'] = 'admin/commerce/coupons';
}

/**
 * Generates the commerce coupon type editing form.
 */
function commerce_coupon_ui_type_form($form, &$form_state, $coupon_type) {
  // Store the initial coupon type in the form state.
  $form_state['coupon_type'] = $coupon_type;

  $form['coupon_type'] = array(
    '#tree' => TRUE,
  );

  $form['coupon_type']['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($coupon_type->label) ? $coupon_type->label : '',
    '#description' => t('The human-readable name of this coupon type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  if (empty($coupon_type->type)) {
    $form['coupon_type']['type'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => isset($coupon_type->type) ? $coupon_type->type : '',
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_coupon_type_load',
        'source' => array('coupon_type', 'label'),
      ),
      '#description' => t('A unique machine-readable name for this coupon type. It must only contain lowercase letters, numbers, and underscores.'),
    );
  }
  else {
    $form['coupon_type']['reset'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reset Fields'),
      '#description' => t('If checked, the default fields for this coupon type will be reset.'),
    );
  }

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save coupon type'),
    '#weight' => 40,
    '#submit' => $submit + array('commerce_coupon_ui_coupon_type_form_submit'),
  );


  if (!$coupon_type->isLocked() && !empty($coupon_type->type)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete coupon type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('commerce_coupon_ui_coupon_type_form_submit')
    );
  }
  else {
    $form['actions']['save_continue'] = array(
      '#type' => 'submit',
      '#value' => t('Save and add fields'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/coupons/types'),
      '#submit' => $submit + array('commerce_coupon_ui_coupon_type_form_submit'),
      '#weight' => 45,
    );
  }

  return $form;
}

/**
 * Validation callback for type form.
 */
function commerce_coupon_ui_coupon_type_form_validate($form, &$form_state) {
  $coupon_type = $form_state['coupon_type'];

  // If saving a new product type, ensure it has a unique machine name.
  if (empty($coupon_type->type)) {
    if (!commerce_product_ui_validate_coupon_type_unique($form_state['values']['coupon_type']['type'])) {
      form_set_error('coupon_type][type', t('The machine name specified is already in use.'));
    }
  }
}

/**
 * Form API submit callback for the type form.
 */
function commerce_coupon_ui_coupon_type_form_submit(&$form, &$form_state) {
  $coupon_type = $form_state['coupon_type'];
  $updated = !empty($coupon_type->type);

  // If a type is set, we should still check to see if a row for the type exists
  // in the database; this is done to accomodate types defined by Features.
  if ($updated) {
    $updated = db_query('SELECT 1 FROM {commerce_coupon_type} WHERE type = :type', array(':type' => $coupon_type->type))->fetchField();
  }

  if (isset($form_state['values']['reset']) && $form_state['values']['reset']) {
    $reset = TRUE;
  }
  else {
    $reset = FALSE;
  }

  foreach ($form_state['values']['coupon_type'] as $key => $value) {
    $coupon_type->{$key} = $value;
  }

  $coupon_type = commerce_coupon_type_save($coupon_type, $reset);

  // Redirect based on the button clicked.
  drupal_set_message(t('Coupon type saved.'));

  if ($form_state['triggering_element']['#parents'][0] == 'save_continue') {
    $form_state['redirect'] = 'admin/commerce/coupons/types/' . strtr($coupon_type->type, '_', '-') . '/fields';
  }
  else {
    $form_state['redirect'] = 'admin/commerce/coupons/types';
  }
}

/**
 * Form callback: confirmation form for deleting a coupon type.
 *
 * @param $coupon_type
 *   The coupon type array to be deleted.
 *
 * @see confirm_form()
 */
function commerce_coupon_ui_coupon_type_delete_form($form, &$form_state, $coupon_type) {
  $form_state['coupon_type'] = $coupon_type;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_coupon_ui') . '/includes/commerce_coupon_ui.forms.inc';

  $form['#submit'][] = 'commerce_coupon_ui_coupon_type_delete_form_submit';

  $form = confirm_form($form, t('Are you sure you want to delete the %name coupon type?', array('%name' => $coupon_type->label)), 'admin/commerce/coupons/types', '<p>' . t('This action cannot be undone.') . '</p>', t('Delete'), t('Cancel'), 'confirm'
  );

  return $form;
}

/**
 * Submit callback for deleting a coupon type.
 */
function commerce_coupon_ui_coupon_type_delete_form_submit($form, &$form_state) {
  $coupon_type = $form_state['coupon_type'];

  commerce_coupon_type_delete($coupon_type->type);

  drupal_set_message(t('The coupon type %name has been deleted.', array('%name' => $coupon_type->label)));
  watchdog('commerce_product', 'Deleted product type %name.', array('%name' => $coupon_type->label), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/commerce/coupons/types';
}
