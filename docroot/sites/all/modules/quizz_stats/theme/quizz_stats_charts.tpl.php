<?php

/**
 * @file
 * Theming the charts page
 *
 * Variables available:
 * $charts (array)
 *
 * The following charts are available:
 *  $charts['top_scorers'] (array or FALSE if chart doesn't exist)
 *  $charts['takeup'] (array or FALSE if chart doesn't exist)
 *  $charts['status'] (array or FALSE if chart doesn't exist)
 *  $charts['grade_range'] (array or FALSE if chart doesn't exist)
 *
 * Each chart has a title, an image and an explanation like this:
 * $charts['top_scorers']['title'] (string)
 * $charts['top_scorers']['chart'] (string - img tag - google chart)
 * $charts['top_scorers']['explanation'] (string)
 */
global $chart_found;

if (!function_exists('__quizz_stats_chart')) {

  function __quizz_stats_chart($chart) {
    global $chart_found;

    if (!$chart) {
      return;
    }

    $chart_found = TRUE;
    return '<h2 class="quiz-charts-title">' . $chart['title'] . '</h2>'
      . $chart['chart']
      . $chart['explanation'];
  }

}

echo __quizz_stats_chart($charts['takeup']);
echo __quizz_stats_chart($charts['top_scorers']);
echo __quizz_stats_chart($charts['status']);
echo __quizz_stats_chart($charts['grade_range']);

if (empty($chart_found)) {
  echo '<p>' . t('If there are no statistics for this quiz (or quiz revision), this is probably because nobody has yet run this quiz (or quiz revision). If the quiz has multiple revisions, it is possible that the other revisions do have statistics. If this is the last revision, taking the quiz should generate some statistics.') . '</p>';
}
