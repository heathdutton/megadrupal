<?php

class CourseObjectMoodleResource extends CourseObjectMoodle {

  function poll() {
    global $user;
    global $_course_moodle_prefix;
    $uid = $user->uid;

    $resource = $this->getComponent();
    $cid = db_query('select cid from {course_moodle} where nid = :nid', array(':nid' => $this->getCourseNid()))->fetchField();

    if (!$moodle_course_module = course_moodle_get_coursemodule_from_id('resource', $requirement['instance'])) {
      //error('Course Module ID was incorrect');
    }
    if (!$resource = course_moodle_moodle_get_record('resource', 'id', $moodle_course_module->instance)) {
      //error('Resource ID was incorrect');
    }

    /**/
    //currently supported Moodle resources:
    // note: 'resource' changed to 'page'
    $sql_check_views = "SELECT ml.*, mu.idnumber as uid FROM {$_course_moodle_prefix}log ml, {$_course_moodle_prefix}user mu WHERE ml.module ='page' AND ml.userid=mu.id AND action='view' AND  ml.course= :cid AND cmid = :instance AND  mu.idnumber = :uid";
    $result_check_view = db_query($sql_check_views, array(':cid' => $cid, ':instance' => $this->getInstanceId(), ':uid' => $uid));
    while ($row = $result_check_view->fetch(PDO::FETCH_ASSOC)) {
      $array_check_view = $row;
    }

    if (is_array($array_check_view) && $array_check_view['id']) {
      $this->getFulfillment()->setComplete(1)->save();
    }
  }

  public function getTakeUrl() {
    return url('moodle/mod/resource/view.php', array('query' => array('id' => $this->getInstanceId())));
  }

}
