<?php

/**
 * @file
 * Admin page callback file for the cmood module.
 */

/**
 * Page callback: Generates the appropriate mood administration form.
 *
 * @return string
 *   A renderable form array for the respective request.
 */
function cmood_admin() {
  $header = array(
    array('data' => t('WID'), 'field' => 'wid', 'sort' => 'asc'),
    array('data' => t('Word'), 'field' => 'name'),
    array('data' => t('Weight'), 'field' => 'weight'),
    array('data' => t('UID'), 'field' => 'uid'),
    array('data' => t('Edit'), 'field' => 'Edit'),
    array('data' => t('Delete'), 'field' => 'Delete'),
  );
  $rows = array();
  $query = db_select('word_with_weight', 'www')->fields('www');
  $table_sort = $query->extend('TableSort')
    ->orderByHeader($header);
  $pager = $table_sort->extend('PagerDefault')
    ->limit(10);
  $results = $pager->execute();
  foreach ($results as $row) {
    $rows[] = array($row->wid,
      check_plain($row->name),
      $row->weight,
      $row->uid,
      l(t('edit'), 'admin/cmood/mood_word/' . $row->wid . '/edit'),
      l(t('delete'), 'admin/cmood/mood_word/' . $row->wid . '/delete',
        array('query' => array('destination' => 'admin/cmood/mood_word'))),
    );
  }
  return theme('table', array(
    'header' => $header,
    'rows' => $rows)) . theme('pager');
}

/**
 * Page callback: Generates the appropriate rank word administration form.
 *
 * @return string
 *   A renderable form array for the respective request.
 */
function cmood_rank_words() {
  $header = array(
    array('data' => t('RWID'), 'field' => 'rwid', 'sort' => 'asc'),
    array('data' => t('Word'), 'field' => 'name'),
    array('data' => t('Weight'), 'field' => 'weight'),
    array('data' => t('UID'), 'field' => 'uid'),
    array('data' => t('Edit'), 'field' => 'edit'),
    array('data' => t('Delete'), 'field' => 'delete'),
  );
  $rows = array();
  $query = db_select('rank_word_with_weight', 'rwww')->fields('rwww');
  $table_sort = $query->extend('TableSort')
    ->orderByHeader($header);
  $pager = $table_sort->extend('PagerDefault')
    ->limit(10);
  $results = $pager->execute();
  foreach ($results as $row) {
    $rows[] = array($row->rwid,
      check_plain($row->name),
      $row->weight,
      $row->uid,
      l(t('edit'), 'admin/cmood/rank_word/' . $row->rwid . '/edit'),
      l(t('delete'), 'admin/cmood/rank_word/' . $row->rwid . '/delete',
        array('query' => array('destination' => 'admin/cmood/rank_word'))),
    );
  }
  return theme('table', array(
    'header' => $header,
    'rows' => $rows)) . theme('pager');
}

/**
 * Page callback: Generates the appropriate node with mood listing.
 *
 * @return string
 *   A renderable form array for the respective request.
 */
function cmood_node_mood_listing() {
  $header = array(
    array('data' => t('ID'), 'field' => 'id', 'sort' => 'asc'),
    array('data' => t('Node Title'), 'field' => 'title'),
    array('data' => t('Node Mood'), 'field' => 'mood'),
  );
  $rows = array();
  $query = db_select('node', 'n')
    ->addTag('node_access')
    ->fields('nm', array('id', 'nid', 'mood'))
    ->fields('n', array('title'));
  $query->join('node_mood', 'nm', 'nm.nid = n.nid');
  $table_sort = $query->extend('TableSort')
    ->orderByHeader($header);
  $pager = $table_sort->extend('PagerDefault')
    ->limit(10);
  $results = $pager->execute();
  foreach ($results as $row) {
    $rows[] = array($row->id,
      truncate_utf8(check_plain($row->title), 30),
      $row->mood,
    );
  }
  return theme('table', array(
    'header' => $header,
    'rows' => $rows)) . theme('pager');
}

/**
 * Form constructor for the content mood word add form.
 *
 * @see cmood_form_validate()
 * @see cmood_form_submit()
 *
 * @ingroup forms
 */
