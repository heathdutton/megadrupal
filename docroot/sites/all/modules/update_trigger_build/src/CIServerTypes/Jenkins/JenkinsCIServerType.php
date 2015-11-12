<?php

/**
 * @file
 *  Implement Jenkins as a CI Server Type.
 */

/**
 * Implement a Jenkins CI Server Type.  
 * 
 * There is a Drupal module called Jenkins, 
 * which has implemented many of the get and set requests to Jenkins, so 
 * make use of that rather than re-inventing the wheel.
 */
class JenkinsCIServerType extends Entity implements CIServerTypeInterface {

  /**
   * Trigger a build on the Jenkins CI Server for the given job.
   * 
   * Code adapted from jenkins_job_build function of Jenkins API module,
   * (http://www.drupal.org/project/jenkins).
   *
   * @param CIBuildScheduleEntity $build_schedule
   *  The build schedule entity.  This contains the job Id along with all other info 
   *  that was entered on the create build schedule screen. 
   *
   * @param CIServerEntity $server
   *  A CI Server instance, which contains the host name and other connection credentials.
   *  There can be more than one CI Server defined on each Drupal site,
   *  e.g. there may be myjenkinsserver1.com and myjenkinsserver2.com.
   *
   * @param array $parameters
   *  A list of parameters that should be sent to the build.  Update trigger build
   *  provides the option to include the projects that should be upgraded, which can
   *  be used in shell commands, such as "drush up porject". The Drupal root directory 
   *  and the site root directory are also available as parameters. 
   */
  public function runBuild($build_schedule, $parameters = array()) {

    $data = array();

    // Check if job name is valid.
    if (!jenkins_is_job_name_valid($build_schedule->ci_job)) {
      watchdog('Update Trigger Build', t("Unable to run build on job with Id: @name.  Job name invalid.", array('@name' => $job_id)));
      return FALSE;
    }

    $job_id = rawurlencode($build_schedule->ci_job);

    // Add the projects as a parameter. Concatenate as string delimited by ','.
    // Projects are machine name format, so no need to URL encode.
    if(isset($parameters['projects'])) {
      $data['parameter'][] = array(
        'name' => 'projects',
        'value' => implode(',', $parameters['projects']),
      );
    }

    // Add the rest of the parameters.
    foreach($parameters as $name => $value) {
      // Projects parameter has already been processed, so ignore it.
      if($name != 'projects') {
        $data['parameter'][] = array(
          'name' => $name,
          'value' => $value,
        );
      }
    }

    // Provide a hook that allows other modules to add additional parameters to the build.
    drupal_alter('update_trigger_build_jenkins_parameters', $data);

    // JSONify the parameters and add the headers.
    $json = 'json=' . json_encode($data);
    $headers = array('Content-Type' => 'application/x-www-form-urlencoded');

    // Fire the request to Jenkins.  $response is passed by reference.
    $this->request("/job/{$job_id}/build", $response, array(), 'POST', $json, $headers, $this->host);

    // Logging.
    if(in_array($response->code, range(200, 202))) {
      watchdog('Update Trigger Build', t('Build schedule "@schedule" successful. Job "@job" on server "@name" has been run.', 
        array('@schedule' => $build_schedule->label, '@job' => $job_id, '@name' => $this->label))
      );
    }
    else {
      watchdog('Update Trigger Build', 
        t('Build schedule "@schedule" failed to run. Job "@job" on server "@name" failed with http status "@status" and message "@message".  Parameters: @parameters', 
          array('@schedule' => $build_schedule->label, '@job' => $job_id, '@name' => $this->label, '@status' => $response->code, '@message' => $response->status_message, '@parameters' => print_r($data, TRUE)))
      );
    }
  }

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
   *  the options of a select form item (i.e. an assocative array).
   * 
   * @param integer $depth
   *  Denotes how much data to get from Jenkins.
   * 
   * @return boolean|array $jobs
   *  An array containing the list of jobs or FALSE if jobs could not be retrieved.
  */
  public function getJobs($options = FALSE, $depth = 0) {

    // If $jobs is still FALSE at the end of the request then that denotes 
    // an issue retrieving the jobs form the server.
    $jobs = array();
    $response = array();

    $query = array(
      'depth' => $depth,
    );

    // Make a request to Jenkins to retrieve the Jobs.
    if ($this->request('/api/json', $response, $query, 'GET', NULL, Array(), $this->host)) {
      $response = json_decode($response->data);

      // If no jobs were retrieved then return FALSE.
      if(!isset($response->jobs) || empty($response->jobs)) {
        return FALSE;
      }

      if($options) {
        // Prepare the jobs for display as select options.
        foreach($response->jobs as $job) {
          $jobs[$job->name] = $job->name . ' (' . $job->url . ')';
        }
      }
      else {
        // Grab the full Job objects.
        $jobs = $response->jobs;
      }
    }
    return $jobs;
  }
  
  /**
   * Perform a request to Jenkins server and return the response.
   *
   * This function was adapted from the jenkins_request function in
   * the Jenkins API module.
   *
   * @param string $path
   *   API path with leading slash, for example '/api/json'.
   *
   * @param object $response
   *   HTTP request response object - see drupal_http_request().
   * 
   * @param $query
   *   Data to be sent as query string.
   *
   * @param string $method
   *   HTTP method, either 'GET' (default) or 'POST'.
   *
   * @param array $data
   *   Post data.
   *
   * @param array $headers
   *   HTTP headers.
   *
   * @param string $url
   *   The base URL of the Jenkins host. 
   *
   * @return bool
   *   TRUE if the request was successful.
   */
  private function request($path, &$response = NULL, $query = array(), $method = 'GET', $data = NULL, $headers = array(), $url = 'http://localhost:8080') {

    $url = $url . $path;

    $options = array(
      'method' => $method,
    );

    // Force request to start immediately.
    if (!isset($query['delay'])) {
      $query['delay'] = '0sec';
    }

    $url .= '?' . drupal_http_build_query($query);

    if ($method == 'POST' && !empty($data)) {
      $options['data'] = $data;
    }

    // Default to JSON unless otherwise specified.
    $default_headers = array(
      'Accept' => 'application/json',
      'Content-Type' => 'application/json',
    );
    $headers += $default_headers;

    if (!empty($headers)) {
      $options['headers'] = $headers;
    }

    // Do HTTP request and get response object.
    $response = drupal_http_request($url, $options);

    // Response code should be something between 200 and 202.
    return in_array($response->code, range(200, 202));
  }
}
