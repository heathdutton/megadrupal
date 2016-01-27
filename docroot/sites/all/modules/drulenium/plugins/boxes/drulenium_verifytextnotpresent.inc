<?php

/**
 * @file drulenium_verifytextnotpresent.inc
 * 
 * Box class for "verifytextnotpresent" plugin type 
 */

class drulenium_verifytextnotpresent extends drulenium_default {
  
  public $test_plugin_name;

  protected function __construct() {
    parent::__construct();
    $this->test_plugin_name = 'verify_text_not_present';
  }

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    return parent::options_defaults();
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    return parent::options_form($form_state);
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    return $block;
  }
}