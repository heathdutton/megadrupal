<?php

/**
 * @file
 * FileMaker web layout node.
 */

class FmWebLayout extends FmBase {

  const ID_FIELD_NAME = 'fmid';
  const TABLE_NAME = 'filemaker_web_layout';

/**********************************************************************************
 * Database Fields
 *********************************************************************************/

  /**
   * Unique key for filemaker_web_layout.
   *
   * @var int $fmid
   */
  public $fmid;

  /**
   * Unique key for node.
   *
   * @var int $nid
   */
  public $nid;

  /**
   * Unique key for the filemaker_connection record.
   *
   * @var int $fmcid
   */
  public $fmcid;

  /**
   * Name of the FileMaker layout that this node interacts with.
   *
   * @var string $layout
   */
  public $layout;

  /**
   * The type of view for the standard Drupal node view.
   * Either: 'find' 'browse' 'create'
   *
   * @var string $drupal_view
   */
  public $drupal_view;

  /**
   * True if the record number should be displayed on the FileMaker node.
   *
   * @var int $show_record_number
   */
  public $show_record_number;

  /**
   * Used in pagination, to determine the number of records to pull back from FileMaker
   * per page (request).
   *
   * @var int $records_per_page
   */
  public $records_per_page;

/**********************************************************************************
 * Non-database variables.
 *********************************************************************************/

  /**
   * Object used for authentication to the FileMaker database.
   *
   * @var FmConnection
   */
  public $connection;

  /**
   * Array of all fields related to this web layout node.
   *
   * @var array of FmFields.
   */  
  public $fields = array();

  /**
   * Page number for pagination, first page is 0. Grabs page number from the url.
   *
   * @var int
   */
  public $page_number;

  /**
   * Used in pagination, to determine the number of records to skip.
   *
   * @var int
   */
  public $records_to_skip;

/**********************************************************************************
 * Public functions.
 *********************************************************************************/

  public function __construct($nid = 0, $ok_to_touch_filemaker = TRUE) {
    if ($nid) {
      $this->build($nid);
      $this->build_elements($ok_to_touch_filemaker);
      $this->set_pagination();
    }
  }

  /**
   * Attempt to load FX library. If successful, build and return fx.
   * Get a FX object, based on our node's connction and layout.
   */
  public function fx() {
    
    if ( ! $this->connection->filemaker_load_api()) {
      return false;
    }

    $https = ($this->connection->https) ? 'HTTPS' : '';
    $fx = new FX($this->connection->host_name, $this->connection->port, 'FMPro' . $this->connection->version, $https);
    
    $file_type = '.fp';
    if ($this->connection->version == '12') {
      $file_type = '.fmp';
    }

    $layout = ( ! isset($this->layout)) ? '' : $this->layout;
    
    $fx->SetDBData($this->connection->database_name . $file_type . $this->connection->version, $layout, $this->records_per_page);
    $fx->SetDBUserPass($this->connection->user_name, $this->connection->password);
    return $fx;
  }

  /**
   * Form callback for filemaker_record_form::'edit'.
   *
   * @param string $mode
   * Either 'filemaker' (if in FileMaker tab) or 'drupal' (if in standard Drupal view).
   *  
   */
  public function edit_form($record = NULL, $mode = 'filemaker') {
    
    $form = array();
    
    // Insert validation, submit function names, and redirect location.
    if ( ! $record) {
      $form['#submit'][] = 'filemaker_create_submit';
    }
    else {
      $form['#submit'][] = 'filemaker_browse_submit';
      
      foreach ($record as $key => $values) {
        $fmid = $key;
        $data = $values;
      }
    }

    if ($mode == 'drupal') {
      $title = '';
      $form['#submit'] = array('filemaker_default_create_submit');
    }
    else {
      $title = t('FileMaker record in layout: @layout', array('@layout' => $this->layout));
    }
    // Create record fieldset.
    $form['record'] = array(
      '#title' => $title,
      '#type' => 'fieldset',
    );

    foreach ($this->fields as $field) {
      if ($mode != 'drupal' || $field->include_field_in_create) {

        $default_value = $this->default_value($field->default_value, $field->default_create_value, 'create');
        $form['record'][$field->name] = array(
          '#title' => t($field->label),
          '#type' => $field->widget,
          '#weight' => $field->weight,
          '#required' => $field->required,
          '#default_value' => ( ! isset($data[$field->name][0])) ? $default_value : $data[$field->name][0],
        );

        switch ($field->widget) {
          case 'select':
          case 'radios':
          case 'checkboxes':
            $form['record'][$field->name]['#options'] = $field->value_list_options;
        }
      }
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Record'),
    );

    return $form;
  }

