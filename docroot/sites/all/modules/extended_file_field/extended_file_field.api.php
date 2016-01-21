<?php

/**
 * @file
 * API documentation for hooks provided by the extended_file_field module.
 */

/**
 * Implementation of hook_extended_file_field_metadata_types().
 *
 * Defines additional metadata provided to the extended_file_field formatter.
 * Returns a parent array keyed by metadata type, where the elements
 * themselves are arrays with the following keys:
 *   - 'title' (Required): The human-readable name of this metadata type, used
 *       as the column header in the resulting table, and
 *   - 'sort' (Optional): Used to determine how the table field should be
 *       ordered when this metadata type is used for sorting.  Intended values
 *       are 'numeric' for numeric sorting, or 'string' for strcomp() sorting.
 *       Setting 'sort' == FALSE will prevent the option from appearing in the
 *       'sort by' settings field.
 *   - 'formatter' (Optional): Defines the function name used to format the
 *       metadata output for this particular metadata type. The function
 *       defined here will be passed the complete $items array, and should
 *       return $output suitable for including as the 'data' parameter of a
 *       table cell (i.e. html output or a render array).
 *
 * @return array
 *   An associative array of metadata information provided by this module.
 */
function hook_extended_file_field_metadata_types() {
  return array(
    'cid' => array(
      'title' => t('Comment ID'),
      'sort' => 'numeric',
      'formatter' => 'mymodule_extended_file_field_cid_formatter',
    ),
  );
}

/**
 * Implementation of hook_extended_file_field_items_alter().
 *
 * Allows external modules to modify the $items array of files before
 * rendering the extended_file_field formatter output.
 *
 * @param array $items
 *   The array of file items from the field, keyed by file ID (fid).
 * @param array $context
 *   Associative array of context for the table of files being altered, with
 *   the following keys:
 *   - field: The field definition array.
 *   - instance: The field instance definition array.
 *   - entity: An object representing the entity the file field is attached to.
 *   - entity_type: String with the type of entity the field is attached to.
 *   - langcode: The language associated with $items.
 *   - display: The display settings to use, as found in the 'display' entry of
 *     the instance definition. Notable keys include the name of the formatter
 *     (in 'type') and the array of formatter settings (in 'settings').
 *
 *  @see drupal_alter()
 */
function hook_extended_file_field_items_alter(&$items, &$context) {
  // Add a 'Comment ID' property to each item in the $items array.
  $file_cids = nodechanges_get_file_cids($context['entity']->nid);
  foreach ($items as $key => $value) {
    // Add the comment id associated with each file to the $items array.
    // If no comment id is found for a given file, set it to '0'.
    $items[$key]['cid'] = isset($file_cids[$value['fid']]) ? $file_cids[$value['fid']] : 0;
  }
}

/**
 * Implementation of hook_extended_file_field_widget_items_alter().
 *
 * Allows external modules to modify file $items located within a
 * theme_extended_file_field_widget_multiple() call, before rendering the final
 * extended_file_field widget output.
 *
 * @param array $items
 *   The array of file items from the field, keyed by file ID (fid).
 * @param array $context
 *   Associative array of context for the table of files being altered, with
 *   the following keys:
 *   - field: The field definition array.
 *   - instance: The field instance definition array.
 *   - entity: An object representing the entity the file field is attached to.
 *   - entity_type: String with the type of entity the field is attached to.
 *   - langcode: The language associated with $items.
 *   - display: The display settings to use, as found in the 'display' entry of
 *     the instance definition. Notable keys include the name of the formatter
 *     (in 'type') and the array of formatter settings (in 'settings').
 *
 *  @see drupal_alter()
 */
function hook_extended_file_field_widget_items_alter(&$items, &$context) {
  // Add a 'Comment ID' property to each item in the $items array.
  $file_cids = nodechanges_get_file_cids($context['entity']->nid);
  foreach ($items as $key => $item) {
    // Add the comment id associated with each file to the $items array.
    // If no comment id is found for a given file, set it to '0'.
    $items[$key]['cid'] = isset($file_cids[$item['fid']]) ? $file_cids[$item['fid']] : 0;
  }
}

