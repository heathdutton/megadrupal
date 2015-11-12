<?php

use Drupal\quizz\Entity\QuizEntity;
use Drupal\quizz_stats\Controller\RevisionListingController;
use Drupal\quizz_stats\Controller\RevisionStatsController;

/**
 * @file
 * Administration file for Quiz stats Module
 */

/**
 * Page for selecting between several quiz revisions
 *
 * Callback for /quiz/%quizz/statistics
 *
 * @return string
 */
function quizz_stats_page(QuizEntity $quiz) {
  $obj = new RevisionListingController($quiz);
  return $obj->render();
}

function quizz_stats_revision_page($quiz_vid, $uid = 0) {
  $obj = new RevisionStatsController($quiz_vid, $uid);
  return $obj->render($quiz_vid, $uid);
}

/**
 * Returns statistics for all available quizzes
 *
 * @param int $uid
 *   User id
 * @return string
 *   HTML table
 */
function quizz_stats_get_basic_stats($uid = 0) {
  return theme('quizz_stats_get_basic_stats', array(
      'results' => quizz_entity_controller()->getStats()->getQuizzesByUserId($uid)
  ));
}

/**
 * Computes quiz attempt count
 *
 * @param int $quiz_id
 * @param int $uid
 * @return int
 *   Integer containing number of times that a quiz has
 *   been attended, can be filtered by uid.
 */
function _quiz_get_attempt_count($quiz_id, $uid = 0) {
  $sql = 'SELECT COUNT(result_id) FROM {quiz_results} WHERE quiz_qid = :qid';
  $arg = array(':qid' => $quiz_id);
  if ($uid) {
    $sql .= ' AND uid = :uid';
    $arg[':uid'] = $uid;
  }
  return db_query($sql, $arg)->fetchField();
}

// Theme functions

/**
 * Theme function for quiz grade range chart generates Chart using CHART API function
 *
 * @param $range
 *   array containg quiz results data structure
 *
 * @return
 *   HTML to render/display chart
 */
function theme_quiz_grade_range($variables) {
  $range = $variables['range'];

  $count = array_sum((array) $range);

  $chart = array(
      '#type'       => 'chart',
      '#chart_type' => 'column',
      '#title'      => t('Score distribution'),
  );
  $chart['range'] = array(
      '#type'  => 'chart_data',
      '#title' => t('% of quiz takers'),
      '#data'  => array(
          round(($range->zero_to_twenty / $count) * 100),
          round(($range->twenty_to_fourty / $count) * 100),
          round(($range->fourty_to_sixty / $count) * 100),
          round(($range->sixty_to_eighty / $count) * 100),
          round(($range->eighty_to_hundred / $count) * 100),
      ),
  );
  $chart['xaxis'] = array(
      '#title'  => t('Score'),
      '#type'   => 'chart_xaxis',
      '#labels' => array('0-20%', '20-40%', '40-60%', '60-80%', '80-100%'),
  );
  $chart['yaxis'] = array(
      '#title' => t('Grade istribution'),
      '#type'  => 'chart_yaxis',
      '#max'   => 100,
  );
  return drupal_render($chart);
}

/**
 * generates a chart of quiz top scorers
 *
 * @param $attendees
 *   array containing quiz data structure
 *
 * @return
 *   HTML to render quiz top scorers chart
 */
function theme_quiz_top_scorers($variables) {
  $attendees = array_reverse($variables['scorer']);

  $chart = array(
      '#type'       => 'chart',
      '#title'      => t('Quiz Top Scorers'),
      '#chart_type' => 'column',
  );
  $chart['bar_data'] = array(
      '#type'  => 'chart_data',
      '#title' => 'Result',
  );
  foreach ($attendees as $attendee) {
    $chart['bar_data']['#data'][] = (float) $attendee['score'];
    $labels[] = $attendee['name'] . ' ' . $attendee['score'] . '%';
  }
  $chart['yaxis'] = array(
      '#title' => t('Score') . ' (%)',
      '#type'  => 'chart_yaxis',
      '#max'   => 100,
  );
  $chart['xaxis'] = array(
      '#title'  => 'Quiz Attendees',
      '#type'   => 'chart_xaxis',
      '#labels' => $labels,
  );
  return drupal_render($chart);
}