  /**
   * Form callback for filemaker_record_form::'find'.
   */
  public function find_form($mode = 'filemaker') {
    
    $form = array();
    
    if ($mode == 'drupal') {
      $title = t('Perform Find');
      $form['#submit'][] = 'filemaker_default_find_submit';
    }
    else {
      $title = t('FileMaker record in layout: @layout', array('@layout' => $this->layout));
      $form['#submit'][] = 'filemaker_find_submit';
    }
    
    $form['record'] = array(
      '#title' => $title,
      '#type' => 'fieldset',
    );
    
    foreach ($this->fields as $field) {
      if ($mode != 'drupal' || $field->searchable) {
        $form['record'][$field->name] = array(
          '#title' => t($field->label),
          '#type' => 'textfield',
          '#weight' => $field->weight,
          '#default_value' => $this->default_value($field->default_value, $field->default_find_value, 'find'),
        );
      }
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit Find Request'),
    );

    return $form;
  }

  public function save_from_node($node) {
    $this->set_values_from_node($node);
    $this->save();
  }

  public function drupal_view_options() {
    return array(
      '' => '',
      'create' => t('Create'),
      'find' => t('Find'),
      'default' => t('Default'),
    );
  }

  /**
   * Form callback when creating new node.
   */
  public function admin_form($node, $connection_options) {
    
    $type = node_type_get_type($node);
    $form = array();

    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => check_plain($type->title_label),
      '#default_value' => ! empty($node->title) ? $node->title : '',
      '#required' => true,
      '#weight' => -5,
    );

    $form['fmcid'] = array(
      '#type' => 'select',
      '#options' => $connection_options,
      '#title' => t('Connection to FileMaker database'),
      '#default_value' => ! isset($node->fmcid) ? 0 : $node->fmcid,
      '#required' => true,
      '#weight' => 1,
    );

