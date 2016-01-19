<?php
/**
 * @file
 * Drupal SSH toolset class.
 */

class SSH {

  private $executable;
  private $keyfile;
  private $hostsfile;
  private $hosts;

  /**
   * Constructor.
   *
   * @param string $executable
   *   (Optional) Path to an executable other than the one
   *   defined in Drupal's variables.
   * @param string $keyfile
   *   (Optional) Path to an alternative keyfile other than the one
   *   defined in Drupal's variables.
   * @param string $hostsfile
   *   (Optional) Path to an alternative keyfile other than the one
   *   defined in Drupal's variables.
   */
  function __construct($executable = NULL, $keyfile = NULL, $hostsfile = NULL) {

    // Settings.
    foreach (array('executable', 'keyfile', 'default_port', 'hostsfile') as $key) {
      $this->$key = isset(${$key}) ?
        ${$key} : variable_get_value("ssh_$key");
    }

    // Hosts.
    $this->hosts = variable_get_value('ssh_hosts');

  }

  /**
   * Executes a command statement for a host.
   *
   * @param int $hid
   *   The internal unique host id.
   * @param string $command
   *   The raw command to be executed on the remote host, as
   *   it would be entered in the shell.
   * @param string $hideCommand
   *   (Optional): If TRUE, the actual SSH command is hidden from logs
   *   and info message displays.
   *
   * @return bool
   *   TRUE, if the SSH command returned no error.
   */
  function execute($hid, $command, $hideCommand = FALSE) {
    if ($command = $this->getCommand($hid, $command)) {
      $exitcode = 0;
      $out = array();
      exec($command, $out, $exitcode);

      $success = ! (bool) $exitcode;

      $out = array_map(function($line) {
        $line = format_string('<li>@line</li>', array(
          '@line' => $line,
        ));
        return $line;
      }, $out);

      $outShort = format_string('(!output)', array(
        '!output' => t('No output.'),
      ));
      $outLog = '(No output.)';

      if (!empty($out)) {
        $outShort = format_string('<ul>!items</ul>', array(
          '!items' => implode('', array_slice($out, -3)),
        ));
        $outLog = format_string('<ul>!items</ul>', array(
          '!items' => implode('', array_slice($out, -15)),
        ));
      }

      $logCommand = $hideCommand ? format_string('***!placeholder***', array(
        '!placeholder' => t('Command display has been disabled')
      )) : $command;

      // Info message
      if (user_access(SSH_INFO_PERMISSION)) {
        $msgVars = array(
          '%command' => $logCommand,
          '!output' => $outShort,
        );
        if($success) {
          $message = t('SSH: Command %command has been successfully executed.', $msgVars);
          $type = 'status';
        }
        else {
          $message = t('SSH: Command %command failed. The remote output ended with: <code>!output</code>', $msgVars);
          $type = 'error';
        }
        drupal_set_message($message, $type);
      }

      // Watchdog
      $w_type = $success ? WATCHDOG_NOTICE : WATCHDOG_ERROR;
      $w_msg = $success ?
        'Execution of %command succeeded. The remote output ended with: !output' :
        'Execution of %command failed. The remote output ended with: !output';
      watchdog('SSH', $w_msg, array(
        '%command' => $logCommand,
        '!output' => $outLog,
      ), $w_type);

      // Status tracker, update the corresponding tracker variable to "now".
      // Used for status report.
      // @see ssh_requirements
      // @see SSH::executionOk()
      variable_set_value($success ? 'ssh_last_success' : 'ssh_last_failure', REQUEST_TIME);

    }

    return FALSE;
  }

  /**
   * Prepares a command statement from a host id, a command and preset data.
   *
   * @param int $hid
   * @param string $command
   *
   * @return string
   */
  function getCommand($hid, $command) {

    $command = trim($command);
    if (!empty($command) && ($host =@ $this->hosts[(int) $hid])) {
      $port = (int) (!empty($host['port']) ? $host['port'] : $this->default_port);
      $command = format_string('!executable -i !key -o !hostsfile -p !port !userhost !command', array(
        '!executable' => $this->executable,
        '!key' => escapeshellarg($this->keyfile),
        '!port' => $port,
        '!hostsfile' => escapeshellarg(sprintf('UserKnownHostsFile=%s', $this->hostsfile)),
        '!userhost' => escapeshellarg(sprintf('%s@%s', $host['user'], $host['hostname'])),
        '!command' => escapeshellarg($command),
      ));
      return $command;
    }
    return FALSE;
  }

  /**
   * Tries to determine the host's default path to the SSH command.
   *
   * @return string|bools
   */
  public static function findExecutable() {

    $out = array();
    $exit = 0;
    $result = exec('which ssh', $out, $exit);

    // Return the result of "which", or FALSE on errors.
    // $exit must be inverted due to bash conventions.
    return !$exit ? $result : FALSE;
  }

  /**
   * Returns the raw executable path.
   *
   * @return string
   *   The bare executable string. DO NOT use for browser output
   *   without sanitation!
   */
  function getExecutable() {
    return $this->executable;
  }

