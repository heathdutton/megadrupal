<?php
/**
 * @file
 * Admin forms and functions for ssh module.
 */

/**
 * Settings form callback.
 */
function ssh_settings($form, &$form_state) {

  $form = array(
    'ssh_executable' => array(
      '#type' => 'textfield',
      '#title' => t('SSH executable'),
      '#description' => t('The path to the ssh executable on the Drupal host.'),
      '#default_value' => variable_get_value('ssh_executable'),
      '#required' => TRUE,
    ),
    'ssh_default_port' => array(
      '#type' => 'textfield',
      '#title' => t('SSH default port'),
      '#description' => t('Use this port if none is specified for a host.'),
      '#default_value' => variable_get_value('ssh_default_port'),
      '#required' => TRUE,
    ),
    'ssh_keyfile' => array(
      '#type' => 'textfield',
      '#title' => t('SSH keyfile'),
      '#description' => t('The path to the ssh private key file to be used by Drupal.'),
      '#default_value' => variable_get_value('ssh_keyfile'),
      '#required' => TRUE,
    ),
    'ssh_hostsfile' => array(
      '#type' => 'textfield',
      '#title' => t('SSH hosts keyfile'),
      '#description' => t('The path to the ssh known hosts file to be used by Drupal. Note that the remote host key must already be in there.'),
      '#default_value' => variable_get_value('ssh_hostsfile'),
      '#required' => TRUE,
    ),
  );
  return system_settings_form($form);
}

/**
 * Form validator.
 */
function ssh_settings_validate($form, &$form_state) {

  $v = &$form_state['values'];

  // Check executable.
  $ssh = new SSH(
    $v['ssh_executable'],
    $v['ssh_keyfile'],
    $v['ssh_hostsfile']
  );
  if (!$ssh->ok()) {
    form_set_error(
      'ssh_executable',
      t('The path %path does not resolve to an executable.', array(
        '%path' => $ssh->getExecutable(),
      ))
    );
  }
  if (!$ssh->keyfileOk()) {
    form_set_error(
      'ssh_keyfile',
      t('%path is not a valid keyfile or has the wrong permissions.', array(
        '%path' => $ssh->getKeyfile(),
      ))
    );
  }
  if (!$ssh->hostsFileOk()) {
    drupal_set_message(t('%path is not a valid known_hosts file or not readable. See README.', array(
        '%path' => $ssh->getHostsFile(),
    )), 'warning');
  }

  // Sanitze port.
  $v['ssh_default_port'] = (int) $v['ssh_default_port'];

}

/**
 * Host configuration form callback.
 */
function ssh_hosts() {

  $ssh = new SSH();
  $form = array(
    'ssh_hosts' => $ssh->getHostsForm(),
  );

  $form['ssh_hosts'] += array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Remote hosts'),
    '#description' => t('Configure an arbitrary number of remote hosts. These will appear as options when confguring rules actions.'),
    '#tree' => TRUE,
  );

  return system_settings_form($form);

}

/**
 * Host configuration form callback.
 */
function ssh_hosts_validate($form, &$form_state) {

  $v = &$form_state['values']['ssh_hosts'];

  foreach ($v as $hid => $host) {

    $host['label'] = trim($host['label']);
    $is_new = $hid == count($v) - 1;

    if (!empty($host['label'])) {
      if (!preg_match('#^[A-Za-z0-9\-_]+$#', $host['label'])) {
        form_set_error(
          "ssh_hosts][${hid}][label",
          t('Invalid label: %label', array(
            '%label' => $host['label'],
          ))
        );
      }

      if (preg_match('#[\s"\'\\\\]#', $host['user'])) {
        form_set_error(
          "ssh_hosts][${hid}][user",
          t('Invalid username: %user', array(
            '%user' => $host['user'],
          ))
        );
      }

      if (preg_match('#[\s"\'\\\\]#', $host['hostname'])) {
        form_set_error(
          "ssh_hosts][${hid}][hostname",
          t('Invalid host name: %hostname', array(
            '%hostname' => $host['hostname'],
          ))
        );
      }

      if (preg_match('#[^0-9]#', $host['port'])) {
        form_set_error("ssh_hosts][${hid}][port",
          t('Invalid port!')
        );
      }

    }
    // Only the "add new" host label may be empty.
    elseif (!$is_new) {
      form_set_error(
        "ssh_hosts][${hid}][label",
        t('Host label must not be empty for an already existing host.')
      );
    }
    // If the "add new" label is empty, prevent the new host from saving.
    else {
      unset($v[$hid]);
    }

  }

}
