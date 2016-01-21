<?php

/**
 * @file
 * Bulk deletion of nodes.
 */

/**
 * Form builder; Return form for deletion of nodes.
 */
function entitybulkdelete_nodes($form, &$form_state) {
  // Display page 2 if $form_state['page_num'] == 2
  if (!empty($form_state['page_num']) && $form_state['page_num'] == 2) {
    return entitybulkdelete_nodes_confirm_form($form, $form_state);
  }
  $form_state['page_num'] = 1;
  // Deletes nodes by node's titles.
  $form['del_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete nodes, by title'),
    '#ajax' => array(
      'callback' => 'entitybulkdelete_title_callback',
      'wrapper' => 'del-title',
    ),
  );
  $form['title_placeholder'] = array(
    '#type' => 'hidden',
    '#prefix' => '<div id="del-title">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  if (!empty($form_state['values']['del_title']) && $form_state['values']['del_title']) {
    $form['title_placeholder']['#type'] = 'fieldset';
    $form['title_placeholder']['#title'] = 'Delete nodes by title';
    $content = entitybulkdelete_node_type(FALSE);
    $content['_none'] = "Select content";
    // Get value from $form_state['values']
    $sel_node_type = isset($form_state['values']['title_placeholder']['node_type']) ? $form_state['values']['title_placeholder']['node_type'] : '_none';
    $form['title_placeholder']['node_type'] = array(
      '#title' => 'Content type',
      '#type' => 'select',
      '#default_value' => '_none',
      '#options' => $content,
      '#ajax' => array(
        'callback' => 'entitybulkdelete_nodetype_callback',
        'wrapper' => 'nodes-title',
      ),
    );
    if (!empty($sel_node_type) && isset($sel_node_type)) {
      if ($sel_node_type != '_none') {
        $node_list = entitybulkdelete_get_node($sel_node_type, FALSE);
        $form_state['nodes'] = $node_list;
      }
    }
    $node_list['_none'] = "Select nodes";
    $form['title_placeholder']['nodes'] = array(
      '#title' => 'Nodes title',
      '#type' => 'select',
      '#options' => $node_list,
      '#multiple' => TRUE,
      '#prefix' => '<div id="nodes-title">',
      '#suffix' => '</div>',
    );
  }
  // Deletes nodes by node's nid.
  $form['del_nid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete nodes, by nid'),
    '#ajax' => array(
      'callback' => 'entitybulkdelete_nid_callback',
      'wrapper' => 'del-nid',
    ),
  );
  $form['nid_placeholder'] = array(
    '#type' => 'hidden',
    '#prefix' => '<div id="del-nid">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  if (!empty($form_state['values']['del_nid']) && $form_state['values']['del_nid']) {
    $form['nid_placeholder']['#type'] = 'fieldset';
    $form['nid_placeholder']['#title'] = 'Delete nodes by nid';
    $form['nid_placeholder']['nid'] = array(
      '#title' => 'Node id',
      '#type' => 'textarea',
      '#rows' => 6,
      '#cols' => 40,
      '#description' => t("Enter node's nid separate by ','."),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Delete',
    '#submit' => array('entitybulkdelete_nodes_submit'),
  );
  return $form;
}

/**
 * Submit handler for the node deletion form.
 */
