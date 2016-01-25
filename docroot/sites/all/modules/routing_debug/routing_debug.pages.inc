<?php
/**
 * @file
 * File routing_debug.pages.inc contains page callbacks.
 */

/**
 * PAGE CALLBACKS -------------------------------------------------------------.
 */

/**
 * Page callback - generates page with routing table.
 */
function routing_debug_page() {
  // Let's get the data we're going to be processing first.
  $menu_router = menu_get_router();

  // Table rows for output table.
  $rows = array();

  // We need to process each record from menu router,
  // extract additional information and prepare it for output.
  foreach ($menu_router as $routing_item_name => $routing_item) {
    /* @var $routing_info RoutingInfo */
    $routing_info = _routing_debug_get_path_info($routing_item_name, $routing_item);

    // First part - we generate URL to /devel/menu/item of the route.
    $complex_urls = '<a href=';
    $complex_urls .= url("devel/menu/item", array('query' => array('path' => $routing_item_name)));
    $complex_urls .= '>/' . $routing_item_name . '</a>';

    /* And if the URL is valid path, we also generate an actual link to it.
     * The '&#8594;' is the same as $rarr; (right array) which doesn't seem
     * to work here. */
    (drupal_valid_path($routing_item_name, FALSE)) ?
      ($complex_urls .= ' <a href=' . url($routing_item_name) . '>&#8594;</a>') : NULL;

    // Let's prepare the array structure for the table.
    $row = array(
      'path' => $complex_urls,
      'callback' => '<em>' . $routing_info->getCallback() . '();</em>',
      'module' => '<strong>' . $routing_item['module'] . '</strong>',
      'file' => $routing_info->getFile(),
      'line' => '<span class="routing-debug-noselect">:</span><span>' . $routing_info->getLine() . '</span>',
    );

    $rows[$routing_item_name] = $row;
  }

  // Header of the table.
  $header = array(
    'path' => t('Route (path)'),
    'callback' => t('Callback'),
    'module' => t('Module'),
    'file' => t('File'),
    'line' => t('Line'),
  );

  // And finally the whole table.
  $variables = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => TRUE,
    'empty' => t('No routes available.'),
  );

  // Theme it as nice Drupal table.
  $output = theme('table', $variables);

  return $output;
}

/**
 * Routing debug - Debug page callback.
 */
function routing_debug_debug_page() {

  $menu_names = menu_get_names();
  $output = kprint_r($menu_names, TRUE, t('Menu names'));
  $menu_router = menu_get_router();
  $output .= kprint_r($menu_router, TRUE, t('Menu router'));
  $system_menus = menu_list_system_menus();
  $output .= kprint_r($system_menus, TRUE, t('System menus'));

  return $output;
}
