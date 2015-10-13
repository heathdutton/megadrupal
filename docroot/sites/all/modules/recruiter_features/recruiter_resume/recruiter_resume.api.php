<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @addtogroup hooks
 * @{
 */


/**
 * Control view access to private fields on a resume profile
 *
 * Modules may implement this hook if they want to influence whether or not a
 * given user has view access on the private field on the resume.
 *
 * The owner of a profile has always access and this can not be altered with
 * this hook.
 *
 * Before the field access system is invoked, access to the resume profile will
 * be checked. Only visible resumes can be accessed by recruiter users.
 *
 * @param $field
 *   The private field for which the access should be determined.
 * @param $profile
 *   The resume profile the field is attached to.
 * @param $account
 *   The current acting user.
 * @return boolean
 *   Return TRUE to grant access, FALSE to explicitly deny access. Return NULL
 *   or nothing to not affect the operation.
 *   Access is granted as soon as a module grants access and no one denies
 *   access. Thus if no module explicitly grants access, access will be denied.
 *
 * @see recruiter_resume_private_fields_view_access()
 */
function hook_recruiter_resume_private_fields_view_access($field, $profile, $account) {
  // E.g., always allow recruiters view access to private field on visible
  // resume profiles.
  return TRUE;
}

/**
 * Alter the default text that is displayed to applicants for their private
 * resume fields.
 *
 * Modules may implement this hook in order to adapt the text to their
 * functionality (e.g. an application feature may add, that it is visible to
 * those who received your job application).
 *
 * @param $text
 *   The default text that can be altered.
 * @param $field_name
 *   The Field API's field name.
 *
 * @see recruiter_resume_private_fields_info_text().
 */
function hook_recruiter_resume_private_fields_info_text_alter(&$text, $field_name) {
  // Add some more information.
  $text .= ' ' . t('It can only be seen by you and your invited recruiters.');
}

/**
 * @}
 */

