<?php

/**
 * @file
 * WHM UI administrative forms and pages.
 */

/**
 * Definition for the WHM list accounts form.
 */
function whm_ui_list_accounts_form() {
  $user_can_suspend = user_access('suspend whm accounts');
  $user_can_terminate = user_access('terminate whm accounts');
  $user_can_edit = user_access('edit whm accounts');
  $header = array(
    t('User/Account Name'),
    t('Domain Name'),
    t('Plan Type'),
    t('Status'),
  );
  if ($user_can_suspend || $user_can_terminate || $user_can_edit) {
    $header[] = t('Operations');
  }
  $rows = array();
  $accts = whm_list_accounts();
  if (!empty($accts)) {
    foreach ($accts as $acct) {
      if ($acct['suspended'] == 1) {
        $status = t('Suspended');
      }
      else {
        $status = t('Active');
      }
      $row = array(
        $acct['user'],
        $acct['domain'],
        _whm_theme_plan_name($acct['plan']),
        $status,
      );
      if ($user_can_suspend) {
        if ($acct['suspended']) {
          $operations = l(t('Unsuspend'), "admin/whm/accounts/account/{$acct['user']}/unsuspend");
        }
        else {
          $operations = l(t('Suspend'), "admin/whm/accounts/account/{$acct['user']}/suspend");
        }
      }
      if ($user_can_terminate) {
        $operations .= ' ' . l(t('Terminate'), "admin/whm/accounts/account/{$acct['user']}/terminate");
      }
      if ($user_can_edit) {
        $operations .= ' ' . l(t('Edit'), "admin/whm/accounts/account/{$acct['user']}/edit");
      }
      if ($user_can_suspend || $user_can_terminate || $user_can_edit) {
        $row[] = $operations;
      }
      $rows[] = $row;
    }
  }
  $output = theme_table(array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(),
    // Table header will be sticky.
    'sticky' => TRUE,
    'caption' => "",
    'colgroups' => array(),
    // The message to be displayed if table is empty.
    'empty' => t('No accounts found. Check WHM settings.'),
    )
  );
  return $output;
}

/**
 * Definition for the WHM view account form.
 */
function whm_ui_view_account_form($account) {
  print $account;
}

/**
 * Definition for the WHM account form.
 */
function _whm_ui_account_form(&$form_state = NULL) {

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('WHM Account Settings'),
  );

  $form['account']['user'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Enter desired username for the account.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['user'],
    '#required' => TRUE,
    '#size' => 16,
  );

  $form['account']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact e-mail'),
    '#description' => t('Enter contact e-mail address for the account.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['email'],
    '#required' => TRUE,
    '#size' => 24,
  );

  $form['account']['pwd1'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Enter the password for the account.'),
    '#default_value' => '',
    '#required' => TRUE,
    '#size' => 16,
  );

  $form['account']['pwd2'] = array(
    '#type' => 'password',
    '#title' => t('Confirm password'),
    '#description' => t('Re-enter the password for the account.'),
    '#default_value' => '',
    '#required' => TRUE,
    '#size' => 16,
  );

  $form['account']['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain name'),
    '#description' => t('Enter the domain name for the account.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['domain'],
    '#required' => TRUE,
    '#size' => 24,
  );

  $plan_array = array();
  foreach (whm_list_plans() as $plan) {
    $plan_array[$plan['name']] = _whm_theme_plan_name($plan['name']);
  }

  $form['account']['plan'] = array(
    '#type' => 'radios',
    '#title' => t('Plan type'),
    '#description' => t('Select the hosting plan type for the account.'),
    '#options' => $plan_array,
    '#default_value' => empty($form_state) ? '' : $form_state['values']['plan'],
    '#required' => TRUE,
  );

  $form['account']['submit'] = array(
    '#type' => 'submit',
    '#value' => empty($form_state) ? t('Create account') : t('Update account'),
    '#weight' => 100,
  );
  return $form;
}

/**
 * Definition for the WHM Create Account form.
 */
function whm_ui_create_account_form() {
  return _whm_ui_account_form();
}

/**
 * Validation for the create account form.
 */
