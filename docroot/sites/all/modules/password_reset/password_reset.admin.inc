<?php

/**
 * @file
 * password_reset_admin.inc contains forms and functions solely for use by those
 * with administrative permissions.
 */

/**
 * Menu callback: Display the settings form.
 */
function password_reset_settings_form() {
  $config = _password_reset_variable_get();

  $form['password_reset'] = array(
    '#tree' => TRUE
  );
  $form['password_reset']['registration_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ask question during registration'),
    '#default_value' => $config['registration_form'],
    '#description' => t('Users will be asked to select their security question and answer during registration.')
  );
  $form['password_reset']['redirect_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect users upon login'),
    '#default_value' => $config['redirect_user'],
    '#description' => t('Users who have <em>not</em> filled in their Password Reset question and answer will be directed to their user edit page upon login.')
  );
  $form['password_reset']['password_verify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require current password'),
    '#default_value' => $config['password_verify'],
    '#description' => t('Users will need to enter their current password in order to add or modify their password reset question in their profile page.')
  );
  $form['password_reset']['flood_control'] = array(
    '#type' => 'select',
    '#title' => t('Hourly threshold'),
    '#options' => range(0, 10),
    '#default_value' => $config['flood_control'],
    '#description' => t('The maximum number of times a user can attempt to reset the password in an hour. Set to 0 for unlimited attempts.')
  );
  $form['password_reset']['email_registration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the e-mail field during registration'),
    '#default_value' => $config['email_registration'],
    '#description' => t('Users will have to enter their e-mail address during registration. If unchecked, e-mail verification will be disabled and users will be asked to choose their password during registration.')
  );
  $form['password_reset']['email_profile'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the e-mail field in the user profile form.'),
    '#default_value' => $config['email_profile'],
    '#description' => t('The e-mail address field will be displayed on the user profile form. If checked and if this field is not displayed during registration, users will compusorily need to choose a valid e-mail address.')
  );

  return system_settings_form($form);
}

/**
 * Menu callback: List the security questions.
 */
function password_reset_admin_questions() {
  $result = db_query('SELECT pr.qid, pr.question, COUNT(pru.qid) as total
    FROM {password_reset} pr
    LEFT JOIN {password_reset_users} pru USING (qid)
    GROUP BY pr.qid
    ORDER BY question'
  );

  $rows = array();
  foreach ($result as $question) {
    $rows[] = array($question->question,
      $question->total,
      l(t('Edit'), 'admin/config/people/password-reset/edit/' . $question->qid, array('attributes' => array('title' => t('Edit this question.')))),
      l(t('Delete'), 'admin/config/people/password-reset/delete/' . $question->qid, array('attributes' => array('title' => t('Delete this question.')))),
      l(t('Empty'), 'admin/config/people/password-reset/empty/' . $question->qid, array('attributes' => array('title' => t('Empty all usage of this question.'))))
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No questions found.'), 'colspan' => 4));
  }

  $header = array(t('Question'), t('Use count'), array('data' => t('Operations'), 'colspan' => 3));

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Menu callback: Question edit page.
 */
function password_reset_admin_edit($form, &$form_state, $qid = NULL) {
  $form['question'] = array(
    '#type' => 'textfield',
    '#title' => t('Security question'),
    '#maxlength' => 255,
    '#description' => t("Example: 'What is your library card number?' or 'What is your mother's maiden name?'."),
    '#required' => TRUE
  );
  $form['answer_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum answer length'),
    '#maxlength' => 4,
    '#description' => t("The minimum length required for the answer."),
    '#default_value' => 1,
    '#required' => TRUE
  );
  $form['case_sensitive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Case sensitivity'),
    '#description' => t('Is the answer case sensitive?'),
    '#default_value' => 0
  );

  $url = l(t('PHP regular expressions'), 'http://www.php.net/manual/en/book.pcre.php');
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Regular expressions'),
    '#description' => t('!url provide a concise and flexible means for "matching" (specifying and recognizing) strings of text.', array('!url' => $url)),
    '#collapsible' => TRUE
  );
  $form['advanced']['regex'] = array(
    '#type' => 'textfield',
    '#title' => t('Regex validation string'),
    '#description' => t('Regular expression that needs to be matched for valid answers. For example, <em>/^\d{5}$/</em> will only allow numbers comprising exactly five digits. The / (or similar) delimiters are required.'),
    '#default_value' => ''
  );
  $form['advanced']['regex_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Regex error message'),
    '#description' => t('Error message to display when the regular expression check fails.')
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Add question'));

  if (arg(4) == 'edit' && isset($qid)) {
    $question = password_reset_question_get($qid, TRUE);

    // Set the default value of question properties.
    $form['question']['#default_value'] = $question->question;
    $form['answer_length']['#default_value'] = $question->answer_length;

    $form['advanced']['#collapsed'] = empty($question->regex);
    $form['advanced']['regex']['#default_value'] = $question->regex;
    $form['advanced']['regex_message']['#default_value'] = $question->regex_message;
    $form['case_sensitive']['#default_value'] = $question->case_sensitive;

    // Questions already being used cannot be edited completely.
    if ($question->total) {
      $form['answer_length']['#disabled'] = TRUE;
      $form['advanced']['regex']['#disabled'] = TRUE;
      $form['case_sensitive']['#disabled'] = TRUE;
    }

    $form['qid'] = array('#type' => 'value', '#value' => $qid);
    $form['submit']['#value'] = t('Update question');
  }

  return $form;
}

