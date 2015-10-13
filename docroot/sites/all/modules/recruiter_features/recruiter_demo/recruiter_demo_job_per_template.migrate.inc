<?php
/**
 * @file
 * Jobs per template migration class for Recruiter Demo.
 */

/**
 * Class for importing job per template.
 */
class RecruiterJobPerTemplateMigration extends RecruiterJobMigration {

  public function __construct() {
    $this->path = drupal_get_path('module', 'recruiter_demo') . '/import/job_per_template.csv';
    $this->type = 'job_per_template';
    parent::__construct();
  }

}
