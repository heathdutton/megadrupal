<?php

/**
 * @file
 * Hooks provided by the Comment Score module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implements hook_comment_score_text_alter().
 *
 * Allows you to alter the score for a particular comment, based on arbitrary
 * rules you define.
 *
 * In this example we are going to add negativity points when the comment is
 * originally submitted between the hours of 2am -> 6am (local time).
 *
 * @param Integer $score
 *   The current score of the comment based on other modules rules.
 * @param Array $score_debug
 *   If your rule matches you should add a unique key and message to this array
 *   to inform administrators why you changed the score and by how much.
 * @param Object $author
 *   The account object of the original author. Changes to this object are not
 *   saved.
 */
function hook_comment_score_text_alter(&$score, &$score_debug, &$author) {
  $now = new DateTime();
  $current_hour = (int) $now->format('m');
  if ($current_hour >= 2 && $current_hour < 6) {
    $score += 25;
    $score_debug['time'] = t('Comment submitted during the hours of 2am to 6am.');
  }
}

/**
 * @} End of "addtogroup hooks".
 */
