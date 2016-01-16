<?php

/**
 * Implementation of hook_ext_application_properties.
 */
function ext_example_ext_application_properties() {
  return array(
    'controllers' => array('node.FieldValues'),
    'autoCreateViewport' => TRUE
  );
}


/**
 * Implementation of hook_ext_additional_js_files.
 */
function ext_example_ext_additional_js_files() {
  return drupal_get_path('module', 'ext_example'). "/js/ext";
}
