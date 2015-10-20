<?php

function support_nag_admin_overview() {
  _support_nag_admin_clear();
  $rows = array();
  $header = array(
    array('data' => t('Action'), 'field' => 'action'),
    array('data' => t('Clients'), 'field' => 'clients'),
    array('data' => t('States'), 'field' => 'states'),
    array('data' => t('Priorities'), 'field' => 'priorities'),
    array('data' => t('Sent'), 'field' => 'count'),
    array('data' => t('Last sent'), 'field' => 'last'),
    array('data' => t('Operations')),
  );
  $result = db_select('support_nags', 's')
    ->fields('s', array('action', 'clients', 'states', 'priorities', 'last', 'count', 'nagid'))
    ->extend('TableSort')
    ->extend('PagerDefault')
    ->orderByHeader($header)
    ->limit(25)
    ->execute();
  foreach ($result as $nag) {
    switch ($nag->action) {
      case ACTION_NOTIFY:
        $action = t('Notification');
        break;
      case ACTION_CHANGE:
        $action = t('Change state');
        break;
    }
    // build client list
    $clients = explode(',', $nag->clients);
    $display_clients = array();
    foreach ($clients as $client) {
      if (!empty($client)) {
        $display_clients[] .= _support_client($client);
      }
    }
    // build state list
    $states = explode(',', $nag->states);
    $display_states = array();
    foreach ($states as $state) {
      if (!empty($state)) {
        $display_states[] .= _support_state($state);
      }
    }
    // build priority list
    $priorities = explode(',', $nag->priorities);
    $display_priorities = array();
    foreach ($priorities as $priority) {
      if (!empty($priority)) {
        $display_priorities[] .= _support_priorities($priority);
      }
    }
    $rows[] = array(
      $action,
      !empty($display_clients) ? _support_nag_truncate(implode(', ', $display_clients)) : t('all'),
      !empty($display_states) ? _support_nag_truncate(implode(', ', $display_states)) : t('all'),
      !empty($display_priorities) ? _support_nag_truncate(implode(', ', $display_priorities)) : t('all'),
      $nag->count ? t('!count times', array('!count' => number_format($nag->count))) : t('n/a'),
      $nag->last ? t('!time ago', array('!time' => format_interval(time() - $nag->last))) : t('never'),
      l(t('edit'), "admin/support/nag/action/$nag->nagid/edit"),
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Administrative page for defining nag actions.
 */
function support_nag_admin_actions($form, &$form_state, $action = array()) {
  $form = array();
  if (!empty($action)) {
    $selected_clients = explode(',', $action->clients);
    $selected_states = explode(',', $action->states);
    $selected_priorities = explode(',', $action->priorities);
    $uids = explode(',', $action->assigned);
    $users = array();
    foreach ($uids as $uid) {
      $users[] = db_query('SELECT name FROM {users} WHERE uid = :uid', array(':uid' => $uid))->fetchField();
    }
    $assigned = implode(', ', $users);
    $subject = $action->subject;
    $message = $action->message;
    $frequency = $action->frequency;
    $uids = explode(',', $action->mailto);
    $users = array();
    foreach ($uids as $uid) {
      $users[] = db_query('SELECT name FROM {users} WHERE uid = :uid', array(':uid' => $uid))->fetchField();
    }
    $mailto = implode(', ', $users);
    $form['nagid'] = array(
      '#type' => 'hidden',
      '#value' => $action->nagid,
    );
    $action = $action->action;
  }
  else if (isset($_SESSION['support_action'])) {
    $action = $_SESSION['support_action'];
    $selected_clients = $_SESSION['support_clients'];
    $selected_states = $_SESSION['support_states'];
    $selected_priorities = $_SESSION['support_priorities'];
    $assigned = $_SESSION['support_assigned'];
    $subject = NULL;
    $message = NULL;
    $frequency = NULL;
    $mailto = NULL;
  }
  else {
    $action = NULL;
    $selected_clients = NULL;
    $selected_states = NULL;
    $selected_priorities = NULL;
    $subject = NULL;
    $message = NULL;
    $frequency = NULL;
    $mailto = NULL;
  }

  if (isset($action)) {
    switch ($action) {
      case ACTION_NOTIFY:
        $action_text = t('Send notification');
        break;
      case ACTION_CHANGE:
        $action_text = t('Change state');
        break;
    }
    $form['action-status'] = array(
      '#type' => 'markup',
      '#markup' => '<strong>'. t('Action') .':</strong><br />'. $action_text,
    );
    $form['action'] = array(
      '#type' => 'hidden',
      '#value' => $action,
    );
  }
  else {
    $form['action'] = array(
      '#type' => 'radios',
      '#title' => t('Action type'),
      '#options' => array(ACTION_NOTIFY => t('Send notification'), ACTION_CHANGE => t('Change state')),
      '#default_value' => ACTION_NOTIFY,
      '#description' => t('Specify which type of action you are defining.'),
    );
  }
  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
    '#collapsible' => isset($action) ? TRUE : FALSE,
    '#collapsed' => isset($action) ? TRUE : FALSE,
  );
  // Clients
  $clients = support_active_clients();
  if (sizeof($clients) == 1) {
    $form['clients'] = array(
      '#type' => 'hidden',
      '#value' => $clients[0]->clid,
    );
  }
  else {
    foreach ($clients as $client) {
      $options[$client->clid] = $client->name;
    }
    $form['filter']['clients'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => t('Only apply action to selected client(s)'),
      '#options' => $options,
      '#description' => t('Optionally select one or more clients to apply this action to.  If no clients are selected, the action will affect all clients.'),
      '#default_value' => isset($selected_clients) ? $selected_clients : array(),
    );
  }
  $states = _support_states();
  $form['filter']['states'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Only apply action to selected state(s)'),
    '#options' => $states,
    '#description' => t('Optionally select one or more ticket states to apply this action to.  If no states are selected, the action will affect all states.'),
      '#default_value' => isset($selected_states) ? $selected_states : array(),
  );
  $priorities = _support_priorities();
  $form['filter']['priorities'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Only apply action to selected priorities'),
    '#options' => $priorities,
    '#description' => t('Optionally select one or more ticket priorities to apply this action to.  If no priorities are selected, the action will affect all priorities.'),
    '#default_value' => isset($selected_priorities) ? $selected_priorities : array(),
  );
  $form['filter']['assigned'] = array(
    '#type' => 'textfield',
    '#title' => t('Only apply action to assigned user(s)'),
    '#autocomplete_path' => 'support/autocomplete/user',
    '#description' => t('Optionally enter one or more comma separated names of users whom these tickets are assigned to.  If no user is specified, the action will apply to tickets assigned to any user or no user.'),
    '#default_value' => isset($assigned) ? $assigned : '',
  );
  $tokens = array();
  if (isset($action)) {
    switch ($action) {
      case ACTION_NOTIFY:
        $message_description = t('The text of the notification email. Click %link link below for a list of tokens that will be replaced with the correct value when notifications are sent.', array('%link' => t('replacement patterns')));
        $subject_description = t('The subject of the notification email. Click %link link below for a list of tokens that will be replaced with the correct value when notifications are sent.', array('%link' => t('replacement patterns')));
        $default_subject = $subject ? $subject : t("Notification");
        $default_message = $message ? $message : t("This is a regular notification for the following open tickets:\n\n[support-notification:tickets]\n\nPlease contact [site:mail] if you have received this notification email in error.\n\n--\n[site:url]");
        if (module_exists('token')) {
          $tokens = array('#theme' => 'token_tree', '#token_types' => array('support-notification'));
        }
        break;
      case ACTION_CHANGE:
        drupal_set_message('WARNING: Change status actions have not yet been implemented. They can be created, but they do not actually do anything.  Patches welcome.', 'error');
        $message_description = t('The text that is posted to the ticket when changing its state. Click %link link below for a list of tokens that will be replaced with the correct value when tickets are automatically closed.', array('%link' => t('replacement patterns')));
        // TODO: If using comment subjects, then we should make it possible to
        // set the subject.
        $default_message = $message ? $message : t('TODO: Default message text and tokens.');
        if (module_exists('token')) {
          $tokens = array('#theme' => 'token_tree', '#token_types' => array('support-change'));
        }
        break;
    }
    $form['message'] = array(
      '#type' => 'fieldset',
      '#title' => t('Message'),
    );
    if ($action == ACTION_NOTIFY) {
      $form['message']['mailto'] = array(
        '#type' => 'textfield',
        '#title' => t('Recipients'),
        '#autocomplete_path' => 'support/autocomplete/user',
        '#required' => TRUE,
        '#default_value' => $mailto,
        '#description' => t('Enter a comma separated list of users to send the email to.'),
      );
      $form['message']['subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => $default_subject,
        '#description' => $subject_description,
      );
    }
    $form['message']['text'] = array(
      '#type' => 'textarea',
      '#title' => t('Body'),
      '#default_value' => $default_message,
      '#description' => $message_description,
    );
    $form['message']['tokens'] = array(
      '#type' => 'fieldset',
      '#title' => 'Replacement patterns',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['message']['tokens']['list'] = $tokens;
    switch ($action) {
      case ACTION_NOTIFY:
        $form['frequency'] = array(
          '#type' => 'fieldset',
          '#title' => 'Frequency',
        );
        $form['frequency']['when'] = array(
          '#type' => 'select',
          '#title' => t('Send notification every:'),
          '#options' => drupal_map_assoc(array(3600, 10800, 21600, 43200, 64800, 86400, 172800, 259200, 345600, 432000, 518400, 604800, 1209600, 1814400, 2419200, 3024000, 3628800, 4838400, 9676800, 31536000), 'format_interval'),
          '#default_value' => $frequency ? $frequency : 86400,
        );
        break;
    }
    if (isset($message)) {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Update action'),
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Create action'),
      );
    }
    $form['#submit'] = array('support_nag_admin_actions_save');
  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
    );
  }
  $form['cancel'] = array(
    '#type' => 'markup',
    '#value' => l(t('Cancel'), 'admin/support/nag'),
  );
  return $form;
}

function support_nag_admin_actions_save($form, &$form_state) {
  $assigned = array();
  if (!empty($form_state['values']['assigned'])) {
    $users = explode(',', $form_state['values']['assigned']);
    foreach ($users as $username) {
      $uid = db_query('SELECT uid FROM {users} WHERE name = :name', array(':name' => trim($username)))->fetchField();
      if (is_numeric($uid)) {
        $assigned[$uid] = $uid;
      }
    }
  }
  $mailto = array();
  if (!empty($form_state['values']['mailto'])) {
    $to = explode(',', $form_state['values']['mailto']);
    foreach ($to as $username) {
      $uid = db_query('SELECT uid FROM {users} WHERE name = :name', array(':name' => trim($username)))->fetchField();
      if (is_numeric($uid)) {
        $mailto[$uid] = $uid;
      }
    }
  }

  //if ($form_state['clicked_button']['#value'] == t('Update action')) {
  if (!empty($form_state['values']['nagid'])) {
    $query = db_update('support_nags')
      ->condition('nagid', $form_state['values']['nagid']);
  }
  else {
    $query = db_insert('support_nags');
  }

  $query
    ->fields(array(
      'clients' => implode(',', $form_state['values']['clients']),
      'states'  => implode(',', $form_state['values']['states']),
      'priorities' => implode(',', $form_state['values']['priorities']),
      'assigned' => implode(',', $assigned),
      'subject' => $form_state['values']['subject'],
      'message' => $form_state['values']['text'],
      'frequency' => $form_state['values']['when'],
      'mailto' => implode(',', $mailto),
    ))
    ->execute();

  if (!empty($form_state['values']['nagid'])) {
    drupal_set_message('Support nag updated.');
  }
  else {
    drupal_set_message('Support nag created.');
  }
  drupal_goto('admin/support/nag');
}

function _support_nag_admin_clear() {
  unset($_SESSION['support_action']);
  unset($_SESSION['support_clients']);
  unset($_SESSION['support_states']);
  unset($_SESSION['support_priorities']);
  unset($_SESSION['support_assigned']);
}

function support_nag_admin_actions_submit($form, &$form_state) {
  $_SESSION['support_action'] = $form_state['values']['action'];
  $_SESSION['support_clients'] = $form_state['values']['clients'];
  $_SESSION['support_states'] = $form_state['values']['states'];
  $_SESSION['support_priorities'] = $form_state['values']['priorities'];
  $_SESSION['support_assigned'] = $form_state['values']['assigned'];
}

function support_nag_admin_settings() {
  $form = array();
  $form['autoclose'] = array(
    '#type' => 'fieldset',
    '#title' => t('Globally close tickets'),
  );
  $states = _support_states();
  $sids = _support_get_state(SUPPORT_STATE_CLOSED);
  foreach ($sids as $sid) {
    $closed[$sid] = _support_state($sid);
  }
  foreach ($states as $state) {
    if (!in_array($state, $closed)) {
      $form['autoclose']["$state"] = array(
        '#type' => 'select',
        '#title' => t('Close %state tickets after inactive for', array('%state' => $state)),
        '#options' => array('0' => t('never')) + drupal_map_assoc(array(86400, 172800, 259200, 345600, 432000, 518400, 604800, 1209600, 1814400, 2419200, 3024000, 3628800, 4838400, 9676800, 31536000), 'format_interval'),
        '#description' => t('Select an amount of time to wait before auto-closing %state tickets.', array('%state' => $state)),
        '#default_value' => variable_get("support_nag_autoclose_$state", 0),
      );
    }
  }
  $form['autoclose']['close_state'] = array(
    '#type' => 'select',
    '#title' => t('Auto-closed state'),
    '#options' => $closed,
    '#description' => t('Select the state to assign to tickets when they are auto-closed.'),
    '#default_value' => variable_get("support_nag_autoclose_close_state", 0),
  );
  $form['autoclose']['close_message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Auto-close message'),
  );
  $form['autoclose']['close_message']['text'] = array(
    '#type' => 'textarea',
    '#title' => t('Message text'),
    '#default_value' => variable_get('support_nag_autoclose_text', t('This ticket has been automatically marked [support-autoclose:closed-state] as it has been inactive for over [support-autoclose:inactive-limit]. You may re-open the ticket if this is in error or you have further updates.')),
    '#description' => t('The message to post when automatically closing tickets. Click %link link below for a list of tokens that will be replaced with the correct value when tickets are automatically closed.', array('%link' => t('replacement patterns'))),
  );

  $form['autoclose']['close_message']['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => 'Replacement patterns',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  if (module_exists('token')) {
    $form['autoclose']['close_message']['tokens']['list'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('support-autoclose'),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
  );
  return $form;
}

function support_nag_admin_settings_submit($form, &$form_state) {
  $states = _support_states();
  $sids = _support_get_state(SUPPORT_STATE_CLOSED);
  foreach ($sids as $sid) {
    $closed[$sid] = _support_state($sid);
  }
  if ($form_state['values']['op'] == t('Reset to defaults')) {
    foreach ($states as $state) {
      variable_del("support_nag_autoclose_$state");
    }
    variable_del("support_nag_autoclose_close_state");
    variable_del('support_nag_autoclose_text');
    drupal_set_message('The settings have been reset to their default values.');
  }
  else {
    foreach ($states as $state) {
      variable_set("support_nag_autoclose_$state", $form_state['values'][$state]);
    }
    variable_set("support_nag_autoclose_close_state", $form_state['values']['close_state']);
    variable_set('support_nag_autoclose_text', $form_state['values']['text']);
    drupal_set_message('The settings have been saved.');
  }
}

/**
 * Menu callback; retrieve a JSON object containing autocomplete suggestions
 * for existing users.
 */
function support_nag_autocomplete_user($string = '') {
  // The user enters a comma-separated list of users. We only autocomplete the
  // last user.
  $array = drupal_explode_tags($string);

  $last_string = trim(array_pop($array));
  $matches = array();
  if ($last_string != '') {
    $result = db_select('users')
      ->fields('users', array('name'))
      ->condition('name', db_like($last_string) . '%', 'LIKE')
      ->range(0, 10)
      ->execute();
    $prefix = count($array) ? implode(', ', $array) .', ' : '';
    foreach ($result as $user) {
      $matches[$prefix . $user->name] = check_plain($user->name);
    }
  }

  drupal_json_output($matches);
}

