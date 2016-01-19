<?php

/**
 * Page callback: Displays a listing of database log messages.
 *
 * Messages are truncated at 56 chars. Full-length messages can be viewed on the
 * message details page.
 *
 * @see dblog_clear_log_form()
 * @see dblog_event()
 * @see dblog_filter_form()
 * @see dblog_menu()
 *
 * @ingroup logging_severity_levels
 */
function twbs_menu_dblog_overview_alter() {
  module_load_include('inc', 'dblog', 'dblog.admin');

  $filter = dblog_build_filter_query();
  $rows = array();
  $classes = array(
    WATCHDOG_DEBUG     => '',
    WATCHDOG_INFO      => '',
    WATCHDOG_NOTICE    => '',
    WATCHDOG_WARNING   => 'warning',
    WATCHDOG_ERROR     => 'danger',
    WATCHDOG_CRITICAL  => 'danger',
    WATCHDOG_ALERT     => 'danger',
    WATCHDOG_EMERGENCY => 'danger',
  );
  $icons = array(
    WATCHDOG_DEBUG     => '<i class="fa fa-fw fa-bug"></i>',
    WATCHDOG_INFO      => '<i class="fa fa-fw fa-info"></i>',
    WATCHDOG_NOTICE    => '&nbsp;',
    WATCHDOG_WARNING   => '<i class="fa fa-fw fa-warning"></i>',
    WATCHDOG_ERROR     => '<i class="fa fa-fw fa-ban"></i>',
    WATCHDOG_CRITICAL  => '<i class="fa fa-fw fa-ban"></i>',
    WATCHDOG_ALERT     => '<i class="fa fa-fw fa-ban"></i>',
    WATCHDOG_EMERGENCY => '<i class="fa fa-fw fa-ban"></i>',
  );

  $build['dblog_filter_form'] = drupal_get_form('dblog_filter_form');
  $build['dblog_filter_form']['filters']['status']['type'] += array(
    '#prefix' => '<div class="row"><div class="col-md-6">',
    '#suffix' => '</div>',
  );
  $build['dblog_filter_form']['filters']['status']['severity'] += array(
    '#prefix' => '<div class="col-md-6">',
    '#suffix' => '</div></div>',
  );
  $build['dblog_clear_log_form'] = drupal_get_form('dblog_clear_log_form');

  $header = array(
    '&nbsp;', // Icon column.
    array('data' => t('Type'), 'field' => 'w.type'),
    array('data' => t('Date'), 'field' => 'w.wid', 'sort' => 'desc'),
    t('Message'),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Operations')),
  );

  $query = db_select('watchdog', 'w')->extend('PagerDefault')->extend('TableSort');
  $query->leftJoin('users', 'u', 'w.uid = u.uid');
  $query
    ->fields('w', array('wid', 'uid', 'severity', 'type', 'timestamp', 'message', 'variables', 'link'))
    ->addField('u', 'name');
  if (!empty($filter['where'])) {
    $query->where($filter['where'], $filter['args']);
  }
  $result = $query
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  foreach ($result as $dblog) {
    $rows[] = array('data' =>
      array(
        // Cells
        $icons[$dblog->severity],
        t($dblog->type),
        format_date($dblog->timestamp, 'short'),
        theme('dblog_message', array('event' => $dblog, 'link' => TRUE)),
        theme('username', array('account' => $dblog)),
        filter_xss($dblog->link),
      ),
      // Attributes for tr
      'class' => array(drupal_html_class('dblog-' . $dblog->type), $classes[$dblog->severity]),
    );
  }

  $build['dblog_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'admin-dblog'),
    '#empty' => t('No log messages available.'),
    '#prefix' => '<div class="table-responsive">',
    '#suffix' => '</div>',
  );
  $build['dblog_pager'] = array('#theme' => 'pager');

  return $build;
}
