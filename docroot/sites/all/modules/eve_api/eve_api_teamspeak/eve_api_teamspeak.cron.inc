<?php
/**
 * @file
 * Functions related to controlling and adding cron jobs.
 */

/**
 * Implements hook_cronapi().
 */
function eve_api_teamspeak_cronapi($op, $job = NULL) {
  if ($op == 'list') {
    $items = array();

    $items['eve_api_teamspeak_cron_role_sync_job'] = array(
      'description' => 'Sync TeamSpeak Permissions with Drupal roles.',
      'rule' => '*/5 * * * *',
    );

    $items['eve_api_teamspeak_cron_user_check_job'] = array(
      'description' => 'Check each user on TeamSpeak to ensure the Unique ID matches the character.',
      'rule' => '* * * * *',
    );

    return $items;
  }
  elseif ($op == 'execute') {
    if (!variable_get('eve_api_enable_cron', TRUE)) {
      return;
    }

    switch ($job) {
      case 'eve_api_teamspeak_cron_role_sync_job':
        if (variable_get('eve_api_enable', FALSE) && variable_get('eve_api_teamspeak_enable', FALSE)) {
          $queue = DrupalQueue::get('eve_api_teamspeak_cron_role_sync');
          $queue->createItem(1);
        }
        break;

      case 'eve_api_teamspeak_cron_user_check_job':
        if (variable_get('eve_api_enable', FALSE) && variable_get('eve_api_teamspeak_enable', FALSE)) {
          $queue = DrupalQueue::get('eve_api_teamspeak_cron_user_check');
          $queue->createItem(1);
        }
        break;
    }
  }
}

/**
 * Implements hook_cron().
 */
function eve_api_teamspeak_cron() {
  if (!variable_get('eve_api_enable_cron', TRUE) || function_exists('elysia_cron_run')) {
    return;
  }

  if (variable_get('eve_api_enable', FALSE) && variable_get('eve_api_teamspeak_enable', FALSE)) {
    if (time() >= variable_get('eve_api_teamspeak_cron_role_next', 0)) {
      $queue = DrupalQueue::get('eve_api_teamspeak_cron_role_sync');
      $queue->createItem(1);

      variable_set('eve_api_teamspeak_cron_role_next', time() + (5 * 60));
    }
  }

  if (variable_get('eve_api_enable', FALSE) && variable_get('eve_api_teamspeak_enable', FALSE)) {
    $queue = DrupalQueue::get('eve_api_teamspeak_cron_user_check');
    $queue->createItem(1);
  }
}

/**
 * Implements hook_cron_queue_info().
 */
function eve_api_teamspeak_cron_queue_info() {
  $queue = array();

  $queue['eve_api_teamspeak_cron_user_check'] = array(
    'worker callback' => 'eve_api_teamspeak_cron_user_check',
    'time' => 60,
  );

  $queue['eve_api_teamspeak_cron_role_sync'] = array(
    'worker callback' => 'eve_api_teamspeak_cron_role_sync',
    'time' => 60,
  );

  return $queue;
}

/**
 * Check each user on TeamSpeak to ensure the Unique ID matches the character.
 *
 * @param mixed $item
 *   whatever was added to the queue via queue->createItem()
 */
