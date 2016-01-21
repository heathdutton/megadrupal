<?php

/**
 * @file
 * Contain functions for help to login/registration process.
 */
module_load_include('inc', 'lr_social_login', 'includes/lr_field_mapping');
/**
 * Show error message when user profile is not retrieved or error message is showing.
 *
 * @param object $data contain error that got after validate userprofile api
 */
function lr_social_login_show_error_message($data) {
  if (variable_get('lr_social_login_debug_mode') == 1) {
    if (isset($data->description) && !empty($data->description)) {
      drupal_set_message(t($data->description), 'error');
    }
    else {
      drupal_set_message(t('UserProfile is not retrieved. Please enable cURL or fsockopen in your php.ini'), 'error');
    }
  }
  else {
    drupal_set_message(t('An error occurred during the processing of your request. Please try again in a few minutes or contact the site admin.'), 'error');
  }

  drupal_goto();
}

/**
 * Get the access token to access all APIs of LoginRadius.
 *
 * @return string LoginRadius access token
 */
function lr_social_login_access_token() {
  $obj = new LoginRadius();
  $secret = trim(variable_get('lr_social_login_apisecret'));
  return $obj->loginradiusFetchAccessToken($secret);
}

/**
 * Function that remove unescaped char from string.
 *
 * @param string $str Unfiltered string
 * @return string Filter string from unescaped characters
 */
function lr_social_login_remove_unescaped_char($str) {
  $in_str = str_replace(array(
    '<',
    '>',
    '&',
    '{',
    '}',
    '*',
    '/',
    '(',
    '[',
    ']',
    '!',
    ')',
    '&',
    '*',
    '#',
    '$',
    '%',
    '^',
    '|',
    '?',
    '+',
    '=',
    '"',
    ',',
  ), array(''), $str);
  $cur_encoding = mb_detect_encoding($in_str);

  if ($cur_encoding == "UTF-8" && mb_check_encoding($in_str, "UTF-8")) {
    return $in_str;
  }
  else {
    return utf8_encode($in_str);
  }
}


/**
 * Email Content when new user registers.
 *
 * @param string $key To identify  subject or message
 * @param Object $language Contain default language information
 * @param type $variables Variables contain user information
 * @return string Contain subject or message
 */
