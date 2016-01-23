<?php

/**
 * @file
 * Contains the Salvattore style plugin.
 */

class views_plugin_style_salvattore extends views_plugin_style {

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['class'] = array('default' => '');

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }
}
