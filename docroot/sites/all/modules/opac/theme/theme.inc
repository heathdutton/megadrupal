<?php

/**
 * @file
 * Theme function for opac module.
 */

/**
 * Returns HTML of content types for a given ILS server.
 *
 * @param array $variables
 *   An associative array containing content types.
 *
 * @ingroup themeable
 */
function theme_opac_admin_content_types_overview_form($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['content_type'])) {
      $item = &$form[$key];

      $row = array();
      $row[] = drupal_render($item['content_type']);
      if (isset($item['weight'])) {
        $item['weight']['#attributes']['class'] = array('opac-content-types-weight');
        $row[] = drupal_render($item['weight']);
      }
      $row[] = drupal_render($item['matching_field']);
      $row[] = drupal_render($item['value']);
      $row[] = drupal_render($item['edit']);
      $row[] = drupal_render($item['mapping']);
      $row[] = drupal_render($item['delete']);

      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
  }
  $header = array(t('Content type'));
  if (isset($item['weight'])) {
    $header[] = t('Weight');
  }
  $header[] = t('Matching field', array(), array('context' => 'libraries'));
  $header[] = t('Value');
  $header[] = array('data' => t('Operations'), 'colspan' => '3');
  drupal_add_tabledrag('opac-content-types', 'order', 'sibling', 'opac-content-types-weight');

  return theme(
    'table',
    array(
      'header' => $header,
      'rows' => $rows,
      'empty' => t('No content type available.'),
      'attributes' => array('id' => 'opac-content-types'),
    )
  ) . drupal_render_children($form);
}

/**
 * Returns HTML of unmapped fields for a given ILS server.
 *
 * @param array $variables
 *   An associative array containing:
 *   - mapping: unmapped fileds for a given ILS server.
 *   - serv_id: ILS server identifier.
 *
 * @ingroup themeable
 */
function theme_opac_mapping_overview($variables) {
  $mapping = $variables['mapping'];
  $serv_id = $variables['serv_id'];

  // Build HTML output.
  $output = '<div>' . t('Click on a field to map it.', array(), array('context' => 'libraries')) . '</div>';
  foreach ($mapping as $data) {
    $fieldname = $data['field_name'];
    $link = l($data['bundle'] . ': ' . $data['label'], "admin/config/opac/server/$serv_id/mapping/" . $data['content_type'] . "/$fieldname/edit");
    $output .= "<div>$link (<cite>$fieldname</cite>)</div>";
  }
  return $output;
}
