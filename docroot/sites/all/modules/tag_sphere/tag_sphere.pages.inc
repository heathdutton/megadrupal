<?php

/**
 * @file
 * Get the Tag sphere block content.
 */

/**
 * This function is used to fetch the terms of particular vocabulary.
 */
function tag_sphere_block_content($vid, $name) {
  $items = array();

  $query = db_select('taxonomy_index', 'ti');
  $query->join('taxonomy_term_data', 'ttd', 'ti.tid = ttd.tid');
  $query->fields('ttd', array('tid', 'name'));
  $query->addExpression('COUNT(ti.nid)', 'count');
  $query->condition('ttd.vid', $vid, '=');
  $query->groupBy('ttd.tid');
  $query->orderBy('count', 'ASC');
  $result = $query->execute()->fetchAll();

  foreach ($result as $val) {
    $term_alias = drupal_get_path_alias('taxonomy/term/' . $val->tid);
    $term_name = l(t('!term_name', array('!term_name' => $val->name)),
      $term_alias,
        array(
          'attributes' => array(
            'rel' => $val->count,
          ),
        )
    );
    $items[$val->tid] = $term_name;
  }
  return theme('item_list', array('items' => $items));
}
