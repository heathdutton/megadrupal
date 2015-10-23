<?php
/**
 * @file
 * FAPI functionality for managing Lyris members.
 */
/**
 * Member optin-out block.
 * FORM
 *
 * Return content for the subscription opt-in block.
 */
function lyris_member_block_list_opt_form($form, &$form_state, $lists) {
  global $user;
  $descriptions = variable_get('lyris_member_list_opt_form_show_descriptions', 0);

  // Sync the user's memberships for all displayed lists.
  $defaults = array();
  $mylists = lyris_member_check_membership($user->mail, array_keys($lists));

  foreach ($mylists as $ListName => $status) {
    $defaults[$ListName] = ($status == 'normal' ? $ListName : 0);
  }

  $form_state['#lists'] = $lists;
  $form_state['#original_values'] = $defaults;
  $form['data']['#tree'] = TRUE;

  foreach ($lists as $ListName => $list) {
    $form['data'][$ListName] = array(
      '#type' => 'checkbox',
      '#title' => $list->getDisplayName(),
      '#return_value' => $ListName,
      '#default_value' => (isset($defaults[$ListName]) ? $defaults[$ListName] : 0),
    );
    if ($descriptions) {
      $form['data'][$ListName]['#description'] = $list->localVal('ShortDescription');
    }
  }

  $form['mail'] = array('#type' => 'value', '#value' => $user->mail);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update Subscriptions'),
  );

  return $form;
}

/**
 * User full subscription page form.
 * FORM
 */
