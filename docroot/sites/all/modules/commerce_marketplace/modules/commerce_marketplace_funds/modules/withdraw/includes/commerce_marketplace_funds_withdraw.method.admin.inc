<?php

/**
 * @file
 * Commerce Marketplace Withdraw entity types UI controllers.
 */

/**
 * Form callback wrapper: create or edit a withdraw method.
 *
 * @param $method
 *   The withdraw method object being edited by this form.
 *
 * @see commerce_marketplace_funds_withdraw_method_edit_form()
 */
function commerce_marketplace_funds_withdraw_method_form_wrapper($method, $owner_type, $owner = NULL) {
  $account = NULL;
  if ($owner) {
    $owner_info = entity_get_info($owner_type);
    $account = commerce_marketplace_funds_account_load_by_owner($owner_type, $owner->{$owner_info['entity keys']['id']});
  }
  if (is_string($method)) {
    // try to load the method if it exists.
    $query = new EntityFieldQuery();
    $method_id = $query->entityCondition('entity_type', 'cmp_funds_withdraw_method')
      ->entityCondition('bundle', $method)
      ->propertyCondition('account_id', $account->account_id)->execute();
    if (isset($method_id['cmp_funds_withdraw_method']) && !empty($method_id['cmp_funds_withdraw_method'])) {
      $result = array_pop($method_id['cmp_funds_withdraw_method']);
      $method = commerce_marketplace_funds_withdraw_method_load($result->id);
    }
    else {
      $values = array(
        'type' => $method,
      );
      if ($account) {
        $values['account_id'] = $account->account_id;
      }
      $method = commerce_marketplace_funds_withdraw_method_create($values);
    }
  }
  return drupal_get_form('commerce_marketplace_funds_withdraw_method_edit_form', $method, $account);
}


/**
 * Form callback wrapper: delete a withdraw method.
 *
 * @param $method
 *   The withdraw method object being edited by this form.
 *
 * @see commerce_marketplace_funds_withdraw_method_delete_form()
 */
function commerce_marketplace_funds_withdraw_method_delete_form_wrapper($method) {
  return drupal_get_form('commerce_marketplace_funds_withdraw_method_delete_form', $method);
}


/**
 * Form callback: create or edit a withdraw method.
 *
 * @param $method
 *   The withdraw method object to edit or for a create form an empty withdraw method object
 *     with only a withdraw method type defined.
 */
function commerce_marketplace_funds_withdraw_method_edit_form($form, &$form_state, $method, $account = NULL) {
  // Add the default field elements.
  if ($method->id) {
    $form['id'] = array(
      '#type' => 'value',
      '#value' => $method->id,
    );
  }
  if ($method->type) {
    $form['type'] = array(
      '#type' => 'value',
      '#value' => $method->type,
    );
  }
  if ($account) {
    $form['account_id'] = array(
      '#type' => 'value',
      '#value' => $account->account_id,
    );
  }
  else {
    $form['account_id'] = array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'funds/account/autocomplete',
      '#title' => t('Account'),
      '#maxlength' => 255,
      '#required' => TRUE,
      '#weight' => -5,
    );
  }

  // Add the field related form elements.
  $form_state['cmp_funds_withdraw_method'] = $method;
  field_attach_form('cmp_funds_withdraw_method', $method, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save withdraw method'),
    '#submit' => $submit + array('commerce_marketplace_funds_withdraw_method_edit_form_submit'),
  );

  if (!empty($method->id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete withdraw method'),
      '#submit' => $submit + array('commerce_marketplace_funds_withdraw_method_form_submit_delete'),
      '#weight' => 45,
    );
  }
  $form['#validate'][] = 'commerce_marketplace_funds_withdraw_method_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the withdraw method form.
 */
function commerce_marketplace_funds_withdraw_method_edit_form_validate($form, &$form_state) {
  $method = $form_state['cmp_funds_withdraw_method'];
  field_attach_form_validate('cmp_funds_withdraw_method', $method, $form, $form_state);
}


