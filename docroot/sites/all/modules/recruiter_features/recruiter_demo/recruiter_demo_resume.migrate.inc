<?php
/**
 * @file
 * Resume migration class for Recruiter Demo.
 */

/**
 * Base class for importing resume field collection items.
 */
abstract class RecruiterResumeFieldCollectionMigration extends RecruiterMigration {

  public function __construct() {
    parent::__construct();
    $this->dependencies = array('RecruiterUser');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'user' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationFieldCollection::getKeySchema()
    );

    // Map the resume profile ID in the prepareRow() method.
    $this->addFieldMapping('host_entity_id', 'pid');
  }

  public function prepareRow($row) {
    $row->pid = recruiter_demo_load_pid_by_user_name($row->user);
    return parent::prepareRow($row);
  }

}

/**
 * Class for importing field collection items from the personal info field collection.
 */
class RecruiterResumePersonalMigration extends RecruiterResumeFieldCollectionMigration {

  public function __construct() {
    parent::__construct();
    $this->description = t("Import resume's personal info field collection items.");

    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'recruiter_demo') . '/import/resume_personal.csv', array(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationFieldCollection('field_resume_personal_info', array(
      'host_entity_type' => 'profile2'
    ));

    $this->addFieldMapping('field_resume_photo', 'photo');
    $this->addFieldMapping('field_resume_photo:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_resume_photo:source_dir')
      ->defaultValue(drupal_get_path('module', 'recruiter_demo') . '/images');

    $this->addFieldMapping('field_resume_salutation', 'salutation');
    $this->addFieldMapping('field_resume_firstname', 'first_name');
    $this->addFieldMapping('field_resume_lastname', 'last_name');

    $arguments = array(
      'administrative_area' => array('source_field' => 'province'),
      'locality' => array('source_field' => 'city'),
      'postal_code' => array('source_field' => 'postal_code'),
      'thoroughfare' => array('source_field' => 'street'),
      'premise' => array('source_field' => 'premise'),
    );
    $this->addFieldMapping('field_resume_address', 'country')->arguments($arguments);
    $this->addFieldMapping('field_resume_phone', 'phone');
    $this->addFieldMapping('field_resume_birthday', 'birthday');
    $this->addFieldMapping('field_resume_notes', 'notes');
  }

}

/**
 * Class for importing field collection items from the job preferences collection.
 */
class RecruiterResumePrefMigration extends RecruiterResumeFieldCollectionMigration {

  public function __construct() {
    parent::__construct();
    $this->description = t("Import resume's job preferences field collection items.");

    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'recruiter_demo') . '/import/resume_job_preferences.csv', array(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationFieldCollection('field_resume_job_preferences', array(
      'host_entity_type' => 'profile2'
    ));

    $this->addFieldMapping('field_resume_job_title', 'title');
    $this->addTermFieldMapping('field_resume_employment_term', 'type');
    $this->addTermFieldMapping('field_resume_career_status', 'career_status');
    $this->addFieldMapping('field_resume_job_availability', 'start');
    $this->addFieldMapping('field_resume_desired_salary', 'salary');
    $this->addTermFieldMapping('field_resume_region', 'region');
    $this->addTermFieldMapping('field_resume_jobpref_field', 'occupation');
    $this->addFieldMapping('field_resume_summary', 'notes');
  }

  /**
   * Mark all resumes as 'visible'.
   */
  public function postImport() {
    foreach ($this->getMap() as $item) {
      if ($fc = field_collection_item_load($item->destid1)) {
        $profile = $fc->hostEntity();
        $profile->field_resume_state[LANGUAGE_NONE][0]['value'] = 'visible';
        profile2_save($profile);
      }
    }
  }

}

/**
 * Class for importing field collection items from the education collection.
 */
class RecruiterResumeEduMigration extends RecruiterResumeFieldCollectionMigration {

