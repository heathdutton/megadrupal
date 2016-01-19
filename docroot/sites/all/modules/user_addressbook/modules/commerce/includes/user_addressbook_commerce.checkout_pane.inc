<?php

/**
 * @file
 * Based on commerce/modules/customer/commerce_customer.checkout_pane.inc
 */

/**
 * Checkout pane callback: returns the customer profile pane's settings form.
 */
function user_addressbook_commerce_checkout_pane_settings_form($checkout_pane) {
  $form = array();
  $form['commerce_checkout_user_addressbook_skip_addresses_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Skip Addresses checkout page for default address set.'),
    '#description' => t('It will happen first time only, when the order has no customer profile defined.'),
    '#default_value' => variable_get('commerce_checkout_user_addressbook_skip_addresses_page', FALSE),
  );
  $customer_profile_panes = user_addressbook_commerce_customer_profile_panes();
  $help_text_default = array(
    'value' => '',
    'format' => 'filtered_html',
  );
  foreach ($customer_profile_panes as $type => $customer_profile_pane) {
    $form[$type] = array(
      '#type' => 'fieldset',
      '#title' => $customer_profile_pane['title'],
    );
    $form[$type]['commerce_checkout_' . $type . '_user_addressbook_autosave'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto save to user address.'),
      '#description' => t('The customer profile data will be saved by default as user address. If unchecked the saving will be optional.'),
      '#default_value' => variable_get('commerce_checkout_' . $type . '_user_addressbook_autosave', FALSE),
    );
    // Custom pane title.
    $user_addressbook_pane_title = variable_get('commerce_checkout_' . $type . '_user_addressbook_pane_title', $customer_profile_pane['title']);
    $form[$type]['commerce_checkout_' . $type . '_user_addressbook_pane_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom pane title'),
      '#default_value' => $user_addressbook_pane_title,
    );
    // Help text User addressbook pane.
    $user_addressbook_help_text = variable_get('commerce_checkout_' . $type . '_user_addressbook_help_text', $help_text_default);
    $form[$type]['commerce_checkout_' . $type . '_user_addressbook_help_text'] = array(
      '#type' => 'text_format',
      '#title' => t('Addresses help text'),
      '#description' => t('The text will appear on top of User addressbook pane.'),
      '#default_value' => $user_addressbook_help_text['value'],
      '#format' => $user_addressbook_help_text['format'],
    );
    // Help text for add new customer profile form.
    $add_new_customer_profile_help_text = variable_get('commerce_checkout_' . $type . '_add_new_customer_profile_help_text', $help_text_default);
    $form[$type]['commerce_checkout_' . $type . '_add_new_customer_profile_help_text'] = array(
      '#type' => 'text_format',
      '#title' => t('Add new customer profile form help text'),
      '#description' => t('The text will appear on top of Customer profile form.'),
      '#default_value' => $add_new_customer_profile_help_text['value'],
      '#format' => $add_new_customer_profile_help_text['format'],
    );

  }
  $var_info = array(
    'site' => array(
      'type' => 'site',
      'label' => t('Site information'),
      'description' => t('Site-wide settings and other global information.'),
    ),
    'commerce_order' => array(
      'label' => t('Order'),
      'type' => 'commerce_order',
    ),
  );

  $form['container']['commerce_checkout_completion_message_help'] = RulesTokenEvaluator::help($var_info);

  return $form;
}

/**
 * Checkout pane callback: returns a customer profile edit form.
 */
