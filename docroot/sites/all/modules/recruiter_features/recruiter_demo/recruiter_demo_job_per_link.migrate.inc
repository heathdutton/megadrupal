<?php
/**
 * @file
 * Jobs per link migration class for Recruiter Demo.
 */

/**
 * Class for importing job per link.
 */
class RecruiterJobPerLinkMigration extends RecruiterJobMigration {

  public function __construct() {
    $this->path = drupal_get_path('module', 'recruiter_demo') . '/import/job_per_link.csv';
    $this->type = 'job_per_link';
    parent::__construct();
    $this->addTermFieldMapping('field_job_link', 'link');
  }

}
