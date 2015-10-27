<?php

/**
 * @file
 * The callback for taxonomy_manager_lite module.
 */

/**
 * Form to delete empty topics.
 */
function taxonomy_manager_lite_delete_empty_terms_form($form, &$form_state) {
  $vocabulary_list = taxonomy_manager_lite_get_vocabulary_list();
  $form['vocabulary_name'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#size' => 1,
    '#options' => $vocabulary_list,
    '#ajax' => array(

      // #ajax has two required keys: callback and wrapper.
      // 'callback' is a function that will be called when this element changes.
      'callback' => 'taxonomy_manager_lite_fill_vocabulary_terms',

      // 'wrapper' is the HTML id of the page element that will be replaced.
      'wrapper' => 'fill_taxonomy_details_div',
    ),
    '#prefix' => '<div id="fill_taxonomy_details_div">',
  );
  $vid = NULL;
  if (!empty($form_state['values']['vocabulary_name'])) {
    $machine_name = $form_state['selected_vocabulary'] = $form_state['values']['vocabulary_name'];
    $vid = taxonomy_manager_lite_get_vid_from_machine_name($machine_name);
    $form_state['selected_vocabulary'] = $vid;
  }
  $rows = _taxonomy_manager_lite_get_empty_terms($vid);
  $header = array('Taxonomy Id', 'Name');
  $form['message'] = array(
    '#type' => 'markup',
    '#markup' => '<strong>' . t('The following topics will be deleted') . '</strong>',
  );
  $form['table'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'empty' => t('All topics in the selected vocabulary has been assigned to a node.'),
      )
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * Function to get vocabulary id from machine name.
 *
 * @param $machine_name
 *   Machine name of the taxonomy vocabulary.
 *
 * @return $vid
 *   Vocabulary id of the taxonomy vocabulary.
 */
function taxonomy_manager_lite_get_vid_from_machine_name($machine_name) {
  $vid = db_query("SELECT vid FROM {taxonomy_vocabulary}
    WHERE machine_name = :machine_name",
    array(':machine_name' => $machine_name))->fetchField();
  return $vid;
}

/**
 * Submit handler for taxonomy_manager_lite_delete_empty_terms form.
 *
 * Deletes topics not associated with any nodes.
 */
function taxonomy_manager_lite_delete_empty_terms_form_submit($form, &$form_state) {
  $count = 0;
  $vid = ($form_state['selected_vocabulary']) ? $form_state['selected_vocabulary'] : NULL;
  $rows = _taxonomy_manager_lite_get_empty_terms($vid);
  foreach ($rows as $row) {
    taxonomy_term_delete($row[0]);
    drupal_set_message(check_plain('Deleted topic - ' . $row[1]));
    $count++;
  }
  drupal_set_message(check_plain('Deleted ' . $count . ' topics which were not associated with any nodes.'));
}

/**
 * Finds topics not associated with nodes.
 *
 * @param $vid
 *   Vocabulary id of the taxonomy vocabulary.
 *
 * @return $row
 *   Array of taxonomy terms associated with the vocabulary.
 *   If $vid is NULL, returns all taxonomy terms in the system.
 */
function _taxonomy_manager_lite_get_empty_terms($vid) {
  $query = "SELECT td.tid, td.name
          FROM
            taxonomy_term_data td
            LEFT JOIN taxonomy_index t ON td.tid = t.tid
          WHERE ";
  if ($vid) {
    $query .= "td.vid = " . $vid . " AND ";
  }
  $query .= "t.tid IS NULL";
  $result = db_query($query);
  $row = array();
  foreach ($result as $term) {
    $row[] = array($term->tid, $term->name);
  }
  return $row;
}

/**
 * Function to get all taxonomy vocabulary in a system.
 *
 * @return $vocabulary_list
 *   Array of vocabulary in a system.
 *   Machine name as key and name as values.
 */
function taxonomy_manager_lite_get_vocabulary_list() {
  $taxonomy_vocabularies = taxonomy_get_vocabularies();
  $vocabulary_list = array(0 => '-Select Vocabulary-');
  foreach ($taxonomy_vocabularies as $taxonomy_vocabulary) {
    $vocabulary_list[$taxonomy_vocabulary->machine_name] = $taxonomy_vocabulary->name;
  }
  if ($vocabulary_list == NULL) {
    drupal_set_message("No Vocabulary has been added yet!", 'status');
  }
  return $vocabulary_list;
}

/**
 * Utility functionality.
 *
 * To reassign terms to nodes which have been assigned a set of terms.
 */
function taxonomy_manager_lite_reassign_terms_form($form, &$form_state) {
  $vocabulary_list = taxonomy_manager_lite_get_vocabulary_list();
  $form['vocabulary_name'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#size' => 1,
    '#options' => $vocabulary_list,
    '#ajax' => array(

      // #ajax has two required keys: callback and wrapper.
      // 'callback' is a function that will be called when this element changes.
      'callback' => 'taxonomy_manager_lite_fill_vocabulary_terms',

      // 'wrapper' is the HTML id of the page element that will be replaced.
      'wrapper' => 'fill_taxonomy_details_div',
    ),
    '#prefix' => '<div id="fill_taxonomy_details_div">',
  );
  $form['terms'] = array(
    '#type' => 'select',
    '#title' => t('Current Terms'),
    '#size' => 1,
    '#multiple' => TRUE,
    '#options' => array('-Select a Vocabulary to view terms-'),
  );
  $taxonomy = array();
  if (!empty($form_state['values']['vocabulary_name'])) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($form_state['values']['vocabulary_name']);
    if ($terms = taxonomy_get_tree($vocabulary->vid)) {
      foreach ($terms as $term) {
        $taxonomy[$term->tid] = $term->name;
      }
    }
    $form['terms']['#size'] = 10;
    $form['terms']['#options'] = $taxonomy;
  }
  $form['terms_condition'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show nodes with any term instead of all terms'),
  );
  $form['preview'] = array(
    '#type' => 'submit',
    '#value' => t('Preview'),
    '#validate' => array('taxonomy_manager_lite_reassign_terms_preview'),
  );

  // Show assign only after preview.
  if (isset($form_state['values']['terms'])) {
    $current_terms = '';
    foreach ($form_state['values']['terms'] as $term) {
      if (is_numeric($term)) {
        if ($current_terms <> '') {
          $current_terms .= ', ';
        }
        $current_terms .= $taxonomy[$term];
      }
    }
    $form['current_terms'] = array(
      '#value' => t('<br /><br />Currently selected terms are <b>@terms</b>', array('@terms' => $current_terms)),
    );
    $form['new_terms'] = array(
      '#type' => 'select',
      '#title' => t('New Terms'),
      '#size' => 10,
      '#multiple' => TRUE,
      '#options' => $taxonomy,
    );
    $form['replace_condition'] = array(
      '#type' => 'checkbox',
      '#title' => t('Replace selected current terms with new terms'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Assign'),
    );
  }
  if (isset($form_state['values']['terms'])) {
    $node_list = taxonomy_manager_lite_reassign_terms_list_nodes(
      $form_state['values']['terms'],
      $form_state['values']['terms_condition']
    );
    $form['sample'] = array(
      '#type' => 'markup',
      '#markup' => $node_list,
      '#suffix' => '</div>',
    );
  }
  return $form;
}

/**
 * Ajax callback from the utility forms.
 */
function taxonomy_manager_lite_fill_vocabulary_terms($form, $form_state) {
  return $form;
}

/**
 * Validate the reassign terms form and also cause form to be rebuilt.
 */
function taxonomy_manager_lite_reassign_terms_preview($form, &$form_state) {
  $_SESSION['taxonomy_manager_lite_reassign_terms_filter']['terms'] = $form_state['values']['terms'];
  $_SESSION['taxonomy_manager_lite_reassign_terms_filter']['terms_condition'] = $form_state['values']['terms_condition'];
  if (!isset($form_state['values']['terms'])) {
    form_set_error('terms', t('Please select at least one term.'));
  }
  elseif (count($form_state['values']['terms']) > 3) {
    form_set_error('terms', t('You can select only upto 3 terms at a time.'));
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the reassign form.
 */
function taxonomy_manager_lite_reassign_terms_form_submit($form, &$form_state) {
  $_SESSION['taxonomy_manager_lite_reassign_terms_filter'] = array(
    'terms' => $form_state['values']['terms'],
    'terms_condition' => $form_state['values']['terms_condition'],
  );

  // Prepare and run the batch job.
  $batch = taxonomy_manager_lite_reassign_terms_prepare_job(
    $form_state['values']['terms'],
    $form_state['values']['terms_condition'],
    $form_state['values']['new_terms'],
    $form_state['values']['replace_condition']
  );
  batch_set($batch);
  $form_state['redirect'] = 'admin/utilities/taxonomy/assign-terms';
}

/**
 * Function that prepares the batch job.
 *
 * @params $terms, $show_any, $new_terms, $replace
 *
 * @return $batch
 */
function taxonomy_manager_lite_reassign_terms_prepare_job($terms, $show_any, $new_terms, $replace) {
  $query = taxonomy_manager_lite_reassign_terms_get_list_nodes_query($terms, $show_any);
  $count_query = "SELECT COUNT(*) AS total FROM (" . $query . ") AS q";
  $count = db_query($count_query)->fetchField();
  $operations = array();
  for ($i = 0; $i < $count; $i++) {
    $operations[] = taxonomy_manager_lite_reassign_terms_batch_run_op($terms, $show_any, $new_terms, $replace);
  }
  $batch = array(
    'title' => t('Reassigning Terms'),
    'operations' => $operations,
    'finished' => 'taxonomy_manager_lite_reassign_terms_batch_finished',
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Batch has encountered an error.'),
  );
  return $batch;
}

/**
 * Batch operation for reassigning terms.
 *
 * @params $terms, $show_any, $new_terms, $replace, $context
 */
function taxonomy_manager_lite_reassign_terms_batch_run_op($terms, $show_any, $new_terms, $replace, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox'] = array();
    $context['sandbox']['progress'] = 0;
    $query = taxonomy_manager_lite_reassign_terms_get_list_nodes_query($terms, $show_any);
    $count_query = "SELECT COUNT(*) AS total FROM (" . $query . ") AS q";
    $count = db_query($count_query)->fetchField();
    $context['sandbox']['current_row'] = 0;
    $context['sandbox']['max'] = $count;
    $context['sandbox']['query'] = $query;
  }
  // Retrieve the next set of rows.
  $result = db_query("SELECT nid
    FROM (" . $context['sandbox']['query'] . ") q
    WHERE nid > " . $context['sandbox']['current_row'] . " ORDER BY nid ASC");
  foreach ($result as $data) {
    $result_nid = $data->nid;
  }
  if ($result_nid) {
    // Assign terms for the current node.
    taxonomy_manager_lite_reassign_terms_in_node($result_nid, $terms, $new_terms, $replace);

    // Store some result for post-processing in the finished callback.
    $context['results'][] = 'Assigned terms for node ' . $result_nid;
    // Update our progress information.
    $context['sandbox']['progress']++;
    $context['sandbox']['current_row'] = (int) $result_nid;
    $context['message'] = 'Processed ' . $context['sandbox']['progress'] . ' node(s).';
  }
  else {
    throw new Exception(t('Error: Incorrect count in batch operation.'));
  }
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    $context['finished'] = 1;
  }
}

/**
 * Function to assign the new terms to a node.
 *
 * @param $nid
 *   Node id.
 * @param $old_terms
 *   Array of old terms.
 * @param $new_terms
 *   Array of new terms.
 * @param $replace
 *   Status that decides whether to replace the existing taxonomy term.
 */
function taxonomy_manager_lite_reassign_terms_in_node($nid, $old_terms, $new_terms, $replace) {
  $result = db_query("SELECT tn.tid FROM {node} n JOIN taxonomy_index tn ON n.nid = tn.nid AND n.nid = :nodeid", array(':nodeid' => $nid));
  $terms = array();
  foreach ($result as $data) {
    $terms[] = $data->tid;
  }
  foreach ($new_terms as $term) {
    if (!in_array($term, $terms)) {
      db_query("INSERT INTO {taxonomy_index} (nid, tid) VALUES (:nodeid, :termid)", array(':nodeid' => $nid, ':termid' => $term));
    }
  }
  if ($replace) {
    foreach ($old_terms as $term) {
      if (!in_array($term, $new_terms)) {
        db_query("DELETE FROM {taxonomy_index} WHERE nid = :nodeid AND tid = :termid", array(':nodeid' => $nid, ':termid' => $term));
      }
    }
  }
}

/**
 * Batch 'finished' callback.
 *
 * @params $success, $results, $operations
 */
function taxonomy_manager_lite_reassign_terms_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results), '1 node updated', '@count nodes updated');
  }
  else {

    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = 'An error occurred while processing ' . $error_operation[0] . ' with arguments :' . print_r($error_operation[0], TRUE);
  }
  drupal_set_message(check_plain($results));
  drupal_set_message(check_plain($message));
}