function eve_api_teamspeak_cron_user_check($item) {
  try {
    $ts3 = eve_api_teamspeak_connect();
    $teamspeak_user_list = eve_api_teamspeak_user_list($ts3);
    $valid_roles = $corp_info = array();
    $roles = user_roles();

    $result = db_query('SELECT corporationName, corporationTicker FROM {eve_api_alliance_list_corporations}');

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $corp_info[(string) $row->corporationName] = (string) $row->corporationTicker;
      }
    }

    foreach ($roles as $rid => $rname) {
      if ($rid == 1 || $rid == 2) {
        continue;
      }

      foreach ($corp_info as $key => $value) {
        if ($key == $rname) {
          $rname = $value;
        }
      }

      $valid_roles[$rname] = $rid;
    }

    foreach ($teamspeak_user_list as $teamspeak_user) {
      $skip_check = FALSE;

      if ($teamspeak_user->client_type == 1) {
        continue;
      }

      if (in_array(variable_get('eve_api_teamspeak_bypass_group', 0), explode(',', (string) $teamspeak_user->client_servergroups))) {
        $skip_check = TRUE;
      }

      $teamspeak_uid = (string) $teamspeak_user->client_unique_identifier;
      $teamspeak_nickname = (string) $teamspeak_user->client_nickname;
      $teamspeak_dbid = (int) $teamspeak_user->client_database_id;

      $result = db_query('SELECT u.name, u.uid FROM {users} u WHERE u.characterID != 0 AND u.teamspeakUID = :teamspeakUID', array(
        ':teamspeakUID' => $teamspeak_uid,
      ));

      if (!$result->rowCount()) {
        $teamspeak_roles = eve_api_teamspeak_user_get_groups($ts3, $teamspeak_dbid);

        foreach ($teamspeak_roles as $rid => $rname) {
          if ((string) $rname['name'] != 'Guest' && isset($valid_roles[(string) $rname['name']])) {
            eve_api_teamspeak_user_remove_group($ts3, $rid, $teamspeak_dbid);
          }
        }

        if ($skip_check) {
          continue;
        }

        if (variable_get('eve_api_teamspeak_reg_message_enable', FALSE)) {
          $teamspeak_user->message('[color=red]' . t('Your TeamSpeak ID is not registered on our server.') . '[/color]');

          if (variable_get('eve_api_teamspeak_reg_kick_enable', FALSE) || variable_get('eve_api_teamspeak_reg_ban_enable', FALSE)) {
            $teamspeak_user->message('[color=red]' . t('If you do not register within @time minutes', array('@time' => variable_get('eve_api_teamspeak_reg_time', 10))) . '[/color]');
            $teamspeak_user->message('[color=red]' . t('you will be removed from the server.'));
          }

          $teamspeak_user->message('[color=red]' . t('To register, go to [url]@homepage[/url]', array('@homepage' => url('user/register', array('absolute' => TRUE)))) . '[/color]');
        }

        if (variable_get('eve_api_teamspeak_reg_poke_enable', FALSE)) {
          $message = t('Please register');

          if (variable_get('eve_api_teamspeak_reg_kick_enable', FALSE) || variable_get('eve_api_teamspeak_reg_ban_enable', FALSE)) {
            $message .= t(' within @time minutes or you will be removed from the server', array('@time' => variable_get('eve_api_teamspeak_reg_time', 10)));
          }

          $message .= '!';

          $teamspeak_user->poke($message);
          $teamspeak_user->poke(t('To register, go to @homepage', array('@homepage' => url('user/register', array('absolute' => TRUE)))));
        }

        if (variable_get('eve_api_teamspeak_reg_kick_enable', FALSE) && variable_get('eve_api_teamspeak_reg_ban_enable', FALSE) == FALSE) {
          if (time() >= ($teamspeak_user->client_lastconnected + (60 * variable_get('eve_api_teamspeak_reg_time', 10)))) {
            $teamspeak_user->kick(TeamSpeak3::KICK_SERVER, t('To register, go to @homepage', array('@homepage' => url('user/register', array('absolute' => TRUE)))));
          }
        }

        if (variable_get('eve_api_teamspeak_reg_ban_enable', FALSE)) {
          if (time() >= ($teamspeak_user->client_lastconnected + (60 * variable_get('eve_api_teamspeak_reg_time', 10)))) {
            $teamspeak_user->ban((60 * variable_get('eve_api_teamspeak_reg_ban_time', 10)), t('You should have registered at @homepage  You are banned for @time minutes.', array(
              '@homepage' => url('user/register', array('absolute' => TRUE)),
              '@time' => (variable_get('eve_api_teamspeak_reg_ban_time', 10)),
            )));
          }
        }

        continue;
      }

      $row = $result->fetchAssoc();

      $expected_teamspeak_nickname = eve_api_teamspeak_format_name($row['name']);

      $result = db_query('SELECT r.rid, r.name FROM {users_roles} ur INNER JOIN {role} r ON (r.rid = ur.rid) WHERE ur.uid = :uid', array(
        ':uid' => (int) $row['uid'],
      ));

      $user_roles = array();

      if ($result->rowCount()) {
        foreach ($result->fetchAll() as $row) {
          $rname = $row->name;

          foreach ($corp_info as $key => $value) {
            if ($key == $rname) {
              $rname = $value;
            }
          }

          $user_roles[$rname] = $row->rid;
          $teamspeakgid = eve_api_teamspeak_group_get_id($ts3, $rname);
          eve_api_teamspeak_user_add_group($ts3, $teamspeakgid, $teamspeak_dbid);
        }
      }

      $roles = eve_api_teamspeak_user_get_groups($ts3, $teamspeak_dbid);

      foreach ($roles as $rid => $rname) {
        if (!isset($user_roles[(string) $rname['name']]) && (string) $rname['name'] != 'Guest' && isset($valid_roles[(string) $rname['name']])) {
          eve_api_teamspeak_user_remove_group($ts3, $rid, $teamspeak_dbid);
        }
      }

      if ($teamspeak_nickname != $expected_teamspeak_nickname && !$skip_check) {
        if (variable_get('eve_api_teamspeak_user_message_enable', FALSE)) {
          $teamspeak_user->message('[color=red]' . t('Your nickname is incorrect.') . '[/color]');

          if (variable_get('eve_api_teamspeak_user_kick_enable', FALSE) || variable_get('eve_api_teamspeak_user_ban_enable', FALSE)) {
            $teamspeak_user->message('[color=red]' . t('If you do not fix your nickname within @time minutes', array('@time' => variable_get('eve_api_teamspeak_user_time', 10))) . '[/color]');
            $teamspeak_user->message('[color=red]' . t('you will be removed from the server.'));
          }

          $teamspeak_user->message('[color=red]' . t('Nickname expected: !nickname', array('!nickname' => $expected_teamspeak_nickname)) . '[/color]');
        }

        if (variable_get('eve_api_teamspeak_user_poke_enable', FALSE)) {
          $message = t('Your nickname is incorrect');

          if (variable_get('eve_api_teamspeak_user_kick_enable', FALSE) || variable_get('eve_api_teamspeak_user_ban_enable', FALSE)) {
            $message .= t(', within @time minutes or you will be removed from the server', array('@time' => variable_get('eve_api_teamspeak_user_time', 10)));
          }

          $message .= '!';

          $teamspeak_user->poke($message);
          $teamspeak_user->poke(t('Nickname expected: !nickname', array('!nickname' => $expected_teamspeak_nickname)));
        }

        if (variable_get('eve_api_teamspeak_user_kick_enable', FALSE) && variable_get('eve_api_teamspeak_user_ban_enable', FALSE) == FALSE) {
          if (time() >= ($teamspeak_user->client_lastconnected + (60 * variable_get('eve_api_teamspeak_user_time', 10)))) {
            $teamspeak_user->kick(TeamSpeak3::KICK_SERVER, t('You should have fixed your nickname!'));
          }
        }

        if (variable_get('eve_api_teamspeak_user_ban_enable', FALSE)) {
          if (time() >= ($teamspeak_user->client_lastconnected + (60 * variable_get('eve_api_teamspeak_user_time', 10)))) {
            $teamspeak_user->ban((60 * variable_get('eve_api_teamspeak_user_ban_time', 10)), t('You should have fixed your nickname, you are banned for @time minutes.', array(
              '@time' => (variable_get('eve_api_teamspeak_user_ban_time', 10)),
            )));
          }
        }
      }
    }

    eve_api_teamspeak_disconnect($ts3);
  }
  catch (EveApiException $e) {
    if ($item == 2) {
      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api_teamspeak', 'Cron Check TeamSpeak Names task has failed 2 times in a row, task was removed. Message: ' . $e->getMessage(), NULL, WATCHDOG_ERROR);
      }

      return FALSE;
    }

    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api_teamspeak', 'Cron Check TeamSpeak Names task has failed. Message: ' . $e->getMessage(), NULL, WATCHDOG_ERROR);
    }

    $item = $item + 1;
    $queue = DrupalQueue::get('eve_api_teamspeak_cron_user_check');
    $queue->createItem($item);

    return FALSE;
  }

  return TRUE;
}

