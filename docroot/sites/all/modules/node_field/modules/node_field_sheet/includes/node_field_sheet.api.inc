<?php

/**
 * Widget for node field sheet.
 * @param array $node_field Node field data.
 * @return array Form array.
 */
function node_field_sheet_widget($node_field) {
  $element_id = drupal_clean_css_identifier('sheet-edit-' . $node_field['machine_name']);
  $default = NULL;

  if (!empty($node_field['value'])) {
    $default = $node_field['value'];
  }
  $output = _sheetnode_inject('sheetfield-' . $element_id, 'sheetnode', $default, $element_id, NULL);

  $form = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => $element_id),
    '#prefix' => theme('form_element_label', array('element' => array('#title' => $node_field['title'], '#title_display' => 'before'))),
    '#suffix' => $output,
  );
  return $form;
}

/**
 * Formatter for node field sheet.
 * @param array $node_field Node field data.
 * @return sheetnode data.
 */
function node_field_sheet_formatter($node_field) {
  if (empty($node_field['value'])) {
    return;
  }
  $output = _sheetnode_inject('sheetfield-' . $node_field['nid'], 'sheetnode', $node_field['value'], FALSE, NULL);
  return $output;
}