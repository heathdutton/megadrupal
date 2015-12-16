<?php

/**
 * @file
 * Profiling user interface theme functions.
 */

/**
 * Implementation of template_preprocess_TEMPLATE().
 */
function template_preprocess_profiling_timeline(&$variables) {
  $time_start = $variables['element']['#time_start'];
  $duration = $variables['element']['#duration'];

  $i = 0;
  $variables['slices'] = array();
  foreach ($variables['element']['#timers'] as $timer) {
    $slice = array(
      'link' => url(profiling_ui_timer_single_path($timer)),
      'title' => check_plain($timer->name),
      'left' => round(($timer->time_start - $time_start) / $duration * 100),
      'width' => ceil(($timer->time_stop - $timer->time_start) / $duration * 100),
      'label' => profiling_ui_format_duration($timer->duration),
    );
    // Ugly fix.
    if ($slice['width'] == 0) {
      $slice['width'] = 1;
    }
    $variables['slices'][] = $slice;
  }
}

/**
 * Implementation of template_preprocess_TEMPLATE().
 */
function template_preprocess_profiling_report(&$variables) {
  drupal_add_css(drupal_get_path('module', 'profiling_ui') . '/theme/profiling.css');

  $variables['items_index'] = array();
  $variables['items'] = array();

  $i = 0;
  foreach ($variables['element']['#items'] as $data) {
    $name = 'report-' . (++$i);

    $variables['items_index'][$name] = $data['title'];
    $variables['items'][$name] = array(
      '#theme' => 'profiling_report_item',
      '#title' => $data['title'],
      '#content' => $data['content'],
    );
  }
}

/**
 * Implementation of template_preprocess_TEMPLATE().
 */
function template_preprocess_profiling_report_item(&$variables) {
  $variables['content'] = $variables['element']['#content'];
  $variables['title'] = $variables['element']['#title'];
  $variables['title_attributes'] = array(
    'title' => t("Click to (un)collapse"),
    'class' => array('profiling-collapse'),
  );
  $variables['attributes']['class'][] = 'profiling-report-item';
}
