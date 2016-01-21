<?php

/**
 * Creates vocabs in code.
 *
 * @code
 * $vocab = CINC::init('TaxonomyVocabulary');
 * @endcode
 */
class CINC_TaxonomyVocabulary extends CINC implements CINC_CRUD {

  public $config_machine_name = 'machine_name';
  public $cinc_yaml_prefix = 'taxonomy.vocabulary';

  /**
   * Constructor
   */
  function __construct() {

    $this->config_filters['machine_name'] = 'set_machine_name';

    // Set defaults.

    $this->config = array(
      'machine_name' => '',
      'name' => '',
      'description' => '',
      'hierachy' => '',
      'module' => 'taxonomy',
      'weight' => 0,
      'pathauto' => '',
    );

    $this->original_config = $this->config;

    return $this;

  }

  /**
   * Auto-adds name when type is set, and cleans type.
   */
  public function set_machine_name($type) {
    if (empty($this->config['name'])) {
      $this->set('name', CINC::display_name_from_machine_name($type));
    }
    return CINC::clean_machine_name($type);
  }

  /**
   * Saves vocab to the database.
   */
  public function create() {
    taxonomy_vocabulary_save((object) $this->config);
    $this->set_variables();
    return $this;
  }

  /**
   * Tests if vocab exists.
   */
  public function exists() {
    $taxonomy = taxonomy_vocabulary_machine_name_load($this->get('machine_name'));
    if ($taxonomy) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Loads vocab from the database.
   */
  public function read() {
    $taxonomy = taxonomy_vocabulary_machine_name_load($this->get('machine_name'));
    if ($taxonomy) {
      $this->config = (array) $taxonomy;
    }
    $this->get_variables();
    return $this;
  }

  /**
   * Saves vocab to the database.
   */
  public function update() {
    taxonomy_taxonomy_vocabulary_update((object) $this->config);
    $this->set_variables();
    return $this;
  }

  /**
   * Removes vocab from the database.
   */
  public function delete() {
    $taxonomy_vocabulary = taxonomy_vocabulary_machine_name_load($this->config[$this->config_machine_name]);
    taxonomy_vocabulary_delete($taxonomy_vocabulary->vid);
    return $this;
  }

  /**
   * Get variables
   */
  public function get_variables() {
    if (module_exists('pathauto')) {
      $this->set('pathauto', variable_get('pathauto_taxonomy_term_' . $this->get('machine_name') . '_pattern', $this->get('pathauto')));
    }
  }

  /**
   * Set variables
   */
  public function set_variables() {
    if (module_exists('pathauto')) {
      variable_set('pathauto_taxonomy_term_' . $this->get('machine_name') . '_pattern', $this->get('pathauto'));
    }
  }

  /**
   * Add a field to this taxonomy.
   */
  public function add_field($field, $weight = NULL) {
    if (!($field instanceof CINC_Field)) {
      if ($field instanceof CINC_MissingDependencies) {
        $dependencies = implode(', ', $field->missing_dependencies);
        drupal_set_message(t('CINC_TaxonomyVocabulary::add_field tried to add field @field_name but is missing dependencies: @dependencies', array('@field_name' => $field->config['field_name'], '@dependencies' => $dependencies)), 'warning');
        watchdog('CINC', format_string('CINC_TaxonomyVocabulary::add_field tried to add field @field_name but is missing dependencies: @dependencies', array('@field_name' => $field->config['field_name'], '@dependencies' => $dependencies)), array(), WATCHDOG_WARNING);
      }
      else {
        throw new Exception('CINC_TaxonomyVocabulary::add_field requires a CINC_Field instance as the first parameter');
      }
    }
    else {
      // Use the CINC_Field Class method to save the field to this vocabulary.
      $field->create_instance_for('taxonomy_term', $this->config['machine_name'], $weight);
    }

    // Add this field to the fields array property.
    if ($is_saved) {
      array_push($field, $this->fields);
    }

    return $this;
  }

  /**
   * Adds field only if it isn't already added.
   */
  public function add_field_if_missing($field, $weight = NULL) {
    $field->instance->set('entity_type', 'taxonomy_term')->set('bundle', $this->get('machine_name'));
    if (!$field->instance->exists()) {
      $this->add_field($field, $weight);
    }
    return $this;
  }

}
