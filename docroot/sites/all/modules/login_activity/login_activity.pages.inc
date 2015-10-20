<?php

/**
 * Page callback for viewing user-specific login activity.
 */
function login_activity_view_user_activity($uid = NULL) {
  $output = '';

  if (!$uid) {
    return FALSE;
  }

  global $user;
  if ($uid != $user->uid && !user_access('administer login activity')) {
    drupal_set_message(t('Access denied.'), 'error');
    
    //In most general case, a user without admin rights for login_activity will be a general site user. Therefore, the user's name (global $user->name) is the best page title for the purpose.
    drupal_set_title($user->name);
    return t("You don't have rights to get details of other users.");
  }

  if (!user_load($uid)) {
    drupal_set_message(t('User does not exist.'), 'error');
    drupal_set_title(t('Login Activity'));
    return t('This user does not exist. Make sure that you are providing the correct UID.');
  }

  $variables = array();

  $variables['header'] = array(
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
  $query->condition('l.uid', $uid, '=')
    ->fields('l', array('host_user_agent', 'hostname', 'timestamp'))
    ->range(0, 20)
    ->extend('TableSort')
    ->orderByHeader($variables['header']);

  $result = $query->execute();

  foreach ($result as $row) {
    $browser = _login_activity_get_browser($row->host_user_agent);
    $variables['rows'][] = array(
      'data' => array(
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
