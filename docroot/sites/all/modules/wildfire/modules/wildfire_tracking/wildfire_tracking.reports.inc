<?php
/**
 * @file
 *  wildfire_tracking.reports.inc
 *
 * Reporting and statistics functionality for Wildfire.
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * Code derived from Wildfire 1:
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 */

/**
 * Page callback to display a report for a particular send job.
 *
 * @param int $jid
 *    The job ID of the job for which the report is to be displayed.
 */
function wildfire_reports_job_form($form, &$form_state, $jid = 0) {
  module_load_include('inc', 'wildfire_jobs', 'wildfire_jobs.send');
  module_load_include('inc', 'wildfire_jobs', 'wildfire_jobs.api');

  // Parameter check.

  $job = new WildfireClientJob($jid);
  // Job failed to load, redirect to the list page.
  if (!isset($job->jid)) {
    drupal_set_message(
      t(
        'Cannot load report as Job #!jid is invalid',
        array(
          '!jid' => $jid
        )
      ),
      'status'
    );
    drupal_goto('admin/wildfire/jobs');
  }

  /**
   * If the job has not yet been sent, the alias links have not been created yet
   * thus there is nothing to report upon. If this is the case, redirect back to
   * the list with a message
   */
  $status = $job->getStatus();
  if (
    $status == WILDFIRE_JOB_PENDING
    || $status == WILDFIRE_JOB_FAILED
    || $status == WILDFIRE_JOB_CANCELLED
  ) {
    drupal_set_message(
      t('A report cannot be generated for this Job. The job status indicates that it has not yet been processed'),
      'status'
    );
    drupal_goto('admin/wildfire/jobs');
  }


  module_load_include('class', 'wildfire_tracking', 'wildfire_tracking');
  module_load_include('module', 'wildfire_tracking', 'wildfire_tracking');

  /**
   * Opens fieldset.
   */
  $form['open_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Job Summary'),
    '#description' => t('The number of users who opened the email. If the user chose to show images, or clicked any of the links in the email, the user will show in this statistic. If the user did not choose to show images, and did not click on any of the links in the email, unfortunately, it is not possible to know whether the user has opened the email or not.'),
  );

  // Load the job and the open count.
  $opens = wildfire_tracking_get_opens($job->jid);

  /**
   * If the count is zero, just display a placeholder chart - the job won't
   * have started if it is zero, so the chart is pretty meaningless at that
   * point.
   */
  if ($job->count > 0) {

    $openpercent = $opens / $job->count * 100;
    $openpercent_display = round($openpercent, 2);

    $closepercent = 100 - $openpercent;
    $closepercent_display = 100 - $openpercent_display;

  }
  else {

    $openpercent = 0;
    $openpercent_display = 0;
    $closepercent = 100;
    $closepercent_display = 0;

  }

  $series = array(
    t('Opens') . ' (' . $openpercent_display . '%)' => $openpercent,
    t('Not tracked') . ' (' . $closepercent_display . '%)' => $closepercent,
  );

  $openchart = new WildfirePieChart('p3', 300, 150, $series);
  $form['open_fieldset']['open_chart'] = array(
    '#markup' => $openchart->render(t('Proportion of users who opened the email'), t('Proportion of users who opened the email'), array('id' => 'report-open-pie')),
  );

  $form['open_fieldset']['summary'] = wildfire_reports_job_summary($jid);

  /**
   * Links fieldset.
   */
  $form['links_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links'),
    '#description' => t('A list of all the links in the email that was sent out, along with how many people clicked on each link. You may click on the \'who\' link for each entry to view the individual users who clicked on each link.'),
  );

  // Create a table to contain all our links.
  $rows = array();
  $headers = array(
    t('URL'),
    t('Total clicks'),
    t('Unique clicks'),
    t('Last clicked'),
  );

  // Get all the links from the links table and go through each one, creating a
  // table row for each.
  $links_result = db_select('wildfire_links', 'cl')
    ->fields('cl', array(
    'linkid',
    'jid',
    'url'
  ))
    ->condition('jid', $jid, '=')
    ->execute();

  while ($dbrow = $links_result->fetchAssoc()) {
    $row = array();

    $row[] = check_url($dbrow['url']);

    // Total clicks.
    $query = db_select('wildfire_link_clicks', 'clc');
    $query->addExpression('COUNT(1)', 'count');
    $query->condition('linkid', $dbrow['linkid'], '=');
    $result = $query->execute();

    $total = $result->fetchField();

    // Add a link to see who clicked the link if there were more than zero
    // clicks.
    if ($total > 0) {
      $total .= ' (' . l(
        t('who?'),
        'admin/wildfire/report/link/' . $dbrow['linkid'],
        array('attributes' => array(
          'title' => t('See a list of the users who have clicked on this link and when'),
        ))
      ) . ')';
    }
    $row[] = $total;

    // Unique clicks.
    $query = db_select('wildfire_link_clicks', 'clc');
    $query->addExpression('COUNT(DISTINCT uid)', 'count');
    $query->condition('linkid', $dbrow['linkid'], '=');
    $result = $query->execute();

    $unique = $result->fetchField();

    $row[] = $unique === FALSE ? 0 : $unique;

    // Last clicked.
    $query = db_select('wildfire_link_clicks', 'clc');
    $query->addExpression('MAX(timestamp)', 'max');
    $query->condition('linkid', $dbrow['linkid'], '=');
    $query->groupBy('linkid');
    $result = $query->execute();

    $last = $result->fetchField();

    $row[] = $last === FALSE ? t('Never') : format_date($last, 'custom', WILDFIRE_DATETIME);

    $rows[] = array(
      'class' => array('linkid-' . $dbrow['linkid']),
      'data' => $row,
    );
  }

  // Deal with a situation where there are no rows in the table.
  if (count($rows) == 0) {
    $row = array(array(
      'colspan' => count($headers),
      'data' => t('There were no links in this email'),
    ));
    $rows[] = $row;
  }

  $form['links_fieldset']['links_table'] = array(
    '#markup' => theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'report-links-table'))),
  );

  return $form;
}