/**
 * List the nodes with the selected terms.
 *
 * @params $terms, $show_any
 *
 * @return $output
 *   String.
 */
function taxonomy_manager_lite_reassign_terms_list_nodes($terms, $show_any) {
  $sample_limit = 50;
  $output = '';
  $query = taxonomy_manager_lite_reassign_terms_get_list_nodes_query($terms, $show_any);
  $result = db_query($query);
  $count_query = "SELECT COUNT(*) AS total FROM (" . $query . ") AS q";
  $count = db_query($count_query)->fetchField();
  $rows = array();
  foreach ($result as $row) {
    $url_edit = l(t('Edit'), 'node/' . $row->nid);
    $url_view = l($row->title, 'node/' . $row->nid);
    $node_terms = taxonomy_manager_lite_reassign_terms_get_node_terms($row->nid);
    $rows[] = array(
      $row->nid,
      '<b>' . $url_view . '</b><br />' . $node_terms,
      $url_edit, date("m/d/Y H:i:s", $row->created),
    );
    if (count($rows) == $sample_limit) {
      break;
    }
  }
  if (!empty($row)) {
    $rows[] = array(
      array(
        'data' => t(
          'There are @count nodes with all the selected terms. Showing only @limit nodes',
          array('@count' => $count, '@limit' => $sample_limit)
        ),
        'colspan' => 4,
      ),
    );
  }
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no nodes with all the selected terms.'),
        'colspan' => 4,
      ),
    );
  }
  $header = array(
    'Nid',
    'Title',
    'Edit',
    'Created',
  );
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= '<br /><b>Total nodes:' . $count . '</b>';
  return $output;
}