/**
 * Form API submit callback for the withdraw method form.
 */
function commerce_marketplace_funds_withdraw_method_edit_form_submit($form, &$form_state) {
  if (isset($form_state['values']['type'])) {
    $form_state['cmp_funds_withdraw_method']->type = $form_state['values']['type'];
  }
  entity_form_submit_build_entity('cmp_funds_withdraw_method', $form_state['cmp_funds_withdraw_method'], $form, $form_state);
  $method = $form_state['cmp_funds_withdraw_method'];
  // Save the model and go back to the list of models
  // Add in created and changed times.
  $method->is_new = isset($method->is_new) ? $method->is_new : 0;
  if ($method->is_new){
    $method->created = REQUEST_TIME;
  }

  $method->changed = REQUEST_TIME;
  $method->save();
  // @todo: change this link.
  $form_state['redirect'] = commerce_marketplace_funds_withdraw_method_uri($method);
}

/**
 * Form API submit callback for the delete button.
 *
 * @todo Remove hard-coded path
 */
function commerce_marketplace_funds_withdraw_method_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/marketplace_funds/withdraw/method/' . $form_state['cmp_funds_withdraw_method']->id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a withdraw method.
 *
 * @param $method
 *   The withdraw method to delete.
 *
 * @see confirm_form()
 */
function commerce_marketplace_funds_withdraw_method_delete_form($form, &$form_state, $method) {
  $form_state['cmp_funds_withdraw_method'] = $method;

  $form['#submit'][] = 'commerce_marketplace_funds_withdraw_method_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete this withdraw method?'),
    commerce_marketplace_funds_withdraw_method_uri($method),
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_marketplace_funds_withdraw_method_delete_form.
 */
function commerce_marketplace_funds_withdraw_method_delete_form_submit($form, &$form_state) {
  $method = $form_state['cmp_funds_withdraw_method'];

  commerce_marketplace_funds_withdraw_method_delete($method);

  drupal_set_message(t('The withdraw method %id has been deleted.', array('%id' => $method->id)));
  watchdog('model', 'Deleted model %id.', array('%id' => $method->id));

  $form_state['redirect'] = commerce_marketplace_funds_account_uri(commerce_marketplace_funds_account_load($method->account_id));
}



/**
 * Page to add Withdraw method Entities.
 *
 * @todo Pass this through a proper theme function
 */
function commerce_marketplace_funds_withdraw_method_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  return theme('cmp_funds_withdraw_method_add_list', array('content' => $content));
}


/**
 * Displays the list of available withdraw method types for withdraw method creation.
 *
 * @ingroup themeable
 */
function theme_cmp_funds_withdraw_method_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="cmp-funds-withdraw-method-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer commerce marketplace funds withdraw method types')) {
      $output = '<p>' . t('Withdraw method entities cannot be added because you have not created any withdraw method types yet. Go to the <a href="@create-method-type">model type creation page</a> to add a new withdraw method type.', array('@create-method-type' => url('admin/commerce/marketplace_funds/withdraw/methods/types'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No withdraw method types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}

/**
 * Returns a rendered table showing all available withdrawal methods with add/edit links.
 */
function commerce_marketplace_funds_withdraw_methods_overview_page($owner_type, $owner) {
  $header = array(
    'type' => t('Withdraw Method'),
    'operations' => t('Operations'),
  );
  $owner_info = entity_get_info($owner_type);
  $account = commerce_marketplace_funds_account_load_by_owner($owner_type, $owner->{$owner_info['entity keys']['id']});
  $uri = commerce_marketplace_funds_account_uri($account);
  $rows = array();
  $types = commerce_marketplace_funds_withdraw_get_types();
  foreach ($types as $type => $info) {
    $rows[$type] = array(
      'type' => $info->label,
      'operations' => l(t('Configure'), $uri['path'] . '/withdraw/' . $type),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}



