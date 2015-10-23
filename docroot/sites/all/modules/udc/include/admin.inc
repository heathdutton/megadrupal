<?php
/**
 * @file
 * -----------------------------------------------------------------------------
 * Drupal 7 - User Data Connector Module - configuration
 * -----------------------------------------------------------------------------
 *
 * @author Stefan Wilhelm (stfwi)
 * @package org.drupal.project.udc
 * @license GPL2
 */

// Unlock the client classes for including in drupal context.
if(!defined('USER_DATA_CONNECTOR_ALLOW_CLIENT_INCLUDE')) {
  define('USER_DATA_CONNECTOR_ALLOW_CLIENT_INCLUDE', TRUE);
}

/**
 * Configuration autocomplete callback for user names, see udc_menu().
 *
 * @param string $string
 * @return string
 */
function udc_users_autocomplete($string = '') {
  $tags = drupal_explode_tags($string);
  $string = trim(array_pop($tags));
  $matches = array();
  if ($string) {
    $query = db_select('users')->fields('users', array('name'))->condition('name', db_like($string) . '%', 'LIKE')->range(0, 10)->execute();
    $prefix = !empty($tags) ? (implode(', ', $tags) . ', ') : ('');
    foreach ($query as $name) {
      if (!in_array($name->name, $tags)) $matches[$prefix . $name->name] = check_plain($name->name);
    }
  }
  drupal_json_output($matches);
}

/**
 * Configuration autocomplete callback for role names, see udc_menu().
 *
 * @param string $string
 * @return string
 */
function udc_roles_autocomplete($string = '') {
  $tags = drupal_explode_tags($string);
  $string = trim(array_pop($tags));
  $matches = array();
  if ($string) {
    $query = db_select('role')->fields('role', array('name'))->condition('name', db_like($string) . '%', 'LIKE')->range(0, 10)->execute();
    $p = !empty($tags) ? (implode(', ', $tags) . ', ') : ('');
    foreach ($query as $role) {
      if (drupal_strtolower($role->name) != 'anonymous user' && drupal_strtolower($role->name) != 'authenticated user') {
        if (!in_array($role->name, $tags)) {
          $matches[$p . $role->name] = check_plain($role->name);
        }
      }
    }
  }
  drupal_json_output($matches);
}

/**
 * Configuration autocomplete callback for user fields/profile fields, see udc_menu().
 *
 * @param string $string
 * @return string
 */
function udc_userfields_autocomplete($string = '') {
  $tags = drupal_explode_tags($string);
  $string = trim(array_pop($tags));
  $matches = array();
  if ($string) {
    if (strpos($string, '<') === 0) {
      $matches['<all>'] = check_plain('<all>');
    }
    else {
      $p = !empty($tags) ? (implode(', ', $tags) . ', ') : ('');
      if (db_table_exists('profile_field')) {
        $query = db_select('profile_field')->fields('profile_field', array('name'))->condition('name', db_like($string) . '%', 'LIKE')->range(0, 10)->execute();
        foreach ($query as $field) {
          if (!in_array($field->name, $tags)) $matches[$p . $field->name] = check_plain($field->name);
        }
      }
      if (db_table_exists('field_config_instance')) {
        $query = db_select('field_config_instance')
          ->fields('field_config_instance', array('field_name'))
          ->condition('field_name', db_like($string) . '%', 'LIKE')
          ->condition('entity_type', 'user', '=')
          ->range(0, 10)->execute();
        foreach ($query as $field) {
          if (!in_array($field->field_name, $tags)) $matches[$p . $field->field_name] = check_plain($field->field_name);
        }
      }
    }
  }
  drupal_json_output($matches);
}


/**
 * Implementation of form admin/config/people/udc for drupal_get_form(),
 * see udc_menu(). Returns the configuration form structure
 *
 * @return array
 */
