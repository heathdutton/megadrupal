<?php
/**
 * @file
 * Defines HtmlToSpeechMigration class.
 */

/**
 * Class HtmlToSpeechMigration.
 *
 * Parent class for all migrations from static HTML to speech content type.
 *
 * @package migration_tools
 */
abstract class HtmlToSpeechMigration extends HtmlToNodeMigration {

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
   * @param object $parser
   *   Parser object (optional)
   */
  public function __construct($arguments, $source_dirs, $options = array(), $regex = NULL, MigrateContentParser $parser = NULL) {
    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_pr_body' => t('Body'),
      'field_pr_date' => t('Date'),
      'field_speech_location' => t('Location'),
      'field_speech_speaker' => t('Speaker'),
      'field_pr_teaser' => t('Teaser'),
      'field_speech_image' => t('Image'),
      'field_pr_component' => t('Component'),
      'field_pr_topic' => t('Topic'),
      'field_speech_attachment' => t('Attachments'),
    );

    if (empty($arguments['source_parser_class'])) {
      $arguments['source_parser_class'] = "NGSpeechSourceParser";
    }

    $component_tid = (!empty($arguments['component_tid'])) ? $arguments['component_tid'] : '';

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options, $parser);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('speech');
    $this->description = t('Migrates Speeches from Html sources.');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_pr_body', 'body');
    $this->addFieldMapping('field_pr_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_pr_component', 'field_speech_component')
        ->defaultValue(array($component_tid));
    $this->addFieldMapping('field_pr_component:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_pr_date', 'field_speech_date');
    $this->addFieldMapping('field_speech_location', 'country')->defaultValue('US');
    $this->addFieldMapping('field_speech_location:locality', 'city');
    $this->addFieldMapping('field_speech_location:administrative_area', 'state');
    $this->addFieldMapping('field_speech_image', 'field_speech_image');
    $this->addFieldMapping('field_pr_topic', 'field_speech_topic');
    $this->addFieldMapping('field_speech_speaker', 'speaker');
    $this->addFieldMapping('field_speech_attachment', 'attachment_filename');
    $this->addFieldMapping('field_speech_attachment:source_type')
      ->defaultValue('filename');
    $this->addFieldMapping('field_speech_attachment:file_class')
      ->defaultValue('MigrateFileUri');
    $this->addFieldMapping('field_speech_attachment:destination_dir', 'destination_dir');
    $this->addFieldMapping('field_speech_attachment:source_dir', 'source_dir');
    $this->addFieldMapping('field_speech_attachment:file_replace')
      ->defaultValue(FILE_EXISTS_REUSE);
  }

  /**
   * {@inheritdoc}
   */
  protected function setProperties($row) {
    $row->content_type = $this->sourceParser->getContentType();
    // The attachmentRedirects property is an array of legacy URLs for every
    // file that is added as an attachment to this row. This property is
    // used in complete() to create redirects to the new file location.
    $row->attachmentRedirects = array();
    $row->title = $this->sourceParser->getTitle();
    $row->field_speech_date = $this->sourceParser->getDate();
    if (empty($row->field_speech_date)) {
      $this->MigrationMessage("The speech @file_id has no date.", array("@file_id" => $row->fileid), WATCHDOG_ERROR);
    }

    // Validate that the date is not in the future.
    $date = $row->field_speech_date;
    $time = strtotime($date);
    if ($time > time()) {
      $this->MigrationMessage("The speeech date for @file_id was in the future.", array("@file_id" => $row->fileid), WATCHDOG_ALERT);
    }

    $this->sourceParser->getLocation();
    // If these were already provided by getLocation, it will use them,
    // otherwise it will run the individual obtainers.
    $row->city = $this->sourceParser->getCity();
    $row->state = $this->sourceParser->getState();
    $row->country = $this->sourceParser->getCountry();

    // Body always has to come last, because it gets what is left behind.
    $row->body = $this->sourceParser->getBody();
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    parent::complete($entity, $row);

    if (module_exists('redirect') && !empty($row->attachmentRedirects)) {
      $this->createAttachmentRedirect($entity, $row->attachmentRedirects, 'field_speech_attachment');
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRollback($entity_id) {
    $entities = entity_load('node', $entity_id);
    $entity = reset($entities);

    $this->rollbackAttachmentRedirect($entity, 'field_speech_attachment');
  }
}
