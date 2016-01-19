<?php
/**
 * @file
 * Forms for creating, editing, and deleting commerce_stock_sources.
 */

/**
 * Form callback: create or edit a commerce_stock_sources.
 *
 * @param $commerce_stock_sources
 *   The commerce_stock_source object to edit or for a create form an empty commerce_stock_source
 *     with only a commerce_stock_source type defined.
 */
function commerce_stock_sources_source_form($form, &$form_state, $commerce_stock_sources) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_stock_sources') . '/includes/commerce_stock_sources.source.inc';

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $commerce_stock_sources->name,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  // Add the field related form elements.
  $form_state['commerce_stock_sources'] = $commerce_stock_sources;

  $langcode = entity_language('commerce_stock_sources', $commerce_stock_sources);

  if (empty($langcode)) {
    $langcode = LANGUAGE_NONE;
  }

  field_attach_form('commerce_stock_sources', $commerce_stock_sources, $form, $form_state, $langcode);

  $form['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#description' => t('Disabled stock sources cannot be added to shopping carts and may be hidden in administrative stock sources lists.'),
    '#options' => array(
      '1' => t('Active'),
      '0' => t('Disabled'),
    ),
    '#default_value' => $commerce_stock_sources->status,
    '#required' => TRUE,
    '#weight' => 35,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 400,
  );

  // Simply use default language
  $form['language'] = array(
    '#type' => 'value',
    '#value' => $langcode,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save stock source'),
    '#submit' => array_merge($submit, array('commerce_stock_sources_source_form_submit')),
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'commerce_stock_sources_source_form_validate';

  return $form;
}

/**
 * Validation callback for commerce_stock_sources_source_form().
 */
function commerce_stock_sources_source_form_validate($form, &$form_state) {}

/**
 * Submit callback for commerce_stock_sources_source_form().
 */
function commerce_stock_sources_source_form_submit($form, &$form_state) {
  global $user;

  $commerce_stock_sources = &$form_state['commerce_stock_sources'];

  // Save default parameters back into the $commerce_stock_sources object.
  $commerce_stock_sources->name = $form_state['values']['name'];
  $commerce_stock_sources->status = $form_state['values']['status'];
  $commerce_stock_sources->language = $form_state['values']['language'];

  // Set the commerce_stock_sources's uid if it's being created at this time.
  if (empty($commerce_stock_sources->source_id)) {
    $commerce_stock_sources->uid = $user->uid;
  }

  // Notify field widgets.
  field_attach_submit('commerce_stock_sources', $commerce_stock_sources, $form, $form_state);

  // Save the commerce_stock_sources.
  commerce_stock_sources_save($commerce_stock_sources);

  // Redirect based on the button clicked.
  drupal_set_message(t('Stock source saved.'));

  $form_state['redirect'] = 'admin/commerce/config/stock/stock-sources';
}

/**
 * Form callback: confirmation form for deleting a commerce_stock_sources.
 *
 * @param $commerce_stock_sources
 *   The commerce_stock_sources object to be deleted.
 *
 * @see confirm_form()
 */
function commerce_stock_sources_source_delete_form($form, &$form_state, $commerce_stock_sources) {
  $form_state['$commerce_stock_sources'] = $$commerce_stock_sources;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_stock_sources') . '/includes/commerce_stock_sources.source.inc';

  $form['#submit'][] = 'commerce_stock_sources_source_delete_form_submit';

  $content = entity_view('commerce_stock_sources', array($commerce_stock_sources->source_id => $commerce_stock_sources));

  $form = confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $commerce_stock_sources->name)),
    '',
    drupal_render($content) . '<p>' . t('Deleting this $commerce_stock_sources cannot be undone.', array('@name' => $commerce_stock_sources->name)) . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_product_product_delete_form().
 */
function commerce_stock_sources_source_delete_form_submit($form, &$form_state) {
  $commerce_stock_sources = $form_state['commerce_stock_sources'];

  if (commerce_stock_sources_delete($commerce_stock_sources->source_id)) {
    drupal_set_message(t('%title has been deleted.', array('%title' => $commerce_stock_sources->name)));
    watchdog('commerce_stock_sources', 'Deleted product %title.', array('%title' => $commerce_stock_sources->name), WATCHDOG_NOTICE);
  }
  else {
    drupal_set_message(t('%title could not be deleted.', array('%title' => $commerce_stock_sources->name)), 'error');
  }
}