function udc_admin() {
  include_once(dirname(__FILE__) . '/admin.txt.inc');
  $token = UserDataConnectorConfig::instance()->getToken();
  if (empty($token)) $token = str_replace('=', '', base64_encode(sha1(print_r($_SERVER, TRUE))));
  $unlisted_users = UserDataConnectorConfig::instance()->getUnlistedUsers();
  $unlisted_users = is_array($unlisted_users) ? drupal_implode_tags($unlisted_users) : '';
  $profile_fields = UserDataConnectorConfig::instance()->getListedProfileFields();
  $profile_fields = ($profile_fields === TRUE) ? '<all>' : (is_array($profile_fields) ? drupal_implode_tags($profile_fields) : '');
  $roles = UserDataConnectorConfig::instance()->getListedRoles();
  $roles = is_array($roles) ? drupal_implode_tags($roles) : '';
  $has_ssl = function_exists('curl_init');

  try {
    include_once(dirname(dirname(__FILE__)) . '/client/DrupalUserInfo.class.inc');
    $udc = new DrupalUserInfo();
    if (!$udc->ping()) {
      // Try without HTTP_HOST instead of localhost
      define('USER_DATA_CONNECTOR_USE_HTTP_HOST_INSTEAD_OF_LOCALHOST', TRUE);
      $udc = new DrupalUserInfo();
      if (!$udc->ping()) {
        $form['udc_no_server_script'] = array(
          '#markup' => '<div style="color:red; font-weight:bold;"><br/>' .
            t('The server script is not accessible. Please modify your server configuration,
               so that the script !script is callable via HTTP/HTTPS.',
              array('!script' => '/sites/all/modules/udc/server/server.php'))
            . '</div>'
        );
      } else {
        $sample_code_need_http_host = true;
      }
    }
    unset($udc);
  } catch (Exception $e) {
    $form['udc_no_server_script'] = array(
      '#markup' => '<div style="color:red; font-weight:bold;"><br/>' .
        t('The test request threw an exception: !ex',
           array('!ex' => $e->getMessage())) . '</div>'
    );
  }

  $form['udc_intro'] = array(
    '#markup' => udc_admin_get_form_introduction()
  );


  $form['udc_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#default_value' => $token,
    '#size' => 100,
    '#maxlength' => 200,
    '#description' => t('The security token used to identify your external script request.
                         This is required because more than one virtual host can be located
                         on the same server with the same IP address. The server script will
                         not accept an empty string here (use some big random string, the module
                         creates one for you at startup).  The client classes provide the static
                         function <code>DrupalUserBase::setToken($string)</code> set the token
                         used for the request. You can also enter the token string in a text
                         file and specify here <i><b>file:</b>./path/to/file</i>, where the path
                         can be absolute (leading "/") or relative to $_SERVER[\'DOCUMENT_ROOT\'].
                         Note that the file must not be accessible via HTTP if you chose this
                         variant.'),
    '#required' => TRUE,
  );
  $form['udc_listed_roles'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed roles'),
    '#description' => t('Here you can restrict the access using the user roles. If you enter roles
                        here, only users with these roles will be accepted/listed by the server script.
                        Leave the field blank to accept all roles, or enter the role names comma
                        separated.'),
    '#size' => 100,
    '#maxlength' => 5000,
    '#autocomplete_path' => 'admin/config/people/udc/autocomplete/roles',
    '#default_value' => $roles
  );
  $form['udc_unlisted_users'] = array(
    '#type' => 'textfield',
    '#title' => t('Unlisted users'),
    '#description' => t('If there are users you don\'t want to be exported, like "admin" etc, specify
                        them here and the server will exclude these accounts from the response. Enter
                        the user names comma separated.'),
    '#size' => 100,
    '#maxlength' => 5000,
    '#autocomplete_path' => 'admin/config/people/udc/autocomplete/users',
    '#default_value' => $unlisted_users
  );
  $form['udc_listed_profile_fields'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed profile fields'),
    '#description' => t('By listing the allowed field names here you can restrict the profile data that
                        the server script returns. If you want no fields to be accessible by external
                        scripts, then leave the text blank. If you want no field restrictions at all,
                        enter "<i><b>&lt;all&gt;</b></i>". The server script will not return an error
                        to the client if suppressed fields are requested, it just does not include them
                        in the response. Enter the fields comma separated.'),
    '#size' => 100,
    '#maxlength' => 5000,
    '#autocomplete_path' => 'admin/config/people/udc/autocomplete/userfields',
    '#default_value' => $profile_fields
  );
  $form['udc_require_https'] = array(
    '#type' => 'checkbox',
    '#title' => t('SSL only'),
    '#description' => ($has_ssl ? '' : ( '<span style="color:#ff7777;">' . t('Your PHP does not support
                        the cURL library, unfortunately you cannot enable SSL.') . '</span> ') )
                    . t('Specifies if the server script accepts only connections via HTTPS. It will then
                        reject unencrypted HTTP connections with an error message. Note that you need
                        the cURL library to enable this feature. The client classes recognize the boolean
                        constant <code>USER_DATA_CONNECTOR_USE_SSL</code> to switch the connection type.'),
    '#default_value' => $has_ssl ? UserDataConnectorConfig::instance()->getRequireHttps() : FALSE,
    '#disabled' => !$has_ssl
  );
  $form['udc_enable_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable server script debugging information'),
    '#description' => t('Enables the server script to send additional debug information in the response. The
                        data can be fetched from the client using <code>$result = $instance->getDebug()</code>,
                        where <code>$instance</code> is an object derived from <code>DrupalUserBase</code>.'),
    '#default_value' => UserDataConnectorConfig::instance()->getDebugEnabled()
  );
  $form['udc_additional'] = array(
    '#type' => 'item',
    '#title' => t('Sample code'),
    '#markup' => udc_admin_get_form_sample_code(
      UserDataConnectorConfig::instance()->getToken(),
      isset($sample_code_need_http_host) && $sample_code_need_http_host,
      UserDataConnectorConfig::instance()->getRequireHttps()
    )
  );
  return system_settings_form($form);
}


/**
 * Form validation callback
 * @param array $form
 * @param array &$form_state
 * @return void
 */
function udc_admin_validate($form, &$form_state) {
  $token = &$form_state['values']['udc_token'];
  $https = &$form_state['values']['udc_require_https'];
  $debug = &$form_state['values']['udc_enable_debug'];
  $unlisted_users = &$form_state['values']['udc_unlisted_users'];
  $fields = &$form_state['values']['udc_listed_profile_fields'];
  $roles = &$form_state['values']['udc_listed_roles'];

  $token = trim($token, " \t");
  $https = (bool) $https;
  $debug = (bool) $debug;
  $unlisted_users = trim($unlisted_users, " \t,");
  $unlisted_users = empty($unlisted_users) ? array() : drupal_explode_tags($unlisted_users);
  $fields = trim($fields, " \t,");
  $fields = empty($fields) ? array() : (drupal_strtolower($fields) == '<all>' ? TRUE : drupal_explode_tags($fields));
  $roles = trim($roles, " \t,");
  $roles = empty($roles) ? array() : drupal_explode_tags($roles);
  if (empty($token)) {
    form_set_error('udc_token', t('The server script will not accept an empty token.'));
  }
  if ($https && !function_exists('curl_init')) {
    form_set_error('udc_require_https', t('PHP has no cURL library, SSL cannot be used.'));
    $https = FALSE;
  }
}
