<?php

/**
 * @file
 * Based on commerce/modules/customer/commerce_customer.checkout_pane.inc
 */

/**
 * Checkout pane callback: returns the customer profile pane's settings form.
 */
function commerce_single_address_pane_settings_form($checkout_pane) {
  $form = array();

  // Extract the type of profile represented by this pane from its ID.
  $type = substr($checkout_pane['pane_id'], 17); // Removes 'customer_profile_'

  // Build an options array of customer profile reference fields available for
  // the data from this customer profile pane.
  $options = array();

  foreach (commerce_info_fields('commerce_customer_profile_reference', 'commerce_order') as $name => $field) {
    if ($type == $field['settings']['profile_type']) {
      $instance = field_info_instance('commerce_order', $name, 'commerce_order');
      $options[$name] = check_plain($instance['label']);
    }
  }

  $form['commerce_' . $checkout_pane['pane_id'] . '_field'] = array(
    '#type' => 'select',
    '#title' => t('Related customer profile reference field'),
    '#description' => t('Specify the customer profile reference field on the order to populate with profile data from this checkout pane.'),
    '#options' => $options,
    '#empty_value' => '',
    '#default_value' => variable_get('commerce_' . $checkout_pane['pane_id'] . '_field', ''),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Checkout pane callback: returns a customer profile edit form.
 */
function commerce_single_address_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  global $user;

  $pane_id = $checkout_pane['pane_id'];
  // Extract the type of profile represented by this pane from its ID.
  $type = substr($pane_id, 17); // Removes 'customer_profile_'

  $field_name = variable_get('commerce_' . $pane_id . '_field', '');
  // The specified profile reference field doesn't exist on this order type.
  // A consequence of the checkout settings applying for all order types.
  if ($field_name && !field_info_instance('commerce_order', $field_name, $order->type)) {
    return array();
  }

  // Find the referenced profile using the related reference field...
  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  $profile = NULL;
  $mode = 'view';

  // If the associated order field has been set...
  if ($field_name) {
    $profile = $wrapper->{$field_name}->value();
  }
  else {
    // Or try the association stored in the order's data array if no field is set.
    if (!empty($order->data['profiles'][$checkout_pane['pane_id']])) {
      $profile = commerce_customer_profile_load($order->data['profiles'][$pane_id]);
    }
  }

  // If an AJAX rebuild happened, we might have our data in form state
  if (!empty($form_state['pane_' . $pane_id])) {
    $profile = $form_state['pane_' . $pane_id]['profile'];
    $mode = $form_state['pane_' . $pane_id]['mode'];
  }

  // No profile set yet. First see if one exists already.
  if (empty($profile)) {
    $profile = commerce_single_address_active_profile_load($user->uid, $type);

    if ($profile) {
      // Incomplete profiles need to go into edit mode.
      if (commerce_single_address_profile_is_incomplete($profile)) {
        $mode = 'edit';
      }
    }
    else {
      // No profiles found. Create a new one.
      $profile = commerce_customer_profile_new($type, $order->uid);
      $mode = 'new';
    }
  }

  // Remember the current profile and mode in form state.
  $form_state['pane_' . $checkout_pane['pane_id']] = array(
    'profile' => $profile,
    'mode' => $mode,
  );

  // Add the entity context of the current cart order.
  $profile->entity_context = array(
    'entity_type' => 'commerce_order',
    'entity_id' => $order->order_id,
  );

  $pane_form = array(
    '#parents' => array($pane_id),
    '#prefix' => '<div id="' . strtr($pane_id, '_', '-') . '-ajax-wrapper">',
    '#suffix' => '</div>',
  );
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['pane'] = drupal_get_path('module', 'commerce_single_address') . '/commerce_single_address.checkout_pane.inc';

  if ($mode == 'view') {
    $content = entity_view('commerce_customer_profile', array($profile->profile_id => $profile), 'customer');
    $pane_form['rendered_profile'] = array(
      '#markup' => drupal_render($content),
    );
    $pane_form['edit_button'] = array(
      '#type' => 'submit',
      '#name' => 'pane-' . $pane_id . '-edit',
      '#value' => t('Edit'),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'commerce_single_address_pane_refresh',
        'wrapper' => strtr($pane_id, '_', '-') . '-ajax-wrapper',
      ),
      '#submit' => array('commerce_single_address_pane_edit'),
    );
  }
  else {
    // Add the field widgets for the profile.
    field_attach_form('commerce_customer_profile', $profile, $pane_form, $form_state);

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
 }

  return $pane_form;
}

