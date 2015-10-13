<?php

/**
 * Page callback for SSH settings.
 *
 * @see ssh_helper_menu()
 */
function ssh_helper_settings($form, &$form_state) {
  $ssh_helper = new sshHelper();
  if (variable_get('ssh_helper_public_key', '')) {
    $form['ssh_helper_public_key'] = array(
      '#type' => 'item',
      '#title' => 'Copy this public key and add to remote servers',
      '#markup' => '<span style="word-wrap: break-word;">' . variable_get('ssh_helper_public_key', '') .'</span>',
    );
  }
  $form['ssh_helper_file_private_key_path'] = array(
    '#type' => 'textfield',
    '#title' => 'Path where the private key should be stored',
    '#default_value' => $ssh_helper->getKeyPath(),
    '#description' => t('Save the values first before generating the key pair'),
  );
  $form['ssh_helper_key_filename'] = array(
    '#type' => 'textfield',
    '#title' => 'Filename of the key. Becomes id_rsa_ssh_helper.pub etc...',
    '#description' => t('Save the values first before generating the key pair'),
    '#default_value' => variable_get('ssh_helper_key_filename', 'id_rsa_ssh_helper'),
  );
  $form['ssh_helper_key_comment'] = array(
    '#type' => 'textfield',
    '#title' => 'Comment that is added at the end of the key to identify the key with',
    '#description' => t('Save the values first before generating the key pair'),
    '#default_value' => variable_get('ssh_helper_key_comment', 'ssh_helper@drupal'),
  );
  $form['ssh_helper_username'] = array(
    '#type' => 'textfield',
    '#title' => 'User to ssh with. Not required but can be used as a default username',
    '#description' => t('Save the values first before generating the key pair'),
    '#default_value' => variable_get('ssh_helper_username', NULL),
  );
  $form['actions']['regenerate'] = array(
    '#type' => 'submit',
    '#submit' => array('ssh_helper_regenerate_keys_form_submit'),
    '#value' => t('Regenerate ssh key pair'),
    '#weight' => 2,
  );
  return system_settings_form($form);
}

/**
 * Form submit callback to regenerate the ssh keys.
 */
function ssh_helper_regenerate_keys_form_submit() {
  $ssh_helper = new sshHelper();
  $ssh_helper->generateKeypair();
  $private_path = $ssh_helper->getKeyPath();
  $t_args = array('%private_path' => drupal_realpath($private_path));
  $message = t("Regenerated Keys. Please install the public key to all servers. The private key is stored at %private_path.", $t_args);
  drupal_set_message($message);
}