/**
 * Sync TeamSpeak Permissions with Drupal roles.
 *
 * @param mixed $item
 *   whatever was added to the queue via queue->createItem()
 */
function eve_api_teamspeak_cron_role_sync($item) {
  try {
    $roles = user_roles();
    $ts3 = eve_api_teamspeak_connect();
    $corp_info = array();

    $result = db_query('SELECT corporationName, corporationTicker FROM {eve_api_alliance_list_corporations}');

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $corp_info[$row->corporationName] = $row->corporationTicker;
      }
    }

    foreach ($roles as $rid => $rname) {
      if ($rid == 1 || $rid == 2) {
        continue;
      }

      foreach ($corp_info as $key => $value) {
        if ($key == $rname) {
          $rname = $value;
        }
      }

      if (eve_api_teamspeak_group_get_id($ts3, $rname)) {
        continue;
      }

      eve_api_teamspeak_group_create($ts3, $rname);
    }

    eve_api_teamspeak_disconnect($ts3);
  }
  catch (EveApiException $e) {
    if ($item == 2) {
      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api_teamspeak', 'Cron Sync TeamSpeak Roles task has failed 2 times in a row, task was removed. Message: ' . $e->getMessage(), NULL, WATCHDOG_ERROR);
      }

      return FALSE;
    }

    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api_teamspeak', 'Cron Sync TeamSpeak Roles task has failed. Message: ' . $e->getMessage(), NULL, WATCHDOG_ERROR);
    }

    $item = $item + 1;
    $queue = DrupalQueue::get('eve_api_teamspeak_cron_role_sync');
    $queue->createItem($item);

    return FALSE;
  }

  return TRUE;
}
