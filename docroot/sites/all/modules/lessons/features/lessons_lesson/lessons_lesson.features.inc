<?php
/**
 * @file
 * lessons_lesson.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function lessons_lesson_ctools_plugin_api() {
  return array("version" => "1");
}

/**
 * Implements hook_node_info().
 */
function lessons_lesson_node_info() {
  $items = array(
    'lesson' => array(
      'name' => t('Lesson'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
