<?php
/**
 * @file
 * Defines HtmlToCaseMigration class.
 */

/**
 * Class HtmlToCaseMigration.
 *
 * Parent class for all migrations from static HTML to case content type.
 *
 * @package migration_tools
 */
abstract class HtmlToCaseMigration extends HtmlToNodeMigration {

  /**
   * Define $this->source, $this->map, $this->destination, and simple mappings.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   * @param array $source_dirs
   *   A flat array of the source directories containing html files.
   * @param array $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   * @param string $regex
   *   Regular expression to guide what is processed by the migration.
   */
  public function __construct($arguments, $source_dirs, $options = array(), $regex = NULL) {
    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_case_body' => t('Body'),
    );

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('case');
    $this->description = t('Migrates Cases from Html sources.');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_case_body', 'body');
    $this->addFieldMapping('field_case_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_case_overview', 'field_case_overview');
    $this->addFieldMapping('field_case_date', 'field_case_date');
    $this->addFieldMapping('field_case_link', 'field_case_link');
    if (array_key_exists('component_tid', $arguments)) {
      $this->addFieldMapping("field_case_component")->defaultValue(array($arguments['component_tid']));
      $this->addFieldMapping('field_case_component:source_type')->defaultValue("tid");
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function setProperties($row) {
    $row->content_type = $this->sourceParser->getContentType();
    $row->title = $this->sourceParser->getTitle();

    // Additional Custom Node fields.
    $row->field_case_date = $this->sourceParser->getDate();
    $row->field_case_link = $this->sourceParser->getLink();
    $row->field_case_overview = $this->sourceParser->getOverview();

    // Body always has to come last, because it gets what is left behind.
    $row->body = $this->sourceParser->getBody();
  }
}