function whm_ui_create_account_form_validate($form_id, &$form_values) {
  if (empty($form_values['values']['user'])) {
    form_set_error('user', t('Please enter a username for the new account.'));
  }
  else {
    $user = $form_values['values']['user'];
    $res = whm_check_username($user);
    if ($res == WHMConstant::WHM_USERNAME_TAKEN) {
      form_set_error('user', t('This username is already taken. Please choose again.'));
    }
    elseif ($res == WHMConstant::WHM_USERNAME_INVALID) {
      form_set_error('user', t('This username is invalid. Please choose a username that contains only letters and numbers.'));
    }
    elseif ($res == WHMConstant::WHM_USERNAME_TOOLONG) {
      form_set_error('user', t('This username is too long. Please choose a username with %len or fewer characters.', array('%len' => WHMConstant::WHM_MAX_USERNAME_LEN)));
    }
  }

  if (empty($form_values['values']['email'])) {
    form_set_error('email', t('Please enter a contact e-mail address for the new account.'));
  }
  $email = $form_values['values']['email'];
  if (whm_check_email($email) !== WHMConstant::WHM_SUCCESS) {
    form_set_error('email', t('This e-mail address is invalid. Please check it and try again.'));
  }

  if (empty($form_values['values']['pwd1'])) {
    form_set_error('pwd1', t('Please enter a password for the new account.'));
  }
  if (empty($form_values['values']['pwd2'])) {
    form_set_error('pwd2', t('Please re-enter the password for the new account.'));
  }
  if ($form_values['values']['pwd1'] !== $form_values['values']['pwd2']) {
    form_set_error('pwd2', t('The passwords do not match.'));
  }
  $pwd = $form_values['values']['pwd1'];
  if (whm_check_password($pwd) !== WHMConstant::WHM_SUCCESS) {
    form_set_error('pwd1', t('This password is invalid. Please enter a password of at least six characters in length.'));
  }

  if (empty($form_values['values']['domain'])) {
    form_set_error('domain', t('Please enter a domain name for this account.'));
  }
  else {
    $domain = $form_values['values']['domain'];
    $res = whm_check_domain($domain);
    if ($res == WHMConstant::WHM_DOMAIN_TAKEN) {
      form_set_error('domain', t('This domain name is already registered with this server.'));
    }
    elseif ($res !== WHMConstant::WHM_SUCCESS) {
      form_set_error('domain', t('This domain name is invalid.'));
    }
  }

  if (empty($form_values['values']['plan'])) {
    form_set_error('plan', t('Please select a hosting plan for this account.'));
  }
  else {
    $plans = whm_list_plans();
    $planid = $form_values['values']['plan'];
    $plan_exists = FALSE;
    foreach ($plans as $plan) {
      if ($plan['name'] == $planid) {
        $plan_exists = TRUE;
        break;
      }
    }
    if (!$plan_exists) {
      form_set_error('plan', t('Please select an existing hosting plan for this account. Alternatively <a href="!uri" title="Create a Plan">Create a Plan</a>', array('!uri' => 'admin/whm/plans/create')));
    }
  }
}

/**
 * Submit processing for the create account form.
 */
function whm_ui_create_account_form_submit($form_id, &$form_values) {
  $user = $form_values['values']['user'];
  $email = $form_values['values']['email'];
  $pwd = $form_values['values']['pwd1'];
  $domain = $form_values['values']['domain'];
  $plan = $form_values['values']['plan'];

  $rv = whm_create_account($user, $domain, array(
    'contactemail' => $email,
    'pass' => $pwd,
    'plan' => $plan,
  ));
  if ($rv === WHMConstant::WHM_SUCCESS) {
    drupal_set_message(t('Successfully created new account:<br />Username: <i>%user</i><br />Domain: <i>%domain</i><br />Plan type: <i>%plan</i><br />',
      array(
        '%user' => $user,
        '%domain' => $domain,
        '%plan' => $plan,
      )
    ));
    $form_values['redirect'] = 'admin/whm/accounts/list';
    return;
  }
  else {
    $error = t('An error occured while trying to create the new account. Please check the specified values and try again.') . '<br />' . t('The WHM server responded with: !rv', array('!rv' => $rv));
    form_set_error('account', $error);
  }
}

/**
 * Definition for the WHM Edit Account form.
 */
