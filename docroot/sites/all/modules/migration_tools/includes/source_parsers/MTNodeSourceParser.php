<?php

/**
 * @file
 * Includes MTNodeSourceParser class, parses static HTML files via queryPath.
 */

/**
 * Class MTNodeSourceParser.
 *
 * @package migration_tools
 */
class MTNodeSourceParser extends MTSourceParser {
  protected $body;
  protected $title;
  // @codingStandardsIgnoreStart
  protected $content_type;
  // @codingStandardsIgnoreEnd

  /**
   * Getter.
   */
  public function getTitle() {
    $title = $this->getProperty('title');
    if (empty($title)) {
      MigrationMessage::makeMessage("The title for @fileid is empty.", array("@fileid" => $this->fileId), WATCHDOG_ALERT);
    }
    return $title;
  }

  /**
   * Getter.
   */
  public function getBody() {
    $this->cleanHtml();
    $body = $this->getProperty('body');
    if (empty($body)) {
      MigrationMessage::makeMessage("The body for @fileid is empty.", array("@fileid" => $this->fileId), WATCHDOG_ALERT);
    }
    return $body;
  }

  /**
   * Getter.
   */
  public function getContentType() {
    return $this->getProperty('content_type');
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
    $type = new ObtainerInfo("content_type");
    $this->addObtainerInfo($type);

    $title = new ObtainerInfo("title");
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("title", 1));
    $this->addObtainerInfo($title);

    $body = new ObtainerInfo("body");
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');
    $this->addObtainerInfo($body);
  }

}
