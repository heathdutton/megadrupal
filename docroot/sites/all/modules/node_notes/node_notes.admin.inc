<?php
/**
 * @file
 * Provides delete confirmation screen and actions for notes.
 */

/**
 * Form builder; Builds the confirmation form for deleting a note.
 *
 * @ingroup forms
 * @see node_notes_confirm_delete_submit()
 */
function node_notes_confirm_delete($form, &$form_state, $note) {
  $form['#note'] = $note;
  $form['noteid'] = array('#type' => 'value', '#value' => $note->noteid);

  return confirm_form(
    $form,
    t('Are you sure you want to delete the note #@noteid?', array('@noteid' => $note->noteid)),
    'node/' . $note->nid . '/notes',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'),
    'node_notes_confirm_delete'
  );
}

/**
 * Process node_notes_confirm_delete form submissions.
 */
function node_notes_confirm_delete_submit($form, &$form_state) {
  $note = $form['#note'];
  // Delete the comment and its replies.
  node_notes_delete($note->noteid);
  drupal_set_message(t('Note #@noteid has been deleted.', array('@noteid' => $note->noteid)));
  watchdog('content', 'Deleted note @noteid.', array('@noteid' => $note->noteid));

  $form_state['redirect'] = "node/$note->nid/notes";
}
