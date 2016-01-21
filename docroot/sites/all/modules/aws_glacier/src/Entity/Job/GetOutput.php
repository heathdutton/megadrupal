<?php

namespace Drupal\aws_glacier\Entity\Job;

use Guzzle\Http\EntityBody;
use Drupal\aws_glacier\Entity\Archive\Archive;

/**
 * Class GetOutput
 * @package Drupal\aws_glacier\Entity\Job
 */
class GetOutput extends Command {

  /**
   * @var string
   */
  protected $filename;

  /**
   * @param string $filename
   * @return $this
   */
  public function setFilename($filename) {
    $directory = drupal_dirname($filename);
    if (file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      $this->filename = $filename;
    }
    return $this;
  }

  /**
   * @return string
   */
  public function getFilename() {
    return $this->filename;
  }

  /**
   * @var object
   * File entity
   */
  protected $file_entity;

  /**
   * @return object
   */
  public function getFileEntity() {
    return $this->file_entity;
  }

  /**
   * {@inheritDoc}
   * @return $this
   */
  function __construct() {
    parent::__construct('GetJobOutput');
    return $this;
  }

  /**
   * {@inheritDoc}
   * @return $this
   */
  public function setJob(Job $Job) {
    $this->Job = $Job;
    $this->setArgs(array(
      'vaultName' => $Job->vaultName,
      'jobId' => $Job->jobId,
    ));
    return $this;
  }

  /**
   * @param \Drupal\aws_glacier\Entity\Archive\Archive $archive
   *
   * @return @return $this
   */
  public function setArchive(Archive $archive) {
    $this->filename = $archive->file_uri;
    return $this;
  }

  /**
   * {@inheritDoc}
   * @return $this
   */
  public function run() {
    $existing_files = file_load_multiple(array(), array('uri' => $this->filename));
    if (count($existing_files)) {
      $existing = reset($existing_files);
      $this->file_entity = file_load($existing->fid);
    }
    else{
      parent::run();
      $this->saveAsFile();
    }
    return $this;
  }

  /**
   * Saves the job output as a file.
   *
   * @return $this
   */
  protected function saveAsFile() {
    if (!empty($this->filename)) {
      $data = $this->getData();
      if (!empty($data['body']) && ($body = $data['body']) && $body instanceof EntityBody) {
        $this->file_entity = file_save_data($body->__toString(), $this->filename, FILE_EXISTS_REPLACE);
      }
    }
    return $this;
  }
}
