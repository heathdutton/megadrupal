<?php

class CourseObjectMoodleQuiz extends CourseObjectMoodle {

  function getOptionsSummary() {
    $summary = parent::getOptionsSummary();
    if ($this->getInstanceId()) {
      $summary['questions'] = l('Edit questions', "node/{$this->getCourseNid()}/course-outline/course-moodle/{$this->getId()}/questions");
    }
    return $summary;
  }

  /**
   *
   * @param type $instance
   * @return type
   */
  function getOverviewCSV() {
    $instance = $this->getMoodleInstanceId();
    // Get array of Drupal user IDs from Moodle CSV
    $users = array();
    $csv = array();

    $url = url("moodle/mod/quiz/report.php", array(
      'query' => array('q' => $instance, 'mode' => 'responses', 'attemptsmode' => 0, 'qmfilter' => 1, 'download' => 'CSV'),
      'absolute' => TRUE,
    ));
    $response = $this->getHttpRequestAsMoodleAdmin($url);
    $text = explode("\n", trim($response->data));
    foreach ($text as $line) {
      $fields = str_getcsv($line);

      unset($fields[0], $fields[1], $fields[3], $fields[4]);
      if (!$skip) {
        $headers = $fields;
        $fields[] = 'Title';
        $skip = TRUE;
      }
      else {
        $fields = array_pad($fields, count($headers), '');
        $newline = array_combine($headers, $fields);
        $newline['title'] = $this->getTitle();
        $newline['idnumber'] = $newline['ID number'];
        $csv[] = $newline;
      }
    }
    return $csv;
  }

  function getReports() {
    $reports = parent::getReports();
    $reports += array(
      'moodle' => array(
        'title' => 'Results',
        'default' => TRUE,
      ),
      'csv' => array(
        'title' => 'CSV',
        'default' => TRUE,
      ),
      'item_analysis' => array(
        'title' => 'Analysis/Compare',
      ),
    );
    return $reports;
  }

  function getReport($key, $subkey = NULL) {
    $default = parent::getReport($key);

    if (!$subkey) {
      $reports = $this->getReports();
      switch ($key) {
        case 'moodle':
          $report = $this->getReportInfo();
          return array(
            'title' => $reports[$key]['title'],
            'url' => $report['url'],
          );
        case 'csv':
          return array(
            'content' => $this->renderOverview(),
          );
        case 'item_analysis':
          $instance = $this->getMoodleInstanceId();
          $url = url("/moodle/mod/quiz/report.php?q=$instance&mode=statistics", array('external' => TRUE));
          return array(
            'title' => $reports[$key]['title'],
            'url' => $url,
          );
        case 'compare':
          return array(
            'title' => 'Comparison',
            'content' => $this->renderCompare(),
          );
      }
    }

    return $default;
  }

  /**
   * Render comparison report.
   * @return type
   */
  function renderCompare() {
    $op = $_GET['op'];
    $coid = $_GET['coid'];

    $out = '';

    $compare = $coid && in_array($op, array('Compare', 'Download comparison'));
    $compare_csv = ($op == 'Download comparison');

    $out .= drupal_get_form('course_moodle_quiz_comparison_form', $this);


    $instance = $this->getInstanceId();

    $url = url('moodle/mod/quiz/report.php', array('query' => array('mode' => 'analysis', 'noheader' => 'yes', 'download' => 'CSV', 'id' => ''), 'absolute' => TRUE));
    $response_base = $this->getHttpRequestAsMoodleAdmin($url . $instance);
    $questions_base = $this->analysisToArray($response_base->data, $headers);

    $strip_columns = array('Question text');

    if ($compare) {
      // Strip out columns that wouldn't make sense.
      $strip_columns = array_merge($strip_columns, array('SD', 'Disc. Index', 'Disc. Coeff. '));
      $compare_additional_headers = array('R. Counts', 'R.%', 'Q. count', '% Correct Facility');
      $headers = array_merge($headers, $compare_additional_headers);
    }

    if ($compare) {
      // If comparing, load the other object. Check to see if it has the same questions.
      $comparedCourseObject = new CourseObjectMoodleQuiz($coid);
      $response_compare = $this->getHttpRequestAsMoodleAdmin($url . $comparedCourseObject->getInstanceId());
      $questions_compare = $this->analysisToArray($response_compare->data);
      if (array_filter(array_diff_key($questions_base, $questions_compare))) {
        return $out . "Sorry, you must compare two tests with identical questions.";
      }
    }

    // Strip headers we don't need.
    foreach ($headers as $key => $header) {
      if (in_array($header, $strip_columns)) {
        unset($headers[$key]);
      }
    }

    // The position where the header starts to show which results are Pre/Post.
    $header_compare_start = 5;

    if ($compare_csv) {
      $rows[0] = array_pad(array(), count($headers), '');
    }
    else {
      $rows[0][0] = array(
        'data' => '',
        'colspan' => $header_compare_start,
      );
    }

    if ($compare) {
      $header_break = $header_compare_start + count($compare_additional_headers);

      $rows[0][$header_compare_start] = $this->getTitle();
      $rows[0][$header_compare_start + count($compare_additional_headers)] = $comparedCourseObject->getTitle();

      if ($_REQUEST['op'] != 'CSV') {
        $rows[0][$header_compare_start] = array(
          'data' => $rows[0][$header_compare_start],
          'colspan' => count($compare_additional_headers),
          'class' => 'test-1',
        );

        $rows[0][$header_break] = array(
          'data' => $rows[0][$header_break],
          'colspan' => count($compare_additional_headers),
          'class' => 'test-2',
        );
      }
    }

    // Combine reports to compare.
    foreach ($questions_base as $qno => $question) {
      foreach ($question as $rid => $response) {
        $row = array();
        $cell = 0;
        foreach ($response as $key => $field) {
          if (!in_array($key, $strip_columns)) {
            $cell++;
            if ($compare && !$compare_csv && $cell >= $header_compare_start + 1) {
              $row[] = array(
                'data' => $field,
                'class' => 'quiz-1',
              );
            }
            else {
              $row[] = $field;
            }
          }
        }

        if ($compare) {
          foreach ($questions_compare[$qno][$rid] as $key => $field) {
            if (in_array($key, array('R. Counts', 'R.%', 'Q. count', '% Correct Facility'))) {
              if (!$compare_csv) {
                $row[] = array(
                  'data' => $field,
                  'class' => 'quiz-2',
                );
              }
              else {
                $row[] = $field;
              }
            }
          }
        }

        if (!$compare_csv) {
          $rows[] = array(
            'data' => $row,
            'class' => $row[0] ? 'question' : '',
          );
        }
        else {
          $rows[] = $row;
        }
      }
    }

    if ($compare_csv) {
      $this->renderCSV($rows, $headers);
    }
    else {
      $out .= theme_table($headers, $rows);
    }

    if ($op == 'Download responses') {
      $mcsv = $this->getOverviewCSV();
      $courseObjectCompare = new CourseObjectMoodleQuiz($coid);
      $ccsv = $courseObjectCompare->getOverviewCSV();
      $combined = array_merge($mcsv, $ccsv);
      $this->renderCSV($combined);
    }

    return $out;
  }