function cmood_form($form, &$form_state, $wid = NULL, $operation = NULL) {
  if (is_numeric($wid) && $operation == 'edit') {
    $w_edit = db_select('word_with_weight', 'www')
      ->fields('www')
      ->condition('wid', $wid)
      ->execute()->fetchAssoc();
  }
  $form['word'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter your word:'),
    '#size' => 10,
    '#maxlength' => 255,
    '#default_value' => isset($w_edit['name']) ? $w_edit['name'] : '',
    '#required' => TRUE,
    '#description' => '<p>' . t('Enter a single word no spaces allowed.')
    . '</p>',
  );
  $weight = cmood_get_weights();
  $form['weight'] = array(
    '#type' => 'select',
    '#title' => t('Select weight of word:'),
    '#options' => $weight,
    '#default_value' => isset($w_edit['weight']) ? $w_edit['weight'] : '0',
    '#description' => '<p>' . t('Select weight of the word e.g. good = 2, better = 3, bad = -2, worse = -3.') . '</p>',
  );
  $form['wid'] = array(
    '#type' => 'hidden',
    '#title' => t('Word Id:'),
    '#value' => isset($w_edit['wid']) ? $w_edit['wid'] : '',
  );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Form validation handler for cmood_form().
 *
 * @see cmood_form_submit()
 */
function cmood_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == 'Save') {
    $word = $form_state['values']['word'];
    if (strpos(trim($word), ' ')) {
      // Word can't contain spaces.
      form_set_error('word', t('Word must not contain spaces.'));
    }
    $weight = $form_state['values']['weight'];
    if (!($weight >= -20 && $weight <= 20)) {
      // Improper weight selected.
      form_set_error('weight', t('Weight must greater than -20 and')
        . t('less than 20.'));
    }
  }
}

/**
 * Form submission handler for cmood_form().
 *
 * @see cmood_form_validate()
 */
function cmood_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $owww = new stdclass();
  $owww->name = $values['word'];
  $owww->weight = $values['weight'];
  $owww->uid = $user->uid;
  $owww->updated = REQUEST_TIME;
  if ($values['wid']) {
    $owww->wid = $values['wid'];
    drupal_write_record('word_with_weight', $owww, 'wid');
    drupal_set_message(t('Your mood word with weight has been saved successfully!'));
    $form_state['redirect'] = 'admin/cmood/mood_word';
  }
  else {
    $owww->created = REQUEST_TIME;
    drupal_write_record('word_with_weight', $owww);
    drupal_set_message(t('Your mood word with weight has been saved successfully!'));
  }
}

/**
 * Form constructor for the content rank word add form.
 *
 * @see rank_word_form_validate()
 * @see rank_word_form_submit()
 *
 * @ingroup forms
 */
function cmood_rank_word_form($form, &$form_state, $rwid = NULL, $operation = NULL) {
  if (is_numeric($rwid) && $operation == 'edit') {
    $rw_edit = db_select('rank_word_with_weight', 'rwww')
      ->fields('rwww')
      ->condition('rwid', $rwid)
      ->execute()->fetchAssoc();
  }
  $form['word'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter your rank word:'),
    '#size' => 10,
    '#maxlength' => 255,
    '#default_value' => isset($rw_edit['name']) ? $rw_edit['name'] : '',
    '#required' => TRUE,
    '#description' => '<p>' . t('Enter a single word no spaces allowed. You can enter words like very, more, loud etc. Whose weight will be multiplied with mood words. For example "good" word has weight of 2 and very has weight of 3 then a node containing "good" word will have a mood of 2 and node containing "very good" will have a mood of 6.') . '</p>',
  );
  $weight = cmood_get_weights();
  unset($weight[0]);
  unset($weight[1]);
  $form['weight'] = array(
    '#type' => 'select',
    '#title' => t('Select weight of rank word:'),
    '#options' => $weight,
    '#default_value' => isset($rw_edit['weight']) ? $rw_edit['weight'] : '2',
    '#description' => '<p>' . t('Select weight of the word e.g. good = 2, better = 3, bad = -2, worse = -3.') . '</p>',
  );
  $form['rwid'] = array(
    '#type' => 'hidden',
    '#title' => t('Rank Word Id:'),
    '#value' => isset($rw_edit['rwid']) ? $rw_edit['rwid'] : '',
  );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Form validation handler for rank_word_form().
 *
 * @see cmood_rank_word_form_submit()
 */
function cmood_rank_word_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == 'Save') {
    $word = $form_state['values']['word'];
    if (strpos(trim($word), ' ')) {
      // Word can't contain spaces.
      form_set_error('word', t('Rank word must not contain spaces.'));
    }
    $weight = $form_state['values']['weight'];
    if (!($weight >= -20 && $weight <= 20)) {
      // Improper weight selected.
      form_set_error('weight', t('Weight must greater than -20
       and less than 20.'));
    }
  }
}

/**
 * Form submission handler for rank_word_form().
 *
 * @see cmood_rank_word_form_validate()
 */
function cmood_rank_word_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $orwww = new stdclass();
  $orwww->name = $values['word'];
  $orwww->weight = $values['weight'];
  $orwww->uid = $user->uid;
  $orwww->updated = REQUEST_TIME;
  if ($values['rwid']) {
    $orwww->rwid = $values['rwid'];
    drupal_write_record('rank_word_with_weight', $orwww, 'rwid');
    drupal_set_message(t('Your rank word with weight has been saved successfully!'));
    $form_state['redirect'] = 'admin/cmood/rank_word';
  }
  else {
    $orwww->created = REQUEST_TIME;
    drupal_write_record('rank_word_with_weight', $orwww);
    drupal_set_message(t('Your rank word with weight has been saved successfully!'));
  }
}

