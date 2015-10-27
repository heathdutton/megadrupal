<?php

class TemplateApiBeanInputType extends TemplateApiInputType {
  /**
   * The Admin form
   *
   * Any extra fields to add to the admin form for the template
   *
   * @param Template
   * @param $input array
   * @return array
   */
  public function extraAdminFields($input) {
    $form = array();

    $options = array();
    foreach (bean_get_types() as $type) {
      if (!empty($type)) {
        $options[$type->type] = $type->getLabel();
      }
    }
    $form['bean_types'] = array(
      '#type' => 'select',
      '#title' => t('Bean Types'),
      '#options' => $options,
      '#multiple' => TRUE,
      '#empty_option' => t('-- Select --'),
      '#default_value' => isset($input['bean_types']) ? $input['bean_types'] : '',
    );

    return $form;
  }

  /**
   * Build the form
   *
   * @param $input_definition array
   * @param $input_values array
   * @return array
   *  Form API compliant array
   */
  public function form($input_definition, $input_values) {
    $form = array();

    if (!empty($input_definition['bean_types'])) {
      template_api_set_types($input_definition['bean_types']);
    }

    $form['bean'] = array(
      '#type' => 'textfield',
      '#title' => $input_definition['label'],
      '#autocomplete_path' => 'template_api/autocomplete/bean',
      '#default_value' => isset($input_values['bean']) ? $input_values['bean'] : '',
      '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
    );

    return $form;
  }

  public function getVariable($input_definition, $input_values) {
    if (!empty($input_values['bean'])) {
      $bean = bean_load($input_values['bean']);
      return drupal_render($bean->view());
    }

    return '';
  }

  public function isEmpty($input_definition, $input_values) {
    return empty($input_values['bean']);
  }
}

function template_api_set_types($types) {
  $_SESSION['template_api_bean_types'] = $types;
}

function template_api_get_types() {
  return isset($_SESSION['template_api_bean_types']) ? $_SESSION['template_api_bean_types'] : array();
}

function template_api_autocomplete_bean($string = '') {
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'bean');

  if ($bean_types = template_api_get_types()) {
    $query->entityCondition('bundle', $bean_types);
  }

  $query->propertyCondition('label', '%' . db_like($string) . '%', 'LIKE');
  $query->range(0, 10);

  $result = $query->execute();

  $beans = bean_load_multiple(array_keys($result['bean']));

  $matches = array();
  foreach ($beans as $bean) {
    $matches[$bean->internalIdentifier()] = $bean->label();
  }

  drupal_json_output($matches);
}
