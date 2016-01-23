<?php

class TemplateApiTextFormatInputType extends TemplateApiInputType {
  public function form($input_definition, $input_values) {
    $form = array();

    if (empty($input_values['textarea']['format'])) {
      $format = empty($input_definition['format']) ? filter_default_format() : $input_definition['format'];
    }
    else {
      $format = $input_values['textarea']['format'];
    }

    $form['textarea'] = array(
      '#type' => 'text_format',
      '#title' => $input_definition['label'],
      '#default_value' => isset($input_values['textarea']['value']) ? $input_values['textarea']['value'] : '',
      '#format' => $format,
      '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
    );

    return $form;
  }

  public function getVariable($input_definition, $input_values) {
    return check_markup($input_values['textarea']['value'], $input_values['textarea']['format']);
  }

  public function isEmpty($input_definition, $input_values) {
    return empty($input_values['textarea']['value']);
  }
}
