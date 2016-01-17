<?php

module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.functions');
/**
 * @file
 * Helper functions file to handle users functionality.
 */

/**
 * @param $form
 * @param $account
 * @param $token
 *
 * Manage identities for the specified user.
 */
function socialloginandsocialshare_user_identities_submit($form, $account, $token) {
  $obj = new LoginRadius();
  $user_title = format_username($account);
  drupal_set_title(check_plain($user_title));

  if (user_is_logged_in() && $obj->loginradius_is_valid_guid($token)) {
    $userprofile = $obj->loginradius_get_user_profiledata($token);

    if (isset($userprofile->ID) && $userprofile->ID != '' && user_is_logged_in()) {
      $lrdata = socialloginandsocialshare_getuser_data($userprofile);
      $authname_exist = db_query("SELECT authname FROM {authmap} am INNER JOIN {socialloginandsocialshare_mapusers} sm ON am.aid = sm.aid WHERE authname = :lrid", array(
        ':lrid' => $lrdata['id'],
          ))->fetchField();
      $result = db_query("SELECT provider FROM {authmap} am INNER JOIN {socialloginandsocialshare_mapusers} sm ON am.aid = sm.aid WHERE module = :module AND uid = :uid", array(
        ':module' => 'socialloginandsocialshare',
        ':uid' => $account->uid
      ));
      $mapped = '';

      foreach ($result as $identity) {
        if ($identity->provider == $lrdata['provider']) {
          $mapped = 'yes';
        }
      }

      if (empty($authname_exist) && !$authname_exist) {
        if ($mapped != 'yes') {
          db_delete('authmap')
              ->condition('authname', $lrdata['id'])
              ->execute();
          db_insert('authmap')
              ->fields(array(
                'uid' => $account->uid,
                'authname' => $lrdata['id'],
                'module' => 'socialloginandsocialshare',
              ))
              ->execute();

          if (variable_get('socialloginandsocialshare_update_profile') == 1) {
            if (variable_get('user_pictures') == 1 && !empty($lrdata['thumbnail'])) {
              socialloginandsocialshare_insert_picture($lrdata, $account);
            }
          }

          $aid = db_query('SELECT aid FROM {authmap} WHERE authname = :id', array('id' => $lrdata['id']))->fetchField();
          socialloginandsocialshare_insert_into_mapping_table($aid, $lrdata['provider'], $lrdata['id']);
          drupal_set_message(t("Your account successfully mapped with this account."));
        }
        else {
          drupal_set_message(t("This social network is already linked with an account. Kindly unmap the current social network before linking new social network."), 'warning');
        }
      }
      else {
        drupal_set_message(t("This account is already linked with an account. try to choose another account."), 'warning');
      }
    }

    drupal_redirect_form($form, $redirect = NULL);
  }
}

/**
 * Function getting social user profile data.
 *
 * @param array $userprofile
 *   An array containing all userprofile data keys:
 *
 * @return array
 */
