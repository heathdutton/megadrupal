<?php
/**
 * @file
 * Performs file tracking, logging, and delivery.
 */

/**
 * Form constructor for the file download gatekeeper form.
 *
 * @see user_login_form_validate()
 * @see user_login_form_submit()
 * @ingroup forms
 */
function filefield_mail_download_form($form, &$form_state, $fid) {
  $form = array();
  global $user;

  // Non-anonymous users use account email.
  if ($user->uid) {
    filefield_mail_log($fid, $user->mail, $user->uid);
    filefield_mail_get_file($fid);
  }

  // Build a form with a field for the email address.
  $form['download']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Please enter your e-mail address to request your personal download link:'),
  );
  $form['download']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#attributes' => array('class' => array('form-submit')),
  );
  $form['download']['fid'] = array(
    '#type' => 'value',
    '#value' => $fid,
  );

  // Provide a link to the previous page if the user does not want the file.
  if (isset($_SESSION['filefield_mail_redirect']) && !url_is_external($_SESSION['filefield_mail_redirect'])) {
    $link = l(t('Return to the previous page'), $_SESSION['filefield_mail_redirect']);
    $form['download']['return'] = array(
      '#type' => 'item',
      '#markup' => '<p>' . $link . '</p>',
    );
  }
  return $form;
}

/**
 * Form validation handler for filefield_mail_download_form().
 *
 * @see filefield_mail_download_form_submit()
 */
function filefield_mail_download_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('download', t('Please enter a valid email address.'));
  }
}

/**
 * Form submission handler for filefield_mail_download_form().
 *
 * @see filefield_mail_download_form_validate()
 */
function filefield_mail_download_form_submit($form, &$form_state) {
  $fid = $form_state['values']['fid'];
  $email = $form_state['values']['email'];

  // Do not write a new record in case this email address has been used before.
  $record = db_query("SELECT * FROM {filefield_mail} WHERE email = :email", array(':email' => $email))
    ->fetchAssoc();

  if (empty($record)) {
    $record = array(
      'fid' => $fid,
      'email' => $email,
      'changed' => time(),
    );
    drupal_write_record('filefield_mail', $record);
  }

  // Call the statistics function.
  filefield_mail_log($fid, $email);

  // Send email and display confirmation.
  filefield_mail_send_mail($fid, $email);
  drupal_set_message(t('Thank you! You will shortly receive an e-mail with your personal download link.'), 'status', TRUE);

  // Go back to the page where the user clicked on the download link.
  if (isset($_SESSION['filefield_mail_redirect']) && !url_is_external($_SESSION['filefield_mail_redirect'])) {
    $form_state['redirect'] = array($_SESSION['filefield_mail_redirect']);
  }
}

/**
 * Updates the download record for a given file ID.
 */
function filefield_mail_log($fid, $email, $uid = 0) {
  $result = db_query('SELECT * FROM {filefield_mail} WHERE ((uid = 0 AND email = :email) OR (uid <> 0 AND uid = :uid)) AND fid = :fid', array(
    ':email' => $email,
    ':uid' => $uid,
    ':fid' => $fid,
    )
  );

  // Increment the counter, if the file has been downloaded by this user before.
  if ($record = $result->fetchAssoc()) {
    $primary_key = 'did';
    $record['count']++;
  }

  // Create a new record if the file hasn't been downloaded by this user before.
  else {
    $primary_key = array();
    $record = array(
      'uid' => $uid,
      'fid' => $fid,
      'email' => $email,
      'count' => 1,
    );
  }
  $record['changed'] = time();
  drupal_write_record('filefield_mail', $record, $primary_key);
}

/**
 * Send mail to the user with download link.
 */
function filefield_mail_send_mail($fid, $mail) {

  global $user;

  // Choose the language.
  $language = $user->uid ? user_preferred_language($user) : language_default();

  // The sender address.
  $from = variable_get('site_mail', NULL);

  // Read file information.
  $file = db_query('SELECT * FROM {file_managed} WHERE fid = :fid', array(':fid' => $fid))->fetchAssoc();

  // Build the body. Sanitize it to prevent dangerous HTML.
  $download_path = file_create_url($file['uri']);

  $params = array(
    'From' => $from,
    'download_path' => $download_path,
  );

  // Send the mail.
  drupal_mail('filefield_mail', 'confirmation', $mail, $language, $params, $from);
}

/**
 * Implements hook_mail().
 */
function filefield_mail_mail($key, &$message, $params) {
  switch ($key) {
    case 'confirmation':
      // Build the subject. Make sure it contains no html.
      $mail_subject = variable_get('filefield_mail_subject', '');
      $mail_subject = filter_xss($mail_subject, array());
      $message['subject'] = t($mail_subject);

      $allowed_tags = array(
        'h1', 'h2', 'h3', 'h4', 'h5', 'h6', 'em', 'strong', 'code', 'del',
        'img', 'blockquote', 'q', 'cite', 'sup', 'sub', 'p', 'br', 'ul', 'ol',
        'li', 'dl', 'dt', 'dd', 'a', 'b', 'u', 'i', 'table', 'tr', 'td', 'th',
        'tbody', 'thead', 'tfoot', 'colgroup', 'caption', 'hr');

      $mail_text_before_link = variable_get('filefield_mail_text_before_link', '');
      $mail_text_before_link = filter_xss($mail_text_before_link, $allowed_tags);

      $mail_text_after_link = variable_get('filefield_mail_text_after_link', '');
      $mail_text_after_link = filter_xss($mail_text_after_link, $allowed_tags);

      $body = t($mail_text_before_link) . '<br />';
      $body .= $params['download_path'];
      $body .= '<br />' . t($mail_text_after_link);
      $message['body'][] = $body;

      $message['from'] = $params['From'];
      break;
  }
}

/**
 * Provides the non-anonymous user with a file download, given a valid file ID.
 */
function filefield_mail_get_file($fid) {
  $file = db_query('SELECT * FROM {file_managed} WHERE fid = :fid', array(':fid' => $fid))->fetchAssoc();
  $headers = array(
    'Pragma' => 'hack',
    'Cache-Control' => 'public, must-revalidate',
    'Content-Type' => $file['filemime'],
    'Content-Disposition' => 'attachment',
    'Content-Transfer-Encoding' => 'binary',
  );
  file_transfer($file['uri'], $headers);
}
