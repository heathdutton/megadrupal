<?php

/**
 * @file
 * Implements Route theme functions.
 */

/**
 * Renders the route progress indicator
 * @param type $variables
 */
function theme_route_progress_indicator($variables) {
  $route = $variables['route'];

  $steps = array();
  foreach($route->steps as $step => $substeps) {
    $step_configuration = $route->get_step_configuration($step);

    if ($route->active_step == $step) {
      $steps[] = array(
        'data' => t($step_configuration['title']),
        'class' => array('active')
      );
    }
    elseif ($route->is_succeeding($step) && $route->is_processed($step) && $route->has_indicator($step)) {
      $steps[] = l($step_configuration['title'], route_build_path(route_get_current_page_subtask_name(), $step));
    }
    elseif ($route->is_processed($step) && $route->has_indicator($step)) {
      $steps[] = array(
        'data' => l($step_configuration['title'], route_build_path(route_get_current_page_subtask_name(), $step)),
        'class' => array('visited')
      );
    }
    else if($route->has_indicator($step)) {
      $steps[] = $step_configuration['title'];
    }
  }

  return theme('item_list', array('items' => $steps, 'type' => 'ol'));
}