function socialloginandsocialshare_getuser_data($userprofile) {
  $lrdata['fullname'] = (!empty($userprofile->FullName) ? trim($userprofile->FullName) : '');
  $lrdata['profilename'] = (!empty($userprofile->ProfileName) ? trim($userprofile->ProfileName) : '');
  $lrdata['nickname'] = (!empty($userprofile->NickName) ? trim($userprofile->NickName) : '');
  $lrdata['fname'] = (!empty($userprofile->FirstName) ? trim($userprofile->FirstName) : '');
  $lrdata['lname'] = (!empty($userprofile->LastName) ? trim($userprofile->LastName) : '');
  $lrdata['id'] = (!empty($userprofile->ID) ? $userprofile->ID : '');
  $lrdata['provider'] = (!empty($userprofile->Provider) ? $userprofile->Provider : '');
  $lrdata['email'] = (sizeof($userprofile->Email) > 0 ? $userprofile->Email[0]->Value : '');
  $lrdata['thumbnail'] = (!empty($userprofile->ImageUrl) ? trim($userprofile->ImageUrl) : '');

  if (empty($lrdata['thumbnail']) && $lrdata['provider'] == 'facebook') {
    $lrdata['thumbnail'] = socialloginandsocialshare_get_facebook_social_picture($lrdata['id']);
  }

  $lrdata['dob'] = (!empty($userprofile->BirthDate) ? $userprofile->BirthDate : '');
  $lrdata['gender'] = (!empty($userprofile->Gender) ? $userprofile->Gender : '');
  $lrdata['company'] = (!empty($userprofile->Positions[1]->Company->Name) ? $userprofile->Positions[1]->Company->Name : '');

  if (empty($lrdata['company'])) {
    $lrdata['company'] = (!empty($userprofile->Industry) ? $userprofile->Industry : '');
  }

  $lrdata['address'] = (!empty($userprofile->Addresses) ? $userprofile->MainAddress : '');

  if (empty($lrdata['address'])) {
    $lrdata['address'] = (!empty($userprofile->HomeTown) ? $userprofile->HomeTown : '');
  }

  $lrdata['aboutme'] = (!empty($userprofile->About) ? $userprofile->About : '');
  $lrdata['website'] = (!empty($userprofile->ProfileUrl) ? $userprofile->ProfileUrl : '');
  $lrdata['state'] = (!empty($userprofile->State) ? $userprofile->State : '');
  $lrdata['city'] = (!empty($userprofile->City) ? $userprofile->City : '');

  if (empty($lrdata['city']) || $lrdata['city'] == 'unknown') {
    $lrdata['city'] = (!empty($userprofile->LocalCity) && $userprofile->LocalCity != 'unknown' ? $userprofile->LocalCity : '');
  }

  $lrdata['country'] = (!empty($userprofile->Country) ? $userprofile->Country : '');

  if (empty($lrdata['country'])) {
    $lrdata['country'] = (!empty($userprofile->LocalCountry) ? $userprofile->LocalCountry : '');
  }

  $lrdata['positions'] = "";

  if (!empty($userprofile->Positions)) {
    for ($i = 0; $i < sizeof($userprofile->Positions); $i++) {
      $lrdata['positions'] .= $userprofile->Positions[$i]->Position . ",";
    }
  }

  $lrdata['educations'] = "";

  if (!empty($userprofile->Educations)) {
    for ($i = 0; $i < sizeof($userprofile->Educations); $i++) {
      $lrdata['educations'] .= ($i + 1) . ". " . $userprofile->Educations[$i]->School . " ";
    }
  }

  $lrdata['languages'] = "";

  if (!empty($userprofile->Languages)) {
    for ($i = 0; $i < sizeof($userprofile->Languages); $i++) {
      $lrdata['languages'] .= $userprofile->Languages[$i]->Name . ",";
    }
  }

  $lrdata['favoritethings'] = "";

  if (!empty($userprofile->FavoriteThings)) {
    for ($i = 0; $i < sizeof($userprofile->FavoriteThings); $i++) {
      $lrdata['favoritethings'] .= $userprofile->FavoriteThings[$i]->Name . ",";
    }
  }

  return $lrdata;
}

/**
 * @param $form
 * @param $account
 * @param string $variable_path
 *
 * Function that redircts user after login/registration.
 */
