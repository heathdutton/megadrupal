<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */


/**
 * OpenLayers Print Behavior
 */
class openlayers_behavior_print extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'processing_controls' => array(),
      'editing_controls' => array(),
      'other_controls' => array(),
    );
  }

  /** configuration form */
  function options_form($defaults = array()) {
    $form = array();
    return $form;
  }

  function js_dependency() {
    return array();
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_css(drupal_get_path('module', 'openlayers_print') . '/includes/openlayers_behavior_print.css');
    drupal_add_js(drupal_get_path('module', 'openlayers_print') . '/includes/openlayers_behavior_print.js');

    // add to map ???

    return $this->options;
  }
}


// vim: ft=php tabstop=2 shiftwidth=2