/**
 * Page callback to display a report for a particular link within a job.
 *
 * This will show the users who clicked on the link, and when.
 *
 * @param int $linkid
 *    The link ID of the link for which the report is to be displayed.
 */
function wildfire_reports_link_form($form, &$form_state, $linkid = 0) {
  module_load_include('module', 'wildfire_tracking', 'wildfire_tracking');

  // Parameter check.
  if (!wildfire_tracking_link_valid($linkid)) {
    drupal_goto('admin/wildfire');
  }

  $form = array();

  /**
   * Click summary fieldset.
   */
  $query = db_select('wildfire_link_clicks', 'clc');
  $query->condition('linkid', $linkid, '=');
  $query->addExpression('COUNT(1)', 'count');
  $result = $query->execute();

  $total = $result->fetchField();

  $query = db_select('wildfire_link_clicks', 'clc');
  $query->condition('linkid', $linkid, '=');
  $query->addExpression('COUNT(DISTINCT uid)', 'count');
  $result = $query->execute();

  $uniques = $result->fetchField();

  $form['click_summary_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Summary'),
    '#description' => t(
      'There @total on this link in total, by @unique.',
      array(
        '@total' => format_plural($total, 'has been one click', 'have been @count clicks'),
        '@unique' => format_plural($uniques, 'one user', '@count different users'),
      )
    ),
  );

  /**
   * Click table fieldset.
   */
  $form['click_table_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clicks'),
    '#description' => t('This table lists who clicked on each link and when. You may sort the list by clicking any of the column headers. Clicking the same header twice in a row will sort it in the opposite order.'),
  );

  $headers = array(
    array(
      'data' => t('User'),
      'field' => 'mail',
    ),
    array(
      'data' => t('Clicked'),
      'field' => 'timestamp',
      'sort' => 'desc',
    ),
  );

  $rows = array();

  $query = db_select('wildfire_link_clicks', 'clc')->extend('PagerDefault');
  $query->limit(100);
  $query->fields('clc', array(
    'uid',
    'linkid',
    'timestamp',
  ));
  $query->fields('cl', array(
    'url',
  ));
  $query->fields('u', array(
    'mail',
  ));
  $query->join('users', 'u', 'clc.uid = u.uid');
  $query->join('wildfire_links', 'cl', 'clc.linkid = cl.linkid');
  $query->condition('clc.linkid', $linkid, '=');
  $query->orderBy('timestamp', 'DESC');
  $result = $query->execute();

  while ($dbrow = $result->fetchAssoc()) {
    $row = array();

    if (!empty($dbrow['mail'])) {
      $row[] = $dbrow['mail'];
    }
    else {
      $row[] = t('Unknown / Deleted User');
    }
    $row[] = format_date($dbrow['timestamp'], 'custom', WILDFIRE_DATETIME);

    $rows[] = $row;
  }

  if (count($rows) == 0) {
    $rows = array(array(
      'colspan' => count($headers),
      'data' => t('There are no clicks for this link yet'),
    ));
  }

  $form['click_table_fieldset']['click_table'] = array(
    '#markup' => theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'link-click-report'))),
  );
  $form['click_table_fieldset']['click_pager'] = array(
    '#markup' => theme('pager', array('tags' => NULL)),
  );

  // Provide a link back to the job report that this link has come from.
  $query = db_select('wildfire_links', 'cl')
    ->fields('cl', array(
    'jid',
  ));
  $query->condition('linkid', $linkid, '=');
  $result = $query->execute();

  $jid = $result->fetchField();


  $form['return_to_job_report'] = array(
    '#markup' => l(
      t('‹ Return to the job report page'),
      'admin/wildfire/report/job/' . $jid . '/view'
    ),
  );

  return $form;
}

