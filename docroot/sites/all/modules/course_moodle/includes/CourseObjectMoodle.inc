<?php

/**
 * Moodle object base class.
 *
 * Provides sessioning support for remote course object creation.
 */
class CourseObjectMoodle extends CourseObject {

  private $cid = NULL;

  /**
   * Specify that this object needs a navigation listener.
   */
  public function hasPolling() {
    return TRUE;
  }

  function getHttpRequestAsMoodleAdmin($url) {
    $session = $this->getSession();
    $headers['Cookie'] = $session['cookie'];
    $response = drupal_http_request($url, array('headers' => $headers));
    return $response;
  }

  function getContentFromUrl($url) {
    global $base_url;
    print($base_url . $url);
    return $this->getHttpRequestAsMoodleAdmin($base_url . $url)->data;
  }

  /**
   * Return a table from a CSV.
   */
  function csv2table($csv_data) {
    $out = '';
    $temp = fopen("php://memory", "rw");
    fwrite($temp, $csv_data);
    fseek($temp, 0);
    while ($line = fgetcsv($temp, 0, "\t")) {
      $row = array();

      foreach ($line as $cell) {
        $row[] = $cell;
      }

      if (!$headers) {
        $headers = $row;
      }
      else {
        $rows[] = $row;
      }
    }
    fclose($temp);
    $out .= theme_table($headers, $rows);
    return $out;
  }

  /**
   * Get the primary report URL for this moodle object.
   *
   * @return array
   *   Array of
   *     'title': The title of the object
   *     'url':  URL of the object's report
   */
  function getReportInfo() {
    $coid = $this->getId();
    $sql = "SELECT cr.*, mcm.instance as m_instance
    FROM {mdl_course_modules} mcm, {course_outline} cr, {course_moodle} cn
    WHERE cr.nid = cn.nid
    AND mcm.course  = cn.cid
    AND cr.instance = mcm.id
    AND cr.coid = :coid";
    $result = db_query($sql, array(':coid' => $coid));

    $row = $result->fetch();
    switch ($row->object_type) {
      case 'quiz':
        $url = 'moodle/mod/quiz/report.php';
        $link = url($url, array('query' => array('q' => $row->m_instance)));
        break;
      case 'questionnaire':
        $url = 'moodle/mod/questionnaire/report.php';
        $link = url($url, array('query' => array('instance' => $row->m_instance, 'sid' => $row->m_instance, 'action' => 'vall')));
        break;
      case 'scorm':
        $url = 'moodle/mod/scorm/report.php';
        $link = url($url, array('query' => array('id' => $row->instance)));
        break;
    }

    if ($link) {
      return array(
        'title' => $row->title,
        'url' => $link,
      );
    }
  }

  /**
   * Return the moodle component instance (different than the course instance).
   */
  function getMoodleInstanceId() {
    $coid = $this->getId();

    $sql = "SELECT cr.*, mcm.instance as m_instance
      FROM {mdl_course_modules} mcm, {course_outline} cr, {course_moodle} cn
      WHERE cr.nid = cn.nid
      AND mcm.course  = cn.cid
      AND cr.instance = mcm.id
      AND cr.coid = :coid";
    $result = db_query($sql, array(':coid' => $coid));
    $row = $result->fetch();
    return $row->m_instance;
  }

  /**
   * $csv is an array of rows, each with an 'idnumber' element being the drupal user ID.
   * @param type $csv
   * @return type
   */
  function attachProfile($csv) {
    foreach ($csv as $row) {
      $users[] = $row['idnumber'];
    }

    $view = views_get_view('course_moodle_profile');
    $view->set_arguments(
      array(
        implode(',', array_filter(array_values($users))),
      )
    );
    $view->preview();

    foreach ($view->style_plugin->rendered_fields as $rowidx => $values) {
      $uid = $values['uid'];
      foreach ($values as $fieldname => $value) {
        $viewarray[$uid][$view->field[$fieldname]->options['label']] = $value;
      }
    }

    foreach ($csv as $rid => &$response) {
      if ($viewarray[$response['idnumber']]) {
        $response = array_merge($response, $viewarray[$response['idnumber']]);
      }
    }

    return $csv;
  }

