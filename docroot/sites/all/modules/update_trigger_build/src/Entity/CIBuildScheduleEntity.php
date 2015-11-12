<?php

/**
 * @file
 *  Class for the CI Build Schedule entity.
 */

/**
 * The CIBuildScheduleEntity holds the information required to run a build on a CI Job. It also 
 * acts as the jump off point for triggering a build.
 * 
 * The CI Server, the CI Job and the information pertaining to the schedule, such as frequency 
 * and last run time are stored on this entity.    
 * 
 * The conditions that trigger a build (core update, module update, 
 * security release only) are also held here.
 *
 * @see CIBuildScheduleEntityController
 */
class CIBuildScheduleEntity extends Entity {

  /**
   * Change the default URI from default/id to ci_build_schedule/id.  This is where
   * the entity can be viewed.
   *
   * @see Entity::defaultURI()
   */
  protected function defaultURI() {
    return array('path' => 'ci_build_schedule/' . $this->identifier());
  }

  /**
   * Determines whether enough time has passed since the last run time.  Frequency is
   * set when creating a build schedule entity and can be 10 mins, 1 hour etc.
   *
   * @return boolean
   *  TRUE if the duration has passed.  FALSE if the frequency duration has not passed.
   *  FALSE will prevent a build from running.
   */
  public function checkFrequency() {

    // Frequency and last run time are stored as UNIX timestamps.
    if(time() - $this->last_run > $this->frequency) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Check whether a build should be triggered.  
   *
   * The following statuses are used by the update module:
   *
   *   UPDATE_CURRENT (5) = "UP TO DATE"
   *   UPDATE_NOT_CURRENT (4) = "Non-security release available"
   *   UPDATE_NOT_SECURE (1) = "Security release available"
   *
   * The above equates to the following triggers used by this module:
   * 
   *   none
   *   all
   *   security
   * 
   * Different values can be set for core, modules, themes.
   *
   * @param array $update_data
   *  The data provided by the update module.  This contains a list of 
   *  projects that can be updated.
   *
   * @return array $projects
   *  A list of projects that require an update and also meet the build triggers.
   *  If an empty array is returned then no build should be triggered.  
   *  The projects list can be passed to the build server as a list of parameters, 
   *  which is useful if the build script wants to do 'drush up project'.
   */
  public function checkTriggers($update_data) {

    $projects = array();
    $parameters = array();
 
    $parameter_config = unserialize($this->parameters);

    foreach ($update_data as $project_name => $data) {

      $add_project = FALSE;

      // The trigger properties on the the CI Build Schedule entity are 
      // in format trigger_core, trigger_module etc.
      $trigger_key = 'trigger_' . $data['project_type'];

      // No build should be triggered for this project type.
      if($this->{$trigger_key} == 'none') {
        continue;
      }
      // If all types of update should trigger a build and we have either a security update 
      // or a non-securty update then add this project to the array.
      else if($this->{$trigger_key} == 'all' && ($data['status'] == UPDATE_NOT_CURRENT || $data['status'] == UPDATE_NOT_SECURE)) {
        $add_project = TRUE;
      }
      // If security releases should trigger a build and we have a security release 
      // then add the project to the list.
      else if($this->{$trigger_key} == 'security' && $data['status'] == UPDATE_NOT_SECURE) {
        $add_project = TRUE;
      }

      // Add this project to the list of projects that require an upgrade.      
      if($add_project) {
        // Drupal core version is formatted differently.
        if($project_name == 'drupal') {
          $projects[] = $project_name . ':' . $data['recommended'];
        }
        else {
          // For modules, themes etc remove the Drupal version. E.g. 7.x-3.0 becomes 3.0.
          $version = explode('.x-', $data['recommended']);
          $projects[] = $project_name . ':' . $version[1];
        }
      }
    }
    return $projects;
  }

  /**
   * Gathers the information required to run a build and passes it to the relevant
   * CI Server to run the build.
   *
   * @param array $projects
   *  The list of projects whose updates meet the build triggers.  The CI Server Type entity
   *  can optionally pass these as parameters to the build.
   */
  public function initiateBuild($projects) {

    $parameters = $this->prepareParameters($projects);

    // Load the CI Server.
    $ci_server_info = explode(':', $this->ci_server);
    $entity_type = $ci_server_info[0];
    $entity_id = $ci_server_info[1];

    $ci_server = entity_load($entity_type, array($entity_id));

    // An array containing one value is returned so pop the the first item.
    $ci_server = reset($ci_server);

    // At this point we have all the info we need to run a build.
    $ci_server->runBuild($this, $parameters);

    // Update the last run time.
    $this->last_run = time();
    $this->save();
  }

  /**
   * Prepare the list of parameters that should be sent to the
   * build.  The user configures which parameters sould be sent in the
   * build schedule.
   * 
   * @param array $projects
   *  The list of projects that should be upgraded.  This is added as
   *  a parameter depending on the config.
   * 
   * @return array $parameters
   *  An associative array of the parameter values, keyed by parameter
   *  machine name.
   */
  private function prepareParameters($projects) {

    $parameters = array();
    $parameter_config = unserialize($this->parameters);

    // Check the config and include the necessary parameters.
    if(isset($parameter_config['projects'])) {
      $parameters['projects'] = $projects;
    }

    if(isset($parameter_config['drupal_root'])) {
      $parameters['drupal_root'] = DRUPAL_ROOT;
    }

    if(isset($parameter_config['site_root'])) {
      $parameters['site_root'] = conf_path();
    }

    return $parameters;
  }
}