function wildfire_reports_open_list($form, &$form_state, $jid = 0) {

  module_load_include('module', 'wildfire_tracking', 'wildfire_tracking');

  // Parameter check.
  $job = new WildfireClientJob($jid);
  // Job failed to load, redirect to the list page.
  if (!isset($job->jid)) {
    drupal_set_message(
      t(
        'Cannot load report as Job #!jid is invalid',
        array(
          '!jid' => $jid
        )
      ),
      'status'
    );
    drupal_goto('admin/wildfire/jobs');
  }

  /**
   * If the job has not yet been sent, the alias links have not been created yet
   * thus there is nothing to report upon. If this is the case, redirect back to
   * the list with a message
   */
  $status = $job->getStatus();
  if (
    $status == WILDFIRE_JOB_PENDING
    || $status == WILDFIRE_JOB_FAILED
    || $status == WILDFIRE_JOB_CANCELLED
  ) {
    drupal_set_message(
      t('A report cannot be generated for this Job. The job status indicates that it has not yet been processed'),
      'status'
    );
    drupal_goto('admin/wildfire/jobs');
  }

  // Draw chart.
  $form[] = wildfire_reports_draw_open_graph($form, $form_state, $jid);

  // Get the open list using paging.
  $query = db_select('users', 'u')->extend('PagerDefault');
  $query->limit(100);;

  $query->fields('u', array(
    'mail',
  ));

  $query->fields('ct', array(
    'timestamp',
  ));
  $query->join('wildfire_tracking', 'ct', 'ct.uid = u.uid');
  $query->condition('ct.jid', $jid, '=');
  $query->orderBy('timestamp', 'DESC');

  $result = $query->execute();

  $rows = array();
  $headers = array(
    t('User email'),
    t('Timestamp'),
  );

  $row = array();

  while ($dbrow = $result->fetchAssoc()) {

    $row = array();

    if (!empty($dbrow['mail'])) {
      $row[] = $dbrow['mail'];
    }
    else {
      $row[] = t('Unknown / Deleted User');
    }


    $row[] = format_date($dbrow['timestamp'], 'custom', WILDFIRE_DATETIME);

    $rows[] = $row;

  }

  if (count($rows) == 0) {
    $row = array(array(
      'colspan' => count($headers),
      'data' => t('Nobody has opened email.'),
    ));
    $rows[] = $row;
  }

  // Populate the array to show the open list.
  $form['open_table_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Open List'),
    '#description' => t('A list of all the users who opened the email.'),
  );

  $form['open_table_fieldset']['open_table'] = array(
    '#markup' => theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'link-click-report'))),
  );
  $form['open_table_fieldset']['open_pager'] = array(
    '#markup' => theme('pager', array('tags' => NULL)),
  );

  return $form;
}

