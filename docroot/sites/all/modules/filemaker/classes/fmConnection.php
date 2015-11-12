<?php

/**
 * @file
 * Class for Authentication to a FileMaker database.
 */

class FmConnection extends FmAdmin {

/**********************************************************************************
 * Database Fields (Public, to work with drupal_write_record() in FmBase).
 *********************************************************************************/

  /**
   * Unique key in Drupal database.
   *
   * @var int $fmcid
   */
  public $fmcid;

  /**
   * IP address, or URL, of the FileMaker host.
   *
   * @var string $host_name
   */
  public $host_name;

  /**
   * The name of the filemaker database, without the .fp7.
   *
   * @var string $database_name
   */
  public $database_name;

  /**
   * The username of the FileMaker database user.
   *
   * @var string $user_name
   */
  public $user_name;

  /**
   * The FileMaker password.
   *
   * @var string $password
   */
  public $password;

  /**
   * The FileMaker port.
   *
   * @var string $port
   */
  public $port = 443;

  /**
   * The FileMaker version, i.e. 7 or 12, the "fm" is not needed.
   *
   * @var string $version
   */
  public $version = 12;

  /**
   * True if HTTPS is used.
   *
   * @var int $https
   */
  public $https;


/**********************************************************************************
 * Consts.
 *********************************************************************************/

  const ID_FIELD_NAME = 'fmcid';
  const TABLE_NAME = 'filemaker_connection';

/**********************************************************************************
 * API -- Public functions.
 *********************************************************************************/

  /**
   * Loads FX.php (open source API for FileMaker) if it's not loaded already.
   *
   * @return
   *  TRUE if FX.php is loaded, FALSE otherwise.
   */
  public function filemaker_load_api() {

    if ( ! class_exists('FX')) {
      
      // Try using libraries module.
      if (module_exists('libraries')) {
        
        $libraries_path = drupal_get_path('module', 'libraries');
        $libraries_include = $libraries_path . '/' . 'libraries.module';
        include_once($libraries_include);

        // Let's see if the FileMaker API is really available from libraries.
        $fx_api = './' . libraries_get_path('fxphp') . '/FX.php';
        if (file_exists($fx_api)) {
          include_once $fx_api;
        }
      }

      // Don't have the libraries module.
      else {
        drupal_set_message(t('Can not find the libraries module. The libraries module is needed to load the FX library. Please enable the libraries module.'), 'error');
        return false;
      }
    }

    if ( ! class_exists('FX')) {
      drupal_set_message(t('Can not find FX.php. Please place FX.php at sites/all/libraries/fxphp/FX. See README.txt for details.'), 'error');
    }
    
    // Tell the caller if the FileMaker class exists.
    return class_exists('FX');
  }

  /**
   * Gets all of the connections out of the dabase and prepare the to be options in the Drupal form.
   */
  public function get_all_as_options_for_form() {
    $connections = $this->get_all();
    $options = array();
    foreach ($connections as $connection) {
      $options[$connection->fmcid] = $connection->database_name . ' ( ' . $connection->user_name . ' / ' . $connection->host_name . ' ) ';
    }
    return $options;
  }

  /**
   * Grabs needed values from the node, and places them in $this.
   */
  public function set_values_from_node(stdClass $node) {
    $this->nid = $node->nid;
  }

  /**
   * Returns a themed HTML table of the connections that are passed in.
   *
   * @param array of FmConnections
   */
  public function table(array $connections) {

    if (empty($connections)) {
      return t('There are not any connections for this FileMaker website.');
    } 

    $header = array('fmcid', 'Hostname', 'Database', 'Username', 'Password', 'Port', 'Version', 'HTTPS', 'Actions');
    $rows = array();

    foreach ($connections as $connection) {

      $edit_link = l('Edit', FILEMAKER_ADMIN_PATH . '/connection/' . $connection->fmcid);
      $delete_link = l('Delete', FILEMAKER_ADMIN_PATH . '/connection/' . $connection->fmcid . '/delete');

      $row = array(
        $connection->fmcid,
        $connection->host_name,
        $connection->database_name,
        $connection->user_name,
        '*********',
        $connection->port,
        $connection->version,
        ($connection->https) ? 'Yes' : 'No',
        $edit_link . ' | ' . $delete_link
      );

      $rows[] = $row;

    }

    $variables = array('header' => $header, 'rows' => $rows);

    return theme('table', $variables);
  }

  /**
   * todo Should this be here?
   */
  public function layout_options($fx) {
    
    $names = $fx->FMLayoutNames();

    if ($names instanceof FX_Error) {
      return $names;
    }
    
    $ret = array();    
    foreach ($names['data'] as $weird_number) {
      foreach ($weird_number as $name) {
        $ret[$name[0]] = $name[0];
      }
    }
    
    return $ret;
    
  }

  /**
   * Tests $this connection to see if it can connect to FileMaker and return data.
   */
  public function test_if_valid() {
    
    $fx = $this->test_connection();
    
    if ($fx instanceof FX) {
      
      $layout_options = $this->layout_options($fx);
      
      if (filemaker_is_error($layout_options)) {
        drupal_set_message(t('The connection is not valid.'), 'error');
        $message = 'Invalid connection, fmcid: %fmcid';
        watchdog('filemaker', $message, array('%fmcid' => $this->fmcid), WATCHDOG_ERROR);
      }
      else {
        drupal_set_message(t('Connected to FileMaker.'));
      }
    }
  }

