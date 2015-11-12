<?php
/**
 * @file
 * best_answer.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function best_answer_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'best_answer_lock_question_p';
  $strongarm->value = 0;
  $export['best_answer_lock_question_p'] = $strongarm;

  return $export;
}
