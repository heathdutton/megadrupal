<?php
/**
 * @file
 * Administrative pages for User expire module.
 */

/**
 * View a list of all users with an expiration date.
 */
function user_expire_report() {
  $header = array(
    'username' => array(
      'data' => t('Username'),
      'field' => 'u.name',
    ),
    'access' => array(
      'data' => t('Last access'),
      'field' => 'u.access',
    ),
    'expiration' => array(
      'data' => t('Expiration'),
      'field' => 'expiration',
      'sort' => 'asc',
    ),
  );
  $rows = array();

  $query = db_select('user_expire', 'ue');
  $query->join('users', 'u', 'ue.uid = u.uid');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('u', array('uid', 'name', 'access'))
    ->fields('ue', array('expiration'))
    ->limit(50)
    ->orderByHeader($header);

  $accounts = $query->execute();

  foreach ($accounts as $account) {
    $rows[$account->uid] = array(
      'username' => theme('username', array('account' => $account)),
      'access' => $account->access ? t('@time ago', array('@time' => format_interval(REQUEST_TIME - $account->access))) : t('never'),
      'expiration' => t('@time from now', array('@time' => format_interval($account->expiration - REQUEST_TIME))),
    );
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
  ));
}
