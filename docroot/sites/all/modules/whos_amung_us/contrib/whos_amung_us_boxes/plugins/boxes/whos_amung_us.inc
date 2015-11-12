<?php

/**
 * @file
 * Boxes plugin for the Whos.amung.us module.
 *
 * @author Francisco J. Cruz Romanos <grisendo@gmail.com>
 */

class whos_amung_us extends boxes_box {

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $default_options = whos_amung_us_default_form_values();
    return $default_options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    module_load_include('inc', 'whos_amung_us', 'includes/whos_amung_us.admin');
    $options = array(
      'values' => $this->options,
    );
    $options['values'] += whos_amung_us_default_form_values();
    return whos_amung_us_configuration_form(array(), $options, 'boxes');
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $options = $this->options;
    $options += whos_amung_us_default_form_values();
    $title = isset($this->title) ? $this->title : NULL;
    return array(
      'delta' => $this->delta,
      'title' => $title,
      'subject' => check_plain($title),
      'content' => theme(
        'whos_amung_us',
        array(
          'widget' => $options['widget'],
          'options' => $options,
          'id' => 'box' . rand(1, 999),
        )
      ),
    );
  }

}