function lyris_member_list_opt_form($form, &$form_state, $lists, $account) {
  $ops = array();

  if (empty($lists)) {
    $form['empty'] = array(
      '#type' => 'markup',
      '#markup' => t('There are no available mailing lists.'),
    );
    return $form;
  }

  // Sync the user's memberships for all displayed lists.
  $defaults = array();
  $mylists = lyris_member_check_membership($account->mail, array_keys($ops));
  foreach ($mylists as $ListName => $status) {
    $defaults[$ListName] = ($status == 'normal' ? $ListName : 0);
  }

  // Reuse the submit handler from the block form.
  $form['#submit'] = array('lyris_member_block_list_opt_form_submit');

  $form_state['#lists'] = $lists;
  $form_state['#original_values'] = $defaults;

  $form['mail'] = array('#type' => 'value', '#value' => $account->mail);
  $form['data']['#tree'] = TRUE;

  foreach ($lists as $ListName => $list) {
    $form['data'][$ListName] = array(
      '#type' => 'checkbox',
      '#title' => $list->getDisplayName(),
      '#return_value' => $ListName,
      '#description' => $list->localVal('ShortDescription'),
      '#default_value' => $defaults[$ListName],
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update Subscriptions'),
  );

  return $form;
}

/**
 * Member optin-out block.
 * SUBMIT
 */
function lyris_member_block_list_opt_form_submit($form, &$form_state) {
  $vals = $form_state['values'];

  $old_vals = $form_state['#original_values'];
  $new_vals = $vals['data'];

  // Create arrays of the lists that this member should be subscribed to or
  // unsubscribed from.
  $diff['sub'] = $diff['unsub'] = array();
  foreach ($new_vals as $list => $status) {
    if ($status !== 0 && (!isset($old_vals[$list]) || $old_vals[$list] !== $status)) {
      $diff['sub'][] = $list;
    }
    elseif ($status === 0 && isset($old_vals[$list]) && $old_vals[$list] !== $status) {
      $diff['unsub'][] = $list;
    }
  }

  // Process the requests.
  foreach ($diff['sub'] as $ListName) {
    $args = array('%title' => $form_state['#lists'][$ListName]->title);
    if (lyris_member_subscribe($vals['mail'], $ListName)) {
      drupal_set_message(t('You have been subscribed to %title', $args));
    }
    else {
      drupal_set_message(t('There was an error subscribing you to %title', $args), 'error');
    }
  }
  foreach ($diff['unsub'] as $ListName) {
    $args = array('%title' => $form_state['#lists'][$ListName]->title);
    if (lyris_member_unsubscribe($vals['mail'], $ListName)) {
      drupal_set_message(t('You have been unsubscribed from %title', $args));
    }
    else {
      drupal_set_message(t('There was an error unsubscribing you from %title', $args), 'error');
    }
  }
}

/**
 * Bulk subscribe members to a list.
 * FORM
 */
function lyris_member_bulk_subscribe_form($form, &$form_state, $ListName) {
  $form_state['#ListName'] = $ListName;

  $form['subscribe'] = array(
    '#type' => 'textarea',
    '#title' => t('Bulk Subscribe'),
    '#description' => t('Enter e-mail addresses to be subscribed to this list.  Separate e-mails from real names with a comma.') . ' <strong>' . t('One member per line.') . '</strong><br />' .
                      t('If the member already exists and has unsubscribed, they will NOT be re-subscribed to the list.') . '<br />' .
                      t('You may enter up to 200 at a time.'),
    '#rows' => 10,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Subscribe Members'),
  );

  return $form;
}

/**
 * Bulk subscribe members to a list.
 * VALIDATE
 */
function lyris_member_bulk_subscribe_form_validate($form, &$form_state) {
  $val = $form_state['values']['subscribe'];

  // Validate email addresses
  $members = explode(PHP_EOL, trim($val));
  $invalid = array();
  foreach ($members as $member) {
    list($email, $name) = explode(',', $member);

    if (!valid_email_address(trim($email))) {
      $invalid[] = $email;
    }
  }

  if ($invalid) {
    form_set_error('subscribe', t('The following e-mail addresses are invalid: !emails', array('!emails' => implode(', ', $invalid))));
  }

  // Validate the number of emails passed in.
  if (count($members) > 200) {
    form_set_error('subscribe', t('You may only subscribe up to 200 members at a time.'));
  }
}

/**
 * Bulk subscribe members to a list.
 * SUBMIT
 */
function lyris_member_bulk_subscribe_form_submit($form, &$form_state) {
  $members = explode(PHP_EOL, trim($form_state['values']['subscribe']));
  $ListName = $form_state['#ListName'];

  // Use 'Create Many Members' call to add the emails directly to Lyris.
  $struct = array();
  foreach ($members as $member) {
    list($email, $name) = explode(',', $member);
    $struct[] = array('EmailAddress' => trim($email), 'FullName' => ($name ? trim($name) : LyrisMember::namifyEmail(trim($email))));
  }

  $lyris = lyris();
  $cnt = $lyris->createManyMembers($struct, $ListName);

  drupal_set_message(t('Subscribed %cnt new members.', array('%cnt' => (int)$cnt)));
}

/**
 * Filter the detailed member list.
 * FORM
 */
function lyris_member_detailed_member_list_filter_form($form, &$form_state) {
  lyris_include('structs', 'lyris_member');
  $status = array('all' => '-- ' . t('All') . ' --') + lyris_member_enum('MemberStatusEnum');

  $form['#method'] = 'GET';

  $form['MemberStatus'] = array(
    '#type' => 'select',
    '#title' => t('Member Status'),
    '#options' => $status,
    '#default_value' => (isset($_GET['MemberStatus']) ? $_GET['MemberStatus'] : ''),
  );

  $form['limit'] = array(
    '#type' => 'select',
    '#title' => t('Show Results'),
    '#options' => array(
      3  => 3,
      25 => 25,
      50 => 50,
      100 => 100,
      200 => 200,
      500 => 500,
      1000 => 1000,
      0 => t('All'),
    ),
    '#default_value' => (isset($_GET['limit']) && is_numeric($_GET['limit']) ? $_GET['limit'] : 50),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply Filter'),
  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function lyris_member_form_lyris_member_detailed_member_list_filter_form_alter(&$form, &$form_state) {
  // Clear hidden fields out of the url params
  unset($form['form_token'], $form['form_build_id'], $form['form_id']);
}

/**
 * Detailed member list to handle multiple subscribe/unsubscribes.
 * FORM
 */
function lyris_member_detailed_member_list_form($form, &$form_state, $list, $members, $locals = array()) {
  lyris_include('structs', 'lyris_member');
  $status  = lyris_member_enum('MemberStatusEnum');

  $form_state['#list'] = $list;

  $header = array(
    'count'  => array('class' => array('count'), 'data' => '&nbsp;'),
    'name'   => t('Name'),
    'mail'   => t('E-mail'),
    'local'  => t('Local Status'),
    'lyris'  => t('Lyris Status'),
    'status' => array('class' => array('status-icon'), 'data' => t('In Sync')),
  );

  // Build results table
  $rows = $member_data = array();
  $i = 1;
  foreach ($members as $member) {
    $local_status = (isset($member['local_status']) ? $member['local_status'] : NULL);
    $lyris_status = (isset($member['remote_status']) ? $member['remote_status'] : NULL);

    // Build the table row
    $row = array(
      'count'  => $i,
      'name'   => $member['FullName'],
      'mail'   => $member['EmailAddress'],
      'local'  => ($local_status ? $status[$local_status] : 'NA'),
      'lyris'  => ($lyris_status ? $status[$lyris_status] : 'NA'),
      'status' => ($local_status && ($lyris_status != $local_status) ? theme('image', array('path' => 'misc/message-24-warning.png', 'alt' => t('Not In-Sync'))) : theme('image', array('path' => 'misc/message-24-ok.png', 'alt' => t('In-Sync')))),
      '#attributes' => array('class' => array($lyris_status)),
    );

    // Compile member data for comparison in the submit handler
    $member_data[$member['EmailAddress']] = array(
      'local_status' => $local_status,
      'lyris_status' => $lyris_status,
      'MemberID' => $member['MemberID'],
    );

    $rows[$member['EmailAddress']] = $row;
    $i++;
  }

  $form['members'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('This list has no members.'),
    '#attached' => array('css' => array(LYRIS_MOD_PATH . '/lyris.css')),
    '#attributes' => array('id' => 'lyris-members'),
  );
  $form_state['#member_data'] = $member_data;

  // Set some actions
  $form['update_status'] = array(
    '#type' => 'select',
    '#title' => t('Update Members\' Statuses'),
    '#options' => $status,
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#weight' => 10,
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Update'),
    ),
  );

  return $form;
}

/**
 * Detailed member list to handle multiple subscribe/unsubscribes.
 * SUBMIT
 */
function lyris_member_detailed_member_list_form_submit($form, &$form_state) {
  $vals        = $form_state['values'];
  $member_data = $form_state['#member_data'];
  $ListName    = $form_state['#list']->localVal('ListName');
  $new_status  = $vals['update_status'];

  /**
   * Possible scenarios and resulting actions:
   * 1. The member is in-sync and their status is changing.
   *    a. Update the membership status.
   *
   * 2. The member is in-sync and their status is not changing.
   *    a. Skip this record.
   *
   * 3. The member is not in-sync and their remote status is changing.
   *    a. If their status is changing to match their local status, attempt to
   *       push the record straight to Lyris.
   *    b. If their status is changing to something other than their local record,
   *      update the local record to the new status.
   *
   * 4. The member is not in-sync and their remote status is not changing.
   *    a. If the new status differs from the current local status, mark the
   *      local record as synced without pushing data to lyris.
   *    b. If the new status is the same as the current local status, skip this
   *      record.
   *
   * 5. If there is no local member and the remote status is changing.
   *    a. Push directly to Lyris.
   */
  foreach ($vals['members'] as $mail) {
    if ($mail === 0) {
      continue;
    }

    $has_local = $member_data[$mail]['local_status'];
    $has_lyris = $member_data[$mail]['lyris_status'];
    $insync = ($member_data[$mail]['local_status'] == $member_data[$mail]['lyris_status']);

    // Scenario 1a
    if ($has_local && $insync && $new_status != $member_data[$mail]['lyris_status']) {
      lyris_member_update_status($mail, $ListName, $new_status);
    }
    // 2a, skip record
    // Scenario 3a
    elseif ($has_local && !$insync && ($new_status != $member_data[$mail]['lyris_status']) && ($new_status == $member_data[$mail]['local_status'])) {
      lyris_member_update_status($mail, $ListName, $new_status);
    }
    // Scenario 3b
    elseif ($has_local && !$insync && ($new_status != $member_data[$mail]['lyris_status']) && ($new_status != $member_data[$mail]['local_status'])) {
      lyris_member_update_status($mail, $ListName, $new_status);
    }
    // Scenario 4a
    elseif ($has_local && !$insync && ($new_status == $member_data[$mail]['lyris_status']) && ($new_status != $member_data[$mail]['local_status'])) {
      lyris_member_update_status($mail, $ListName, $new_status);
      lyris_member_set_field_synced($mail, $ListName, 'MemberStatus');
    }
    // 4b, skip record
    // Scenario 5a
    elseif (!$has_local && ($new_status != $member_data[$mail]['lyris_status'])) {
      $lyris = lyris();
      $sms = array(
        'MemberID' => $member_data[$mail]['MemberID'],
        'EmailAddress' => $mail,
        'ListName' => $ListName,
      );
      $lyris->updateMemberStatus($sms, $new_status);
    }
  }

  drupal_set_message(t('Member statuses updated.'));
}
