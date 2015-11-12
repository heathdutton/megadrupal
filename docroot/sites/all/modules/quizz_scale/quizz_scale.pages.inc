<?php

use Drupal\quizz_scale\Form\ConfigForm\FormSubmit;

/**
 * Handles the scale collection form.
 */
function quizz_scale_manage_collection_form_submit($form, &$form_state) {
  $obj = new FormSubmit();
  return $obj->submit($form, $form_state);
}

/**
 * Validates the scale collection form
 */
function quizz_scale_manage_collection_form_validate($form, &$form_state) {
  // If the user is trying to create a new collection
  if (drupal_strlen($form_state['values']['configuration']['collections']['collectionnew']['alternative0']) > 0) {
    // If the new collection don't have two alternatives
    if (!drupal_strlen($form_state['values']['configuration']['collections']['collectionnew']['alternative1'])) {
      // This can't be replaced by adding #required to the form elements. If we
      // did so we would always have to create a new collection when we press submit
      form_error($form['configuration']['collections']['collectionnew']['alternative1'], t('New preset must have atleast two alternatives.'));
    }
  }
}

/**
 * Validate the scale config form values
 */
function quizz_scale_config_validate($form, $form_state) {
  if (!quizz_valid_integer($form_state['values']['configuration']['scale_max_num_of_alts'], 2, 50)) {
    form_set_error('scale_max_num_of_alts', t('The number of alternatives must be between 2 and 50'));
  }
}
