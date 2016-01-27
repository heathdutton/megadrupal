<?php

class openlayers_blocks_behavior_block extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  function options_init() {
    $options = array();
    return $options;
  }

  /**
   * Override of options_form().
   */
  function options_form($defaults = array()) {
    $form = parent::options_form();
    return $form;
  }

  /**
   * Render.
   */
  function render(&$map) {
    return $this->options;
  }
}
