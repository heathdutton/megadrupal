<?php

/**
 * @file
 * Page callbacks for the role_activity module.
 */

/**
 * Reviews role activity log.
 */
function role_activity_log() {
  $header = array(
    array(
      'data' => t('Timestamp'),
      'field' => 'timestamp',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Action'),
      'field' => 'action',
    ),
    array(
      'data' => t('User'),
      'field' => 'uid',
    ),
    array('data' => t('Activity')),
    array(
      'data' => t('Path'),
      'field' => 'uri',
    ),
    array(
      'data' => t('IP/Host'),
      'field' => 'ip',
    ),
    array(
      'data' => t('Type'),
      'field' => 'type',
    ),
    array(
      'data' => t('Message'),
      'field' => 'message',
    ),
    array(
      'data' => t('Referrer'),
      'field' => 'referer',
    ),
    array('data' => t('Link')),
  );

  $results = db_select('role_activity', 'r')
    ->extend('TableSort')
    ->fields('r')
    ->orderByHeader($header)
    ->extend('PagerDefault')
    ->limit(50)
    ->execute();

  $rows = array();
  foreach ($results as $log) {
    $host = $log->ip;
    $account = user_load($log->uid);

    $rows[] = array(
      array(
        'data' => format_date($log->timestamp, 'short'),
        'nowrap' => 'nowrap',
      ),
      array('data' => $log->action),
      array('data' => theme('username', array('account' => $account))),
      array('data' => l(t('Activity'), 'admin/reports/role_activity/user/' . $account->uid)),
      array('data' => $log->uri),
      l($host, 'http://whois.domaintools.com/' . $log->ip),
      array('data' => $log->type),
      array('data' => $log->message),
      _role_activity_format_path($log->referer),
      $log->link,
    );
  }

  $build['activity'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No activity available.'),
  );
  $build['activity_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Lists top users by activity on the site.
 */
function role_activity_top_users() {
  $header = array(
    array(
      'data' => t('User'),
      'field' => 'uid',
    ),
    array('data' => t('Activity')),
    array(
      'data' => t('Number of events'),
      'field' => 'count',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Last'),
      'field' => 'last',
    ),
  );

  $query = db_select('role_activity', 'r')
    ->fields('r')
    ->groupBy('uid');

  $query->addExpression('COUNT(uid)', 'count');
  $query->addExpression('MAX(timestamp)', 'last');

  $query = $query->extend('TableSort')->orderByHeader($header);
  $query = $query->extend('PagerDefault')->limit(50);

  $results = $query->execute();

  $rows = array();
  foreach ($results as $log) {
    $account = user_load($log->uid);

    $rows[] = array(
      array('data' => theme('username', array('account' => $account))),
      array('data' => l(t('Activity'), 'admin/reports/role_activity/user/' . $account->uid)),
      array(
        'data' => $log->count,
        'align' => 'right',
      ),
      array(
        'data' => format_date($log->last, 'short'),
        'nowrap' => 'nowrap',
      ),
    );
  }

  $build['activity'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No activity available.'),
  );
  $build['activity_pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Displays per-user activity data on the site.
 */
function role_activity_by_user($account) {
  $header = array(
    array(
      'data' => t('Timestamp'),
      'field' => 'timestamp',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Action'),
      'field' => 'action',
    ),
    array(
      'data' => t('Path'),
      'field' => 'uri',
    ),
    array(
      'data' => t('IP/Host'),
      'field' => 'ip',
    ),
    array(
      'data' => t('Type'),
      'field' => 'type',
    ),
    array(
      'data' => t('Message'),
      'field' => 'message',
    ),
    array(
      'data' => t('Referrer'),
      'field' => 'referer',
    ),
    array('data' => t('Link')),
  );

  $results = db_select('role_activity', 'r')
    ->extend('TableSort')
    ->fields('r')
    ->condition('uid', $account->uid)
    ->orderByHeader($header)
    ->extend('PagerDefault')
    ->limit(50)
    ->execute();

  $rows = array();
  foreach ($results as $log) {
    $host = $log->ip;

    $rows[] = array(
      array(
        'data' => format_date($log->timestamp, 'short'),
        'nowrap' => 'nowrap',
      ),
      array('data' => $log->action),
      array('data' => $log->uri),
      l($host, 'http://whois.domaintools.com/' . $log->ip),
      array('data' => $log->type),
      array('data' => $log->message),
      _role_activity_format_path($log->referer),
      $log->link,
    );
  }

  $build['user_activity'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No activity available.'),
    '#caption' => t('Details for !user', array('!user' => theme('username', array('account' => $account)))),
  );
  $build['user_activity_pager'] = array('#theme' => 'pager');
  return $build;
}
