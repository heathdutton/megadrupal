<?php

/**
 * @file
 * Beans plugin for the Whos.amung.us module.
 *
 * @author Francisco J. Cruz Romanos <grisendo@gmail.com>
 */

class WhosAmungUsBean extends BeanPlugin {

  /**
   * Declares default block settings.
   */
  public function values() {
    return array_merge(parent::values(), whos_amung_us_default_form_values());
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    module_load_include('inc', 'whos_amung_us', 'includes/whos_amung_us.admin');
    $options = array(
      'values' => (array) $bean,
    );
    $options['values'] += whos_amung_us_default_form_values();
    return whos_amung_us_configuration_form(array(), $options, 'beans');
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    $options = (array) $bean;
    $options += whos_amung_us_default_form_values();
    return theme(
      'whos_amung_us',
      array(
        'widget' => $options['widget'],
        'options' => $options,
        'id' => 'bns' . rand(1, 999),
      )
    );
  }

}
