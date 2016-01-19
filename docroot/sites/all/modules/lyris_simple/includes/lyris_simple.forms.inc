<?php
/**
 * @file
 * Form processing for Lyris Simple.
 *
 * @todo: Add flood control options.
 */

/**
 * Subscribe form.
 * FORM
 */
function lyris_simple_subscribe_form($form, &$form_state) {
  $form = array();
  $membername = variable_get('lyris_simple_member_name', LYRIS_SIMPLE_NAME_OPTIONAL);
  $data_fields = array(
    'list' => variable_get('lyris_simple_list_name'),
    'confirm' => variable_get('lyris_simple_subscribe_confirm'),
    'showconfirm' => 'F',
    'appendsubinfotourl' => 'F',
  );

  // If a Lyris list has not been set, let admins know and hide the form.
  if (empty($data_fields['list'])) {
    if (user_access('administer lyris simple')) {
      $link = l(t('set the Lyris list'), 'admin/config/services/lyris-simple', array('query' => drupal_get_destination()));
      $form['nolist'] = array(
        '#type' => 'markup',
        '#markup' => t('You need to !link for users to subscribe to.', array('!link' => $link)),
      );
    }

    return $form;
  }

  // Check for flood control
  $threshold = variable_get('lyris_simple_flood_threshold', LYRIS_SIMPLE_FLOOD_DEFAULT_THRESHOLD);
  $window = variable_get('lyris_simple_flood_window', LYRIS_SIMPLE_FLOOD_DEFAULT_WINDOW);
  if ($threshold > 0 && !user_access('lyris simple bypass flood control') && !flood_is_allowed('lyris_simple_subscribe', $threshold, $window)) {
    if ($message = variable_get('lyris_simple_flood_message', array())) {
      $form['flooded'] = array(
        '#type' => 'markup',
        '#markup' => check_markup($message['value'], $message['format']),
      );
    }

    return $form;
  }

  $form['data']['#tree'] = TRUE;

  // Member's Email
  $form['data']['email'] = array(
    '#type' => 'emailfield',
    '#title' => t('Your Email'),
    '#required' => TRUE,
    '#weight' => 1,
    '#element_validate' => array('elements_validate_email'),
  );

  // Add the user's email if logged in.
  if (user_is_logged_in()) {
    $form['data']['email']['#default_value'] = $GLOBALS['user']->mail;
  }

  // Add field for user name
  if (in_array($membername, array(LYRIS_SIMPLE_NAME_REQUIRED, LYRIS_SIMPLE_NAME_OPTIONAL))) {
    $form['data']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Your Name'),
      '#required' => $membername == LYRIS_SIMPLE_NAME_REQUIRED,
      '#weight' => 5,
    );

    if (user_is_logged_in()) {
      $form['data']['name']['#default_value'] = format_username($GLOBALS['user']);
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Subscribe'),
    '#weight' => 10,
  );

  // Add URL field so we can redirect
  $form['redirect'] = array(
    '#type' => 'value',
    '#value' => variable_get('lyris_simple_subscribe_redirect', ''),
  );

  // Add data fields for lyris
  foreach ($data_fields as $field => $value) {
    $form['data'][$field] = array(
      '#type' => 'value',
      '#value' => $value,
    );
  }

  return $form;
}

/**
 * Subscribe form.
 * SUBMIT
 */