function lr_social_login_mail_text($key, $language = NULL, $variables = array()) {
  $langcode = isset($language) ? $language->language : NULL;

  // No override, return default string.
  switch ($key) {
    case 'newuser_subject':
      $text = t('Thank you for registering at [site:name]', array(), array('langcode' => $langcode));
      break;
    case 'newuser_body':
      $text = t("[user:name]

Thank you for registering at [site:name].

You will be able to login in the future using

Username : [user:name]
Password : " . $variables['pass'] . "

--  [site:name] team", array(), array('langcode' => $langcode));
      break;
  }

  $text = token_replace($text, $variables, array(
    'language' => $language,
    'callback' => 'user_mail_tokens'
  ));
  return $text;
}

/**
 * Function that insert social user picture.
 *
 * @param string $image_url Image Url that got from Social Network
 * @param object $account User account information
 */
function lr_social_login_insert_picture($image_url, $account) {
  if ($account->uid) {
    $uri = (!empty($account->picture->uri) ? $account->picture->uri : '');
    $fid = (!empty($account->picture->fid) ? $account->picture->fid : '');

    if (empty($uri)) {
      $fid = (!empty($account->picture) ? $account->picture : 0);
      $picture = file_load($fid);
      $uri = (!empty($picture->uri) ? $picture->uri : '');
    }

    if (file_exists($uri) && !empty($uri)) {
      $picture = $account->picture;
      file_delete(file_load($fid), TRUE);
    }
    $image_directory = file_default_scheme() . '://' . variable_get('user_picture_path', 'pictures');

    if (file_prepare_directory($image_directory, FILE_CREATE_DIRECTORY)) {
      $image_result = @drupal_http_request($image_url);
      $picture_path = file_stream_wrapper_uri_normalize($image_directory . '/picture-' . $account->uid . '-' . REQUEST_TIME . '.jpg');
      if (isset($image_result->data) && !empty($image_result->data)) {
        $picture_file_data = file_save_data($image_result->data, $picture_path, FILE_EXISTS_REPLACE);
        $max_dimensions = variable_get('user_picture_dimensions', '85x85');
        file_validate_image_resolution($picture_file_data, $max_dimensions);
        $picture_file_data->uid = $account->uid;
        $picture_file = file_save($picture_file_data);
        file_usage_add($picture_file, 'user', 'user', $account->uid);
        db_update('users')
          ->fields(array(
            'picture' => $picture_file->fid,
          ))
          ->condition('uid', $account->uid)
          ->execute();
        $account->picture = $picture_file->fid;
      }
    }
  }
}

/**
 * Get callback where authenticate loginradius token.
 *
 * @param string $key Check this function is called by friend invite/post/social login widget
 * @return string Contain encoded calleback where you page land.
 */
function lr_social_login_get_callback_url() {
  $destination = drupal_get_destination();
  $callback = $destination['destination'];

  if (strpos($callback, 'ajax') !== FALSE) {
    if (isset($_SESSION['redirect_url'])) {
      return $_SESSION['redirect_url'];
    }
    else {
      $callback = 'user';
    }
  }

  $query[] = array('destination' => $callback);
  $url = url('lr_social_login/token_handler', array(
    'query' => $query,
    'absolute' => TRUE,
  ));

  if (module_exists('ajax_register') || module_exists('modal_forms')) {
    $_SESSION['redirect_url'] = $url;
  }

  return drupal_encode_path($url);
}

/**
 * Get email that generate on basis of provider id.
 *
 * @param string $provider Social Network
 * @param string $id Provider ID
 * @return string Generated email address on basis of provider and provider id.
 */
function lr_social_login_get_email_by_provider_id($provider, $id) {
  $email_name = drupal_substr(str_replace(array("-", "/", "."), "_", $id), -10);
  $email = $email_name . '@' . $provider . '.com';
  $account = user_load_by_mail($email);

  if ($account && variable_get('lr_social_login_force_registration') != 0) {
    $id = $email_name . rand();
    $email = lr_social_login_get_email_by_provider_id($id, $provider);
  }
  return $email;
}

/**
 * Fill the user registration form with social data.
 *
 * @param object $userprofile User profile data that you got from social network
 */
function lr_social_login_enable_force_registration($userprofile) {
  if (variable_get('lr_social_login_force_registration') == 0) {
    $_SESSION['social_lrdata'] = $userprofile;
    unset($_GET['destination']);
    drupal_goto('user/register');
  }
}

/**
 * Get form element t show popup
 *
 * @param string $theme conatin rendered html form
 * @return array form elements values
 */
function lr_social_login_get_popup_form_element_array($theme) {
  return array(
    '#type' => 'item',
    '#title' => '',
    '#markup' => $theme,
    '#weight' => 20,
  );
}


/**
 * Insert loginradius mapping data into database.
 *
 * @param string $id Provider ID
 * @param string $provider Social Network
 * @param int $account_id User account ID
 */
function lr_social_login_insert_into_mapping_table($id, $provider, $account_id) {
  try {
    db_delete('authmap')
      ->condition('authname', $id)
      ->execute();
    db_insert('authmap')
      ->fields(array(
        'uid' => $account_id,
        'authname' => $id,
        'module' => 'lr_social_login',
      ))
      ->execute();
    $aid = db_query('SELECT aid FROM {authmap} WHERE authname = :id', array('id' => $id))->fetchField();
    $check_aid = db_query('SELECT aid FROM {loginradius_mapusers} WHERE aid = :id', array('id' => $aid))->fetchField();

    if (!empty($check_aid)) {
      db_delete('loginradius_mapusers')
        ->condition('aid', $aid)
        ->execute();
    }

    if (db_field_exists('loginradius_mapusers', 'user_id')) {
      db_insert('loginradius_mapusers')
        ->fields(array(
          'aid' => $aid,
          'user_id' => $account_id,
          'provider' => $provider,
          'provider_id' => $id,
        ))
        ->execute();
    }
    else {
      db_insert('loginradius_mapusers')
        ->fields(array(
          'aid' => $aid,
          'provider' => $provider,
          'provider_id' => $id,
        ))
        ->execute();
    }
  }
  catch (Exception $e) {
    watchdog_exception('type', $e);
  }
}

/**
 * Fill field mapping data into registration form.
 *
 * @param array $form Nested array of form elements that comprise the form
 * @param object $userprofile User profile data that you got from social network
 */

function lr_social_login_getuser_data($userprofile) {
  $userprofile->Email_value = (count($userprofile->Email) > 0 ? $userprofile->Email[0]->Value : '');
  $userprofile->Company_name = (isset($userprofile->Company->Name) ? $userprofile->Company->Name : '');
  $userprofile->Country_name = (isset($userprofile->Country->Name) ? $userprofile->Country->Name : '');
  $userprofile->PhoneNumber = (isset($userprofile->PhoneNumbers) && count($userprofile->PhoneNumbers) > 0 ? $userprofile->PhoneNumbers[0]->PhoneNumber : '');
  return $userprofile;
}

/**
 * submit callback Manage lr_social_login identities for the specified user.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param object $account User account information
 * @param string $token LoginRadius access Token
 * @function
 */
function lr_social_login_user_identities_submit(&$form, $account, $token) {
  $user_title = format_username($account);
  drupal_set_title(check_plain($user_title));

  if (user_is_logged_in() && $token != '') {
    $obj = new LoginRadius();
    $userprofile = $obj->loginradiusGetUserProfiledata($token);

    if (isset($userprofile->ID) && $userprofile->ID != '' && user_is_logged_in()) {
      //Advanced module LR Code Hook Start
      if (count(module_implements('add_user_identities_submit')) > 0) {
        // Call all modules that implement the hook, and let them make changes to $variables.
        module_invoke_all('add_user_identities_submit', $userprofile, $account);
      }
      //Advanced module LR Code Hook End
      $authname_exist = db_select('authmap', 'authname')
        ->fields('authname')
        ->condition('authname', $userprofile->ID)
        ->execute()
        ->fetchAssoc();
      $result = db_query("SELECT provider FROM {authmap} am INNER JOIN {loginradius_mapusers} sm ON am.aid = sm.aid WHERE uid = :uid", array(
        ':uid' => $account->uid,
      ));
      $mapped = '';

      foreach ($result as $identity) {
        if ($identity->provider == $userprofile->Provider) {
          $mapped = 'yes';
        }
      }

      if (empty($authname_exist) && !$authname_exist) {
        if ($mapped != 'yes') {
          if (variable_get('lr_social_login_update_profile') == 1) {
            if (variable_get('user_pictures') == 1 && !empty($userprofile->ImageUrl)) {
              lr_social_login_insert_picture($userprofile->ImageUrl, $account);
            }
          }

          lr_social_login_insert_into_mapping_table($userprofile->ID, $userprofile->Provider, $account->uid);
          drupal_set_message(t("Your account successfully mapped with this account."));
        }
        else {
          drupal_set_message(t("This social ID is already linked with an account. Kindly unmap the current ID before linking new Social ID."), 'warning');
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
 * Menu callback Delete the specified lr_social_login identity from the system.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 * @param object $account User account information
 * @param int $aid Authmap ID
 * @return array Renderable html confirm form
 */
function lr_social_login_user_delete_form($form, $form_state, $account, $lrid = 0, $provider = '') {
  if (empty($provider)) {
    $provider = db_query("SELECT provider FROM {authmap} am INNER JOIN {loginradius_mapusers} sm ON am.aid = sm.aid WHERE uid = :uid AND am.authname = :authname", array(
      ':uid' => $account->uid,
      ':authname' => $lrid,
    ))
      ->fetchField();
  }
  return confirm_form(array(), t('Are you sure you want to delete the Social Login %provider for %user?', array(
      '%provider' => $provider,
      '%user' => $account->name,
    )), 'user/' . $account->uid . '/lr_social_login');
}

/**
 * Menu callback Delete form submit for lr_social_login identity from the system.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 */
function lr_social_login_user_delete_form_submit($form, &$form_state) {
  if (isset($form_state['build_info']['args'][2])) {
    $provider = $form_state['build_info']['args'][2];
  }
  else {
    $provider = db_query('SELECT provider FROM {loginradius_mapusers} WHERE provider_id = :id', array('id' => $form_state['build_info']['args'][1]))->fetchField();
  }
  //Advanced module LR Code Hook Start
  if (count(module_implements('add_user_delete_form_submit')) > 0) {
    // Call all modules that implement the hook, and let them make changes to $variables.
    module_invoke_all('add_user_delete_form_submit', $form_state, $provider);
  }
  //Advanced module LR Code Hook End
  $query = db_delete('authmap')
    ->condition('uid', $form_state['build_info']['args'][0]->uid)
    ->condition('authname', $form_state['build_info']['args'][1])
    ->execute();
  $query2 = db_delete('loginradius_mapusers')
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
 * Respond to altering user login/register form.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 */
function lr_social_login_user_login_form_alter(&$form, &$form_state) {
  $api_key = trim(variable_get('lr_social_login_apikey'));
  $secret = trim(variable_get('lr_social_login_apisecret'));
  $iconpos = (variable_get('lr_social_login_showicon_location', 2) ? 102 : -102);
  if (!empty($api_key) && !empty($secret) && (!preg_match('/^\{?[A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}\}?$/i', $api_key) || !preg_match('/^\{?[A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}\}?$/i', $secret))) {
    $value = t('<div class="interface_error">Your LoginRadius API key or secret is not valid, please correct it or contact LoginRadius support at <a href="@this" target="_blank">www.LoginRadius.com</a></div>', array('@this' => 'http://www.Loginradius.com/'));
  }
  elseif (!empty($api_key) && !empty($secret)) {
    $value = theme('lr_social_login_links');
  }
  //Advanced module LR Code Hook Start
  if (count(module_implements('add_social_login_interface_widget')) > 0) {
    // Call all modules that implement the hook, and let them make changes to $variables.
    $result = module_invoke_all('add_social_login_interface_widget');
    $widget = end($result);
    if (!empty($widget)) {
      $value = $widget;
    }
  }
  //Advanced module LR Code Hook End
  if (!empty($value)) {
    $my_settings = lr_social_login_interface_settings();
    $form['lr_social_login_links'] = array(
      '#type' => 'item',
      '#title' => variable_get('lr_social_login_label_string', 'Login with Social ID'),
      '#markup' => $value,
      '#attributes' => array('class' => array('lr_social_login-links')),
      '#weight' => $iconpos,
      '#attached' => array(
        'js' => array(
          array(
            'data' => array('lrsociallogin' => $my_settings),
            'type' => 'setting'
          )
        )
      ),
    );
  }
}

/**
 * @return array
 */
function lr_social_login_interface_settings() {
  $interfacerow = variable_get('lr_social_login_interface_columns');
  $interfacerow = !empty($interfacerow) && is_numeric($interfacerow) ? trim($interfacerow) : 0;
  $my_settings = array(
    'interfacesize' => variable_get('lr_social_login_interface_size') == 0 ? "small" : "",
    'lrinterfacebackground' => variable_get('lr_social_login_interface_bgcolor'),
    'noofcolumns' => (int) $interfacerow,
    'apikey' => trim(variable_get('lr_social_login_apikey')),
    'location' => urldecode(lr_social_login_get_callback_url()),
    'callback' => url('', array('absolute' => TRUE)),
  );
  return $my_settings;
}

/**
 * Function that redirects user after login.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param object $account User account information
 * @param object $userprofile Conatin user profile information
 * @param string $variable_path Key to check its for login or registration.
 * @return array A renderable html form
 */
function lr_social_login_user_redirect($form, $account, $userprofile, $variable_path = '') {
  //Advanced module LR Code Hook Start
  // Make sure at least one module implements our hook.
  if (count(module_implements('before_user_redirect')) > 0) {
    // Call all modules that implement the hook, and let them make changes to $variables.
    $use_data = array('userprofile' => $userprofile, 'form' => $form, 'account' => $account);

    $data = module_invoke_all('before_user_redirect', $use_data);
    if (!empty($data) && $data != 'true') {
      return $data;
    }
  }
  //Advanced module LR Code Hook End

  $variable_path = (!empty($variable_path) ? $variable_path : 'lr_social_login_userlogin_redirect');
  $variable_custom_path = (($variable_path == 'lr_social_login_userlogin_redirect') ? 'lr_social_login_custom_redirection' : 'lr_social_login_custom_register_redirection');
  $request_uri = request_uri();

  if (strpos($request_uri, 'user/register') && $variable_path != 'lr_social_login_userregister_redirect') {
    $variable_path = 'lr_social_login_userregister_redirect';
    lr_social_login_user_redirect($form, $account, $userprofile, $variable_path);
  }
  else {

    if (variable_get($variable_path) == 1) {
      // Redirect to profile.
      unset($_GET['destination']);
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
 * Function that create username.
 *
 * @param object $userprofile Contain user profile information
 * @return string Username of user
 */
function lr_social_login_get_username($userprofile) {
  if (!empty($userprofile->FullName)) {
    $username = $userprofile->FullName;
  }
  elseif (!empty($userprofile->ProfileName)) {
    $username = $userprofile->ProfileName;
  }
  elseif (!empty($userprofile->NickName)) {
    $username = $userprofile->NickName;
  }
  elseif (!empty($userprofile->Email_value)) {
    $user_name = explode('@', $userprofile->Email_value);
    $username = $user_name[0];
  }
  else {
    $username = $userprofile->ID;
  }
  return $username;
}

/**
 * Function that check username exist or not.
 *
 * @param object $userprofile user profile information
 * @return array Contain username, firstname and lastname
 */
function lr_social_login_check_exist_username($userprofile) {
  $user_name = lr_social_login_username_option($userprofile);
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

  $data['username'] = lr_social_login_remove_unescaped_char($user_name);
  $data['fname'] = (!empty($userprofile->FirstName) ? $userprofile->FirstName : '');
  $data['lname'] = (!empty($userprofile->LastName) ? $userprofile->LastName : '');

  if (empty($data['fname'])) {
    $data['fname'] = lr_social_login_get_username($userprofile);
  }

  if (empty($data['lname'])) {
    $data['lname'] = lr_social_login_get_username($userprofile);
  }

  return $data;
}

/**
 * Get username from user profile data
 *
 * @param object $userprofile User profile information
 * @return string Username of user
 */
function lr_social_login_username_option($userprofile) {
  if (!empty($userprofile->FirstName) && !empty($userprofile->LastName) && variable_get('lr_social_login_display_username') != 2) {
    if (variable_get('lr_social_login_display_username') == 1) {
      $username = $userprofile->FirstName . '-' . $userprofile->LastName;
    }
    else {
      $username = $userprofile->FirstName . ' ' . $userprofile->LastName;
    }
  }
  elseif (variable_get('lr_social_login_display_username') == 2 && !empty($userprofile->Email_value)) {
    $username = $userprofile->Email_value;
  }
  else {
    $username = lr_social_login_get_username($userprofile);
  }

  return $username;
}

/**
 * update data of existing user.
 *
 * @param object $account User account information
 * @param object $userprofile User proifle data
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 */
function lr_social_login_existing_user_save($account, $userprofile, $form, $form_state) {
  if (variable_get('lr_social_login_update_profile') == 1) {
    module_load_include('inc', 'lr_social_login', 'includes/lr_social_login.field');
    $data = array();

    lr_field_mapping_create_user($data, $userprofile, $account);
    if (module_exists('profile2')) {
      lr_field_mapping_create_profile_user($account, $userprofile, $form, $form_state);
    }

    $account = user_save($account, $data);

    if (variable_get('user_pictures') == 1 && !empty($userprofile->ImageUrl)) {
      lr_social_login_insert_picture($userprofile->ImageUrl, $account);
    }

  }
  //Advanced module LR Code Hook Start
  if (count(module_implements('add_existing_user_save')) > 0) {
    // Call all modules that implement the hook, and let them make changes to $variables.
    module_invoke_all('add_existing_user_save', $userprofile, $account);
  }
  //Advanced module LR Code Hook End

  $form_state['uid'] = $account->uid;
  user_login_submit(array(), $form_state);
}

/**
 * Function for adding social user
 *
 * @param object $userprofile User proifle data
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 * @return array A renderable html form
 */
function lr_social_login_adduser($userprofile, &$form, &$form_state) {
  if (isset($userprofile->ID) && !empty($userprofile->ID)) {
    $account = user_external_load($userprofile->ID);
    //Advanced module LR Code Hook Start
    // Make sure at least one module implements our hook.
    if (count(module_implements('check_raas_uid')) > 0) {
      // Call all modules that implement the hook, and let them make changes to $variables.
      $result = module_invoke_all('check_raas_uid', $userprofile);
      $account = end($result);
    }
    //Advanced module LR Code Hook End

    if (!$account) {
      $account = user_load_by_mail($userprofile->Email_value);

      if ($account) {
        $authname_exist = db_select('authmap', 'authname')
          ->fields('authname')
          ->condition('authname', $userprofile->ID)
          ->execute()
          ->fetchAssoc();

        if (empty($authname_exist) && !$authname_exist) {
          if (variable_get('lr_social_login_update_profile') == 1) {
            if (variable_get('user_pictures') == 1 && !empty($userprofile->ImageUrl)) {
              lr_social_login_insert_picture($userprofile->ImageUrl, $account);
            }
          }

          lr_social_login_insert_into_mapping_table($userprofile->ID, $userprofile->Provider, $account->uid);
        }
      }
    }

    if (isset($account->uid) && $account->uid != 0) {
      return lr_social_login_provide_login_to_unblock_user($account, $userprofile, $form, $form_state);
    }
    else {
      return lr_social_login_save_new_user($userprofile, $form, $form_state);
    }
  }
}

/**
 * Handle email popup submission.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 * @param type $post_value POST Values
 * @return array A renderable html form
 */
function lr_social_login_email_popup_submit(&$form, &$form_state, $post_value) {
  if (isset($_SESSION['lrdata']) && !empty($_SESSION['lrdata'])) {
    $userprofile = $_SESSION['lrdata'];
    $userprofile->Email_value = trim($post_value['email']);

    if (!valid_email_address($userprofile->Email_value)) {
      $popup_params = array(
        'msg' => t('This email is invalid. Please choose another one.'),
        'provider' => $userprofile->Provider,
        'msgtype' => 'warning',
      );
      $theme = theme('lr_social_login_popup', array('popup_params' => $popup_params));
      $form['lr_social_login_popup'] = lr_social_login_get_popup_form_element_array($theme);
    }
    else {
      $check_mail = user_load_by_mail($userprofile->Email_value);

      if (!empty($check_mail)) {
        $email_wrong = variable_get('lr_social_login_emailrequired_popup_wrong');
        $popup_params = array(
          'msg' => t($email_wrong),
          'provider' => $userprofile->Provider,
          'msgtype' => 'warning',
        );
        $theme = theme('lr_social_login_popup', array('popup_params' => $popup_params));
        $form['lr_social_login_popup'] = lr_social_login_get_popup_form_element_array($theme);
      }
      else {
        variable_set('user_verify', 1);
        unset($_SESSION['lrdata']);
        return $value = lr_social_login_adduser($userprofile, $form, $form_state);
      }
    }
  }

  return $form;
}

/**
 * Check user is exist in database
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 * @param object $userprofile User profile data
 * @return array A renderable html form
 */
function lr_social_login_check_existing_user(&$form, &$form_state, $userprofile) {
  if ((empty($userprofile->Email_value) && variable_get('lr_social_login_email_required') == 0) || !empty($userprofile->Email_value)) {
    if (empty($userprofile->Email_value) && variable_get('lr_social_login_email_required') == 0) {
      $userprofile->Email_value = lr_social_login_get_email_by_provider_id($userprofile->Provider, $userprofile->ID);
    }

    $account = user_external_load($userprofile->ID);
    if (!$account) {
      $account = user_load_by_mail($userprofile->Email_value);

      if (!$account) {
        lr_social_login_enable_force_registration($userprofile);
      }
    }

    return $form = lr_social_login_adduser($userprofile, $form, $form_state);
  }
}

/**
 * Provide login to unblock user
 *
 * @param object $account User account information
 * @param object $userprofile User profile data
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 * @return array A renderable html form
 */
function lr_social_login_provide_login_to_unblock_user($account, $userprofile, &$form, &$form_state) {
  // $verified = db_query('SELECT verified FROM {loginradius_mapusers} WHERE provider_id = :id', array('id' => $userprofile->ID))->fetchField();
  // if ($verified == 1) {
  if ($account->login) {
    // Check if user is blocked.
    $form_state['values']['name'] = $account->name;
    user_login_name_validate(array(), $form_state);

    if (!form_get_errors()) {
      lr_social_login_existing_user_save($account, $userprofile, $form, $form_state);
    }

    $_SESSION['current_social_provider'] = $userprofile->ID;

    return lr_social_login_user_redirect($form, $account, $userprofile);
  }
  else {
    drupal_set_message(t("You are either blocked, or have not activated your account. Please check your email."), 'error');
    drupal_goto();
  }
}

/**
 * Save new user into database and provide login
 *
 * @param object $userprofile
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 * @return array A renderable html form
 */
function lr_social_login_save_new_user($userprofile, &$form, &$form_state) {
  $data = lr_social_login_check_exist_username($userprofile);
  $form_state['redirect'] = NULL;
  $form_state['values']['name'] = $data['username'];
  $form_state['values']['mail'] = $userprofile->Email_value;
  $form_state['values']['pass'] = user_password();

  if (variable_get('user_register') != 2 && (variable_get('user_register') == 1 || variable_get('lr_social_login_enable_user_registration') == 1)) {
    $form_state['values']['status'] = 1;
  }

  $form_state['values']['init'] = $userprofile->Email_value;
  $form_state['values']['field_first_name'] = array(LANGUAGE_NONE => array(array('value' => $data['fname'])));
  $form_state['values']['field_last_name'] = array(LANGUAGE_NONE => array(array('value' => $data['lname'])));

  if (variable_get('user_register', 1) || variable_get('user_register', 2) || variable_get('lr_social_login_enable_user_registration') == 1) {
    // Field module support.

    lr_field_mapping_create_user($form_state['values'], $userprofile);
    $account = user_save(NULL, $form_state['values']);
    if ($account->status && !variable_get('user_email_verification', TRUE) && !variable_get('user_verify', 1)) {
      db_update('users')
        ->fields(array(
          'login' => REQUEST_TIME,
        ))
        ->condition('uid', $account->uid)
        ->execute();
    }
    if (module_exists('profile2')) {
      lr_field_mapping_create_profile_user($account, $userprofile, $form, $form_state);
    }

    if (variable_get('user_pictures') == 1 && !empty($userprofile->ImageUrl)) {
      lr_social_login_insert_picture($userprofile->ImageUrl, $account);
    }
    //Advanced module LR Code Hook Start
    if (count(module_implements('add_user_data_after_save')) > 0) {
      // Call all modules that implement the hook, and let them make changes to $variables.
      module_invoke_all('add_user_data_after_save', $account, $userprofile);
    }
    //Advanced module LR Code Hook End
    lr_social_login_insert_into_mapping_table($userprofile->ID, $userprofile->Provider, $account->uid);

    if (!$account) {
      drupal_set_message(t("Error saving user account."), 'error');
      $form_state['redirect'] = '';
      return;
    }

    $form_state['user'] = $account;
    $form_state['values']['uid'] = $account->uid;
    $status = FALSE;
    if (!variable_get('user_email_verification', TRUE)) {
      $status = TRUE;
    }
    //Advanced module LR Code Hook Start
    // Make sure at least one module implements our hook.
    if (count(module_implements('check_send_verification_email')) > 0) {
      // Call all modules that implement the hook, and let them make changes to $variables.
      $userprofile->Password = $form_state['values']['pass'];
      $result = module_invoke_all('check_send_verification_email', $account, $userprofile);
      if (isset($result['lr_social_invite_message_popup'])) {
        return array('lr_social_invite_message_popup' => $result['lr_social_invite_message_popup']);
      }
      $status = end($result);
    }
    //Advanced module LR Code Hook End
    if ($account->status && $status && !variable_get('user_verify', 1)) {
      watchdog('lr_social_login', 'New user: %name (%email).', array(
        '%name' => $data['username'],
        '%email' => $userprofile->Email_value,
      ), WATCHDOG_NOTICE, l(t('edit'), 'user/' . $account->uid . '/edit'));

      if (variable_get('lr_social_login_send_user_credentials') == 1) {
        $params = array(
          'account' => $account,
          'pass' => $form_state['values']['pass'],
        );
        drupal_mail('lr_social_login', 'newuser', $account->mail, language_default(), $params);
      }

      $form_state['uid'] = $account->uid;
      user_login_submit(array(), $form_state);
      unset($_SESSION['lrdata']);
      $_SESSION['current_social_provider'] = $userprofile->ID;
      return lr_social_login_user_redirect($form, $account, $userprofile, 'lr_social_login_userregister_redirect');
    }
    elseif (variable_get('user_register') != 2 && ($account->status || (variable_get('user_verify', 1) && $status))) {
      // Require email confirmation.
      _user_mail_notify('status_activated', $account);
      variable_set('user_verify', 0);
      drupal_set_message(t('Once you have verified your e-mail address, you may log in via Social Login.'));
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
