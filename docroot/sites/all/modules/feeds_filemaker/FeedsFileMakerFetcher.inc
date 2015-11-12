<?php

/**
 * @file
 * Home of the FeedsFilemakerFetcher and related classes.
 */

/**
 * Result of FeedsFileMakerFetcher::fetch().
 */
class FeedsFileMakerFetcherResult extends FeedsFetcherResult {
  protected $config;

  /**
   * Constructor.
   */
  public function __construct($config) {
    $this->config = $config;
    parent::__construct('');
  }

  /**
   * Overrides FeedsFetcherResult::getRaw().
   */
  public function getRaw() {
    // Check that the config options have been set.
    if (!isset($this->config['database'], $this->config['hostspec'], $this->config['username'], $this->config['password'], $this->config['layout'])) {
      throw new Exception(t('One or more of the configuration options have not been set.  Check that you have entered the database, url, username, password and layout in the FileMaker fetcher configuration'));
    }

    $filemaker_library = libraries_load('filemaker');
    if ($filemaker_library['loaded']) {

      // Connect to FileMaker using the API and retrieve the records.
      $fm = new FileMaker();
      $fm->setProperty('database', $this->config['database']);
      $fm->setProperty('hostspec', $this->config['hostspec']);
      $fm->setProperty('username', $this->config['username']);
      $fm->setProperty('password', $this->config['password']);

      $find_command =& $fm->newFindCommand($this->config['layout']);

      // Add the filter criteria.
      foreach ($this->config["criteria"] as $criteria_value) {
        $find_command->addFindCriterion($criteria_value['field_id'], $criteria_value['field_value']);
      }

      $result = $find_command->execute();

      // Check for errors, if none then return the result set.
      if (Filemaker::isError($result)) {
        throw new exception(t('An error has occured whilst retrieving records from FileMaker. Error message: !error_message.  Error code: !error_code.',
          array('!error_message' => $result->message, '!error_code' => $result->code)));
      }
      else {
        return $result->getRecords();
      }
    }
    else {
      throw new Exception(t('Unable to load FileMaker API.  Ensure you have copied the API files into a folder into the /libraries/filemaker folder.'));
    }
  }
}

/**
 * Fetches data from FileMaker.
 */
class FeedsFileMakerFetcher extends FeedsFetcher {

  /**
   * Override FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $fetcher_result = new FeedsFileMakerFetcherResult($this->config);
    return $fetcher_result;
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'database' => NULL,
      'hostspec' => NULL,
      'username' => NULL,
      'password' => NULL,
      'layout' => NULL,
      'criteria' => array(),
      'num_criteria' => 1,
    );
  }

  /**
   * Override parent::configForm().
   *
   * Provide configuration options for FileMaker connection info (database,
   * hostspec, username, password and layout).  Also allow user to specify
   * search criteria, which will filter the records returned
   *
   * @todo allow user to delete any criteria, not just the last one
   * @todo allow multiple layouts (will require using newCompoundFindCommand)
   */
  public function configForm(&$form_state) {
    $config = $this->getConfig();
    return $this->sourceForm($config, $form_state);
  }

  /**
   * Submission handler for configForm().
   *
   * @param array $values
   *   The values from the $form_state
   */
  public function configFormSubmit(&$values) {
    if ($values['password'] == '') {
      $values['password'] = $this->config['password'];
    }
    $this->addConfig($values);
    $this->save();
    drupal_set_message(t('Your changes have been saved.'));
    feeds_cache_clear(FALSE);
  }

