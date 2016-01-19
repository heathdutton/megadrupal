<?php
/**
 * @file
 * This file contains the processing functions for the time-tracker.
 */

/**
 * Function for generating the effective table.
 *
 * @param array $list
 *   List containing all the time entries.
 *
 * @return string
 *   The HTML for the effective table.
 */
function openlucius_timetracker_build_effective($list) {
  $effective = array(
    'total' => 0,
    'effective' => 0,
    'not_effective' => 0,
  );

  // Loop all the ol_todo_lists and store the time of the type.
  foreach ($list as $item) {

    // Add to general total.
    $effective['total'] += $item['total_time'];
    if (!empty($item['type']->field_timetracker_type_effective) && $item['type']->field_timetracker_type_effective[LANGUAGE_NONE][0]['value']) {
      $effective['effective'] += $item['total_time'];
    }
    else {
      // Add time to not effective.
      $effective['not_effective'] += $item['total_time'];
    }

    // Also track the time for each type.
    $effective['types'][$item['type']->name] += $item['total_time'];
  }
  $header = array();
  // Build the table based on the types, it contains 2 data rows.
  foreach ($effective['types'] as $time_type => $time) {

    // Each type gets it owns column.
    $header[]        = ucfirst($time_type);
    $time_row[]      = openlucius_timetracker_format_time($time);
    $effective_row[] = openlucius_timetracker_calculate_progress($time, $effective['total']);
  }

  // Add the total and effective to the table.
  $time_row[]      = openlucius_timetracker_format_time($effective['total']);
  $effective_row[] = '<strong>' . openlucius_timetracker_calculate_progress($effective['effective'], $effective['total']) . '</strong>';

  // Place them after each other.
  $rows[]   = $time_row;
  $rows[]   = $effective_row;
  $header[] = t('Effective');

  // Create the table.
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  // Now its time to build a detailed overview for each type.
  $header = array(t('Project'), t('Time'), '');

  // Loop all the types.
  foreach ($effective['types'] as $time_type => $value) {
    $rows = array();

    // Loop the list again.
    foreach ($list as $item) {
      $row = array();

      // If the type of the list matches the current type.
      if ($item['type']->name == $time_type && $item['total_time'] > 0) {
        $row[]  = l($item['title'], 'node/' . $item['nid'] . '/time');
        $row[]  = openlucius_timetracker_format_time($item['total_time']);
        $row[]  = openlucius_timetracker_calculate_progress($item['total_time'], $value);
        $rows[] = $row;
      }
    }

    // Type is done, add it to the output.
    $output .= '<p>' . ucfirst($time_type) . '</p>' . theme('table', array(
      'header' => $header,
      'rows'   => $rows,
    ));
  }
  return $output;
}

/**
 * Function to generate the progress with a custom string.
 *
 * @param int $time
 *   Time in seconds.
 * @param int $max
 *   Max in seconds.
 * @param string $string
 *   The string to place inside the span.
 *
 * @return string
 *   The raw progress.
 */
function openlucius_timetracker_calculate_progress_raw($time, $max, $string) {
  $progress = openlucius_timetracker_calculate_percent($time, $max);
  if ($progress > 100) {
    $progress = '<span style="color:red;">' . $string . '</span>';
  }
  elseif ($progress > 75) {
    $progress = '<span style="color:orange;">' . $string . '</span>';
  }
  else {
    $progress = $string;
  }
  return $progress;
}

/**
 * Function to format seconds.
 *
 * @param int $seconds
 *   Seconds.
 * @param string $splitter
 *   Optional splitter string.
 *
 * @return string
 *   The formatted string.
 */
function openlucius_timetracker_format_time($seconds, $splitter = ':') {
  $positive_seconds   = abs($seconds);
  $remaining_seconds  = $positive_seconds % 3600;
  $hours              = ($positive_seconds - $remaining_seconds) / 3600;
  $remaining_seconds2 = $remaining_seconds % 60;
  $minutes            = ($remaining_seconds - $remaining_seconds2) / 60;
  $negative           = ($seconds < 0) ? '-' : '';
  return sprintf("%s%d" . $splitter . "%02d", $negative, $hours, $minutes);
}

