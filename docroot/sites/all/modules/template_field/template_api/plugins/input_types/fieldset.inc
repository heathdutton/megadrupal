<?php

class TemplateApiFieldsetInputType extends TemplateApiInputType {
  public function form($input_definition, $input_values) {
    $form = array();

    $form['fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => $input_definition['label'],
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
    );

    foreach ($input_definition['children'] as $key => $input) {
      $values = isset($input_values['fieldset'][$key]) ? $input_values['fieldset'][$key] : array();
      $form['fieldset'][$key] = template_api_get_input_type_controller($input['type'])
        ->getForm($input, $values);
    }

    return $form;
  }

  public function getVariable($input_definition, $input_values) {
    $output = array();

    foreach ($input_values['fieldset'] as $key => $value) {
      $input_type = $input_definition['children'][$key]['type'];
      $input_def = $input_definition['children'][$key];
      foreach ($value as $type => $input_value) {
        $output[$key] = template_api_get_input_type_controller($input_type)->getVariable($input_def, $value);
      }
    }

    return $output;
  }

  public function isEmpty($input_definition, $input_values) {
    foreach ($input_values['fieldset'] as $key => $value) {
      if (is_array($value)) {
        $input_type = $input_definition['children'][$key]['type'];
        $input_def = $input_definition['children'][$key];
        foreach ($value as $type => $input_value) {
          if (!template_api_get_input_type_controller($input_type)->isEmpty($input_def, $value)) {
            return FALSE;
          }
        }
      }
    }

    return TRUE;
  }
}