function lyris_simple_subscribe_form_submit($form, &$form_state) {
  $redirect = $form_state['values']['redirect'];
  $vars = array(
    '@email' => $form_state['values']['data']['email'],
    '@name' => isset($form_state['values']['data']['name']) ? '(' . $form_state['values']['data']['name'] . ')' : '',
    '@list' => $form_state['values']['data']['list'],
    '%email' => $form_state['values']['data']['email'],
    '%name' => isset($form_state['values']['data']['name']) ? $form_state['values']['data']['name'] : '',
    '%list' => $form_state['values']['data']['list'],
  );

  // Talk to Lyris
  $response = lyris_simple_request(LYRIS_SIMPLE_SUBSCRIBE, $form_state['values']['data']);
  $status = lyris_simple_detect_lyris_response($response->data);
  $vars['@lyris'] = $status;
  $vars['@code'] = $response->code;

  if ($response->code == '200') {
    // Log a flood control event if we have a threshold set
    if (variable_get('lyris_simple_flood_threshold', LYRIS_SIMPLE_FLOOD_DEFAULT_THRESHOLD) > 0) {
      flood_register_event('lyris_simple_subscribe', variable_get('lyris_simple_flood_window', LYRIS_SIMPLE_FLOOD_DEFAULT_WINDOW));
    }

    // Check response from Lyris
    if (in_array($status, array(
      LYRIS_SIMPLE_SUBSCRIBE_ALREADY_SUBSCRIBED,
      LYRIS_SIMPLE_SUBSCRIBE_PENDING,
      LYRIS_SIMPLE_SUBSCRIBE_SUCCESS
    ))) {
      // Set redirect
      if (!empty($redirect)) {
        $form_state['redirect'] = $redirect;
      }

      // Set confirmation message
      $msg = variable_get('lyris_simple_subscribe_confirm', 'none') == 'one'
        ? variable_get('lyris_simple_subscribe_pending_message', LYRIS_SIMPLE_SUBSCRIBE_PENDING_MESSAGE)
        : variable_get('lyris_simple_subscribe_success_message', LYRIS_SIMPLE_SUBSCRIBE_SUCCESS_MESSAGE);

      $msg = filter_xss($msg);
      drupal_set_message(t($msg, $vars));

      // Log to watchdog
      watchdog('lyris_simple', '@email @name subscribed to @list. (Lyris: @lyris)', $vars, WATCHDOG_INFO);
    }
    else {
      $msg = variable_get('lyris_simple_subscribe_failure_message', LYRIS_SIMPLE_SUBSCRIBE_FAILURE_MESSAGE);
      $msg = filter_xss($msg);

      drupal_set_message(t($msg, $vars), 'error');
      watchdog('lyris_simple', 'There was an error subscribing @email @name to @list.  (Lyris: @lyris) - drupal_http_request() returned @code', $vars, WATCHDOG_ERROR);
    }
  }
  else {
    $msg = variable_get('lyris_simple_subscribe_failure_message', LYRIS_SIMPLE_SUBSCRIBE_FAILURE_MESSAGE);
    $msg = filter_xss($msg);

    drupal_set_message(t($msg, $vars), 'error');
    watchdog('lyris_simple', 'There was an error subscribing @email @name to @list.  drupal_http_request() returned @code', $vars, WATCHDOG_ERROR);
  }
}

/**
 * Unsubscribe form.
 * FORM
 */
function lyris_simple_unsubscribe_form($form, &$form_state) {
  $data_fields = array(
    'lists' => variable_get('lyris_simple_list_name'),
    'email_notification' => variable_get('lyris_simple_unsubscribe_email_notify', 0) == 0 ? 'F' : 'T',
    'showconfirm' => 'F',
    'confirm_first' => 'F',
    'appendsubinfotourl' => 'F',
  );

  // If a Lyris list has not been set, let admins know and hide the form.
  if (empty($data_fields['lists'])) {
    if (user_access('administer lyris simple')) {
      $link = l(t('set the Lyris list'), 'admin/config/services/lyris-simple', array('query' => drupal_get_destination()));
      $form['nolist'] = array(
        '#type' => 'markup',
        '#markup' => t('You need to !link for users to unsubscribe from.', array('!link' => $link)),
      );
    }

    return $form;
  }

  // Check for flood control
  $threshold = variable_get('lyris_simple_flood_threshold', LYRIS_SIMPLE_FLOOD_DEFAULT_THRESHOLD);
  $window = variable_get('lyris_simple_flood_window', LYRIS_SIMPLE_FLOOD_DEFAULT_WINDOW);
  if ($threshold > 0 && !user_access('lyris simple bypass flood control') && !flood_is_allowed('lyris_simple_unsubscribe', $threshold, $window)) {
    if ($message = variable_get('lyris_simple_flood_message', array())) {
      $form['flooded'] = array(
        '#type' => 'markup',
        '#markup' => check_markup($message['value'], $message['format']),
      );
    }

    return $form;
  }

  $form['data']['#tree'] = TRUE;

  $form['data']['email'] = array(
    '#type' => 'emailfield',
    '#title' => t('Your Email'),
    '#required' => TRUE,
    '#weight' => 1,
    '#element_validate' => array('elements_validate_email'),
  );

  // Add the user's email if logged in.
  if (user_is_logged_in()) {
    $form['data']['email']['#default_value'] = $GLOBALS['user']->mail;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Unsubscribe'),
    '#weight' => 10,
  );

  // Redirect URL
  $form['redirect'] = array(
    '#type' => 'hidden',
    '#value' => variable_get('lyris_simple_unsubscribe_redirect', ''),
  );

  // Add data fields for lyris
  foreach ($data_fields as $field => $value) {
    $form['data'][$field] = array(
      '#type' => 'value',
      '#value' => $value,
    );
  }

  return $form;
}

