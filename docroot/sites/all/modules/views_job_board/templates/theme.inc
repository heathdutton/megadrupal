<?php

function template_preprocess_views_job_board(&$vars) {
  $view = &$vars['view'];
  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;

  $results = $vars['rows'];
  $renders = $handler->render_fields($results);

  $vars['classes_array'][] = 'views-job-board-style';

  if ($vars['options']['randomize']) {
    $module_path = drupal_get_path('module', 'views_job_board');
    drupal_add_js($module_path . '/js/views-job-board.js');
  }

  $company_name_field = $options['company_name'];
  $job_field = $options['job'];
  $logo_field = $options['extra']['logo'];
  $sticky_field = $options['extra']['sticky'];
  $job_type_filed = $options['extra']['job_type'];

  $vars['job_board'] = array();

  // Group rows by company.
  foreach ($results as $key => $row) {
    $company = $renders[$key][$company_name_field];
    $vars['job_board'][$company]['company_name'] = $company;

    if (isset($renders[$key][$job_field])) {
      // Split by node type.
      $job_type = $renders[$key][$job_type_filed] ?: 'ungrouped';
      $vars['job_board'][$company]['jobs'][$job_type][] = $renders[$key][$job_field];
    }

    // Not required.
    if (isset($renders[$key][$logo_field])) {
      $vars['job_board'][$company]['logo'] = $renders[$key][$logo_field];
    }

    $vars['job_board'][$company]['sticky'] = $renders[$key][$sticky_field] ? TRUE : FALSE;
  }
}
