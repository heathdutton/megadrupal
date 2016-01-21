<?php
/**
 * @file
 * Callback for the example charts page.
 */

/**
 * Page callback: Display the example charts.
 */
function tether_stats_charts_examples_callback() {

  $build = array();
  $charts_info = charts_info();

  // Make sure we have at least one chart library installed as these examples
  // will use the default library.
  if (!empty($charts_info)) {

    if (variable_get('tether_stats_earliest_activation_time', 0)) {

      $build['pie_chart'] = tether_stats_charts_example_pie_chart();

      $build['combo_chart'] = tether_stats_charts_example_combo_chart();

      $build['pie_iterative_chart'] = tether_stats_charts_example_iterative_pie_chart();

      $build['combo_iterative_chart'] = tether_stats_charts_example_iterative_combo_chart();
    }
    else {

      $build['no_data'] = array(
        '#markup' => '<p>' . t('The chart examples are based on live Tether Stats data. To view these examples, please activate data collection on the !settings_link page.',
          array('!settings_link' => l(t('settings'), 'admin/config/system/tether_stats'))) . '</p>',
      );
    }
  }
  else {

    drupal_set_message(t('No chart library installed for the Charts module.'), 'error');

    $build['no_chart_library'] = array(
      '#markup' => '<p>' . t('To the use the Charts module, at least one chart library module installed such as Google Charts. See !module_link.',
        array('!module_link' => l(t('modules'), 'admin/modules'))) . '</p>',
    );
  }

  return $build;
}

/**
 * Build a renderable array for a pie chart example.
 */
function tether_stats_charts_example_pie_chart() {

  module_load_include('inc', 'tether_stats_charts', 'tether_stats_charts.data');

  $old_db = db_set_active(tether_stats_set_db());
  $top_pages = db_select('tether_stats_element', 'e')
    ->fields('e', array('elid', 'url', 'query'))
    ->condition(db_or()
      ->condition('entity_type', 'node')
      ->condition(db_and()
        ->isNull('entity_type')
        ->isNull('name')
      )
    )
    ->isNull('e.derivative_id')
    ->orderBy('count', 'DESC')
    ->range(0, 5)
    ->execute()
    ->fetchAllAssoc('elid', PDO::FETCH_ASSOC);
  db_set_active($old_db);

  // Choose a data domain interval of three days.
  $domain_interval = new DateInterval('P3D');

  // Construct a Schema to describe the chart we wish to generate.
  $schema = new TetherStatsChartsPieChartSchema('my_pie_chart_id', $domain_interval);

  foreach ($top_pages as $elid => $element) {

    $title = '<front>';

    if (!empty($element['url'])) {

      $title = $element['url'];
    }
    if (!empty($element['query'])) {

      $title .= '?' . $element['query'];
    }

    $schema->addElementItem($elid, 'hit', $title);
  }

  // Start the domain three days ago to match our domain interval. This should
  // include all data up to the current time.
  $period_starting = new DateTime();

  // Normalize the start to the beginning of the current day.
  tether_stats_normalize_date('day', $period_starting);

  $period_starting->sub($domain_interval);

  // Add one day to include the current day.
  $period_starting->add(new DateInterval('P1D'));

  // Construct the chart data object.
  $chart = new TetherStatsChartsPieChart($schema, $period_starting);

  $chart_renderable = $chart->getRenderable();

  $chart_renderable['#title'] = t('Hit Comparison of Top Five Pages Over The Past Three Days');

  return array(
    'chart' => $chart_renderable,
  );
}

/**
 * Build a renderable array for an iterative pie chart example.
 */
function tether_stats_charts_example_iterative_pie_chart() {

  module_load_include('inc', 'tether_stats_charts', 'tether_stats_charts.data');

  $old_db = db_set_active(tether_stats_set_db());
  $top_pages = db_select('tether_stats_element', 'e')
    ->fields('e', array('elid', 'url', 'query'))
    ->condition(db_or()
      ->condition('entity_type', 'node')
      ->condition(db_and()
        ->isNull('entity_type')
        ->isNull('name')
      )
    )
    ->isNull('e.derivative_id')
    ->orderBy('count', 'DESC')
    ->range(0, 5)
    ->execute()
    ->fetchAllAssoc('elid', PDO::FETCH_ASSOC);
  db_set_active($old_db);

  // Choose a data domain interval of seven days.
  $domain_interval = new DateInterval('P7D');

  // Construct a Schema to describe the chart we wish to generate.
  $schema = new TetherStatsChartsPieChartSchema('my_iterative_pie_chart_id', $domain_interval);

  foreach ($top_pages as $elid => $element) {

    $title = '<front>';

    if (!empty($element['url'])) {

      $title = $element['url'];
    }
    if (!empty($element['query'])) {

      $title .= '?' . $element['query'];
    }

    $schema->addElementItem($elid, 'hit', $title);
  }

  // Start the domain three days ago to match our domain interval. This should
  // include all data up to the current time.
  $period_starting = new DateTime();

  // Normalize the start to the beginning of the current day.
  tether_stats_normalize_date('day', $period_starting);

  $period_starting->sub($domain_interval);

  // Add one day to include the current day.
  $period_starting->add(new DateInterval('P1D'));

  // Construct the chart data object.
  $chart = new TetherStatsChartsPieChart($schema, $period_starting);

  $chart_renderable = tether_stats_charts_get_iterable_renderable_chart($chart);

  $chart_renderable['#title'] = t('Iterative Hit Comparison of Top Five Pages Over The Past Week');

  return $chart_renderable;
}