  function analysisToArray($data, &$headers = NULL) {
    $csv = explode("\n", trim($data));
    $questions = array();
    foreach ($csv as $line) {
      $line = explode("\t", $line);
      if (!$headers) {
        $headers = $line;
      }

      $line = array_pad($line, count($headers), '');

      if (is_numeric($line[0]) && $line[0] != $inquestion) {
        // This is the start of a question, or continuation.
        $inquestion = $line[0];
      }

      if ($inquestion) {
        $questions[$inquestion][] = array_combine($headers, $line);
      }
    }

    return $questions;
  }

  function renderOverview() {
    $mcsv = $this->getOverviewCSV();
    $dcsv = $this->attachProfile($mcsv);
    $this->renderCSV($dcsv);
  }

  function poll() {
    global $user;
    global $_course_moodle_prefix;

    $requirement = $this->getOptions();
    //'Quiz'
    if ($requirement['instance']) {
      if (!$moodle_course_module = course_moodle_get_coursemodule_from_id('quiz', $requirement['instance'])) {
        //error("There is no coursemodule with id $id");
      }
      if (!$moodle_quiz = course_moodle_moodle_get_record("quiz", "id", $moodle_course_module->instance)) {
        //error("The quiz with id $cm->instance corresponding to this coursemodule $id is missing");
      }
    }
    $sql_get_quizzes = "SELECT mqa.*,q.sumgrades as quiz_sumgrades from {$_course_moodle_prefix}quiz_attempts mqa
      INNER JOIN {$_course_moodle_prefix}user u on (u.id = mqa.userid)
      INNER JOIN {$_course_moodle_prefix}quiz q on (q.id = mqa.quiz)
      WHERE u.idnumber = :uid and mqa.quiz = :quizid
      ORDER BY mqa.sumgrades ASC";
    $results = db_query($sql_get_quizzes, array(':uid' => $user->uid, ':quizid' => $moodle_quiz->id));

    while ($row = $results->fetch(PDO::FETCH_ASSOC)) {
      $quiz_record = $row;
    }

    if ($requirement['required'] && (!is_array($quiz_record) || !$quiz_record['timefinish'])) {
      // If the quiz is required and they haven't finished an attempt, fail.
      return FALSE;
    }

    if (is_array($quiz_record)) {
      // @todo sumgrades is the number of points now - we need percentage for
      // Course right now
      $passing_grade = $requirement['passing_grade'] / 100;
      $attempt = $quiz_record['sumgrades'] / $quiz_record['quiz_sumgrades'];
      if ($passing_grade <= $attempt && $quiz_record['timefinish'] > 0) {
        $this->getFulfillment()->setOption('grade_result', intval($attempt * 100))->setComplete(1)->save();
      }
    }
  }

  function getTakeUrl() {
    return url('moodle/mod/quiz/view.php', array('query' => array('id' => $this->getInstanceId())));
  }

  function getMoodleQuestionsEditUrl() {
    return url('moodle/mod/quiz/edit.php', array('query' => array('cmid' => $this->getInstanceId())));
  }

  /**
   * Moodle quiz options.
   */
  public function optionsDefinition() {
    $options = parent::optionsDefinition();

    $options['passing_grade'] = 75;

    return $options;
  }

  /**
   * Add a passing grade option.
   */
  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);
    $defaults = $this->getOptions();

    $form['grading']['passing_grade'] = array(
      '#title' => t('Passing grade'),
      '#type' => 'textfield',
      '#size' => 4,
      '#default_value' => $defaults['passing_grade'],
      '#description' => t('The user will not be able to proceed past this object unless this grade is met.'),
    );
  }

  function isGraded() {
    return TRUE;
  }

}
