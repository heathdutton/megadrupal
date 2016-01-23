<?php

/**
 * @file
 * input_type.inc
 *
 * The interface and abstract class for input type plugin
 */

/**
 * The interface for the input type plugin
 */
interface TemplateApiInputTypeInterface {
  /**
   * The Admin form
   *
   * Any extra fields to add to the admin form for the template
   *
   * @param Template
   * @param $input
   *  the input definiton
   * @return array
   */
  public function extraAdminFields($input);

  /**
   * Build the form
   *
   * @param Template
   * @return array a Form API compliant array
   */
  public function form($input_definition, $input_values);

  /**
   * Validate function for input type
   *
   * @param array
   *  the element array
   * @param array
   *  the form_state array
   */
  public function validate($input_definition, $input_values, &$form_state);

  /**
   * Build the variable array
   */
  public function getVariable($input_definition, $input_values);

  /**
   * Is the value really empty?
   *
   * @return boolean
   */
  public function isEmpty($input_definition, $input_values);
}

/**
 * The abstract class for input types
 */
abstract class TemplateApiInputType extends TemplateApiPlugin implements TemplateApiInputTypeInterface {
  /**
   * The Admin form
   *
   * Any extra fields to add to the admin form for the template
   *
   * @param Template
   * @param $input_key
   * @return array
   */
  public function extraAdminFields($input) {
    return array();
  }

  /**
   * Validate function for input type
   *
   * @param array $input_values
   * @param array
   *  the form_state array
   */
  public function validate($input_definition, $input_values, &$form_state) {}
}

/**
 * The controller for the renderer
 */
class TemplateApiInputTypeController extends TemplateApiPluginController {

  /**
   * The plugin type
   */
  protected function pluginType() {
    return 'input_type';
  }

  /**
   * Get extra admin fields
   */
  public function getExtraAdminFields($input) {
    if (!empty($this->plugin)) {
      return $this->plugin->extraAdminFields($input);
    }
    return array();
  }

  /**
   * Build the variable array
   */
  public function getVariable($input_definition, $input_values) {
    return $this->callHook('getVariable', $input_definition, $input_values);
  }

  /**
   * Validate
   */
  public function validateForm($input_definition, $input_values, &$form_state) {
    $this->plugin->validate($input_definition, $input_values, $form_state);
  }

  /**
   * Is empty
   */
  public function isEmpty($input_definition, $input_values) {
    return $this->callHook('isEmpty', $input_definition, $input_values);
  }

  public function getForm($input_definition, $input_values, Template $template, $input_key) {
    $form = $this->callHook('form', $input_definition, $input_values);
    if (isset($input_definition['required']) && !empty($input_definition['required'])) {
        $this->addRequired($form, $input_definition['required']);
    }
    $this->addStuffToElement($form, $input_definition, $template, $input_key);
    return $form;
  }

  protected function addStuffToElement(&$element, $input_definition, Template $template, $input_key) {
    foreach (element_children($element) as $element_key) {
      $element_id = $template->name . '-' . $input_key . '-' . $element_key;
      $attributes = array('class' => array($element_id));
      if (isset($element[$element_key]['#attributes'])) {
        array_merge_recursive($element[$element_key]['#attributes'], $attributes);
      }
      else {
        $element[$element_key]['#attributes'] = $attributes;
      }

      if (empty($element[$element_key]['#prefix'])) {
        $element[$element_key]['#prefix'] = '';
      }

      if (empty($element[$element_key]['#suffix'])) {
        $element[$element_key]['#suffix'] = '';
      }

      $element[$element_key]['#prefix'] .= '<div class="template-input ' . $element_id . '">';
      $element[$element_key]['#suffix'] = '</div>' . $element[$element_key]['#suffix'];
    }
  }

  protected function callHook($method, $input_definition, $input_values) {
    if (method_exists($this->plugin, $method)) {
      return $this->plugin->$method($input_definition, $input_values);
    }

    return FALSE;
  }

  /**
   * An update to the template has happened
   */
  public function update($input_definition, $input_values) {
    return $this->callHook('update', $input_definition, $input_values);
  }

  /**
   * A new template is being inserted
   */
  public function insert($input_definition, $input_values) {
    return $this->callHook('insert', $input_definition, $input_values);
  }

  /**
   * A template is being deleted
   */
  public function delete($input_definition, $input_values) {
    return $this->callHook('delete', $input_definition, $input_values);
  }

  /**
   * Add the required elements
   */
  protected function addRequired(&$form, $required_keys) {
    foreach (element_children($form) as $key) {
      if ((is_array($required_keys) && isset($required_keys[$key]))
        || (!is_array($required_keys) && $required_keys)) {

        $form[$key]['#required'] = TRUE;
      }

    }
  }
}
