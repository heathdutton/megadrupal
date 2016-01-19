<?php

/**
 * @file
 * Definition of views_plugin_display_homebox.
 */

/**
 * The base display plugin to handles homebox specific block rendering to separate the exposed filters as homebox settings.
 *
 * We do not modify the exposed filter form here, because the values then not automatically exportable by features.
 */
class views_plugin_display_homebox extends views_plugin_display_block {

  function options_form(&$form, &$form_state) {

    // Get the exposed field information and store it in a static variable, that the exposed form settings can get this information
    $options = &drupal_static('homebox_filter_fields');
    $options = array();
    $filters = $this->get_handlers('filter');
    foreach ($filters as $key => $filter) {
      $options[$key] = $filter->definition['title'];
    }

    // When the static options are stet, generate the options form
    parent::options_form($form, $form_state, $options);
  }
}
