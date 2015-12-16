<?php

class GenMforms extends GenComponent {

  public $mformsPath;
  public $stepsFile;
  public $stepsFilePath;
  public $pagesFilePath;
  public $storeKey;
  public $formSteps;
  public $storeClass;
  public $controlsClass;

  function __construct($module_name, $module_path, $params = array()) {
    parent::__construct($module_name, $module_path, $params);

    $this->storeKey = isset($params['store_key']) ? $params['store_key'] :
        $this->prompt(dt('Provide store key.'), NULL, TRUE, 'validateStoreKey');

    $this->formSteps = isset($params['form_steps']) ? $params['form_steps'] :
        $this->prompt(dt('Provide form step callbacks separated by a comma (i.e. _myform_step1,_myform_step2).'), NULL, TRUE, 'validateFormSteps');
    $this->formSteps = genova_get_options_array($this->formSteps, '1D');

    $this->storeClass = isset($params['store_class']) ? $params['store_class'] :
        $this->prompt(dt('Provide store class (i.e. MformsFormStateStore).'), NULL, TRUE, 'validateStoreClass');

    $this->controlsClass = isset($params['controls_class']) ? $params['controls_class'] :
        $this->prompt(dt('Provide controls class (i.e. MformsSimpleControls)'), NULL, TRUE, 'validateControlsClass');

    // Init file names and paths.
    $this->mformsPath = $this->modulePath . '/mforms';
    $this->stepsFile = $this->moduleName . '.' . $this->storeKey . '.inc';
    $this->stepsFilePath = $this->mformsPath . '/' . $this->stepsFile;
    $this->pagesFilePath = $this->modulePath . '/' . $this->moduleName . '.pages.inc';

    if (!file_exists($this->mformsPath)) {
      mkdir($this->mformsPath);
    }
  }

  function validateInputs() {
    $this->validateStoreKey($this->storeKey);
    $this->validateFormSteps($this->formSteps);
    $this->validateStoreClass($this->storeClass);
    $this->validateControlsClass($this->controlsClass);
  }

  function buildComponent() {
    $page_callback = $this->moduleName . '_' . $this->storeKey . '_page';
    $form_callback = $this->moduleName . '_' . $this->storeKey . '_form';

    // Create page callback for new mform.
    genova_drush_component_create($this->moduleName, 'menu');
    genova_inject_code_snippet($this->pagesFilePath, $page_callback,
      "  \$params = array();\n\n  return drupal_get_form('$form_callback', \$params);",
      " *(return).*", NULL, NULL, TRUE
    );

    // Generate basic form lifecycle callbacks.
    $mform_data = array(
      'name' => $this->moduleName,
      'store_key' => $this->storeKey,
      'store_class' => $this->storeClass,
      'controls_class' => $this->controlsClass,
      'first_step' => $this->formSteps[0],
      'steps_btns_definition' => '',
    );
    genova_put_code_snippet($this->pagesFilePath, $this->getCodeSnippet('mforms_build', $mform_data));
    genova_put_code_snippet($this->pagesFilePath, $this->getCodeSnippet('mforms_validate', $mform_data));
    genova_put_code_snippet($this->pagesFilePath, $this->getCodeSnippet('mforms_submit', $mform_data));

    // Generate form steps.
    genova_put_code_snippet($this->stepsFilePath, $this->getCodeSnippet('php', array(), 'general'));

    for ($i = 0; count($this->formSteps) > $i; $i++) {

      $mform_data['step'] = $this->formSteps[$i];
      $mform_data['weight'] = 110 - $i;
      $mform_data['next_step'] = (isset($this->formSteps[$i + 1]) ? "'{$this->formSteps[$i + 1]}'" : 'NULL');
      $mform_data['steps_btns_definition'] .= $this->getCodeSnippet('mforms_steps_btns_definition', $mform_data);

      genova_put_code_snippet($this->stepsFilePath, $this->getCodeSnippet('mforms_step_build', $mform_data));
      genova_put_code_snippet($this->stepsFilePath, $this->getCodeSnippet('mforms_step_validate', $mform_data));
      genova_put_code_snippet($this->stepsFilePath, $this->getCodeSnippet('mforms_step_submit', $mform_data));

    }

    // Generate mforms init hook.
    if ($this->controlsClass == 'MformsMultistepControls') {
      $tpl = 'mforms_multistep_init';
    }
    else {
      $tpl = 'mforms_init';
    }
    genova_put_code_snippet(genova_get_output_file_path($this->moduleName, 'module'),
      $this->getCodeSnippet($tpl, $mform_data));
  }

  /**
   * Gets component name.
   *
   * @return string
   */
  public function componentName() {
    return 'mforms';
  }

  public function genovaPath() {
    return __DIR__ . '/';
  }

  function validateControlsClass($controls_class) {
    if (!class_exists($controls_class)) {
      throw new GenException(dt('Provided controls class does not exists.'));
    }

    $interfaces = class_implements($controls_class, TRUE);

    if (!in_array('MformsIcontrols', $interfaces)) {
      throw new GenException(dt('Provided controls class does not implement MformsIcontrols.'));
    }
  }

  function validateStoreClass($store_class) {
    if (!class_exists($store_class)) {
      throw new GenException(dt('Provided store class does not exists.'));
    }

    $interfaces = class_implements($store_class, TRUE);

    if (!in_array('MformsIstore', $interfaces)) {
      throw new GenException(dt('Provided store class does not implement MformsIstore.'));
    }
  }

  function validateStoreKey($store_key) {
    if (!genova_is_valid_func_name($store_key)) {
      throw new GenException(dt('Store key string must be a valid PHP function name.'));
    }
  }

  function validateFormSteps($form_steps) {

    if (!is_array($form_steps)) {
      $form_steps = genova_get_options_array($form_steps, '1D');
    }

    foreach ($form_steps as $form_step) {
      if (!genova_is_valid_func_name($form_step)) {
        throw new GenException(dt('Invalid form step name provided for @step', array('@step' => $form_step)));
      }
    }
  }
}
