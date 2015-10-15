<?php

/**
 * @file
 * Defines a rules action for granting access to a course by modifying
 * the course access entity.
 */

/**
 * Implements hook_rules_action_info().
 */
function elearning_rules_action_info() {
  return array(
    'elearning_action_course_grant_access' => array(
      'label' => t('Grant user access to this course'),
      'parameter' => array(
        'elearning_course_access' => array(
          'label' => t('Course Access'),
          'type' => 'elearning_course_access',
          'optional' => TRUE,
          'allow null' => TRUE,
        ),
        'elearning_course' => array(
          'label' => t('Course'),
          'type' => 'elearning_course',
          'optional' => TRUE,
          'allow null' => TRUE,
        ),
        'user' => array(
          'label' => t('User'),
          'type' => 'user',
          'optional' => TRUE,
          'allow null' => TRUE,
        ),
      ),
      'group' => t('E-learning'),
    ),
    'elearning_action_course_deny_access' => array(
      'label' => t('Deny user access to this course'),
      'parameter' => array(
        'elearning_course_access' => array(
          'label' => t('Course Access'),
          'type' => 'elearning_course_access',
          'optional' => TRUE,
          'allow null' => TRUE,
        ),
        'elearning_course' => array(
          'label' => t('Course'),
          'type' => 'elearning_course',
          'optional' => TRUE,
          'allow null' => TRUE,
        ),
        'user' => array(
          'label' => t('User'),
          'type' => 'user',
          'optional' => TRUE,
          'allow null' => TRUE,
        ),
      ),
      'group' => t('E-learning'),
    ),
  );
}

/**
 * Rules Callback to grant access to a course.
 */
function elearning_action_course_grant_access($course_access, $course = NULL, $user = NULL) {

  $success = elearning_action_course_modify_access(TRUE, $course_access, $course, $user);
  if (!$success) {
    $access_wrapper = entity_metadata_wrapper('elearning_course_access', $course_access);
    $account = $access_wrapper->uid->value();
    drupal_set_message(t("User %user already has access to this course.", array(
          "%user" => format_username($account),
        )), 'warning');
  }
}

/**
 * Rules Callback to deny access to a course.
 */
function elearning_action_course_deny_access($course_access, $course, $user) {
  $success = elearning_action_course_modify_access(FALSE, $course_access, $course, $user);
  if (!$success) {
    $access_wrapper = entity_metadata_wrapper('elearning_course_access', $course_access);
    $account = $access_wrapper->uid->value();
    drupal_set_message(t("User %user was already denied access to this course.", array(
          "%user" => format_username($account),
        )), 'warning');
  }
}

/**
 * Helper function for denying or granting access to a course.
 */
function elearning_action_course_modify_access($op, $course_access = NULL, $course = NULL, $user = NULL) {

  if (!$course_access) {
    if ($course && $user) {
      $results = db_select('elearning_course_access', 'ca')->fields('ca', array('course_access_id'))->condition('uid', $user->uid)->condition('course_id', $course->course_id)->execute()->fetchAssoc();

      $course_access_id = end($results);
      $course_access = entity_load_single('elearning_course_access', $course_access_id);
    }
    else {
      drupal_set_message(t("Both the user and the course need to be passed as
        variables if the course access entity isn't present."), 'error');
      exit;
    }
  }

  $access_wrapper = entity_metadata_wrapper('elearning_course_access', $course_access);

  if ($access_wrapper->status->value() == !$op) {
    $access_wrapper->status = $op;
    if ($op) {
      $access_wrapper->granted = time();
    }
    $access_wrapper->save();
    return TRUE;
  }
  else {
    return FALSE;
  }
}
