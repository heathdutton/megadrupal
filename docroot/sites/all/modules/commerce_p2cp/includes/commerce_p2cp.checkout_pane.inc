<?php

/**
 * Implements the callback of the checkout pane form setting.
 */
function commerce_p2cp_panes_contents_settings_form($checkout_pane) {
  $form = array();
  return $form;
}

/**
 * Implements the callback of the checkout pane form
 */
function commerce_p2cp_panes_contents_checkout_form(&$form, &$form_state, $checkout_pane, $order) {
  $pane_form = array('#parents' => array($checkout_pane['pane_id']));

  // Extract the type of profile2 represented by this pane from its ID.
  $type = substr($checkout_pane['pane_id'], strlen(P2CP_PREFIX)); // Removes P2CP_PREFIX

  // Find the referenced profile using the related reference field...
  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  $profile2 = NULL;

  // If the associated order field has been set...
  if ($field_name = variable_get($checkout_pane['pane_id'] . '_field', '')) {
    $wrapper->{$field_name}->value();
  }
  else {
    // Or try the association stored in the order's data array if no field is set.
    if (!empty($order->data['profile2'][$checkout_pane['pane_id']])) {
      $profile2 = profile2_load($order->data['profiles2'][$checkout_pane['pane_id']]);
    }
  }

  // Create a new profile of the specified type if it hasn't already been made.
  if (empty($profile2)) {
    $profile2 = entity_get_controller('profile2')->create(array(
      'type' => $type,
      'user' => $order->uid,
    ));
  }

  // Add the entity context of the current cart order.
  $profile2->entity_context = array(
    'entity_type' => 'commerce_order',
    'entity_id' => $order->order_id,
  );

  $pane_form['profile2'] = array(
    '#type' => 'value',
    '#value' => $profile2,
  );

  // Add the field widgets for the profile.
  field_attach_form('profile2', $profile2, $pane_form, $form_state);

  return $pane_form;
}

/**
 * Implements the callback for the checkout pane form validate
 */
function commerce_p2cp_panes_contents_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  $profile2 = $form_state['values'][$checkout_pane['pane_id']]['profile2'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('profile2', $profile2, $form[$checkout_pane['pane_id']], $form_state);

  return TRUE;
}

/**
 * Implements the callback for the checkout pane form submit
 */
function commerce_p2cp_panes_contents_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $profile2 = $form_state['values'][$checkout_pane['pane_id']]['profile2'];

  // Set the profile's uid if it's being created at this time.
  if (empty($profile2->pid)) {
    $profile2->uid = $order->uid;
  }

  // Notify field widgets.
  field_attach_submit('profile2', $profile2, $form[$checkout_pane['pane_id']], $form_state);

  // Save the profile.
  $profile2->save();

  // Store the profile ID for the related field as specified on the settings form.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  if ($field_name = variable_get($checkout_pane['pane_id'] . '_field', '')) {
    $wrapper->{$field_name} = $profile2;
  }
  else {
    // Or make the association in the order's data array if no field was found.
    $order->data['profiles'][$checkout_pane['pane_id']] = $profile2->pid;
  }
}

/**
 * Checkout pane callback: returns the cart contents review data for the
 *   Review checkout pane.
 */
function commerce_p2cp_panes_contents_review($form, $form_state, $checkout_pane, $order) {
  // Load the profile based on the related customer profile reference field...
  if ($field_name = variable_get($checkout_pane['pane_id'] . '_field', '')) {
    $profile2 = entity_metadata_wrapper('commerce_order', $order)->{$field_name}->value();
  }
  else {
    // Or use the association stored in the order's data array if no field is set.
    $profile2 = profile2_load($order->data['profiles'][$checkout_pane['pane_id']]);
  }

  if ($profile2) {
    // @TODO use a specific display type instead of "account"
    $content = entity_view('profile2', array($profile2->pid => $profile2), 'account');
    return drupal_render($content);
  }
  else {
    return t('No information');
  }
}
