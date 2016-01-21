<?php

/**
 * @file
 * Style plugin to render each item in an ordered or unordered list.
 * @ingroup views_style_plugins
 */
class mediabox_ui_grid_style_plugin extends views_plugin_style {

  function option_definition() {
    $options = parent::option_definition();

    $options['columns'] = array('default' => '3');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // no options yet

    $form['columns'] = array(
      '#type' => 'textfield',
      '#title' => t('Columns'),
      '#size' => 2,
      '#default_value' => $this->options['columns'],
    );
  }

}
