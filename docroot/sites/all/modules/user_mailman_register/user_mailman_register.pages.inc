<?php
/**
 * @file
 * User pages for User Mailman Register.
 */

/**
 * Menu callback for subscription form page.
 *
 * Shows user edit page (depending on settings) or anonymous subscription form.
 */
function user_mailman_register_page() {
  global $user;
  if ($user->uid > 0) {
    if (variable_get('user_mailman_register_ownform', 1)) {
      $path = 'user/' . $user->uid . '/user_mailman_register';
    }
    else {
      $path = 'user/' . $user->uid . '/edit';
    }
    drupal_goto($path);
  }
  else {
    return drupal_get_form('user_mailman_register_form', $user, TRUE);
  }
}


/**
 * Form callback for user subscription form.
 *
 * @param $account
 *   the account we're trying to subscribe. NULL for anonymous.
 * @param $show_submit
 *   TRUE = display this as a standalone form with submission button, vs a subform.
 *
 * @ingroup forms
 * @see user_mailman_register_form_validate()
 * @see user_mailman_register_form_submit()
 */
function user_mailman_register_form($form, &$form_state, $account = NULL, $show_submit = TRUE) {
  if ($show_submit) {
    if (isset($account)) {
      if ($account->uid > 0) {
        drupal_set_title(t('Mailing Lists for %name', array('%name' => $account->name)), PASS_THROUGH);
      }
      else {
        drupal_set_title(t('Mailing Lists'), PASS_THROUGH);
      }
    }
  }
  $form['user_mailman_register']['lists'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mailing Lists'),
    '#description' => t('Join a mailing list to receive or participate in its discussions via email.'),
    '#collapsible' => FALSE,
    '#attributes' => array('id' => array('user_mailman_register'), 'class' => array('umr_list_fieldset')),
  );
  $lists = _user_mailman_register_get_lists();
  if (empty($lists)) {
    $form['user_mailman_register']['no_lists'] = array(
      '#type' => 'markup',
      '#prefix' => '<p>',
      '#markup' => t('There are no mailing lists.'),
      '#suffix' => '</p>',
    );
    return $form;
  }
  $lists_available = FALSE;
  foreach ($lists as $list) {
    if (!_user_mailman_register_allowed_list($list, $account)) {
      continue;
    }
    $list_status = USER_MAILMAN_REGISTER_UNSUBSCRIBED;
    $lists_available = TRUE;
    $options = array();
    $tlid = trim($list['lid']);
    $list_id = 'list' . $tlid;
    $form['user_mailman_register']['lists'][$list_id] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($list['name']),
      '#description' => check_markup($list['description']),
      '#collapsible' => TRUE,
      '#attributes' => array('class' => array('umr_list_' . $tlid)),
    );

    if (isset($account) && $account->uid > 0) {
      $subscription = _user_mailman_register_get_remote_status($account, $list);
      if (isset($subscription['lmessage'])) {
        $form['user_mailman_register']['lists'][$list_id]['listmsg'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="messages error">',
          '#markup' => check_plain($subscription['lmessage']),
          '#suffix' => '</div>',
        );
      }
      $list_status = $subscription['lstatus'];
    }
    if ($list_status == USER_MAILMAN_REGISTER_UNSUBSCRIBED) {
      $title = t('Subscribe to this list?');
      if ($list['allow_unsubscribe']) {
        $options[USER_MAILMAN_REGISTER_UNSUBSCRIBED] = t('No');
      }
      $options[USER_MAILMAN_REGISTER_SUBSCRIBED_NORMAL] = t('Yes');
      // Feeds specified default value into the form if allowed
      if ($list['defaultnewreg'] || !$list['allow_unsubscribe']) {
        $list_status = USER_MAILMAN_REGISTER_SUBSCRIBED_NORMAL;
      }
    }
    else {
      $title = t('Subscription settings');
      if ($list['allow_unsubscribe']) {
        $options[USER_MAILMAN_REGISTER_DO_UNSUBSCRIBE] = t('Unsubscribe');
      }
      if ($list['allow_temp_disable'] || $list_status == USER_MAILMAN_REGISTER_SUBSCRIBED_DISABLED) {
        $options[USER_MAILMAN_REGISTER_SUBSCRIBED_DISABLED] = _user_mailman_register_t_status(USER_MAILMAN_REGISTER_SUBSCRIBED_DISABLED);
      }
      if ($list['allow_digest'] || $list_status == USER_MAILMAN_REGISTER_SUBSCRIBED_DIGEST) {
        $options[USER_MAILMAN_REGISTER_SUBSCRIBED_DIGEST] = _user_mailman_register_t_status(USER_MAILMAN_REGISTER_SUBSCRIBED_DIGEST);
      }
      $options[USER_MAILMAN_REGISTER_SUBSCRIBED_NORMAL] =  _user_mailman_register_t_status(USER_MAILMAN_REGISTER_SUBSCRIBED_NORMAL);
      // Display the moderation checkbox
      if (user_access('administer user_mailman_register')) {
        $form['user_mailman_register']['lists'][$list_id]['mod' . $tlid] = array(
          '#type' => 'checkbox',
          '#title' => t('Moderate'),
          '#default_value' => $subscription['lmod'],
          '#description' => t('If checked, member postings will be moderated.'),
          '#weight' => 1,
        );
      }
    }

    $form['user_mailman_register']['lists'][$list_id]['options' . $tlid] = array(
      '#type' => 'radios',
      '#title' => $title,
      '#options' => $options,
      '#default_value' => $list_status,
    );
  }

  if (!$lists_available) {
    $form['user_mailman_register']['lists']['#description'] = t('No mailing lists are available.');
    $form['user_mailman_register']['lists']['#attributes']['class'] = 'umr_list_nolists';
  }
  elseif ($show_submit) {
    if (!$account || $account->uid === 0) {
      // Anonymous users can subscribe but need to enter a mailing address.
      $form['user_mailman_register']['lists']['reqaddress'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#size' => 15,
        '#weight' => -1,
        '#required' => FALSE,
      );
    }
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save')
    );
  }

  $form['mailman_account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  return $form;
}


