<?php
/**
 * @file
 * File for common configuration methods.
 */

/**
 * Function to implement configuration form.
 */
function form_submission_timeout_configuration($form, &$form_state) {
  $form['sub_out_countdown'] = array(
    '#markup' => '<div>' . l(t('Submission Countdown'), 'admin/config/development/form_submission_timeout/countdown') . '</div>'
  );
  $form['sub_out_countdown_description'] = array(
    '#markup' => '<div>You can time each and every form on the drupal site. If user does not submit a form within a timeout period, the form will have to be either filled out again, or the page will have to be refreshed in order to make a successful form submission</div>',
  );

  $form['sub_out_timed'] = array(
    '#markup' => '<div>' . l(t('Timed Submission'), 'admin/config/development/form_submission_timeout/timed') . '</div>'
  );
  $form['sub_out_timed_description'] = array(
    '#markup' => '<div>You can keep a time limitation on a form. You can decide when will the submission on a form can start or end in a day, week, or a month. Basically, you can decide when does a particular form gets activated.</div>',
  );
  return $form;
}

/**
 * Function to remove a form_id from settings form.
 */
function form_submission_timeout_remove_formid($name, $form_id, $goto) {
  _form_submission_timeout_update_form_ids($name, $form_id, 'remove');
  drupal_goto('admin/config/development/form_submission_timeout/' . $goto);
}

/**
 * Function to reset form_id timer in $_SESSION.
 */
function form_submission_timeout_reset_timer() {
  $form_id = str_replace('-', '_', $_REQUEST['form_id']);
  $_SESSION['form_submission_timeout_form_ids'][$form_id . '_session_start']['status'] = 'reset';
}
