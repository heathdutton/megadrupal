<?php

/**
 * @file
 * Page callbacks for the Seen module.
 *
 * @copyright Copyright(c) 2013 PreviousNext Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

/**
 * Page callback for the seen page.
 */
function seen_page_listing($nid) {

  $seen_results = seen_get_sorted_views($nid);

  if (empty($seen_results)) {
    return t('No results');
  }

  // Process total viewed count.
  $output = seen_prepare_viewed_count($seen_results);

  // Process individual row counts.
  $output .= seen_prepare_table_output($nid, $seen_results);

  return $output;
}

/**
 * Prepare themed output for the result summary.
 *
 * @param array $seen_results
 *   The results for who has seen the item.
 *
 * @return string
 *   A prepared string.
 */
function seen_prepare_viewed_count($seen_results) {

  $total_views = count($seen_results);
  if ($total_views == 1) {
    $pluralised_users = t('user');
  }
  else {
    $pluralised_users = t('users');
  }

  $user_count_output = t('Seen by %count %pluralised_users', array(
    '%count' => $total_views,
    '%pluralised_users' => $pluralised_users,
  ));

  return '<div class="seen-user-count"><p>' . $user_count_output . '</p></div>';
}

/**
 * Prepare detailed information about who has seen an item.
 *
 * @param string $nid
 *   The node identifier.
 * @param array $seen_results
 *   The results for who has seen the item.
 *
 * @return string
 *   A prepared string.
 */
function seen_prepare_table_output($nid, $seen_results) {

  $rows = array();

  $paged_results = seen_pager_array_splice($seen_results, SEEN_DEFAULT_LIST_COUNT);

  // Get a date format, or use default.
  $date_format = variable_get('seen_date_format', 'custom');
  // Get tick mark.
  $tick = variable_get('seen_tick', '&#10004;');
  // See all or only viewed.
  $see_all = variable_get('seen_show_all_users', TRUE);

  foreach ($paged_results as $user_id => $access_records) {
    $number_views = count($access_records);

    // Ignore this row if we are not showing "all users".
    if ($see_all == FALSE && $number_views == 0) {
      continue;
    }

    // Defaults for no view.
    $view_count = '0';
    $last_access = t('None');
    $check = '';

    $account = user_load($user_id);
    if ($account == FALSE) {
      $user_name = t('Unknown or deleted user');
    }
    else {
      $user_name = theme('username', array('account' => $account));
    }

    // Count the number of views recorded.
    if (is_array($access_records) && !empty($access_records)) {
      ksort($access_records);

      $view_count = (string) $number_views;
      $last_record = end($access_records);
      $last_access = format_date($last_record->timestamp, $date_format, seen_default_date_format());

      $check = $tick;
    }

    $rows[] = array(
      'check' => $check,
      'user' => $user_name,
      'last' => $last_access,
      'count' => $view_count,
    );
  }

  $table_vars = array(
    'header' => array(
      array(
        'data' => t('Seen'),
        'field' => 'seen',
      ),
      array(
        'data' => t('User'),
        'field' => 'user',
      ),
      array(
        'data' => t('Last access'),
        'field' => 'access',
      ),
      array(
        'data' => t('View count'),
        'field' => 'view',
      ),
    ),
    'rows' => $rows,
  );

  $output = theme('table', $table_vars);

  $output .= theme('pager', array('quantity' => 5));

  return $output;
}


