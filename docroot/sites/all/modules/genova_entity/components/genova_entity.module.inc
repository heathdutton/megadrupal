<?php

/**
 * Generates Drupal entity module with requested components.
 */
class GenEntityModule extends GenComponent {

  public $humanName;
  public $description;
  public $className;
  public $baseURI;
  //public $dependencies;
  //public $components = array();

  /**
   * Init.
   *
   * @param string $module_name
   *   Name of the module where the component is going to be generated.
   * @param string $module_path
   *   Path to the module where the component is going to be generated.
   * @param array $params
   *   Init parameters passed into constructor.
   */
  public function drushInit($module_name, $module_path, $params = array()) {
    parent::init($module_name, $module_path, $params);

    //$this->components = drush_get_option_list('components');

    $human_name = ucwords(str_replace('_', ' ', $module_name));
    $this->humanName = !empty($params['human_readable_name']) ? $params['human_readable_name'] :
        $this->drushPrompt('Enter human readable module name', $human_name);

    $this->description = !empty($params['description']) ? $params['description'] :
        $this->drushPrompt('Enter module description', $this->humanName . ' custom entity', FALSE);

    $class_name = str_replace(' ', '', $this->humanName);
    $this->className = !empty($params['class_name']) ? $params['class_name'] :
        $this->drushPrompt('Enter entity class name', $class_name);

    $this->baseURI = !empty($params['base_uri']) ? $params['base_uri'] :
        $this->drushPrompt('Enter base entity menu path', str_replace('_', '/', $module_name));

//    $this->dependencies = !empty($params['dependencies']) ? $params['dependencies'] :
//        $this->drushPrompt('Enter comma separated dependencies', NULL, FALSE, 'validateDependencies');
//    $this->dependencies = genova_get_options_array($this->dependencies, '1D');
  }

  /**
   * Implements logic to validate all inputs needed to build a component.
   *
   * @throws GenException
   *   On input error.
   */
  public function validateInputs() {

    if (!genova_is_valid_func_name($this->moduleName)) {
      throw new GenException(strtr('Invalid module name provided "@module". Make sure it complies with Drupal coding standards.',
        array('@module' => $this->moduleName)));
    }

//    $this->validateDependencies($this->dependencies);
  }

  /**
   * Validates provided dependencies.
   *
   * @param mixed $dependencies
   * @throws GenException
   */
//  public function validateDependencies($dependencies) {
//
//    if (empty($dependencies)) {
//      return;
//    }
//
//    if (!is_array($dependencies)) {
//      $dependencies = genova_get_options_array($dependencies, '1D');
//    }
//
//    foreach ($dependencies as $dependency) {
//      if (!genova_is_valid_func_name($dependency)) {
//        throw new GenException(strtr('Provided dependency @name is not a valid module name.', array('@name' => $dependency)));
//      }
//    }
//  }

  /**
   * Implements logic to build component.
   */
  public function buildComponent() {
    $this->buildModule();
//    $this->buildComponents();
  }

  /**
   * Prints success message.
   */
  public function onSuccess() {
    genova_get_messenger()->msg(strtr('Module @module created at @dir.',
      array('@module' => $this->moduleName, '@dir' => $this->modulePath)));
  }

  /**
   * Gets component name.
   *
   * @return string
   */
  public function componentName() {
    return 'entity module';
  }

  /**
   * We are trying first to load templates from genova_entity and then from
   * genova.
   */
  public function getCodeSnippet($name, $data = array(), $type = NULL) {
    $genova_entity_path = drupal_get_path('module', 'genova_entity') . '/';

    if (!empty($type)) {
      $tpl_path = 'templates/' . $type . '/' . $name . '.txt';
    }
    else {
      $tpl_path = 'templates/' . $name . '.txt';
    }

    // If we have our custom genova_entity template use it.
    if (file_exists($genova_entity_path . $tpl_path)) {
      $contents = file_get_contents($genova_entity_path . $tpl_path);
      foreach ($data as $key => $value) {
        $contents = str_replace('{' . $key . '}', $value, $contents);
      }
    }
    // If not use genova default implementation for building code snippets.
    else {
      $contents = parent::getCodeSnippet($name, $data, $type);
    }

    return $contents;
  }

  /**
   * Builds basic module structure.
   *
   * @throws GenException
   */
  private function buildModule() {
    $data = array(
      'name' => $this->moduleName,
      'human_name' => $this->humanName,
      'human_name_low_case' => $this->humanNameLowCase,
      // @todo - This is probably to lame and need to be defined in input ;)
      'plural_human_name' => $this->humanName . 's',
      'description' => $this->description,
      'class_name' => $this->className,
      'base_uri' => $this->baseURI,
    );

    // Remove description line from snippet if none was provided.
    $info_file_snippet = $this->getCodeSnippet('info', $data);
    if (empty($data['description'])) {
      $info_file_snippet = genova_remove_line('description =', $info_file_snippet);
    }

    // Add dependencies.
//    foreach ($this->dependencies as $dependency) {
//      $info_file_snippet .= "dependencies[] = $dependency\n";
//    }

    // Basic module files.
    $module_path = $this->modulePath . '/' . $this->moduleName;
    genova_put_code_snippet($module_path . '.info', $info_file_snippet);
    genova_put_code_snippet($module_path . '.install', $this->getCodeSnippet('install', $data));
    genova_put_code_snippet($module_path . '.module', $this->getCodeSnippet('module', $data));

    // Views files.
    genova_put_code_snippet($this->modulePath . '/views/' . $this->moduleName . '.views.inc', $this->getCodeSnippet('views', $data, 'views'));
    genova_put_code_snippet($this->modulePath . '/views/' . $this->moduleName . '.view.inc', $this->getCodeSnippet('view', $data, 'views'));
    genova_put_code_snippet($this->modulePath . '/views/handlers/' . $this->moduleName . '_handler_field_operations.inc', $this->getCodeSnippet('handler_field_operations', $data, 'views/handlers'));

    // Various includes files for entity support.
    $includes_path = $this->modulePath . '/includes/' . $this->moduleName;
    genova_put_code_snippet($includes_path . '.controller.inc', $this->getCodeSnippet('controller', $data, 'includes'));
    genova_put_code_snippet($includes_path . '.entity.inc', $this->getCodeSnippet('entity', $data, 'includes'));
    genova_put_code_snippet($includes_path . '.info.inc', $this->getCodeSnippet('info', $data, 'includes'));
    genova_put_code_snippet($includes_path . '.pages.inc', $this->getCodeSnippet('pages', $data, 'includes'));
    genova_put_code_snippet($includes_path . '.ui_controller.inc', $this->getCodeSnippet('ui_controller', $data, 'includes'));
  }

  /**
   * Builds requested components.
   */
//  private function buildComponents() {
//    foreach ($this->components as $component) {
//      genova_component_create($this->moduleName, $component);
//    }
//  }

}
