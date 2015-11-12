<?php
/**
 * @file
 *  SSH ID module pages functions.
 */

/**
 * Returns the for for generating a new private key.
 */
function sshid_private_key_form($form, $form_state) {
  $form['intro'] = array(
    '#markup' => '<p>'
      . t('Generate a new encrypted SSH private key. If you have already generated a key, it will be overwritten. Please make sure to remember the password you choose.')
      . '</p><p>'
      . t('You wil be prompted to enter this password sometimes when you take an action which will result in code changes.')
      . '</p>',
  );

  $form['password'] = array(
    '#type' => 'password_confirm',
    '#title' => t('Choose a password'),
    '#description' => t('The new SSH private key will be encrypted with this password.'),
    '#required' => TRUE,
    '#size' => 25,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 10,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create key'),
  );

  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'user'),
  );

  return $form;
}

/**
 * Submit handler for sshid_private_key_form
 */
function sshid_private_key_form_submit($form, &$form_state) {
  $tmpdir = drupal_tempnam(file_directory_temp(), 'sshid');

  if (FALSE === $tmpdir) {
    drupal_set_message(t("Could not create temporary directory"), 'error');
    return FALSE;
  }
  drupal_unlink($tmpdir);
  drupal_mkdir($tmpdir);
  $shell = new Shell();

  try {
    $user = reset($form_state['build_info']['args']);
    $key = array(
      'private_key' => "$tmpdir/" . sshid_key_filename($user),
      'public_key' => "$tmpdir/" . sshid_key_filename($user) . '.pub',
    );

    // Generate ssh key pair
    $result = $shell->run('ssh-keygen', array('-t' => 'rsa', '-f' => $key['private_key'], '-N' => $form_state['values']['password']));

    if (FALSE === $result) {
      throw new RuntimeException($shell->getError());
    }

    if (preg_match('#([a-f0-9]{2}:){15}[a-f0-9]{2}#', $result, $matches)) {
      $key['fingerprint'] = $matches[0];
    }

    if (empty($key['fingerprint'])) {
      throw new RuntimeException(t('Could not get key fingerprint'));
    }

    drupal_set_message(t('New public / private key successfully generated.'));
    rules_invoke_event_by_args('generate_ssh_key_pair', array('ssh_key_pair' => $key, 'user' => $user));
  }
  catch (RuntimeException $e) {
    watchdog_exception('sshid', $e);
    drupal_set_message(t('Failed to generate the new SSH Private Key'), 'error');
  }

  // Remove the temp directory
  $shell->run('rm', array('-rf' => NULL), array($tmpdir));
}
