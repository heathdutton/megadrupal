<?php

class GenSDBO extends GenComponent {

  public $tableName;
  public $fields;
  public $primaryKey;
  public $schema;

  public $sdboFilePath;

  public function drushInit($module_name, $module_path, $params = array()) {
    parent::init($module_name, $module_path, $params);

    $this->sdboFilePath = genova_get_output_file_path($this->moduleName, 'sdbo.inc');

    $this->tableName = !empty($params['table_name']) ? $params['table_name'] :
      $this->drushPrompt(dt('Enter table name over which SDBO will operate'), NULL, TRUE, 'validateTableName');

    $this->fields = !empty($params['fields']) ? $params['fields'] :
      $this->drushPrompt(dt('Enter comma delimited persisted attribute names (i.e. id,name,body)'), NULL, TRUE, 'validateFields');
    $this->fields = genova_get_options_array($this->fields, '1D');

    $this->primaryKey = !empty($params['primary_key']) ? $params['primary_key'] :
      $this->drushPrompt(dt('Enter primary key'), 'id', TRUE, 'validatePrimaryKey');

    $this->schema = !empty($params['schema']) ? $params['schema'] :
      $this->drushPrompt(dt('Do you wish to generate schema definition? (yes/no)'), 'yes');
  }

  /**
   * Implements logic to validate all inputs needed to build a component.
   *
   * @throws GenException
   *   On input error.
   */
  public function validateInputs() {
    $this->validateFields($this->fields);
    $this->validatePrimaryKey($this->primaryKey);
  }

  /**
   * Implements logic to build component.
   */
  public function buildComponent() {

    // SDBO file does not exist yet, create it.
    if (!file_exists($this->sdboFilePath)) {
      genova_put_code_snippet($this->sdboFilePath, $this->getCodeSnippet('php', array(), 'general'));
      genova_put_code_snippet(genova_get_output_file_path($this->moduleName, 'info'), "\nfiles[] = {$this->moduleName}.sdbo.inc");
    }

    $attributes = '';

    foreach ($this->fields as $field) {
      $attributes .= $this->getCodeSnippet('sdb_attribute', array('name' => $field)) . "\n";
    }

    $data = array(
      'table' => $this->tableName,
      'class' => genova_camelize($this->tableName, TRUE),
      'attributes' => $attributes,
      'primary_key' => $this->primaryKey,
    );

    // Get new SDBO class code snippet and write it into file.
    genova_put_code_snippet($this->sdboFilePath, $this->getCodeSnippet('SDBObject', $data));

    // Generate schema for newly created SDBO.
    if ($this->schema == 'yes') {
      $schema_params = array(
        'table_name' => $this->tableName,
        'primary_key' => $this->primaryKey,
        'fields' => $this->fields,
      );
      genova_drush_component_create($this->moduleName, 'schema', $schema_params);
    }
  }

  /**
   * Gets component name.
   *
   * @return string
   */
  public function componentName() {
    return 'SDBO';
  }

  public function genovaPath() {
    return __DIR__ . '/';
  }

  public function validateFields($fields) {
    if (!is_array($fields)) {
      $fields = genova_get_options_array($fields, '1D');
    }

    foreach ($fields as $field) {
      if (!genova_is_valid_func_name($field)) {
        throw new GenException(dt('Invalid attribute name "@name" provided.', array('@name' => $field)));
      }
    }
  }

  public function validatePrimaryKey($primary_key) {
    if (!in_array($primary_key, $this->fields)) {
      throw new GenException(dt('Attribute for provided primary key @name is not defined.', array('@name' => $primary_key)));
    }
  }

  public function validateTableName($table_name) {
    if (!genova_is_valid_func_name($table_name)) {
      throw new GenException(dt('Invalid table name provided.'));
    }
  }
}
