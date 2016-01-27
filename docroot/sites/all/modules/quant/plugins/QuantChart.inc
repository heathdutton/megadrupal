<?php

/**
 * QuantChart class used to provide chart-rendering plugins
 */
abstract class QuantChart {
  var $quant = NULL;
  var $plugin = NULL;
  
  function __construct($quant, $plugin) {
    $this->quant = $quant;
    $this->plugin = $plugin;
  }

  /**
   * Perform all operations to build data required to output a chart
   */
  abstract function build();
  
  /**
   * Return the chart output in HTML format
   */
  abstract function render();
  
  /**
   * Provide a list of Drupal variables handled by this plugin so they
   * can be properly removed during module uninstall
   * 
   * @return
   *   An array of variable names
   */
  function variables() {
    return array();
  }
  
  /**
   * Provide additional admin settings to the form
   */
  function adminSettings() {
    return NULL;
  }
  
  /**
   * Validate the admin settings form
   */
  function adminSettingsValidate(&$form, &$form_state) {
    
  }
  
  /**
   * Submit the admin settings form
   */
  function adminSettingsSubmit(&$form, &$form_state) {
    
  }
}
