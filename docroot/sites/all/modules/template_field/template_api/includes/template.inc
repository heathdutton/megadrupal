<?php

/**
 * @file
 * The data object for the template
 */

class Template {
  // Values of the table
  public $template_id;
  public $name;
  public $label;
  public $renderer;
  public $inputs;
  public $content;
  public $attached;
  public $tags;
  public $description;
  public $owner;
  public $owner_type;

  // Ctools fields
  public $export_type;
  public $disabled;
  public $type;
  public $table;

  public function form($input_values) {
    return template_api_get_template_controller($this)->form($input_values);
  }

  public function validate($form, $form_state) {
    return template_api_get_template_controller($this)->validate($form, $form_state);
  }

  public function getExtraAdminFields($input_type, $input) {
    return template_api_get_input_type_controller($input_type)->getExtraAdminFields($input);
  }

  public function getFlattenedInputs() {
    return self::flatten_input_array($this->inputs);
  }

  public function setUp($definition) {
    foreach ($definition as $name => $value) {
      switch ($name) {
        case 'content':
          if ($file_content = $this->getAttachedContent($value)) {
            $value = $file_content;
          }
          break;
        case 'attached':
          $value['js'] = isset($value['js']) ? $value['js'] : '';
          $value['css'] = isset($value['css']) ? $value['css'] : '';
          break;
      }

      $this->$name = $value;
    }
  }

  protected function getAttachedContent($content) {
    if ($file_content = TemplateApiTemplateController::getFileContents($content)) {
      return $file_content;
    }
    return $content;
  }

  /**
   * Flattens the nested inputs array into a single level
   * for use in the template configuration screen.
   *
   * @static
   * @param $input_values
   * The array of fields on this template.
   */
  static function flatten_input_array($input_values, $return = array()) {
    foreach ($input_values as $key => $value) {
      if (isset($input_values[$key]['children']) && is_array($input_values[$key]['children']) && !empty($input_values[$key]['children'])) {
        unset($value['children']);
        $return[$key] = $value;
        $return = self::flatten_input_array($input_values[$key]['children'], $return);
      }
      else {
        if (isset($input_values[$key])) {
          $return[$key] = $value;
        }
      }
    }
    return $return;
  }
}

class TemplateApiTemplateController {
  // An instance of the Template Base class
  protected $template;
  protected $template_key;
  protected $template_variables;

  /**
   * @param Template $template
   * @param array $section_data
   */
  public function __construct($template_key) {
    if ($template_key instanceof Template) {
      $this->template = $template_key;
      $this->template_key = $template_key->name;
    }
    elseif (empty($template_key)) {
      ctools_include('export');
      $this->template = ctools_export_crud_new('template');
    }
    else {
      ctools_include('export');
      $this->template = ctools_export_crud_load('template', $template_key);
      if (empty($this->template)) {
        throw new Exception("Template $template_key could not be loaded");
      }
    }
  }

  /**
   * Load a file
   */
  public static function getFileContents($path) {
    $path = drupal_realpath($path);
    if (file_exists($path)) {
      return file_get_contents($path);
    }
    return '';
  }

  /**
   * Get the template
   */
  public function getTemplate() {
    return $this->template;
  }

  /**
   * Render the template
   *
   * @param array
   *  section id's to the content to render.
   *
   * @return array
   */
  public function view($data) {
    $template_variables = $this->getVariables($data);
    return template_api_get_renderer_controller($this->template->renderer)
      ->render($this->template, $template_variables);
  }

  /**
   * Get the variables for this template without rendering it
   */
  public function getVariables($input_values) {
    $template_variables = $input_values;

    foreach($this->template->inputs as $key => $input) {
      $input_value = empty($input_values[$key]) ? array() : $input_values[$key];
      $template_variables[$key] = template_api_get_input_type_controller($input['type'])
        ->getVariable($this->getInputDefinition($key), $input_value, $input_value);
    }

    global $conf, $base_url, $user;

    // Add in some globals
    $template_variables['globals'] = array(
      'variables' => $conf,
      'base_path' => base_path(),
      'base_url' => $base_url,
      'user' => $user,
    );

    // allow other modules to modify the variables, passing the template as context
    drupal_alter('template_api_variables', $template_variables, $this->template);

    return $template_variables;
  }


  /**
   * Get the input form for the template
   */
  public function getForm($input_values) {
    $form = array();
    foreach ($this->template->inputs as $key => $input) {
      $input_value = array_key_exists($key, $input_values) ? (empty($input_values[$key]) ? array() : $input_values[$key]) : array();
      $input_def = $this->getInputDefinition($key);
      $form[$key] = template_api_get_input_type_controller($input['type'])->getForm($input_def, $input_value, $this->template, $key);
    }

    return $form;
  }

  /**
   * Get the input form for the template
   */
  public function validate($input_values, &$form_state) {
    foreach ($this->template->inputs as $key => $input) {
      $input_value = empty($input_values[$key]) ? array() : $input_values[$key];
      template_api_get_input_type_controller($input['type'])
        ->validateForm($this->getInputDefinition($key), $input_value, $form_state);
    }
  }

  /**
   * Get the input definition
   */
  protected function getInputDefinition($input_key) {
    return $this->template->inputs[$input_key];
  }

  /**
   * Is the template empty?
   */
  public function isEmpty($input_values) {
    foreach ($this->template->inputs as $key => $input) {
      $input_value = empty($input_values[$key]) ? array() : $input_values[$key];
      if (template_api_get_input_type_controller($input['type'])->isEmpty($this->getInputDefinition($key), $input_value) === FALSE) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * The Content attached to the template is being updated
   */
  public function update($input_values) {
    foreach ($this->template->inputs as $key => $input) {
      $input_value = empty($input_values[$key]) ? array() : $input_values[$key];
      template_api_get_input_type_controller($input['type'])->update($this->getInputDefinition($key), $input_value);
    }
    return $this;
  }

  /**
   * The Content attached to the template is being inserted
   */
  public function insert($input_values) {
    foreach ($this->template->inputs as $key => $input) {
      $input_value = empty($input_values[$key]) ? array() : $input_values[$key];
      template_api_get_input_type_controller($input['type'])->insert($this->getInputDefinition($key), $input_value);
    }
    return $this;
  }

  /**
   * The Content attached to the template is being deleted
   */
  public function delete($input_values) {
    foreach ($this->template->inputs as $key => $input) {
      $input_value = empty($input_values[$key]) ? array() : $input_values[$key];
      template_api_get_input_type_controller($input['type'])->delete($this->getInputDefinition($key), $input_value);
    }
    return $this;
  }

  /**
   * Override get
   */
  public function __get($name) {
    return $this->template->$name;
  }

  /**
   * Override set
   */
  public function __set($name, $value) {
    $this->template->$name = $value;
  }
}