/**
 * Ajax callback for the "edit" button.
 */
function commerce_single_address_pane_refresh($form, &$form_state) {
  $pane_id = reset($form_state['triggering_element']['#parents']);
  return $form[$pane_id];
}

/**
 * Submit callback for the "edit" button.
 */
function commerce_single_address_pane_edit($form, &$form_state) {
  $pane_id = reset($form_state['triggering_element']['#parents']);
  $form_state['pane_' . $pane_id]['mode'] = 'edit';
  $form_state['rebuild'] = TRUE;
}

/**
 * Checkout pane callback: validates a customer profile edit form.
 */
function commerce_single_address_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  $pane_id = $checkout_pane['pane_id'];
  $field_name = variable_get('commerce_' . $pane_id . '_field', '');
  // The specified profile reference field doesn't exist on this order type.
  // A consequence of the checkout settings applying for all order types.
  if ($field_name && !field_info_instance('commerce_order', $field_name, $order->type)) {
    return TRUE;
  }

  // Validate "edit" and "new" modes.
  if ($form_state['pane_' . $pane_id]['mode'] != 'view') {
    $profile = $form_state['pane_' . $pane_id]['profile'];

    // Notify field widgets to validate their data.
    field_attach_form_validate('commerce_customer_profile', $profile, $form[$checkout_pane['pane_id']], $form_state);

    // If there were any errors in the field attach validate process for fields on
    // this checkout pane's customer profile, fail the checkout pane validation.
    if ($errors = form_get_errors()) {
      foreach ($errors as $field => $error) {
        if (substr($field, 0, strlen($pane_id) + 2) == $pane_id . '][') {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

/**
 * Checkout pane callback: submits a customer profile edit form.
 */
function commerce_single_address_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $pane_id = $checkout_pane['pane_id'];
  $field_name = variable_get('commerce_' . $pane_id . '_field', '');
  // The specified profile reference field doesn't exist on this order type.
  // A consequence of the checkout settings applying for all order types.
  if ($field_name && !field_info_instance('commerce_order', $field_name, $order->type)) {
    return;
  }

  $profile = $form_state['pane_' . $pane_id]['profile'];
  // Save the customer profile in the "new" and "edit" modes.
  if ($form_state['pane_' . $pane_id]['mode'] != 'view') {
    // Ensure the profile is active.
    $profile->status = TRUE;

    // Set the profile's uid if it's being created at this time.
    if (empty($profile->profile_id)) {
      $profile->uid = $order->uid;
    }

    // Notify field widgets.
    field_attach_submit('commerce_customer_profile', $profile, $form[$checkout_pane['pane_id']], $form_state);

    // Make sure that the incomplete flag is removed, if it was previously set.
    if (!empty($profile->data['incomplete'])) {
      unset($profile->data['incomplete']);
    }

    // Save the profile.
    commerce_customer_profile_save($profile);
  }

  // Store the profile ID for the related field as specified on the settings form.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  if ($field_name) {
    $wrapper->{$field_name} = $profile;
  }
  else {
    // Or make the association in the order's data array if no field was found.
    $order->data['profiles'][$checkout_pane['pane_id']] = $profile->profile_id;
  }
}

/**
 * Checkout pane callback: returns the cart contents review data for the
 *   Review checkout pane.
 *
 * Copied verbatim from commerce_customer_profile_pane_review.
 */
function commerce_single_address_pane_review($form, $form_state, $checkout_pane, $order) {
  // Load the profile based on the related customer profile reference field...
  if ($field_name = variable_get('commerce_' . $checkout_pane['pane_id'] . '_field', '')) {
    $profile = entity_metadata_wrapper('commerce_order', $order)->{$field_name}->value();
  }
  else {
    // Or use the association stored in the order's data array if no field is set.
    $profile = commerce_customer_profile_load($order->data['profiles'][$checkout_pane['pane_id']]);
  }

  if ($profile) {
    $content = entity_view('commerce_customer_profile', array($profile->profile_id => $profile), 'customer');

    return drupal_render($content);
  }
  else {
    return t('No information');
  }
}
