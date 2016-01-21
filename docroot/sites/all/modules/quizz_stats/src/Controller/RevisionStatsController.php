<?php

namespace Drupal\quizz_stats\Controller;

class RevisionStatsController {

  private $quiz_vid;
  private $uid;

  /**
   * @param int $quiz_vid
   *   quiz revision id
   * @param int $uid
   *   User id
   */
  public function __construct($quiz_vid, $uid = 0) {
    $this->quiz_vid = $quiz_vid;
    $this->uid = $uid;
  }

  /**
   * Get stats for a single quiz. Maybe also for a single user.
   *
   * @return
   *   HTML page with charts/graphs
   */
  public function render() {
    return array(
        '#theme'    => 'quizz_stats_charts',
        '#attached' => array(
            'css' => array(drupal_get_path('module', 'quizz_stats') . '/quizz_stats.css')
        ),
        '#charts'   => array(
            'takeup'      => $this->getDateVSTakeupCountChart(),
            // line chart/graph showing quiz takeup date along x-axis and count along y-axis
            'status'      => $this->getQuizStatusChart($this->quiz_vid, $this->uid),
            // 3D pie chart showing percentage of pass, fail, incomplete quiz status
            'top_scorers' => $this->getQuizTopScorersChart(),
            // Bar chart displaying top scorers
            'grade_range' => $this->getQuizGradeRangeChart(),
        ),
    );
  }

  /**
   * Generates chart showing how often the quiz has been taken the last days
   *
   * @return string
   *   HTML to display chart
   */
  private function getDateVSTakeupCountChart() {
    $end = 30;
    $sql = "SELECT COUNT(result_id) AS count,
            DATE_FORMAT(FROM_UNIXTIME(time_start), '%Y-%m-%e') AS date
            FROM {quiz_results}
            WHERE quiz_vid = :vid AND time_start > (UNIX_TIMESTAMP(NOW()) - (86400*$end))";
    $sql_args[':vid'] = $this->quiz_vid;
    if ($this->uid) {
      $sql .= " AND uid = :uid";
      $sql_args[':uid'] = $this->uid;
    }
    $sql .= " GROUP BY date ORDER BY time_start ASC";
    $results = db_query($sql, $sql_args);

    // generate date vs takeup count line chart
    if ($res_o = $results->fetchAll()) {
      $chart = theme('date_vs_takeup_count', array('takeup' => $res_o));
      return array(
          // wrapping the chart output with div for custom theming.
          'chart'       => '<div id="date_vs_takeup_count" class="quiz-stats-chart-space">' . $chart . '</div>',
          'title'       => t('Activity'),
          'explanation' => t('This chart shows how many times the quiz has been taken the last !days days.', array('!days' => $end)),
      );
    }
  }

  /**
   * Generates a chart showing the status for all registered responses to a quiz
   *
   * @return string
   *   HTML to render to chart/graph
   */
  private function getQuizStatusChart() {
    // get the pass rate of the given quiz by vid
    $pass_rate = db_query("SELECT pass_rate "
      . " FROM {quiz_entity_revision} "
      . " WHERE vid = :vid", array(':vid' => $this->quiz_vid))->fetchField();
    if (!$pass_rate) {
      return;
    }

    // get the count value of results row above and below pass rate
    $quiz = db_query(
      "SELECT "
      . "   SUM(score >= $pass_rate) AS no_pass, "
      . "   SUM(score < $pass_rate) AS no_fail, "
      . "   SUM(is_evaluated = 0) AS no_incomplete "
      . " FROM {quiz_results} "
      . " WHERE quiz_vid = :vid", array(':vid' => $this->quiz_vid))->fetchAssoc();

    // no sufficient data
    if (($quiz['no_pass'] + $quiz['no_fail'] + $quiz['no_incomplete']) < 1) {
      return FALSE;
    }

    // generates quiz status chart 3D pie chart
    $chart = '<div id="get_quiz_status_chart" class="quiz-stats-chart-space">';
    $chart .= theme('get_quiz_status_chart', array('quiz' => $quiz));
    $chart .= '</div>';
    return array(
        'chart'       => $chart,
        'title'       => t('Status'),
        'explanation' => t('This chart shows the status for all attempts made to answer this revision of the quiz.'),
    );
  }

  /**
   * Generates the top scorers chart
   *
   * @return array
   *   array with chart and metadata
   */
  private function getQuizTopScorersChart() {
    $top_scorers = array();
    $sql = 'SELECT name, score FROM {quiz_results} qnr LEFT JOIN {users} u ON (u.uid = qnr.uid) WHERE quiz_vid = :vid';
    $arg[':vid'] = $this->quiz_vid;
    if ($this->uid) {
      $sql .= ' AND qnr.uid = :uid';
      $arg[':uid'] = $this->uid;
    }
    $sql .= ' ORDER by score DESC LIMIT 10';
    $results = db_query($sql, $arg);
    while ($result = $results->fetchAssoc()) {
      $key = $result['name'] . '-' . $result['score'];
      $top_scorers[$key] = $result;
    }

    if (!count($top_scorers)) {
      return FALSE;
    }

    $chart = theme('quiz_top_scorers', array('scorer' => $top_scorers));
    return array(
        'chart'       => '<div id="quiz_top_scorers" class="quiz-stats-chart-space">' . $chart . '</div>',
        'title'       => t('Top scorers'),
        'explanation' => t('This chart shows which quiz takers have the highest scores.'),
    );
  }

  /**
   * Generates grade range chart
   *
   * @return array
   *   array with chart and metadata
   */
  private function getQuizGradeRangeChart() {
    // @todo: make the ranges configurable
    $sql = 'SELECT SUM(score >= 0 && score < 20) AS zero_to_twenty,
              SUM(score >= 20 && score < 40) AS twenty_to_fourty,
              SUM(score >= 40 && score < 60) AS fourty_to_sixty,
              SUM(score >= 60 && score < 80) AS sixty_to_eighty,
              SUM(score >= 80 && score <= 100) AS eighty_to_hundred
            FROM {quiz_results}
            WHERE quiz_vid = :vid';
    $arg[':vid'] = $this->quiz_vid;
    if ($this->uid) {
      $sql .= ' AND uid = :uid';
      $arg[':uid'] = $this->uid;
    }
    $range = db_query($sql, $arg)->fetch();
    $count = $range->zero_to_twenty + $range->twenty_to_fourty + $range->fourty_to_sixty + $range->sixty_to_eighty + $range->eighty_to_hundred;
    if ($count < 2) {
      return FALSE;
    }

    // Get the charts
    $chart = theme('quiz_grade_range', array('range' => $range));

    // Return the chart with some meta data
    return array(
        'chart'       => '<div id="quiz_top_scorers" class="quiz-stats-chart-space">' . $chart . '</div>',
        'title'       => t('Distribution'),
        'explanation' => t('This chart shows the distribution of the scores on this quiz.'),
    );
  }

}
