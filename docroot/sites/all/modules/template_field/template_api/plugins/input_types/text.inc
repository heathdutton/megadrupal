<?php

class TemplateApiTextInputType extends TemplateApiInputType {
  public function form($input_definition, $input_values) {
    $form = array();

    $form['textarea'] = array(
      '#type' => 'textarea',
      '#title' => $input_definition['label'],
      '#default_value' => isset($input_values['textarea']) ? $input_values['textarea'] : '',
      '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
    );

    return $form;
  }

  public function getVariable($input_definition, $input_values) {
    return $input_values['textarea'];
  }

  public function isEmpty($input_definition, $input_values) {
    return empty($input_values['textarea']);
  }
}