/**
 * Function to confirm delete action on mood word.
 *
 * @see cmood_form_submit()
 */
function cmood_word_delete_confirm($form, &$form_state, $wid) {
  $form['wid'] = array(
    '#type' => 'value',
    '#value' => $wid,
  );
  return confirm_form($form,
    t('Are you sure you want to delete this mood word?'), '',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));
}

/**
 * Form submission handler for cmood_word_delete_confirm().
 *
 * @see cmood_word_delete_confirm()
 */
function cmood_word_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    cmood_word_delete($form_state['values']['wid']);
    drupal_set_message(t('Mood word has been deleted successfully.'));
  }
}

/**
 * Function to confirm delete action on rank word.
 *
 * @param int $rwid
 *   Rank word id to delete.
 *
 * @see cmood_rank_word_delete_confirm_submit()
 */
function cmood_rank_word_delete_confirm($form, &$form_state, $rwid) {
  $form['rwid'] = array(
    '#type' => 'value',
    '#value' => $rwid,
  );
  return confirm_form($form,
    t('Are you sure you want to delete this rank word?'), '',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));
}

/**
 * Form submission handler for cmood_rank_word_delete_confirm().
 *
 * @see cmood_rank_word_delete_confirm()
 */
function cmood_rank_word_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    cmood_rank_word_delete($form_state['values']['rwid']);
    drupal_set_message(t('Rank word has been deleted successfully.'));
  }
}

/**
 * Form constructor for the cmood settings form.
 *
 * @see cmood_settings_form_validate()
 * @see cmood_settings_form_submit()
 *
 * @ingroup forms
 */
function cmood_settings_form($form, &$form_state) {
  $sdefault_type_options = variable_get('cmood_settings', '');
  if ($sdefault_type_options == '') {
    $default_type_options = array('all');
  }
  else {
    $default_type_options = unserialize($sdefault_type_options);
  }
  $all_types = array('all' => 'All Content types');
  $types = array_merge($all_types, node_type_get_names());
  $form['visibility']['node_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#group' => 'visibility',
  );
  $form['visibility']['node_type']['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Calculate mood for content types:'),
    '#default_value' => $default_type_options,
    '#options' => $types,
    '#description' => t('Calculate mood for given content type(s). If you select no types, there will be no type-specific limitation.'),
  );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Form validation handler for cmood_settings_form().
 *
 * @see cmood_settings_form_submit()
 */
function cmood_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == 'Save') {
    $values = $form_state['values'];
    if ($values['types']['all'] === 0) {
      // Check if any content type is chosen.
      unset($values['types']['all']);
      foreach ($values['types'] as $key => $value) {
        if ($value === 0) {
          unset($values['types'][$key]);
        }
      }
      if (count($values['types']) == 0) {
        form_set_error('types', t('No types chosen.'));
      }
    }
  }
}

/**
 * Form submission handler for cmood_settings_form().
 *
 * @see cmood_settings_form_validate()
 */
function cmood_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  variable_set('cmood_settings', serialize($values['types']));
  drupal_set_message(t('Your settings have been saved.'));
}
