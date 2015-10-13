<?php
/**
 * @file
 * Functions to handle the Rackspace Cloud Files backup destination.
 */

/**
 * A destination for sending database backups to a Rackspace Cloud Files server.
 *
 * @ingroup backup_migrate_destinations
 */
class backup_migrate_destination_opencloud extends backup_migrate_destination {
  var $supported_ops = array('scheduled backup', 'manual backup', 'restore', 'list files', 'configure', 'delete');

  /**
   * Override base class method
   */
  function get_location() {
    $parts[] = $this->settings('auth_type') . ' @ ' . $this->settings('auth_url');
    $parts[] = 'User: ' . $this->settings('auth_username');
    if ($this->settings('auth_tenantName')) {
      $parts[] = 'Tenant: ' . $this->settings('auth_tenantName');
    }
    return implode(', ', $parts);
  }

  /**
   * Save to to the OpenCloud destination.
   */
  function save_file($file, $settings) {
    _backup_migrate_opencloud_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_opencloud_dbg(func_get_args());

    try {
      // Connect to the target OpenStack Container object
      $cont = $this->_get_container();

      // Now try to actually save the file data
      _backup_migrate_opencloud_dbg('Getting DataObject');
      $obj = $cont->DataObject();
      $obj->Create(
        array(
          'name' => $file->filename(),
          'content_type' => file_get_mimetype($file->filepath()),
        ),
        $file->filepath()
      );
      return $file;
    }
    catch (Exception $e) {
      _backup_migrate_opencloud_log('Failed saving object: @msg', array('@msg' => $e->getMessage()));
    }

    return FALSE;
  }

  /**
   * Load from the OpenCloud destination.
   */
  function load_file($file_id) {
    _backup_migrate_opencloud_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_opencloud_dbg(func_get_args());

    try {
      // Connect to the target OpenStack Container object
      $cont = $this->_get_container();

      // Now try to actually save the file data
      backup_migrate_include('files');
      $file = new backup_file(array('filename' => $file_id));
      _backup_migrate_opencloud_dbg('Getting DataObject');
      $obj = $cont->DataObject($file->filename());
      $obj->SaveToFilename($file->filepath());
      return $file;
    }
    catch (Exception $e) {
      _backup_migrate_opencloud_log('Failed loading object: @msg', array('@msg' => $e->getMessage()));
    }

    return NULL;
  }

  /**
   * Delete from the OpenCloud destination.
   */
  function delete_file($file_id) {
    _backup_migrate_opencloud_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_opencloud_dbg(func_get_args());

    try {
      // Connect to the target OpenStack Container object
      $cont = $this->_get_container();

      // Now try to actually delete the file data
      _backup_migrate_opencloud_dbg('Getting DataObject');
      $obj = $cont->DataObject($file_id);
      $obj->Delete();
    }
    catch (Exception $e) {
      _backup_migrate_opencloud_log('Failed deleting object: @msg', array('@msg' => $e->getMessage()));
    }
  }

  /**
   * List all files from the OpenCloud container.
   */
  function list_files() {
    _backup_migrate_opencloud_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_opencloud_dbg(func_get_args());


    try {
      // Connect to the target OpenStack Container object
      $cont = $this->_get_container();

      // Now try to collect listing
      backup_migrate_include('files');
      $files = array();
      $objlist = $cont->ObjectList();
      while ($obj = $objlist->Next()) {
        $info = array(
          'filename' => $obj->name,
          'filesize' => $obj->bytes,
          'filetime' => strtotime($obj->last_modified . '-0000'),
        );
        $files[$info['filename']] = new backup_file($info);
      }
    }
    catch (Exception $e) {
      _backup_migrate_opencloud_log('Failed deleting object: @msg', array('@msg' => $e->getMessage()));
    }

    return $files;
  }

