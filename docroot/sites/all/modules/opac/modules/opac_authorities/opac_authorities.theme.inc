<?php
/**
 * @file
 * Theme function for opac authorities module.
 */

/**
 * Returns HTML of authority types for a given ILS server.
 *
 * @param array $variables
 *   An associative array containing authority types.
 *
 * @ingroup themeable
 */
function theme_opac_authorities_admin_authority_types_overview_form($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['authtype'])) {
      $item = &$form[$key];

      $row = array();
      $row[] = drupal_render($item['authtype']);
      if (isset($item['weight'])) {
        $item['weight']['#attributes']['class'] = array('opac-authority-types-weight');
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
  $header = array(t('authority type'));
  if (isset($item['weight'])) {
    $header[] = t('Weight');
  }
  $header[] = t('Matching field');
  $header[] = t('Value');
  $header[] = array('data' => t('Operations'), 'colspan' => '3');
  drupal_add_tabledrag('opac-authority-types', 'order', 'sibling', 'opac-authority-types-weight');

  return theme(
    'table',
    array(
      'header' => $header,
      'rows' => $rows,
      'empty' => t('No type available.'),
      'attributes' => array('id' => 'opac-authority-types'),
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
function theme_opac_authorities_mapping_overview($variables) {
  $mapping = $variables['mapping'];

  $serv_id = $variables['serv_id'];

  // Build HTML output.
  $output = '<div>' . t('Click on a field to map it.') . '</div>';
  foreach ($mapping as $data) {
    $fieldname = $data['field_name'];
    $href = '/admin/config/opac/server/' . $serv_id . '/authority-mapping/' . $data['authtype'] . '/' . $fieldname . '/edit';
    $output .= "<div><a href='$href'>" . $data['bundle'] . ': ' . $data['label'] . " (<cite>" . $fieldname . "</cite>)</a></div>";
  }
  return $output;
}
