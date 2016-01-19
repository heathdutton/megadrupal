<?php
/**
 * @file lessons_lesson.theme.inc
 * Theme Lesson nodes provided by Lessons module.
 */

/**
 * Impliments hook_theme().
 */
function lessons_lesson_theme($existing, $type, $theme, $path) {
  $items = array(
    'node__lesson' => array(
      'render element' => 'elements',
      'path' => drupal_get_path('module', 'lessons_lesson') ."/theme",
      'template' => 'node--lesson',
    ),
  );

  return $items;
}
