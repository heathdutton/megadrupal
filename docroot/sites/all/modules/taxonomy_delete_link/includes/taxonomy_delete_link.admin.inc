<?php
/**
 * @file
 * Provide the term delete form.
 */

/**
 * Form builder for the term delete form.
 */
function taxonomy_delete_link_confirm_delete($form, &$form_state, $term) {
  // Always provide entity id in the same form key as in the entity edit form.
  $form['tid'] = array('#type' => 'value', '#value' => $term->tid);

  $vocabulary = NULL;
  if (isset($term->vid)) {
    $vocabulary = taxonomy_vocabulary_load($term->vid);
  }

  $form['#vocabulary'] = $vocabulary;
  $form['#term'] = $term;
  $form['type'] = array('#type' => 'value', '#value' => 'term');
  $form['name'] = array('#type' => 'value', '#value' => $term->name);
  $form['vocabulary_machine_name'] = array('#type' => 'value', '#value' => $term->vocabulary_machine_name);
  $form['delete'] = array('#type' => 'value', '#value' => TRUE);
  return confirm_form($form,
    t('Are you sure you want to delete the term %title?',
    array('%title' => $term->name)),
    'admin/structure/taxonomy',
    t('Deleting a term will delete all its children if there are any. This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));
}

/**
 * Submit handler to delete a term after confirmation.
 */
function taxonomy_delete_link_confirm_delete_submit($form, &$form_state) {
  taxonomy_term_delete($form_state['values']['tid']);
  taxonomy_check_vocabulary_hierarchy($form['#vocabulary'], $form_state['values']);
  drupal_set_message(t('Deleted term %name.', array('%name' => $form_state['values']['name'])));
  watchdog('taxonomy', 'Deleted term %name.', array('%name' => $form_state['values']['name']), WATCHDOG_NOTICE);
  $form_state['redirect'] = 'admin/structure/taxonomy';
  cache_clear_all();
  return TRUE;
}
