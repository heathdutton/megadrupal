<?php

/**
 * @file
 * Forms for creating / editing and deleting products.
 */


/**
 * Form callback: create or edit a product key type.
 *
 * @param $product_key_type
 *   The product key type array to edit or for a create form an empty product key type
 *     array with properties instantiated but not populated.
 */
function commerce_product_key_ui_product_key_type_form($form, &$form_state, $product_key_type) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product_key_ui') . '/includes/commerce_product_key_ui.forms.inc';

  // Store the initial product key type in the form state.
  $form_state['product_key_type'] = $product_key_type;

  $form['product_key_type'] = array(
    '#tree' => TRUE,
  );

  $form['product_key_type']['product_id'] = array(
    '#type' => 'select',
    '#title' => t('Product'),
    '#options' => commerce_product_reference_options_list(array('field_name' => 'line_item_product_selector')),
    '#default_value' => $product_key_type['product_id'],
  );

  $form['product_key_type']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $product_key_type['name'],
    '#description' => t('The human-readable name of this sequence. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 32,
    '#field_suffix' => ' <small id="edit-product-type-name-suffix">' . t('Machine name: @type', array('@type' => $product_key_type['type'])) . '</small>',
  );

  if (empty($product_key_type['type'])) {
    $form['product_key_type']['type'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $product_key_type['type'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_product_key_type_load',
        'source' => array('product_key_type', 'name'),
      ),
      '#description' => t('The machine-readable name of this sequence. This name must contain only lowercase letters, numbers, and underscores, it must be unique.'),
    );
  }

  $form['product_key_type']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Describe this sequence. The text will be displayed on the <em>Add new content</em> page.'),
    '#default_value' => $product_key_type['description'],
    '#rows' => 3,
  );

  $form['product_key_type']['help'] = array(
    '#type' => 'textarea',
    '#title' => t('Explanation or submission guidelines'),
    '#description' => t('This text will be displayed at the top of the page when creating or editing products of this type.'),
    '#default_value' => $product_key_type['help'],
    '#rows' => 3,
  );

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
    '#value' => t('Save sequence'),
    '#submit' => $submit + array('commerce_product_key_ui_product_key_type_form_submit'),
  );

  if (!empty($form_state['product_key_type']['type'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete sequence'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/product-keys/types'),
      '#submit' => $submit + array('commerce_product_key_ui_product_key_type_form_delete_submit'),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['save_continue'] = array(
      '#type' => 'submit',
      '#value' => t('Save and add fields'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/product-keys/types'),
      '#submit' => $submit + array('commerce_product_key_ui_product_key_type_form_submit'),
      '#weight' => 45,
    );
  }

  $form['#validate'][] = 'commerce_product_key_ui_product_key_type_form_validate';

  return $form;
}

/**
 * Validation callback for commerce_product_key_product_key_type_form().
 */
function commerce_product_key_ui_product_key_type_form_validate($form, &$form_state) {
  $product_key_type = $form_state['product_key_type'];

  // If saving a new product key type, ensure it has a unique machine name.
  if (empty($product_key_type['type'])) {
    if (!commerce_product_key_ui_validate_product_key_type_unique($form_state['values']['product_key_type']['type'])) {
      form_set_error('product_key_type][type', t('The machine name specified is already in use.'));
    }
  }
}

/**
 * Form submit handler: save a product key type.
 */
function commerce_product_key_ui_product_key_type_form_submit($form, &$form_state) {
  $product_key_type = $form_state['product_key_type'];
  $updated = !empty($product_key_type['type']);

  foreach ($form_state['values']['product_key_type'] as $key => $value) {
    $product_key_type[$key] = $value;
  }

  // Write the product key type to the database.
  $product_key_type['is_new'] = !$updated;
  commerce_product_key_ui_product_key_type_save($product_key_type);

  // Redirect based on the button clicked.
  drupal_set_message(t('Sequence saved.'));

  if ($form_state['clicked_button']['#parents'][0] == 'save_continue') {
    $form_state['redirect'] = 'admin/commerce/product-keys/types/' . strtr($product_key_type['type'], '_', '-') . '/fields';
  }
  else {
    $form_state['redirect'] = 'admin/commerce/product-keys/types';
  }
}

/**
 * Submit callback for delete button on commerce_product_key_ui_product_key_type_form().
 *
 * @see commerce_product_key_ui_product_key_type_form()
 */
function commerce_product_key_ui_product_key_type_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/product-keys/types/' . strtr($form_state['product_key_type']['type'], '_', '-') . '/delete';
}

/**
 * Form callback: confirmation form for deleting a product key type.
 *
 * @param $product_key_type
 *   The product key type array to be deleted.
 *
 * @see confirm_form()
 */
function commerce_product_key_ui_product_key_type_delete_form($form, &$form_state, $product_key_type) {
  $form_state['product_key_type'] = $product_key_type;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product_key_ui') . '/includes/commerce_product_key_ui.forms.inc';

  $form['#submit'][] = 'commerce_product_key_ui_product_key_type_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete the %name sequence?', array('%name' => $product_key_type['name'])),
    'admin/commerce/product-keys/types',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_product_key_product_key_type_delete_form().
 */
function commerce_product_key_ui_product_key_type_delete_form_submit($form, &$form_state) {
  $product_key_type = $form_state['product_key_type'];

  commerce_product_key_ui_product_key_type_delete($product_key_type['type']);

  drupal_set_message(t('The sequence %name has been deleted.', array('%name' => $product_key_type['name'])));
  watchdog('commerce_product_key', 'Deleted sequence %name.', array('%name' => $product_key_type['name']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/commerce/product-keys/types';
}