/**
 * Get the list of terms in nodes.
 *
 * @params $terms, $show_any
 *
 * @return $query
 */
function taxonomy_manager_lite_reassign_terms_get_list_nodes_query($terms, $show_any) {
  if (!$show_any) {
    $i = 0;
    $join = '';
    foreach ($terms as $term) {
      $i++;
      if (is_numeric($term)) {
        $join .= '' . " JOIN taxonomy_index tn" . $i . "
            ON tn" . $i . ".nid = n.nid
            AND tn" . $i . ".tid = " . $term . " ";
      }
    }
  }
  else {
    $join = "JOIN taxonomy_index tn
      ON tn.nid = n.nid
      AND tn.tid IN ";
    $filter = '';
    foreach ($terms as $term) {
      if (is_numeric($term)) {
        if ($filter <> '') {
          $filter .= ',';
        }
        $filter .= $term;
      }
    }
    $join .= '(' . $filter . ')';
  }
  $query = '' . "SELECT DISTINCT n.nid, n.title, n.created
    FROM
      node n " . $join . "ORDER BY n.nid ASC ";
  return $query;
}

/**
 * Get terms assigned to a given node.
 *
 * @params $nid
 *   Node id.
 *
 * @return $output
 *   String.
 */
function taxonomy_manager_lite_reassign_terms_get_node_terms($nid) {
  $output = '';
  $query = '' . "SELECT t.name, t.tid
    FROM node n
    JOIN taxonomy_index tn
      ON n.nid = tn.nid
        AND n.nid = " . $nid . "
    LEFT JOIN taxonomy_term_data t
      ON tn.tid = t.tid " . "";
  $result = db_query($query);
  foreach ($result as $row) {
    if ($output <> '') {
      $output .= ', ';
    }
    $url_view = l($row->name, 'taxonomy/term/' . $row->tid);
    $url_edit = l(t('Edit'), 'taxonomy/term/' . $row->tid . '/edit');
    $output .= $url_view . '(' . $url_edit . ')';
  }
  return $output;
}