function wildfire_reports_draw_open_graph($form, &$form_state, $jid = 0) {

  module_load_include('module', 'wildfire_tracking', 'wildfire_tracking');

  // Parameter check.
  $job = new WildfireClientJob($jid);
  // Job failed to load, redirect to the list page.
  if (!isset($job->jid)) {
    drupal_set_message(
      t(
        'Cannot load report as Job #!jid is invalid',
        array(
          '!jid' => $jid
        )
      ),
      'status'
    );
    drupal_goto('admin/wildfire/jobs');
  }

  /**
   * If the job has not yet been sent, the alias links have not been created yet
   * thus there is nothing to report upon. If this is the case, redirect back to
   * the list with a message
   */
  $status = $job->getStatus();
  if (
    $status == WILDFIRE_JOB_PENDING
    || $status == WILDFIRE_JOB_FAILED
    || $status == WILDFIRE_JOB_CANCELLED
  ) {
    drupal_set_message(
      t('A report cannot be generated for this Job. The job status indicates that it has not yet been processed'),
      'status'
    );
    drupal_goto('admin/wildfire/jobs');
  }

  // Get the total users
  $total_users = $job->count;

  // Get the open list from database.
  $query = db_select('users', 'u');

  $query->fields('u', array(
    'mail',
  ));

  $query->fields('ct', array(
    'timestamp',
  ));
  $query->join('wildfire_tracking', 'ct', 'ct.uid = u.uid');
  $query->condition('ct.jid', $jid, '=');
  $query->orderBy('timestamp', 'DESC');

  $result = $query->execute();

  $users_per_timestamp = array();

  // Traverse through each open detail.
  while ($dbrow = $result->fetchAssoc()) {

    $row = array();

    if (!empty($dbrow['mail'])) {
      $row[] = $dbrow['mail'];
    }
    else {
      $row[] = t('Unknown / Deleted User');
    }

    $timestamp = format_date($dbrow['timestamp'], 'custom', WILDFIRE_DATE);
    $percentageOfUsers = 1;
    if (array_key_exists($timestamp, $users_per_timestamp)) {
      $percentageOfUsers = $users_per_timestamp[$timestamp] + 1;
    }
    $users_per_timestamp[$timestamp] = $percentageOfUsers;

  }

  // If $users_per_timestamp array is not empty then draw graph.
  if (!empty($users_per_timestamp)) {

    $min_value = min($users_per_timestamp);
    $max_value = max($users_per_timestamp);
    $average_value = (min($users_per_timestamp) + max($users_per_timestamp))/2;

    if ($min_value == $max_value) {
      $form_users_label[] = 0;
      $form_users_label[] = $max_value;
    } else {
      $form_users_label[] = $min_value;
      $value_temp = number_format(($min_value + $average_value)/2);
      $form_users_label[] = $value_temp;
      $form_users_label[] = number_format(($value_temp + $min_value)/2);
      $form_users_label[] = number_format($average_value);
      $value_temp = number_format(($max_value + $average_value)/2);
      $form_users_label[] = $value_temp;
      $form_users_label[] = number_format(($value_temp + $max_value)/2);
      $form_users_label[] = $max_value;
    }

    // Converst number of users into percentage.
    $users_per_timestamp_temp = $users_per_timestamp;
    foreach($users_per_timestamp_temp as $key => $value) {
      if ($value == $min_value && $value == $max_value) {
        $users_per_timestamp[$key] = 100;
      } else if ($users_per_timestamp[$key] == $max_value) {
        $users_per_timestamp[$key] = 100;
      }  else if ($users_per_timestamp[$key] == $min_value) {
        $users_per_timestamp[$key] = 0;
      }
      else
      {
        // Pre-process the values in percentage.
        $users_per_timestamp[$key] = 100 * (($value - min($users_per_timestamp)) / (max($users_per_timestamp) - min($users_per_timestamp)));
      }
    }

    asort($form_users_label);

    $form['open_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Response from mailing system'),
      '#description' => t('The number of users who opened the email in different month and year.
                          If the user chose to show images, or clicked any of the links in the email,
                          the user will show in this statistic. If the user did not choose to show images,
                          and did not click on any of the links in the email, unfortunately,
                          it is not possible to know whether the user has opened the email or not.
                          Email sent to total ' . $total_users . ' users'),
    );

    // Populate the bar graph parameters.
    $series = array(
      'values' => implode(",", array_reverse($users_per_timestamp)),
      'co-ordinates' =>  '0:|' . implode("|", array_reverse(array_keys($users_per_timestamp))) . '|1:|' . implode("|", $form_users_label),
      'bar_labels' => t('Number of users opened the mail')
    );

    // Dynamically create the width and height based on the number of bars.
    $width = 300;
    $height = 300;
    $num_of_bars = count($users_per_timestamp);
    if ($num_of_bars > 1) {
      $width = $width + ($num_of_bars * 70);
    }

    $graph_type = 'bvg';
    // Graph width should not exceed 1000 px.
    if ($width > 1000) {
      $height = 500;
      $width = 1000;
      $graph_type = 'bhg';
    }

    // Draw chart.
    $openchart = new WildfireBarChart($graph_type, $width, $height, $series);
    $form['open_fieldset']['open_chart'] = array(
      '#markup' => $openchart->render(t('Number of users who opened the email'), t('Number of users who opened the email'), array('id' => 'report-open-bar')),
    );

  }

  return $form;
}

