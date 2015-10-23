<?php
/**
 * @file
 * Administrative interface for the taxonomy term status module.
 */

/**
 * Term status settings form.
 */
function termstatus_settings($form, &$form_state) {
  $count = _termstatus_count_nullstatus();
  $enabled = variable_get('termstatus_enable', FALSE);

  if ($count) {
    $form['message'] = array(
      '#markup' => t('There are %count taxonomy terms which do not have any status setting. You need to rebuild the taxonomy term status records.', array('%count' => $count)),
    );
  }

  $form['termstatus_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable term status'),
    '#description' => t('If enabled, unpublished taxonomy terms are hidden'),
    '#default_value' => $enabled,
    '#disabled' => (!$enabled && $count > 0),
  );

  $form['actions']['rebuild'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild missing status records'),
    '#disabled' => $count == 0,
    '#submit' => array('termstatus_rebuild_submit'),
  );

  return system_settings_form($form);
}

/**
 * Submit callback.
 */
function termstatus_rebuild_submit($form, &$form_state) {
  _termstatus_rebuild();
  drupal_set_message(t('Term status rebuild complete.'));
}

/**
 * Return the number of taxonomy terms which do not have any status record.
 */
function _termstatus_count_nullstatus() {
  $count = db_query('SELECT COUNT(*) FROM {taxonomy_term_data} AS td LEFT JOIN {termstatus} AS ts ON td.tid = ts.tid WHERE ts.status IS NULL')->fetchField();
  return $count;
}

/**
 * Create status records for every taxonomy term which does not have one yet.
 *
 * FIXME: Do we need to batch here?
 */
function _termstatus_rebuild() {
  $query = db_select('taxonomy_term_data', 'td');
  $query->leftjoin('termstatus', 'ts', 'td.tid = ts.tid');
  $query->fields('td', array('tid'));
  $query->addExpression('1', 'status');
  $query->condition('ts.status', NULL, 'IS NULL');

  db_insert('termstatus')
    ->from($query)
    ->execute();
}
