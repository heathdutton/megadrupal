<?php

/**
 * @file
 * File zipalerts_for_non_recruiter.features.inc.
 */

/**
 * Implements hook_node_info().
 */
function zipalerts_for_non_recruiter_node_info() {
  $items = array(
    'job_per_template' => array(
      'name' => t('Job per template'),
      'base' => 'node_content',
      'description' => t('Create a new job and provide the job details now.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