  /**
   * Overrides FeedsPlugin::sourceForm().
   */
  public function sourceForm($source_config, &$form_state = array()) {
    $form = array();
    $form['credentials'] = array(
      '#type' => 'fieldset',
      '#title' => t('Connection Credentials'),
    );
    $form['credentials']['database'] = array(
      '#type' => 'textfield',
      '#title' => t('FileMaker Database'),
      '#description' => t('FileMaker database id'),
      '#default_value' => $this->config['database'],
      '#required' => TRUE,
    );
    $form['credentials']['hostspec'] = array(
      '#type' => 'textfield',
      '#title' => t('FileMaker URL'),
      '#description' => t('Hostspec of web server in FileMaker Server deployment.  May be the URL where you login to the FileMaker web interface.  Defaults to http://localhost, if left blank.'),
      '#default_value' => $this->config['hostspec'],
      '#required' => TRUE,
    );
    $form['credentials']['username'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#description' => t('Username for connecting to FileMaker'),
      '#default_value' => $this->config['username'],
      '#required' => TRUE,
    );
    $form['credentials']['password'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#description' => t('Password for connecting to FileMaker.  If password has been saved, then box will appear blank to protect password.'),
      '#default_value' => $this->config['password'],
    );
    $form['credentials']['layout'] = array(
      '#type' => 'textfield',
      '#title' => t('Layout'),
      '#description' => t('The FileMaker layout that the data should be retrieved from.  Enter exactly as it appears in FileMaker.'),
      '#default_value' => $this->config['layout'],
      '#required' => TRUE,
    );
    $form['criteria'] = array(
      '#type' => 'fieldset',
      '#title' => t('Find Criteria'),
      '#description' => t('If you want to restrict the records that are retrieved form FileMaker, based on the values in certain fields then add some criteria below.  For example, you could choose to only retrieve records where the "surname" field is equal to "Smith."'),
      '#tree' => TRUE,
    );
    // Allow the user to add as many find criteria as they want.
    if (empty($form_state['num_criteria'])) {
      $form_state['num_criteria'] = $this->config['num_criteria'];
    }
    else {
      // Store the number of criteria in the form so that it gets added to the
      // config when the form is submitted.
      $form['num_criteria'] = array(
        '#type' => 'hidden',
        '#value' => $form_state['num_criteria'],
      );
    }
    // Build the number of textfields indicated by $form_state['num_criteria'].
    for ($i = 1; $i < $form_state['num_criteria']; $i++) {
      $form['criteria'][$i] = array(
        '#type' => 'fieldset',
        '#title' => t('Criterion #@num', array('@num' => $i)),
      );
      $form['criteria'][$i]['field_id'] = array(
        '#type' => 'textfield',
        '#title' => t('FileMaker Field Id'),
        '#description' => t("Enter field Id exactly as it appears in FileMaker."),
        '#default_value' => isset($this->config['criteria'][$i]['field_id']) ? $this->config['criteria'][$i]['field_id'] : '',
      );
      $form['criteria'][$i]['field_value'] = array(
        '#type' => 'textfield',
        '#title' => t('FileMaker Field Value'),
        '#description' => t('Enter the field value exactly as it appears in FileMaker.'),
        '#default_value' => isset($this->config['criteria'][$i]['field_value']) ? $this->config['criteria'][$i]['field_value'] : '',
      );
    }
    // Adds "Add a criterion" button.
    $form['criteria']['add_row'] = array(
      '#type' => 'submit',
      '#value' => t('Add a criterion'),
      '#submit' => array('feeds_filemaker_add_row'),
      '#limit_validation_errors' => array(),
      '#tree' => FALSE,
    );

    // If we have more than one row, this button allows removal of the
    // last row.
    if ($form_state['num_criteria'] > 1) {
      $form['criteria']['remove_row'] = array(
        '#type' => 'submit',
        '#value' => t('Remove latest criterion'),
        '#submit' => array('feeds_filemaker_remove_row'),
        // Since we are removing a row, don't validate until later.
        '#limit_validation_errors' => array(),
        '#tree' => FALSE,
      );
    }
    return $form;
  }

  /**
   * Validation handler for sourceForm().
   *
   * @param array $values
   *   The values form the $form_state
   */
  public function sourceFormValidate(&$values) {

    // If the password has already been saved then it is shown as a blank
    // textfield on the front end for security reasons.  When submitting the
    // form, we need to retrieve it from the config to ensure it gets retained.
    if ($values['credentials']['password'] == '') {
      $values['credentials']['password'] = $this->config['password'];
    }
  }
}

/**
 * Submit handler to add an additional row to the config form.
 */
function feeds_filemaker_add_row($form, &$form_state) {
  $form_state['num_criteria']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler to remove the latest row from the config form.
 */
function feeds_filemaker_remove_row($form, &$form_state) {
  if ($form_state['num_criteria'] > 1) {
    $form_state['num_criteria']--;
  }
  $form_state['rebuild'] = TRUE;
}
