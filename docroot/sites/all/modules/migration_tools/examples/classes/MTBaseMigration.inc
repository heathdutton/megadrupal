<?php

/**
 * @file
 * Contains base migration class for all migrations.
 */

/**
 * The default system path for the MT legacy webroot. Can be overriden via
 * migration_tools_base_dir variable.
 */
define('MT_LEGACY_WEBROOT', DRUPAL_ROOT . '/../../legacy/www.somesite.com');

/**
 * Abstract intermediate class holding common settings.
 *
 * @package migration_tools
 */
abstract class MTBaseMigration extends Migration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->mergeArguments($arguments);

  }

  /**
   * Add multiple field mappings at once.
   *
   * @param array $mappings
   *   An array of field mappings in the form of source_key => dest_key.
   *
   * @param bool $warn_on_override
   *   Set to FALSE to prevent warnings when there's an existing mapping.
   */
  public function addFieldMappings(array $mappings, $warn_on_override = TRUE) {
    foreach ($mappings as $source => $destination) {
      $this->addFieldMapping($source, $destination, $warn_on_override);
    }
  }

  /**
   * Sets the field mapping for an organic group parent using uuid.
   *
   * @param string $uuid
   *   The unique identifier for the Organic Group.
   *
   * @return string
   *   The entity id of the Organic Group.
   */
  public function addGroupReferenceMapping($uuid) {
    // Set the Organic Group to which this content belongs. Get the data
    // here so that it isn't fetched per $row in prepare().
    list($og_eid) = array_values(entity_get_id_by_uuid('node', array($uuid)));
    $this->addFieldMapping('og_group_ref')->defaultValue($og_eid);

    return $og_eid;
  }

  /**
   * Arguements in keyed array passed among migration classes and parsers.
   *
   * @var array $arguments
   */
  protected $arguments = array();
  /**
   * Basic cetter for $arguments.
   *
   * @return array
   *   Whatever has been stored in $this->arguments.
   */
  public function getArguments() {
    return $this->arguments;
  }
  /**
   * Merges an array into the existing arguments array.
   *
   * @param array $new_args
   *   An array matching the format of the arguments array, to be merged.
   *
   * @return array
   *   Array of the complete arguments array with the new material.
   */
  protected function mergeArguments($new_args = '') {
    if (!empty($new_args) && is_array($new_args)) {
      $this->arguments = array_merge($this->arguments, $new_args);
    }
  }

  /**
   * Gets a single argument from the arguments array.
   *
   * @param string $arg_key
   *   The key of the item to return from the Arguments array.
   *
   * @return mixed
   *   Whatever is stored in the $keys's value, or NULL if not in the arguments.
   */
  protected function getArgument($arg_key = '') {
    if (!empty($arg_key)) {
      $args = $this->getArguments();
      if (array_key_exists($arg_key, $args)) {
        return $args[$arg_key];
      }
    }
    return NULL;
  }


  /**
   * Unsets NULL properties on a single dimensional object.
   *
   * @param obj $row
   *   The object to iterate over.
   */
  public function removeEmptyProperties(&$row) {
    foreach ($row as $key => $property) {
      if (is_null($property)) {
        unset($row->$key);
      }
      elseif (is_string($property) && !$property) {
        unset($row->$key);
      }
    }
  }

  /**
   * Adds path and path auto field mapping to 'Do not migrate' issue group.
   */
  public function addPathDnmMapping() {
    // Map path and pathauto fields to NULL, assigning to DNM.
    if (module_exists('path')) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (module_exists('pathauto')) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Prepare title property by trimming to 255 chars and fixing encoding.
    if (!empty($row->title)) {
      $row->title = mb_strimwidth($row->title, 0, 255, "...");
      $row->title = html_entity_decode($row->title, ENT_QUOTES, 'UTF-8');
    }
  }

  /**
   * Alters language to LANGUAGE_NONE if used in prepare().
   *
   * This is to correct issues with redirects and aliases having languages.
   *
   * @param object $entity
   *   Fully loaded entity.
   */
  public function languageSideStep($entity) {
    // Check language, if other than LANGUAGE_NONE, sidestep it.
    if ($entity->language !== LANGUAGE_NONE) {
      // Save the original langage for processing in the complete().
      $entity->original_language = $entity->language;
      $entity->language = LANGUAGE_NONE;
    }
  }

  /**
   * Alters language on node back to originally specified if used in complete().
   *
   * This is to correct issues with redirects and aliases having languages.
   *
   * @param object $entity
   *   Fully loaded entity.
   */
  public function languageReturnStep($entity) {
    // Check to see if language was sidestepped in prepare().
    if (!empty($entity->original_language)) {
      // Language was sidestepped, so put it back and resave.
      $entity->language = $entity->original_language;
      unset($entity->original_language);
      entity_save('node', $entity);
    }
  }


  /**
   * Extracts metadata from pdf file.
   *
   * @param string $pdf_file_path
   *   The absolute file path of the pdf on the local system.
   *
   * @return array
   *   An associative array of pdf metadata.
   */
  public function getPdfMetadata($pdf_file_path) {
    try {
      $pdfinfo_bin = variable_get('pdfinfo_binary', '/home/mt/xpdf/bin64/pdfinfo');
      $file_arg = escapeshellarg($pdf_file_path);
      $command = "$pdfinfo_bin $file_arg";
      $output = shell_exec($command);
      $pdf_info_rows = explode("\n", $output);
      $pdf_metadata = array();
      foreach ($pdf_info_rows as $row) {
        $columns = explode(':', $row);
        $pdf_metadata[$columns[0]] = $columns[1];
      }

      return $pdf_metadata;
    }
    catch (Exception $e) {
      $message = t('Could not parse attached PDF for @fileid. Exception thrown: @exception', array('@fileid' => $pdf_file_path, '@exception' => $e->getMessage()));
      $this->queueMessage($message);
      watchdog('migration_tools', $message);
    }
  }

  /**
   * Extracts the contents of a pdf file.
   *
   * @param string $pdf_file_path
   *   The absolute file path of the pdf on the local system.
   *
   * @return string
   *   The text content of the pdf.
   *
   * @throws \XPDF\Exception\BinaryNotFoundException
   */
  public function getPdfContents($pdf_file_path) {
    try {
      // Get PDF contents.
      $pdftotext_bin = variable_get('pdftotext_binary', '/home/mt/xpdf/bin64/pdftotext');
      $pdf_parser = XPDF\PdfToText::create(array('pdftotext.binaries' => $pdftotext_bin));
      $pdf_contents = $pdf_parser->getText($pdf_file_path);

      return $pdf_contents;
    }
    catch (Exception $e) {
      $message = t('Could not parse attached PDF for @fileid. Exception thrown: @exception', array('@fileid' => $pdf_file_path, '@exception' => $e->getMessage()));
      $this->queueMessage($message);
      watchdog('migration_tools', $message);
    }
  }

  /**
   * Creates redirects for files attached to a given entity's field field.
   *
   * @param object $entity
   *   The fully loaded entity.
   *
   * @param array $source_urls
   *   A flat array of source urls that should redirect to the attachments
   *   on this entity. $source_urls[0] will redirect to the first attachment,
   *   $entity->$field_name[$language][0], and so on.
   *
   * @param string $field_name
   *   The machine name of the attachment field.
   *
   * @param string $language
   *   Optional. Defaults to LANGUAGE_NONE.
   */
  public static function createAttachmentRedirect($entity, $source_urls, $field_name, $language = LANGUAGE_NONE) {
    if (empty($source_urls)) {
      // Nothing to be done here.
      $json_entity = json_encode($entity);
      watchdog("migration_tools", "redirect was not created for attachment in entity {$json_entity}");
      return;
    }

    $field = $entity->$field_name;
    if (!empty($field[$language])) {
      foreach ($field[$language] as $delta => $item) {
        // $file = file_load($item['fid']);
        // $url = file_create_url($file->uri);
        // $parsed_url = parse_url($url);
        // $destination = ltrim($parsed_url['path'], '/');
        $source = $source_urls[$delta];

        // Create redirect.
        $redirect = redirect_load_by_source($source);
        if (!$redirect) {
          $redirect = new stdClass();
          redirect_object_prepare($redirect);
          $redirect->source = $source;
          $redirect->redirect = "file/{$item['fid']}/download";
          redirect_save($redirect);
        }
      }
    }
  }

  /**
   * Deletes any redirects associated files attached to an entity's file field.
   *
   * @param object $entity
   *   The fully loaded entity.
   *
   * @param string $field_name
   *   The machine name of the attachment field.
   *
   * @param string $language
   *   Optional. Defaults to LANGUAGE_NONE.
   */
  public static function rollbackAttachmentRedirect($entity, $field_name, $language = LANGUAGE_NONE) {
    $field = $entity->$field_name;
    if (!empty($field[$language])) {
      foreach ($field[$language] as $delta => $item) {
        $file = file_load($item['fid']);
        $url = file_create_url($file->uri);
        $parsed_url = parse_url($url);
        $destination = ltrim($parsed_url['path'], '/');
        redirect_delete_by_path($destination);
      }
    }
  }

  /**
   * Logs a system message.
   *
   * @param string $message
   *   The message to store in the log. Keep $message translatable
   *   by not concatenating dynamic values into it! Variables in the
   *   message should be added by using placeholder strings alongside
   *   the variables argument to declare the value of the placeholders.
   *   See t() for documentation on how $message and $variables interact.
   * @param array $variables
   *   Array of variables to replace in the message on display or
   *   NULL if message is already translated or not possible to
   *   translate.
   * @param int $severity
   *   The severity of the message; one of the following values as defined in
   *   - WATCHDOG_EMERGENCY: Emergency, system is unusable.
   *   - WATCHDOG_ALERT: Alert, action must be taken immediately.
   *   - WATCHDOG_CRITICAL: Critical conditions.
   *   - WATCHDOG_ERROR: Error conditions.
   *   - WATCHDOG_WARNING: Warning conditions.
   *   - WATCHDOG_NOTICE: (default) Normal but significant conditions.
   *   - WATCHDOG_INFO: Informational messages.
   *   - WATCHDOG_DEBUG: Debug-level messages.
   *
   * @param int $indent
   *   (optional). Sets indentation for drush output. Defaults to 1.
   *
   * @link http://www.faqs.org/rfcs/rfc3164.html RFC 3164: @endlink
   */
  protected function MigrationMessage($message, $variables = array(), $severity = WATCHDOG_NOTICE, $indent = 1) {
    $type = get_class($this);
    watchdog($type, $message, $variables, $severity);

    if (drupal_is_cli() && variable_get('migration_tools_drush_debug', FALSE)) {
      $formatted_message = format_string($message, $variables);
      drush_print($formatted_message, $indent);
      if ((variable_get('migration_tools_drush_stop_on_error', FALSE)) && ($severity <= WATCHDOG_ERROR)) {
        throw new MigrateException("$type: Stopped for debug.\n -- Run \"drush mi {migration being run}\" to try again. \n -- Run \"drush vset migration_tools_drush_stop_on_error FALSE\" to disable auto-stop.");
      }
    }
  }
}
