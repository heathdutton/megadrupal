<?php

/**
 * @file
 * Checkout pane callback functions for the commerce_mailchimp module.
 */

/**
 * Checkout pane callback: returns the MailChimp pane's settings form.
 */
function commerce_mailchimp_pane_settings_form($checkout_pane) {
  $form = array();

  $lists = mailchimp_get_lists();
  foreach ($lists as $list_id => $list) {
    $form["commerce_mailchimp_{$list_id}"] = array(
      '#type' => 'fieldset',
      '#title' => t($list['name'] . ' List Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form["commerce_mailchimp_{$list_id}"]["commerce_mailchimp_{$list_id}_checkout_pane_enabled"] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable this list on the Checkout Pane'),
      '#default_value' => variable_get("commerce_mailchimp_{$list_id}_checkout_pane_enabled", 0),
      '#description' => 'Show a checkbox for subscribing to this list on the checkout pane.',
    );
    $form["commerce_mailchimp_{$list_id}"]["commerce_mailchimp_{$list_id}_checkout_pane_toggled"] = array(
      '#type' => 'select',
      '#title' => t('Default Value for subscription checkbox'),
      '#options' => array(
        1 => 'Checked',
        0 => 'Unchecked'
      ),
      '#default_value' => variable_get("commerce_mailchimp_{$list_id}_checkout_pane_toggled", 1),
      '#description' => 'Show a checkbox for subscribing to this list on the checkout pane.',
    );
    $form["commerce_mailchimp_{$list_id}"]["commerce_mailchimp_{$list_id}_checkout_pane_toggled_disabled"] = array(
      '#type' => 'checkbox',
      '#title' => t('Force'),
      '#default_value' => variable_get("commerce_mailchimp_{$list_id}_checkout_pane_toggled_disabled", 0),
    );
    $form["commerce_mailchimp_{$list_id}"]["commerce_mailchimp_{$list_id}_checkout_pane_label"] = array(
      '#type' => 'textfield',
      '#title' => t($list['name'] . ' Checkbox label'),
      '#default_value' => variable_get("commerce_mailchimp_{$list_id}_checkout_pane_label", 'Subscribe To The ' . $list['name'].' Mailing List'),
    );
    $form["commerce_mailchimp_{$list_id}"]["commerce_mailchimp_{$list_id}_checkout_pane_desc"] = array(
      '#type' => 'textfield',
      '#title' => t($list['name'] . ' Checkbox description'),
      '#default_value' => variable_get("commerce_mailchimp_{$list_id}_checkout_pane_desc", ""),
    );
  }

  return $form;
}

/**
 * Checkout pane callback: returns the MailChimp View for inclusion in the
 *   checkout form.
 */
function commerce_mailchimp_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();

  $lists = mailchimp_get_lists();

  foreach ($lists as $list_id => $list) {
    mailchimp_cache_clear_member($list_id, $order->mail);
    if (variable_get("commerce_mailchimp_{$list_id}_checkout_pane_enabled", 0) && !mailchimp_is_subscribed($list_id, $order->mail)) {
      $pane_form[$list_id] = array(
        '#type' => 'checkbox',
        '#title' => variable_get("commerce_mailchimp_{$list_id}_checkout_pane_label", 'Subscribe To The ' . $list['name'] . ' Mailing List'),
        '#default_value' => isset($order->data[$checkout_pane['pane_id']][$list_id])
          ? $order->data[$checkout_pane['pane_id']][$list_id]
          : variable_get("commerce_mailchimp_{$list_id}_checkout_pane_toggled", 1),
        '#description' => variable_get("commerce_mailchimp_{$list_id}_checkout_pane_desc", ""),
      );
    }
  }

  return $pane_form;
}

/**
 * Checkout pane callback: validates a MailChimp edit form.
 */
function commerce_mailchimp_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  if(!empty($form_state['values']['commerce_mailchimp'])){
    foreach ($form_state['values']['commerce_mailchimp'] as $list_id => $value) {
      $force = variable_get("commerce_mailchimp_{$list_id}_checkout_pane_toggled_disabled",0);
      if ($force && !$value) {
        form_set_error('', t("To be able to send important product updates we need your permission to email you. Please check the email subscription box below to give us that permission."));
        return FALSE;
      }
    }
  }

  return TRUE;
}

/**
 * Checkout pane callback: submits a MailChimp edit form.
 */
function commerce_mailchimp_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  if (isset($form_state['values'][$checkout_pane['pane_id']])) {
    $wrapper = entity_metadata_wrapper('commerce_order', $order);
    $order->data[$checkout_pane['pane_id']] = $form_state['values'][$checkout_pane['pane_id']];
  }
}

/**
 * Checkout pane callback: returns the MailChimp review data for the
 *   Review checkout pane.
 */
function commerce_mailchimp_pane_review($form, $form_state, $checkout_pane, $order) {
  if (!isset($order->data[$checkout_pane['pane_id']])) {
    return;
  }
  $lists = mailchimp_get_lists();
  $items = array();
  foreach ($order->data[$checkout_pane['pane_id']] as $list_id => $value) {
    if ($value) {
      $list = $lists[$list_id];
      $items[] = variable_get("commerce_mailchimp_{$list_id}_checkout_pane_label", 'Subscribe To The ' . $list['name'] . ' Mailing List');
    }
  }
  return theme('item_list', array('items' => $items));
}