/**
 * Unsubscribe form.
 * SUBMIT
 */
function lyris_simple_unsubscribe_form_submit($form, &$form_state) {
  $redirect = $form_state['values']['redirect'];
  $vars = array(
    '@email' => $form_state['values']['data']['email'],
    '@list' => $form_state['values']['data']['lists'],
    '%email' => $form_state['values']['data']['email'],
    '%list' => $form_state['values']['data']['lists'],
  );

  // Talk to Lyris
  $response = lyris_simple_request(LYRIS_SIMPLE_UNSUBSCRIBE, $form_state['values']['data']);
  $status = lyris_simple_detect_lyris_response($response->data);
  $vars['@lyris'] = $status;
  $vars['@code'] = $response->code;

  if ($response->code == '200') {
    // Log a flood control event if we have a threshold set
    if (variable_get('lyris_simple_flood_threshold', LYRIS_SIMPLE_FLOOD_DEFAULT_THRESHOLD) > 0) {
      flood_register_event('lyris_simple_unsubscribe', variable_get('lyris_simple_flood_window', LYRIS_SIMPLE_FLOOD_DEFAULT_WINDOW));
    }

    // Check the returned data from Lyris
    if ($status == LYRIS_SIMPLE_UNSUBSCRIBE_SUCCESS) {
      // Set redirect
      if (!empty($redirect)) {
        $form_state['redirect'] = $redirect;
      }

      // Set confirmation message
      $msg = variable_get('lyris_simple_unsubscribe_success_message', LYRIS_SIMPLE_UNSUBSCRIBE_SUCCESS_MESSAGE);

      $msg = filter_xss($msg);
      drupal_set_message(t($msg, $vars));

      // Log to watchdog
      watchdog('lyris_simple', '@email unsubscribed from @list. (Lyris: @lyris)', $vars, WATCHDOG_INFO);
    }
    else {
      $msg = variable_get('lyris_simple_unsubscribe_failure_message', LYRIS_SIMPLE_UNSUBSCRIBE_FAILURE_MESSAGE);
      $msg = filter_xss($msg);
      drupal_set_message(t($msg, $vars), 'error');
      watchdog('lyris_simple', 'There was an error unsubscribing @email from @list. (Lyris: @lyris) - drupal_http_request() returned @code', $vars, WATCHDOG_ERROR);
    }
  }
  else {
    $msg = variable_get('lyris_simple_unsubscribe_failure_message', LYRIS_SIMPLE_UNSUBSCRIBE_FAILURE_MESSAGE);
    $msg = filter_xss($msg);
    drupal_set_message(t($msg, $vars), 'error');
    watchdog('lyris_simple', 'There was an error unsubscribing @email from @list. drupal_http_request() returned @code', $vars, WATCHDOG_ERROR);
  }
}

/**
 * Validate lyris list name format.
 */
function lyris_simple_element_validate_list_name(&$element, &$form_state) {
  if (!preg_match('/^[a-z0-9\_\-]*$/', $element['#value'])) {
    form_error($element, t('The name %name is not valid. Lyris list names contain only lowercase letters, numbers, underscores and hyphens.', array('%name' => $element['#value'])));
  }
}
