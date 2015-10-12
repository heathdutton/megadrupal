<?php
/**
 * @file
 * Report table interface.
 */

/**
 * Page callback: Displays a tracker table report form.
 */
function db_tracker_table() {
  // Get array of field's names of db_track table.
  $header = array();
  foreach (drupal_schema_fields_sql('db_track') as $column) {
    $header[] = $column;
  }

  // Get array of changes.
  $changes = db_select('db_track', 'n');
  $changes->fields('n', $header);
  $changes = $changes->extend('TableSort')->extend('PagerDefault')->limit(25);
  $result = $changes->execute();

  $rows = array();

  $uids = db_select('db_track', 'n')
    ->fields('n', array('user'))
    ->execute()
    ->fetchCol();

  $users = user_load_multiple(array_unique($uids));

  foreach ($result as $change) {
    $change_time = format_date(
      $change->change_time,
      'custom',
      t('H:i (j M Y)', array(), array('context' => 'php date format'))
    );

    $rows[] = array(
      $change->change_id,
      $change_time,
      $change->url,
      l(filter_xss($users[$change->user]->name), 'user/' . $change->user),
      $change->form_id,
      $change->field_name,
      filter_xss($change->start_value),
      filter_xss($change->end_value),
    );
  }

  $output[] = array(
    'drop_track_table' => array(
      '#type' => 'item',
      '#markup' => l(t('Drop Track table'), 'admin/reports/db_track/drop_track', array('attributes' => array('class' => 'button'))),
    ),
    'tracker_table' => array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#attributes' => array('class' => array('tracker_table')),
      '#empty' => t('No changes yet'),
      '#prefix' => '<div id="tracker_table">',
      '#suffix' => '</div>',
    ),
  );

  return $output;
}
