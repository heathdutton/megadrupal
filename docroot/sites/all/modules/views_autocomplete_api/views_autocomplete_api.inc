<?php

/**
 * Create autcomplete field from Views.
 * @param string $viewName
 */
function views_autocomplete_api_autocomplete() {
  $arg = arg();
  $views_list = array();
  // Get all the referenced views.
  if (!empty($arg)) {
    $prefix = $search = array_pop($arg);
    array_shift($arg);
    $views_list = $arg;
  }
  preg_match_all('/[^, ]*$/', $search, $match);
  $search = $match[0][0];
  $viewData = array();
  // Loop all the referenced views to get.
  foreach ($views_list as $view_name) {
    // Get the view.
    $view = views_get_view($view_name);
    // Check wether the view exists and user has access to it.
    
    if ($view) {
      //Make a foreach to support Views 3 "OR" statement and search for multiple fields.
      foreach ($view->display['default']->display_options['filters'] AS $key => $value) {
        if (!$value['value'] && $search) {
          $view->display['default']->display_options['filters'][$key]['value'] = $search;
        }
      }
      $view->render();
      // Check for access.
      if ($view->access('default')) {
        if ($view->style_plugin->rendered_fields) {
          // Get the views header if it exists.
          if (!empty($view->header)) {
            $header = $view->header;
            $header = array_shift($header);
            $viewData[$view_name] = '<strong class="group-header">' . strip_tags($header->options['content']) . '</strong>';
          }
        }
        // Get all the values.
        foreach ($view->style_plugin->rendered_fields as $row) {
          $rowValues = array_values($row);
          $key = $val = $rowValues[count($row) - 2];
          $count = count($rowValues);
          //Take the last field to allow to call more that one and "Rewrite field" and call them all.
          if ($count > 1) {
            $val = $rowValues[$count - 1];
            preg_match_all("/^(.*),/", $prefix, $string);
          }
          // Key should not have HTML tags.
          $new_key = @$string[0][0] . strip_tags($key);
          while (isset($viewData[$new_key])) {
            $new_key .= ' ';
          }
          // Do not include if there is no output.
          if ($val) {
            $viewData[$new_key] = '<div class="' . $view_name . '">' . $val . '</div>';
          }
        }
      }
    }
  }
  // Data can be altered.
  drupal_alter('autocomplete_api', $viewData, $views_list);
  drupal_json_output(array_unique($viewData));
}
