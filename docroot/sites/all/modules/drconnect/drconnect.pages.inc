<?php

/**
 * Draugiem.lv Passport prompt page.
 *
 * This page is opened after authorizing with Draugiem.lv profile.
 */
function drconnect_prompt_page() {
  if (drconnect_draugiem_client(drconnect_api_filepath())->getSession()) {
    $druid = drconnect_get_druid();
    if (user_is_anonymous()) {
      // User is not logged in with his site account.
      $uid = _is_drconnect_user($druid);
      if ($uid) {
        // Login.
        $form_state['uid'] = $uid;
        user_login_submit(array(), $form_state);
        drupal_goto();
      }
      else {
        
        // If fast registration is enabled and linking with existing profile
        // is disabled, the user is created automatically without email address.
        if (!variable_get('drconnect_link_option', 1) && variable_get('drconnect_fast_register', 0) == 1) {
          drupal_goto('drconnect/register');
        }
        
        // Ask user to create new site user or login with existing account.
        $output = '';
        $reg_msg = t('Create new account linked to Draugiem.lv profile');
        $options = array();
        if (variable_get('drconnect_fast_register', 0) == 1) {
          $output .= l($reg_msg, 'drconnect/register');
        }
        else {
          if (variable_get('user_register', 0) == 1 && variable_get('user_email_verification', 1) == 0) {
            $options = array(
              'query' => array(
                'destination' => 'drconnect/link',
              ),
            );
          }
          $output .= l($reg_msg, 'user/register', $options);
        }
        if (variable_get('drconnect_link_option', 1) == 1) {
          $output .= '</br>';
          $link_msg = t('Link an existing account to Draugiem.lv profile');
          $options = array(
            'query' => array(
              'destination' => 'drconnect/link',
            ),
          );
          $output .= l($link_msg, 'user/login', $options);
        }
        if ($output == '') {
          // Nor user registration, nor existing account linking is available.
          drupal_goto();
        }
        return $output;
      }
    }
    else {
      // User already logged in with his site account.
      global $user;
      drupal_goto('user/' . $user->uid . '/drconnect');
    }
    return 'BUJA';
  }
  else {
    watchdog('drconnect', 'Authorization page accessed without a valid Draugiem.lv session');
    drupal_goto();
  }
}
