<?php

/**
 * Implements hook_drush_help().
 */
function tracdelight_drush_help($command) {
  switch ($command) {
    case 'drush:tracdelight-import':
      return dt('Imports products');
  }
}

/**
 * Implements hook_drush_command().
 */
function tracdelight_drush_command() {
  $items = array();
  $items['tracdelight-import'] = array(
    'description' => dt('Imports products.'),
    'arguments' => array(
      'query' => dt('SearchQuery'),
      'items' => dt('Items'),
    ),
    'aliases' => array('ti'),
  );
  return $items;
}

function drush_tracdelight_import($query = NULL, $items = 10) {

  if (!isset($query)) {
    drush_print('You need to specify a search query');
    return;
  }

  try {
    tracdelight_import_products(array('Query' => $query), $items);
  } catch (Exception $e) {

    drush_set_error('error', $e->getMessage());
  }
}

