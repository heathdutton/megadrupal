<?php
/**
 * @file
 * Functions related to controlling and adding cron jobs.
 */

/**
 * Implements hook_cronapi().
 */
function eve_api_jabber_cronapi($op, $job = NULL) {
  if ($op == 'list') {
    $items = array();

    $items['eve_api_jabber_cron_user_sync_job'] = array(
      'description' => 'Sync Jabber Permissions with Drupal roles.',
      'rule' => '*/5 * * * *',
    );

    return $items;
  }
  elseif ($op == 'execute') {
    if (!variable_get('eve_api_enable_cron', TRUE)) {
      return;
    }

    switch ($job) {
      case 'eve_api_jabber_cron_user_sync_job':
        if (variable_get('eve_api_enable', FALSE) && variable_get('eve_api_jabber_enable', FALSE)) {
          $queue = DrupalQueue::get('eve_api_jabber_cron_user_sync');
          $result = db_query('SELECT uid, name FROM {users} WHERE characterID != 0');

          if ($result->rowCount()) {
            foreach ($result->fetchAll() as $item) {
              $queue->createItem(array(
                'uid' => $item->uid,
                'name' => $item->name,
                'runs' => 1,
              ));
            }
          }
        }
        break;
    }
  }
}

/**
 * Implements hook_cron().
 */
function eve_api_jabber_cron() {
  if (!variable_get('eve_api_enable_cron', TRUE) || function_exists('elysia_cron_run')) {
    return;
  }

  if (variable_get('eve_api_enable', FALSE) && variable_get('eve_api_jabber_enable', FALSE)) {
    if (time() >= variable_get('eve_api_jabber_cron_user_next', 0)) {
      $queue = DrupalQueue::get('eve_api_jabber_cron_user_sync');
      $result = db_query('SELECT uid, name FROM {users} WHERE characterID != 0');

      if ($result->rowCount()) {
        foreach ($result->fetchAll() as $item) {
          $queue->createItem(array(
            'uid' => $item->uid,
            'name' => $item->name,
            'runs' => 1,
          ));
        }
      }

      variable_set('eve_api_jabber_cron_user_next', time() + (5 * 60));
    }
  }
}

/**
 * Implements hook_cron_queue_info().
 */
function eve_api_jabber_cron_queue_info() {
  $queue = array();
  $queue['eve_api_jabber_cron_user_sync'] = array(
    'worker callback' => 'eve_api_jabber_cron_user_sync',
    'time' => 15,
  );
  return $queue;
}

/**
 * Update user permissions in Jabber based on roles.
 *
 * @param mixed $item
 *   whatever was added to the queue via queue->createItem()
 */
function eve_api_jabber_cron_user_sync($item) {
  try {
    $uid = (int) $item['uid'];
    $character_name = (string) $item['name'];

    $result = db_query('SELECT r.rid, r.name FROM {users_roles} ur INNER JOIN {role} r ON (r.rid = ur.rid) WHERE ur.uid = :uid', array(
      ':uid' => $uid,
    ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        if ($row->rid == variable_get('eve_api_unverified_role', 2)) {
          $result = eve_api_jabber_url_query('delete', $character_name);
          return TRUE;
        }
      }
    }
    else {
      $result = eve_api_jabber_url_query('delete', $character_name);
    }
  }
  catch (EveApiException $e) {
    if ($item['runs'] == 2) {
      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api_jabber', 'Cron Update Jabber User Roles task has failed 2 times in a row, task was removed. Data: ' . drupal_http_build_query($item) . ' Message: ' . $e->getMessage(), NULL, WATCHDOG_ERROR);
      }

      return FALSE;
    }

    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api_jabber', 'Cron Update Jabber User Roles task has failed. Data: ' . drupal_http_build_query($item) . ' Message: ' . $e->getMessage(), NULL, WATCHDOG_ERROR);
    }

    $item['runs'] = $item['runs'] + 1;
    $queue = DrupalQueue::get('eve_api_jabber_cron_user_sync');
    $queue->createItem($item);

    return FALSE;
  }

  return TRUE;
}
