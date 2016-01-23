<?php

/**
 * Theme for webform matrix.
 */
function strapped_webform_matrix_table($variables) {
    $element = $variables['element'];
    $component = $element['#webform_component'];
    $form_key = $component['form_key'];
    $pid = $component['pid'];
    $id = "edit-$form_key-$pid";
    $matrix_col = $component['extra']['matrix_col'];
    $matrix_row = $element['#temp_max_row']; //$component['extra']['matrix_row'];

    $headers = $element['#headers'];
    // Make table headers translatable
    foreach ($headers as $key => $header) {
        $headers[$key] = t($header);
    }

    $rows = array();
    for ($i = 1; $i <= $matrix_row; $i++) {
        $cols = array();
        for ($j = 1; $j <= $matrix_col; $j++) {
            // Make select options translatable
            // this includes the default "Select" label
            if ($element[$i][$j]['#type'] === 'select') {
                // To translate just the "Select" label
                // use the following line instead of the foreach loop
                // $element[$i][$j]['#options'][''] = t($element[$i][$j]['#options']['']);
                foreach ($element[$i][$j]['#options'] as $key => $option) {
                    $element[$i][$j]['#options'][$key] = t('@option', array('@option' => $option));
                }
            }
            // @strapped - remove wrappers
            $element[$i][$j]['#form_group_wrapper'] = FALSE;
            $cols[] = drupal_render($element[$i][$j]);
        }
        $rows[] = $cols;
    }

    // @strapped - only render the add_row if it is turned on.
    if (isset($element['add_row'])) {
        $rows[] = array(array('data' => drupal_render($element['add_row']), 'colspan' => $matrix_col));
    }

    $form_class = str_replace('_', '-', $form_key);

    $form = array();


    // Add the children
    $variables['element']['#title'] = $element['#matrix_header'] ;
    $variables['element']['#wrapper_attributes']['class'] = array('webform-component', 'webform-component-matrix', 'webform-component--'.$form_class, 'webform-component-'.$form_class);
    $variables['element']['#wrapper_attributes']['id'] = $id;
    $variables['element']['#children'] = theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => array("$id-table"))));
    $variables['element']['#children'] .= drupal_render_children($element);

    return theme('form_element', $variables);

//
//  $output = "<div class='form-item webform-component webform-component-matrix webform-component--$form_class webform-component-$form_class' id='$id'>SASQUATCH";
//  $output .=!empty($element['#matrix_header']) ? "<label>" . $element['#matrix_header'] . "</label>" : "";
//  $output .= theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => array("$id-table"))));
//  $output .= drupal_render_children($element);
//  $output .= "</div>";
//  return $output;
}

