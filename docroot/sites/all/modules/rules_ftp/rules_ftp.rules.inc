<?php

/**
 * @file
 * Contains rules integration for the Rules FTP module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Action: Sends a file to an FTP server.
 */
class RulesFTPActionPut extends RulesActionHandlerBase {

  /**
   * Defines the action.
   */
  public static function getInfo() {
    return array(
      'name' => 'rules_ftp_put',
      'label' => t('Send file via FTP'),
      'group' => t('Rules FTP'),
      'parameter' => array(
        'host' => array(
          'label' => t('Host'),
          'type' => 'text',
          'description' => t("The FTP server address. This parameter shouldn't have any slashes and shouldn't be prefixed with a protocol (i.e. ftp:// or ftps://)."),
        ),
        'port' => array(
          'label' => t('Port'),
          'type' => 'integer',
          'description' => t('If omitted, the default FTP port (21) will be used.'),
          'optional' => TRUE,
        ),
        'ssl' => array(
          'label' => t('SSL'),
          'type' => 'boolean',
          'description' => t('If checked, the ftps:// protocol will be used, otherwise the ftp:// protocol will be used.'),
          'default value' => TRUE,
        ),
        'username' => array(
          'label' => t('Username'),
          'type' => 'text',
          'description' => t('FTP account name.'),
        ),
        'password' => array(
          'label' => t('Password'),
          'type' => 'text',
          'description' => t('FTP account password.'),
        ),
        'path' => array(
          'label' => t('Path'),
          'type' => 'text',
          'description' => t('Optional path to a sub-directory the files should be uploaded to.'),
          'optional' => TRUE,
        ),
        'file' => array(
          'label' => t('File'),
          'type' => 'file',
          'description' => t('File to send to the FTP server.'),
        ),
      ),
      'help' => t('Sends a file to an FTP server.'),
    );
  }

  /**
   * Executes the action.
   */
  public function execute($host, $port, $ssl, $username, $password, $path, $file) {
    $host = trim($host, '/');
    $port = !empty($port) ? $port : 21;
    $protocol = $ssl ? 'ftps://' : 'ftp://';
    $username = drupal_encode_path($username);
    $password = drupal_encode_path($password);
    $path = !empty($path) ? trim($path, '/') . '/' : '';
    $destination = check_url("$protocol$username:$password@$host:$port/$path") . $file->filename;

    // Copy the file to the FTP server.
    if (!@copy($file->uri, $destination)) {
      watchdog('file', 'The specified file %file could not be copied to %destination.', array('%file' => $file->uri, '%destination' => $destination), WATCHDOG_ERROR);
      return FALSE;
    }
  }

}

/**
 * @}
 */
