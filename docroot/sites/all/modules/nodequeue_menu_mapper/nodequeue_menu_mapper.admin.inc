<?php

/**
 * Page callback for deleting an associative ID
 *
 * @param $aid - Integer for associative ID.
 */

function nodequeue_menu_mapper_delete_associative_id($aid) {
  // Delete nodequeue menu association based on passed associative ID.
  if (db_delete('nodequeue_menu_mapper')->condition('associative_id', $aid)->execute()) {
    drupal_set_message('Nodequeue to menu association deleted!', 'status');
  }
  else {
    // Display error if it fails.
    drupal_set_message('Issue deleting nodequeue to menu association!', 'error');
  }
  drupal_goto('admin/structure/nodequeue-menu-mapper');
}

/**
 * Call back function for the nodequeue menu mapper administrative page.
 */
function nodequeue_menu_mapper_admin_page() {
  // Setup table headers.
  $header = array(
    array(
      'data' => t('Nodequeue'),
      'field' => 'nodequeue_name',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Menu Name'),
      'field' => 'menu_name',
    ),
    array(
      'data' => t('Delete Relationship'),
    ),
  );

  $query = db_select('nodequeue_menu_mapper', 'nmm')
    ->extend('TableSort')
    ->orderByHeader($header)
    ->fields('nmm', array(
      'associative_id',
      'nodequeue_name',
      'menu_name',
    ));

  // Get relationship data.
  $results = $query
    ->execute();

  // Establish table data.
  $rows = array();
  foreach ($results as $result) {
    $rows[] = array(
      array('data' => $result->nodequeue_name),
      array('data' => $result->menu_name),
      array('data' => l(t('Delete Association'), 'admin/structure/nodequeue-menu-mapper/delete/' . $result->associative_id)),
    );
  }

  // Output a table of all the views, allow deleting nodequeue relationships.
  $html = theme('table', array(
    'header' => $header,
    'rows' => $rows));

  return $html;
}