function whm_ui_edit_account_form($form, &$form_state, $account) {

  $res = whm_account_info($account);
  $account_data = $res[0];
  $current_state = array(
    'values' => array(
      'user' => $account_data['user'],
      'domain' => $account_data['domain'],
      'email' => $account_data['email'],
      'plan' => $account_data['plan'],
    ),
  );
  $form_state['values'] = array_merge($current_state['values'], $form_state['input']);

  $form = _whm_ui_account_form($form_state);
  $form['account']['password'] = array(
    '#type' => 'fieldset',
    '#title' => t('Change Password'),
    '#description' => t("Enter values here <b>ONLY</b> if you want to change this account's password."),
    '#collapsible' => TRUE,
    '#collapsed' => empty($form_state['values']['pwd1']) && empty($form_state['values']['pwd2']),
  );
  $form['account']['password']['pwd1'] = $form['account']['pwd1'];
  $form['account']['password']['pwd1']['#required'] = FALSE;
  $form['account']['password']['pwd2'] = $form['account']['pwd2'];
  $form['account']['password']['pwd2']['#required'] = FALSE;
  unset($form['account']['pwd1']);
  unset($form['account']['pwd2']);
  unset($form['account']['email']);
  unset($form['account']['plan']);

  $form['current_user'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['user'],
    '#required' => TRUE,
  );
  $form['current_domain'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['domain'],
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Validation for the edit account form.
 */
function whm_ui_edit_account_form_validate($form, &$form_state) {
  if (empty($form_state['values']['user'])) {
    form_set_error('user', t('Please enter a username for the new account.'));
  }
  elseif ($form_state['values']['user'] != $form_state['values']['current_user']) {
    $res = whm_check_username($form_state['values']['user']);
    if ($res == WHMConstant::WHM_USERNAME_TAKEN) {
      form_set_error('user', t('This username is already taken. Please choose again.'));
    }
    elseif ($res == WHMConstant::WHM_USERNAME_INVALID) {
      form_set_error('user', t('This username is invalid. Please choose a username that contains only letters and numbers.'));
    }
    elseif ($res == WHMConstant::WHM_USERNAME_TOOLONG) {
      form_set_error('user', t('This username is too long. Please choose a username with %len or fewer characters.', array('%len' => WHMConstant::WHM_MAX_USERNAME_LEN)));
    }
    elseif ($res !== WHMConstant::WHM_SUCCESS) {
      form_set_error('user', t('There was a problem with this username.'));
    }
  }

  if (!empty($form_state['values']['pwd1']) || !empty($form_state['values']['pwd2'])) {
    if (empty($form_state['values']['pwd1'])) {
      form_set_error('pwd1', t('Please enter a valid password for the account.'));
    }
    if (empty($form_state['values']['pwd2'])) {
      form_set_error('pwd2', t('Please re-enter the password for the account.'));
    }
    if ($form_state['values']['pwd1'] !== $form_state['values']['pwd2']) {
      form_set_error('pwd2', t('The passwords do not match.'));
    }
    $pwd = $form_state['values']['pwd1'];
    if (whm_check_password($pwd) !== WHMConstant::WHM_SUCCESS) {
      form_set_error('pwd1', t('This password is invalid. Please enter a password of at least six characters in length.'));
    }
  }

  if (empty($form_state['values']['domain'])) {
    form_set_error('domain', t('Please enter a domain name for this account.'));
  }
  elseif ($form_state['values']['domain'] != $form_state['values']['current_domain']) {
    $res = whm_check_domain($form_state['values']['domain']);
    if ($res == WHMConstant::WHM_DOMAIN_TAKEN) {
      form_set_error('domain', t('This domain name is already registered with this server.'));
    }
    elseif ($res !== WHMConstant::WHM_SUCCESS) {
      form_set_error('domain', t('This domain name is invalid.'));
    }
  }
}

/**
 * Submit processing for the edit account form.
 */
function whm_ui_edit_account_form_submit($form, &$form_state) {
  $new_user = $form_state['values']['user'];
  $current_user = $form_state['values']['current_user'];
  $new_domain = $form_state['values']['domain'];
  $current_domain = $form_state['values']['current_domain'];
  $new_pass = $form_state['values']['pwd1'];

  $changed = array();
  if ($new_user != $current_user) {
    $changed['newuser'] = $new_user;
  }
  if ($new_domain != $current_domain) {
    $changed['domain'] = $new_domain;
  }

  if (!empty($new_pass)) {
    $rv = whm_change_password($current_user, $new_pass);
    if ($rv === WHMConstant::WHM_PASSWORD_INVALID) {
      form_set_error('pwd1', t('The password %pass was invalid. Please try again with a valid password.', array('%pass' => $new_pass)));
      return;
    }
    elseif ($rv !== WHMConstant::WHM_SUCCESS) {
      $error = t('An error occured while trying to update the password. Please check the specified values and try again.') . '<br />' . t('The WHM server responded with: !rv', array('!rv' => $rv));
      form_set_error('pwd1', $error);
      return;
    }
    elseif ($rv === WHMConstant::WHM_SUCCESS) {
      drupal_set_message(t('Successfully updated password.'));
    }
  }

  if (count($changed) == 0) {
    if (empty($new_pass)) {
      drupal_set_message(t('Account %user was not updated. No updated values were provided.', array('%user' => $current_user)));
    }
    $form_state['redirect'] = 'admin/whm/accounts/list';
    return;
  }

  $rv = whm_modify_account($current_user, $changed);
  if ($rv === WHMConstant::WHM_SUCCESS) {
    drupal_set_message(t('Successfully updated account:<br />Username: <i>%user</i><br />Domain: <i>%domain</i>', array('%user' => $new_user, '%domain' => $new_domain)));
    $form_state['redirect'] = 'admin/whm/accounts/list';
    return;
  }
  elseif ($rv === WHMConstant::WHM_USERNAME_TAKEN) {
    form_set_error('account', t('The username %user was already taken. Please try again with another username.', array('%user' => $new_user)));
  }
  else {
    $error = t('An error occured while trying to update the account. Please check the specified values and try again.') . '<br /> ' . t('The WHM server responded with: !rv', array('!rv' => $rv));
    form_set_error('account', $error);
  }
}

/**
 * Definition for the account suspension form.
 */
function whm_ui_confirm_suspend_account($form, &$form_state, $account) {
  $form['_whm_account'] = array('#type' => 'value', '#value' => $account);
  return confirm_form(
    $form,
    t('Are you sure you want to suspend the WHM account !name?',
      array('!name' => $account)
    ),
    'admin/whm/accounts/list',
    t('This will suspend the account on the WHM server.'),
    t('Suspend'),
    t('Cancel')
  );
}

/**
 * Submit function for the confirm form for user deletion.
 */
function whm_ui_confirm_suspend_account_submit($form, &$form_state) {
  whm_suspend_account($form_state['values']['_whm_account']);
  drupal_set_message(t('WHM account %name has been suspended.', array('%name' => $form_state['values']['_whm_account'])));

  if (!isset($_REQUEST['destination'])) {
    $form_state['redirect'] = 'admin/whm/accounts/list';
  }
}

/**
 * Definition for the account unsuspension form.
 */
function whm_ui_confirm_unsuspend_account($form, &$form_state, $account) {
  $form['_whm_account'] = array('#type' => 'value', '#value' => $account);
  return confirm_form(
    $form,
    t('Are you sure you want to unsuspend the WHM account %name?',
      array('%name' => $account)
    ),
    'admin/whm/accounts/list',
    t('This will unsuspend the account on the WHM server.'),
    t('Unsuspend'),
    t('Cancel')
  );
}

/**
 * Submit function for the confirm form for user deletion.
 */
function whm_ui_confirm_unsuspend_account_submit($form, &$form_state) {
  whm_unsuspend_account($form_state['values']['_whm_account']);
  drupal_set_message(t('WHM account %name has been unsuspended.',
    array(
      '%name' => $form_state['values']['_whm_account'],
    )
  ));

  if (!isset($_REQUEST['destination'])) {
    $form_state['redirect'] = 'admin/whm/accounts/list';
  }
}

/**
 * Definition for the account termination form.
 */
function whm_ui_confirm_terminate_account($form, &$form_state, $account) {
  $form['_whm_account'] = array('#type' => 'value', '#value' => $account);
  return confirm_form(
    $form,
    t('Are you sure you want to terminate the WHM account %name?',
      array('%name' => $account)
    ),
    'admin/whm/accounts/list',
    t('This action is not reversible.'),
    t('Terminate'),
    t('Cancel')
  );
}

/**
 * Submit function for the confirm form for account termination.
 */
function whm_ui_confirm_terminate_account_submit($form, &$form_state) {
  whm_terminate_account($form_state['values']['_whm_account']);
  drupal_set_message(t('WHM account %name has been terminated.', array('%name' => $form_state['values']['_whm_account'])));

  if (!isset($_REQUEST['destination'])) {
    $form_state['redirect'] = 'admin/whm/accounts/list';
  }
}

/**
 * Form builder; Return form for user administration filters.
 *
 * @ingroup forms
 */
function whm_ui_filter_form() {
  $session = &$_SESSION['whm_overview_filter'];
  $session = is_array($session) ? $session : array();
  $filters = whm_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only accounts where'),
    '#theme' => 'whm_filters',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    // Merge an array of arrays into one if necessary.
    $options = $type == 'permission' ? call_user_func_array('array_merge', $filters[$type]['options']) : $filters[$type]['options'];
    $params = array('%property' => $filters[$type]['title'] , '%value' => $options[$value]);
    if ($i++ > 0) {
      $form['filters']['current'][] = array('#value' => t('<em>and</em> where <strong>%property</strong> is <strong>%value</strong>', $params));
    }
    else {
      $form['filters']['current'][] = array('#value' => t('<strong>%property</strong> is <strong>%value</strong>', $params));
    }
  }

  foreach ($filters as $key => $filter) {
    $names[$key] = $filter['title'];
    $form['filters']['status'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
    );
  }

  $form['filters']['filter'] = array(
    '#type' => 'radios',
    '#options' => $names,
  );
  $form['filters']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => (count($session) ? t('Refine') : t('Filter')),
  );
  if (count($session)) {
    $form['filters']['buttons']['undo'] = array(
      '#type' => 'submit',
      '#value' => t('Undo'),
    );
    $form['filters']['buttons']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }

  drupal_add_js('misc/form.js', 'core');

  return $form;
}

