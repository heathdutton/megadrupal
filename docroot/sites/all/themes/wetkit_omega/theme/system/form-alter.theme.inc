<?php
/**
 * @file
 * form-alter.theme.inc
 */


/**
 * Alter the forms.
 */
function wetkit_omega_form_alter(&$form, &$form_state, $form_id) {
  if (!empty($form['actions']) && isset($form['actions']['submit'])) {
    $form['actions']['submit']['#attributes']['class'][] = 'gcwu-srch-submit button';
  }
}
