<?php

/**
 * @file
 * Bulk deletion of taxonomy terms.
 */

/**
 * Form builder; Return form for deletion of terms.
 */
function entitybulkdelete_terms($form, &$form_state) {
  // Display page 2 if $form_state['page_num'] == 2
  if (!empty($form_state['page_num']) && $form_state['page_num'] == 2) {
    return entitybulkdelete_terms_confirm_form($form, $form_state);
  }
  $form['msg'] = array(
    '#type' => 'markup',
    '#markup' => '<strong>' . t('Warning: If parent terms selected, its children also get deleted.') . '</strong><br>',
  );
  $form_state['page_num'] = 1;
  // Deletes taxonomy-term by name.
  $form['del_terms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete taxonomy-terms, by name'),
    '#ajax' => array(
      'callback' => 'entitybulkdelete_term_callback',
      'wrapper' => 'del-terms',
    ),
  );
  $form['term_placeholder'] = array(
    '#type' => 'hidden',
    '#prefix' => '<div id="del-terms">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  if (!empty($form_state['values']['del_terms']) && $form_state['values']['del_terms']) {
    $form['term_placeholder']['#type'] = 'fieldset';
    $form['term_placeholder']['#title'] = 'Delete taxonomy-terms by name';
    $vocab_name = entitybulkdelete_vocab_name(FALSE);
    $vocab_name['_none'] = "Select vocabulary";
    // Get value from $form_state['values']
    $sel_vocabname = isset($form_state['values']['term_placeholder']['vocab_name']) ? $form_state['values']['term_placeholder']['vocab_name'] : '_none';
    $form['term_placeholder']['vocab_name'] = array(
      '#title' => 'Vocabulary name',
      '#type' => 'select',
      '#default_value' => '_none',
      '#options' => $vocab_name,
      '#ajax' => array(
        'callback' => 'entitybulkdelete_vocabname_callback',
        'wrapper' => 'terms-name',
      ),
    );
    if (!empty($sel_vocabname) && isset($sel_vocabname)) {
      if ($sel_vocabname != '_none') {
        $terms_list = entitybulkdelete_get_terms($sel_vocabname, FALSE);
        $form_state['terms'] = $terms_list;
      }
    }
    $terms_list['_none'] = "Select terms";
    $form['term_placeholder']['terms'] = array(
      '#title' => 'Taxonomy terms',
      '#type' => 'select',
      '#options' => $terms_list,
      '#multiple' => TRUE,
      '#prefix' => '<div id="terms-name">',
      '#suffix' => '</div>',
    );
  }
  // Deletes taxonomy-terms by term's tid.
  $form['del_tid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete terms, by tid'),
    '#ajax' => array(
      'callback' => 'entitybulkdelete_tid_callback',
      'wrapper' => 'del-tid',
    ),
  );
  $form['tid_placeholder'] = array(
    '#type' => 'hidden',
    '#prefix' => '<div id="del-tid">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  if (!empty($form_state['values']['del_tid']) && $form_state['values']['del_tid']) {
    $form['tid_placeholder']['#type'] = 'fieldset';
    $form['tid_placeholder']['#title'] = 'Delete terms by tid';
    $form['tid_placeholder']['tid'] = array(
      '#title' => 'Terms id',
      '#type' => 'textarea',
      '#rows' => 6,
      '#cols' => 40,
      '#description' => t("Enter term's tid separate by ','."),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Delete',
    '#submit' => array('entitybulkdelete_terms_submit'),
  );
  return $form;
}

/**
 * Submit handler for the taxonomy-terms deletion form.
 */
