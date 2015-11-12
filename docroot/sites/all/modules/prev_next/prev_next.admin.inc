<?php

/**
 * @file
 * Creates the administrative interface for previous next.
 */

/**
 * Menu callback argument. Creates the prev_next administration form.
 */
function prev_next_admin($form, &$form_state) {
  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Indexing status'),
  );

  $max_nid = variable_get('prev_next_index_nid', 0);
  $cond =  _prev_next_node_types_sql();

  $total = db_query("SELECT COUNT(nid) FROM {node} WHERE status = 1 $cond")->fetchField();
  $completed = db_query("SELECT COUNT(nid) FROM {prev_next_node}")->fetchField();
  $remaining = max(0, $total - $completed);

  $percentage = ((int) min(100, 100 * ($total - $remaining) / max(1, $total))) . '%';
  $status = t('<p>%percentage of nodes have been indexed. There are %remaining items left to index, out of a total of %total.</p>',
    array(
    '%percentage' => $percentage,
    '%remaining' => $remaining,
    '%total' => $total,
  ));
  $status .= ($max_nid) ? t('<p>Max node ID for indexing on the next cron run: @max.</p>', array('@max' => $max_nid)) : t('<p>Existing nodes have finished prev/next indexing.</p>');

  $form['status']['#description'] = $status;

  $form['status']['reindex'] = array(
    '#type' => 'submit',
    '#value' => t('Re-index'),
  );

  $form['prev_next_batch_size'] = array(
    '#title' => t('Batch size'),
    '#description' => t('Number of nodes to index during each cron run.'),
    '#type' => 'textfield',
    '#size' => 6,
    '#maxlength' => 7,
    '#default_value' => variable_get('prev_next_batch_size', PREV_NEXT_BATCH_SIZE_DEFAULT),
    '#required' => TRUE,
  );

  $form['prev_next_num_blocks'] = array(
    '#title' => t('Blocks'),
    '#description' => t('Number of blocks available.'),
    '#type' => 'textfield',
    '#size' => 2,
    '#maxlength' => 3,
    '#default_value' => variable_get('prev_next_num_blocks', PREV_NEXT_NUM_BLOCKS_DEFAULT),
    '#required' => TRUE,
  );

  $form['node_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#description' => t('Define settings for each content type. If none of them is included, then all of them will be.'),
  );

  foreach (node_type_get_types() as $type => $name) {

    $form['node_types'][$type] = array(
      '#type' => 'fieldset',
      '#description' => t('Note: changing one of these values will reset the entire Prev/Next index.'),
      '#title' => node_type_get_name($type),
      '#collapsible' => TRUE,
      '#collapsed' => !variable_get(PREV_NEXT_NODE_TYPE . $type, 0),
    );

    $form['node_types'][$type][PREV_NEXT_NODE_TYPE . $type] = array(
      '#type' => 'checkbox',
      '#title' => t('Include'),
      '#default_value' => variable_get(PREV_NEXT_NODE_TYPE . $type, 0),
    );
    $form['node_types'][$type][PREV_NEXT_NODE_TYPE . $type . '_current'] = array(
      '#type' => 'hidden',
      '#default_value' => variable_get(PREV_NEXT_NODE_TYPE . $type, 0),
    );

    $form['node_types'][$type][PREV_NEXT_NODE_TYPE . $type . '_indexing_criteria'] = array(
      '#title' => t('Indexing criteria'),
      '#type' => 'select',
      '#options' => array(
        'nid' => t('Node ID'),
        'created' => t('Post date'),
        'changed' => t('Updated date'),
        'title' => t('Title'),
      ),
      '#default_value' => variable_get(PREV_NEXT_NODE_TYPE . $type . '_indexing_criteria', PREV_NEXT_INDEXING_CRITERIA_DEFAULT),
    );
    $form['node_types'][$type][PREV_NEXT_NODE_TYPE . $type . '_indexing_criteria_current'] = array(
      '#type' => 'hidden',
      '#value' => variable_get(PREV_NEXT_NODE_TYPE . $type . '_indexing_criteria', PREV_NEXT_INDEXING_CRITERIA_DEFAULT),
    );

    $form['node_types'][$type][PREV_NEXT_NODE_TYPE . $type . '_same_type'] = array(
      '#type' => 'checkbox',
      '#title' => t('Only nodes with same content type'),
      '#default_value' => variable_get(PREV_NEXT_NODE_TYPE . $type . '_same_type', 0),
    );
    $form['node_types'][$type][PREV_NEXT_NODE_TYPE . $type . '_same_type_current'] = array(
      '#type' => 'hidden',
      '#default_value' => variable_get(PREV_NEXT_NODE_TYPE . $type . '_same_type', 0),
    );

    $categories = array();
    $instances = field_info_instances('node', $type);
    foreach ($instances as $instance) {
      $field_info = field_info_field($instance['field_name']);
      if ($field_info['type'] == 'taxonomy_term_reference') {
        $categories[$instance['field_name']] = $instance['label'];
      }
    }
    if (count($categories) > 0) {
      $form['node_types'][$type][PREV_NEXT_NODE_TYPE . $type . '_category_criteria'] = array(
          '#title' => t('Category criteria'),
          '#type' => 'select',
          '#options' => array(
              'none' => t('None')
          ) + $categories,
          '#default_value' => variable_get(PREV_NEXT_NODE_TYPE . $type . '_category_criteria', 'none'),
          '#states' => array(
              'visible' => array(
                  ':input[name=' . PREV_NEXT_NODE_TYPE . $type . '_same_type]' => array('checked' => TRUE),
              ),
          ),
      );
    }
    $form['node_types'][$type][PREV_NEXT_NODE_TYPE . $type . '_category_criteria_current'] = array(
        '#type' => 'hidden',
        '#default_value' => variable_get(PREV_NEXT_NODE_TYPE . $type . '_category_criteria', 'none'),
    );

  }

  $form['#submit'][] = 'prev_next_admin_submit';

  return system_settings_form($form);
}

