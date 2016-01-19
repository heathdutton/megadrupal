<?php
/**
 * @file
 * Loads classes for the generation of charts.
 */

require_once __DIR__ . '/src/chart/TetherStatsChartsPieChart.php';
require_once __DIR__ . '/src/chart/TetherStatsChartsComboChart.php';

/**
 * JSON callback: Generate chart data for chart iteration.
 *
 * @param TetherStatsChartsSchema $chart_schema
 *   The chart schema object representing the data to query.
 */
function tether_stats_charts_data_callback(TetherStatsChartsSchema $chart_schema) {

  $json = array(
    'status' => FALSE,
  );
  $start = $_GET['start'];
  $direction = $_GET['direction'];
  $chart = FALSE;

  if (isset($chart_schema) && is_numeric($start) && $chart_schema instanceof TetherStatsChartsIteratorInterface) {

    $date_start = new DateTime();
    $date_start->setTimestamp($start);

    // Iterate to the new start date.
    switch ($direction) {

      case 'next':
        $date_start = $chart_schema->nextDateTime($date_start);
        break;

      case 'prev':
      default:
        $date_start = $chart_schema->previousDateTime($date_start);
        break;

    }

    if (is_a($chart_schema, 'TetherStatsChartsComboChartSchema')) {

      $chart = new TetherStatsChartsComboChart($chart_schema, $date_start, tether_stats_set_db());
    }
    elseif (is_a($chart_schema, 'TetherStatsChartsPieChartSchema')) {

      $chart = new TetherStatsChartsPieChart($chart_schema, $date_start, tether_stats_set_db());
    }

    if ($chart) {
      $data = $chart->getDataTable();

      // Determine the active state of the iteration buttons depending on
      // available data.
      $next_start = $chart_schema->nextDateTime($date_start);
      $current_time = new DateTime();

      // Normalize the current time to the beginning of the current day.
      TetherStatsChartsSteppedChartSchema::normalizeDate('day', $current_time);

      $json = array(
        'status' => TRUE,
        'data' => $data,
        'previous' => ($date_start->getTimestamp() > variable_get('tether_stats_earliest_activation_time', REQUEST_TIME)),
        'next' => ($next_start <= $current_time),
        'start' => $date_start->getTimeStamp(),
      );
    }
  }
  else {

    watchdog('tether_stats_charts', 'Invalid or unknown chart schema specified in %function.',
    array('%function' => 'tether_stats_charts_data_callback'), WATCHDOG_ERROR);
  }

  echo drupal_json_encode($json);
  exit;
}

/**
 * Page callback: Displays a report for a given stats element.
 *
 * @param object $stats_element
 *   A tether stats element object.
 */
function tether_stats_charts_admin_element_report_callback($stats_element) {

  module_load_include('inc', 'tether_stats_charts', 'tether_stats_charts.forms');

  $build['filter_form'] = drupal_get_form('tether_stats_charts_admin_element_report_filter_form', $stats_element);

  if (isset($_SESSION['tether_stats']['element_report_filter'])) {

    $filter =& $_SESSION['tether_stats']['element_report_filter'];

    $period_ending = clone $filter['period_ending'];

    if ($filter['period_starting'] == $filter['period_ending']) {

      // In the case of the dates being the same, we want to include the full
      // day and not the first hour, so we add the remaining 23 hours.
      TetherStatsChartsSteppedChartSchema::addInterval('hour', $period_ending, 23);
    }

    $schema = new TetherStatsChartsComboChartSchema("tether_stats_chart_elid_{$stats_element->elid}");
    $schema->addElementItem($stats_element->elid, $filter['activity_type'], "Element {$stats_element->elid}");
    $schema->calcDomainStep($filter['period_starting'], $period_ending);

    $chart = new TetherStatsChartsComboChart($schema, $filter['period_starting'], tether_stats_set_db());

    $chart_renderable = $chart->getRenderable();

    $chart_renderable['#title'] = t('Element Report');

    $charts_info = charts_info();

    // Make sure we have at least one chart library installed as this page will
    // generate a chart using the default library.
    if (!empty($charts_info)) {

      $build['chart'] = $chart_renderable;
    }
    else {

      drupal_set_message(t('No chart library installed for the Charts module.'), 'error');

      $build['chart'] = array(
        '#markup' => '<p>' . t('To the use the Charts module, at least one chart library module installed such as Google Charts. See <a href="!module_url">modules</a>.',
          array('!module_url' => url('admin/modules'))) . '</p>',
      );
    }
  }

  return $build;
}