function entitybulkdelete_terms_submit($form, &$form_state) {
  $valid_rows = array();
  $del_by_terms = $form_state['values']['del_terms'];
  if ($del_by_terms) {
    $sel_terms = $form_state['values']['term_placeholder']['terms'];
    if (isset($form_state['terms']) && !empty($form_state['terms'])) {
      $terms = $form_state['terms'];
      foreach ($sel_terms as $key => $value) {
        if (array_key_exists($key, $terms)) {
          $valid_rows[$key] = array('tid' => $key, 'terms' => check_plain($terms[$key]));
        }
      }
    }
  }
  $del_by_tid = $form_state['values']['del_tid'];
  if ($del_by_tid) {
    $find = array(' ', '-', '+');
    $temp_terms = check_plain($form_state['values']['tid_placeholder']['tid']);
    if (!empty($temp_terms)) {
      $terms_tid = str_replace($find, '', $temp_terms);
      $terms_tid = explode(',', $terms_tid);
      if (!empty($terms_tid)) {
        foreach ($terms_tid as $key => $value) {
          if (!is_numeric($value)) {
            unset($terms_tid[$key]);
          }
          else {
            $temp_key = (int) $value;
            $termstid[$temp_key] = $temp_key;
          }
        }
        // Get list of term's tid and terms-name.
        $terms = taxonomy_term_load_multiple($termstid);
        foreach ($terms as $term) {
          $valid_rows[$term->tid] = array('tid' => $term->tid, 'terms' => check_plain($term->name));
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
 * Form builder; Return confirmation form for taxonomy-terms deletion.
 */
function entitybulkdelete_terms_confirm_form($form, &$form_state) {
  $header['tid'] = 'Tid';
  $header['terms'] = 'Terms';
  $valid_row = $form_state['valid_rows'];
  $form['msg'] = array(
    '#type' => 'markup',
    '#markup' => t('Are you sure you want to deletes these terms ?') .
    '<br><strong>' . t('Warning: If parent terms marked, its children also get deleted.') . '</strong><br>' . t('This action cannot be undone.') . '<br>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Confirm',
    '#submit' => array('entitybulkdelete_terms_confirm_form_submit'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => 'Cancel',
    '#submit' => array('entitybulkdelete_terms_cancel_form_submit'),
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
 * Submit handler(Cancel) of confirmation form for taxonomy-term deletion.
 */
function entitybulkdelete_terms_cancel_form_submit($form, &$form_state) {
  $form_state['page_num'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler(Confirm) of confirmation form for taxonomy-term deletion.
 */
function entitybulkdelete_terms_confirm_form_submit($form, &$form_state) {
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
      // entitybulkdelete_terms_delete_batch.
      $batch_tbl = array_slice($table, $i, 5);
      if (!empty($batch_tbl)) {
        $operations[] = array('entitybulkdelete_terms_delete_batch',
          array($batch_tbl, &$form_state));
      }
    }
    // Execute the batch.
    $batch = array(
      'operations' => $operations,
      'title' => t('Terms Delete Batch'),
      'init_message' => t('Term(s) deleting is in process...'),
      'progress_message' => t('Processed @current*5 out of @total*5.'),
      'finished' => 'entitybulkdelete_terms_delete_batch_finished',
      'file' => drupal_get_path('module', 'entitybulkdelete') . '/entitybulkdelete.terms.inc',
    );
    batch_set($batch);
  }
  $form_state['page_num'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 * Batch operation entitybulkdelete_terms_delete_batch.
 *
 * Delete terms, 5 by five after each group of 5 control
 * is returned to the batch API for later continuation.
 */
function entitybulkdelete_terms_delete_batch($table, $form_state, &$context) {
  if (!empty($table)) {
    foreach ($table as $value) {
      // Delete taxonomy-term.
      taxonomy_term_delete($value);
      watchdog('entitybulkdelete', 'Term %tid marked as deleted.', array('%tid' => $value), WATCHDOG_NOTICE);
    }
  }
}

/**
 * Batch 'finished' callback used by entitybulkdelete_terms_delete_batch.
 */
function entitybulkdelete_terms_delete_batch_finished($success, $results, $operations) {
  if ($success) {
    $msg = t('Term(s) %success has been deleted.');
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
 * Fetch list of vocabularies.
 *
 * @param bool $reset
 *   Contain boolean value.
 *
 * @return array $content
 *   List of content type.
 */
function entitybulkdelete_vocab_name($reset = FALSE) {
  $vocab = &drupal_static(__FUNCTION__, NULL, $reset);
  if (!isset($vocab)) {
    if (!$reset && ($cache = cache_get("entitybulkdelete:vocabularies")) && !empty($cache->vocab)) {
      $vocab = $cache->vocab;
    }
    else {
      // Get list of vocabularies.
      $vocab_array = taxonomy_get_vocabularies();
      if (!empty($vocab_array)) {
        foreach ($vocab_array as $obj) {
          $vocab[$obj->vid] = check_plain($obj->name);
        }
      }
      cache_set("entitybulkdelete:vocabularies", $vocab);
    }
  }
  return $vocab;
}

/**
 * Fetch list of terms.
 *
 * @param int $vid
 *   Contain vocabularies vid.
 * @param bool $reset
 *   Content boolean value.
 *
 * @return array $terms_list
 *   List of terms.
 */
function entitybulkdelete_get_terms($vid, $reset = FALSE) {
  $terms_list = &drupal_static(__FUNCTION__, NULL, $reset);
  if (!isset($terms_list)) {
    if (!$reset && ($cache = cache_get("entitybulkdelete:terms list")) && !empty($cache->terms_list)) {
      $terms_list = $cache->terms_list;
    }
    else {
      // Get list of all terms from vid.
      $terms_list = array();
      $vocab_array = taxonomy_get_tree($vid);
      if (!empty($vocab_array)) {
        foreach ($vocab_array as $obj) {
          $terms_list[$obj->tid] = check_plain($obj->name);
        }
      }
      cache_set("entitybulkdelete:terms list", $terms_list);
    }
  }
  return $terms_list;
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The term_placeholder element.
 */
function entitybulkdelete_term_callback($form, $form_state) {
  return $form['term_placeholder'];
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The terms element.
 */
function entitybulkdelete_vocabname_callback($form, $form_state) {
  return $form['term_placeholder']['terms'];
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The tid_placeholder element.
 */
function entitybulkdelete_tid_callback($form, $form_state) {
  return $form['tid_placeholder'];
}
