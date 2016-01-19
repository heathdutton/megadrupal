<?php

/**
 * @file
 * Contains function to determine previous/next node.
 */

/**
 * Returns previous/next node.
 *
 * The query is dynamically constructed. It should be:
 * select nid from {node} where status = :status
 * and type = :type and (created < :created or sticky < :sticky)
 * order by sticky desc, created desc
 * for the previous and
 * select nid from {node} where status = :status
 * and type = :type and (created > :created or sticky > :sticky)
 * order by sticky desc, created desc
 * for the next node
 *
 * @param int $node
 *   The current node
 * @param bool $back
 *   Which node to return, previous or next. Default is next
 *
 * @return mixed
 *   Node or FALSE if there are no more nodes in the sequence
 */
function _sequential_nodes_page_link($node, $back = FALSE) {

  // Dynamically construct the query depending of what node we want to get.
  $or_field = '';
  $preferred = variable_get('sequential_nodes_preferable', 1);

  switch ($preferred) {
    case 1:
      $or_field = 'n.sticky';
      $compare = $node->sticky;
      break;

    case 2:
      $or_field = 'n.promote';
      $compare = $node->promote;
      break;
  }

  $query = db_select('node', 'n')
  ->fields('n', array('nid'))
  ->condition('n.type', $node->type)
  ->condition('n.status', $node->status)
  // Add node_access tag to the query to avoid node access bypass.
  ->addTag('node_access')
  // #2021011 by chx: Performance problems.
  ->range(0, 1);
  if (empty($or_field)) {
    $query = $query->condition('n.created', $node->created, $back ? '<' : '>');
  }
  else {
    $or = db_or()->condition($or_field, $compare, $back ? '<' : '>')
    ->condition(db_and()->condition('n.created', $node->created, $back ? '<' : '>')
    ->condition($or_field, $compare, '='));
    $query = $query->condition($or);
    $query = ($back ? $query->orderBy($or_field, 'DESC') : $query->orderBy($or_field, 'ASC'));
  }

  $query = ($back ? $query->orderBy('n.created', 'DESC') : $query->orderBy('n.created', 'ASC'));
  $next_nid = $query->execute()->fetchField();

  if ($next_nid) {
    return node_load($next_nid);
  }
  else {
    return FALSE;
  }
}
