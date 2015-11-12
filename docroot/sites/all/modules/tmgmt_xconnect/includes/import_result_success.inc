<?php
/**
 * @file
 * Success result.
 */

/**
 * Success result.
 */
class TmgmtXConnectImportResultSuccess extends TmgmtXConnectImportResultAbstract {
  /**
   * The Job for who the import was performed.
   *
   * @var TMGMTJob
   */
  private $job;

  /**
   * Construct a result.
   *
   * @param string $file_name
   *   The processed file name.
   * @param TMGMTJob $job
   *   The job for who the translation is imported.
   * @param string $message
   *   The result message.
   * @param array $message_arguments
   *   The arguments for the message.
   */
  public function __construct(
    $file_name,
    TMGMTJob $job,
    $message = NULL,
    $message_arguments = array()
  ) {
    $this->setFileName($file_name);
    $this->setJob($job);
    $this->setMessage($message);
    $this->setMessageArguments($message_arguments);
  }

  /**
   * Get the job that holds the imported translation.
   *
   * @return TMGMTJob
   *   The processed job.
   */
  public function getJob() {
    return $this->job;
  }

  /**
   * Set the Job for who the import was processed.
   *
   * @param TMGMTJob $job
   *   The processed job.
   */
  protected function setJob(TMGMTJob $job) {
    $this->job = $job;
  }

}