/**
 * Implementation of hook_extended_file_field_output_alter().
 *
 * Allows external modules to modify the table structure prior to rendering the
 * final extended_file_field formatter output.
 *
 * @param array $elements
 *   The extended_file_field render array. The contents of this array are
 *   dependent on the field formatter configuration settings, and the presence
 *   of any files with 'display' set to FALSE.
 *   This should always return a render array for the file table, with the
 *   $rows array keyed by file id (fid):
 *     array(
 *       '#theme' => 'table',
 *       '#header' => $header,
 *       '#rows' => $rows,
 *      )
 *   If $settings['showhidden'] == 'table', and there are hidden
 *   files to be displayed, then $elements will contain a second render
 *   array containing a 'Hidden Files' label and the table of hidden files:
 *     array(
 *       '#markup' => t('Hidden Files'),
 *     ),
 *     array(
 *       '#theme' => 'table',
 *       '#header' => $header,
 *       '#rows' => $hiddenrows,
 *     )
 *  If $settings['usefieldset'] == TRUE, then this hidden files array will
 *  be wrapped in a collapsed fieldset before being added to the render array:
 *    array(
 *      'fieldset' => array(
 *        '#type' => 'fieldset',
 *        '#title' => t('Hidden files'),
 *        '#attributes' => array('class' => array('collapsible', 'collapsed')),
 *        'content' => array($table),
 *      ),
 *    )
 *  If adding rows to the table, be sure to inherit the row class from the
 *  related file row, to ensure your additions abide by the formatter's
 *  show/hide settings.
 *
 * @param array $context
 *   Associative array of context for the table of files being altered, with
 *   the following keys:
 *   - items: Array of file items included in the file field.
 *   - field: The field definition array.
 *   - instance: The field instance definition array.
 *   - entity: An object representing the entity the file field is attached to.
 *   - entity_type: String with the type of entity the field is attached to.
 *   - langcode: The language associated with $items.
 *   - display: The display settings to use, as found in the 'display' entry of
 *     the instance definition. Notable keys include the name of the formatter
 *     (in 'type') and the array of formatter settings (in 'settings').
 *
 * @see theme_table()
 * @see drupal_alter()
 *
 */
function hook_extended_file_field_output_alter(&$elements, &$context) {
  $items = $context['items'];
  $entity = $context['entity'];
  // Update the contents of all 'Comment ID' cells to provide 'jump' links.
  foreach ($elements[0]['#rows'] as $key => $row) {
    foreach ($row as $column => $cell) {
      if ($column == 'cid') {
        if ($items[$key]['cid'] === 0) {
          $data = l('none', 'node/' . $entity->nid, array('fragment' => 'content'));
        }
        else {
          $data = l('#' . comment_get_display_ordinal($items[$key]['cid'], $entity->type), 'node/' . $entity->nid, array('fragment' => 'comment-' . $items[$key]['cid']));
        }
        $elements[0]['#rows'][$key][$column]['data'] = $data;
      }
    }
  }
}

/**
 * Implements a file metadata property formatter.
 *
 * Used to format the output for an externally provided metadata property.  The
 * actual function name must match that given in the 'formatter' property for
 * this metadata type, as defined in hook_extended_file_field_metadata_types().
 * This function is passed the complete $file item for context, but should only
 * return the output for the particular piece of metadata defined.
 *
 * @param array $item
 *   An array of file metadata types and values for the file in question.
 * @param array $context
 *   Associative array of context for the table of files being altered, with
 *   the following keys:
 *   - field: The field definition array.
 *   - instance: The field instance definition array.
 *   - entity: An object representing the entity the file field is attached to.
 *   - entity_type: String with the type of entity the field is attached to.
 *   - langcode: The language associated with $items.
 *   - display: The display settings to use, as found in the 'display' entry of
 *     the instance definition. Notable keys include the name of the formatter
 *     (in 'type') and the array of formatter settings (in 'settings').
 *
 * @return array or string
 *   A render array defining the table cell contents for this file/metadata
 *   combination, or alternatively an html string defining the actual rendered
 *   output.
 */
function mymodule_extended_file_field_cid_formatter($item, $context) {
  // Generate formatted Comment ID jumplinks for a file, based on the 'cid'
  // metadata property.
  $entity = $context['entity'];
  if ($item['cid'] === 0) {
    $data = l('none', 'node/' . $entity->nid, array('fragment' => 'content'));
  }
  else {
    $data = l('#' . comment_get_display_ordinal($item['cid'], $entity->type), 'node/' . $entity->nid, array('fragment' => 'comment-' . $item['cid']));
  }
  return $data;
}
