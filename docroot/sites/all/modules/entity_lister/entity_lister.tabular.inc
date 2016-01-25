<?php

/**
 * @file
 * Functions for tabular lists.
 */

/**
 * Add js and css for a tabular list.
 *
 * See the Entity Lister Example module for an example of how to use this
 * function: https://drupal.org/project/entity_lister_example
 *
 * @param array $array
 *   The render array for the page.
 * @param array $table_sort
 *   The table_sort array.
 */
function entity_lister_table_sort(&$array, $table_sort) {

  $array['#attached']['js'][] = array(
    'data' => drupal_get_path('module', 'entity_lister') . '/table-sort.js',
  );
  $array['#attached']['css'][] = array(
    'data' => drupal_get_path('module', 'entity_lister') . '/table-sort.css',
  );

  $js_settings['table_sort'] = $table_sort;

  $array['#attached']['js'][] = array(
    'data' => array('entity_lister' => $js_settings),
    'type' => 'setting',
  );

}

/**
 * Theme the table headers for a tabular list.
 *
 * See the Entity Lister Example module for an example of how to use this
 * function: https://drupal.org/project/entity_lister_example
 */
function theme_entity_lister_headers($vars) {

  if (isset($vars['filter_form']) && isset($vars['filter_form']['filters'])) {
    // Add any current filter values to the column header query strings.
    foreach (element_children($vars['filter_form']['filters']) as $name) {
      if (isset($_GET[$name])) {
        $options['query'][$name] = $_GET[$name];
      }
    }
  }

  // This should match the sort direction in $table_sort.
  $options['query']['sort'] = $vars['dir'];
  $arrows = '<span class="asc">&#916;</span><span class="desc">&#8711;</span>';

  $output = '<tr>' . "\n";

  foreach ($vars['headers'] as $id => $value) {
    $options['query']['order'] = $value['label'];
    $output .= ' <th id="' . check_plain($id) . '">';
    if ($value['sort']) {
      $output .= l($value['label'], $_GET['q'], $options) . $arrows;
    }
    else {
      $output .= $value['label'];
    }
    $output .= '</th>' . "\n";
  }

  $output .= '</tr>' . "\n";

  return $output;

}
