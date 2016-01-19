<?php

class TemplateApiColorInputType extends TemplateApiInputType {
  public function form($input_definition, $input_values) {
    $form = array();

    if (module_exists('jquery_colorpicker')) {
      $form['hex'] = array(
        '#type' => 'jquery_colorpicker',
        '#title' => $input_definition['label'],
        '#default_value' => isset($input_values['hex']) ? $input_values['hex'] : '',
        '#size' => 8,
        '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
      );
    }
    else {
      $form['hex'] = array(
        '#type' => 'textfield',
        '#title' => $input_definition['label'],
        '#default_value' => isset($input_values['hex']) ? $input_values['hex'] : '',
        '#size' => 8,
        '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
      );
    }

    return $form;
  }

  public function getVariable($input_definition, $input_values) {
    return new ColorMustacheView(array('hex' => $input_values['hex']));
  }

  public function isEmpty($input_definition, $input_values) {
    return empty($input_values['hex']);
  }
}

class ColorMustacheView extends TemplateMustacheView {
  public function hex() {
    return '#' . $this->data['hex'];
  }

  public function rgb() {
    $r = $this->r();
    $g = $this->g();
    $b = $this->b();

    return "rgb($r, $g, $b)";
  }

  public function r() {
    return base_convert(substr($this->data['hex'], 0, 2), 16, 10);
  }

  public function g() {
    return base_convert(substr($this->data['hex'], 2, 2), 16, 10);
  }

  public function b() {
    return base_convert(substr($this->data['hex'], 4, 2), 16, 10);
  }

}
