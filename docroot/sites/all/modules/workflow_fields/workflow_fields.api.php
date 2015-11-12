<?php
/**
 * @file
 * Hooks provided by the workflow_fields module.
 */

/**
 * Used to describe non-fields to be handled during form rendering.
 *
 * @param $type
 *   Content type being handled.
 *
 * @return
 *   An array of keyed field entries.
 *   key: field name
 *   'label' (required): field label
 *   'path' (optional): explicit field path
 *   'process' (optional): callback to handle hiding / read-only with signature process(&$form, $field, $path, $visible, $editable)
 *   'process arguments' (optional): array of extra arguments to pass to callback
 */
function hook_workflow_fields($type) {
  // The Worflow Fields module uses this hook to manage the title field.
  // Here is an example extracted from workflow_fields.module:
  // Add the title to the list of fields managed by workflow_fields.
  $content = node_type_get_type($type);
  $fields = array();
  if ($content->has_title) {
    $fields['title'] = array(
      'label' => $content->title_label,
      'process' => '_workflow_fields_show_title',
    );
  }
  return $fields;
}

/**
 * Perform alterations to described non-fields before a form is rendered.
 *
 * @param $fields
 *   The array of additional fields defined by hook_workflow_fields.
 *
 * @param $type
 *   Content type being handled.
 */
function hook_workflow_fields_alter(&$fields, $type) {
  // The Worflow Fields module does nothing with this hook.
  // Here is an example how to change the widget label for the page content type.
  $content = node_type_get_type($type);
  if ($content->type == 'page') {
    $fields['page']['title']['widget']['label'] = 'Reference';
  }
}
