<?php

/**
 * @file
 * Admin page callbacks for the sql_views module.
 */

/**
 * Displays list of database views.
 */
function sql_views_overview_page() {

  $database = sql_views_database_name();

  $views = array();
  switch (db_driver()) {

    case 'mysql':
      $result = db_query("SHOW FULL TABLES IN $database WHERE TABLE_TYPE LIKE 'VIEW'")->fetchALL();
      foreach ($result as $record) {
        $views[] = reset($record);
      }
      break;

    default;
      $message = t('Sorry %driver driver is not supported yet.', array('%driver' => db_driver()));
      drupal_set_message($message, 'warning');
      return MENU_NOT_FOUND;
  }

  $header = array(
    '#',
    t('Name'),
    t('Records'),
    t('Time, ms'),
  );

  $rows = array();
  foreach ($views as $index => $view) {
    timer_start('sql_views_view_query');
    $records = db_query("SELECT COUNT(*) FROM $view")->fetchField();
    $time = timer_read('sql_views_view_query');
    $rows[] = array(
      $index + 1,
      l($view, "admin/reports/sql_views/$view"),
      l($records, "admin/reports/sql_views/$view/records"),
      $time,
    );
  }

  $build['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No results were found.'),
  );

  return $build;
}

/**
 * Show view definition.
 */
function sql_views_view_definition_page($view) {

  switch (db_driver()) {
    case 'mysql':
      $info = db_query("SHOW CREATE VIEW $view")->fetchAll();
      break;

    default;
      $message = t('Sorry %driver driver is not supported yet.', array('%driver' => db_driver()));
      drupal_set_message($message, 'warning');
      return MENU_NOT_FOUND;
  }

  drupal_set_title(t('Definition of the !view view', array('!view' => $view)));

  $header = array_keys((array) $info[0]);
  $rows[] = array_values((array) $info[0]);

  $build['definition'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}


/**
 * Show view description.
 */
function sql_views_view_description_page($view) {

  switch (db_driver()) {
    case 'mysql':
      $info = db_query("DESCRIBE $view")->fetchAll();
      break;

    default;
      $message = t('Sorry %driver driver is not supported yet.', array('%driver' => db_driver()));
      drupal_set_message($message, 'warning');
      return MENU_NOT_FOUND;
  }

  drupal_set_title(t('Description of the @view view', array('@view' => $view)));

  $header = array_keys((array) $info[0]);

  $rows = array();
  foreach ($info as $row) {
    $rows[] = array_values((array) $row);
  }

  $build['describe_info'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}

/**
 * Show all records for the view.
 */
function sql_views_view_records_page($view) {

  drupal_set_title(t('Records of the !view view', array('!view' => $view)));

  $result = db_select($view, 'v')
    ->fields('v')
    ->extend('PagerDefault')
    ->limit(20)
    ->execute();

  $rows = $header = array();
  foreach ($result as $row) {
    $rows[] = (array) $row;
  }

  if (isset($row)) {
    $header = array_keys((array) $row);
  }

  $build['records'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#empty' => t('No results were found.'),
    '#header' => $header,
  );

  $build['pager'] = array('#markup' => theme('pager'));

  return $build;
}