function user_addressbook_commerce_checkout_pane_checkout_form($form, &$form_state, &$checkout_pane, $order) {
  global $user;
  $pane_form = array();

  $pane_id = $checkout_pane['pane_id'];
  if (empty($form_state['customer_profiles_panes']) && empty($form_state['#user_addressbook_profile_type'])) {
    $panes = user_addressbook_commerce_customer_profile_panes();
    // Check if it is about changing a specific address from checkout review page.
    $path_parts = arg();
    if (isset($path_parts[3]) && $panes[$path_parts[3]]) {
      $form_state['#user_addressbook_profile_type'] = $path_parts[3];
    }
    if (!empty($form_state['#user_addressbook_profile_type'])) {
      $form_state['checkout_page']['next_page'] = 'review';
      $form_state['customer_profiles_panes'][0] = $panes[$form_state['#user_addressbook_profile_type']];
    }
    else {
      $form_state['customer_profiles_panes'] = array_values($panes);
    }
    if (!isset($form_state['customer_profiles_step'])) {
      $form_state['customer_profiles_step'] = 0;
    }
  }
  // Always change the next page if it is about changing
  // a specific address from checkout review page.
  // For form rebuilt.
  if (!empty($form_state['#user_addressbook_profile_type'])) {
    $form_state['checkout_page']['next_page'] = 'review';
  }
  $type = $form_state['customer_profiles_panes'][$form_state['customer_profiles_step']]['type'];
  if (!isset($order->data['addresses'])) {
    $order->data['addresses'] = array();
  }
  $user_addressbook_pane_title = variable_get('commerce_checkout_' . $type . '_user_addressbook_pane_title', $form_state['customer_profiles_panes'][$form_state['customer_profiles_step']]['title']);
  $user_addressbook_pane_title = commerce_i18n_string('commerce:checkout:addresses:commerce_checkout_' . $type . '_user_addressbook_pane_title', $user_addressbook_pane_title, array('sanitize' => FALSE));
  $checkout_pane['title'] = $form_state['customer_profiles_panes'][$form_state['customer_profiles_step']]['title'] = $user_addressbook_pane_title;
  $field_name = variable_get('commerce_customer_profile_' . $type . '_field', '');

  // Find the referenced profile using the related reference field...
  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  $customer_profile = NULL;

  // If the associated order field has been set...
  if ($field_name) {
    $customer_profile = $wrapper->{$field_name}->value();
  }
  else {
    // Or try the association stored in the order's data array if no field is set.
    if (!empty($order->data['profiles']['customer_profile_' . $type])) {
      $customer_profile = commerce_customer_profile_load($order->data['profiles']['customer_profile_' . $type]);
    }
  }

  // Get the user addresses.
  $user_addresses = user_addresses_load_by_user($order->uid);
  // Default form mode.
  $mode = 'select_address';
  if (isset($order->data['addresses'][$type]) && $order->data['addresses'][$type] == 'edit') {
    $mode = 'edit_profile';
  }
  elseif (empty($user_addresses)) {
    $mode = 'new_customer_profile';
  }
  // If an AJAX rebuild happened, we might have our data in form state
  elseif (!empty($form_state['pane_' . $pane_id])) {
    $mode = $form_state['pane_' . $pane_id]['mode'];
  }
  $action = NULL;
  if (!empty($form_state['pane_' . $pane_id]) && !empty($form_state['pane_' . $pane_id]['action'])) {
    $action = $form_state['pane_' . $pane_id]['action'];
  }

  // No profile set yet. First see if one exists already.
  if (empty($customer_profile)) {
    $customer_profile = commerce_customer_profile_new($type, $order->uid);

    // Save the new profile.
    commerce_customer_profile_save($customer_profile);

    // Store the profile ID for the related field as specified on the settings form.
    $wrapper = entity_metadata_wrapper('commerce_order', $order);

    if ($field_name = variable_get('commerce_customer_profile_' . $customer_profile->type . '_field', '')) {
      $wrapper->{$field_name} = $customer_profile;
    }
    else {
      // Or make the association in the order's data array if no field was found.
      $order->data['profiles']['customer_profile_' . $customer_profile->type] = $customer_profile->profile_id;
    }
    commerce_order_save($order);
  }

  $wrapper_id = strtr($pane_id, '_', '-') . '-ajax-wrapper';
  // Remember the current profile and mode in form state.
  $form_state['pane_' . $checkout_pane['pane_id']] = array(
    'customer_profile' => $customer_profile,
    'mode' => $mode,
  );

  $pane_form = array(
    '#parents' => array($pane_id),
    '#prefix' => '<div id="' . $wrapper_id . '">',
    '#suffix' => '</div>',
  );
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['pane'] = drupal_get_path('module', 'user_addressbook_commerce') . '/includes/user_addressbook_commerce.checkout_pane.inc';

  // Addresses help text.
  $help_text_default = array(
    'value' => '',
    'format' => 'filtered_html',
  );
  $user_addressbook_help_text = variable_get('commerce_checkout_' . $type . '_user_addressbook_help_text', $help_text_default);
  if (!empty($user_addressbook_help_text['value'])) {
    // Perform translation.
    $user_addressbook_help_text['value'] = commerce_i18n_string('commerce:checkout:addresses:commerce_checkout_' . $type . '_user_addressbook_help_text', $user_addressbook_help_text['value'], array('sanitize' => FALSE));
    // Perform token replacement.
    $user_addressbook_help_text['value'] = token_replace($user_addressbook_help_text['value'], array('commerce-order' => $order), array('clear' => TRUE));
    // Apply the proper text format.
    $user_addressbook_help_text['value'] = check_markup($user_addressbook_help_text['value'], $user_addressbook_help_text['format']);
    $pane_form['user_addressbook_help_text'] = array(
      '#markup' => '<div class="user-addressbook-help-text">' . $user_addressbook_help_text['value'] . '</div>',
      '#weight' => -110,
    );
  }

  // View addressbook button, for edit customer profile case.
  if ($mode == 'edit_profile' && !empty($user_addresses) && $action != 'view_addressbook') {
    $pane_form['select_address'] = array(
      '#type' => 'submit',
      '#value' => t('View addressbook'),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'user_addressbook_ajax_form_refresh',
        'wrapper' => strtr($pane_id, '_', '-') . '-ajax-wrapper',
      ),
      '#submit' => array('user_addressbook_commerce_checkout_pane_view_addressbook'),
      '#weight' => -100,
    );
  }
  // Get the User addressbook view form in the pane.
  if (($mode != 'edit_profile' || ($mode == 'edit_profile' && $action == 'view_addressbook')) && !empty($user_addresses)) {
    $user_addresses_checkout = _user_addressbook_retrieve_view_form($pane_form, $form_state, 'user_addressbook_checkout', 'default', array($user->uid, $wrapper_id, $type, $order->order_id));
    $user_addresses_checkout['#profile_type'] = $type;

    unset($user_addresses_checkout['actions']);
    $pane_form['user_addresses'] = $user_addresses_checkout;
    $pane_form['user_addresses']['#weight'] = -100;
  }

  // Add new profile button.
  if ($mode == 'select_address' || empty($user_addresses)) {
    $pane_form['customer_profile'] = array(
      '#type' => 'submit',
      '#value' => t('Add new'),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'user_addressbook_ajax_form_refresh',
        'wrapper' => strtr($pane_id, '_', '-') . '-ajax-wrapper',
      ),
      '#submit' => array('user_addressbook_commerce_checkout_pane_add_new_customer_profile'),
    );
  }

  // Add/edit profile form. 
  if (in_array($mode, array('new_customer_profile', 'edit_profile')) || empty($user_addresses)) {
    if ($mode == 'new_customer_profile') {
      $new_profile = commerce_customer_profile_new($type, $order->uid);
      $customer_profile = (object) array_intersect_key((array) $customer_profile, (array) $new_profile);
    }

    // Help text for add new customer profile form.
    $add_new_customer_profile_help_text = variable_get('commerce_checkout_' . $type . '_add_new_customer_profile_help_text', $help_text_default);
    if (!empty($add_new_customer_profile_help_text['value'])) {
      // Perform translation.
      $add_new_customer_profile_help_text['value'] = commerce_i18n_string('commerce:checkout:addresses:commerce_checkout_' . $type . '_add_new_customer_profile_help_text', $add_new_customer_profile_help_text['value'], array('sanitize' => FALSE));
      // Perform token replacement.
      $add_new_customer_profile_help_text['value'] = token_replace($add_new_customer_profile_help_text['value'], array('commerce-order' => $order), array('clear' => TRUE));
      // Apply the proper text format.
      $add_new_customer_profile_help_text['value'] = check_markup($add_new_customer_profile_help_text['value'], $add_new_customer_profile_help_text['format']);
      $pane_form['add_new_customer_profile_help_text'] = array(
        '#markup' => '<div class="add-new-customer-profile-help-text">' . $user_addressbook_help_text['value'] . '</div>',
        '#weight' => -50,
      );
    }

    $pane_form['customer_profile'] = array(
      '#type' => 'value',
      '#value' => $customer_profile,
    );
    // Add the field widgets for the profile.
    field_attach_form('commerce_customer_profile', $customer_profile, $pane_form, $form_state);

    // Tweak the form to remove the fieldset from the address field if there
    // is only one on this profile.
    $addressfields = array();
    foreach (commerce_info_fields('addressfield', 'commerce_customer_profile') as $field_name => $field) {
      if (!empty($pane_form[$field_name]['#language'])) {
        $langcode = $pane_form[$field_name]['#language'];

        // Only consider this addressfield if it's represented on the form.
        if (!empty($pane_form[$field_name][$langcode])) {
          $addressfields[] = array($field_name, $langcode);
        }
      }
    }

    // Check to ensure only one addressfield was found on the form.
    if (count($addressfields) == 1) {
      list($field_name, $langcode) = array_shift($addressfields);

      foreach (element_children($pane_form[$field_name][$langcode]) as $delta) {
        // Don't mess with the "Add another item" button that could be present.
        if ($pane_form[$field_name][$langcode][$delta]['#type'] != 'submit') {
          $pane_form[$field_name][$langcode][$delta]['#type'] = 'container';
        }
      }
    }

    $user_address_auto_save = variable_get('commerce_checkout_' . $type . '_user_addressbook_autosave', FALSE);
    // Auto save to user address.
    if ($user_address_auto_save) {
      $pane_form['add_to_user_addressbook'] = array(
        '#type' => 'value',
        '#value' => 1,
      );
    }
    // Optional saving to used address.
    else {
      $pane_form['add_to_user_addressbook'] = array(
        '#type' => 'checkbox',
        '#title' => t('Save to my addressbook'),
        '#default_value' => ($mode == 'new_customer_profile') ? TRUE : FALSE,
      );

      $pane_form['user_address_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Address name') . ' ' . theme('form_required_marker', array()),
        '#element_validate' => array('user_addressbook_commerce_user_address_name_validate'),
        '#states' => array(
          // Hide the settings when the select checkbox is disabled.
          'invisible' => array(
           ':input[name="user_addressbook[add_to_user_addressbook]"]' => array('checked' => FALSE),
          ),
        ),
      );
    }

    if (!empty($user_addresses)) {
      $pane_form['user_address_set_default'] = array(
        '#type' => 'checkbox',
        '#title' => t('Make default'),
        '#states' => array(
          // Hide the settings when the select checkbox is disabled.
          'invisible' => array(
           ':input[name="user_addressbook[add_to_user_addressbook]"]' => array('checked' => FALSE),
          ),
        ),
      );
    }

    // Save button for add/edit customer profile.
    $pane_form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      //'#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'user_addressbook_ajax_form_refresh',
        'wrapper' => strtr($pane_id, '_', '-') . '-ajax-wrapper',
      ),
      '#submit' => array('user_addressbook_commerce_checkout_pane_customer_profile_save'),
    );
    // Cancel button for add/edit customer profile form.
    if (!empty($user_addresses)) {
      $pane_form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#limit_validation_errors' => array(),
        '#ajax' => array(
          'callback' => 'user_addressbook_ajax_form_refresh',
          'wrapper' => strtr($pane_id, '_', '-') . '-ajax-wrapper',
        ),
        '#submit' => array('user_addressbook_commerce_checkout_pane_customer_profile_form_cancel'),
      );
    }
  }

  return $pane_form;
}

