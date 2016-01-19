<?php

/**
 * @file
 *  The Update Trigger Build API.
 */

/**
 * This interface allows the module to be extended.  Each implementation will
 * provide an additional CI Server type. Jenkins is implemented by this module 
 * but developers may wish to implement Travis, Go, Bamboo or some other CI Server. 
 *
 * @see JenkinsCIServerEntity for an implementation of this interface.
 */
interface CIServerTypeInterface {
  
  /**
   * Trigger a build on the CI Server for the given job.
   *
   * @param string $job_id
   *  The unique identifier used by the CI Server.  On Jenkins, this is the 
   *  job name.
   *
   * @param array $params
   *  A list of projects to be upgraded.  These can be passed as parameters to the CI Server for
   *  use in shell commands, such as "drush up $project".
   */
  public function runBuild($job_id, $projects = array());

  /**
   * Get all jobs that exist on a CI Server.
   *
   * @param CIServerEntity $server
   *  A CI Server instance.   $server contains the host name and other connection credentials.
   *  There can be more than one CI Server defined on each Drupal site for each 
   *  CI Server Type, e.g. there may be myjenkinsserver1.com and myjenkinsserver2.com. 
   *
   * @param boolean $options
   *  Whether the list should be returned in a format suitable for populating 
   *  the options of a select form item (i.e. an assocative array). The key 
   *  should contain an Id, the value should contain the display name. 
   *  @see CIEntityHelper::convertToOptions 
   */
  public function getJobs($options = FALSE);
}