  public function __construct() {
    parent::__construct();
    $this->description = t("Import resume's education field collection items.");

    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'recruiter_demo') . '/import/resume_education.csv', array(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationFieldCollection('field_resume_education', array(
      'host_entity_type' => 'profile2'
    ));

    $this->addTermFieldMapping('field_resume_degree_level_term', 'degree');
    $this->addFieldMapping('field_resume_education_instituti', 'institution');
    $this->addTermFieldMapping('field_resume_of_study', 'field_of_study');
    $this->addFieldMapping('field_resume_time_period', 'period');
    $this->addFieldMapping('field_resume_summary', 'summary');
  }

  public function prepareRow($row) {
    $this->_parsePeriod($row, 'period');
    return parent::prepareRow($row);
  }

}

/**
 * Class for importing field collection items from the work experience collection.
 */
class RecruiterResumeWorkMigration extends RecruiterResumeFieldCollectionMigration {

  public function __construct() {
    parent::__construct();
    $this->description = t("Import resume's work experience field collection items.");

    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'recruiter_demo') . '/import/resume_work_experience.csv', array(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationFieldCollection('field_resume_xp', array(
      'host_entity_type' => 'profile2'
    ));

    $this->addFieldMapping('field_resume_job_title', 'title');
    $this->addTermFieldMapping('field_resume_xp_occupation_field', 'occupation');
    $this->addTermFieldMapping('field_resume_employment_term', 'type');
    $this->addFieldMapping('field_resume_xp_organization', 'company_name');
    $this->addFieldMapping('field_resume_xp_location', 'location');
    $this->addFieldMapping('field_resume_xp_link', 'company_link');
    $this->addFieldMapping('field_resume_time_period', 'period');
    $this->addFieldMapping('field_resume_summary', 'notes');
  }

  public function prepareRow($row) {
    $this->_parsePeriod($row, 'period');
    return parent::prepareRow($row);
  }

}

/**
 * Class for importing field collection items from the language collection.
 */
class RecruiterResumeLanguageMigration extends RecruiterResumeFieldCollectionMigration {

  public function __construct() {
    parent::__construct();
    $this->description = t("Import resume's language field collection items.");

    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'recruiter_demo') . '/import/resume_language.csv', array(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationFieldCollection('field_resume_languages', array(
      'host_entity_type' => 'profile2'
    ));

    $this->addFieldMapping('field_resume_languages_level', 'language')->separator(',');
    $this->addFieldMapping('field_resume_languages_level:level', 'language_level')->separator(',');
    $this->addFieldMapping('field_resume_notes', 'notes');
  }
}

  /**
   * Class for importing field collection items from the general skills collection.
   */
class RecruiterResumeSkillsGeneralMigration extends RecruiterResumeFieldCollectionMigration {

  public function __construct() {
    parent::__construct();
    $this->description = t("Import resume's general skills field collection items.");

    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'recruiter_demo') . '/import/resume_skills_general.csv', array(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationFieldCollection('field_resume_skills_general', array(
      'host_entity_type' => 'profile2'
    ));

    $this->addFieldMapping('field_resume_skills_general_lev', 'skill')->separator(',');
    $this->addFieldMapping('field_resume_skills_general_lev:level', 'skill_level')->separator(',');
    $this->addFieldMapping('field_resume_notes', 'notes');
  }
}

/**
 * Class for importing field collection items from the it skills collection.
 */
class RecruiterResumeSkillsITMigration extends RecruiterResumeFieldCollectionMigration {

  public function __construct() {
    parent::__construct();
    $this->description = t("Import resume's it skills field collection items.");

    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'recruiter_demo') . '/import/resume_skills_it.csv', array(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationFieldCollection('field_resume_skills_it', array(
      'host_entity_type' => 'profile2'
    ));

    $this->addFieldMapping('field_resume_skills_it_level', 'skill')->separator(',');
    $this->addFieldMapping('field_resume_skills_it_level:level', 'skill_level')->separator(',');
    $this->addFieldMapping('field_resume_notes', 'notes');
  }
}

/**
 * Load a resume profile PID by user name.
 */
function recruiter_demo_load_pid_by_user_name($user_name) {
  return $pid = db_query("SELECT p.pid FROM {profile} p JOIN {users} u ON p.uid = u.uid WHERE u.name = :name AND p.type = 'resume'", array(':name' => $user_name))->fetchField();
}
