<?php

/**
 * @file
 * Wunderlist CRUD functions.
 */

/**
 * Save your Wunderlist list.
 *
 * @param array $list
 *   An array with the data to save.
 * @param Wunderlist $oauth2_client
 *   The oauth2 client.
 */
function wunderlist_list_save(array $list, Wunderlist $oauth2_client) {
  foreach ($list as $item) {
    $item['public'] = (int) $item['public'];
    $item['created_at'] = strtotime($item['created_at']);
    try {
      db_merge('wunderlist_list')
        ->key(array('id' => $item['id']))
        ->fields($item)
        ->execute();
      $webhook = wunderlist_get_webhook($item['id']);
      // There is a limit in the amount of webhooks per list,
      // so we only create one for this website.
      if (empty($webhook)) {
        $webhook_id = $oauth2_client->createWebhook($item['id']);
        db_insert('wunderlist_webhook')
          ->fields(array(
            'id' => $webhook_id,
            'list_id' => $item['id'],
          ))->execute();
      }
      $tasks = $oauth2_client->getTasks($item['id']);
      foreach ($tasks as $task) {
        wunderlist_task_save($task);
        wunderlist_user_save($task['created_by_id'], $task['avatar']);
      }
    }
    catch (Exception $e) {
      watchdog_exception('wunderlist', $e);
    }
  }
}

/**
 * Save a Wunderlist task.
 *
 * @param array $task
 *   The task data.
 */
function wunderlist_task_save(array $task) {
  try {
    $task['completed'] = (int) $task['completed'];
    $task['created_at'] = strtotime($task['created_at']);
    if (isset($task['due_date'])) {
      $task['due_date'] = strtotime($task['due_date']);
    }
    if (isset($task['assignee_id'])) {
      $task['assignee_id'] = (int) $task['assignee_id'];
    }
    $task['created_by_id'] = (int) $task['created_by_id'];
    $task['list_id'] = (int) $task['list_id'];
    $task['revision'] = (int) $task['revision'];
    $task['starred'] = (int) $task['starred'];
    unset($task['avatar']);
    db_merge('wunderlist_task')
      ->key(array('id' => $task['id']))
      ->fields($task)
      ->execute();
  }
  catch (Exception $e) {
    watchdog_exception('wunderlist', $e);
  }
}

/**
 * Save a Wunderlist user.
 *
 * @param int $user_id
 *   The id of the user.
 * @param string $avatar
 *   Base64 string.
 */
function wunderlist_user_save($user_id, $avatar) {
  try {
    db_merge('wunderlist_user')
      ->key(array('id' => $user_id))
      ->fields(array(
        'id' => $user_id,
        'avatar' => $avatar,
      ))
      ->execute();
  }
  catch (Exception $e) {
    watchdog_exception('wunderlist', $e);
  }
}

/**
 * Returns a webhook for a list item.
 *
 * @param int $list_id
 *   The list id.
 *
 * @return array
 *   The webhook id and list id in an array.
 */
function wunderlist_get_webhook($list_id) {
  $webhook = db_query('SELECT id, list_id FROM {wunderlist_webhook}
    WHERE list_id = :list_id', array(':list_id' => $list_id))->fetchAll();
  return reset($webhook);
}

/**
 * Returns all saved webhooks.
 *
 * Return array
 *   Webhooks.
 */
function wunderlist_get_webhooks() {
  return db_query('SELECT id, list_id FROM {wunderlist_webhook}')->fetchAll();
}
