<?php
/**
 * @file
 * Etherpad module pages.
 */

/**
 * Form builder for field edit.
 */
function etherpad_edit_form($form, &$form_state, $node, $field) {
  $field_form = array();
  field_attach_form('node', $node, $field_form, $form_state);
  $form[$field] = $field_form[$field];
  $form['#node'] = $node;
  $form['#field'] = $field;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  // Create group, user and start session.
  etherpad_enrich_form($form, $form_state);
  return $form;
}

function etherpad_edit_form_submit($form, &$form_state) {
  $field_name = $form['#field'];
  $node = &$form['#node'];
  $langcode = field_language('node', $node, $field_name);
  $node->{$field_name}[$langcode][0]['value'] = $form_state['values'][$field_name][$langcode]['0']['value'];
  node_save($node);
}