/**
 * Definition for the WHM List Plans form.
 */
function whm_ui_list_plans_form() {
  $user_can_delete = user_access('delete whm plan');
  $user_can_edit = user_access('edit whm plan');
  $header = array(
    t('Plan Name'),
  );
  if ($user_can_delete || $user_can_edit) {
    $header[] = t('Operations');
  }
  $plans = whm_list_plans();
  $rows = array();
  if (!empty($plans)) {
    foreach ($plans as $plan) {
      $row = array(
        _whm_theme_plan_name($plan['name']),
      );
      if ($user_can_delete) {
        $operations = l(t('Delete'), "admin/whm/plans/plan/{$plan['name']}/delete");
      }
      if ($user_can_edit) {
        $operations .= ' ' . l(t('Edit'), "admin/whm/plans/plan/{$plan['name']}/edit");
      }
      if ($user_can_delete || $user_can_edit) {
        $row[] = $operations;
      }
      $rows[] = $row;
    }
  }
  $output = theme_table(array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(),
    // Table header will be sticky.
    'sticky' => TRUE,
    'caption' => "",
    'colgroups' => array(),
    // The message to be displayed if table is empty.
    'empty' => t('No plans found. Check WHM settings.'),
    )
  );
  return $output;
}

/**
 * Definition for the WHM Plan form.
 */