  /**
   * Returns an FAPI snippet containing the current host settings.
   *
   * @param int $hid
   *   Optionally retrieve one host (the one with $hid).
   */
  function getHosts($hid = NULL) {

    return isset($hid) ? $this->hosts[$hid] : $this->hosts;

  }

  /**
   * Returns an id => label array for host select options.
   *
   * @see ssh.module
   * @see ssh_rules_action_info().
   */
  function getHostOptions() {

    $ret = array();

    foreach ($this->hosts as $hid => $host) {
      $ret[$hid] = check_plain($host['label']);
    }

    return $ret;

  }

  /**
   * Returns an FAPI snippet containing the current host settings.
   *
   * @param bool $add
   *   Whether to add a "new host" fieldset".
   */
  function getHostsForm($add = TRUE) {

    $hosts = $this->hosts;

    $defaultPort = variable_get_value('ssh_default_port');

    if ($add) {
      $hosts[] = array(
        'label' => '',
        'user' => 'user',
        'hostname' => 'host.example.com',
        'port' => '',
        '#collapsed' => FALSE,
      );
    }

    $form = array();

    foreach ($hosts as $hid => $host) {
      $form[$hid] = array(
        '#type' => 'fieldset',
        '#title' => !empty($host['label']) ? $host['label'] : t('Add host')  ,
        '#collapsible' => TRUE,
        'label' => array(
          '#title' => t('Host label'),
          '#description' => t('A unique label to identify this host. 7-bit ASCII alphanumeric only, plus dashes and underscores.'),
          '#type' => 'textfield',
          '#default_value' => $host['label'],
          '#collapsed' => isset($host['#collapsed']) ? $host['#collapsed'] : TRUE,
        ),
        'user' => array(
          '#title' => t('Username'),
          '#description' => t('Connect to the remote host on behalf of this user.'),
          '#type' => 'textfield',
          '#default_value' => $host['user'],
        ),
        'hostname' => array(
          '#title' => t('Host name'),
          '#description' => t('The actual host name (or IP address) without port.'),
          '#type' => 'textfield',
          '#default_value' => $host['hostname'],
        ),
        'port' => array(
          '#title' => t('Remote port'),
          '#description' => t('The SSH port for this host. If empty, the system default will be used (currently port %port).', array(
            '%port' => $defaultPort,
          )),
          '#type' => 'textfield',
          '#default_value' => $host['port'],
        ),
      );
      if ($hid < count($hosts) - 1) {
        $form[$hid]['example'] = array(
          '#type' => 'fieldset',
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#title' => t('Shell example'),
          '#description' => t("With the current settings, an SSH call from your web server to this host would look like so."),
          'code' => array(
            '#markup' => format_string('<code>!code</code>', array(
              '!code' => $this->getCommand($hid, 'echo "Hello world!"'),
            )),
          ),
        );
      }
    }

    return $form;

  }

  /**
   * Returns the raw executable path.
   *
   * @return string
   *   The bare keyfile path string.
   */
  function getHostsfile() {
    return $this->hostsfile;
  }

  /**
   * Returns the raw executable path.
   *
   * @return string
   *   The bare keyfile path string.
   */
  function getKeyfile() {
    return $this->keyfile;
  }

  /**
   * Returns whether the last execution worked (if any).
   *
   * @return bool
   *   TRUE, if either no error ever occured or, since then,
   *   at least one successful execution was performed.
   *   Should only be considered a local host status indicator.
   */
  function executionOk() {
    $lastSuccess = variable_get_value('ssh_last_success');
    $lastFailure = variable_get_value('ssh_last_failure');
    return empty($lastFailure) || $lastSuccess > $lastFailure;
  }

  /**
   * Checks whether the known_hosts file is readable.
   *
   * @param $path
   *   (Optional) file path, if other than the one stored by Drupal.

   * @return bool
   *   TRUE, if the defined path is valid and readable.
   */
  function hostsFileOk($path = NULL) {

    if (!isset($path)) {
      $path = $this->hostsfile;
    }
    return is_readable($path);
  }

  /**
   * Checks whether the keyfile has the right read permissions.
   *
   * @param $path
   *   (Optional) keyfile path, if other than the one stored by Drupal.
   * @param $filemask
   *   (Optional) keyfile mask to check for if other than "0600".

   * @return bool
   *   TRUE, if the defined keyfile path is valid the permissions.
   *   are set to $filemask. FALSE otherwise.
   */
  function keyfileOk($path = NULL, $filemask = '0600') {

    if (!isset($path)) {
      $path = $this->keyfile;
    }
    $fileperms = substr(sprintf('%o', fileperms($path)), -4);
    return $filemask == $fileperms;
  }

  /**
   * Checks whether the executable is "ok".
   *
   * @param $path
   *   (Optional) file path, if other than the one stored by Drupal.
   *
   * @return bool
   *   TRUE, if the defined executable path is valid and executable.
   *   FALSE otherwise.
   */
  function ok($path = NULL) {
    if (!isset($path)) {
      $path = $this->executable;
    }
    return is_executable($this->executable);
  }

}
