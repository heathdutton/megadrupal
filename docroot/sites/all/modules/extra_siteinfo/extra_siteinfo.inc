<?php
/**
 * @file
 * Included file for the extra_siteinfo module.
 */

/**
 * Function returns currently loggedin users data on the page.
 *
 * @return array
 *   A form array of currently loggedin users.
 */
function extra_siteinfo_currently_loggedin_users_page() {
  global $user;
  $header = array(t('User Id'), t('User Name'), t('Action'));
  $rows = array();
  $query = db_select('sessions', 's');
  $query->join('users', 'u', 's.uid = u.uid');
  $query->fields('s', array('uid'))
    ->fields('u', array('name'));
  $query->orderBy('uid', 'DESC');
  $query->distinct();
  $result = $query->execute();
  $inc = 0;
  while ($record = $result->fetchAssoc()) {
    $end_session = "";
    if ($user->uid != $record['uid']) {
      $end_session = l(t('End Session'), 'admin/reports/extra-siteinfo/currently-loggedin-users/end-session/' . $record['uid']);
    }
    $rows['extra_siteinfo_users' . $inc] = array(
      $record['uid'],
      l($record['name'], 'user/' . $record['uid']),
      $end_session,
    );
    $inc = $inc + 1;
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Function handles the confirmation form for user end session.
 */
function extra_siteinfo_currently_loggedin_users_page_end_session($form, &$form_state, $uid) {
  $form_state['user_id'] = $uid;
  return confirm_form(
    $form,
    'End Session of Currenlty loggedin Users.',
    'admin/reports/extra-siteinfo/currently-loggedin-users',
    'Are you sure to end this particular users session?',
    'End Session'
  );
}

/**
 * Function ends the session of selected user.
 */
function extra_siteinfo_currently_loggedin_users_page_end_session_submit($form_id, &$form_state) {
  $user = user_load($form_state['user_id']);
  db_delete('sessions')
    ->condition('uid', $user->uid)
    ->execute();
  drupal_set_message(t('@username ( @userid ) user session has been ended.',
    array(
      '@username' => $user->name,
      '@userid' => $user->uid,
    )
  ));
  drupal_goto('admin/reports/extra-siteinfo/currently-loggedin-users');
}

/**
 * Function filters content and users based on the selection made in arg().
 */
function extra_siteinfo_filters() {
  $filter = array(array(arg(2), arg(3)));
  if (arg(1) == 'nodes') {
    if (arg(2) == 'all') {
      $_SESSION['node_overview_filter'] = array();
    }
    else {
      $_SESSION['node_overview_filter'] = $filter;
    }
    drupal_goto('admin/content');
  }
  elseif (arg(1) == 'users') {
    if (arg(2) == 'all') {
      $_SESSION['user_overview_filter'] = array();
    }
    else {
      $_SESSION['user_overview_filter'] = $filter;
    }
    drupal_goto('admin/people');
  }
  else {
    drupal_goto('admin/reports/extra-siteinfo');
  }
}
