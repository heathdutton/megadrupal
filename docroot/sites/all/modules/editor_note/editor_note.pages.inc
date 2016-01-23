<?php

/**
 * @file
 * Page callbacks defined in hook_menu for the Editor Notes module.
 */

/**
 * Builds confirmation page for updating selected note.
 */
function editor_note_confirm_edit_page($ajax = NULL, $note_id = NULL) {
  if ($note_id == NULL) {
    return t('Error: no note id was sent.');
  }

  if ($ajax) {
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'title' => t('Update selected item'),
    );

    $output = ctools_modal_form_wrapper('editor_note_confirm_edit_form', $form_state);
    // If the form has been submitted, there may be additional instructions
    // such as dismissing the modal popup.
    if (!empty($form_state['ajax_commands'])) {
      $output = $form_state['ajax_commands'];
    }

    // Returns the ajax instructions to the browser via ajax_render().
    print ajax_render($output);
    drupal_exit();
  }
  else {
    return drupal_get_form('editor_note_confirm_edit_form', $note_id);
  }
}

/**
 * Builds confirmation form for editing the note which displays in popup.
 */
function editor_note_confirm_edit_form($form, $form_state, $note_id = NULL) {
  $form = array();
  $note_id = isset($note_id) ? $note_id : (int) arg(4);
  $note = editor_note_load($note_id);

  if ($note) {
    $field = field_info_field($note->field_name);

    // Defines whether form has been opened via ajax or via regular page load.
    // Accepts either 'ajax' or 'nojs'.
    $form['method'] = array(
      '#type' => 'value',
      '#value' => arg(2) === 'ajax' ? 'ajax' : 'nojs',
    );

    $form['note'] = array(
      '#type' => 'textarea',
      '#title' => t('Update note'),
      '#default_value' => $note->note,
      '#required' => TRUE,
      '#rows' => $field['settings']['notes_size'],
      '#resizable' => FALSE,
      '#attributes' => array(
        'maxlength' => $field['settings']['notes_maxlength'] > 0 ? $field['settings']['notes_maxlength'] : '',
        'placeholder' => $field['settings']['notes_placeholder'],
      ),
    );

    $form['note_id'] = array(
      '#type' => 'value',
      '#value' => $note->id,
    );

    $form['note_entity_type'] = array(
      '#type' => 'value',
      '#value' => $note->entity_type,
    );

    $form['note_bundle'] = array(
      '#type' => 'value',
      '#value' => $note->bundle,
    );

    $form['note_entity_id'] = array(
      '#type' => 'value',
      '#value' => $note->entity_id,
    );

    $form['note_field_name'] = array(
      '#type' => 'value',
      '#value' => $note->field_name,
    );

    $form['actions'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('element-inline', 'editor-note-actions'),
      ),
    );

    $form['actions']['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#submit' => array('editor_note_confirm_form_cancel'),
    );

    $form['actions']['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#validate' => array('editor_note_confirm_edit_form_validate_update'),
      '#submit' => array('editor_note_confirm_edit_form_update'),
    );
  }
  else {
    $form['note'] = array(
      '#markup' => '<p>' . t('Selected note cannot be loaded. Cancel action and reload the page to refresh notes widget.') . '</p>',
    );

    $form['actions']['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#submit' => array('editor_note_confirm_form_cancel'),
    );
  }

  return $form;
}

/**
 * Validates value of 'Create a note' textarea during updating a note via ajax.
 */
function editor_note_confirm_edit_form_validate_update(&$form, &$form_state) {
  $note = $form_state['values']['note'];
  $field_name = $form_state['values']['note_field_name'];
  $field = field_info_field($field_name);
  $field_info = field_info_instance($form_state['values']['note_entity_type'], $field_name, $form_state['values']['note_bundle']);
  if (!empty($note) && $field['settings']['notes_maxlength'] > 0) {
    if (drupal_strlen($note) > $field['settings']['notes_maxlength']) {
      form_set_error('note', t('%name: the value cannot not be longer than %max characters.',
        array('%name' => $field_info['label'], '%max' => $field['settings']['notes_maxlength'])));
    }
  }
}

/**
 * Submit callback for 'Update' button in 'editor_note_confirm_edit_form'.
 */
function editor_note_confirm_edit_form_update(&$form, &$form_state) {
  global $user;
  $message_text = t('Note has been updated and saved.');
  $field_name = $form_state['values']['note_field_name'];
  $entity_type = $form_state['values']['note_entity_type'];
  $entity_id = $form_state['values']['note_entity_id'];
  $note_id = $form_state['values']['note_id'];
  $field = field_info_field($field_name);
  $edit_path = implode('/', array($entity_type, $entity_id, 'edit'));

  // Updates note entity in the database.
  $note = editor_note_load($note_id);
  $note->note = $form_state['values']['note'];
  $note->changed = REQUEST_TIME;
  $note->uid = $user->uid;
  $note->save();

  if ($form_state['values']['method'] == 'ajax') {
    // Closes the modal.
    $form_state['ajax_commands'][] = ctools_modal_command_dismiss();

    // Displays remove message.
    $message = theme(
      'editor_note_message',
      array(
        'field_name' => $field_name,
        'message_text' => $message_text,
        'message_type' => 'status',
      )
    );

    $form_state['ajax_commands'][] = ajax_command_replace('#status_message_' . $field_name, $message);

    // Updates notes table and pager.
    $entity_array = entity_load($entity_type, array($entity_id));
    $entity = $entity_array[$entity_id];
    $notes = editor_note_get_notes($entity_type, $entity, $field);
    $formatted_notes = editor_note_get_formatted_notes($field, $notes, TRUE, $edit_path);
    $form_state['ajax_commands'][] = ajax_command_replace('#formatted_notes_' . $field_name, drupal_render($formatted_notes));
  }
  else {
    drupal_set_message($message_text);
  }
}