function socialloginandsocialshare_user_redirect($form, $account, $variable_path = '') {
  $variable_path = (!empty($variable_path) ? $variable_path : 'socialloginandsocialshare_userlogin_redirect');
  $variable_custom_path = (($variable_path == 'socialloginandsocialshare_userlogin_redirect') ? 'socialloginandsocialshare_custom_redirection' : 'socialloginandsocialshare_custom_register_redirection');
  $request_uri = request_uri();

  if (variable_get('sociallogin_user_register_redirect') == 1) {
    variable_del('sociallogin_user_register_redirect');
    $variable_path = 'socialloginandsocialshare_userregister_redirect';
  }

  if (strpos($request_uri, 'user/register') && $variable_path != 'socialloginandsocialshare_userregister_redirect') {
    $variable_path = 'socialloginandsocialshare_userregister_redirect';
    socialloginandsocialshare_user_redirect($form, $account, $variable_path);
  }
  else {
    if (variable_get($variable_path) == 1) {
      unset($_GET['destination']);
      // Redirect to profile.
      drupal_goto('user/' . $account->uid . '/edit');
    }
    elseif (variable_get($variable_path) == 2) {
      // Redirect to custom page.
      $custom_url = variable_get($variable_custom_path);

      if (!empty($custom_url)) {
        unset($_GET['destination']);
        drupal_goto($custom_url);
      }
      else {
        drupal_redirect_form($form, $redirect = NULL);
      }
    }
    else {
      // Redirect to same page.
      drupal_redirect_form($form, $redirect = NULL);
    }
  }
}

/**
 * @param $lrdata
 * @return mixed
 *
 * Get the username on the basis of avaialablity of data.
 */
function socialloginandsocialshare_get_username($lrdata) {
  if (!empty($lrdata['fullname'])) {
    $username = $lrdata['fullname'];
  }
  elseif (!empty($lrdata['profilename'])) {
    $username = $lrdata['profilename'];
  }
  elseif (!empty($lrdata['nickname'])) {
    $username = $lrdata['nickname'];
  }
  elseif (!empty($lrdata['email'])) {
    $user_name = explode('@', $lrdata['email']);
    $username = $user_name[0];
  }
  else {
    $username = $lrdata['id'];
  }

  return $username;
}

/**
 * @param $lrdata
 * @return mixed
 *
 * Function that check username exist or not.
 */
function socialloginandsocialshare_check_exist_username($lrdata) {
  $user_name = socialloginandsocialshare_username_option($lrdata);
  // Look for user with username match.
  $index = 0;

  while (TRUE) {
    if (user_load_by_name($user_name)) {
      $index++;
      $user_name = $user_name . $index;
    }
    else {
      break;
    }
  }

  $data['username'] = socialloginandsocialshare_remove_unescaped_char($user_name);
  $data['fname'] = $lrdata['fname'];
  $data['lname'] = $lrdata['lname'];

  if (empty($data['fname'])) {
    $data['fname'] = socialloginandsocialshare_get_username($lrdata);
  }

  if (empty($data['lname'])) {
    $data['lname'] = socialloginandsocialshare_get_username($lrdata);
  }

  return $data;
}

/**
 * @param $lrdata
 * @return mixed
 *
 * Generate username as per admin settings save into database.
 */
function socialloginandsocialshare_username_option($lrdata) {
  if (!empty($lrdata['fname']) && !empty($lrdata['lname']) && variable_get('socialloginandsocialshare_display_username') != 2) {
    if (variable_get('socialloginandsocialshare_display_username') == 1) {
      $username = $lrdata['fname'] . '-' . $lrdata['lname'];
    }
    else {
      $username = $lrdata['fname'] . ' ' . $lrdata['lname'];
    }
  }
  elseif (variable_get('socialloginandsocialshare_display_username') == 2 && !empty($lrdata['email'])) {
    $username = $lrdata['email'];
  }
  else {
    $username = socialloginandsocialshare_get_username($lrdata);
  }

  return $username;
}

/**
 * @param $form
 * @param $form_state
 * @param $account
 * @param int $lrid
 * @return mixed
 *
 * Delete the identities.
 */
