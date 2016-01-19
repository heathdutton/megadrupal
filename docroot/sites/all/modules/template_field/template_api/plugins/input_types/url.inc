<?php

class TemplateApiURLInputType extends TemplateApiInputType {
  public function form($input_definition, $input_values) {
    $form = array();

    $form['url'] = array(
      '#type' => 'textfield',
      '#title' => $input_definition['label'],
      '#default_value' => isset($input_values['url']) ? $input_values['url'] : '',
      '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
    );

    $form['options'] = array(
      '#tree' => TRUE
    );

    $form['options']['absolute'] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($input_values['options']['absolute']) ? $input_values['options']['absolute'] : FALSE,
      '#title' => t('Absolute'),
    );

    return $form;
  }

  public function getVariable($input_definition, $input_values) {
    $url_parts = drupal_parse_url($input_values['url']);
    $options = (isset($input_values['options']) && is_array($input_values['options'])) ? $input_values['options'] : array() ;
    if (isset($url_parts['query']) && count($url_parts['query']) > 0) {
      $options['query'] = $url_parts['query'];
    }
    if (isset($url_parts['fragment']) && !empty($url_parts['fragment'])) {
      $options['fragment'] = $url_parts['fragment'];
    }
    return (isset($url_parts['path']) && !empty($url_parts['path'])) ? url($url_parts['path'], $options) : '';
  }

  public function isEmpty($input_definition, $input_values) {
    return empty($input_values['url']);
  }
}