    return $form;
  }

  /**
   * Form callback for node configuration settings.
   */
  public function layout_settings_form($layout_names) {

    $form = array();
    
    $form['#submit'][] = 'filemaker_settings_submit';
    
    $form['layout'] = array(
      '#title' => t('FileMaker Layout'),
      '#type' => 'fieldset',
    );

    $form['layout']['name'] = array(
      '#type' => 'select',
      '#options' => $layout_names,
      '#title' => t('FileMaker layout'),
      '#default_value' => ! empty($this->layout) ? $this->layout : '',
      '#required' => true,
      '#weight' => 1,
      '#description' => t('The layout, inside of FileMaker, that this Drupal web layout will interact with'),  
    );

    $form['layout']['drupal_view'] = array(
      '#type' => 'select',
      '#options' => $this->drupal_view_options(),
      '#title' => t('Default Drupal view'),
      '#default_value' => ! empty($this->drupal_view) ? $this->drupal_view : '',
      '#required' => true,
      '#weight' => 2,
      '#description' => t('The view at node/[nid]'),  
    );

    $form['layout']['records_per_page'] = array(
      '#type' => 'select',
      '#options' => array(5 => 5, 25 => 25, 50 => 50, 100 => 100),
      '#title' => t('Records Per Page'),
      '#default_value' => ($this->records_per_page) ? $this->records_per_page : 50,
      '#required' => true,
      '#weight' => 3,
      '#description' => t('Number of FileMaker records to display per page. The higher the number, the slower the requests.'),  
    );

    $form['layout']['show_record_number'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show record number?'),
      '#weight' => 4,
      '#default_value' => isset($this->show_record_number) ? $this->show_record_number : 1,
    );


    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Settings'),
      '#weight' => 5,
    );
    
    return $form;
  }

  /**
   * Sets $this from the passed in $node.
   */
  public function set_values_from_node(stdClass $node) {
    
    if (isset($node->nid)) {
      $this->build($node->nid);
    }

    $this->layout = (isset($node->layout)) ? $node->layout : '';
    $this->fmcid =  (isset($node->fmcid)) ? $node->fmcid : '';
    $this->drupal_view =  (isset($node->drupal_view)) ? $node->drupal_view : '';
    $this->show_record_number = (isset($node->show_record_number)) ? $node->show_record_number : '';
    $this->records_per_page = (isset($node->records_per_page)) ? $node->records_per_page : '';
    
    return true;
  }

  /**
   * Sets values in passed in $node from $this.
   */
  public function set_values_in_node(stdClass &$node) {
    $node->fmid = $this->fmid;
    $node->fmcid = $this->fmcid;
    $node->layout = $this->layout;
    $node->drupal_view = $this->drupal_view;
    $node->show_record_number = $this->show_record_number;
    $node->records_per_page = $this->records_per_page;
  }

  /**
   * Sets $this from a database result.
   */
  public function set_values_from_database(stdClass $object) {
    $this->fmid = $object->fmid;
    $this->fmcid = $object->fmcid;
    $this->layout = $object->layout;
    $this->drupal_view = $object->drupal_view;
    $this->show_record_number = $object->show_record_number;
    $this->records_per_page = $object->records_per_page;
  }

  /**
   * Show the view of a node, which can include a create new record form.
   */
  public function view($node) {
  
    if ($node->drupal_view == 'create') {
      $node->content = drupal_get_form($this->edit_form('', 'drupal'));
    }
  
    if ($node->drupal_view == 'find') {
      $node->content = $this->find_form('drupal');
    }

    return $node;
  }

  /**
   * Initial creation of node and setting of its attributes.
   */
  public function build($nid) {

    $this->nid = $nid;
    $query = db_select($this::TABLE_NAME, 't');
    $query
      ->condition('nid', $this->nid)
      ->fields('t')
      ->range(0, 1);

    $result = $query->execute();
    $record = $result->fetchAllAssoc('nid');
    
    if ( ! $record) {

      $this->errors[] = t('No record found in @table_name for nid: @nid', array(
        '@table_name' => $this::TABLE_NAME, 
        '@nid' => $this->nid,
      ));

      $this->is_valid_in_drupal = FALSE;
      return FALSE;
    }

    else {
      $record = $record[$nid];
      $this->set_id($record->fmid);
      $this->set_values_from_database($record);
      $this->is_valid_in_drupal = TRUE;
      return TRUE;
    }
  }

  /**
   * Returns a FileMaker found set in an html table, plus the pager.
   */
  function found_set() {

    $output = '';

    // First, check to see if there is a found set.
    if (!empty($_SESSION['filemaker'][$this->nid]['foundset']['table'])) {
      
      $output .= $this->pager();

      // Table of records in foundset.
      $output .= $_SESSION['filemaker'][$this->nid]['foundset']['table'];
    }

    else {
      $output .= t('No found set.');
    }

    return $output;
  }

  /**
   * Performs a FileMaker find using the default values for each FileMaker field.
   */
  public function default_find() {

    $request = $this->fx();

    // Loop through fields, setting each one with the default value.
    foreach ($this->fields as $field) {
      $default_value = $this->default_value($field->default_value, $field->default_find_value, 'find');
      if ($default_value) {
        $request->addDBParam($field->name, $default_value);
      }
    }
    
    // Store find request in session so we can perform another find
    // if record is edited, created, or deleted.
    $_SESSION['filemaker'][$this->nid]['foundset']['request'] = serialize($request);
    $this->perform_find();
  }

  /**
   * Performs a find in a FileMaker database.
   *
   * Retrieves a fx request from the $_SESSION of the user.
   *
   * Doesn't return a value, but updates the found set in the $_SESSSION, which is used in 'Browse mode'.
   */
  public function perform_find($tab = NULL) {

    // Don't perform a find if there is no saved find criteria, or we don't have a request.
    if ( ! $_SESSION['filemaker'][$this->nid]['foundset']['request']) return;
    $request = $this->unserialize_request();
    if ( ! $request) return;

    // FileMaker results will be a table.
    $head = $rows = $field_names = array();
    $this->results_header($head, $field_names);
    
    $result = $this->perform_actual_find($request);
    if ( ! $result) return;

    $this->persist_perform_find($request, $result);
    
    // Place our result data in our rows.
    $this->format_found_set_as_table($result['data'], $rows, $field_names);
    
    $results_table = $this->theme_found_set_table($head, $rows);
    $this->persist_results_table($results_table);

    // Only display success message to user if we initiated find from 'Find mode' tab.
    if ($tab == 'find') {
      drupal_set_message(t('FileMaker find performed.'));
    }
  }

  /**
   * Handles exact finds, if needed.
   */
  public function prepare_fields_for_find(array $fields_from_form) {
    $ret = array();
    foreach ($this->fields as $fm_field) {
      foreach ($fields_from_form as $field_name => $field_value) {
        $ret[$field_name] = ($fm_field->exact_search) ? '==' . $field_value : $field_value;
      }
    }
    return $ret;
  }

  /**
   * Removes Drupal form stuff, to just get the values we care about.
   */
  public function get_params_from_form_state_input($input) {
    $ret = array();
    foreach ($input as $key => $value) {
      if ($key != 'op' && $key != 'form_build_id' && $key != 'form_token' && $key != 'form_id') {
        $ret[$key] = $value;
      }
    }
    return $ret;
  }

  /**
   * Determines the default value for a field in find, browse, or create mode.
   *
   * @param $default_value
   *  User supplied default value (text) for the field.
   *
   * @param $default_mode_value
   *  One of the following values: default, username, uid, title, nid, none.
   *    If default, we use the $default_value.
   *
   * @param $mode
   *  One of the following values: find, create.
   *
   * @return
   *  Returns the appropriate default value for this field.
   */
  public function default_value($default_value, $default_mode_value, $mode, $is_checkbox = FALSE) {

    // User entered default as default value?
    if (empty($default_mode_value) || $default_mode_value == 'default') {
      return $default_value;
    }

    // Drupal username or uid as default value?
    elseif ($default_mode_value == 'username' || $default_mode_value == 'uid') {

      global $user;
      
      // If find mode, clean up reserved character (in case e-mail is being used for username).
      if ($mode == 'find') {
        $username = str_replace('@', '\@', $user->name);
      }
      else {
        $username = $user->name;
      }

      $criterion = ($default_mode_value == 'username') ? $username : $user->uid;

      return $this->exact_find_if_needed($criterion, $mode);
    }

    // Drupal node title as default value?
    elseif ($default_mode_value == 'title') {
      $node = node_load($this->nid);
      return $this->exact_find_if_needed($node->title, $mode);
    }

    // Drupal nid as default value?
    elseif ($default_mode_value == 'nid') {
      return $this->exact_find_if_needed($this->nid, $mode);
    }

    // No default value?
    elseif ($default_mode_value == 'none') {
      return '';
    }
  }

  /**
   * Might need to perform an exact find (by prefacing our criteria with '==').
   *
   * If we are not in find mode, we don't want the preceeding '=='.
   *
   * @param $criterion
   *  Our value to use in the FileMaker field. In find mode, we search for the exact
   *    value by prefacing '==' to our criteria.
   *    In create mode, we just use the value.
   *
   * @param $mode
   *  Either 'find' or 'create'.
   *
   * @return
   *  Either '==$criterion' (find mode) or '$criteria' (create mode).
   */
  public function exact_find_if_needed($criterion, $mode) {

    if ($mode == 'find') {
      return '==' . $criterion;
    }
    else {
      return $criterion;
    }
  }
  
 /**
  * Does normal save from FmBase, but adds comment stats if needed.
  *
  * This is due to a bug in the comment module, there is a patch for:
  *   http://drupal.org/node/1020658
  *
  * To avoid requiring users to hack core with the patch, we added this
  * function, and the private functions it depends upon (also in this class).
  */
  public function save() {
    if ($this->nid) {
      $this->set_comment_stats();
    }
    parent::save();
  }

  public function pager() {

    $foundset_count = $_SESSION['filemaker'][$this->nid]['foundset']['foundset_count'];
    $record_count = $_SESSION['filemaker'][$this->nid]['foundset']['record_count'];

    $pages = ceil($record_count / $this->records_per_page);

    $x_of_n = $foundset_count . ' of ' . $record_count . ' found';
    $previous_link = $this->pager_link('previous');
    $next_link = $this->pager_link('next', $pages);

    $output = '<h2>' . t('Found Set') . '</h2>';
    $output .= '<div id="">' . $previous_link . 'Page # ' . $this->page_number . ' of ' . $pages . $next_link . '</div>';
    $output .= 'Showing records ' . $x_of_n;

    return $output;
  }

  /*************************************************************
   * Formats data for FileMaker
   *************************************************************/


  /**
   * Formats data to be written to FileMaker.
   */
  public function format_all($field, &$form_state, &$record) {

      // Format date for FileMaker.
      if ($field->widget == "date") {
        $form_state['values'][$field->name] = filemaker_format_date_for_filemaker($form_state['values'][$field->name]);
      }

      // Prepare checkboxes data (which might have more than one value).
      if ($field->widget == "checkboxes") {
        $form_state['values'][$field->name] = filemaker_format_checkboxes_for_filemaker($form_state['values'][$field->name]);
      }

      // Update FileMaker_Record object with value from form.
      $record->setField($field->name, $form_state['values'][$field->name]);
  }


  /**
   * Formats data in Drupal checkboxes to be written to FileMaker.
   *
   * Turns an array of options into a string of selected options.
   *
   * @param $form_state_value
   *   An array of options.
   *   array(
   *      'option1' => 0, // not selected.
   *      'option2' => 'option2', // Selected.
   *   )
   *
   * @return
   *   A string, containing all of the selected values (separated by a new line) from
   *   the $form_state_value array.
   */
  public function format_checkboxes_for_filemaker($form_state_value) {

    // Only store selected values.
    $selected = array();

    foreach ($form_state_value as $value) {
      if ($value) {
        $selected[] = $value;
      }
    }

    // Turn array of selected values into a string.
    return implode("\r", $selected);
  }


  /**
   * Formats a Drupal date for FileMaker.
   *
   * @param $date
   *   A date value stored as an array, from Drupal form API.
   *
   * @return
   *   The date as a string: mm/dd/yyyy
   */
  public function format_date_for_filemaker($date) {
    return $date['month'] . '/' . $date['day'] . '/' . $date['year'];
  }

