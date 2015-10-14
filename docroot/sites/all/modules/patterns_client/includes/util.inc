<?php

/**
 * @file
 * Provides utils for the own module or for developers.
 */

/**
 * Display information regarding current
 *
 * Uses _drupal_set_message_ to display links to the configuration page.
 *
 * @return bool|array
 *   Associative array with url of server and credentials, or FALSE if they
 *   are not set correctly.
 */
function patterns_client_check_credentials_and_display_warns($verbose = TRUE, $pclient = FALSE) {

  $out = array(
    'url' => FALSE,
    'id' => FALSE,
    'credentials' => FALSE,
    'friend' => FALSE,
    'd2d_id' => FALSE,
    'name' => FALSE,
    'description' => FALSE,
    'public_key' => FALSE,
    'ready' => FALSE,
  );

  // Get the current server.
  $url = variable_get('patterns_client_server');

  if (empty($url)) {
    if ($verbose) {
      if (!$pclient) {
        $url_msg = t('You are not registered to any Patterns server. Check the Patterns Client !configuration.', array(
          '!configuration' => l(t('configuration'), 'admin/patterns/share'),
        ));
      }
      else {
        $url_msg = t('You are not registered to any Patterns server.');
      }
      drupal_set_message($url_msg, 'warning');
    }
    return $out;
  }
  else {
    $out['url'] = $url;
    $instances = d2d_api_instance_get();

    // Determine if Patterns Server is a registered instance and friend.
    foreach ($instances as $i) {
      if ($i['is_me'] === 1) {
        // Falsy.
        continue;
      }
      if ($i['url'] === $url) {
        $out['id'] = $i['id'];
        $out['name'] = $i['name'];
        $out['description'] = $i['description'];
        $out['d2d_id'] = $i['d2d_id'];
        $out['public_key'] = $i['public_key'];
        $out['friend'] = $i['is_friend'];
        break;
      }
    }

    if (empty($out['id']) || empty($out['friend'])) {
      if (!$pclient) {
        if ($verbose) {
          $pserver_link = l(t('Patterns server'), $url, array(
             'attributes' => array('target'=>'_blank'),
          ));
          $friend_msg = t('You are currently not friend with !server. Check the Patterns Client !configuration.', array(
            '!server' => $pserver_link,
            '!configuration' => l(t('configuration'), 'admin/patterns/share'),
          ));
        }
        else {
          $friend_msg = t('You are currently not friend with !server.', array(
            '!server' => $pserver_link,
          ));
        }
        drupal_set_message($friend_msg, 'warning');
      }
    }
    else {
      $credentials = patterns_client_get_full_credentials($url);

      if (empty($credentials)) {
        if ($verbose) {
          $pserver_link = l(t('Patterns server'), $url, array(
            'attributes' => array('target'=>'_blank'),
          ));

          if (!$pclient) {
            $credentials_msg = t('You have not registered a username on the !server yet. Check the Patterns Client !configuration.', array(
              '!server' => $pserver_link,
              '!configuration' => l(t('configuration'), 'admin/patterns/share'),
            ));
          }
          else {
            $credentials_msg = t('You have not registered a username on !server yet.' , array(
              '!server' => $pserver_link,
            ));
          }
          drupal_set_message($credentials_msg, 'warning');
        }
      }
      else {
        $out['credentials'] = $credentials;
        $out['ready'] = TRUE;
      }

    }
  }
  return $out;
}