/**
 * Process the password_reset question edit page form submission.
 */
function password_reset_admin_edit_submit($form, &$form_state) {
  // Perform a test of the regex to inform the admin if it is invalid via PHP
  // notices (if enabled).
  if (!empty($form_state['values']['regex'])) {
    _password_reset_regex_match($form_state['values']['regex'], 'test string');
  }

  if (isset($form_state['values']['qid'])) {
    password_reset_question_add($form_state['values'], 'qid');

    // Redirect only updated question submissions back to the question list.
    $form_state['redirect'] = 'admin/config/people/password-reset';
  }
  else {
    password_reset_question_add($form_state['values']);
  }
}

/**
 * Menu callback: Question delete page.
 */
function password_reset_admin_delete($form, &$form_state, $qid) {
  if ($question = db_query("SELECT * FROM {password_reset} WHERE qid = :qid", array(':qid' => $qid))->fetchObject()) {
    $form['question'] = array('#type' => 'value', '#value' => $question->question);
    $form['qid'] = array('#type' => 'value', '#value' => $question->qid);

    return confirm_form(
      $form,
      t('Are you sure you want to delete %question?', array('%question' => $question->question)),
      'admin/config/people/password-reset',
      t('Users who have chosen this as their security question will have to update their profiles. This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }
}

/**
 * Process question delete form submission.
 */
function password_reset_admin_delete_submit($form, &$form_state) {
  password_reset_question_delete($form_state['values']);
  $form_state['redirect'] = 'admin/config/people/password-reset';
}

/**
 * Menu callback: Question empty page.
 */
function password_reset_admin_empty($form, &$form_state, $qid) {
  if ($question = password_reset_question_get($qid)) {
    $form['question'] = array('#type' => 'value', '#value' => $question->question);
    $form['qid'] = array('#type' => 'value', '#value' => $question->qid);

    return confirm_form(
      $form,
      t('Are you sure you want to empty all uses of the question: %question?', array('%question' => $question->question)),
      'admin/config/people/password-reset',
      t('Users who have chosen this as their security question will have to choose a question once again. This action cannot be undone.'),
      t('Empty'),
      t('Cancel')
    );
  }
}

/**
 * Process question delete form submission.
 */
function password_reset_admin_empty_submit($form, &$form_state) {
  password_reset_question_empty($form_state['values']);
  $form_state['redirect'] = 'admin/config/people/password-reset';
}

/**
 * Add / update a security question.
 *
 * @param Array $question
 *   An array containing question information formatted for drupal_write_record.
 * @param Array $update
 *   In case of an update, add the key, else leave empty. A single key can be
 *   passed as a string.
 * @param Boolean $notify
 *   Displays status messages to the user only if set to TRUE.
 * @return $status
 *   Status variable as per drupal_write_record.
 */
function password_reset_question_add($question, $update = array(), $notify = TRUE) {
  $status = drupal_write_record('password_reset', $question, $update);

  switch ($status) {
    case SAVED_NEW:
      $message = 'Password reset question %question added.';
      break;
    case SAVED_UPDATED:
      $message = 'Password reset question %question updated.';
      break;
  }

  $vars = array('%question' => $question['question']);
  watchdog('user', $message, $vars);
  if ($notify) {
    drupal_set_message(t($message, $vars));
  }

  return $status;
}
