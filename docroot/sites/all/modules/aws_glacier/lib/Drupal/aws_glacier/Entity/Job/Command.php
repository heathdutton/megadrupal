<?php

namespace Drupal\aws_glacier\Entity\Job;

use Drupal\aws_glacier\Command as CommandWrapper;

/**
 * Class Command
 * @package Drupal\aws_glacier\Entity\Job
 */
class Command extends CommandWrapper {

  /**
   * @var \Drupal\aws_glacier\Entity\Job\Job
   */
  protected $Job;

  /**
   * Sets some required values for the command.
   *
   * @param \Drupal\aws_glacier\Entity\Job\Job $Job
   */
  public function setJob(Job $Job) {
    $this->Job = $Job;
    $this->setArgs(array(
      'vaultName' => $Job->vaultName,
      'Format' => $Job->Format,
      'Type' => $Job->Type,
      'Description' => $Job->Description,
      'jobId' => $Job->jobId,
    ));
    if ($Job->Type == 'archive-retrieval') {
      $this->setArgs(array('ArchiveId' => $Job->ArchiveId));
    }
    return $this;
  }
}
