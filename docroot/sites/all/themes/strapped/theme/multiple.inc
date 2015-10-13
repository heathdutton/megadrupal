<?php
/**
 * @file
 * Multiple field related theme functions.
 */


/**
 * Returns HTML for an individual form element.
 *
 * Combine multiple values into a table with drag-n-drop reordering.
 * TODO : convert to a template.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element representing the form element.
 *
 * @ingroup themeable
 */
function strapped_field_multiple_value_form($variables) {
  $element = $variables['element'];


  // for field collections we must copy the layout settings to the correct locations.
  if (isset($element[0]['#entity_type'] ) && $element[0]['#entity_type'] == 'field_collection_item') {
    // Define the array of fields
    $fields = array(
      'form_type',
      'title_display',
      'title_width',
      'field_prefix',
      'field_suffix',
      'input_group',
      'title_tooltip',
      'element_offset',
      'element_columns',
    );

    foreach ($fields as $key => $field) {
      if (isset($element[0]['#' . $field])) {
        $element['#' . $field] = $element[0]['#' . $field];
        unset($element[0]['#' . $field]);
      }
    }
  }


  $output = '';

  $element += array(
    '#title_width' => '2',
  );

  // @todo add the ability for tooltips to appear here.

  if ($element['#title_display'] == 'inline') {
    $output .= '<div class="row"><div class="col-md-' . $element['#title_width'] . ' bootstrap-label">' . $element['#title'] . '</div><div class="col-md-' . (12 - $element['#title_width']) . '">';
  }

  if ($element['#cardinality'] > 1 || $element['#cardinality'] == FIELD_CARDINALITY_UNLIMITED) {
    $table_id = drupal_html_id($element['#field_name'] . '_values');
    $order_class = $element['#field_name'] . '-delta-order';
    $required = !empty($element['#required']) ? theme('form_required_marker', $variables) : '';

    $header = array(
      array(
        'data' => '<label>' . t('!title !required', array(
            '!title' => $element['#title'],
            '!required' => $required
          )) . "</label>",
        'colspan' => 2,
        'class' => array('field-label'),
      ),
      t('Order'),
    );
    if ($element['#title_display'] == 'inline') {
      $header = array();
    }

    $rows = array();

    // Sort items according to '_weight' (needed when the form comes back after
    // preview or failed validation)
    $items = array();
    foreach (element_children($element) as $key) {
      if ($key === 'add_more') {
        $add_more_button = & $element[$key];
      }
      else {
        $items[] = & $element[$key];
      }
    }
    usort($items, '_field_sort_items_value_helper');

    // Add the items as table rows.
    foreach ($items as $key => $item) {
      $item['_weight']['#attributes']['class'] = array($order_class);
      $delta_element = drupal_render($item['_weight']);
      $cells = array(
        array('data' => '', 'class' => array('field-multiple-drag')),
        drupal_render($item),
        array('data' => $delta_element, 'class' => array('delta-order')),
      );
      $rows[] = array(
        'data' => $cells,
        'class' => array('draggable'),
      );
    }

    $output .= '<div class="form-item">';
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(
        'id' => $table_id,
        'class' => array('field-multiple-table')
      )
    ));
    $output .= $element['#description'] ? '<div class="description">' . $element['#description'] . '</div>' : '';
    $output .= '<div class="clearfix">' . drupal_render($add_more_button) . '</div>';
    $output .= '</div>';

    drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);
  }
  else {
    foreach (element_children($element) as $key) {
      $output .= drupal_render($element[$key]);
    }
  }
  if ($element['#title_display'] == 'inline') {
    $output .= '</div></div>';
  }
  return $output;
}
