<?php

class TemplateApiSelectInputType extends TemplateApiInputType {
  /**
   * The Admin form
   *
   * Any extra fields to add to the admin form for the template
   *
   * @param Template
   * @param $input_key the key that i
   * @return array
   */
  public function extraAdminFields($input) {
    $form = array();

    $form['options'] = array(
      '#title' => t('Options for select'),
      '#type' => 'textarea',
      '#description' => t('A comma separated list of options for the drop down'),
      '#default_value' => isset($input['options']) ? $input['options'] : '',
    );

    return $form;
  }

  /**
   * Build the form
   *
   * @param Template
   * @return array a Form API compliant array
   */
  public function form($input_definition, $input_values) {
    $form = array();
    $options = $input_definition['options'];
    if (is_string($options)) {
      $options = explode(',', $options);
    }
    $form['select'] = array(
      '#type' => 'select',
      '#title' => $input_definition['label'],
      '#options' => drupal_map_assoc($options),
      '#empty_option' => t('-- Select --'),
      '#default_value' => isset($input_values['select']) ? $input_values['select'] : '',
      '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
    );

    return $form;
  }

  public function getVariable($input_definition, $input_values) {
    return $input_values['select'];
  }

  public function isEmpty($input_definition, $input_values) {
    return empty($input_values['select']);
  }
}
