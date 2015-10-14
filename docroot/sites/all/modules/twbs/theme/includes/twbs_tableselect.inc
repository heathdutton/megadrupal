<?php

/**
 * Returns HTML for a table with radio buttons or checkboxes.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties and children of
 *     the tableselect element. Properties used: #header, #options, #empty,
 *     and #js_select. The #options property is an array of selection options;
 *     each array element of #options is an array of properties. These
 *     properties can include #attributes, which is added to the
 *     table row's HTML attributes; see theme_table(). An example of per-row
 *     options:
 *     @code
 *     $options = array(
 *       array(
 *         'title' => 'How to Learn Drupal',
 *         'content_type' => 'Article',
 *         'status' => 'published',
 *         '#attributes' => array('class' => array('article-row')),
 *       ),
 *       array(
 *         'title' => 'Privacy Policy',
 *         'content_type' => 'Page',
 *         'status' => 'published',
 *         '#attributes' => array('class' => array('page-row')),
 *       ),
 *     );
 *     $header = array(
 *       'title' => t('Title'),
 *       'content_type' => t('Content type'),
 *       'status' => t('Status'),
 *     );
 *     $form['table'] = array(
 *       '#type' => 'tableselect',
 *       '#header' => $header,
 *       '#options' => $options,
 *       '#empty' => t('No content available.'),
 *     );
 *     @endcode
 *
 * @ingroup themeable
 */
function twbs_tableselect($variables) {
  $element = $variables['element'];
  $rows = array();
  $header = $element['#header'];
  if (!empty($element['#options'])) {
    // Generate a table row for each selectable item in #options.
    foreach (element_children($element) as $key) {
      $row = array();

      $row['data'] = array();
      if (isset($element['#options'][$key]['#attributes'])) {
        $row += $element['#options'][$key]['#attributes'];
      }
      // Render the checkbox / radio element.
      $row['data'][] = drupal_render($element[$key]);

      // As theme_table only maps header and row columns by order, create the
      // correct order by iterating over the header fields.
      foreach ($element['#header'] as $fieldname => $title) {
        $row['data'][] = $element['#options'][$key][$fieldname];
      }
      $rows[] = $row;
    }
    // Add an empty header or a "Select all" checkbox to provide room for the
    // checkboxes/radios in the first table column.
    if ($element['#js_select']) {
      // Add a "Select all" checkbox.
      drupal_add_js('misc/tableselect.js');
      array_unshift($header, array('class' => array('select-all')));
    }
    else {
      // Add an empty header when radio buttons are displayed or a "Select all"
      // checkbox is not desired.
      array_unshift($header, '');
    }
  }
  return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => $element['#empty'], 'attributes' => $element['#attributes']));
}
