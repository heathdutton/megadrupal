<?php
// $Id: statspro_graphs.inc,v 1.1.2.5 2010/11/05 23:02:14 rsevero Exp $

/**
 * @file
 *   Graphs generation help.
 *
 */

/**
 * Returns the available charting option that should be used as default.
 *
 * @return <string>
 */
function statspro_get_default_charting_option() {
  static $default = NULL;

  if ($default === NULL) {
    $default = '';
    $default_charting_libraries_per_preference = array(
      'bluff' => 'charts_graphs_bluff',
      'google_charts' => 'charts_graphs_google_charts',
      'open-flash' => 'charts_graphs_open_flash',
      'amcharts' => 'charts_graphs_amcharts',
    );
    foreach ($default_charting_libraries_per_preference as $default_charting_library => $module_id) {
      if (module_exists($module_id)) {
        $default = $default_charting_library;
        break;
      }
    }
  }
  return $default;
}

/**
 * Returns the charting option to be used.
 *
 * @return <string>
 */
function statspro_get_current_charting_option() {
  static $current = NULL;

  if ($current === NULL) {
    $default_charting_option = statspro_get_default_charting_option();
    $current = variable_get(
      'statspro_charting_library',
      $default_charting_option
    );
    $charting_libraries = charts_graphs_apis();
    if (!isset($charting_libraries[$current])) {
      $current = $default_charting_option;
    }
  }

  return $current;
}

/**
 * Returns the HTML for one pie graph.
 *
 * @param <string> $title
 * @param <array> $series
 * @param <array> $x_labels
 * @return <string>
 */
function statspro_get_pie_chart($title, $series, $x_labels) {
  $canvas = charts_graphs_get_graph(statspro_get_current_charting_option());

  $canvas->title = $title;
  $canvas->type = 'pie';
  $canvas->y_legend = 'Y Legend';
  $canvas->theme = '37signals';
  $canvas->width = 700;
  $canvas->height = 200;
  $canvas->series = $series;
  $canvas->x_labels = $x_labels;

  return "\n<div align='center'>\n" . $canvas->get_chart() . "\n</div>\n";
}

/**
 * Returns the HTML for one bar graph.
 *
 * @param <string> $title
 * @param <array> $series
 * @param <array> $x_labels
 * @param <mixed> $transpose NULL or BOOL
 * @return <string>
 */
function statspro_get_bar_chart($title, $series, $x_labels, $transpose = NULL) {
  $canvas = charts_graphs_get_graph(statspro_get_current_charting_option());

  $canvas->title = $title;
  $canvas->type = 'bar';
  $canvas->y_legend = 'Y Legend';
  $canvas->theme = '37signals';
  $canvas->width = 700;
  $canvas->height = 200;
  $canvas->series = $series;
  $canvas->x_labels = $x_labels;
  if ($transpose !== NULL) {
    $canvas->transpose = $transpose;
  }

  return "\n<div align='center'>\n" . $canvas->get_chart() . "\n</div>\n";
}