  function renderCSV($dcsv, $headers = NULL) {
    $fh = fopen('php://temp/maxmemory:' . (12 * 1024 * 1024), 'r+');
    if ($headers) {
      fputcsv($fh, $headers);
    }
    foreach ($dcsv as $row) {
      if (!$headers) {
        fputcsv($fh, array_keys($row));
        $headers = TRUE;
      }
      fputcsv($fh, $row);
    }
    rewind($fh);
    $output = stream_get_contents($fh);
    fclose($fh);
    header('Content-type: text/csv');
    header('Content-disposition: attachment; filename=' . $this->getCourseNid() . '-' . str_replace(' ', '-', $this->getTitle()) . '.csv');
    print $output;
    exit();
  }

  /**
   * Create a Moodle course to associate course objects to.
   */
  function create() {
    $courseNode = node_load($this->getCourseNid());
    if (!$courseNode->course_moodle['cid']) {
      $courseNode->course_moodle['cid'] = $this->createCourse($courseNode);
      course_moodle_node_update($courseNode, 'update', NULL, NULL);
    }
  }

  /**
   * Create a Moodle course.
   *
   * @return int
   *   Moodle course ID.
   */
  public static function createCourse($courseNode) {
    $params = array(
      'courses' => array(
        'fullname' => $courseNode->title,
        'categoryid' => 1,
        'shortname' => $courseNode->course_moodle['machine_name'],
        'idnumber' => $courseNode->course['external_id'],
      ),
    );
    $result = course_moodle_call('core_course_create_courses', $params);
    return $result[0]['id'];
  }

  /**
   * Verify shared session and get sesskey:
   */
  static function getSession() {
    global $base_url;
    static $session_info = array();

    if (!empty($session_info)) {
      return $session_info;
    }

    $name = session_name();
    $value = session_id();
    $cookie = "$name=$value;";

    // url of the moodle-drupal homepage
    $url = course_moodle_path(TRUE) . '/login/index.php';

    // necessary for the POST method
    $headers = array('Cookie' => $cookie);
    // if successful, parse to get the sesskey
    $login_response = drupal_http_request($url, array('headers' => $headers));
    preg_match_all('/MoodleSession=(.*);/U', $login_response->headers['set-cookie'], $matches);
    foreach ($matches as $match) {
      $last_session = $match[1];
    }
    $headers['Cookie'] .= "MoodleSession=$last_session";
    $response = drupal_http_request(course_moodle_path(TRUE), array('headers' => $headers));
    preg_match('/sesskey":"(\w+)/', $response->data, $match);
    if (isset($match[1])) {
      $sesskey = $match[1];
    }

    if (isset($sesskey) && strlen($sesskey)) {
      $session_info = array(
        'cookie' => $headers['Cookie'],
        'sesskey' => $sesskey,
      );
      return $session_info;
    }
    else {
      return NULL;
    }
  }

  public function getTakeType() {
    return 'iframe';
  }

  public function take() {
    // Save an empty fulfillment record upon starting.
    if (!$this->getFulfillment()->getId()) {
      $this->getFulfillment()->save();
    }

    // Set a cookie with a URL to the course (for externservercourse.php)
    // @todo probably need to set up something for when moodle is run off-site,
    // and on a different domain. services maybe? a service to retrieve the
    // course path in drupal from a moodle course id
    setcookie('course_moodle_course_url', url("course_moodle/escapeframe/{$this->getCourseNid()}", array('absolute' => TRUE)), 0, '/');
  }

  public function getEditUrl() {
    return url("node/{$this->getCourseNid()}/course-outline/course-moodle/{$this->getId()}/edit");
  }

  function getMoodleEditUrl() {
    if (!$this->getInstanceId()) {
      // Create new course object.
      return course_moodle_path(TRUE) . "/course/modedit.php?section=0&add={$this->getComponent()}&course={$this->getCourse()->getNode()->course_moodle['cid']}";
    }
    else {
      // Edit existing course object.
      return course_moodle_path(TRUE) . "/course/modedit.php?update={$this->getInstanceId()}";
    }
  }

  function getMoodleQuestionsEditUrl() {

  }

}
