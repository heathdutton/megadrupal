<?php
/**
 * @file
 * Defines JsonToDatasetMigration class.
 */

/**
 * Class JsonToDatasetMigration.
 *
 * Parent class for dataset migrations with source data stored in a JSON feed.
 *
 * @package migration_tools
 */
class JsonToDatasetMigration extends OpenDataJSONtoDatasetMigration {

  /**
   * Prepare the JSON source.
   *
   * This will setup $this->source.
   * It will also add field mappings.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   */
  public function __construct($arguments) {
    // Use a specific source but allow for possible overides in extention.
    $arguments['json_source_url'] = empty($arguments['json_source_url']) ? 'http://www.somesite.com/data.json' : $arguments['json_source_url'];

    parent::__construct($arguments);

    // Document the source fields.
    $source_fields = array(
      'title'              => 'The title of the dataset.',
      'description'        => 'The description of the dataset.',
      'keyword'            => 'The keywords used to define the dataset',
      'modified'           => 'The date modified in format yyyy-mm-dd.',
      'publisher'          => 'The name of the agency publishing the dataset.',
      'contactPoint'       => 'The name of the person to contact.',
      'mbox'               => 'The email address of the contact point.',
      'identifier'         => 'Identifier (duplicate of title)',
      'accessLevel'        => 'The access level of the dataset',
      'accessLevelComment' => 'A comment related to the access level.',
      'bureauCode'         => 'The bureau code for the dataset.',
      'programCode'        => 'The program code for the dataset.',
      'license'            => 'The license under which the dataset is offered.',
      'landingPage'        => 'The human facing page for the dataset.',
      'language'           => 'The language code of the dataset.',
    );

    // $this->source was already built, but the structure of this JSON is non-
    // conformant so we must use a different source.
    unset($this->source);
    $this->source = new MigrateSourceJSON($arguments['json_source_url'], 'identifier', $source_fields);

    // Map non-conformant source fields.
    $this->addFieldMapping('field_opendata_rights', 'accessLevelComment', FALSE);
    $this->addFieldMapping('field_opendata_person', 'contactPoint', FALSE);
    $this->addFieldMapping('field_opendata_mbox', 'mbox', FALSE);

    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');
    $this->addFieldMapping('workbench_moderation_state_current')->defaultValue('published');

    $this->addFieldMapping('og_group_ref')
    ->defaultValue('open')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    // Intentionally not running parent::prepareRow.
    $this->processKeywords($row);
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    migration_tools_prepare_workbench_settings($entity, $row);
  }
}