/**
 * Page callback: Displays a chart of overall hits for the past seven days.
 */
function tether_stats_charts_overview_callback() {

  $build = array();

  $charts_info = charts_info();

  // Make sure we have at least one chart library installed as this page will
  // generate a chart using the default library.
  if (!empty($charts_info)) {

    // Show a chart for all hits in the past 7 days.
    $interval = new DateInterval('P6D');
    $period_starting = new DateTime();

    // Reduce the $period_starting to the beginning of the day, then again by
    // another 6 days.
    TetherStatsChartsSteppedChartSchema::normalizeDate('day', $period_starting);
    $period_starting->sub($interval);

    $schema = new TetherStatsChartsComboChartSchema('tether_stats_charts_overall_hits', 7, 'day');
    $schema->addEventItem('hit', "Total Hits");

    $chart = new TetherStatsChartsComboChart($schema, $period_starting, tether_stats_set_db());

    $chart_renderable = $chart->getRenderable();

    $chart_renderable['#title'] = t('Overall hits over the past week');

    $build['chart'] = $chart_renderable;
  }
  else {

    drupal_set_message(t('No chart library installed for the Charts module.'), 'error');

    $build['no_chart_library'] = array(
      '#markup' => '<p>' . t('To view this chart, at least one chart library module installed for the Charts module such as Google Charts. See <a href="!module_url">modules</a>.',
        array('!module_url' => url('admin/modules'))) . '</p>',
    );
  }

  return $build;
}

/**
 * Normalizes a given date.
 *
 * Reduces the date to the start of its given period. The period
 * may be 'hour', 'day', 'month', or 'year'.
 *
 * @param string $period
 *   The period to normalize. May be 'hour', 'day', 'month', or
 *   'year'.
 * @param DateTime $date
 *   The date to normalize.
 */
function tether_stats_normalize_date($period, DateTime &$date) {

  TetherStatsChartsSteppedChartSchema::normalizeDate($period, $date);
}

/**
 * Gets the elements with the most activity entries.
 *
 * Finds the top performing elements for the given type and over the
 * optional time period.
 *
 * Activity is aggregated in hourly chunks. This method uses the aggregated
 * hour counts for efficiency. However, without normalizing the $start before
 * calling this method, activity that was added before the first hour
 * turnover would be missed.
 *
 * @param string $activity_type
 *   The type of activity to search for.
 * @param int $start
 *   (Optional) The period start date.
 * @param int $finish
 *   (Optional) The period end date.
 * @param int $limit
 *   (Optional) The number of elements to return.
 * @param string|null $database_id
 *   (Optional) The id of the database to use.
 *
 * @return array
 *   An associative array of element elids to aggregated activity counts.
 */
function tether_stats_get_top_elements_for_activity($activity_type, $start = NULL, $finish = NULL, $limit = 5, $database_id = NULL) {

  if (!isset($database_id)) {

    $database_id = tether_stats_set_db();
  }

  $old_db = db_set_active($database_id);
  $select = db_select('tether_stats_hour_count', 'c')
    ->fields('c', array(
      'elid',
    ))
    ->condition('type', $activity_type);

  if (isset($start)) {

    $select->condition('hour', $start, '>=');

  }

  if (isset($finish)) {

    $select->condition('hour', $finish, '<');
  }

  $select->addExpression('SUM(count)', 'count');

  $select->groupBy('elid')
    ->orderBy('count', 'DESC')
    ->range(0, $limit);

  $result = $select->execute()->fetchAllKeyed();
  db_set_active($old_db);

  return $result;
}
