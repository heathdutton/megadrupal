<?php
/**
 * @file
 * Functions related to database call for opac marc module.
 */

/**
 * Get marc structure.
 *
 * @param string $type
 *   A string containing marc structure type.
 *
 * @return array
 *   Marc structure.
 */
function opac_marc_structure_get($type) {
  $query = db_select('opac_marc_subfield_structure', 't');

  $query->fields('t')
    ->condition('t.structure_type', $type, '=')
    ->extend('PagerDefault')->limit(20);

  $structure = array();
  $result = $query->execute();
  while ($element = $result->fetchAssoc()) {
    $structure[$element['tag']][$element['subfield']] = $element;
  }
  return $structure;
}

/**
 * Get existing marc structure types.
 *
 * @return array
 *   Marc structure types.
 */
function opac_marc_structure_types_get() {
  $select = db_select('opac_marc_structure_types', 'types')
    ->fields('types');

  $types = array();
  $result = $select->execute();
  while ($type = $result->fetchAssoc()) {
    $types[$type['structure_type']] = $type['structure_label'];
  }
  return $types;
}

/**
 * Add marc structure type.
 *
 * @param array $structure_type
 *   A string containing marc structure type name and label.
 */
function opac_marc_structure_type_add($structure_type) {
  $result = db_insert('opac_marc_structure_types')
    ->fields(array(
      'structure_type' => $structure_type['structure_type'],
      'structure_label' => $structure_type['structure_label'],
    ))
    ->execute();
}

/**
 * Add marc subfield.
 *
 * @param array $values
 *   An array containing subfield values.
 */
function opac_marc_structure_subfields_add($values) {
  $result = db_insert('opac_marc_subfield_structure')
    ->fields(array(
      'tag' => $values['tag'],
      'subfield' => $values['subfield'],
      'structure_type' => $values['structure_type'],
      'label' => $values['label'],
      'hidden' => $values['hidden'],
    ))
    ->execute();
}

/**
 * Make a marc tag visible.
 *
 * @param string $type
 *   Marc structure type name.
 *
 * @param string $tag
 *   Marc tag of the subfield.
 *
 * @param string $subfield
 *   Marc subfield to make visible.
 */
function opac_marc_structure_subfields_show($type, $tag, $subfield) {
  $success = db_update('opac_marc_subfield_structure')
    ->fields(array(
      'hidden' => 0,
    ))
    ->condition('tag', $tag)
    ->condition('subfield', $subfield)
    ->condition('structure_type', $type)
    ->execute();
}

/**
 * Make a marc tag hidden.
 *
 * @param string $type
 *   Marc structure type name.
 *
 * @param string $tag
 *   Marc tag of the subfield.
 *
 * @param string $subfield
 *   Marc subfield to make hidden.
 */
function opac_marc_structure_subfields_hide($type, $tag, $subfield) {
  $success = db_update('opac_marc_subfield_structure')
    ->fields(array(
      'hidden' => 1,
    ))
    ->condition('tag', $tag)
    ->condition('subfield', $subfield)
    ->condition('structure_type', $type)
    ->execute();
}

/**
 * Delete a marc subfield.
 *
 * @param string $type
 *   Marc structure type name.
 *
 * @param string $tag
 *   Marc tag of the subfield.
 *
 * @param string $subfield
 *   Marc subfield to delete.
 */
function opac_marc_structure_subfields_delete($type, $tag, $subfield) {
  $success = db_delete('opac_marc_subfield_structure')
    ->condition('tag', $tag)
    ->condition('subfield', $subfield)
    ->condition('structure_type', $type)
    ->execute();
}
