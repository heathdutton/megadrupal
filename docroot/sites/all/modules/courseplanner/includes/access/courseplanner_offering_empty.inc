<?php

/**
 * @file
 * Plugin to provide access control/visibility based on whether a course
 * offering has lessons attached to it or not.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Course planner: Course offering is empty'),
  'description' => t('Control access based on whether a course offering has lessons or not.'),
  'callback' => 'courseplanner_offering_empty_check',
  'summary' => 'courseplanner_offering_empty_summary',
  'required context' => new ctools_context_required(t('Course offering'), 'node'),
);

/**
 * Access check for 'Course offering is empty'.
 */
function courseplanner_offering_empty_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data)) {
    return FALSE;
  }

  // Get the course offering and verify the content type.
  $offering = &$context->data;
  if ($offering->type != 'cp_offering') {
    return FALSE;
  }

  return (count($offering->cp_offering_lessons) == 0);
}

/**
 * Provide a summary of this access condition.
 */
function courseplanner_offering_empty_summary($conf, $context) {
  return t('Node is course offering and has at least one lesson.');
}
