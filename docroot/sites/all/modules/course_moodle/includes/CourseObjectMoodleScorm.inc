<?php

class CourseObjectMoodleScorm extends CourseObjectMoodle {

  function poll() {
    global $user;
    global $_course_moodle_prefix;
    $uid = $user->uid;

    $requirement = $this->getOptions();
    $node = node_load($requirement['nid']);
    //'SCORM/AICC'
    $sql_get_scorms = "SELECT * FROM {$_course_moodle_prefix}scorm WHERE course = :cid";
    $results = db_query($sql_get_scorms, array(':cid' => $node->course_moodle['cid']));

    while ($row = $results->fetch(PDO::FETCH_ASSOC)) {
      $scormid = $row['id'];
    }

    $moodle_user = course_moodle_get_moodle_user($uid);

    if ($scormid) {
      //check the scorm records for the user
      $tracked_scorm_complete = $this->scormtrack($moodle_user['id'], $scormid);

      if (is_array($tracked_scorm_complete) && $tracked_scorm_complete['complete']) {
        $this->getFulfillment()->setComplete(1)->save();
      }
    }
  }

  /**
   * Track a SCORM object.
   */
  function scormtrack($userid, $scormid) {
    global $_course_moodle_prefix;


    $res = db_query("SELECT * FROM {$_course_moodle_prefix}scorm WHERE id = :scoid", array(':scoid' => $scormid));
    while ($row = $res->fetch()) {
      $scorm = $row;
    }

    $SQL = "SELECT * FROM {$_course_moodle_prefix}scorm_scoes_track WHERE element LIKE :like AND (value like :passed OR value like :finished OR value like :finished)  AND scormid = :scormid AND userid = :userid";
    $resq = db_query($SQL, array(':like' => '%lesson_status%', ':complete' => 'complete%', ':passed' => 'passed%', ':finished' => 'finished%', ':scormid' => $scormid, ':userid' => $userid));
    while ($row = $resq->fetch()) {
      $scormtrack = $row;
    }

    if (is_object($scormtrack)) {
      $date_completed = NULL;
      $complete = NULL;

      if (trim($scormtrack->value) == 'passed' || trim($scormtrack->value) == 'completed' || trim($scormtrack->value) == 'complete' || trim($scormtrack->value) == 'finished') {
        $date_completed = $scormtrack->timemodified;
        $complete = 1;
      }
      elseif ($scormtrack->value == 'incomplete') {
        $date_completed = NULL;
        $complete = 0;
      }
    }
    else {
      $date_completed = NULL;
      $complete = NULL;
    }

    if ($scormtrack->id) {
      //echo "track";
    }
    return array(
      'date_completed' => $date_completed,
      'complete' => $complete,
    );
  }

  public function getTakeUrl() {
    return url('moodle/mod/scorm/view.php', array('query' => array('id' => $this->getInstanceId())));
  }

  function getReport($key) {
    $reports = $this->getReports();
    if ($key == 'moodle') {
      $report = $this->getReportInfo();
      return array(
        'title' => $reports[$key]['title'],
        'url' => $report['url'],
      );
    }

    return parent::getReport($key);
  }

  function getReports() {
    $reports = parent::getReports();
    $reports += array(
      'moodle' => array(
        'title' => 'Results',
        'default' => TRUE,
      ),
    );
    return $reports;
  }

  function getMoodleQuestionsEditUrl() {
    return course_moodle_path(TRUE) . "/course/modedit.php?section=0&add=scorm&course={$this->getCourse()->getNode()->course_moodle['cid']}";
  }

}