/**
 * Validate callback.
 */
function prev_next_admin_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Re-index')) {
    drupal_goto('admin/config/system/prev_next/re-index');
  }

  // Max_nid is just a markup field and should not cause a variable to be set.
  unset($form_state['values']['max_nid']);

  // The variables must be non-negative and numeric.
  if (!is_numeric($form_state['values']['prev_next_batch_size']) || $form_state['values']['prev_next_batch_size'] <= 0) {
    form_set_error('prev_next_batch_size', t('The batch size must be a number and greater than zero.'));
  }
}

/**
 * Submit callback.
 */
function prev_next_admin_submit($form, &$form_state) {
  $rebuild = FALSE;

  // Test sensitive values.
  foreach (node_type_get_types() as $type => $name) {
    if ($form_state['values'][PREV_NEXT_NODE_TYPE . $type . '_current'] != $form_state['values'][PREV_NEXT_NODE_TYPE . $type] || $form_state['values'][PREV_NEXT_NODE_TYPE . $type . '_indexing_criteria_current'] != $form_state['values'][PREV_NEXT_NODE_TYPE . $type . '_indexing_criteria'] || $form_state['values'][PREV_NEXT_NODE_TYPE . $type . '_same_type_current'] != $form_state['values'][PREV_NEXT_NODE_TYPE . $type . '_same_type'] || (isset($form_state['values'][PREV_NEXT_NODE_TYPE . $type . '_category_criteria']) && $form_state['values'][PREV_NEXT_NODE_TYPE . $type . '_category_criteria_current'] != $form_state['values'][PREV_NEXT_NODE_TYPE . $type . '_category_criteria'])
    ) {
      $rebuild = TRUE;
    }
  }

  // If the search criterias has been changed, re-index.
  if ($rebuild) {
    prev_next_reindex();
    drupal_set_message(t('The Prev/Next index will be rebuilt.'));
  }
  $form_state['redirect'] = 'admin/config/system/prev_next';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function prev_next_reindex_confirm($form, &$form_state) {
  return confirm_form(array(), t('Are you sure you want to re-index Prev/Next?'),
    'admin/config/system/prev_next', t('The entire Prev/Next index will be reset and rebuilt incrementally as cron runs. action cannot be undone.'), t('Re-index'), t('Cancel'));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function prev_next_reindex_confirm_submit(&$form, &$form_state) {
  if ($form['confirm']) {
    prev_next_reindex();
    drupal_set_message(t('The Prev/Next index will be rebuilt.'));
    $form_state['redirect'] = 'admin/config/system/prev_next';
  }
}
