<?php

/**
 * @file
 * Administrative forms for Mandrill Webhooks module.
 */

/**
 * Administrative settings.
 *
 * @param array $form
 *   The form
 *
 * @param array $form_state
 *   The form state
 *
 * @return array
 *   An array containing form items to place on the module settings page.
 */
function mandrill_webhooks_config_form($form, &$form_state) {
  $table_data = array();
  $token = drupal_get_token(MANDRILL_WEBHOOKS_TOKEN);

  $rebuild = mandrill_webhooks_rebuild_local_cache();
  $webhooks = variable_get(MANDRILL_WEBHOOKS_VARIABLE, array());
  if (empty($webhooks)) {
    drupal_set_message(t('There are no Webhooks configured yet'), 'warning');
  }
  else {
    foreach ($webhooks as $webhook) {
      $table_data[] = array(
        'id' => $webhook['id'],
        'endpoint' => $webhook['url'],
        'key' => $webhook['auth_key'],
        'events' => implode(', ', $webhook['events']),
        'actions' => l(t('update'), 'admin/config/services/mandrill/webhooks/' . $webhook['id'] . '/update') . ' ' . l(t('delete'), 'admin/config/services/mandrill/webhooks/' . $webhook['id'] . '/delete/' . $token),
      );
    }
  }

  $table = array(
    '#theme' => 'table',
    '#header' => array(
      t('Mandrill id'),
      t('Endpoint'),
      t('Key'),
      t('Events'),
      t('Actions')),
    '#rows' => $table_data,
  );

  // Bring in dynamic form.
  $form = mandrill_add_update_form();
  $form['current'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Current Webhooks'),
  );
  $form['current']['webhooks'] = array(
    '#markup' => drupal_render($table),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Validate function for mandrill_webhooks_config_form.
 *
 * @param array $form
 *   The Form
 *
 * @param array $form_state
 *   The Form state
 */
function mandrill_webhooks_config_form_validate($form, &$form_state) {
  $url = $form_state['values']['endpoint_url'] . $form_state['values']['endpoint'];
  $description = $form_state['values']['description'];
  $events = array();
  foreach ($form_state['values']['events'] as $event => $status) {
    if ($status !== 0) {
      $events[] = $event;
    }
  }
  if (!empty($form_state['values']['endpoint']) && !empty($events)) {
    try {
      $mailer = mandrill_get_api_object();
      $webhooks = $mailer->webhooks->add($url, $description, $events);
    }
    catch (Mandrill_Exception $e) {
      drupal_set_message(t('Mandrill: %message', array('%message' => check_plain($e->getMessage()))), 'error');
      watchdog_exception('mandrill', $e);
      return FALSE;
    }
  }
}

/**
 * Submit function for mandrill_webhooks_config_form.
 *
 * @param array $form
 *   The form
 *
 * @param array $form_state
 *   The form state
 */
function mandrill_webhooks_config_form_submit($form, &$form_state) {
  mandrill_webhooks_rebuild_local_cache();
  variable_set('mandrill_webhooks_log_data', $form_state['values']['log']);
}

/**
 * Updates a Webhook.
 *
 * @param array $form
 *   The form
 *
 * @param array $form_state
 *   The form state
 *
 * @param string $id
 *   The id of the webhook
 *
 * @return array
 *   $form An array containing form items to place on the module settings page.
 */
function mandrill_webhooks_update_form($form, &$form_state, $id) {
  $form = mandrill_add_update_form($id);
  return $form;
}

/**
 * Validate function for mandrill_webhooks_update_form.
 *
 * @param array $form
 *   The form
 *
 * @param array $form_state
 *   The form state
 *
 * @return bool
 *   Validation for form
 */
function mandrill_webhooks_update_form_validate($form, &$form_state) {
  $id = $form_state['values']['id'];
  $url = $form_state['values']['endpoint'];
  $description = $form_state['values']['description'];
  $events = array();
  foreach ($form_state['values']['events'] as $event => $status) {
    if ($status !== 0) {
      $events[] = $event;
    }
  }
  if (!empty($form_state['values']['endpoint']) && !empty($events)) {
    try {
      $mailer = mandrill_get_api_object();
      $webhook = $mailer->webhooks->update($id, $url, $description, $events);
      drupal_set_message(t('Webhook @id updated successfully', array('@id' => $webhook['id'])));
    }
    catch (Mandrill_Exception $e) {
      drupal_set_message(t('Mandrill: %message', array('%message' => check_plain($e->getMessage()))), 'error');
      watchdog_exception('mandrill', $e);
      return FALSE;
    }
  }
}

/**
 * Submit function for mandrill_webhooks_update_form.
 *
 * @param array $form
 *   The form
 *
 * @param array $form_state
 *   The form state
 */
function mandrill_webhooks_update_form_submit($form, &$form_state) {
  mandrill_webhooks_rebuild_local_cache();
  drupal_goto('admin/config/services/mandrill/webhooks');
}


/**
 * Dynamic form generation.
 *
 * @param int $id
 *   the Mandrill Webhook id.
 *
 * @return array
 *   the form array.
 */
function mandrill_add_update_form($id = NULL) {
  global $base_url;
  $endpoint_url = $base_url . '/' . MANDRILL_WEBHOOKS_ENDPOINT . '/';

  if (!$id) {
    $task = 'Add';
  }
  else {
    $task = 'Update';
    if (!$webhook = mandrill_webhooks_get_webhook($id)) {
      return array();
    }
    $endpoint_url = $webhook['url'];
    $description  = $webhook['description'];
  }

  $form['add_update'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('@task a Webhook', array('@task' => $task)),
  );
  $form['add_update']['endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint string'),
    '#required' => TRUE,
    '#default_value' => isset($id) ? $endpoint_url : NULL,
    '#description' => t('Your endpoint will be <strong>@url[endpoint_string]</strong>', array('@url' => $endpoint_url)),
  );
  $form['add_update']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#required' => TRUE,
    '#default_value' => isset($id) ? $description : NULL,
  );

  $form['add_update']['events'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Events'),
    '#required' => TRUE,
    '#options' => array(
      'send' => t('Message Is Sent'),
      // Deferral seems to not be accepted by Mandrill's API as an event.
      // 'deferral' => t('Message Is Delayed'),
      'hard_bounce' => t('Message Is Bounced'),
      'soft_bounce' => t('Message Is Soft-Bounced'),
      'open' => t('Message Is Opened'),
      'click' => t('Message Is Clicked'),
      'spam' => t('Message Is Marked As Spam'),
      'unsub' => t('Message Recipient Unsubscribes'),
      'reject' => t('Message Is Rejected'),
    ),
    '#default_value' => isset($id) ? $webhook['events'] : array(),
  );
  $form['add_update']['endpoint_url'] = array(
    '#type' => 'hidden',
    '#value' => $endpoint_url,
  );
  if (isset($id)) {
    $form['add_update']['id'] = array(
      '#type' => 'hidden',
      '#value' => $id,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('@task Webhook', array('@task' => $task)),
    );
  }
  return $form;
}
