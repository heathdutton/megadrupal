<?php
// $Id: aggregator_node.inc,v 1.4 2008/07/20 18:06:12 aronnovak Exp $

/**
 * @file
 *   Node operations for aggregator_node
 */

/**
 * Creates/updates a node via drupal_execute().
 *
 * @param $item_node
 *   Node object of the item.
 * @param
 *   Feed node object skeleton for taxonomy copying.
 *   
 * @return
 *   The nid of the newly created node item.
 */
function _aggregator_node_save($item_node, $parent_node = FALSE) {
  $form_state = array();
  module_load_include('inc', 'node', 'node.pages');
  $user = user_load(array('uid' => $item_node->uid));
  $time_str = date('Y-m-d H:i:s O', $item_node->timestamp);
  $node = array('type' => $item_node->type, 'name' => $user->name, 'date' => $time_str);
  if (is_object($parent_node) && !empty($parent_node->taxonomy)) {
    $node['taxonomy'] = $parent_node->taxonomy;
    foreach ($node['taxonomy'] as $k => $v) {
    	if (is_array($v) && $k != 'tags') {
    	  foreach ($v as $t1 => $t2) {
    	    $node['taxonomy'][] = taxonomy_get_term($t2);
    	  }
    	}
    }
  }
  $form_state['values']['title'] = $item_node->title;
  $form_state['values']['body'] = $item_node->body;
  $form_state['values']['name'] = $user->name;
  $form_state['values']['date'] = $time_str;
  if (!empty($item_node->nid)) {
    $node = (array) node_load($item_node->nid);
  }
  $form_state['values']['op'] = t('Save');
  drupal_execute($item_node->type . '_node_form', $form_state, (object)$node);
  return $form_state['nid'];
}