  /**
   * Get the form for the settings for this filter
   */
  function edit_form() {
    // Check to see if the OpenCloud library is installed where the Libraries module can load it
    $library = libraries_detect('php-opencloud');
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
            '%path' => 'sites/all/libraries/php-opencloud',
          )
        ),
      );
      $form['library_return'] = array(
        '#prefix' => '<p>',
        '#suffix' => '</p>',
        '#markup' => t('Once the %libname has been installed, please return to this page to add an OpenCloud destination.', array('%libname' => $library['name'])),
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

    $settings['auth_info'] = array(
      '#type' => 'fieldset',
      '#title' => t('Authentication settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $auth_info = &$settings['auth_info'];

    $auth_info['auth_type'] = array(
      '#type' => 'select',
      '#title' => t('Connection class'),
      '#description' => t('Please select the class of OpenStack platform you are connecting to'),
      '#default_value' => array($this->settings('auth_type')),
      '#options' => drupal_map_assoc(array('Rackspace', 'OpenStack')),
      '#required' => TRUE,
    );
    $auth_info['auth_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Endpoint URL'),
      '#description' => t('Please enter the endpoint URL for your provider'),
      '#default_value' => $this->settings('auth_url'),
      '#required' => TRUE,
    );
    $auth_info['auth_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#description' => t('Please enter the username for your provider'),
      '#default_value' => $this->settings('auth_username'),
      '#required' => TRUE,
    );
    $auth_info['auth_password'] = array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#description' => t('Please enter the password for your provider'),
      '#default_value' => $this->settings('auth_password'),
      '#required' => TRUE,
    );
    $auth_info['auth_tenantName'] = array(
      '#type' => 'textfield',
      '#title' => t('Tenant Name'),
      '#description' => t('Optionally provide the tenant name to use on this OpenStack instance'),
      '#default_value' => $this->settings('auth_tenantName'),
      '#required' => FALSE,
    );

    $settings['storage_info'] = array(
      '#type' => 'fieldset',
      '#title' => t('Storage settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $storage_info = &$settings['storage_info'];

    $storage_info['storage_container'] = array(
      '#type' => 'textfield',
      '#title' => t('Container'),
      '#description' => t('Please enter the name of the container where you want the backup files stored'),
      '#default_value' => $this->settings('storage_container'),
      '#required' => TRUE,
    );
    $storage_info['storage_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Service name'),
      '#description' => t('Please enter the object store service name (e.g. %sample)', array('%sample' => 'cloudFiles')),
      '#default_value' => $this->settings('storage_name'),
      '#required' => TRUE,
    );
    $storage_info['storage_region'] = array(
      '#type' => 'textfield',
      '#title' => t('Region identifier'),
      '#description' => t('Please enter the object store region identifier (e.g. %sample)', array('%sample' => 'DFW, LON')),
      '#default_value' => $this->settings('storage_region'),
      '#required' => TRUE,
    );
    $storage_info['storage_urltype'] = array(
      '#type' => 'textfield',
      '#title' => t('URL type'),
      '#description' => t('Please enter the object store URL type (e.g. %sample)', array('%sample' => 'publicURL')),
      '#default_value' => $this->settings('storage_urltype'),
      '#required' => TRUE,
    );

    /**
     * TODO - Determine if this is even valid for the new OpenCloud API!
     * 
     * Figure out how to refactor for D6 and D7 keying off DRUPAL_CORE_COMPATIBILITY
     * This is the only place other than the .info file that the branches differ
     * 

    ///////////////////////////////////
    // D7 Code
    ///////////////////////////////////
    // Give the user the option to use system-level tokens for CF meta tags
    $metadata = $this->settings('metadata');
    $form['settings']['metadata'] = array(
      '#type' => 'textarea',
      '#title' => t('Cloud File Metadata'),
      '#description' => t('The metadata entries to tag each backup file with in the Cloud Files system. Enter one value per line, in the format <strong>name|value</strong>. The <strong>name</strong> cannot contain spaces, only alhpanumeric values, dashes, and undescores.  The <strong>value</strong> can contain any text as long as it resides on one line; it may also contain any system tokens.'),
      '#rows' => 10,
      '#cols' => 60,
      '#default_value' => (!empty($metadata)) ? $metadata : "site-name|[site:name]\nsite-mail|[site:mail]",
    );

    // If the Token module is installed and enabled, offer help for tokens available for metadata tags
    if (module_exists('token')) {
      $form['settings']['token_help'] = array(
        '#title' => t('Replacement patterns'),
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['settings']['token_help']['global'] = array(
        '#theme' => 'token_tree',
        '#global_types' => TRUE, // A boolean TRUE or FALSE whether to include 'global' context tokens like [current-user:*] or [site:*]. Defaults to TRUE.
        '#click_insert' => TRUE, // A boolean whether to include the 'Click this token to insert in into the the focused textfield' JavaScript functionality. Defaults to TRUE.      );
      );
    }

    ///////////////////////////////////
    // D6 Code
    ///////////////////////////////////
    // If the Token module is installed and enabled, offer metadata tags using tokens
    if (module_exists('token')) {
      $metadata = $this->settings('metadata');
      $form['settings']['metadata'] = array(
        '#type' => 'textarea',
        '#title' => t('Cloud File Metadata'),
        '#description' => t('The metadata entries to tag each backup file with in the Cloud Files system. Enter one value per line, in the format <strong>name|value</strong>. The <strong>name</strong> cannot contain spaces, only alhpanumeric values, dashes, and undescores.  The <strong>value</strong> can contain any text as long as it resides on one line; it may also contain any of the tokens below.'),
        '#rows' => 10,
        '#cols' => 60,
        '#default_value' => (!empty($metadata)) ? $metadata : "site-name|[site-name]\nsite-mail|[site-mail]",
      );

      $form['settings']['token_help'] = array(
        '#title' => t('Replacement patterns'),
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        );
      $form['settings']['token_help']['global'] = array(
        '#value' => theme('token_help', 'global'),
      );
    }

     */

    return $form;
  }

  /**
   * Validate handler for settings form
   */
  function edit_form_validate($form, &$form_state) {
    parent::edit_form_validate($form, $form_state);
  }

  /**
   * Submit the form for the settings for the OpenCloud destination.
   */
  function edit_form_submit($form, &$form_state) {
    // Flatten out values from fieldsets
    $form_state['values']['settings'] += $form_state['values']['settings']['auth_info'];
    unset($form_state['values']['settings']['auth_info']);

    $form_state['values']['settings'] += $form_state['values']['settings']['storage_info'];
    unset($form_state['values']['settings']['storage_info']);

    parent::edit_form_submit($form, $form_state);
  }

  /**
   * Util method to load the OpenCloud lib and connect to a Container
   * 
   * @param profile Profile object containing settinsg information (optional)
   * @return OpenStack Container object
   * @throws Exception (either from us or the OpenCloud lib)
   */
  private function _get_container($profile = NULL) {
    _backup_migrate_opencloud_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_opencloud_dbg(func_get_args());

    $conn = NULL;
    if (!$profile) {
      $profile = $this;
    }

    // Load OpenCloud library
    $library = libraries_load('php-opencloud');
    if (empty($library['loaded'])) {
      throw new Exception(t('Unable to load %libname', array('%libname' => $library['name'])));
    }

    // Pull curl option overrides from variables (or $conf override in settings.php) and allow alter hooks to modify the array
    $curl_options = variable_get('backup_migrate_opencloud_curl_options', array());
    drupal_alter('backup_migrate_opencloud_curl_options', $curl_options);

    // Suss out connection class to use, and connect to endpoint
    $conn_class = '\\OpenCloud\\' . $profile->settings('auth_type');
    $conn = new $conn_class(
      $profile->settings('auth_url'),
      array(
        'username' => $profile->settings('auth_username'),
        'password' => $profile->settings('auth_password'),
        'tenantName' => $profile->settings('auth_tenantName'),
      ),
      $curl_options
    );

    // Connect to object store service
    _backup_migrate_opencloud_dbg('Getting ObjectStore');
    $ostore = $conn->ObjectStore(
      $profile->settings('storage_name'), 
      $profile->settings('storage_region'), 
      $profile->settings('storage_urltype')
    );

    // Make sure the target container exists
    _backup_migrate_opencloud_dbg('Getting Container');
    $cont = $ostore->Container();
    $cont->Create(array(
      'name' => $profile->settings('storage_container'),
    ));

    return $cont;
  }

  /**
   * Util method to build the metadata array for the OpenCloud storage object from our tokenized values
   */
  private function _get_metadata($settings) {
    _backup_migrate_opencloud_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_opencloud_dbg(func_get_args());

    $metadata = array();

    if (!empty($settings->destination->settings['metadata'])) {
      _backup_migrate_opencloud_dbg(array('Raw CF metadata' => $settings->destination->settings['metadata']));
      $lines = explode("\n", $settings->destination->settings['metadata']);
      foreach ($lines as $line) {
        // Toss out empty lines
        $line = trim($line);
        if (!empty($line)) {
          $tmp = explode('|', $line, 2);
          if (2 == count($tmp)) {
            // OK, we have a name and value pair; clean up name:
            $name = preg_replace('/[^0-9,a-z,A-Z,\-_]/', '-', trim($tmp[0]));

            // Replace tokens in value string
            $value = token_replace(trim($tmp[1]));

            // Store off in array for return
            $metadata[$name] = check_plain($value);
          }
        }
      }
    }

    _backup_migrate_opencloud_dbg(array('Expanded CF metadata' => $metadata));
    return $metadata;
  }
}


