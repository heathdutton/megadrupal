<?php

/**
 * @file
 * Style plugin to render each item in an ordered or unordered list.
 * @ingroup views_style_plugins
 */
class mediabox_ui_style_plugin extends views_plugin_style {

  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // no options yet
  }

  function validate() {
    $errors = parent::validate();

    return array();
  }

}