/**
 * Build a renderable array for a combo chart example.
 */
function tether_stats_charts_example_combo_chart() {

  module_load_include('inc', 'tether_stats_charts', 'tether_stats_charts.data');

  $old_db = db_set_active(tether_stats_set_db());
  $top_pages = db_select('tether_stats_element', 'e')
    ->fields('e', array('elid', 'url', 'query'))
    ->condition(db_or()
      ->condition('entity_type', 'node')
      ->condition(db_and()
        ->isNull('entity_type')
        ->isNull('name')
      )
    )
    ->isNull('e.derivative_id')
    ->orderBy('count', 'DESC')
    ->range(0, 3)
    ->execute()
    ->fetchAllAssoc('elid', PDO::FETCH_ASSOC);
  db_set_active($old_db);

  // Construct a Schema to describe the chart we wish to generate.
  // Choose a data domain interval of seven days.
  $schema = new TetherStatsChartsComboChartSchema('my_combo_chart_id', 5, 'day', 1);

  foreach ($top_pages as $elid => $element) {

    $title = '<front>';

    if (!empty($element['url'])) {

      $title = $element['url'];
    }
    if (!empty($element['query'])) {

      $title .= '?' . $element['query'];
    }

    $schema->addElementItem($elid, 'hit', $title);
  }

  // Start the domain seven days ago to match our domain interval. This should
  // include all data up to the current time.
  $period_starting = new DateTime();

  // Normalize the start to the beginning of the current day.
  tether_stats_normalize_date('day', $period_starting);

  // Reduce by four days, so the fifth will be the current day.
  $period_starting->sub(new DateInterval('P4D'));

  // Construct the chart data object.
  $chart = new TetherStatsChartsComboChart($schema, $period_starting);

  $chart_renderable = $chart->getRenderable();

  $chart_renderable['#title'] = t('Hit Comparison of Top Three Pages Over The Past Five Days');

  return array(
    'chart' => $chart_renderable,
  );
}

/**
 * Build a renderable array for an iterative combo chart example.
 */
function tether_stats_charts_example_iterative_combo_chart() {

  module_load_include('inc', 'tether_stats_charts', 'tether_stats_charts.data');

  $old_db = db_set_active(tether_stats_set_db());
  $top_pages = db_select('tether_stats_element', 'e')
    ->fields('e', array('elid', 'url', 'query'))
    ->condition(db_or()
      ->condition('entity_type', 'node')
      ->condition(db_and()
        ->isNull('entity_type')
        ->isNull('name')
      )
    )
    ->isNull('e.derivative_id')
    ->orderBy('count', 'DESC')
    ->range(0, 3)
    ->execute()
    ->fetchAllAssoc('elid', PDO::FETCH_ASSOC);
  db_set_active($old_db);

  // Construct a Schema to describe the chart we wish to generate.
  // Choose a data domain interval of seven days.
  $schema = new TetherStatsChartsComboChartSchema('my_iterative_combo_chart_id', 7, 'day');

  foreach ($top_pages as $elid => $element) {

    $title = '<front>';

    if (!empty($element['url'])) {

      $title = $element['url'];
    }
    if (!empty($element['query'])) {

      $title .= '?' . $element['query'];
    }

    $schema->addElementItem($elid, 'hit', $title);
  }

  $schema->addSummationLineSeries(t('Total'));

  // Start the domain seven days ago to match our domain interval. This should
  // include all data up to the current time.
  $period_starting = new DateTime();

  // Normalize the start to the beginning of the current day.
  tether_stats_normalize_date('day', $period_starting);

  // Reduce by six days, so the seventh will be the current day.
  $period_starting->sub(new DateInterval('P6D'));

  // Construct the chart data object.
  $chart = new TetherStatsChartsComboChart($schema, $period_starting);

  $chart_renderable = tether_stats_charts_get_iterable_renderable_chart($chart);

  $chart_renderable['#title'] = t('Iterative Hit Comparison of Top Three Pages');

  return $chart_renderable;
}
