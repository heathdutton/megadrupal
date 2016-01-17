<?php

class TemplateApiHiddenInputType extends TemplateApiInputType {
  public function form($input_definition, $input_values) {
    $form = array();

    $form['hidden'] = array(
      '#type' => 'hidden',
      '#value' => isset($input_definition['value']) ? $input_definition['value'] : '',
      '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
    );

    return $form;
  }

  public function getVariable($input_definition, $input_values) {
    return $input_values['hidden'];
  }

  public function isEmpty($input_definition, $input_values) {
    // Intentionally left blank.
  }
}
