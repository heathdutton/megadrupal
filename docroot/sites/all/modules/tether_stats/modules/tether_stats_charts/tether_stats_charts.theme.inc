<?php
/**
 * @file
 * Theme methods for the tether_stats_charts module.
 */

/**
 * Implements hook_theme().
 */
function _tether_stats_charts_theme() {

  $path = drupal_get_path('module', 'tether_stats_charts');

  $base = array(
    'file' => 'tether_stats_charts.theme.inc',
    'path' => $path,
  );

  $items = array();

  $items['tether_stats_charts_iterable_chart'] = $base + array(
    'render element' => 'element',
  );

  $items['tether_stats_charts_iterable_chart_controls'] = $base + array(
    'render element' => 'element',
  );

  return $items;
}

/**
 * Theme callback for an iterable chart.
 *
 * Used for a build array constructed by
 * tether_stats_charts_get_iterable_renderable_chart.
 *
 * @param array $vars
 *   An associative array containing:
 *   - element: The renderable build array.
 *
 * @ingroup themeable
 *
 * @see tether_stats_charts_get_iterable_renderable_chart()
 */
function theme_tether_stats_charts_iterable_chart(array &$vars) {

  $element =& $vars['element'];

  // Let the charts module render the title for the chart.
  if (isset($element['#title'])) {

    $element['chart']['#title'] = $element['#title'];
  }

  $output = render($element['chart']) .
    render($element['controls']) .
    drupal_render_children($element);

  return $output;
}

/**
 * Theme callback for the control bar of an iterable chart.
 *
 * Used in the build array constructed by
 * tether_stats_charts_get_iterable_renderable_chart.
 *
 * @param array $vars
 *   An associative array containing:
 *   - element: The renderable build array.
 *
 * @ingroup themeable
 *
 * @see tether_stats_charts_get_iterable_renderable_chart()
 */
function theme_tether_stats_charts_iterable_chart_controls(array &$vars) {

  $element =& $vars['element'];

  // Set default attributes.
  $element['#attributes'] += array(
    'class' => array('tether_stats_charts-iterator'),
  );

  $output = '<div ' . drupal_attributes($element['#attributes']) . '>' .
    render($element['previous_link']) . render($element['next_link']) . '</div>';

  $output .= drupal_render_children($element);

  return $output;
}