  /**
   * Attempt to load FX library. If successful, build and return fx.
   * Takes a node, returns an FX object.
   * Get a FX object, based on our node's connction and layout.
   */
  public function test_connection($node = NULL) {
    
    if ( ! $this->filemaker_load_api()) {
      return false;
    }

    $https = ($this->https) ? 'HTTPS' : '';
    $fx = new FX($this->host_name, $this->port, 'FMPro' . $this->version, $https);
    
    $file_type = '.fp';
    if ($this->version == '12') {
      $file_type = '.fmp';
    }

    $layout = ( ! isset($node->layout)) ? '' : $node->layout;
    
    $fx->SetDBData($this->database_name . $file_type . $this->version, $layout);
    $fx->SetDBUserPass($this->user_name, $this->password);
    return $fx;
  }

  /**
   * Validate function for adding/editing a connection.
   */
  public function validate_form(array $form_state) {
    $this->set_values_from_form_state($form_state);
    $this->validate_database_name();
  }

  /**
   * Helper function to validate the connection form. Validates the db name.
   */
  private function validate_database_name() {
    if (substr_count($this->database_name, '.fp7') || substr_count($this->database_name, '.fmp12')) {
      form_set_error('database_name', t('Do not include the file extension (.fp7 or fmp12) in your database name.'));
    }
  }

  /**
   * Form to add or edit a connection.
   */
  public function admin_form() {

    $form = array();

    if ($this->fmcid > 0) {
      $_SESSION['filemaker']['fmcid'] = $this->fmcid;
    }
    else {
      unset($_SESSION['filemaker']['fmcid']);
    }
    
    // Insert validation, submit function names, and redirect location.
    $form['#submit'][] = 'filemaker_connection_submit';
    // $form['#redirect'] = FILEMAKER_ADMIN_PATH;
    $form['#validate'][] = 'filemaker_connection_validate';

    // Create connection fieldset.
    $form['connection'] = array(
      '#title' => t('FileMaker Connection'),
      '#type' => 'fieldset',
    );

    // Database field.
    $form['connection']['database_name'] = array(
      '#title' => t('Database Name'),
      '#type' => 'textfield',
      '#size' => 50,
      '#description' => t('Do not include the file extension.  If your database name includes spaces, you must rename it.'),
      '#default_value' => $this->database_name,
      '#required' => true,
    );

    // Version field.
    $form['connection']['version'] = array(
      '#title' => t('Version'),
      '#type' => 'textfield',
      '#size' => 3,
      '#description' => t('The number at the end of the file extension. For example, ".fp7" becomes 7 and ".fmp12" becomes 12.'),
      '#default_value' => $this->version,
      '#required' => true,
    );

    // Create connection fieldset.
    $form['connection']['credentials'] = array(
      '#title' => t('Login credentials'),
      '#type' => 'fieldset',
    );

    // Username field.
    $form['connection']['credentials']['user_name'] = array(
      '#title' => t('Username'),
      '#type' => 'textfield',
      '#size' => 50,
      '#description' => t('The username for this database. Please make sure this user\'s privilege set includes XML.'),
      '#default_value' => $this->user_name,
      '#required' => true,
    );

    // Password field.
    $form['connection']['credentials']['password'] = array(
      '#title' => t('Password'),
      '#type' => 'password',
      '#size' => 50,
      '#description' => t('The FileMaker password.'),
      '#default_value' => $this->password,
      '#required' => true,
    );

    // Hostname field.
    $form['connection']['host_name'] = array(
      '#title' => t('Host Name'),
      '#type' => 'textfield',
      '#size' => 50,
      '#description' => t('The domain name or IP address.  No need to include https:// or http://.'),
      '#default_value' => $this->host_name,
      '#required' => true,
    );

    // Port field.
    $form['connection']['port'] = array(
      '#title' => t('Port'),
      '#type' => 'textfield',
      '#size' => 4,
      '#description' => t('Although this number could be anything, traditionally is is 443 for HTTPS and 80 for HTTP.'),
      '#default_value' => $this->port,
      '#required' => true,
    );

    $form['connection']['https'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use HTTPS?'),
      '#default_value' => isset($this->https) ? $this->https : 1,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Connection'),
    );
    
    return $form;
  }

  /**
   * Build $this based on the values in the $form_state.
   */
  public function set_values_from_form_state(array $form_state) {

    // Do we have a fmcid?
    if (isset($_SESSION['filemaker']['fmcid'])) {

      $fmcid = $_SESSION['filemaker']['fmcid'];
      
      // Is our fmcid valid?
      if ($fmcid > 0) {
        $test_connection = new FmConnection($fmcid);
        if ($test_connection->is_valid_in_drupal) {
          $this->fmcid = $fmcid;
        }
      }
    }

    $this->host_name = $form_state['values']['host_name'];
    $this->database_name = $form_state['values']['database_name'];
    $this->user_name = $form_state['values']['user_name'];
    $this->password = $form_state['values']['password'];
    $this->port = $form_state['values']['port'];
    $this->version = $form_state['values']['version'];
    $this->https = $form_state['values']['https'];
    
    return $this;
  }

/**********************************************************************************
 * Protected functions.
 *********************************************************************************/

  /**
   * Sets values in $this based on a database result.
   */
  protected function set_values(stdClass $connection) {

    if (isset($connection->fmcid)) {
      $this->fmcid = $connection->fmcid;
    }

    $this->host_name = $connection->host_name;
    $this->database_name = $connection->database_name;
    $this->user_name = $connection->user_name;
    $this->password = $connection->password;
    $this->port = $connection->port;
    $this->version = $connection->version;
    $this->https = $connection->https;
  }
}
