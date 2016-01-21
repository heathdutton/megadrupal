<?php

/**
 * @file
 * Includes SourceParser class, which parses static HTML files via queryPath.
 */

/**
 * Class MTSimpleSourceParser.
 *
 * @package migration_tools
 */
class MTSimpleSourceParser extends MTSourceParser {

  /**
   * {@inheritdoc}
   */
  public function __construct($file_id, $html) {
    parent::__construct($file_id, $html);

    $this->cleanHtml();
  }

  /**
   * Set the html var after some cleaning.
   */
  protected function cleanHtml() {
    try {
      $this->initQueryPath();
      HtmlCleanUp::convertRelativeSrcsToAbsolute($this->queryPath, $this->fileId);

      // Clean up specific to this site.
      HtmlCleanUp::stripOrFixLegacyElements($this->queryPath);
    }
    catch (Exception $e) {
      MigrationMessage::makeMessage('@file_id Failed to clean the html, Exception: @error_message', array('@file_id' => $this->fileId, '@error_message' => $e->getMessage()), WATCHDOG_ERROR);
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
  }

}