function socialloginandsocialshare_user_delete_form($form, $form_state, $account, $lrid = 0) {
  $provider = db_query("SELECT provider FROM {authmap} am INNER JOIN {socialloginandsocialshare_mapusers} sm ON am.aid = sm.aid WHERE uid = :uid AND am.authname = :authname AND module = 'socialloginandsocialshare'", array(
    ':uid' => $account->uid,
    ':authname' => $lrid,
      ))
      ->fetchField();
  return confirm_form(array(), t('Are you sure you want to delete the Social Login %provider for %user?', array(
    '%provider' => $provider,
    '%user' => $account->name
      )), 'user/' . $account->uid . '/edit');
}

/**
 * @param $form
 * @param $form_state
 *
 * submit form to delete identity from the system.
 */
function socialloginandsocialshare_user_delete_form_submit($form, &$form_state) {
  $provider = db_query("SELECT provider FROM {authmap} am INNER JOIN {socialloginandsocialshare_mapusers} sm ON am.aid = sm.aid WHERE uid = :uid AND am.authname = :authname AND module = 'socialloginandsocialshare'", array(
    ':uid' => $form_state['build_info']['args'][0]->uid,
    ':authname' => $form_state['build_info']['args'][1],
  ))
    ->fetchField();
  $query = db_delete('authmap')
    ->condition('uid', $form_state['build_info']['args'][0]->uid)
    ->condition('authname', $form_state['build_info']['args'][1])
    ->condition('module', 'socialloginandsocialshare')
    ->execute();
  $query2 = db_delete('socialloginandsocialshare_mapusers')
    ->condition('provider_id', $form_state['build_info']['args'][1])
    ->execute();

  if ($query && $query2) {
    drupal_set_message(t('Your social login identity for %provider successfully deleted.', array('%provider' => $provider)));
  }
  else {
    drupal_set_message(t('We were unable to delete the linked account.'), 'error');
  }

  $form_state['redirect'] = 'user/' . $form_state['build_info']['args'][0]->uid . '/edit';
}

/**
 * @param $account
 * @param $lrdata
 *
 * Update picture and insert mapping data into authmap and loginradius mapping table.
 */
function socialloginandsocialshare_update_mapping_data($account, $lrdata) {
  $authname_exist = db_query("SELECT authname FROM {authmap} am INNER JOIN {socialloginandsocialshare_mapusers} sm ON am.aid = sm.aid WHERE am.authname = :authname", array(
    ':authname' => $lrdata['id'],
      ))
      ->fetchField();

  if (empty($authname_exist) && !$authname_exist) {
    db_delete('authmap')
        ->condition('authname', $lrdata['id'])
        ->execute();
    db_insert('authmap')
        ->fields(array(
          'uid' => $account->uid,
          'authname' => $lrdata['id'],
          'module' => 'socialloginandsocialshare',
        ))
        ->execute();

    if (variable_get('socialloginandsocialshare_update_profile') == 1) {
      if (variable_get('user_pictures') == 1 && !empty($lrdata['thumbnail'])) {
        socialloginandsocialshare_insert_picture($lrdata, $account);
      }
    }

    $aid = db_query('SELECT aid FROM {authmap} WHERE authname = :id', array('id' => $lrdata['id']))->fetchField();
    db_delete('socialloginandsocialshare_mapusers')
        ->condition('provider_id', $lrdata['id'])
        ->execute();

    db_insert('socialloginandsocialshare_mapusers')
        ->fields(array(
          'aid' => $aid,
          'provider' => $lrdata['provider'],
          'provider_id' => $lrdata['id'],
        ))
        ->execute();
  }
}

/**
 * @param $account
 * @param $lrdata
 * @param $form
 * @param $form_state
 *
 * provide login to exisitng user and update the data.
 */
function socialloginandsocialshare_existing_user_save($account, $lrdata, $form, &$form_state) {
  if (variable_get('socialloginandsocialshare_update_profile') == 1) {
    module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.field');
    $data = array();
    $lrdata['gender'] = !empty($lrdata['gender']) ? socialloginandsocialshare_get_gender($lrdata['gender']) : '';
    socialloginandsocialshare_field_create_user($data, $lrdata);

    if (module_exists('profile2')) {
      socialloginandsocialshare_field_create_profile_user($account, $lrdata, $form, $form_state);
    }
    $account = user_save($account, $data);

    if (variable_get('user_pictures') == 1 && !empty($lrdata['thumbnail'])) {
      socialloginandsocialshare_insert_picture($lrdata, $account);
    }
  }

  $form_state['uid'] = $account->uid;
  user_login_submit(array(), $form_state);
}

