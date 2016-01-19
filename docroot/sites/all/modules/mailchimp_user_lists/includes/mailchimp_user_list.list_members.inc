<?php
/**
 * @file
 * Mailchimp list of members.
 */

/**
 * Wrapper function to generate table and form for member list.
 *
 * @return string
 *   html table and form
 */
function mailchimp_user_list_list_members_page($lid) {
  $page = mailchimp_user_list_list_members_table($lid);
  $form = drupal_get_form('mailchimp_user_list_list_members_table_form', $lid);
  $page .= drupal_render($form);
  return $page;
}


/**
 * Wrapper function to generate table and form for member list.
 *
 * @return string
 *   html table and form
 */
function mailchimp_user_list_list_members_table($lid) {
  $results = mailchimp_user_list_export_members($lid);
  $table = theme('table', $results);
  return $table;
}

/**
 * Generate download button for member list.
 *
 * @return array
 *   form array
 */
function mailchimp_user_list_list_members_table_form($form, &$form_state, $lid) {
  $form['mailchimp_download_members'] = array(
    '#type' => 'submit',
    '#value' => t('Download Member List'),
    '#submit' => array('mailchimp_user_list_list_members_csv'),
  );
  $form['mailchimp_lid'] = array(
    '#type' => 'hidden',
    '#value' => $lid,
  );
  return $form;
}

/**
 * CSV download wrapper for form button.
 */
function mailchimp_user_list_list_members_csv_wrapper($form, &$form_state) {
  mailchimp_user_list_list_members_csv($form_state['values']['mailchimp_lid']);
}

/**
 * Function to generate csv download.
 */
function mailchimp_user_list_list_members_csv($form, &$form_state) {
  $results = mailchimp_user_list_export_members($form_state['values']['mailchimp_lid']);
  drupal_add_http_header('Content-type', 'text/csv');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=\'output.csv\'');
  $output = '';
  $output .= '"' . implode('","', $results['header']) . "\"\n";
  foreach ($results['rows'] as $row) {
    $output .= '"' . implode('","', $row) . "\"\n";
  }
  print $output;
  exit;
}
/**
 * Function to retrieve List members with details.
 *
 * Generic API does not allow for details on mailing lists.
 * Mailchimp Export API required.
 *
 * @return array
 *   array container header array and rows array
 */
function mailchimp_user_list_export_members($lid) {
  global $user;
  $apikey = variable_get('mailchimp_api_key', '');
  $api_server = substr($apikey, -3);
  $chunk_size = 4096;
  $url = 'http://' . $api_server . '.api.mailchimp.com/export/1.0/list?apikey=' . $apikey . '&id=' . $lid;
  $handle = @fopen($url, 'r');
  if (!$handle) {
    drupal_set_message(t('There was a problem retrieving the members on this list, please contact your system administrator.'));
    watchdog('mailchimp',
      'Could not retrieve list members for @user on lists id @list_id',
      array(
        '@user' => $user->name,
        '@list_id' => $lid,
      ),
      WATCHDOG_ERROR
    );
  }
  else {
    $i = 0;
    $header = array();
    while (!feof($handle)) {
      $buffer = fgets($handle, $chunk_size);
      if (trim($buffer) != '') {
        $obj = json_decode($buffer, TRUE);
        if ($i == 0) {
          // Store the header row.
          // TODO make the columns available configurable by admin.
          $header = array($obj[0],
            $obj[1],
            $obj[2],
          );
        }
        else {
          $rows[] = array(
            $obj[0],
            $obj[1],
            $obj[2],
          );
        }
        $i++;
      }
    }
    fclose($handle);
  }
  if (!is_array($rows)) {
    $rows = array('', '', '');
  }
  usort($rows, 'mailchimp_user_list_my_sorter');
  $output = array(
    'header' => $header,
    'rows' => $rows,
  );
  return $output;
}

/**
 * Function to sort members on list.
 */
function mailchimp_user_list_my_sorter($a, $b) {
  return strcmp($a[2] . $a[1], $b[2] . $b[1]);
}
