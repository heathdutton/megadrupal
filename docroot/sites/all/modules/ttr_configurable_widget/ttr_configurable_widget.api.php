<?php 
/**
 * @file
 * Documents API functions for TTR Configurable Widget module.
 */

/**
 * Defines the basic information of your view.
 * @return array
 *   An associative array whose keys are display names and whose values
 *   are an associative array containing:
 *    - title: The user friendly title of your display.
 *    - description: A brief description of your display.
 * 
 * @see hook_ttrcw_VIEW_ID_view_form()
 */
function hook_ttrcw_view_info() {
  return array(
    'display_name' => array(
      'title' => t('Display name'),
      'description' => t('Description for the display.'),
    ),
  );
}

/**
 * Defines the form element of your display.
 * @return array
 *   An associative array representing a form element.
 */
function hook_ttrcw_VIEW_ID_view_form() {
  $form_element = array();

  $form_element = array(
    '#markup' => t('Define the form element in this array.'),
  );

  return $form_element;
}