/**********************************************************************************
 * Private functions.
 *********************************************************************************/

  private function set_pagination() {
    $this->set_page_number();
    $this->set_records_to_skip();
  }

  private function set_page_number() {
    $this->page_number = (arg(4) == 'page') ? arg(5) : 1;
  }

  private function set_records_to_skip() {
    $this->records_to_skip = $this->records_per_page * ($this->page_number - 1);
  }

  private function pager_link($previous_or_next, $pages = NULL) {
    switch($previous_or_next) {
      case 'previous':
        if ($this->page_number < 2) return;
        return l(t('Previous'), $this->pager_path('previous')) . ' | ';
      case 'next':
        if ($this->page_number >= $pages) return;
        return ' | ' . l(t('Next'), $this->pager_path('next'));
    }
  }

  private function pager_path($previous_or_next) {
    $path = 'node/' . $this->nid . '/filemaker/browse/page/';
    $path .= ($previous_or_next == 'previous') ? $this->page_number - 1 : $this->page_number + 1; 
    return $path;
  }

  private function build_elements($ok_to_touch_filemaker) {

    if ($ok_to_touch_filemaker) {
      $this->connection = new FmConnection($this->fmcid);
      $fx = $this->fx();
      $layout_details = $fx->FMView();
      $this->fields = $this->get_fields();
      $this->set_value_list_options($layout_details);
    }
  }

  public function set_value_list_options($layout_details) {
    foreach ($layout_details['fields'] as $field) {
      if ($field['valuelist']) {
        $this->add_value_list_to_field($field['name'], $layout_details['valueLists'][$field['valuelist']]);
      }
    }
  }

  public function add_value_list_to_field($field_name, $value_list) {
    foreach ($this->fields as $field) {
       if ($field->name == $field_name) {
        
        $clean_value_list = array();
        foreach ($value_list as $key => $value) {
          $clean_value_list[$value] = $value;
        }
        
        $field->value_list_options = $clean_value_list;
        return;
       }
     }
  }

  /**
   * Returns all the fields attached to a node.
   */
  public function get_fields() {
    $field_model = new FmField();
    return $field_model->get_by_nid($this->nid);
  }

  /**
   * Retrieve field names from FileMaker using FMView.
   */
  public function field_options() {
    
    $fx = $this->fx();
    $layout_details = $fx->FMView();
    $ret = array();
     
    if ( ! ($layout_details instanceof FX_Error)) {
      foreach ($layout_details['fields'] as $field) {
        $ret[$field['name']] = $field['name'];
      }
    }
    return $ret;
  }
  
  /**
   * Adds an empty record to node_comment_statistics, if needed.
   *
   * This would not be needed at all, if it weren't for a bug in the comment module.
   */
  private function set_comment_stats() {
    if ( ! $this->comment_stats_exist()) {
      $this->insert_empty_comment_stats();
    }
  }

  /**
   * Inserts a single record into the node_comment_statistics table to prevent PHP errors.
   */
  private function insert_empty_comment_stats() {
    return db_insert('node_comment_statistics')
      ->fields(array(
        'nid' => $this->nid,
        'cid' => 0,
        'last_comment_timestamp' => 0,
        'last_comment_uid' => 0,
        'comment_count' => 0,
      ))
      ->execute();
  }

  /**
   * Tests to see if there is a record for this node in node_comment_statistics.
   *
   * @return bool
   */
  private function comment_stats_exist() {
    return db_query('SELECT 1 FROM {node_comment_statistics} WHERE nid = :nid LIMIT 1', array(':nid' => $this->nid))->fetchField();
  }

  // HELPERS FOR $this->perform_find().


  /**
   * Store table of foundset in session for browse mode to display.
   */
  private function persist_results_table($results_table) {
    $_SESSION['filemaker'][$this->nid]['foundset']['table'] = $results_table;
  }

  private function persist_perform_find(FX $request, array $result) {
    $this->persist_request($request);
    $this->persist_x_of_n_widget_data($result);
    $this->persist_time_of_find();
  }

  /**
   * Store find request in session so that we can perform a new find to recreate the found set if record in it is edited or deleted.
   */
  private function persist_request(FX $request) {
    $_SESSION['filemaker'][$this->nid]['foundset']['request'] = serialize($request);
  }

  private function results_header(array &$head, array &$field_names) {
    $this->add_show_record_number_to_header($head);
    $this->add_field_labels_to_header($head, $field_names);
    $this->add_actions_to_header($head);
  }

  /**
   * Get the time from the server, so we can refresh the found set later when it gets too old.
   */
  private function persist_time_of_find() {
    $timestamp = format_date((int) $_SERVER['REQUEST_TIME']);
    $_SESSION['filemaker'][$this->nid]['timestamp'] = $timestamp;
  }

  private function persist_x_of_n_widget_data(array $result) {
    $to_number = $this->records_to_skip + $this->records_per_page;
    $to_number = ($to_number > $result['foundCount']) ? $result['foundCount'] : $to_number;
    $foundset_count = ($this->records_to_skip) ? ($this->records_to_skip + 1) . ' to ' . $to_number : '1 to ' . $to_number;
    $_SESSION['filemaker'][$this->nid]['foundset']['foundset_count'] = $foundset_count;
    $_SESSION['filemaker'][$this->nid]['foundset']['record_count'] = $result['foundCount'];
  }

  /**
   * If valid access, add 'Actions' to the header.
   */
  private function add_actions_to_header(&$head) {   
    if (user_access('filemaker edit record') || user_access('filemaker delete record')) { 
      $head[] = t('Actions');
    }
  }

  /**
   * Loop through fields, adding the label to the head[] array and the name to the field_names[] array.
   */
  private function add_field_labels_to_header(array &$head, array &$field_names) {
    foreach ($this->fields as $field) {
      if ($field->include_field_in_search_results) {
        $head[] = array(
          'data' => check_plain($field->label),
          'class' => 'header-' . $field->name,
        );
        $field_names[] = check_plain($field->name);
      }
    }
  }

  private function unserialize_request() {
    if (class_exists('FX')) {
      return unserialize($_SESSION['filemaker'][$this->nid]['foundset']['request']);
    }
    else {
      drupal_set_message(t('Found set is not updated.'));
      return false;
    }
  }

  private function add_show_record_number_to_header(&$head) {
    if ($this->show_record_number) {
      $head[] = array(
        'data' => t('Record #'),
        'class' => 'header-record-number',
      );
    }
  }

  private function perform_actual_find(FX $request) {

    if ($this->records_to_skip) {
      $result = $request->FMSkipRecords($this->records_to_skip);
    }
    if ($request->dataParams) {
      $result = $request->FMFind();
    }
    else {
      return $request->FMFindAll();
    }

    if (filemaker_is_error($request)) {
      drupal_set_message('Request is broken.', 'error');
      return false;
    }
    return $result;
  }

  private function theme_found_set_table(array $head, array $rows) {
    $variables = array(
      'header' => $head,
      'rows' => $rows,
      'attributes' => array(),
      'caption' => '',
      'colgroups' => array(),
      'sticky' => '',
      'empty' => '',
    );
    return theme_table($variables);
  }

  private function format_found_set_as_table(array $records, array &$rows, array $field_names) {
    $i = 1;
    foreach ($records as $key => $record) {

      $record_detail = explode('.',$key);
      $current_id = $record_detail[0];

      if ($this->show_record_number) {
        $rows[$i]['row_number'] = array(
          'data' => $i + $this->records_to_skip,
          'class' => 'cell-record-number',
        );
      }

      foreach ($field_names as $field_name) {
        $rows[$i][$field_name] = array(
          'data' => check_plain($record[$field_name][0]), 
          'class' => 'cell-' . $field_name,
        );
      }
      
      $this->perform_find_action_cell($rows, $current_id, $i);
      $i++;
    }
  }

  private function perform_find_action_cell(array &$rows, $current_id, $i) {
    $action_links = '';
    if (user_access('filemaker edit record')) {
      $action_links .= l('Edit', 'node/' . $this->nid . '/filemaker/browse/' . $current_id);
    }
    if (user_access('filemaker edit record') && user_access('filemaker delete record')) {
      $action_links .= ' | ';
    }
    if (user_access('filemaker delete record')) {
      $action_links .= l('Delete', 'node/' . $this->nid . '/filemaker/browse/' . $current_id . '/delete');
    }
    if (user_access('filemaker edit record') || user_access('filemaker delete record')) {
      $rows[$i]['actions'] = $action_links;
    }
  }

}
