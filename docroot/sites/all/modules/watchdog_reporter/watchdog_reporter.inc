<?php

/**
 * @file
 * Watchdog reporter module inc file.
 */

/**
 * Cron callback.
 *
 * @param integer $id
 */
function _watchdog_reporter_cron_job($id) {

  $presets = variable_get('watchdog_reporter_preset_list', array());
  $query = db_select('watchdog', 'w')->fields('w', array('wid', 'message', 'variables'));

  if (isset($presets[$id]['#values']['watchdog_reporter_severity'])) {
    $query->condition('severity', $presets[$id]['#values']['watchdog_reporter_severity'], 'IN');
  }

  if (isset($presets[$id]['#values']['watchdog_reporter_type'])) {
    $types = variable_get('watchdog_reporter_dblog_types');
    foreach ($presets[$id]['#values']['watchdog_reporter_type'] as $value) {
      $type[] = $types[$value];
    }
    $query->condition('type', $type, 'IN');
  }

  if (isset($presets[$id]['#values']['watchdog_reporter_roles']) || isset($presets[$id]['#values']['watchdog_reporter_users'])) {
    $db_or = db_or();

    if (isset($presets[$id]['#values']['watchdog_reporter_roles'])) {
      $query->join('users_roles', 'ur', 'w.uid = ur.uid');
      $db_or->condition('ur.rid', $presets[$id]['#values']['watchdog_reporter_roles'], 'IN');
    }

    if (isset($presets[$id]['#values']['watchdog_reporter_users'])) {
      $query->join('users', 'u', 'w.uid = u.uid');
      $db_or->condition('u.name', explode(',', $presets[$id]['#values']['watchdog_reporter_users']), 'IN');
    }

    $query->condition($db_or);
  }

  $query->condition('w.timestamp', $presets[$id]['#last_run'], '>');

  $presets[$id]['#last_run'] = time();
  variable_set('watchdog_reporter_preset_list', $presets);

  if ($presets[$id]['#values']['watchdog_reporter_group'] == WATCHDOG_REPORTER_GROUP_SQL) {
    $query->groupBy('w.message');
    $query->addExpression('COUNT(message)', 'count');
    $query->orderBy('count', 'DESC');
    $result = $query->execute()->fetchAllAssoc('message');
  }
  else {
    $result = $query->execute()->fetchAllAssoc('wid');
  }

  if ($presets[$id]['#values']['watchdog_reporter_group'] == WATCHDOG_REPORTER_GROUP_PHP) {
    $result = _watchdog_reporter_group_result($result);
    arsort($result);
  }

  if (!empty($result)) {
    drupal_mail('watchdog_reporter', 'watchdog_reporter_mail', $presets[$id]['#values']['watchdog_reporter_email'], language_default(), array('result' => $result, 'preset' => $presets[$id], 'preset_id' => $id));
  }
}

/**
 * Group results.
 *
 * @param object $result
 */
function _watchdog_reporter_group_result($result) {

  $counts = array();
  foreach ($result as $key => $value) {
    $vars = unserialize($value->variables);
    _watchdog_reporter_replace_tokens($result[$key]->message, $vars, $counts);
  }
  return $counts;
}

/**
 * Replace tokens.
 *
 * @param string $message
 * @param array $vars
 */
function _watchdog_reporter_replace_tokens(&$message, $vars, &$counts) {

  foreach ($vars as $key => $value) {
    $message = str_replace($key, $value, $message);
  }
  if (isset($counts[$message])) {
    $counts[$message]->count++;
  }
  else {
    $counts[$message] = new stdClass();
    $counts[$message]->count = 1;
  }
}

/**
 * Implements hook_mail().
 */
function watchdog_reporter_mail($key, &$message, $params) {

  if ($key == 'watchdog_reporter_mail') {
    $message['subject'] = t('Watchdog reporter from @site_name', array('@site_name' => variable_get('site_name', 'The site')));

    /*if (module_enable(array('mimemail'))) {
      $message['body'][] = theme('watchdog_reporter_html', $params);
    }
    else {
      $message['body'][] = theme('watchdog_reporter_plain', $params);
    }*/
    $message['body'][] = theme('watchdog_reporter_plain', $params);
  }
}