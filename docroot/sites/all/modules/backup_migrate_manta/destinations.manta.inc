<?php
/**
 * @file
 * Functions to handle the Joyent Manta backup destination.
 */

/**
 * A destination for sending database backups to a Joyent Manta server.
 *
 * @ingroup backup_migrate_destinations
 */
class backup_migrate_destination_manta extends backup_migrate_destination {
  var $supported_ops = array('scheduled backup', 'manual backup', 'restore', 'list files', 'configure', 'delete');

  /**
   * Override base class method
   */
  function get_location() {
    $parts[] = 'Manta @ ' . $this->settings('endpoint') . '/' . $this->settings('dir');
    $parts[] = 'User: ' . $this->settings('login');
    return implode(', ', $parts);
  }

  /**
   * Save to to the Manta destination.
   */
  function save_file($file, $settings) {
    _backup_migrate_manta_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_manta_dbg(func_get_args());

    try {
      // Connect to the Manta server
      $manta = $this->_get_manta_client();

      // Make sure directory exists
      $manta->PutDirectory($this->settings('dir'), TRUE);

      // Now try to actually save the file data
      $manta->PutObject(
        file_get_contents($file->filepath()),
        $file->filename(),
        $this->settings('dir'),
        array(
          'Content-type: ' . file_get_mimetype($file->filepath()),
        )
      );

      return $file;
    }
    catch (Exception $e) {
      _backup_migrate_manta_log('Failed saving object: @msg', array('@msg' => $e->getMessage()));
    }

    return FALSE;
  }

  /**
   * Load from the Manta destination.
   */
  function load_file($file_id) {
    _backup_migrate_manta_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_manta_dbg(func_get_args());

    try {
      // Connect to the Manta server
      $manta = $this->_get_manta_client();

      // Now try to actually load the file data
      backup_migrate_include('files');
      $file = new backup_file(array('filename' => $file_id));
      $resp = $manta->GetObject($file->filename(), $this->settings('dir'));
      if (isset($resp['data'])) {
        file_put_contents($file->filepath(), $resp['data']);
      }
      return $file;
    }
    catch (Exception $e) {
      _backup_migrate_manta_log('Failed loading object: @msg', array('@msg' => $e->getMessage()));
    }

    return NULL;
  }

  /**
   * Delete from the Manta destination.
   */
  function delete_file($file_id) {
    _backup_migrate_manta_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_manta_dbg(func_get_args());

    try {
      // Connect to the Manta server
      $manta = $this->_get_manta_client();

      // Now try to actually delete the file data
      $manta->DeleteObject($file_id, $this->settings('dir'));
    }
    catch (Exception $e) {
      _backup_migrate_manta_log('Failed deleting object: @msg', array('@msg' => $e->getMessage()));
    }
  }

  /**
   * List all files from the Manta container.
   */
  function list_files() {
    _backup_migrate_manta_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_manta_dbg(func_get_args());


    try {
      // Connect to the Manta server
      $manta = $this->_get_manta_client();

      // Now try to collect listing
      backup_migrate_include('files');
      $files = array();
      $resp = $manta->ListDirectory($this->settings('dir'));
      foreach ($resp['data'] as $item) {
        $info = array(
          'filename' => $item['name'],
          'filesize' => $item['size'],
          'filetime' => strtotime($item['mtime']),
        );
        $files[$info['filename']] = new backup_file($info);
      }
    }
    catch (Exception $e) {
      _backup_migrate_manta_log('Failed retrieving object list: @msg', array('@msg' => $e->getMessage()));
    }

    return $files;
  }

  /**
   * Get the form for the settings for this filter
   */
  function edit_form() {
    // Check to see if the Manta library is installed where the Libraries module can load it
    $library = libraries_detect('php-manta');
    if (empty($library['installed'])) {
      $form['library_missing'] = array(
        '#prefix' => '<h2>',
        '#suffix' => '</h2>',
        '#markup' => t('%libname cannot be found on your system.', array('%libname' => $library['name'])),
      );
      $form['library_download'] = array(
        '#prefix' => '<p>',
        '#suffix' => '</p>',
        '#markup' => t(
          'Please go to !url to download the latest copy, and place it in the folder %path',
          array(
            '!url' => l($library['download url'], $library['vendor url'], array('attributes' => array('target' => '_blank'))),
            '%path' => 'sites/all/libraries/php-manta',
          )
        ),
      );
      $form['library_return'] = array(
        '#prefix' => '<p>',
        '#suffix' => '</p>',
        '#markup' => t('Once the %libname has been installed, please return to this page to add a Manta destination.', array('%libname' => $library['name'])),
      );

      return $form;
    }

    // OK, we found it so go ahead and request destination info
    $form = parent::edit_form();

    $form['settings'] = array(
      '#type' => 'markup',
      '#weight' => 50,
      '#tree' => TRUE,
    );
    $settings = &$form['settings'];

    $settings['endpoint'] = array(
      '#type' => 'textfield',
      '#title' => t('Endpoint URL'),
      '#description' => t('Please enter the endpoint URL for your provider'),
      '#default_value' => $this->settings('endpoint'),
      '#required' => TRUE,
    );
    $settings['login'] = array(
      '#type' => 'textfield',
      '#title' => t('Login'),
      '#description' => t('Please enter the login for your provider'),
      '#default_value' => $this->settings('login'),
      '#required' => TRUE,
    );
    $settings['keyid'] = array(
      '#type' => 'textfield',
      '#title' => t('Key ID'),
      '#description' => t('Please enter the access key ID for your provider'),
      '#default_value' => $this->settings('keyid'),
      '#required' => TRUE,
    );
    $settings['priv_key'] = array(
      '#type' => 'textarea',
      '#title' => t('Private key'),
      '#description' => t('Please provide the OpenSSH private key used to sign API calls (associated with the key id above)'),
      '#default_value' => $this->settings('priv_key'),
      '#required' => TRUE,
    );

    $settings['dir'] = array(
      '#type' => 'textfield',
      '#title' => t('Directory'),
      '#description' => t('Please enter the name of the directory where you want the backup files stored'),
      '#default_value' => $this->settings('dir'),
      '#required' => FALSE,
    );

    return $form;
  }

  /**
   * Validate handler for settings form
   */
  function edit_form_validate($form, &$form_state) {
    parent::edit_form_validate($form, $form_state);
  }

  /**
   * Submit the form for the settings for the Manta destination.
   */
  function edit_form_submit($form, &$form_state) {
    parent::edit_form_submit($form, $form_state);
  }

  /**
   * Util method to load the Manta lib and create a client
   *
   * @param profile Profile object containing settings information (optional)
   * @return Manta client object
   * @throws Exception (either from us or the Manta lib)
   */
  private function _get_manta_client($profile = NULL) {
    _backup_migrate_manta_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_manta_dbg(func_get_args());

    $conn = NULL;
    if (!$profile) {
      $profile = $this;
    }

    // Load Manta library
    $library = libraries_load('php-manta');
    if (empty($library['loaded'])) {
      throw new Exception(t('Unable to load %libname', array('%libname' => $library['name'])));
    }

    // Pull curl option overrides from variables (or $conf override in settings.php) and allow alter hooks to modify the array
    $curl_options = variable_get('backup_migrate_manta_curl_options', array());
    drupal_alter('backup_migrate_manta_curl_options', $curl_options);

    $client = new MantaClient(
      $profile->settings('endpoint'),
      $profile->settings('login'),
      $profile->settings('keyid'),
      $profile->settings('priv_key'),
      'RSA-SHA256',
      $curl_options
    );

    return $client;
  }
}