/**
 * Submit callback for the "Select" address button.
 */
function user_addressbook_commerce_field_handler_checkout_select_user_address_submit($form, &$form_state) {
  $pane_id = reset($form_state['triggering_element']['#array_parents']);
  $user_address = user_address_load($form_state['triggering_element']['#name']);

  $customer_profile = $form_state['pane_' . $pane_id]['customer_profile'];
  $order = $form_state['order'];
  $customer_profile_pane = $form_state['customer_profiles_panes'][$form_state['customer_profiles_step']];

  // Use this event to do extra operations with Rules to the new account and/or order.
  rules_invoke_all('user_addressbook_commerce_user_address_select', $user_address, $customer_profile, $order);
  // Set the user address order data for this type.
  $order->data['addresses'][$customer_profile->type] = $user_address->address_id;
  commerce_order_save($order);
  // Select address message.
  drupal_set_message(t('Address %address_name was selected for %profile_pane.', array('%address_name' => $user_address->name, '%profile_pane' => $customer_profile_pane['title'])));
  // Next profile
  $form_state['customer_profiles_step']++;
  // Redirect to the next checkout step.
  if (empty($form_state['customer_profiles_panes'][$form_state['customer_profiles_step']])) {
    $order = commerce_order_status_update($form_state['order'], 'checkout_' . $form_state['checkout_page']['next_page'], FALSE, NULL, t('Customer continued to the next checkout page via a submit button.'));
    $form_state['redirect'] = 'checkout/' . $order->order_id . '/' . $form_state['checkout_page']['next_page'];
  }
  // Form rebuild.
  else {
    $form_state['pane_' . $pane_id]['mode'] = 'select_address';
    $form_state['pane_' . $pane_id]['action'] = 'select_address';
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Submit callback for the "view addressbook" button.
 */
function user_addressbook_commerce_checkout_pane_view_addressbook($form, &$form_state) {
  $pane_id = reset($form_state['triggering_element']['#parents']);
  $form_state['pane_' . $pane_id]['action'] = 'view_addressbook';
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit callback for the "add new" customer profile button.
 */
function user_addressbook_commerce_checkout_pane_add_new_customer_profile($form, &$form_state) {
  $pane_id = reset($form_state['triggering_element']['#parents']);
  $form_state['pane_' . $pane_id]['mode'] = 'new_customer_profile';
  $form_state['rebuild'] = TRUE;
}

/**
 * Validate callback for the address "name" form element.
 */
function user_addressbook_commerce_user_address_name_validate($element, &$form_state, $form) {
  $pane_id = reset($form_state['triggering_element']['#array_parents']);
  if (empty($element['#value']) && !empty($form_state['values'][$pane_id]['add_to_user_addressbook'])) {
    form_error($element, t('This address name is required.'));
  }
}

/**
 * Submit callback for the "Save" button of the add/edit customer profile form.
 */
function user_addressbook_commerce_checkout_pane_customer_profile_save($form, &$form_state) {
  $pane_id = reset($form_state['triggering_element']['#parents']);

  $customer_profile = $form_state['values'][$pane_id]['customer_profile'];
  $order = $form_state['order'];
  $customer_profile_pane = $form_state['customer_profiles_panes'][$form_state['customer_profiles_step']];

  // Notify field widgets.
  field_attach_submit('commerce_customer_profile', $customer_profile, $form[$pane_id], $form_state);

  // Save the profile.
  commerce_customer_profile_save($customer_profile);

  // Store the profile ID for the related field as specified on the settings form.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  if ($field_name = variable_get('commerce_customer_profile_' . $customer_profile->type . '_field', '')) {
    $wrapper->{$field_name} = $customer_profile;
  }
  else {
    // Or make the association in the order's data array if no field was found.
    $order->data['profiles']['customer_profile_' . $customer_profile->type] = $customer_profile->profile_id;
  }

  // Save order customer profile message.
  drupal_set_message(t('Address of %profile_pane added to your order.', array('%profile_pane' => $customer_profile_pane['title'])));

  // Customer profile to user address.
  if (!empty($form_state['values'][$pane_id]['add_to_user_addressbook'])) {
    $user_address = user_addressbook_new($order->uid);
    // Set the user address name.
    if (!empty($form_state['values'][$pane_id]['user_address_name'])) {
      $user_address->name = $form_state['values'][$pane_id]['user_address_name'];
    }
    else {
      $customer_profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $customer_profile);
      $commerce_customer_address = $customer_profile_wrapper->commerce_customer_address->value();
      if (!empty($commerce_customer_address['name_line'])) {
        $user_address->name = $commerce_customer_address['name_line'];
      }
      elseif (!empty($commerce_customer_address['first_name'])) {
        $user_address->name = $commerce_customer_address['first_name'] . ' ' . $commerce_customer_address['last_name'];
      }
      else {
        $user_address->name = $commerce_customer_address['thoroughfare'];
      }
    }
    // Use this event to do extra operations with Rules to the new account and/or order.
    rules_invoke_all('user_addressbook_commerce_add_new_customer_profile', $user_address, $customer_profile, $order);
    if (!empty($form_state['values'][$pane_id]['user_address_set_default'])) {
      user_addressbook_set_default_address($user_address, $customer_profile->type);
    }
    $order->data['addresses'][$customer_profile->type] = $user_address->address_id;
    commerce_order_save($order);
    // Save user address message.
    drupal_set_message(t('%address_name address of %profile_pane added to your addressbook.', array('%address_name' => $user_address->name, '%profile_pane' => $customer_profile_pane['title'])));
  }
  else {
    $order->data['addresses'][$customer_profile->type] = 'edit';
    commerce_order_save($order);
  }
  // Next profile
  $form_state['customer_profiles_step']++;
  // Redirect to the next checkout step.
  if (empty($form_state['customer_profiles_panes'][$form_state['customer_profiles_step']])) {
    $order = commerce_order_status_update($form_state['order'], 'checkout_' . $form_state['checkout_page']['next_page'], FALSE, NULL, t('Customer continued to the next checkout page via a submit button.'));
    $form_state['redirect'] = 'checkout/' . $order->order_id . '/' . $form_state['checkout_page']['next_page'];
  }
  else {
    $form_state['pane_' . $pane_id]['mode'] = 'select_address';
    $form_state['pane_' . $pane_id]['action'] = 'save_profile';
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Submit callback for the "cancel" button of the add/edit customer profile form.
 */
function user_addressbook_commerce_checkout_pane_customer_profile_form_cancel($form, &$form_state) {
  $pane_id = reset($form_state['triggering_element']['#array_parents']);

  $customer_profile = $form_state['pane_' . $pane_id]['customer_profile'];
  $order = $form_state['order'];
  if (!empty($order->data['addresses'][$customer_profile->type]) && $order->data['addresses'][$customer_profile->type] == 'edit') {
    unset($order->data['addresses'][$customer_profile->type]);
    commerce_order_save($order);
  }
  $form_state['pane_' . $pane_id]['mode'] = 'select_address';

  $form_state['rebuild'] = TRUE;
}

/**
 * Checkout pane callback: returns the customer profiles review data for the
 *   Review checkout pane.
 *
 * Use commerce_customer_profile_pane_review.
 */
function user_addressbook_commerce_checkout_pane_review($form, $form_state, $checkout_pane, $order) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  module_load_include('inc', 'commerce_customer', 'includes/commerce_customer.checkout_pane');
  $content = array();

  $panes = user_addressbook_commerce_customer_profile_panes();
  foreach ($panes as $type => $pane) {
    $content['addresses'][$type] = array(
      '#type' => 'item',
      '#title' => $pane['title'],
      '#id' => drupal_html_class($pane['pane_id']),
      '#markup' => commerce_customer_profile_pane_review($form, $form_state, $pane, $order),
    );
  }

  return drupal_render($content);
}
