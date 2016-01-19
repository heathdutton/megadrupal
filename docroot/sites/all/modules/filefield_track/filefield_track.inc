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
function filefield_track_download_form($form, &$form_state, $fid) {
  $form = array();

  global $user;

  // Non-anonymous users use account email.
  if ($user->uid) {
    filefield_track_log($fid, $user->mail, $user->uid);
    filefield_track_get_file($fid);
  }
  elseif (!empty($_COOKIE['fft_auth'])) {
    list($email, $auth) = explode("|", $_COOKIE['fft_auth']);
    $result = db_query("SELECT * FROM {filefield_track} WHERE email = :email AND auth = :auth", array(':email' => $email, ':auth' => $auth));
    $auth_obj = $result->fetchObject();
    if ($auth_obj != NULL && $auth_obj->email == $email) {
      filefield_track_log($fid, $email);
      filefield_track_get_file($fid);
    }
  }

  $form['download']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Please provide a valid email address to download this file'),
  );
  $form['download']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['download']['fid'] = array(
    '#type' => 'value',
    '#value' => $fid,
  );
  if (isset($_GET['redirect']) && !url_is_external($_GET['redirect'])) {
    $link = l(t('Return to the previous page'), $_GET['redirect']);
    $form['download']['return'] = array(
      '#type' => 'item',
      '#markup' => '<p>' . $link . '</p>',
    );
  }

  return $form;
}

/**
 * Form validation handler for filefield_track_download_form().
 *
 * @see filefield_track_download_form_submit()
 */
function filefield_track_download_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('download', t('Please enter a valid email address.'));
  }
}

/**
 * Form submission handler for filefield_track_download_form().
 *
 * @see filefield_track_download_form_validate()
 */
function filefield_track_download_form_submit($form, &$form_state) {
  $fid = $form_state['values']['fid'];
  $email = $form_state['values']['email'];

  // Just in case email has been used before but cookie was deleted.
  $record = db_query("SELECT * FROM {filefield_track} WHERE email = :email", array(':email' => $email))
    ->fetchAssoc();

  if (empty($record)) {
    $record = array(
      'fid' => $fid,
      'email' => $email,
      'auth' => md5(uniqid("")),
      'changed' => time(),
    );
    drupal_write_record('filefield_track', $record);
  }
  setcookie("fft_auth", $email . "|" . $record['auth'], strtotime('+10 years'));

  filefield_track_log($fid, $email);
  filefield_track_get_file($fid);
}


/**
 * Creates a download record for a given file ID.
 */
function filefield_track_log($fid, $email, $uid = 0) {
  $result = db_query('SELECT * FROM {filefield_track} WHERE ((uid = 0 AND email = :email) OR (uid != 0 AND uid = :uid)) AND fid = :fid', array(
    ':email' => $email,
    ':uid' => $uid,
    ':fid' => $fid,
    )
  );

  if ($record = $result->fetchAssoc()) {
    $update = 'did';
    $record['count']++;
  }
  else {
    $update = array();
    $record = array(
      'uid' => $uid,
      'fid' => $fid,
      'email' => $email,
    );
  }
  $record['changed'] = time();
  drupal_write_record('filefield_track', $record, $update);
}

/**
 * Provides the user with a file download, given a valid file ID.
 */
function filefield_track_get_file($fid) {
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
