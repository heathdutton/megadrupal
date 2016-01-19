<?php

function bounce_handler_admin_settings() {
  $form['bounce_handler_email_soft_limit'] = array(
    '#type' => 'select',
    '#title' => t('Disable mail sending (soft bounces).'),
    '#default_value' => variable_get('bounce_handler_email_soft_limit', 'Disabled'),
    '#options' => drupal_map_assoc(array('Disabled', 0, 5, 10, 20, 50, 100)),
    '#description' => t('Disable sending outgoing mail to accounts that have more soft bounces than the chosen limit. A warning will be written to the watchdog. Choose "Disabled" to disable this feature.'),
  );
  $form['bounce_handler_email_hard_limit'] = array(
    '#type' => 'select',
    '#title' => t('Disable mail sending (hard bounces).'),
    '#default_value' => variable_get('bounce_handler_email_hard_limit', 'Disabled'),
    '#options' => drupal_map_assoc(array('Disabled', 0, 5, 10, 20, 50, 100)),
    '#description' => t('Disable sending outgoing mail to accounts that have more hard bounces than the chosen limit. A warning will be written to the watchdog. Choose "Disabled" to disable this feature.'),
  );
  $form['bounce_handler_unsubscribe_soft_limit'] = array(
    '#type' => 'select',
    '#title' => t('Disable subscriptions (soft bounces).'),
    '#default_value' => variable_get('bounce_handler_unsubscribe_soft_limit', 'Disabled'),
    '#options' => drupal_map_assoc(array('Disabled', 0, 5, 10, 20, 50, 100, 200)),
    '#description' => t('Try to unsubscribe accounts from outgoing mail to email addresses that have more soft bounces than the chosen limit. Choose 0 to disable this feature. This depends on the mail sending module having a unsubscribe feature. Refer to the README for supported modules. (Requires a correctly configured <a href="@cron">cron maintenance task</a>.)', array('@cron' => url('admin/reports/status'))),
  );
  $form['bounce_handler_unsubscribe_hard_limit'] = array(
    '#type' => 'select',
    '#title' => t('Disable subscriptions (hard bounces).'),
    '#default_value' => variable_get('bounce_handler_unsubscribe_hard_limit', 'Disabled'),
    '#options' => drupal_map_assoc(array('Disabled', 0, 5, 10, 20, 50, 100, 200)),
    '#description' => t('Try to unsubscribe accounts from outgoing mail to email addresses that have more hard bounces than the chosen limit. Choose 0 to disable this feature. This depends on the mail sending module having a unsubscribe feature. Refer to the README for supported modules. (Requires a correctly configured <a href="@cron">cron maintenance task</a>.)', array('@cron' => url('admin/reports/status'))),
  );
  $form['bounce_handler_row_limit'] = array(
    '#type' => 'select',
    '#title' => t('Discard bounce entries above the following row limit'),
    '#default_value' => variable_get('bounce_handler_row_limit', 1000),
    '#options' => drupal_map_assoc(array(100, 1000, 10000, 100000, 1000000)),
    '#description' => t('The maximum number of rows to keep in the bounce log database. Older entries will be automatically discarded. (Requires a correctly configured <a href="@cron">cron maintenance task</a>.)', array('@cron' => url('admin/reports/status')))
  );

  return system_settings_form($form);
}

function bounce_handler_overview() {
  $severity = bounce_handler_severity_levels();
  $categories = bounce_handler_categories();
  $types = bounce_handler_bounce_types();

  $header = array(
    array('data' => t('Type'), 'field' => 'b.bmh_type'),
    array('data' => t('Category'), 'field' => 'b.bmh_cat'),
    array('data' => t('Date'), 'field' => 'b.bid', 'sort' => 'desc'),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Operations')),
  );

  $query = db_select('bounces', 'b')->extend('PagerDefault')->extend('TableSort');
  $query->leftJoin('users', 'u', 'b.uid = u.uid');
  $query
    ->fields('b', array('bid', 'sid', 'uid', 'severity', 'bmh_type', 'bmh_cat', 'timestamp', 'message'))

    ->addField('u', 'name');
  if (!empty($filter['where'])) {
    $query->where($filter['where'], $filter['args']);
  }

 $result = $query
    ->limit(50)
    ->orderByHeader($header)
    ->execute();


  $rows = array();
  foreach ($result as $bounce) {
    $rows[] = array('data' =>
                    array(
                      // Cells
                      $types[$bounce->bmh_type],
                      $categories[$bounce->bmh_cat],
                      format_date($bounce->timestamp, 'short'),
                      theme('username', array('account' => $bounce)),
                      l(t('See bounce'), 'admin/reports/bounce/'. $bounce->bid) .' '. l(t('See all bounces'), 'admin/reports/bounce_mail/'. $bounce->sid),
                    ),
                    // Attributes for tr
                    'class' => array(drupal_html_class('dblog-' . $bounce->bmh_type)),
    );
  }

  $build['bounce_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'admin-bounces'),
    '#empty' => t('No bounce messages available.'),
  );
  $build['bounce_pager'] = array('#theme' => 'pager');

  return $build;
}