/**
 * Function to calculate percent.
 *
 * @param int $time
 *   Time in seconds.
 * @param int $max
 *   Max in seconds.
 *
 * @return int
 *   The percent.
 */
function openlucius_timetracker_calculate_percent($time, $max) {
  return round($time / $max * 100);
}

/**
 * Function to render the progress.
 *
 * @param int $time
 *   Time in seconds.
 *
 * @param int $max
 *   Max in seconds.
 *
 * @param bool $color
 *   Optional use of color.
 *
 * @return string
 *   The progress string.
 */
function openlucius_timetracker_calculate_progress($time, $max, $color = FALSE) {
  $progress = openlucius_timetracker_calculate_percent($time, $max);
  if ($color && $progress > 100) {
    $progress = '<span style="color: red;">' . $progress . '%</span>';
  }
  elseif ($color && $progress > 75) {
    $progress = '<span style="color: orange;">' . $progress . '%</span>';
  }
  else {
    $progress .= '%';
  }
  return $progress;
}

/**
 * Function to build entry table.
 *
 * @param array $list
 *   The list containg all time entries.
 *
 * @param int $total
 *   Max in seconds.
 *
 * @return string
 *   Array of header and rows.
 */
function openlucius_timetracker_build_todo_table($list, $total) {

  // Load processing functions.
  module_load_include('inc', 'openlucius_timetracker', 'includes/processing');

  $header = array(t('Date'), t('User'), t('Message'), t('Hours'));
  $rows   = array();

  foreach ($list as $main) {
    foreach ($main['cases'] as $case_row) {

      // This is the only version where each entry get printed.
      foreach ($case_row['log'] as $entry) {
        $row    = array();
        $row[]  = format_date($entry->start, 'custom', 'd-m-Y');
        $row[]  = $entry->name;
        $row[]  = $entry->message;
        $row[]  = openlucius_timetracker_format_time($entry->stop - $entry->start);
        $rows[] = $row;
      }
    }
  }
  $rows[] = array('', '', '', '');
  $rows[] = array(
    t('Total'),
    '',
    '',
    openlucius_timetracker_format_time($total),
  );
  return array('header' => $header, 'rows' => $rows);
}

/**
 * Function to build entry table.
 *
 * @param array $list
 *   The list containg all time entries.
 *
 * @param int $total
 *   Max in seconds.
 *
 * @return string
 *   Array of header and rows.
 */
function openlucius_timetracker_build_list_table($list, $total) {
  $header = array(
    t('Title'),
    t('Status'),
    t('Assigned'),
    t('Time'),
    t('Max'),
    t('Completed'),
    t('Modified'),
  );
  $rows   = array();

  foreach ($list as $parent_nid => $main) {
    // Add the information of the list.
    $rows[] = array(
      '<h3>' . l($main['title'], 'node/' . $parent_nid . '/time') . ' ' . openlucius_timetracker_format_time($main['total_time']) . '</h3>',
      '',
      '',
      '',
      '',
      '',
      '',
    );

    // Add the information of each case.
    foreach ($main['cases'] as $nid => $case_row) {
      $row    = array();
      $row[]  = l($case_row['title'], 'node/' . $nid);
      $row[]  = $case_row['status'];
      $row[]  = $case_row['user'];
      $row[]  = openlucius_timetracker_format_time($case_row['time']);
      $row[]  = openlucius_timetracker_format_time($case_row['max']);
      $row[]  = openlucius_timetracker_calculate_progress($case_row['time'], $case_row['max']);
      $row[]  = format_date($case_row['changed'], 'custom', 'd-m-Y');
      $rows[] = $row;
    }
    $rows[] = array(
      t('Total'),
      '',
      '',
      openlucius_timetracker_format_time($main['total_time']),
      '',
      '',
      '',
    );
  }
  return array('header' => $header, 'rows' => $rows);
}

