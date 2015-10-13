<?php

class CourseObjectMoodleQuestionnaire extends CourseObjectMoodle {

  function getOptionsSummary() {
    $summary = parent::getOptionsSummary();
    if ($this->getInstanceId()) {
      $summary['questions'] = l('Edit questions', "node/{$this->getCourseNid()}/course-outline/course-moodle/{$this->getId()}/questions");
    }
    return $summary;
  }

  function poll() {
    global $user;
    global $_course_moodle_prefix;
    $uid = $user->uid;

    $requirement = $this->getOptions();
    //'Evaluation'
    if ($requirement['instance']) {
      if (!$moodle_course_module = course_moodle_get_coursemodule_from_id('questionnaire', $requirement['instance'])) {
        //error("There is no coursemodule with id $id");
      }
      if (!$moodle_questionnaire = course_moodle_moodle_get_record("questionnaire", "id", $moodle_course_module->instance)) {
        //error("The Moodle questionnaire with id $moodle_course_module->instance corresponding to this coursemodule $requirement['instance'] is missing");
      }
    }

    //check for questionnaire submission


    $sql = "SELECT mq.* FROM {$_course_moodle_prefix}questionnaire_attempts mq, {$_course_moodle_prefix}user mu
      WHERE mq.userid = mu.id AND qid = :quid AND mu.idnumber = :uid";
    $results_check_questionnaire_check = db_query($sql, array(':quid' => $moodle_questionnaire->id, ':uid' => $uid));
    while ($row = $results_check_questionnaire_check->fetch(PDO::FETCH_ASSOC)) {
      $array_check_questionnaire = $row;
    }

    if (is_array($array_check_questionnaire)) {
      $this->getFulfillment()->setComplete(1)->save();
    }
  }

  public function getTakeUrl() {
    return url('moodle/mod/questionnaire/view.php', array('query' => array('id' => $this->getInstanceId())));
  }

  function getSummaryCSV() {
    $instance = $this->getMoodleInstanceId();
    // Get array of Drupal user IDs from Moodle CSV
    $users = array();
    $csv = array();
    $url = url('moodle/mod/questionnaire/report.php', array('query' => "instance=$instance&action=dcsv", 'absolute' => TRUE));
    $response = $this->getHttpRequestAsMoodleAdmin($url);
    $text = explode("\n", trim($response->data));
    foreach ($text as $key => $line) {
      if ($key) {
        // Any line other than the first line that doesn't have a response ID
        // should be merged with the header row.
        if (!preg_match('#^\d+\t#', $line)) {
          $text[0] .= $line;
          unset($text[$key]);
        }
      }
    }

    foreach ($text as $line) {
      $fields = explode("\t", $line);
      $fields = array_map('trim', $fields);

      if (!$skip) {
        $headers = $fields;
        $skip = TRUE;
      }
      else {
        $fields = array_pad($fields, count($headers), '');
        $newline = array_combine($headers, $fields);
        $keep = array(
          'Response',
          'Submitted on:',
          'Course',
          'ID',
        );
        foreach ($newline as $key => &$field) {
          if (!in_array($key, $keep) && strpos($key, 'Q') !== 0) {
            unset($newline[$key]);
          }
        }
        $csv[$newline['Response']] = $newline;
        $users[$newline['ID']] = NULL;
      }
    }

    // get drupal IDs from array of moodle IDs in keys
    // $users is an array of drupalID=>moodleID
    $values = array_filter(array_keys($users));
    $sql = "select id, idnumber from {mdl_user} u where id in (:ids)";
    $result = db_query($sql, array(':ids' => $values));
    while ($row = $result->fetch()) {
      $users[$row->id] = $row->idnumber;
    }

    foreach ($csv as &$row) {
      $row['idnumber'] = $users[$row['ID']];
    }

    return $csv;
  }

  function getReports() {
    $reports = parent::getReports();
    $reports += array(
      'moodle' => array(
        'title' => 'Results',
        'default' => TRUE,
        'pdf' => TRUE,
        'children' => array(
          'csv' => array(
            'title' => 'Export with profile data',
            'file' => TRUE,
          ),
        ),
      ),
    );
    return $reports;
  }

  function getReport($key, $subkey) {
    if ($key == 'moodle' && !$subkey) {
      $reports = $this->getReports();
      $report = $this->getReportInfo();
      return array(
        'title' => $reports[$key]['title'],
        'url' => $report['url'],
      );
    }
    if ($key == 'overview' && $subkey == 'csv') {
      $this->renderOverview();
    }

    return parent::getReport($key);
  }

  function renderOverview() {
    $mcsv = $this->getSummaryCSV();
    $dcsv = $this->attachProfile($mcsv);
    $this->renderCSV($dcsv);
  }

  function getMoodleQuestionsEditUrl() {
    return url('moodle/mod/questionnaire/questions.php', array('query' => array('id' => $this->getInstanceId())));
  }

}