/**
 * @param $lrdata
 * @param $form
 * @param $form_state
 *
 * Check exisitng user through email/ID into database and add user.
 */
function socialloginandsocialshare_check_existing_user($lrdata, $form, &$form_state) {
  if ((empty($lrdata['email']) && variable_get('socialloginandsocialshare_email_required') == 0) || !empty($lrdata['email'])) {
    if (empty($lrdata['email']) && variable_get('socialloginandsocialshare_email_required') == 0) {
      $lrdata['email'] = socialloginandsocialshare_get_email_by_provider_id($lrdata['email'], $lrdata['id'], $lrdata['provider']);
    }

    $account = user_external_load($lrdata['id']);

    if (!$account) {
      $account = user_load_by_mail($lrdata['email']);
      if (!$account) {
        socialloginandsocialshare_enable_force_registration($lrdata);
      }
    }

    socialloginandsocialshare_adduser($lrdata, $form, $form_state);
  }
}

/**
 * @param $account
 * @param $lrdata
 * @param $form
 * @param $form_state
 * 
 * Provide login to unblock user
 */
function socialloginandsocialshare_provide_login_to_unblock_user($account, $lrdata, $form, &$form_state) {

  if ($account->login) {
    // Check if user is blocked.
    $authname_exist = db_query("SELECT am.aid FROM {authmap} am INNER JOIN {socialloginandsocialshare_mapusers} sm ON am.aid = sm.aid WHERE am.authname = :authname", array(
      ':authname' => $lrdata['id'],
        ))
        ->fetchField();

    if (empty($authname_exist) && !$authname_exist) {
      try {
        $aid = db_query('SELECT aid FROM {authmap} WHERE authname = :id', array('id' => $lrdata['id']))->fetchField();
        socialloginandsocialshare_insert_into_mapping_table($aid, $lrdata['provider'], $lrdata['id']);
      }
      catch (Exception $e) {
        watchdog_exception('type', $e);
      }
    }

    $form_state['values']['name'] = $account->name;
    user_login_name_validate(array(), $form_state);

    if (!form_get_errors()) {
      socialloginandsocialshare_existing_user_save($account, $lrdata, $form, $form_state);
    }

    $_SESSION['current_social_provider'] = $lrdata['id'];
    socialloginandsocialshare_user_redirect($form, $account);
  }
  else {
    drupal_set_message(t("You are either blocked, or have not activated your account. Please check your email."), 'error');
    drupal_goto();
  }
}

/**
 * @param $lrdata
 * @param $form
 * @param $form_state
 *
 * Function for adding social user.
 */
function socialloginandsocialshare_adduser($lrdata, $form, &$form_state) {
  if (isset($lrdata['id']) && !empty($lrdata['id'])) {
    //check social provider is is already registered.
    $account = user_external_load($lrdata['id']);

    if (!$account) {
      //check email address is already registered.
      $account = user_load_by_mail($lrdata['email']);

      if ($account) {
        socialloginandsocialshare_update_mapping_data($account, $lrdata);
      }
    }

    if (isset($account->uid)) {
      socialloginandsocialshare_provide_login_to_unblock_user($account, $lrdata, $form, $form_state);
    }
    else {
      socialloginandsocialshare_save_new_user($lrdata, $form, $form_state);
    }
  }
}

/**
 * @param $lrdata
 * @param $form
 * @param $form_state
 *
 * Save new user and provde login to user.
 */
