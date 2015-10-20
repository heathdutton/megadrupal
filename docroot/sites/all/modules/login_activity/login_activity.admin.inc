<?php

/**
 * Page callback for generating site-wide users login activity report for admins.
 */
function login_activity_admin_report() {
  $output = '';

  $variables = array();

  $variables['header'] = array(
    array(
      'data' => t('UID'),
      'field' => 'uid',
    ),
    array(
      'data' => t('User'),
    ),
    array(
      'data' => t('Date & Time'),
      'field' => 'timestamp',
      'sort' => 'desc',
    ),
    array(
      'data' => t('IP Address'),
      'field' => 'hostname',
    ),
    t('Browser'),
    t('Platform'),
    t('Mobile Device'),
    t('HTTP User Agent'),
  );

  $query = db_select('login_activity', 'l');
  $query->fields('l', array('uid', 'host_user_agent', 'hostname', 'timestamp'))
    ->range(0, 20)
    ->extend('TableSort')
    ->orderByHeader($variables['header']);

  $result = $query->execute();

  foreach ($result as $row) {
    $browser = _login_activity_get_browser($row->host_user_agent);
    $variables['rows'][] = array(
      'data' => array(
        check_plain($row->uid),
        l(user_load($row->uid)->name, 'user/' . $row->uid),
        format_date($row->timestamp, 'long'),
        check_plain($row->hostname),
        $browser->browser,
        $browser->platform,
        ($browser->ismobiledevice) ? t('Yes') : t('No'),
        check_plain($row->host_user_agent),
      ),
    );
  }

  $output = theme('table', $variables);

  return $output;
}
