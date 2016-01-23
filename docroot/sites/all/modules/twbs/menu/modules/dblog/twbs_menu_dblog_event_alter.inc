<?php

/**
 * Page callback: Displays details about a specific database log message.
 *
 * @param int $id
 *   Unique ID of the database log message.
 *
 * @return array|string
 *   If the ID is located in the Database Logging table, a build array in the
 *   format expected by drupal_render(); otherwise, an empty string.
 *
 * @see dblog_menu()
 */
function twbs_menu_dblog_event_alter($id) {
  $severity = watchdog_severity_levels();
  $result = db_query('SELECT w.*, u.name, u.uid FROM {watchdog} w INNER JOIN {users} u ON w.uid = u.uid WHERE w.wid = :id', array(':id' => $id))->fetchObject();
  if ($dblog = $result) {
    $rows = array(
      array(
        array('data' => t('Type'), 'header' => TRUE),
        t($dblog->type),
      ),
      array(
        array('data' => t('Date'), 'header' => TRUE),
        format_date($dblog->timestamp, 'long'),
      ),
      array(
        array('data' => t('User'), 'header' => TRUE),
        theme('username', array('account' => $dblog)),
      ),
      array(
        array('data' => t('Location'), 'header' => TRUE),
        l($dblog->location, $dblog->location),
      ),
      array(
        array('data' => t('Referrer'), 'header' => TRUE),
        l($dblog->referer, $dblog->referer),
      ),
      array(
        array('data' => t('Message'), 'header' => TRUE),
        theme('dblog_message', array('event' => $dblog)),
      ),
      array(
        array('data' => t('Severity'), 'header' => TRUE),
        $severity[$dblog->severity],
      ),
      array(
        array('data' => t('Hostname'), 'header' => TRUE),
        check_plain($dblog->hostname),
      ),
      array(
        array('data' => t('Operations'), 'header' => TRUE),
        $dblog->link,
      ),
    );
    $build['dblog_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#prefix' => '<div class="table-responsive">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('dblog-event')),
    );
    return $build;
  }
  else {
    return '';
  }
}
