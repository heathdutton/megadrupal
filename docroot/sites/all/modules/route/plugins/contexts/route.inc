<?php

/**
 * @file
 * Implements route control context to Panels
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Route"),
  'description' => t('Route CTools context used primarily to control the flow of Route.'),
  'context' => 'route_context_create',
  'keyword' => 'route',
  'context name' => 'route',
  'convert list' => 'route_context_convert_list',
  'convert' => 'route_context_convert',
);
  
/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function route_context_create($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('route');
  $context->plugin = 'route';
  
  // Unpack data parameter
  $active_step = isset($data['active_step'])?$data['active_step']:NULL;
  $subtask_name = isset($data['subtask_name'])?$data['subtask_name']:route_get_current_page_subtask_name();
  $route_data = isset($data['data']) ? $data['data'] : array();
  $reset = isset($data['reset']) && $data['reset'] == TRUE;
  $steps = _route_get_steps($subtask_name);
  
  // Fetch cached data associated with current user's token.
  // We're passing $step data as default, so it will prepopulate the object if it's not cached yet.
  if (!$route = route_get(route_token($subtask_name, $reset))) {
    $route = new Route();
  }

  // Set any route data please.
  if($route_data && !$empty) {
    $route->set_data($route_data);
  }

  // Set steps. This will replace cached steps with factual information about steps available in the panel.
  $route->set_steps($steps);

  // Handle active step if user provided the argument. If no arguments, set active step to NULL.
  $route->active_step = $active_step;

  // Expose current task name inside context.
  $route->current_subtask = $subtask_name;

  // Allow other modules to alter the Route object. For example, module may want to remove a step from progress indicator.
  drupal_alter('route_context', $route);

  // Cache Settings
  route_set(route_token($subtask_name), $route);
  $route = route_get(route_token($subtask_name));
   
  $context->data = $route;
  $context->title = t("Route");
  return $context;
}

/**
 * Provide a list of ways that this context can be converted to a string.
 */
function route_context_convert_list() {
  $list = array(
    'active_step' => t('Currently active step'),
    'total_steps' => t('Total steps in this route')
  );

  // Let other modules inject more elements to this list. This way, modules that implement their own routes can expose their contextual data for Panels and other sorts!
  drupal_alter('route_context_convert_list', $list);

  return $list;
}

/**
 * Convert a context into a string.
 */
function route_context_convert($context, $property) {
  return $context->data->format($property);
}