function socialloginandsocialshare_save_new_user($lrdata, $form, &$form_state) {
  $data = socialloginandsocialshare_check_exist_username($lrdata);
  $form_state['redirect'] = NULL;
  $form_state['values']['name'] = $data['username'];
  $form_state['values']['mail'] = $lrdata['email'];
  $form_state['values']['pass'] = user_password();

  if (variable_get('user_register') == 1 || (variable_get('socialloginandsocialshare_enable_user_registration') == 1 && variable_get('user_register') == 0)) {
    $form_state['values']['status'] = 1;
  }

  $form_state['values']['init'] = $lrdata['email'];
  $form_state['values']['field_first_name'] = array(LANGUAGE_NONE => array(array('value' => $data['fname'])));
  $form_state['values']['field_last_name'] = array(LANGUAGE_NONE => array(array('value' => $data['lname'])));

  if (variable_get('user_register', 1) || variable_get('user_register', 2) || (variable_get('socialloginandsocialshare_enable_user_registration') == 1 && variable_get('user_register') == 0)) {
    // Field module support.
    module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.field');
    $lrdata['gender'] = !empty($lrdata['gender']) ? socialloginandsocialshare_get_gender($lrdata['gender']) : '';
    socialloginandsocialshare_field_create_user($form_state['values'], $lrdata);
    $account = user_save(NULL, $form_state['values']);

    if (module_exists('profile2')) {
      socialloginandsocialshare_field_create_profile_user($account, $lrdata, $form, $form_state);
    }

    if (variable_get('user_pictures') == 1 && !empty($lrdata['thumbnail'])) {
      socialloginandsocialshare_insert_picture($lrdata, $account);
    }

    db_delete('authmap')
        ->condition('authname', $lrdata['id'])
        ->execute();
    user_set_authmaps($account, array('authname_socialloginandsocialshare' => $lrdata['id']));
    $aid = db_query('SELECT aid FROM {authmap} WHERE authname = :id', array('id' => $lrdata['id']))->fetchField();
    socialloginandsocialshare_insert_into_mapping_table($aid, $lrdata['provider'], $lrdata['id']);

    if (!$account) {
      drupal_set_message(t("Error saving user account."), 'error');
      $form_state['redirect'] = '';
      return;
    }

    $form_state['user'] = $account;
    $form_state['values']['uid'] = $account->uid;

    if ($account->status && !variable_get('user_verify', 1)) {
      watchdog('socialloginandsocialshare', 'New user: %name (%email).', array(
        '%name' => $data['username'],
        '%email' => $lrdata['email']
          ), WATCHDOG_NOTICE, l(t('edit'), 'user/' . $account->uid . '/edit'));

      if (variable_get('socialloginandsocialshare_send_user_credentials') == 1) {
        $params = array(
          'account' => $account,
          'pass' => $form_state['values']['pass'],
        );
        drupal_mail('socialloginandsocialshare', 'newuser', $account->mail, language_default(), $params);
      }

      $form_state['uid'] = $account->uid;
      user_login_submit(array(), $form_state);
      unset($_SESSION['lrdata']);
      $_SESSION['current_social_provider'] = $lrdata['id'];
      socialloginandsocialshare_user_redirect($form, $account, 'socialloginandsocialshare_userregister_redirect');
    }
    elseif ($account->status || variable_get('user_verify', 1)) {
      // Require email confirmation
      _user_mail_notify('status_activated', $account);
      drupal_set_message(t('Once you have verified your e-mail address, you may log in via Social Login.'));
      variable_set('user_verify', 0);
      drupal_goto();
    }
    else {
      _user_mail_notify('register_pending_approval', $account);
      drupal_set_message(t('Thank you for applying for an account. Your account is currently pending approval by the site administrator.<br />In the meantime, a welcome message with further instructions has been sent to your e-mail address.'));
      drupal_goto();
    }
  }
  else {
    drupal_set_message(t('Only site administrators can create new user accounts.'), 'error');
    drupal_goto();
  }
}
