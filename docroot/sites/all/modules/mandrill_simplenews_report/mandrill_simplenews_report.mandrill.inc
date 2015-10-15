<?php

/**
 * @file
 *   Class file that extends mandrill api class.
 */

class MandrillSimplenewsReport extends Mandrill {

  /**
   * Request mandrill to prepare report and import the same to local table
   */
  public function activity_report_import($date_from = NULL, $date_to = NULL) {
    // Part #1: Initiate request to export most recent report
    // (part #2: To save the same in local table
    // when request state changes from "waiting" to "completed").

    // By default request for last 24 hours report.
    if (!isset($data_from)) {
      $date_from = REQUEST_TIME - 86400;
    }
    if (!isset($date_to)) {
      $date_to = REQUEST_TIME;
    }

    // Request report per newsletter category, as the report returned is not
    // very structured. This helps us to see the total sent/open count by
    // newsletter for the instance, when same email address is subscribed
    // to more than one newsletter.
    $tids = db_query('SELECT tid, from_address FROM simplenews_category');
    foreach ($tids as $tid) {
      $_params = array(
        'date_from' => format_date($date_from, 'custom', 'Y-m-d H:i:s'),
        'date_to' => format_date($date_to, 'custom', 'Y-m-d H:i:s'),
        'senders' => array($tid->from_address),
        'states' => array('sent'),
        'tags' => array('simplenews_node'),
      );
      // Initiate report export request
      $new_request = $this->request('exports/activity.json', $_params);
      if (is_array($new_request) && empty($new_request['id'])) {
        $message = 'Unexpected response from Mandrill "exports/activity.json" api call';
        $message .= '<br/><pre>' . print_r($new_request, TRUE) . '</pre>';
        watchdog('mandrill_simplenews_report', $message, array(), WATCHDOG_ERROR);
        continue;
      }
      // Save the request id to queue table until it changes from
      // "waiting" to "completed" state.
      db_insert('mandrill_simplenews_report_newsletter_report')
        ->fields(array(
          'report_id' => $new_request['id'],
          'tid' => $tid->tid,
          'created' => strtotime($new_request['created_at']),
        ))
        ->execute();
    }
    // End of part #1.

    // Part #2: Check if activity.csv is ready for already requested reports.
    // Fetch the requets id from local table, download the report, if ready
    // import the same data to {mandrill_simplenews_report_newsletter_sent} table.
    $reports = db_query_range('SELECT report_id, tid
      FROM {mandrill_simplenews_report_newsletter_report}
      WHERE created < :one_hour
      ORDER BY created ASC', 0, 10, array(':one_hour' => REQUEST_TIME - 3600));
    $import_count = 0;
    foreach ($reports as $report) {
      if ($import_count >= 4) {
        // To prevent overloadding the server with too many imports.
        watchdog('mandrill_simplenews_report', 'Reports import job completed Successfully.');
        return;
      }
      $info = $this->request('exports/info.json', array('id' => $report->report_id));
      if (isset($info['state']) && $info['state'] != 'complete') {
        // Report is not ready yet.
        continue;
      }
      if ($info['state'] == 'complete' && !valid_url($info['result_url'])) {
        $message = 'Unexpected response from Mandrill "exports/info.json" api call';
        $message .= '<br/><pre>' . print_r($info, TRUE) . '</pre>';
        watchdog('mandrill_simplenews_report', $message, array(), WATCHDOG_ERROR);
        continue;
      }
      // Converts Remote URL to Drupal stream wrapper path
      $file_srp = system_retrieve_file($info['result_url']);
      // Get Drupal stream wrapper path to local file path
      $local_file = drupal_realpath($file_srp);
      $za = new ZipArchive();
      $res = $za->open($local_file);
      if ($res != TRUE) {
        $message = 'ZipArchive is unable to open file (local path) !local_file
        (file stream url !file_srp, mandrill export url !result_url)';
        $watchdog_variables = array(
          '!local_file' => $local_file,
          '!file_srp' => $file_srp,
          '!result_url' => $info['result_url'],
        );
        watchdog('mandrill_simplenews_report', $message, $watchdog_variables, WATCHDOG_ERROR);
        continue;
      }
      $extract_to = str_replace('.zip', '', $local_file);
      $za->extractTo($extract_to);
      $za->close();
      $csv_file = $extract_to . "/activity.csv";
      if (($handle = fopen($csv_file, "r")) == FALSE) {
        $message = 'Unable to open activity.csv file (local path) !local_file
        (file stream url !file_srp, mandrill export url !result_url, extracted to
        folder !extracted_to, csv file full path !csv_file)';
        $watchdog_variables = array(
          '!local_file' => $local_file,
          '!file_srp' => $file_srp,
          '!result_url' => $info['result_url'],
          '!extracted_to' => $extract_to,
          '!csv_file' => $csv_file,
        );
        watchdog('mandrill_simplenews_report', $message, $watchdog_variables, WATCHDOG_ERROR);
        continue;
      }
      $line = 0;
      while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
        if ($line != 0) {
          db_merge('mandrill_simplenews_report_newsletter_sent')
            ->key(array(
              'sent_timestamp' => strtotime($data[0]),
              'sent_to_mail' => $data[1],
              'tid' => $report->tid,
            ))
            ->fields(array(
              'sent_timestamp' => strtotime($data[0]),
              'sent_to_mail' => $data[1],
              'from_mail' => $data[2],
              'subject' => $data[3],
              'tid' => $report->tid,
              'open_count' => $data[7],
              'click_count' => $data[8],
            ))
            ->execute();
        }
        $line++;
      }
      fclose($handle);
      // Delete the report .zip file and extracted folder.
      file_unmanaged_delete_recursive($file_srp);
      file_unmanaged_delete_recursive($extract_to);
      db_delete('mandrill_simplenews_report_newsletter_report')
        ->condition('report_id', $report->report_id)
        ->execute();
      $import_count++;
    }
  }
}
