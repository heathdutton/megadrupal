<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard Drupal
 * manner.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter all wizards forms from a single point.
 *
 * This is basically the classic hook_form_BASE_FORM_ID_alter(). The base form
 * ID for all forms, except the first one ('user_register_form') is 'step_step'.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @see hook_form_BASE_FORM_ID_alter()
 */
function hook_form_step_step_alter(&$form, &$form_state) {
  $message = t('See <a href="/terms">Terms & Conditions</a>');
  $form['profile2_register_description']['#markup'] .= ' ' . $message;
}

/**
 * Alter a single wizard step form.
 *
 * This is basically the classic hook_form_FORM_ID_alter(). The name of a
 * specific wizard form follows the pattern: "step_step_$step", where $step is
 * the step ID. This hook can be applied to all the steps except except the
 * first one ('user_register_form'). In this case the form ID is
 * 'step_step_photo'.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @see hook_form_FORM_ID_alter()
 */
function hook_form_step_step_photo_alter(&$form, &$form_state) {
  $form['credits'] = array(
    '#type' => 'textarea',
    '#title' => t('Credits'),
  );
}

/**
 * @}
 */
