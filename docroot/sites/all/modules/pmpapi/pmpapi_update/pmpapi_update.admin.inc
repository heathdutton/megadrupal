<?php

function pmpapi_update_config_form() {

  // If not subscribed, try to subscribe
  $updated = variable_get('pmpapi_update_subscribed_to_updated');
  $deleted = variable_get('pmpapi_update_subscribed_to_deleted');
  if (!$updated) {
    pmpapi_update_subscribe_client('updated');
  }
  if (!$deleted) {
    pmpapi_update_subscribe_client('deleted');
  }

  $form = array();
  $form['pmpapi_update_updates_active'] = array(
    '#type' => 'radios',
    '#title' => t('Activate automatic updates from the PMP'),
    '#default_value' => variable_get('pmpapi_update_updates_active', 1),
    '#options' => array('No', 'Yes'),
  );

  $messages = array();
  if ($updated) {
    $messages[] = 'This site is subscribed to the topic: updated.';
  }
  else {
    $messages[] = 'This site is NOT subscribed to the topic: updated.';
  }

  if ($deleted) {
    $messages[] = 'This site is subscribed to the topic: deleted.';
  }
  else {
    $messages[] = 'This site is NOT subscribed to the topic: deleted.';
  }
  $form['subscription_messages'] = array(
    '#markup' => theme_item_list(
      array(
        'items' => $messages,
        'type' => 'ul',
        'title' => 'Subscriptions',
        'attributes' => array(),
      )
    ),
  );
  return system_settings_form($form);
}

function pmpapi_update_subscribe_client($topic) {
  $topic_uri = pmpapi_update_get_topic_uri($topic);
  $update = new PMPAPIDrupalUpdate($topic_uri, 'subscribe');
  $pending = array(
    'verify_token' => $update->verify_token,
    'mode' => $update->mode,
    'topic_uri' => $topic_uri,
  );
  variable_set('pmpapi_update_subscribe_pending', $pending);
  $update->sendRequestToHub();
  if ($update->response->code == 204) {
    variable_set('pmpapi_update_subscribed_to_' . $topic, TRUE);
  }
  variable_del('pmpapi_update_subscribe_pending');
}