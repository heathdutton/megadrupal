<?php
/**
 * @file
 * Admin scoring form.
 */

/**
 * Form constructor for managing and viewing scoring entries.
 *
 * @see pardot_admin_scoring_validate()
 * @see pardot_admin_scoring_submit()
 *
 * @ingroup forms
 */
function pardot_admin_scoring($form, &$form_state) {
  $form = array();

  $scores = db_query('SELECT * FROM {pardot_scoring}');

  foreach ($scores as $score) {
    //dsm($score);
    $form['scores'][$score->scoring_id] = array(
      '#score' => $score,
      'path' => array(
        '#value' => $score->path,
        '#markup' => $score->path,
      ),
      'score' => array(
        '#value' => $score->score,
        '#markup' => $score->score,
      ),
    );
  }

  $form['new'] = array(
    'path' => array(
      '#prefix' => '<div class="add-new-placeholder">' . t('Add new path') . '</div>',
      '#type' => 'textfield',
      '#size' => 30,
    ),
    'score' => array(
      '#prefix' => '<div class="add-new-placeholder">' . t('Score') . '</div>',
      '#type' => 'textfield',
      '#size' => 30,
    ),
  );

  $form['add'] = array(
    '#prefix' => '<div class="add-new-placeholder"></div>',
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Form validation handler for pardot_admin_scoring().
 *
 * @see pardot_admin_scoring_submit()
 */
function pardot_admin_scoring_validate($form, $form_state) {
  if (!is_numeric($form_state['values']['score'])) {
    form_set_error('score', t('Your score must be a number'));
  }
  if (db_query("SELECT path FROM {pardot_scoring} WHERE path = :path", array(':path' => $form_state['values']['path']))->fetchField()) {
    form_set_error('path', t('Your path much be unique'));
  }
}

/**
 * Form submission handler for pardot_admin_scoring().
 *
 * @see pardot_admin_scoring_validate()
 */
function pardot_admin_scoring_submit($form, $form_state) {
  $score = (object) $form_state['values'];
  drupal_write_record('pardot_scoring', $score);
}

/**
 * Theme callback for pardot_admin_scoring.
 *
 * @see pardot_admin_scoring()
 */
function theme_pardot_admin_scoring($variables) {
  $form = $variables['form'];
  if (isset($form['scores'])) {
    foreach (element_children($form['scores']) as $id) {
      $score = $form['scores'][$id]['#score'];

      $row = array();
      $row[] = drupal_render($form['scores'][$id]['path']);
      $row[] = drupal_render($form['scores'][$id]['score']);

      $ops = array();
      $ops[] = l(t('Edit'), 'admin/config/services/pardot/scoring/' . $score->scoring_id . '/edit');
      $ops[] = l(t('Delete'), 'admin/config/services/pardot/scoring/' . $score->scoring_id . '/delete');
      $row[] = implode(' | ', $ops);
      $rows[] = $row;
    }
  }
  $rows[] = array(
    drupal_render($form['new']['path']),
    drupal_render($form['new']['score']),
    drupal_render($form['add']),
  );

  $headers = array(t('Path'), t('Score'), t('Operations'));
  $output = theme('table', array('header' => $headers, 'rows' => $rows));

  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Form constructor for editing scoring entries.
 *
 * @param object $score
 *   Scoring database object.
 *
 * @see pardot_admin_scoring_edit_validate()
 * @see pardot_admin_scoring_edit_submit()
 *
 * @ingroup forms
 */
function pardot_admin_scoring_edit($form, $form_state, $score) {

  $form = array();

  $form['scoring_id'] = array(
    '#type' => 'value',
    '#value' => $score->scoring_id,
  );
  $form['path'] = array(
    '#title' => t('Path'),
    '#type' => 'textfield',
    '#size' => 30,
    '#default_value' => $score->path,
  );
  $form['score'] = array(
    '#title' => t('Score'),
    '#type' => 'textfield',
    '#size' => 30,
    '#default_value' => $score->score,
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validation handler for pardot_admin_scoring_edit().
 *
 * @see pardot_admin_scoring_edit_submit()
 */
function pardot_admin_scoring_edit_validate($form, $form_state) {
  if (!is_numeric($form_state['values']['score'])) {
    form_set_error('score', t('Your score must be a number'));
  }
  if (db_query("SELECT path FROM {pardot_scoring} WHERE path = :path AND scoring_id <> :scoring_id", array(':path' => $form_state['values']['path'], ':scoring_id' => $form_state['values']['scoring_id']))->fetchField()) {
    form_set_error('path', t('Your path much be unique'));
  }
}

/**
 * Form submission handler for pardot_admin_scoring_edit().
 *
 * @see pardot_admin_scoring_edit_validate()
 */
function pardot_admin_scoring_edit_submit($form, &$form_state) {
  $score = (object) $form_state['values'];
  drupal_write_record('pardot_scoring', $score, array('scoring_id'));
  $form_state['redirect'] = 'admin/config/services/pardot/scoring';
}

/**
 * Form constructor for deleting scoring entries.
 *
 * @param object $score
 *   Scoring database object.
 *
 * @see pardot_admin_scoring_delete_submit()
 *
 * @ingroup forms
 */
function pardot_admin_scoring_delete($form, &$form_state, $score) {
  $form = array();
  //$form['#scoring_id'] = $score->scoring_id;

  $description = t('Are you sure you want to delete this scoring (%score)? This path will no logger trigger additional scoring in Pardot.', array('%score' => $score->scoring_id));

  return confirm_form($form, t('Are you sure you want to delete this score (%score)?', array('%score' => $score->scoring_id)), 'admin/config/services/pardot/scoring', $description, 'Delete');
}

/**
 * Form submission handler for pardot_admin_scoring_delete().
 */
function pardot_admin_scoring_delete_submit($form, &$form_state) {
  $score = $form_state['build_info']['args'][0];
  db_delete('pardot_scoring')
    ->condition('scoring_id', $score->scoring_id)
    ->execute();
  $form_state['redirect'] = 'admin/config/services/pardot/scoring';
}