function entitybulkdelete_nodes_submit($form, &$form_state) {
  $valid_rows = array();
  $del_by_title = $form_state['values']['del_title'];
  if ($del_by_title) {
    $sel_nid = $form_state['values']['title_placeholder']['nodes'];
    if (isset($form_state['nodes']) && !empty($form_state['nodes'])) {
      $nodes = $form_state['nodes'];
      foreach ($sel_nid as $key => $value) {
        if (array_key_exists($key, $nodes)) {
          $valid_rows[$key] = array('nid' => $key, 'title' => check_plain($nodes[$key]));
        }
      }
    }
  }
  $del_by_nid = $form_state['values']['del_nid'];
  if ($del_by_nid) {
    $find = array(' ', '-', '+');
    $temp_nodes = check_plain($form_state['values']['nid_placeholder']['nid']);
    if (!empty($temp_nodes)) {
      $nodes_bynid = str_replace($find, '', $temp_nodes);
      $nodes_bynid = explode(',', $nodes_bynid);
      if (!empty($nodes_bynid)) {
        foreach ($nodes_bynid as $key => $value) {
          if (!is_numeric($value)) {
            unset($nodes_bynid[$key]);
          }
          else {
            $temp_key = (int) $value;
            $node_nid[$temp_key] = $temp_key;
          }
        }
        // Get list of node's nid and title.
        $query = db_select('node', 'n');
        $query->fields('n', array('nid', 'title'));
        $query->condition('nid', $node_nid, 'IN');
        $node_list = $query->execute()->fetchAllKeyed(0, 1);
        foreach ($node_list as $key => $value) {
          $valid_rows[$key] = array('nid' => $key, 'title' => check_plain($value));
        }
      }
    }
  }
  if (!empty($valid_rows)) {
    $form_state['valid_rows'] = $valid_rows;
    $form_state['page_num'] = 2;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Form builder; Return confirmation form for nodes deletion.
 */
function entitybulkdelete_nodes_confirm_form($form, &$form_state) {
  $header['nid'] = 'Nid';
  $header['title'] = 'Title';
  $valid_row = $form_state['valid_rows'];
  $form['msg'] = array(
    '#type' => 'markup',
    '#markup' => t('Are you sure you want to deletes these nodes ?') .
    '<br>' . t('This action cannot be undone.') . '<br>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Confirm',
    '#submit' => array('entitybulkdelete_nodes_confirm_form_submit'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => 'Cancel',
    '#submit' => array('entitybulkdelete_nodes_cancel_form_submit'),
  );
  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $valid_row,
    '#empty' => t('No Valid Row(s).'),
  );
  return $form;
}

/**
 * Submit handler(Cancel) of confirmation form for nodes deletion.
 */
function entitybulkdelete_nodes_cancel_form_submit($form, &$form_state) {
  $form_state['page_num'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler(Confirm) of confirmation form for nodes deletion.
 */
function entitybulkdelete_nodes_confirm_form_submit($form, &$form_state) {
  $table = $form_state['values']['table'];
  $table = array_flip($table);
  if (array_key_exists('0', $table)) {
    unset($table[0]);
  }
  if (!empty($table)) {
    $count = count($table);
    $operations = array();
    for ($i = 0; $i < $count; $i = $i + 5) {
      // Set up an operations array with 5 elements, each doing function
      // entitybulkdelete_node_delete_batch.
      $batch_tbl = array_slice($table, $i, 5);
      if (!empty($batch_tbl)) {
        $operations[] = array('entitybulkdelete_node_delete_batch',
          array($batch_tbl, &$form_state));
      }
    }
    // Execute the batch.
    $batch = array(
      'operations' => $operations,
      'title' => t('Node Delete Batch'),
      'init_message' => t('Node(s) deleting is in process...'),
      'progress_message' => t('Processed @current*5 out of @total*5.'),
      'finished' => 'entitybulkdelete_node_batch_finished',
      'file' => drupal_get_path('module', 'entitybulkdelete') . '/entitybulkdelete.nodes.inc',
    );
    batch_set($batch);
  }
  $form_state['page_num'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 * Batch operation entitybulkdelete_node_delete_batch.
 *
 * Delete nodes, 5 by five after each group of 5 control
 * is returned to the batch API for later continuation.
 */
function entitybulkdelete_node_delete_batch($table, $form_state, &$context) {
  if (!empty($table)) {
    foreach ($table as $value) {
      $nids[$value] = $value;
      watchdog('entitybulkdelete', 'Node %nid marked as deleted.', array('%nid' => $value), WATCHDOG_NOTICE);
    }
    // Deletes nodes.
    node_delete_multiple($nids);
  }
}

/**
 * Batch 'finished' callback used by entitybulkdelete_node_batch.
 */
function entitybulkdelete_node_batch_finished($success, $results, $operations) {
  if ($success) {
    $msg = t('Node(s) %success has been deleted.');
    watchdog('entitybulkdelete', $msg, array('%success' => 'successfuly'), WATCHDOG_NOTICE);
  }
  else {
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing
      @operation with arguments : @args', array(
        '@operation' => $error_operation[0],
        '@args' => print_r($error_operation[0], TRUE))));
  }
}

/**
 * Fetch list of content types.
 *
 * @param bool $reset
 *   Contain boolean value.
 *
 * @return array $content
 *   List of content type.
 */
function entitybulkdelete_node_type($reset = FALSE) {
  $content = &drupal_static(__FUNCTION__, NULL, $reset);
  if (!isset($content)) {
    if (!$reset && ($cache = cache_get("entitybulkdelete:node type")) && !empty($cache->content)) {
      $content = $cache->content;
    }
    else {
      // Get list of content type.
      $cont_type = node_type_get_types();
      foreach ($cont_type as $key => $value) {
        $content[$key] = check_plain($value->name);
        asort($content);
      }
      cache_set("entitybulkdelete:node type", $content);
    }
  }
  return $content;
}

/**
 * Fetch list of nodes.
 *
 * @param string $node_type
 *   Content type name.
 * @param bool $reset
 *   Contain boolean value.
 *
 * @return array $node_list
 *   List of nodes.
 */
function entitybulkdelete_get_node($node_type, $reset = FALSE) {
  $node_list = &drupal_static(__FUNCTION__, NULL, $reset);
  if (!isset($node_list)) {
    if (!$reset && ($cache = cache_get("entitybulkdelete:node list")) && !empty($cache->node_list)) {
      $node_list = $cache->node_list;
    }
    else {
      // Get list of node from specific content type.
      $query = db_select('node', 'n');
      $query->fields('n', array('nid', 'title'));
      $query->condition('type', $node_type);
      $node_list = $query->execute()->fetchAllKeyed(0, 1);
      cache_set("entitybulkdelete:node list", $node_list);
    }
  }
  return $node_list;
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The title_placeholder element.
 */
function entitybulkdelete_title_callback($form, $form_state) {
  return $form['title_placeholder'];
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The nodes element.
 */
function entitybulkdelete_nodetype_callback($form, $form_state) {
  return $form['title_placeholder']['nodes'];
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The nid_placeholder element.
 */
function entitybulkdelete_nid_callback($form, $form_state) {
  return $form['nid_placeholder'];
}
