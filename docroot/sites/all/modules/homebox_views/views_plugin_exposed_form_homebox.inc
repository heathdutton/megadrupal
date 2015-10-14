<?php

/**
 * @file
 * Definition of views_plugin_exposed_form_homebox.
 */

/**
 * The base plugin to handles homebox specific exposed filter forms.
 */
class views_plugin_exposed_form_homebox extends views_plugin_exposed_form_basic {

  function option_definition() {
    $options = parent::option_definition();
    $options['homebox_filters'] = array();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = &drupal_static('homebox_filter_fields');

    if (!empty($options)) {
      $form['homebox_filters'] = array(
        '#title' => 'Homebox-enabled filters',
        '#type' => 'checkboxes',
        '#options' => $options,
        '#description' => "These filters will show up in a homebox block configuration, and be saved to the user's homebox settings. They will not show up as normal view filters.",
        '#default_value' => $this->options['homebox_filters'] ? $this->options['homebox_filters'] : array(),
      );
    }
  }
}