/**
 * Builds confirmation page for removing selected note.
 */
function editor_note_confirm_remove_page($ajax = NULL, $note_id = NULL) {
  if ($note_id == NULL) {
    return t('Error: no note id was sent.');
  }

  if ($ajax) {
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'title' => t('Are you sure you want to delete the note?'),
    );

    $output = ctools_modal_form_wrapper('editor_note_confirm_remove_form', $form_state);
    // If the form has been submitted, there may be additional instructions
    // such as dismissing the modal popup.
    if (!empty($form_state['ajax_commands'])) {
      $output = $form_state['ajax_commands'];
    }

    // Returns the ajax instructions to the browser via ajax_render().
    print ajax_render($output);
    drupal_exit();
  }
  else {
    return drupal_get_form('editor_note_confirm_remove_form', $note_id);
  }
}

/**
 * Builds confirmation form for removing the note which displays in popup.
 */
function editor_note_confirm_remove_form($form, $form_state, $note_id = NULL) {
  $form = array();
  $note_id = isset($note_id) ? $note_id : (int) arg(4);
  $note = editor_note_load($note_id);

  if ($note) {
    // Defines whether form has been opened via ajax or via regular page load.
    // Accepts either 'ajax' or 'nojs'.
    $form['method'] = array(
      '#type' => 'value',
      '#value' => arg(2) === 'ajax' ? 'ajax' : 'nojs',
    );

    $form['note'] = array(
      '#markup' => '<p>' . t('Are you sure you want to remove the note? This action cannot be undone.') . '</p>',
    );

    $form['note_id'] = array(
      '#type' => 'value',
      '#value' => $note->id,
    );

    $form['note_entity_type'] = array(
      '#type' => 'value',
      '#value' => $note->entity_type,
    );

    $form['note_bundle'] = array(
      '#type' => 'value',
      '#value' => $note->bundle,
    );

    $form['note_entity_id'] = array(
      '#type' => 'value',
      '#value' => $note->entity_id,
    );

    $form['note_field_name'] = array(
      '#type' => 'value',
      '#value' => $note->field_name,
    );

    $form['actions'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('element-inline', 'editor-note-actions'),
      ),
    );

    $form['actions']['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#submit' => array('editor_note_confirm_form_cancel'),
    );

    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#submit' => array('editor_note_confirm_remove_form_remove'),
    );
  }
  else {
    $form['note'] = array(
      '#markup' => '<p>' . t('Selected note cannot be loaded. Cancel action and reload the page to refresh notes widget.') . '</p>',
    );

    $form['actions']['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#submit' => array('editor_note_confirm_form_cancel'),
    );
  }

  return $form;
}

/**
 * Submit callback for 'Remove' button in 'editor_note_confirm_remove_form'.
 */
function editor_note_confirm_remove_form_remove(&$form, &$form_state) {
  $message_text = t('Note has been removed.');
  $field_name = $form_state['values']['note_field_name'];
  $entity_type = $form_state['values']['note_entity_type'];
  $entity_id = $form_state['values']['note_entity_id'];
  $note_id = $form_state['values']['note_entity_id'];
  $edit_path = implode('/', array($entity_type, $entity_id, 'edit'));

  // Removes note entity from the database.
  editor_note_delete($form_state['values']['note_id']);

  if ($form_state['values']['method'] == 'ajax') {
    // Closes the modal.
    $form_state['ajax_commands'][] = ctools_modal_command_dismiss();

    // Displays remove message.
    $message = theme(
      'editor_note_message',
      array(
        'field_name' => $field_name,
        'message_text' => $message_text,
        'message_type' => 'status',
      )
    );

    $form_state['ajax_commands'][] = ajax_command_replace('#status_message_' . $field_name, $message);

    // Updates notes table and pager.
    $field = field_info_field($field_name);
    $entity_array = entity_load($entity_type, array($entity_id));
    $entity = $entity_array[$note_id];
    $notes = editor_note_get_notes($entity_type, $entity, $field);
    $formatted_notes = editor_note_get_formatted_notes($field, $notes, TRUE, $edit_path);
    $form_state['ajax_commands'][] = ajax_command_replace('#formatted_notes_' . $field_name, drupal_render($formatted_notes));
  }
  else {
    drupal_set_message($message_text);
    $form_state['redirect'] = implode('/', array(
      $entity_type,
      $entity_id,
      'edit',
    ));
  }
}

/**
 * Submit callback for 'Cancel' button in 'editor_note_confirm_remove_form'.
 */
function editor_note_confirm_form_cancel(&$form, &$form_state) {
  if ($form_state['values']['method'] == 'ajax') {
    $field_name = $form_state['values']['note_field_name'];

    // Closes the modal.
    $form_state['ajax_commands'][] = ctools_modal_command_dismiss();

    // Removes previous status message.
    $message = theme('editor_note_message', array('field_name' => $field_name));

    $form_state['ajax_commands'][] = ajax_command_replace('#status_message_' . $field_name, $message);
  }
  else {
    $form_state['redirect'] = implode('/', array(
      $form_state['values']['note_entity_type'],
      $form_state['values']['note_entity_id'],
      'edit',
    ));
  }
}
