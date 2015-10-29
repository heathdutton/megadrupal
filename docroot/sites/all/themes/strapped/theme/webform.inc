<?php
/**
 * @file
 * Webform related theme functions.
 */


/**
 * Replacement for theme_form_element().
 */
function strapped_webform_element($variables) {
  // let make webform use the same funciton as regular fields.
  return strapped_form_element($variables);
}

/**
 * @todo should this be here or in smartsite?
 * @param $form
 * @param $form_state
 */
function strapped_form_webform_client_form_alter(&$form, $form_state) {

  // Pull the indent option from the webform and apply it to the actions wrapper.
  if (isset($form['#node']->webform['element_offset']) && $form['#node']->webform['element_offset'] != 0) {
    $form['actions']['#form_type'] = 'horizontal';
    $form['actions']['#title_display'] = 'inline';
    $form['actions']['#element_offset'] = $form['#node']->webform['element_offset'];
    unset($form['actions']['#theme_wrappers']);
  }

}


/**
 * Called by form elements theme function to merge in webform properties.
 */
function _strapped_merge_webform_properties(&$element, &$attributes) {


  if (isset($element['#webform_component']) && $element['#wrapper_attributes']) {
    // remove webform container inline as its a pain
    $index = array_search('webform-container-inline', $element['#wrapper_attributes']['class']);
    if ($index !== FALSE) {
      unset($element['#wrapper_attributes']['class'][$index]);
    }
    // webforms need the webform classes for conditional stuff
    $attributes['class'] =  array_merge(  $attributes['class'],$element['#wrapper_attributes']['class'] );
  }


  // We only need bother if this is actually a webform component.
  if (isset($element['#webform_component'])) {


    // Basic grid vs Advanced Grid
    if ($element['#webform_component']['extra']['grid'] == 0) { // BASIC GRID

      // If a webform has a title width set via the bootstrap_elements module then add it to the right place
      if (isset($element['#webform_component']['extra']['basic_grid']['title_width'])) {
        $element['#title_width'] =  $element['#webform_component']['extra']['basic_grid']['title_width'];
      }

      // If a webform has an element_offset  set via the bootstrap_elements module then add it to the right place
      if (isset($element['#webform_component']['extra']['basic_grid']['element_offset'])) {
        $element['#element_offset'] = $element['#webform_component']['extra']['basic_grid']['element_offset'];
      }

    } else { // ADVANCED GRID
      if (isset($element['#webform_component']['extra']['advanced_grid']['title_grid'])) {
        $element['#title_grid'] =  $element['#webform_component']['extra']['advanced_grid']['title_grid'];
      }
      if (isset($element['#webform_component']['extra']['advanced_grid']['element_grid'])) {
        $element['#element_grid'] =  $element['#webform_component']['extra']['advanced_grid']['element_grid'];
      }
    }


    // If a webform has a form_type set via the bootstrap_elements module then add it to the right place
    if (isset($element['#webform_component']['extra']['form_type'])) {
      $element['#form_type'] = $element['#webform_component']['extra']['form_type'];
    }


    // If a webform has an title_tooltip  set via the bootstrap_elements module then add it to the right place
    if (isset($element['#webform_component']['extra']['tooltips']['title_tooltip'])) {
      $element['#title_tooltip'] = $element['#webform_component']['extra']['tooltips']['title_tooltip'];
    }


    // If a webform has an suffix_tooltip  set via the bootstrap_elements module then add it to the right place
    if (isset($element['#webform_component']['extra']['tooltips']['suffix_tooltip'])) {
      $element['#suffix_tooltip'] = $element['#webform_component']['extra']['tooltips']['suffix_tooltip'];
    }

    // If a webform has an prefix_tooltip  set via the bootstrap_elements module then add it to the right place
    if (isset($element['#webform_component']['extra']['tooltips']['prefix_tooltip'])) {
      $element['#prefix_tooltip'] = $element['#webform_component']['extra']['tooltips']['prefix_tooltip'];
    }

    // If a webform has an input_group  set via the bootstrap_elements module then add it to the right place
    if (isset($element['#webform_component']['extra']['input_group'])) {
      $element['#input_group'] = $element['#webform_component']['extra']['input_group'];
    }


    // Wrappers
    if (isset($element['#webform_component']['extra']['wrappers']['form_field_wrapper'])) {
      $element['#form_field_wrapper'] = $element['#webform_component']['extra']['wrappers']['form_field_wrapper'];
    }
    if (isset($element['#webform_component']['extra']['wrappers']['form_type_wrapper'])) {
      $element['#form_type_wrapper'] = $element['#webform_component']['extra']['wrappers']['form_type_wrapper'];
    }
    if (isset($element['#webform_component']['extra']['wrappers']['form_group_wrapper'])) {
      $element['#form_group_wrapper'] = $element['#webform_component']['extra']['wrappers']['form_group_wrapper'];
    }


  // Dealt with webform matrix components
    if (isset($element['#webform_component']['type']) && $element['#webform_component']['type'] == 'matrix'  ) {
      $element['#title_display'] = $element['#webform_component']['extra']['title_display'];
    }




    // Deal with the markup element in webforms.
    if (isset($element['#type']) && in_array($element['#type'], array(
        'markup',
        'fieldset',
            'matrix'
      )) && isset($element['#webform_component']['extra']['title_display'])
    ) {
      $element['#title_display'] = $element['#webform_component']['extra']['title_display'];
      $element['#title'] = $element['#webform_component']['extra']['title'];
    }


  }



}



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
      $cols[] = drupal_render($element[$i][$j]);
    }
    $rows[] = $cols;
  }

  $rows[] = array(array('data' => drupal_render($element['add_row']), 'colspan' => $matrix_col));
  $form_class = str_replace('_', '-', $form_key);

  $form = array();


  // Add the children
  $variables['element']['#title'] = 'test';

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

