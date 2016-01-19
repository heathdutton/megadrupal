<?php

/**
 * @file
 * Get an array of all menus, with machine name as key
 * and 'pretty' name as value
 *
 * @param $queue_items
 *  Can take a string or single key|value array
 *    String: default value
 *    Array: $queue_name => $menu_name
 * @return array
 *  List of all nodequeues that are not currently assigned.
 */

function nodequeue_menu_mapper_get_available_menus($queue_items) {
  // Get all availible menues and assign them to an array.
  $menus = menu_get_menus();

  // Check to see if we're passing in an array.
  if (is_array($queue_items)) {
    $queue_name = key($queue_items);
    $menu_name = $queue_items[$queue_name];
  }
  else {
    $menu_name = '';
    $queue_name = $queue_items;
  }

  if ($queue_name == 'none' && !empty($menus)) {
    // Get all menus already in use and assign them to an array.
    $assigned_menus = nodequeue_menu_mapper_assigned_nodequeue_menus();

    // Remove nodequeues from return array if they're currently in use.
    if (!empty($assigned_menus)) {
      // Assign pairs to the returned nodequeue array.
      foreach ($assigned_menus as $a_menu => $a_queue) {
        // Skip over nodeques that are assigned to the current menu.
        if ($menu_name == $a_menu) {
          continue;
        }
        elseif (array_key_exists($a_menu, $menus)) {
          unset($menus[$a_menu]);
        }
      }
    }
  }

  return $menus;
}


/**
 * Takes an array of nodequeues, machine name as key and pretty name as value.
 *
 * @param array,string $menu_items
 *   Can take a string or single key|value array
 *   String: default value
 *   Array: $menu_name => $nodqueue_name
 *
 * @return array
 *   List of all nodequeues that are not currently assigned
 */
function nodequeue_menu_mapper_get_nodequeue_array_for_menus($menu_items) {
  $nodequeues = array();

  // Check to see if we're passing in an array.
  if (is_array($menu_items)) {
    $menu_name = key($menu_items);
    $queue_name = $menu_items[$menu_name];
  }
  else {
    $menu_name = '';
    $queue_name = $menu_items;
  }

  // Get all availible nodeques and assign them to an array.
  foreach ($queues = nodequeue_get_qid_map() as $q_mn => $q_id) {
    $nodequeues[$q_mn] = str_replace('_', ' ', $q_mn);
  }

  if ($queue_name != 'none' && !empty($nodequeues)) {
    // Get all nodqueues already in use and assign them to an array.
    $assigned_nodequeues = nodequeue_menu_mapper_assigned_nodequeue_menus();

    // Remove nodequeues from return array if they're currently in use.
    if (!empty($assigned_nodequeues)) {
      // Assign pairs to the returned nodequeue array.
      foreach ($assigned_nodequeues as $a_menu => $a_queue) {
        // Skip over nodeques that are assigned to the current menu.
        if ($menu_name == $a_menu) {
          continue;
        }
        elseif (array_key_exists($a_queue, $nodequeues)) {
          unset($nodequeues[$a_queue]);
        }
      }
    }
  }

  return $nodequeues;
}

/**
 * Nodequeue submission form handling to update mapping to menu.
 */
function nodequeue_mapper_map_nodequeue_form($form, &$form_state) {
  // Check for initial queue name, if there is we are editing a form.
  $qname_check = (isset($form['name']['#value']) ? TRUE : FALSE);

  if ($qname_check) {
    $relationship = db_select('nodequeue_menu_mapper', 'nmm')
      ->fields('nmm', array('associative_id'))
      ->condition('nodequeue_name', $form['name']['#value'], '=')
      ->execute()
      ->fetchField();
  }
  else {
    // There was no initial queue name.
    $relationship = FALSE;
  }

  // If queue name in mapper.
  if ($relationship) {
    // Check that value "none" was NOT passed.
    if ($form_state['input']['relevant_menu'] != 'none') {
      // Update the corresponding row.
      db_update('nodequeue_menu_mapper')
        ->fields(array(
          'menu_name' => $form_state['input']['relevant_menu'],
        ))
        ->condition('associative_id', $relationship)
        ->execute();
    }
    else {
      // Value is none, DELETE row.
      db_delete('nodequeue_menu_mapper')
        ->condition('associative_id', $relationship)
        ->execute();
    }
  }
  else {
    // Queue name not in mapper INSERT! NOTE: New queue name's are assigned.
    // If they select a value, insert it. If none, do not insert!
    if ($form_state['input']['relevant_menu'] != 'none') {
      db_insert('nodequeue_menu_mapper')
        ->fields(array(
          'associative_id' => NULL,
          'nodequeue_name' => $form_state['values']['name'],
          'menu_name'      => $form_state['values']['relevant_menu'],
        ))
        ->execute();
    }
  }
}

/**
 * Menu submission form mapping to assign to select nodequeue.
 */