/**
 * Function to process the time-tracker results.
 *
 * @param \DatabaseStatementInterface $result
 *   A DatabaseStatementInterface to be processed.
 *
 * @param string $type
 *   How the data should be processed.
 *
 * @return array
 *   An array containing the lists, total time in seconds and the list_types.
 */
function openlucius_timetracker_process_results(\DatabaseStatementInterface $result, $type) {

  // Initialize user as NULL.
  $user = NULL;

  // The whole data tree will be stored here.
  $list = array();

  // Small helper array that contains totals.
  $total = array();

  // To store the type of a node, like 'project' or 'sla' for easy reference.
  $list_type = array();

  // Get all the node titles in bulk.
  $titles = openlucius_timetracker_get_titles();

  // Get all the terms that are needed to display.
  $terms = openlucius_core_get_terms('labels_todo_s');

  // Get all the timetrackers types that are needed to display.
  $types = openlucius_core_get_terms('timetracker_type', TRUE);

  // Get any filters from the url.
  $filter = openlucius_timetracker_get_filter();

  switch ($type) {

    case OPENLUCIUS_TIMETRACKER_DEFAULT:
    case OPENLUCIUS_TIMETRACKER_BLOCK:
    case OPENLUCIUS_TIMETRACKER_RAW:
      // Get all the users in bulk.
      $users = openlucius_timetracker_get_users();
      break;

    case OPENLUCIUS_TIMETRACKER_USER:
      // Filter the url input.
      $user = user_load(filter_xss($_GET['user']));

      // For a user page we only need the active user.
      $users = array($user->uid => $user->name);
      break;

    default:
      break;
  }

  foreach ($result as $item) {
    $nid = $item->nid;

    // Id of the ol_todo_list
    $parent_nid = isset($item->list) ? $item->list : 0;

    // This is the first time this parent is encountered.
    if (!isset($list[$parent_nid])) {
      $list[$parent_nid] = array();

      // Reference for easy access.
      $parent = &$list[$parent_nid];

      // Store the title.
      $parent['title'] = isset($titles[$parent_nid]) ? $titles[$parent_nid] : '';

      // Fetch item type.
      $item_type = isset($item->type) ? $item->type : '';

      // Each case that is linked to this node will be stored here.
      $parent['cases']      = array();
      $parent['total_time'] = 0;
      $parent['type']       = isset($types[$item_type]) ? $types[$item_type] : '';
      $parent['nid']        = $parent_nid;

      // Store the type of this list.
      $list_type[$parent_nid] = isset($types[$item_type]->name) ? ucfirst($types[$item_type]->name) : '';
    }
    else {
      // Reference for easy access.
      $parent = &$list[$parent_nid];
    }

    // This is the first time this case is encountered.
    if (!isset($parent['cases'][$nid])) {
      $parent['cases'][$nid] = array();

      // Reference for easy access.
      $case = &$parent['cases'][$nid];

      // Store the defaults for this case, only needs to be done once.
      $case['time']   = 0;
      $case['title']  = isset($titles[$nid]) ? $titles[$nid] : '';
      $case['user']   = isset($item->uid) && isset($users[$item->uid]) ? $users[$item->uid] : '';
      $case['status'] = isset($item->status) && isset($terms[$item->status]) ? $terms[$item->status] : '';

      // Convert to seconds.
      $case['max']     = isset($item->max) ? ($item->max * 3600) : 0;
      $case['changed'] = isset($item->changed) ? $item->changed : 0;

      // All entries for this case will be stored here.
      $case['log'] = array();
    }
    else {
      // Reference for easy access.
      $case = &$parent['cases'][$nid];
    }

    $diff = $item->stop - $item->start;

    // Add the time of the entry to the case.
    $case['time'] += $diff;

    // Add the diff of the entry to itself.
    $item->diff = $diff;

    // Store the time entry for later reuse.
    $case['log'][] = $item;

    // If we are checking the time of a user do not update
    // the totals if it does not belong to the user.
    if ($type === OPENLUCIUS_TIMETRACKER_BLOCK || (isset($filter['user']) && !empty($filter['user']) && $item->uid == $user->uid) || !isset($filter['user'])) {

      // Total time on the list.
      $parent['total_time'] += $diff;

      // Global time.
      if (isset($total['sec'])) {
        $total['sec'] += $diff;
      }
      else {
        $total['sec'] = $diff;
      }
    }
  }

  return array(
    'list' => $list,
    'total' => $total,
    'list_type' => $list_type,
  );
}