function bounce_handler_top() {
  $header = array(
    array('data' => t('Mail')),
    array('data' => t('Count (Total)'), 'field' => 'bounce_count',),
    array('data' => t('Hard bounces'), 'field' => 'hard_bounces', 'sort' => 'desc'),
    array('data' => t('Last bounce'), 'field' => 'b.sid'),
    array('data' => t('User')),
    array('data' => t('Operations')),
  );

  $query = db_select('bounce_statistics', 'bs')->extend('PagerDefault')->extend('TableSort');
  $query->join('users', 'u', 'bs.mail = u.mail');
  $query
    ->fields('bs', array('sid', 'count', 'remove', 'mail', 'timestamp'))
    ->addField('u', 'name');
  $query->addExpression('SUM(bs.count)', 'bounce_count');
  $query->addExpression('SUM(bs.remove)', 'hard_bounces');
  $query->groupBy('bs.sid');
  $result = $query
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  $rows = array();
  foreach ($result as $bounce) {
    $rows[] = array(
      'data' =>
      array(
        check_plain($bounce->mail),
        $bounce->bounce_count,
        $bounce->hard_bounces,
        format_date($bounce->timestamp, 'small'),
        theme('username', array('account' => $bounce)),
        l(t('See bounces'), 'admin/reports/bounce_mail/'. $bounce->sid),
      ),
      // Attributes for tr
      'class' => array(drupal_html_class('bounce-top')),
    );
  }

  $build['bounce_top_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'admin-bounce-top'),
    '#empty' => t('No bounce messages available.'),
  );
  $build['bounce_top_pager'] = array('#theme' => 'pager');

  return $build;
}

function bounce_handler_view_bounce($bid) {
  $severity = bounce_handler_severity_levels();
  $categories = bounce_handler_categories();
  $types = bounce_handler_bounce_types();
  $output = '';

  $result = db_query('SELECT b.*, u.name, u.uid FROM {bounces} b LEFT JOIN {users} u ON b.uid = u.uid WHERE b.bid = :bid', array(':bid' => $bid))->fetchObject();
  if ($bounce = $result) {
    $rows = array(
      array(
        array('data' => t('Destination'), 'header' => TRUE),
        check_plain($bounce->destination),
      ),
      array(
        array('data' => t('Date'), 'header' => TRUE),
        format_date($bounce->timestamp, 'large'),
      ),
      array(
        array('data' => t('User'), 'header' => TRUE),
        theme('username', array('account' => $bounce)),
      ),
      array(
        array('data' => t('BMH No.'), 'header' => TRUE),
        $bounce->bmh_no,
      ),
      array(
        array('data' => t('Category'), 'header' => TRUE),
        $categories[$bounce->bmh_cat],
      ),
      array(
        array('data' => t('Type'), 'header' => TRUE),
        $types[$bounce->bmh_type],
      ),
      array(
        array('data' => t('Severity'), 'header' => TRUE),
        $severity[$bounce->severity],
      ),
      array(
        array('data' => t('Message'), 'header' => TRUE),
        _bounce_handler_format_message($bounce->message),
      ),
      array(
        array('data' => t('Operations'), 'header' => TRUE),
        l(t('See all bounces'), 'admin/reports/bounce_mail/'. $bounce->sid),
      ),
    );
    $build['bounce_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#attributes' => array('class' => array('bounce-event')),
    );
    return $build;
  }
}

/**
 * Menu callback
 *
 * Shows the bounces triggered by sending to a particular email address,
 * identify by the associated ID.
 */
function bounce_handler_view_bounce_address($sid) {
  $severity = bounce_handler_severity_levels();
  $categories = bounce_handler_categories();
  $types = bounce_handler_bounce_types();
  $output = '';

  $header = array(
    array('data' => t('Address')),
    array('data' => t('Bounce category')),
    array('data' => t('Bounce type')),
    array('data' => t('Severity')),
    array('data' => t('User')),
    array('data' => t('Operations')),
  );

  $result = db_query('SELECT b.*, u.name, u.uid FROM {bounces} b LEFT JOIN {users} u ON b.uid = u.uid WHERE b.sid = :sid ORDER BY b.bid DESC', array(':sid' => $sid));

  $rows = array();
  foreach ($result as $bounce) {
    $rows[] = array(
      'data' =>
      array(
        check_plain($bounce->destination),
        $categories[$bounce->bmh_cat],
        $types[$bounce->bmh_type],
        $severity[$bounce->severity],
        theme('username', array('account' => $bounce)),
        l(t('See bounce'), 'admin/reports/bounce/'. $bounce->bid)
      ),
      // Attributes for tr
      'class' => array(drupal_html_class('bounce-events-user')),
    );
  }
  $build['bounce_user_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('bounce-events-user')),
  );
  return $build;
}


function bounce_handler_categories() {
  require_once drupal_get_path('module', 'bounce_handler') .'/phpmailer-bmh_rules.php';
  global $rule_categories;
  return array_combine(array_keys($rule_categories), array_map('t', array_keys($rule_categories)));
}

function bounce_handler_severity_levels() {
  return array(0 => t("don't remove yet"), 1 => t('remove'));
}

function bounce_handler_bounce_types() {
  require_once drupal_get_path('module', 'bounce_handler') .'/phpmailer-bmh_rules.php';
  global $rule_categories;
  $types = array();
  foreach ($rule_categories as $category) {
    $types[$category['bounce_type']] = t($category['bounce_type']);
  }
  return $types;
}

function _bounce_handler_format_message($message) {
  return '<pre>' . filter_xss_admin($message) . '</pre>';
}
