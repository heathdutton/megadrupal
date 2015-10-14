<?php

/**
 * @file
 * Provides page callbacks and functions for various sso callback processes.
 */

/**
 * Callback function for single sign on login process.
 *
 * This returns a spacer image.
 */
function sso_multi_domain_spacer_image($hash, $url) {

  $url = base64_decode($url);
  $url = $url . '/' . $hash;
  // Send request to retrieve user details stored using unique hash.
  $response = drupal_http_request($url);
  if ($response->code == 200) {
    // Decodes the json data returned by requested url.
    $login_data = drupal_json_decode($response->data);
    // Process login procedure if we status is 1 returned by web service.
    if ($login_data['status'] == 1) {
      $username = $login_data['username'];
      $password = $login_data['password'];
      $user_data = $login_data['account'];

      // Decrypts the username and password.
      $username = sso_multi_domain_decrypt_string($username);
      $password = sso_multi_domain_decrypt_string($password);

      // Check if an account with given username exists or not.
      $account = user_load_by_name($username);

      // If account do not exists.
      if (!$account) {
        $is_registration_allowed = variable_get('sso_multi_domain_allow_registration', 0);

        // Check if registration allowed for this domain or not.
        // If registration allowed then call create account function to create
        // the user account.
        if ($is_registration_allowed) {
          sso_multi_domain_create_account($account, $username, $password, $user_data);
        }
      }

      // Check if user account exists and user is authenticated then process
      // login procedure.
      if ($account && user_authenticate($username, $password)) {

        $user_obj = user_load_by_name($username);
        $form_state = array();
        $form_state['uid'] = $user_obj->uid;
        user_login_submit(array(), $form_state);
      }

      // Creates a spacer image to used in image tag used by requesting domain.
      $my_img = imagecreate(1, 1);
      $text_colour = imagecolorallocate($my_img, 255, 255, 0);
      $line_colour = imagecolorallocate($my_img, 128, 255, 0);
      imagestring($my_img, 4, 30, 25, time(), $text_colour);
      imagesetthickness($my_img, 5);
      imageline($my_img, 30, 45, 165, 45, $line_colour);

      header("Content-type: image/png");
      imagepng($my_img);

      imagedestroy($my_img);
    }
  }
}

/**
 * Callback function for fetch login details web service.
 */
function sso_multi_domain_fetch_login_details($hash) {
  $query = db_select('sso_multi_domain_sessions', 's');
  $query->fields('s', array('username', 'password', 'uid'));
  $query->condition('s.hash', $hash);
  $result = $query->execute();
  $data = array();
  if ($result->rowCount()) {
    $login_details = $result->fetchObject();
    $data['status'] = 1;
    $data['username'] = $login_details->username;
    $data['password'] = $login_details->password;
    $account = user_load($login_details->uid);
    $data['account'] = $account;
  }
  else {
    $data['status'] = 0;
  }
  return $data;
}

/**
 * Callback function for single sign on process execution page.
 */
function sso_multi_domain_execute_sso() {
  drupal_add_js(drupal_get_path('module', 'sso_multi_domain') . '/js/sso_multi_domain.js');
  drupal_add_css(drupal_get_path('module', 'sso_multi_domain') . '/css/sso_multi_domain.css');
  if (isset($_SESSION['process_sso']) && $_SESSION['process_sso']) {
    drupal_add_js(array('sso_multi_domain' => array('destination' => url($_SESSION['destination'], array('absolute' => TRUE)))), 'setting');

    // Load domains list.
    $sso_multi_domains = variable_get('sso_multi_domain_domain_list', '');
    $domains = explode(',', str_replace(array("\r", "\n"), ',', $sso_multi_domains));

    // Load loader image fid and renders the images.
    $loader_image_fid = variable_get('sso_multi_domain_loader_image_fid', '');

    if ($loader_image_fid) {
      $loader_image = file_load($loader_image_fid);
      $loader_image = theme('image', array('path' => $loader_image->uri));
    }
    else {
      $default_loader_image_path = base_path() . drupal_get_path('module', 'sso_multi_domain') . '/images/loader.gif';
      $loader_image = '<img src="' . $default_loader_image_path . '" alt="' . t('Loading...') . '" title="' . t('Loading...') . '" />';
    }

    // Load processing page message.
    $message = variable_get('sso_multi_domain_message', '');
    $processing_page_text = isset($message['value']) ? check_markup($message['value'], $message['format']) : t('Please wait while we are processsing');

    $url_to_check = base64_encode(url('sso_multi_domain/fetch_login_details', array('absolute' => TRUE)));
    $images = '';
    foreach ($domains as $key => $domain) {
      $domain = trim($domain);
      if ($domain) {
        $images .= '<img src="' . url('http://' . $domain . '/sso_multi_domain/spacer/' . $_SESSION['sso_multi_domain_hash'] . '/' . $url_to_check, array('absolute' => TRUE)) . '" />';
      }
    }

    $html = theme('sso_multi_domain_process_page', array(
      'images' => $images,
      'loader_image' => $loader_image,
      'processing_page_text' => $processing_page_text,
        )
    );

    // Clear the sessions.
    unset($_SESSION['process_sso']);
    unset($_SESSION['sso_multi_domain_hash']);
    unset($_SESSION['destination']);

    return $html;
  }
}

/**
 * Helper function to create account and roles.
 */
function sso_multi_domain_create_account(&$account, $username, $password, $user_data) {

  $is_roles_sysc_allowed = variable_get('sso_multi_domain_allow_roles_sysc', 0);
  $user_roles = array();

  foreach ($user_data['roles'] as $this_role) {
    switch ($this_role) {
      case 'administrator':
      case 'authenticated user':
        $role = user_role_load_by_name($this_role);
        if (isset($role->rid) && is_numeric($role->rid)) {
          $user_roles[$role->rid] = $role->name;
        }
        break;

      default:
        // Check if the role exists then assign push it in array else check if
        // role synchronization is enabled or not. If enabled then create role
        // and push it in array.
        $role = user_role_load_by_name($this_role);
        if (isset($role->rid) && is_numeric($role->rid)) {
          $user_roles[$role->rid] = $role->name;
        }
        elseif ($is_roles_sysc_allowed) {
          $role = new stdClass();
          $role->name = $this_role;
          if (user_role_save($role)) {
            $role = user_role_load_by_name($this_role);
            if (isset($role->rid) && is_numeric($role->rid)) {
              $user_roles[$role->rid] = $role->name;
            }
          }

          break;
        }
    }
  }

  // Set up the user fields.
  $fields = array(
    'name' => $username,
    'mail' => $user_data['mail'],
    'pass' => $password,
    'status' => 1,
    'init' => 'email address',
    'roles' => $user_roles,
  );

  // The first parameter is left blank so a new user is created.
  $account = user_save('', $fields);
}