function nodequeue_mapper_map_menu_form($form, &$form_state) {
  // Check if new menu or not.
  $menu_check = (isset($form_state['values']['old_name']) ? TRUE : FALSE);

  if ($menu_check) {
    // There is an old name, a previous relationship may exist.
    $associative_id = db_select('nodequeue_menu_mapper', 'nmp')
      ->fields('nmp', array('associative_id'))
      ->condition('menu_name', $form_state['values']['old_name'])
      ->execute()
      ->fetchField();
  }
  else {
    $associative_id = FALSE;
  }

  if ($associative_id) {
    // Determine if already an entry in table for this menu.
    if ($form_state['input']['relevant_nodequeue'] == 'none') {
      // If it is set to none, DELETE THE ROW!
      db_delete('nodequeue_menu_mapper')
        ->condition('associative_id', $associative_id)
        ->execute();
    }
    else {
      // Exists in table, needs to be updated!
      db_update('nodequeue_menu_mapper')
        ->fields(array(
          'nodequeue_name' => $form_state['input']['relevant_nodequeue'],
          'menu_name' => $form_state['values']['menu_name'],
        ))
        ->condition('associative_id', $associative_id)
        ->execute();
    }
  }
  else {
    // There is no entry in the database yet!
    if ($form_state['input']['relevant_nodequeue'] != 'none') {
      // If queue not set none, a relationship must be inserted!
      db_insert('nodequeue_menu_mapper')
        ->fields(array(
          'associative_id' => NULL,
          'nodequeue_name' => $form_state['input']['relevant_nodequeue'],
          'menu_name'      => $form_state['values']['menu_name'],
        ))->execute();
    }
  }
}

/**
 *  Call back from nodequeue admin form when a node is added. Gets NID, and adds to correspodning menu
 */
function nodequeue_menu_mapper_add_node_to_nodequeue($form, &$form_state) {
  // Check if nodequeue is in a relationshup with a menu
  if($menu_name = nodequeue_menu_mapper_get_menu_for_nodequeue($form['nodes']['#queue']['name'])){
    // Convert worthless NID submitted value to NID value!
    $nid = explode('[nid:', $form_state['values']['add']['nid']);
    $nid = intval($nid[1]);
    $node = node_load($nid);
    // Add NID to menu...
    $menuItem = array(
      'menu_name'  => $menu_name,
      'weight'     => 50,
      'link_title' => $node->title,
      'link_path'  => 'node/' . $nid,
      'mlid'       => NULL,
    );
    menu_link_save($menuItem);;
  }
}

/**
 * Write nodequeue choice for menu to the database.
 */
function nodequeue_menu_mapper_menu_adjust_submit($form, &$form_state) {
  // Initialize menu order variable.
  $menu_order = array();

  // Loop through all form items until an MLID is found.
  foreach ($form_state['input'] as $input) {
    // If it is an input value concerning a menu link
    // add it to the menu order array.
    if (isset($input['mlid'])) {
      $menu_order[] = array(
        'mlid' => $input['mlid'],
        'nid'  => menu_node_get_node($input['mlid'], FALSE),
      );
    }
  }

  // Get menu name from first menu item, all items will be in same menu.
  $menu_name = db_select('menu_links', 'menu_links')
    ->fields('menu_links', array('menu_name'))
    ->condition('mlid', $menu_order[0]['mlid'])
    ->execute()
    ->fetchField();

  // Get queue name from menu name.
  $queue_name = nodequeue_menu_mapper_get_nodequeue_for_menu($menu_name);

  // Load the nodequeue.
  $nodequeue = nodequeue_load_queue_by_name($queue_name);

  // Get qid from loaded queue.
  $qid = $nodequeue->qid;

  // Get count of nodequeue.
  $queue_count = db_select('nodequeue_nodes', 'nq')
    ->fields('nq', array('nid '))
    ->condition('qid', $qid)
    ->execute()
    ->rowCount();

  // Delete all from queue.
  nodequeue_subqueue_remove($qid, 1, $queue_count);

  // Load nodeuque and subqueue.
  $nq = nodequeue_load($qid);
  $sq = nodequeue_load_subqueue($qid);

  // Add new items to queue.
  foreach ($menu_order as $menu_item) {
    nodequeue_subqueue_add($nq, $sq, $menu_item['nid']);
  }
}

/**
 * Helper function takes an unrelated drupal menu value and return useful value.
 *
 * Kept in one place for ease of maintenance as new nuances are discovered.
 */
function nodequeue_menu_mapper_regex_drupal_menu_name($drupal_menu_name) {
  return preg_replace('/:0/', "$1", $drupal_menu_name);
}

/**
 * Function called on nodequeue deletion.
 */
function nodequeue_menu_mapper_delete_nodequeue($form, &$form_state) {
  $nodequeue = nodequeue_load($form_state['values']['qid']);
  $queue_name = $nodequeue->name;
  db_delete('nodequeue_menu_mapper')
    ->condition('nodequeue_name', $queue_name)
    ->execute();
}

/**
 * Function to hijack nodequeue menu mappers deletion functionality.
 */
function nodequeue_menu_mapper_delete_nodequeue_item($queue, $subqueue, $node) {
  // Get menu name while simultaneously checking if it relates to a nodequeue.
  if ($menu_name = nodequeue_menu_mapper_get_menu_for_nodequeue($queue->name)) {
    // Get all menu links.
    $menu_links = menu_tree_all_data($menu_name);
    // Loop through all links to find correct value.
    foreach ($menu_links as $menu_link) {
      // Remove 'node/' and see if node id remains.
      if (str_replace('node/', '', $menu_link['link']['link_path']) == $node->nid) {
        // Get mlid to delete.
        $mlid_to_delete = $menu_link['link']['mlid'];
      }
    }
    // Delete MLID.
    if (isset($mlid_to_delete)) {
      menu_link_delete($mlid_to_delete);
    }
  }
  // Pass original callback data onwards to original function.
  nodequeue_admin_remove_node($queue, $subqueue, $node);
}