function wildfire_reports_job_summary($jid) {

  $headers = array(
    t('User email'),
    t('Timestamp'),
  );

  $headers = array(
    t('Title'),
    t('List'),
    t('Send count'),
    t('Open Count'),
    t('Started'),
    t('Status'),
  );

  $rows = array();

  $query = db_select('wildfire_jobs', 'wj');

  $query->fields('wj', array(
    'lid',
    'title',
    'count',
    'started',
    'status',
  ));
  $query->fields('wl', array('name'
  ));

  $query->addExpression('COUNT(1)', 'opencount');
  $query->join('wildfire_lists', 'wl', 'wj.lid = wl.lid');
  $query->join('wildfire_tracking', 'wt', 'wt.jid = wj.jid');

  $query->condition('wj.jid', $jid, '=');
  $query->orderBy('timestamp', 'DESC');
  $result = $query->execute();

  $rows = array();
  while ($dbrow = $result->fetchAssoc()) {
    $row[] = $dbrow['title'];
    $row[] = l(
      $dbrow['name'],
      'admin/wildfire/lists/' . $dbrow['lid'] . '/view',
      array('attributes' => array('title' => t('View the contents of this list.')))
    );

    $row[] = $dbrow['count'];
    $row[] = $dbrow['opencount'];
    $row[] = ($dbrow['started'] > 0 ? format_date($dbrow['started'], 'custom', WILDFIRE_DATETIME) : 'No date found');
    $row[] = render(wildfire_get_status_text($dbrow['status']));
    $rows[] = $row;
  }

  if (count($rows) == 0) {
    $row = array(array(
      'colspan' => count($headers),
      'data' => t('No data found.'),
    ));
    $rows[] = $row;
  }

  return array(
    '#markup' => theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'report-job-summary-table'))),
  );

}