/**
 * Function to process a reports page.
 *
 * @param array $data
 *   An array containing the list, time and list_types.
 * @param null $filter
 *   (optional) Filtering for the reports page.
 *
 * @return string
 *   The html for the reports page.
 *
 * @throws \Exception
 */
function openlucius_timetracker_reports_processing(array $data, $filter = NULL) {

  // Add the javascript for the chart.
  _openlucius_timetracker_add_chart_js();
  $output = '<canvas id="timeTrackerOverview" width="800" height="400"></canvas>';

  // Build the effective table.
  $output .= openlucius_timetracker_build_effective($data['list']);

  // Get all the node titles in bulk.
  $titles = openlucius_timetracker_get_titles();

  // Store a lot of totals for a detailed table output.
  $day_list       = array();
  $node_day_total = array();
  $user_total     = array();
  $day_total      = array();

  // Loop all the lists.
  foreach ($data['list'] as $list_id => $main) {

    // Loop all the cases within a list.
    foreach ($main['cases'] as $nid => $case_row) {

      // Loop all the time entries with a case.
      foreach ($case_row['log'] as $entry) {

        // Convert to date for easy reference.
        $date = date('d-m-Y', $entry->start);

        // Check if we already have a case for list X on day Z.
        if (!isset($day_list[$date][$list_id][$nid])) {
          $case_row_clone = $case_row;

          // Unset the log from the clone, not needed later.
          unset($case_row_clone['log']);

          // Add the defaults to the day_list.
          $day_list[$date][$list_id][$nid] = $case_row_clone;
        }
        $diff = $entry->diff;

        // If an user is selected.
        if (!empty($filter['user'])) {

          // And the user of the entry is the same as the selected user.
          if ($entry->uid == $filter['user']) {

            // Add the time entry to all appropriate lists.
            $day_list[$date][$list_id][$nid]['user_time'] += $diff;
            $user_total[$nid] += $diff;
            $day_total[$date] += $diff;
            $node_day_total[$date][$list_id] += $diff;
            $node_day_total[$date][$nid] += $diff;
          }
          // Entry is not relevant because the users do not match.
          // Do not store anything.
        }

        // No user is selected (default).
        if (empty($filter['user']) || $filter['user'] == 0) {

          // Add the time entry only to the general totals.
          $node_day_total[$date][$nid] += $diff;
          $day_total[$date] += $diff;
          $node_day_total[$date][$list_id] += $diff;
        }
      }
    }
  }

  $day_total_js_temp = array();

  $min = $filter['start'];
  $max = $filter['end'] + 86400;

  // Refactor the day totals.
  foreach ($day_total as $key => $value) {
    $day_total_js_temp[] = array(
      'value' => openlucius_timetracker_format_time($value, '.'),
      'time'  => $key,
    );
  }

  $current            = $min;
  $day_total_js_temp2 = array();

  // Add empty items for each day that is missing between.
  // Also add the valid data to the new array.
  while ($current < $max) {

    $found = FALSE;
    foreach ($day_total_js_temp as $item) {

      // Check the date of the current item.
      if ($item['time'] == date('d-m-Y', $current)) {

        // We found data, copy it to the new array.
        $day_total_js_temp2[$current] = $item['value'];
        $found                        = TRUE;
        break;
      }
    }

    // No data is found for this day, add the blank.
    if (!$found) {
      $day_total_js_temp2[$current] = 0;
    }

    // Go to next day.
    $current += 86400;
  }

  // day_total_js_temp2 has keys with timestamps in the correct order.
  // To make it human readable we convert it back to dates.
  $day_total_js = array();
  foreach ($day_total_js_temp2 as $key => $value) {
    $day_total_js[date('d-m-Y', $key)] = $value;
  }

  // Add the data to the javascript for further use.
  drupal_add_js(array('openlucius_timetracker' => array('data' => $day_total_js)), 'setting');

  $rows = array();
  // Finally build the output of each day.
  foreach ($day_list as $date => $day) {

    // Only show the day if there is actually any time on it.
    if ($day_total[$date] > 0) {

      // Add a row containing the information of this day.
      $rows[] = array(
        '<p><strong>' . $date . '</strong></p>',
        '',
        '<strong>' . openlucius_timetracker_format_time($day_total[$date]) . '</strong>',
        '',
        '',
        '',
        '',
      );

      // Loop each list for every day.
      foreach ($day as $list_id => $options) {
        if ($node_day_total[$date][$list_id] > 0) {

          // Add a row containing the information of this list.
          $rows[] = array(
            $titles[$list_id] . ' ' . openlucius_timetracker_format_time($node_day_total[$date][$list_id]),
            $options['list_type'][$list_id],
            '',
            '',
            '',
            '',
            '',
          );

          // Now print each case for this list on this day.
          foreach ($options as $nid => $case) {
            // Get the progress, including colors.
            $progress = openlucius_timetracker_calculate_progress($case['time'], $case['max'], TRUE);

            $row   = array();
            $row[] = l($case['title'], 'node/' . $nid . '/time');
            $row[] = $case['status'];
            $row[] = openlucius_timetracker_format_time($node_day_total[$date][$nid]);

            // If the results are filtered on a user, only show the case
            // if the user actually has time on the case.
            if (isset($filter['user']) && !empty($filter['user']) && $case['user_time'] > 0) {
              $row[] = openlucius_timetracker_format_time($user_total[$nid]);
            }
            else {
              $row[] = '';
            }
            $row[] = openlucius_timetracker_format_time($case['time']);
            $row[] = openlucius_timetracker_format_time($case['max']);
            $row[] = $progress;

            if (isset($filter['user']) && !empty($filter['user']) && $case['user_time'] > 0) {
              $rows[] = $row;
            }
            elseif (isset($filter['user']) && !empty($filter['user']) && $case['user_time'] == 0) {
              // Nothing, user has no clocked time on this case.
            }
            // Default.
            else {
              $rows[] = $row;
            }
          }
          $rows[] = array('', '', '', '', '', '', '');
        }
      }
    }
  }

  // Header if an user is selected.
  if (isset($filter['user']) && !empty($filter['user'])) {
    $header = array(
      t('Title'),
      t('Status'),
      t('Time'),
      t('User total'),
      t('Case Total'),
      t('Max'),
      '',
    );
  }
  // Default header.
  else {
    $header = array(
      t('Title'),
      t('Status'),
      t('Time'),
      '',
      t('Case Total'),
      t('Max'),
      '',
    );
  }
  $table_options = array('header' => $header, 'rows' => $rows);

  // Build the table.
  $output .= theme('table', $table_options);

  // Prepend total.
  $output = t('Total') . ': ' . openlucius_timetracker_format_time($data['total']['sec']) . ' ' . t('hours') . $output;

  // Allow other modules to change the output if needed.
  drupal_alter('timetracker_output', $output, $data);

  // Return themed data.
  return $output;
}
