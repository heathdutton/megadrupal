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
 * Called by form elements theme function to merge in webform properties.
 */
function _strapped_merge_webform_properties(&$element, &$attributes) {


  if (isset($element['#webform_component']) && isset($element['#wrapper_attributes'])) {
    // remove webform container inline as its a pain
    $index = array_search('webform-container-inline', $element['#wrapper_attributes']['class']);
    if ($index !== FALSE) {
      unset($element['#wrapper_attributes']['class'][$index]);
    }
    // webforms need the webform classes for conditional stuff
    $attributes['class'] =  array_merge(  $attributes['class'],$element['#wrapper_attributes']['class'] );

    if(!empty($element['#wrapper_attributes']['id'])){
      $attributes['id'] =  $element['#wrapper_attributes']['id'];
    }

  }


  // We only need bother if this is actually a webform component.
  if (isset($element['#webform_component'])) {



      if (isset($element['#webform_component']['extra']['grid']['title_grid'])) {
        $element['#title_grid'] =  $element['#webform_component']['extra']['grid']['title_grid'];
      }
      if (isset($element['#webform_component']['extra']['grid']['element_grid'])) {
        $element['#element_grid'] =  $element['#webform_component']['extra']['grid']['element_grid'];
      }

//    if (isset($element['#webform_component']['extra']['grid']['element_columns'])) {
//      $element['#element_columns'] =  $element['#webform_component']['extra']['grid']['element_columns'];
//    }

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


