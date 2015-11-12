<?php
/**
 * @file
 * Contains argument default plugin for getting current MLID items.
 */

/**
 * Default argument plugin to provide current mlid item.
 */
class vms_plugin_argument_default_mlid extends views_plugin_argument_default {
  function option_definition() {
    $options = parent::option_definition();
    $options['trail'] = array('default' => 'current');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['trail'] = array(
      '#type' => 'select',
      '#title' => t('Menu item(s) to return'),
      '#default_value' => $this->options['trail'],
      '#options' => vms_mlid_query_types(),
    );

  }

  function get_argument() {
    return $this->options['trail'];
  }
}
