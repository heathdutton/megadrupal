<?php

/**
 * @file course_relationships.course.inc
 * Holds course module specific hooks.
 */

/**
 * Outline callback for Course module.
 */
function course_relationships_outline($node, $user) {
  drupal_goto("node/$node->nid");
}

/**
 * Implements hook_course_handlers().
 */
function course_relationships_course_handlers() {
  return array(
    // Set an outline for now only so that we have something to select on the
    // parent. We can no longer check $node->course['lms'] == 'relationships'.
    'outline' => array(
      'relationships' => array(
        'name' => t('Relationships'),
        'description' => t('A parent course displaying a tree of children.'),
        // For now display the stock outline.
        'callback' => 'course_relationships_outline',
        'file' => 'course_relationships.course.inc',
        'file path' => drupal_get_path('module', 'course_relationships'),
      ),
    ),
    'object' => array(
      'course' => array(
        'name' => 'Course',
        'description' => 'Add a course to this course.',
        'class' => 'CourseObjectCourse',
      ),
    ),
  );
}

/**
 * Course settings handler callback.
 */
function course_relationships_settings_form() {
  $form = array();

  return system_settings_form($form);
}

/**
 * Implementation of hook_course_enrollment_alter().
 *
 * Apply the parent's enrollment end to the child.
 */
function course_relationships_course_enrollment_presave($enrollment) {
  // Apply parent enrollment end date to the child.
  $child_node = node_load($enrollment->nid);
  $co = course_get_course_object('course_relationships', 'course', $child_node->nid);
  $child_node->cr_parent = $co->getCourseNid();
  if ($parent_node = node_load($child_node->cr_parent)) {
    $parent_enrollment = course_enrollment_load($child_node->cr_parent, $enrollment->uid);
    if (!empty($parent_enrollment->enroll_end) && empty($child_node->course['duration'])) {
      // User has a parent enrollment with an end date.
      $enrollment->enroll_end = $parent_enrollment->enroll_end;
    }
    else if (!empty($parent_node->course['duration'])) {
      // User is not enrolled in parent (so end date not set yet), but parent
      // has a duration (enrolling from child).
      // @see course_relationships_course_enroll which runs after this and would
      // set the parent enrollment end. ie the two blocks here should have the
      // same effect.
      $enrollment->enroll_end = time() + $parent_node->course['duration'];
    }
  }
}

/**
 * Implementation of hook_course_enrollment_update().
 */
function course_relationships_course_enrollment_update($enrollment) {
  $parent_node = node_load($enrollment->nid);
  if (!empty($enrollment->enroll_end)) {
    // Loop over all the children and set their enrollment end dates.
    foreach (course_get_course($parent_node)->getObjects() as $courseObject) {
      // Trigger a save on all the children.
      if ($courseObject->getModule() == 'course_relationships') {
        $child_enrollment = course_enrollment_load($courseObject->getInstanceId(), $enrollment->uid);
        course_enrollment_save($child_enrollment);
      }
    }
  }
}