/**
 * Validation callback for user subscription form.
 */
function user_mailman_register_form_validate($form, &$form_state) {
  // Validation of email address for anonymous user subscription request
  if (isset($form['user_mailman_register']['lists']['reqaddress'])) {
    $req_address = $form_state['values']['reqaddress'];
    if (!valid_email_address($req_address))  {
      form_set_error('reqaddress', t('"%reqaddress" is not a valid email address',
        array('%reqaddress' => $req_address)));
    }
  }
  // Validation if at least one subscription is required
  global $user;
  if (variable_get('user_mailman_register_required', 0) && !user_access('administer user_mailman_register')) {
    $no_lists = TRUE;
    $lists = _user_mailman_register_get_lists();
    if (!empty($lists)) {
      foreach ($lists as $list) {
        $edit_list = $form['options' . trim($list['lid'])];
        // Check all allowed lists except for those with the unsubscribe option disabled
        if (isset($edit_list) && _user_mailman_register_allowed_list($list, $account) && $list['allow_unsubscribe']) {
          $no_lists = FALSE;
          if ($edit_list > USER_MAILMAN_REGISTER_UNSUBSCRIBED) {
            return TRUE;
          }
        }
      }
    }
    if (!$no_lists) {
      form_set_error('user_mailman_register', t('At least one mailing list subscription is required'));
    }
    return $no_lists;
  }
}


/**
 * Submission callback for user subscription form.
 */
function user_mailman_register_form_submit($form, &$form_state) {
  $account = $form_state['values']['mailman_account'];
  $user_mail = FALSE;
  if (!$account || !is_numeric($account->uid)) {
    return;
  }
  if (isset($form_state['values']['reqaddress'])) {
    $user_mail = $form_state['values']['reqaddress'];
  }
  _user_mailman_register_subscribe($account, $form_state['values'], FALSE, $user_mail);
}

