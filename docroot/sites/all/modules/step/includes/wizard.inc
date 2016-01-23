<?php

/**
 * @file
 * CTools wizard implementation.
 */

/**
 * Registration wizard callback.
 */
function step_wizard($step = STEP_REGISTER) {
  ctools_include('wizard');
  ctools_include('object-cache');

  $form_info = array(
    'id' => 'step',
    'path' => 'user/register/%step',
    'show trail' => variable_get('step_show_trail', TRUE),
    'show back' => variable_get('step_show_back', FALSE),
    'return path' => variable_get('step_return_path', '<front>'),
    'next text' => t(variable_get('step_button_next', 'Continue')),
    'back text' => t(variable_get('step_button_back', 'Back')),
    'finish text' => t(variable_get('step_button_finish', 'Finish')),
    'forms' => step_get_steps(),
  );

  // Pass data to all wizard steps.
  $form_state[StepCache::CTOOLS_NAMESPACE] = StepCache::get('data');
  if ($step != STEP_REGISTER) {
    $form_state['user'] = user_load($form_state[StepCache::CTOOLS_NAMESPACE]['uid']);
  }

  // Return a CTools multi-step form.
  return ctools_wizard_multistep_form($form_info, $step, $form_state);
}

/**
 * Returns a list of form information suitable to be used in CTools wizard.
 *
 * @return array
 *   An associative array having step ID as keys and form data as values.
 */
function step_get_steps() {
  $return = array();

  foreach (variable_get('step_steps', step_default()) as $step) {
    $return[$step] = array(
      'title' => variable_get("step_title_$step", step_default_title($step)),
      'form id' => $step == STEP_REGISTER ? 'user_register_form' : "step_step_$step",
    );
  }

  return $return;
}

/**
 * CTools wizard step callback.
 */
function step_step($form, &$form_state) {
  // Hide back button when we're on the next step to 'register'. We cannot enter
  // again basic data of an user.
  if ($form_state['form_info']['show back'] && $form_state['previous'] == STEP_REGISTER) {
    unset($form['buttons']['previous']);
  }

  // Workaround CTools "Back" button submit behaviour. CTools doesn't allow
  // saving of data in the current step when hitting "Back" button. We just want
  // to save the values too, in this case.
  if ($form_state['form_info']['show back']) {
    unset($form['buttons']['previous']['#submit']);
    $form['buttons']['previous']['#limit_validation_errors'] = FALSE;
  }

  // Add description, if case.
  step_add_description($form, $form_state);

  return $form;
}

/**
 * Callback executed when the 'next' button is clicked.
 */
function step_next(&$form_state) {
  // Fill the cache only in the first step.
  if ($form_state['step'] == STEP_REGISTER) {
    StepCache::set('data', $form_state['values']);
  }
}

/**
 * Callback executed when the entire form submission is finished.
 *
 * @see hook_mail_alter()
 * @see step_mail_alter()
 * @see user_register_submit()
 */
function step_finish(&$form_state) {
  StepCache::clear('data');

  // Send now the registration E-mail.
  $account = $form_state['user'];

  // Set a flag along with the account, telling where the E-mail is originated.
  // This is used in hook_mail_alter() to allow the registration messages sent
  // from this point.
  // @see step_mail_alter()
  $account->step_finish = TRUE;

  // We are following here the logic from user_register_submit() but without
  // admin registering. We are also dropping post-submit redirection.
  // @see user_register_submit()

  // No e-mail verification required; log in user immediately.
  if (!variable_get('user_email_verification', TRUE) && $account->status) {
    _user_mail_notify('register_no_approval_required', $account);
    $form_state['uid'] = $account->uid;
    user_login_submit(array(), $form_state);
    drupal_set_message(t('Registration successful. You are now logged in.'));
  }
  // No administrator approval required.
  elseif ($account->status) {
    _user_mail_notify('register_no_approval_required', $account);
    drupal_set_message(t('A welcome message with further instructions has been sent to your e-mail address.'));
  }
  // Administrator approval required.
  else {
    _user_mail_notify('register_pending_approval', $account);
    drupal_set_message(t('Thank you for applying for an account. Your account is currently pending approval by the site administrator.<br />In the meantime, a welcome message with further instructions has been sent to your e-mail address.'));
  }
}
