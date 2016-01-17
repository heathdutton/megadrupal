<?php
/**
 * @file
 * Jobs migration class for Recruiter Demo.
 */

/**
 * Class for importing basic pages.
 */
abstract class RecruiterJobMigration extends RecruiterMigration {

  /**
   * The csv file path.
   */
  var $path;

  /**
   * The job content type (eg.: job_per_template).
   */
  var $type;

  public function __construct() {
//    $this->sourceFields['level'] = t('Term level, extracted in prepareRow()');

    parent::__construct();
    $this->description = t('Import job nodes of type ' . $this->type);
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'dummy_id' => array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
          ),
        ),
        MigrateDestinationNode::getKeySchema()
    );

    $csvcolumns = array();
    $options = array('header_rows' => 1);
    $this->source = new MigrateSourceCSV($this->path, $csvcolumns, $options);

    $this->destination = new MigrateDestinationNode($this->type, array('text_format' => 'full_html'));

    // Add field mappings common for all job types.
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'description');
    $this->addFieldMapping('field_job_workflow_state', 'workflow_state');
    $this->addFieldMapping('field_job_organization', 'organization');
    $this->addFieldMapping('field_job_location', 'location');

    $this->addTermFieldMapping('field_job_region', 'region');
    $this->addTermFieldMapping('field_job_occupational_field', 'occupational_fields');
    $this->addTermFieldMapping('field_job_of_study', 'fields_of_study');
    $this->addTermFieldMapping('field_job_employment_type_term', 'employment_type');
    $this->addTermFieldMapping('field_job_experience_term', 'experience');
    $this->addTermFieldMapping('field_job_languages', 'languages');

    $this->addFieldMapping('field_job_email', 'application_email');

    $this->addTermFieldMapping('field_job_skills_it', 'it_skills');
    $this->addTermFieldMapping('field_job_skills_general', 'general_skills');

    $this->addFieldMapping('field_job_salary', 'salary');
    $this->addTermFieldMapping('field_job_degree_level', 'degree_level');
  }

  function addTermFieldMapping($destination_field, $source_field = NULL, $warn_on_override = TRUE) {
    $this->addFieldMapping($destination_field, $source_field, $warn_on_override)->separator(',');
  }

}
