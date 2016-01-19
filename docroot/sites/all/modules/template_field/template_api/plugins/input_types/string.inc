<?php

class TemplateApiStringInputType extends TemplateApiInputType {
  public function form($input_definition, $input_values) {
    $form = array();

    $form['textfield'] = array(
      '#type' => 'textfield',
      '#title' => $input_definition['label'],
      '#default_value' => isset($input_values['textfield']) ? $input_values['textfield'] : '',
      '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
    );

    return $form;
  }

  public function getVariable($input_definition, $input_values) {
    return $input_values['textfield'];
  }

  public function isEmpty($input_definition, $input_values) {
    return empty($input_values['textfield']);
  }
}
