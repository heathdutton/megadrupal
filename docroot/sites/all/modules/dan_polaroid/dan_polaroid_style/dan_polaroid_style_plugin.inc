<?php

class dan_polaroid_style_plugin extends views_plugin_style {

  function options(&$options) {
    $options['height'] = array('default' => '300px');
  }

  function options_form(&$form, &$form_state) {

    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#size' => '30',
      '#description' => t('This field determines how tall the timeline will be'),
      '#default_value' => $this->options['height'],
    );
  }

}
