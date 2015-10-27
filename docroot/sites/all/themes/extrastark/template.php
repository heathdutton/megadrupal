<?php
/**
* Preprocesses the wrapping HTML.
*
* @param array &$variables
*   Template variables.
*/
function extrastark_preprocess_html(&$vars) {
  
  // Add viewport zoom for responsible devices to work
  $viewport_zoom = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1.0',
    )
  );
  
  // Add viewport zoom Meta Tag to head
  drupal_add_html_head($viewport_zoom, 'viewport_zoom');

}