<?php
/**
 * @file
 * Defines HtmlToPressReleaseMigration class.
 */

/**
 * Class HtmlToPressReleaseMigration.
 *
 * Parent class for all migrations from static HTML to press release
 * content type.
 *
 * @package migration_tools
 */
abstract class HtmlToPressReleaseMigration extends HtmlToNodeMigration {

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
      'field_pr_body' => t('Body'),
    );

    if (empty($arguments['source_parser_class'])) {
      $arguments['source_parser_class'] = "MTPressReleaseSourceParser";
    }

    if (array_key_exists('component_tid', $arguments)) {
      $component_tid = $arguments['component_tid'];
    }
    else {
      throw new Exception(sprintf("%s: HtmlToPressReleaseMigration requires the component_tid argument.", $arguments['machine_name']));
    }

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('press_release');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_pr_body', 'body');
    $this->addFieldMapping('field_pr_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_pr_number', 'field_pr_number');
    $this->addFieldMapping("field_pr_component")->defaultValue(array($component_tid));
    $this->addFieldMapping('field_pr_component:source_type')->defaultValue("tid");
    $this->addFieldMapping('field_pr_subtitle', 'field_pr_subtitle');
    $this->addFieldMapping('field_pr_date', 'field_pr_date');
  }

  /**
   * {@inheritdoc}
   */
  protected function setProperties($row) {
    $row->content_type = $this->sourceParser->getContentType();
    $row->title = $this->sourceParser->getTitle();
    $row->field_pr_subtitle = $this->sourceParser->getSubTitle();
    $row->field_pr_date = $this->sourceParser->getDate();
    if (empty($row->field_pr_date)) {
      $this->MigrationMessage("The press release @file_id has no date.", array("@file_id" => $row->fileid), WATCHDOG_ERROR);
    }

    // Validate that the date is not in the future.
    $date = $row->field_pr_date;
    $time = strtotime($date);
    if ($time > time()) {
      $this->MigrationMessage("The press release date for @file_id was in the future.", array("@file_id" => $row->fileid), WATCHDOG_ALERT);
    }

    $row->field_pr_number = $this->sourceParser->getPrNumber();
    $row->body = $this->sourceParser->getBody();
  }
}
