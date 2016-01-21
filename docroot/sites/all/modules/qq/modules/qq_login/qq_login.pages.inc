<?php

/**
 * @file
 * User page callbacks for the qq_login module.
 */

/**
 * qq login response.
 */
function qq_login_response() {
  // CSRF protection.
  $_SESSION['state'] = md5(uniqid(rand(), TRUE));
  $login_url = "https://graph.qq.com/oauth2.0/authorize?response_type=code&client_id="
    . variable_get('appid', '222222') . "&redirect_uri=" . urlencode(variable_get('callback', url(NULL, array('absolute' => TRUE)) . 'qq_login/callback'))
    . "&state=" . $_SESSION['state']
    . "&scope=" . variable_get('scope', 'get_user_info,add_share,list_album,add_album,upload_pic,add_topic,add_one_blog,add_weibo');

  drupal_goto($login_url);
}

/**
 * qq callback page.
 */
function qq_login_callback_page() {
  // CSRF.
  if ($_REQUEST['state'] == $_SESSION['state']) {
    $token_url = "https://graph.qq.com/oauth2.0/token?grant_type=authorization_code&"
      . "client_id=" . variable_get('appid', '222222') . "&redirect_uri=" . urlencode(variable_get('callback', url(NULL, array('absolute' => TRUE)) . 'qq_login/callback'))
      . "&client_secret=" . variable_get('appkey', '005831692a444765a0db25a4a5ac052c') . "&code=" . $_REQUEST["code"];

    $response = drupal_http_request($token_url);
    if (!isset($response->error) && $response->code == 200) {
      // Set access token to session.
      $access_token = explode('&', $response->data);
      $access_token = explode('=', $access_token[0]);
      $_SESSION['access_token'] = $access_token[1];

      if ($_SESSION['access_token']) {
        // Get openid.
        $graph_url = "https://graph.qq.com/oauth2.0/me?access_token=" . $_SESSION['access_token'];
        $response = drupal_http_request($graph_url);
        if (!isset($response->error) && $response->code == 200) {
          $lpos = strpos($response->data, "(");
          $rpos = strrpos($response->data, ")");
          $response->data = drupal_substr($response->data, ($lpos + 1), ($rpos - $lpos - 1));
          $account = json_decode($response->data);

          if (isset($account->error)) {
            drupal_set_message(t('<h3>Error</h3>: %error <h3>Msg</h3>: %msg .', array('%error' => $account->error, '%msg' => $account->error_description)), 'error');
            drupal_goto('user');
          }

          $_SESSION['openid']  = $account->openid;

          // Find an existing user.
          $result = db_select('qq_login')
            ->fields('qq_login')
            ->condition('openid', $_SESSION['openid'])
            ->execute()
            ->fetchObject();

          if (isset($result->uid) && $result->uid > 0) {
            $form_state['uid'] = $result->uid;
            user_login_submit(array(), $form_state);

            drupal_goto('user');
          }
          else {
            drupal_goto('qq_login/bind/new');
          }
        }
        else {
          drupal_set_message($response->error, 'error');
          drupal_goto('user');
        }
      }
    }
    else {
      drupal_set_message($response->error, 'error');
      drupal_goto('user');
    }
  }
  else {
    drupal_set_message(t('The state does not match. You may be a victim of CSRF.'), 'error');
    drupal_goto('user');
  }
}