/**
 * generates quiz status chart number of pass, fail, incomplete
 *
 * @param $quiz
 *   array of quiz data structure
 *
 * @return
 *   HTML to render quiz status chart
 */
function theme_get_quiz_status_chart($variables) {
  $quiz = $variables['quiz'];
  $quiz['total'] = $quiz['no_pass'] + $quiz['no_fail'] + $quiz['no_incomplete'];

  $chart = array(
      '#type'       => 'chart',
      '#title'      => t('Quiz results'),
      '#chart_type' => 'pie',
  );

  $chart['pie_data'] = array(
      '#type'   => 'chart_data',
      '#title'  => t('Result'),
      '#labels' => array('Pass', 'Incomplete', 'Fail'),
      '#data'   => array(
          (int) $quiz['no_pass'],
          (int) $quiz['no_incomplete'],
          (int) $quiz['no_fail']
      ),
  );
  return drupal_render($chart);
}

/**
 * Generates date vs takeup count chart
 *
 * @param $takeup
 *   Array quiz data structure
 * @return
 *   HTML to render/display chart
 */
function theme_date_vs_takeup_count($variables) {
  $chart = array(
      '#type'       => 'chart',
      '#title'      => t('Activity by date'),
      '#chart_type' => 'line',
  );

  $chart['line_data'] = array(
      '#type'   => 'chart_data',
      '#title'  => t('Result'),
      '#labels' => array('Something'),
  );

  $chart['line_data']['#data'] = array();

  $labels = array();
  foreach ($variables['takeup'] as $point) {
    $chart['line_data']['#data'][] = (int) $point->count;
    $labels[] = $point->date;
  }

  $chart['xaxis'] = array('#type' => 'chart_xaxis', '#labels' => $labels);
  $chart['yaxis'] = array('#title' => 'Participants', '#type' => 'chart_yaxis');
  return drupal_render($chart);
}

/**
 * Generates table of results from quiz data structure.
 *
 * @param @results
 *   Array containing quiz results data structure
 * @return
 *   HTML table containing quiz title, author, questions count, attempt count
 */
function theme_quizz_stats_get_basic_stats($variables) {
  module_load_include('inc', 'quiz', 'quiz.pages');

  $results = $variables['results'];
  if (empty($results)) {
    return '<p>' . t('No questions were found.') . '</p>';
  }

  $rows = array();
  $header = array('title' => t('Quiz'), 'creator' => t('Author'), 'created' => t('Created'));
  if (user_access('access author stats')) {
    $header['questions_count'] = t('Questions');
    $header['attempt_count'] = t('Attempts');
  }

  $chart_icon = theme('image', array(
      'path'   => drupal_get_path('module', 'quizz_stats') . '/chart.png',
      'width'  => t('Charts'),
      'height' => t('See charts')
  ));
  foreach ($results as $result) {
    $title_link = user_access('access author stats') ? 'quiz/' . $result['qid'] . '/statistics' : 'user/' . arg(1) . '/stats/';
    $row = array(
        'title'   => l($chart_icon . ' ' . $result['title'], $title_link, array('html' => TRUE)),
        'creator' => l($result['name'], 'user/' . $result['uid']),
        'created' => format_date($result['created'], 'short'),
    );
    if (user_access('access author stats')) {
      $row['questions_count'] = quizz_entity_controller()->getStats()->countAllQuestions($result['vid']);
      $row['attempt_count'] = _quiz_get_attempt_count($result['qid']);
    }
    $rows[] = $row;
  }
  $cc = '<em>' . t('Chart icon from !url', array('!url' => 'pinvoke.com')) . '</em>';
  return theme('table', array(
        'header'     => $header,
        'rows'       => $rows,
        'attributes' => array('id' => 'tablesorter')
    )) . $cc;
}
