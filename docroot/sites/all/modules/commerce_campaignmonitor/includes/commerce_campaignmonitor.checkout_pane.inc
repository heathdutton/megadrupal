<?php

/**
 * Payment pane: form callback.
 */
function commerce_campaignmonitor_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();
  global $user;

  // Get connected to the API and get lists.
  $cm = CampaignMonitor::getConnector();
  $lists = $cm->getLists();

  // Build options for the form selector.
  $options = array();
  $default = array();
  foreach ($lists as $list_id => $list) {
    // Check if the list is selected to be shown.
    $list_options = variable_get('campaignmonitor_list_' . $list_id, array());
    if (campaignmonitor_is_list_enabled($list_id) && isset($list_options['display']['user']) && $list_options['display']['user']) {
      $options[$list_id] = $list['name'];

      // Check if the user is subscribed to the current list.
      $default[$list_id] = 0;
      if ($user->uid == 0 || $cm->isSubscribed($list_id, $user->mail)) {
        $default[$list_id] = $list_id;
      }
    }
  }

  $defaults = variable_get('campaignmonitor_general', array());
  if (!empty($options)) {
    $pane_form['lists'] = array(
      '#type' => 'checkboxes',
      '#description' => !empty($defaults['instructions']) ? t($defaults['instructions']) : t('Select the news lists, that you want to subscribe to from campaign monitor.'),
      '#options' => $options,
      '#default_value' => $default,
    );
  }
  else {
    drupal_set_message('There are no available lists to subscribe to at the moment.', 'warning');
  }

  return $pane_form;
}

function commerce_campaignmonitor_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  global $user;
  if ($user->uid == 0) {
    // user is not logged in, we assign the email address and name from the account pane to subscribe the user
    $user->mail = $user->name = $form_state['values']['account']['login']['mail'];
  }

  // Get connected to the API and get lists.
  $cm = CampaignMonitor::getConnector();
  $lists_info = $cm->getLists();

  // Loop through the lists
  if (isset($form_state['values']['commerce_campaignmonitor']['lists'])) {
    foreach ($form_state['values']['commerce_campaignmonitor']['lists'] as $list_id => $selected) {
      if ($selected) {
        // If not subscribed, subscribe else do nothing. The subscribe state is
        // already in the cache, so it cheaper to check then re-subscribe.
        if (!$cm->isSubscribed($list_id, $user->mail)) {
          if (!$cm->subscribe($list_id, $user->mail, $user->name)) {
            form_set_error('', t('You were not subscribed to the list, please try again.'));
            return FALSE;
          }
          drupal_set_message(t('You are now subscribed to the "@list" list.', array('@list' => $lists_info[$list_id]['name'])), 'status');
        }
      }
      else {
        // We block the unsubscribe -- this is not the place
      }
    }
  }

  return TRUE;
}