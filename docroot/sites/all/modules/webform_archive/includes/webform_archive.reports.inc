<?php
/**
 * @file
 * Archive Webforms Reports.
 */

/**
 * Page Callback function.
 */
function webform_archive_report_page() {
  $title = t('Archived Webforms');
  drupal_set_title($title);
  $reports = webform_archive_get_reports();
  $output = drupal_render($reports);
  return $output;
}

/**
 * Gives the webform archive reports table.
 */
function webform_archive_get_reports() {
  // Table header.
  $header = array(
    array(
      'data' => t('NID'),
      'field' => 'w.nid',
    ),
    array(
      'data' => t('Title'),
      'field' => 'w.title',
    ),
    array(
      'data' => t('Status'),
      'field' => 'w.status',
    ),
    array(
      'data' => t('Archived till'),
      'field' => 'w.archived',
    ),
    array(
      'data' => t('Download'),
      'fields' => 'w.download',
    ),
  );
  // Get all webform archieved data.
  $result = webform_archive_report_table($header);
  $rows = array();
  if (!empty($result)) {
    foreach ($result as $data) {
      $rows[] = array(
        'data' => array(
          l($data->nid, 'node/' . $data->nid),
          webform_archive_webform_title($data->nid),
          webform_archive_webform_status($data->nid),
          date('Y-m-d', $data->archived),
          l(t('Download'), 'node/' . $data->nid . '/webform-results/download-reports'),
        ),
      );
    }
  }

  // Construct the call for the theme function to run on this.
  $output['webform_archive_table'] = array(
    '#theme'  => 'table',
    '#header' => $header,
    '#rows'   => $rows,
    '#empty'  => t('No Webforms Archived.'),
  );

  // Adds the pager buttons to the bottom of the table.
  $output['webform_archive_table_pager'] = array('#theme' => 'pager');

  return $output;
}

/**
 * Gives data for archieved webforms.
 *
 * @param array $header
 *   Header for archived webform reports table.
 *
 * @return array
 *   Archived webforms.
 */
function webform_archive_report_table($header) {
  $query = db_select('archive_webform', 'w')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('w')
    ->limit(30)
    ->orderByHeader($header);
  $result = $query->execute();

  return $result;
}

/**
 * Gives Webform title.
 *
 * @param int $nid
 *   NID of the webform.
 *
 * @return string
 *   Title of the webform.
 */
function webform_archive_webform_title($nid) {
  $title = db_select('node', 'n')
  ->fields('n', array('title'))
  ->condition('n.nid', $nid)
  ->execute();
  $title = $title->fetchAssoc();
  return $title['title'];
}

/**
 * Webform Status for the given NID.
 *
 * @param int $nid
 *   NID of the webform.
 *
 * @return string
 *   Status of the webform.
 */
function webform_archive_webform_status($nid) {
  $output = '';
  $status = db_select('webform', 'w')
  ->fields('w', array('status'))
  ->condition('w.nid', $nid)
  ->execute();
  $status = $status->fetchAssoc();
  $output = ($status['status'] == 0) ? t('Closed') : t('Active');
  return $output;
}
