<?php
/**
 * @file
 * This file contains the core override functions.
 */

/**
 * Reimplementation of taxonomy_get_tree().
 *
 * Limit db fetch to only specified parent AND use presorting.
 * @see taxonomy_get_tree()
 */
function taxonomy_nested_intervals_core_get_tree($vid, $parent = 0, $max_depth = NULL, $load_entities = FALSE) {
  $distance = 0;
  if ($parent) {
    $point = db_query("SELECT lft, rgt, distance FROM {taxonomy_nested_intervals} WHERE tid = :parent", array(':parent' => $parent))->fetchObject();
    $distance = $point->distance + 1;
  }

  $query = db_select('taxonomy_nested_intervals', 'e');
  $query->join('taxonomy_term_data', 'd', 'd.tid = e.tid');

  $query
    ->addTag('translatable')
    ->addTag('term_access')
    ->fields('e', array('distance'))
    ->fields('d')
    ->condition('e.vid', $vid)
    ->orderBy('e.lft');
  $query->addExpression('e.parent', 'parents');

  if ($parent) {
    $query->where("e.lft > $point->lft AND e.lft < $point->rgt");
  }

  if (isset($max_depth)) {
    $query->condition('e.distance', $max_depth + $distance, '<');
  }

  $query_result = $query->execute();

  $result = array();
  $tree = array();

  foreach ($query_result as $term) {
    $term->depth = $term->distance - $distance;
    unset($term->distance);
    if (!isset($tree[$term->tid])) {
      $tree[$term->tid] = array($term->parents);
    }
    else {
      $tree[$term->tid][] = $term->parents;
      $tree[$term->tid] = array_unique($tree[$term->tid]);
    }
    $term->parents = &$tree[$term->tid];
    $result[] = $term;
  }

  // Load full entities, if necessary. The entity controller statically
  // caches the results.
  if ($load_entities) {
    $term_entities = taxonomy_term_load_multiple(array_keys($tree));
  }

  // Free memory for $tree.
  unset($tree);
  $tree = &$result;

  // Stash entities into tree if necessary.
  if ($load_entities) {
    foreach ($tree as &$term) {
      // Store values depth and parents, because the entity which overwrites
      // does not carry these.
      $t_depth = $term->depth;
      $t_parents = $term->parents;
      $term = $term_entities[$term->tid];
      // Re-add depth and parents to the term (entity).
      $term->depth = $t_depth;
      $term->parents = $t_parents;
    }
    unset($term_entities);
  }

  return $tree;
}

/**
 * Reimplementation of taxonomy_select_nodes() allowing depth query.
 */
function taxonomy_nested_intervals_core_select_nodes($tid, $pager = TRUE, $limit = FALSE, $depth = 0, $order = NULL) {
  if (!variable_get('taxonomy_maintain_index_table', TRUE)) {
    return array();
  }

  // If no depth, just use core (faster query due to no joins).
  if ($depth == 0) {
    $args = isset($order) ? array($tid, $pager, $limit, $order) : array($tid, $pager, $limit);
    return call_user_func_array('taxonomy_select_nodes', $args);
  }

  // Set default order if not specified.
  if (!isset($order)) {
    $order = array(
      'n.sticky' => 'DESC',
      'n.created' => 'DESC',
      'n.nid' => 'DESC'
    );
  }

  // Ensure that valid (but ridiculous?) usage doesn't break query.
  unset($order['t.tid']);

  // Lookup point for better query on nested_intervals table.
  $point = db_query_range("SELECT vid, lft, rgt, distance FROM {taxonomy_nested_intervals} WHERE tid = :tid", 0, 1, array(':tid' => $tid))->fetchObject();
  if (!$point) {
    return FALSE;
  }

  // Locate nodes.
  $subquery = db_select('taxonomy_index', 'i');
  $subquery->join('taxonomy_nested_intervals', 'e', 'e.tid = i.tid');
  $subquery->condition('e.vid', $point->vid);
  $subquery->where("e.lft >= $point->lft AND e.lft < $point->rgt");
  $subquery->condition('e.distance', $depth + $point->distance, '<=');
  $subquery->fields('i', array('nid'));

  // Fetch nodes.
  $query = db_select('node', 'n');
  $query->addTag('node_access');
  $query->condition('n.nid', $subquery, 'IN');
  if ($pager) {
    $count_query = db_select('taxonomy_index', 't');
    $count_query->join('taxonomy_nested_intervals', 'e', 'e.tid = t.tid');
    $count_query->condition('e.vid', $point->vid);
    $count_query->where("e.lft >= $point->lft AND e.lft < $point->rgt");
    $count_query->condition('e.distance', $depth + $point->distance, '<=');
    $count_query->addExpression('COUNT(DISTINCT t.nid)');

    $query = $query->extend('PagerDefault');
    if ($limit !== FALSE) {
      $query = $query->limit($limit);
    }
    $query->setCountQuery($count_query);
  }
  else {
    if ($limit !== FALSE) {
      $query->range(0, $limit);
    }
  }
  $query->addField('n', 'nid');
  foreach ($order as $field => $direction) {
    $query->orderBy($field, $direction);
    // ORDER BY fields need to be loaded too, assume they are in the form
    // table_alias.name
    list($table_alias, $name) = explode('.', $field);
    $query->addField($table_alias, $name);
  }
  return $query->execute()->fetchCol();
}