function _whm_ui_plan_form(&$form_state = NULL) {
  $form['plan']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Enter the desired name for the new plan.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['name'],
    '#required' => TRUE,
    '#size' => 16,
  );
  $form['plan']['resources'] = array(
    '#type' => 'fieldset',
    '#title' => t('Resources'),
  );
  $form['plan']['resources']['quota'] = array(
    '#type' => 'textfield',
    '#title' => t('Quota'),
    '#description' => t('Disk space quota for the package (in Megabytes). Maximum: unlimited'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['quota'],
    '#size' => '12',
  );
  $form['plan']['resources']['bwlimit'] = array(
    '#type' => 'textfield',
    '#title' => t('Bandwidth Limit'),
    '#description' => t('The amount of bandwidth the account can use in a month. Maximum: unlimited'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['bwlimit'],
    '#size' => '12',
  );
  $form['plan']['resources']['maxftp'] = array(
    '#type' => 'textfield',
    '#title' => t('Max FTP Accounts'),
    '#description' => t('The number of FTP accounts a user assigned to the package can create. Maximum: 999'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['maxftp'],
    '#size' => '12',
  );
  $form['plan']['resources']['maxpop'] = array(
    '#type' => 'textfield',
    '#title' => t('Max POP Accounts'),
    '#description' => t('The number of email accounts a user assigned to the package can create. Maximum: 999'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['maxpop'],
    '#size' => '12',
  );
  $form['plan']['resources']['maxlists'] = array(
    '#type' => 'textfield',
    '#title' => t('Max EMail Lists'),
    '#description' => t('The number of email lists a user assigned to the package can create. Maximum: 999'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['maxlists'],
    '#size' => '12',
  );
  $form['plan']['resources']['maxsql'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Databases'),
    '#description' => t('The number of SQL databases a user assigned to the package can create. Maximum: 999'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['maxsql'],
    '#size' => '12',
  );
  $form['plan']['resources']['maxsub'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Sub Domains'),
    '#description' => t('The number of subdomains a user assigned to the package can create. Maximum: 999'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['maxsub'],
    '#size' => '12',
  );
  $form['plan']['resources']['maxpark'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Parked Domains'),
    '#description' => t('The number of parked domains a user assigned to the package can create. Maximum: 999'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['maxpark'],
    '#size' => '12',
  );
  $form['plan']['resources']['maxaddon'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Addon Domains'),
    '#description' => t('The number of addon domains a user assigned to the package can create. Maximum: 999'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['maxaddon'],
    '#size' => '12',
  );
  $form['plan']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  $form['plan']['settings']['ip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Dedicated IP Address'),
    '#description' => t('Whether or not the account has a dedicated IP address.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['ip'],
  );
  $form['plan']['settings']['hasshell'] = array(
    '#type' => 'checkbox',
    '#title' => t('Shell Access'),
    '#description' => t('Whether or not the account has shell access.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['hasshell'],
  );
  $form['plan']['settings']['frontpage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Frontpage Extensions'),
    '#description' => t('Whether or not the account can install FrontPage extensions.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['frontpage'],
  );
  $form['plan']['settings']['cgi'] = array(
    '#type' => 'checkbox',
    '#title' => t('CGI Access'),
    '#description' => t('Whether or not the account has CGI access.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['cgi'],
  );
  $form['plan']['settings']['cpmod'] = array(
    '#type' => 'select',
    '#title' => t('cPanel Theme'),
    '#description' => t('The default theme for the package.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['cpmod'],
    '#options' => array(
      '' => t('TODO - Get themes'),
    ),
  );
  $form['plan']['settings']['featurelist'] = array(
    '#type' => 'select',
    '#title' => t('Feature List'),
    '#description' => t('Name of the feature list to be used for the package.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['featurelist'],
    '#options' => array(
      '' => t('TODO - Get features'),
    ),
  );
  $form['plan']['settings']['language'] = array(
    '#type' => 'select',
    '#title' => t('Locale'),
    '#description' => t('Name of the language to be used for this package by default.'),
    '#default_value' => empty($form_state) ? '' : $form_state['values']['language'],
    '#options' => array(
      '' => t('TODO - Get languages'),
    ),
  );
  $form['plan']['submit'] = array(
    '#type' => 'submit',
    '#value' => empty($form_state) ? t('Create plan') : t('Edit plan'),
  );
  return $form;
}

/**
 * Definition for the WHM Create Plan form.
 */
function whm_ui_create_plan_form() {
  return _whm_ui_plan_form();
}

/**
 * Validation for the create plan form.
 */
function whm_ui_create_plan_form_validate($form_id, &$form_values) {
  $whmuser = variable_get('whm_username', '');
  $form_values['values']['name'] = $whmuser . '_' . $form_values['values']['name'];
}

/**
 * Submit processing for the create plan form.
 */
function whm_ui_create_plan_form_submit($form_id, $form_values) {
  $options = $form_values['values'];
  foreach ($options as $key => $value) {
    if (empty($value)) {
      unset($options[$key]);
    }
  }
  _whm_ui_unset_form_api($options);
  $rv = whm_create_plan($form_values['values']['name'], $options);
  if ($rv === WHMConstant::WHM_SUCCESS) {
    drupal_set_message(t('Successfully created plan %plan',
      array(
        '%plan' => $form_values['values']['name'],
      )
    ));
    return 'admin/whm/plans/list';
  }
  else {
    $error = t('An error occured while trying to create the new account. Please check the specified values and try again.') . '<br />' . t('The WHM server responded with: !rv', array('!rv' => $rv));
    form_set_error('plan', $error);
  }
}

/**
 * Definition for the WHM Edit Plan form.
 */
function whm_ui_edit_plan_form($form, &$form_state, $plan) {
  $res = whm_plan_info($plan);
  $plan_data = $res;
  if ($plan_data['IP'] == 'y') {
    $ip = 1;
  }
  else {
    $ip = 0;
  }
  if ($plan_data['HASSHELL'] == 'y') {
    $hasshell = 1;
  }
  else {
    $hasshell = 0;
  }
  if ($plan_data['FRONTPAGE'] == 'y') {
    $frontpage = 1;
  }
  else {
    $frontpage = 0;
  }
  if ($plan_data['CGI'] == 'y') {
    $cgi = 1;
  }
  else {
    $cgi = 0;
  }
  $current_state = array(
    'values' => array(
      'name' => $plan_data['name'],
      'quota' => $plan_data['QUOTA'],
      'bwlimit' => $plan_data['BWLIMIT'],
      'maxftp' => $plan_data['MAXFTP'],
      'maxpop' => $plan_data['MAXPOP'],
      'maxlists' => $plan_data['MAXLST'],
      'maxsql' => $plan_data['MAXSQL'],
      'maxsub' => $plan_data['MAXSUB'],
      'maxpark' => $plan_data['MAXPARK'],
      'maxaddon' => $plan_data['MAXADDON'],
      'ip' => $ip,
      'hasshell' => $hasshell,
      'frontpage' => $frontpage,
      'cgi' => $cgi,
      'cpmod' => $plan_data['CPMOD'],
      'featurelist' => $plan_data['FEATURELIST'],
      'language' => $plan_data['LANG'],
    ),
  );
  $form_state['values'] = array_merge($current_state['values'], $form_state['input']);

  $form = _whm_ui_plan_form($form_state);

  $form['current_name'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['name'],
    '#required' => TRUE,
  );
  $form['current_quota'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['quota'],
    '#required' => TRUE,
  );
  $form['current_bwlimit'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['bwlimit'],
    '#required' => TRUE,
  );
  $form['current_maxftp'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['maxftp'],
    '#required' => TRUE,
  );
  $form['current_maxpop'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['maxpop'],
    '#required' => TRUE,
  );
  $form['current_maxlists'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['maxlists'],
    '#required' => TRUE,
  );
  $form['current_maxsql'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['maxsql'],
    '#required' => TRUE,
  );
  $form['current_maxsub'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['maxsub'],
    '#required' => TRUE,
  );
  $form['current_maxpark'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['maxpark'],
    '#required' => TRUE,
  );
  $form['current_maxaddon'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['maxaddon'],
    '#required' => TRUE,
  );
  $form['current_ip'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['ip'],
    '#required' => TRUE,
  );
  $form['current_hasshell'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['hasshell'],
    '#required' => TRUE,
  );
  $form['current_frontpage'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['frontpage'],
    '#required' => TRUE,
  );
  $form['current_cgi'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['cgi'],
    '#required' => TRUE,
  );
  $form['current_cpmod'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['cpmod'],
    '#required' => TRUE,
  );
  $form['current_featurelist'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['featurelist'],
    '#required' => TRUE,
  );
  $form['current_language'] = array(
    '#type' => 'hidden',
    '#default_value' => $current_state['values']['language'],
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Validation for the edit plan form.
 */
function whm_ui_edit_plan_form_validate($form_id, &$form_values) {
  $whmuser = variable_get('whm_username', '');
  $form_values['values']['name'] = $whmuser . '_' . $form_values['values']['name'];
}

/**
 * Submit processing for the edit plan form.
 */
function whm_ui_edit_plan_form_submit($form, &$form_state) {
  $new_name = $form_state['values']['name'];
  $current_name = $form_state['values']['current_name'];
  $new_quota = $form_state['values']['quota'];
  $current_quota = $form_state['values']['current_quota'];
  $new_bwlimit = $form_state['values']['bwlimit'];
  $current_bwlimit = $form_state['values']['current_bwlimit'];
  $new_maxftp = $form_state['values']['maxftp'];
  $current_maxftp = $form_state['values']['current_maxftp'];
  $new_maxpop = $form_state['values']['maxpop'];
  $current_maxpop = $form_state['values']['current_maxpop'];
  $new_maxlists = $form_state['values']['maxlists'];
  $current_maxlists = $form_state['values']['maxlists'];
  $new_maxsql = $form_state['values']['maxsql'];
  $current_maxsql = $form_state['values']['current_maxsql'];
  $new_maxsub = $form_state['values']['maxsub'];
  $current_maxsub = $form_state['values']['current_maxsub'];
  $new_maxpark = $form_state['values']['maxpark'];
  $current_maxpark = $form_state['values']['current_maxpark'];
  $new_maxaddon = $form_state['values']['maxaddon'];
  $current_maxaddon = $form_state['values']['current_maxaddon'];
  $new_ip = $form_state['values']['ip'];
  $current_ip = $form_state['values']['current_ip'];
  $new_hasshell = $form_state['values']['hasshell'];
  $current_hasshell = $form_state['values']['current_hasshell'];
  $new_frontpage = $form_state['values']['frontpage'];
  $current_frontpage = $form_state['values']['current_frontpage'];
  $new_cgi = $form_state['values']['cgi'];
  $current_cgi = $form_state['values']['current_cgi'];
  $new_cpmod = $form_state['values']['cpmod'];
  $current_cpmod = $form_state['values']['current_cpmod'];
  $new_featurelist = $form_state['values']['featurelist'];
  $current_featurelist = $form_state['values']['current_featurelist'];
  $new_language = $form_state['values']['language'];
  $current_language = $form_state['values']['current_language'];

  $changed = array();
  if ($new_name != $current_name) {
    $changed['name'] = $new_name;
  }
  if ($new_quota != $current_quota) {
    $changed['quota'] = $new_quota;
  }
  if ($new_bwlimit != $current_bwlimit) {
    $changed['bwlimit'] = $new_bwlimit;
  }
  if ($new_maxftp != $current_maxftp) {
    $changed['maxftp'] = $new_ftp;
  }
  if ($new_maxpop != $current_maxpop) {
    $changed['maxpop'] = $new_maxpop;
  }
  if ($new_maxlists != $current_maxlists) {
    $changed['maxlists'] = $new_maxlists;
  }
  if ($new_maxsql != $current_maxsql) {
    $changed['maxsql'] = $new_maxsql;
  }
  if ($new_maxsub != $current_maxsub) {
    $changed['maxsub'] = $new_maxsub;
  }
  if ($new_maxpark != $current_maxpark) {
    $changed['maxpark'] = $new_maxpark;
  }
  if ($new_maxaddon != $current_maxaddon) {
    $changed['maxaddon'] = $new_maxaddon;
  }
  if ($new_ip != $current_ip) {
    $changed['ip'] = $new_ip;
  }
  if ($new_hasshell != $current_hasshell) {
    $changed['hasshell'] = $new_hasshell;
  }
  if ($new_frontpage != $current_frontpage) {
    $changed['frontpage'] = $new_frontpage;
  }
  if ($new_cgi != $current_cgi) {
    $changed['cgi'] = $new_cgi;
  }
  if ($new_cpmod != $current_cpmod) {
    $changed['cpmod'] = $new_cpmod;
  }
  if ($new_featurelist != $current_featurelist) {
    $changed['featurelist'] = $new_featurelist;
  }
  if ($new_language != $current_language) {
    $changed['language'] = $new_language;
  }

  if (count($changed) == 0) {
    drupal_set_message(t('Plan %plan was not updated. No updated values were provided.',
      array(
        '%plan' => $current_name,
      )
    ));
    $form_state['redirect'] = 'admin/whm/plans/list';
    return;
  }

  $rv = whm_modify_plan($current_name, $changed);
  if ($rv === WHMConstant::WHM_SUCCESS) {
    drupal_set_message(t('Successfully updated plan: <i>%plan</i>', array('%plan' => $new_name)));
    $form_state['redirect'] = 'admin/whm/plans/list';
    return;
  }
  else {
    $error = t('An error occured while trying to update the account. Please check the specified values and try again.') . '<br />' . t('The WHM server responded with: !rv', array('!rv' => $rv));
    form_set_error('plan', $error);
  }
}

/**
 * Definition for the WHM Plan Deletion form.
 */
function whm_ui_confirm_delete_plan($form, &$form_state, $plan_name) {
  $form['_whm_plan'] = array('#type' => 'value', '#value' => $plan_name);
  return confirm_form(
    $form,
    t('Are you sure you want to delete the plan %name?', array(
      '%name' => _whm_theme_plan_name($plan_name),
    )),
    'admin/whm/plans/list',
    t('This will delete the selected plan/package from the WHM server and cannot be undone, be certain you are absolutely sure!'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit function for the confirm form for user deletion.
 */
function whm_ui_confirm_delete_plan_submit($form, &$form_state) {
  whm_delete_plan($form_state['values']['_whm_plan']);
  drupal_set_message(t('Plan %name has been deleted.', array(
    '%name' => _whm_theme_plan_name($form_state['values']['_whm_plan']))
  ));

  if (!isset($_REQUEST['destination'])) {
    $form_state['redirect'] = 'admin/whm/plans/list';
  }
}

/**
 * Strips default elements so values can be passed to API without modification.
 */
function _whm_ui_unset_form_api(&$form_values) {
  unset($form_values['op']);
  unset($form_values['submit']);
  unset($form_values['form_build_id']);
  unset($form_values['form_token']);
  unset($form_values['form_id']);
}
