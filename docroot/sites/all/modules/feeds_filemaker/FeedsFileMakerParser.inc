<?php

/**
 * @file
 * Contains the FeedsFileMakerParser class.
 */

/**
 * Parses a FileMaker result set
 */
class FeedsFileMakerParser extends FeedsParser {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {

    $result = $fetcher_result->getRaw();

    $parser_config = $this->getConfig();

    $records = array();

    // Pull out each of the fields from the mapping and insert the field value
    // from FileMaker.
    foreach ($result as $record) {

      $multi_value = '';

      foreach ($parser_config['mappings'] as $key => $filemaker_field) {

        // FileMaker fields with multiple values require different logic to
        // extract the values.  FileMaker stores them in a single string, which
        // is delimited by \n.
        if($filemaker_field['is_multi_value']) {

          // Split the field using \n as delimiter.
          $field_value = explode("\n", $record->getField($filemaker_field['mapping']));

          // Store the multiple values in array format.
          foreach($field_value as $value) {
            $multi_value[] = $value;
          }
          $fields[$key] = $multi_value;
        }
        else {
          // Single value field.
          $fields[$key] = $record->getField($filemaker_field['mapping']);
        }
      }
      $records[] = $fields;
    }
    return new FeedsParserResult($records, $source->feed_nid);
  }

  /**
   * Override parent::getMappingSources().
   */
  public function getMappingSources() {

    $mappings = $this->filterMappings(feeds_importer($this->id)->processor->config['mappings']);

    // Find out what the next number for filemakerfield:n should be.
    $next = 0;
    if (!empty($mappings)) {
      $keys = array_keys($mappings);
      foreach ($keys as &$key) {
        $key = str_replace('filemakerfield:', '', $key);
      }
      $next = max($keys) + 1;
    }

    // This select item is added to the dropdown on the mapping page.
    return array(
      'filemakerfield:' . $next => array(
        'name' => t('FileMaker Field'),
        'description' => t('Allows you to choose a FileMaker field that will populate this field.  The specific FileMaker field is selected in the FileMaker fetcher configuration area.'),
      ),
    ) + parent::getMappingSources();
  }

  /**
   * Filters mappings, returning the ones that belong to us.
   *
   * @param array $mappings
   *   A mapping array from a processor.
   *
   * @return array
   *   An array of mappings keyed source => target.
   */
  protected function filterMappings($mappings) {
    $our_mappings = array();
    foreach ($mappings as $mapping) {
      if (strpos($mapping['source'], 'filemakerfield:') === 0) {
        $our_mappings[$mapping['source']] = $mapping['target'];
      }
    }
    return $our_mappings;
  }

  /**
   * Override parent::getSourceElement() to use only lower keys.
   */
  public function getSourceElement(FeedsSource $source, FeedsParserResult $result, $element_key) {
    return parent::getSourceElement($source, $result, drupal_strtolower($element_key));
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults() {
    // Intentionally empty.  No source defaults.
    return array();
  }

  /**
   * Source form.
   */
  public function sourceForm($source_config) {
    $fetcher_config = feeds_importer($this->id)->fetcher->config;
    $form['error'] = array();

    // No password is perfectly valid.  If no password has been entered,
    // set to empty string.
    if(!isset($fetcher_config['password'])) {
      $fetcher_config['password'] = '';
    }

    // Get a list of fields that are available on the FileMaker layout (the
    // layout is specified by the user in the fetcher config).  We obviously
    // can only connect to the API if we have all of the connection info.
    if (!isset($fetcher_config['database'], $fetcher_config['hostspec'], $fetcher_config['layout'],
    $fetcher_config['username'], $fetcher_config['password'])) {
      $form['error']['#markup'] = t('Please enter the database, url, username, password and layout in the FileMaker fetcher first. <br />');
      return $form;
    }

    // Ensure that the FileMaker API files have been added to the library.
    // Otherwise we can't connect to FileMaker.
    $library = libraries_load('filemaker');
    if (!$library || empty($library['loaded'])) {
      $form['error']['#markup'] = t('Unable to load FileMaker API.  Please ensure it has been added to the /libraries/filemaker folder <br />');
      return $form;
    }

    // Connect to the API and grab the fields.
    $fm = new FileMaker($fetcher_config['database'], $fetcher_config['hostspec'], $fetcher_config['username'], $fetcher_config['password']);

    if (Filemaker::isError($fm)) {
      $form['error']['#markup'] = t('An error has occured whilst connecting to FileMaker.  @error_message <br />',
        array('@error_message' => $fm->message));
      return $form;
    }

    $fm_layout = $fm->getLayout($fetcher_config['layout']);

    if (Filemaker::isError($fm_layout)) {
      $form['error']['#markup'] = t('An error has occured whilst retrieving the layout.  !error_message <br />',
        array('!error_message' => $fm_layout->message));
      return $form;
    }

    $fields = $fm_layout->getFields();

    if (Filemaker::isError($fields)) {
      $form['error']['#markup'] = t('An error has occured whilst retrieving the list of fields from FileMaker.  !error_message <br />',
        array('!error_message' => $fields->message));
      return $form;
    }

    // Create a select option for each FileMaker field.
    $field_options = array();
    foreach ($fields as $field_name => $field) {
      $field_options[$field_name] = $field_name;
    }

    // Check if we actually received fields from the API.
    if (empty($field_options)) {
      $form['error']['#markup'] = t('No fields exist on layout !layout',
        array('!layout' => $fetcher_config['layout']));
      return $form;
    }

    // Sort the field options alphabetically (case insensitively).
    uksort($field_options, 'strcasecmp');

    // The user chooses Drupal fields as targets from within the mappings
    // section.  Here we will give the user the ability to specify which
    // FileMaker fields map to the Drupal fields (i.e. "sources" to "targets")
    $mappings = $this->filterMappings(feeds_importer($this->id)->processor->config['mappings']);

    if (empty($mappings)) {
      $form['error']['#markup'] = t('No mappings are defined.  You can define mappings !here.',
        array('!here' => l(t('here'), 'admin/structure/feeds/' . $this->id . '/mapping')));
      return $form;
    }

    $form['mappings'] = array(
      '#type' => 'fieldset',
      '#title' => 'Please map the Drupal fields to their respective FileMaker fields.',
      '#tree' => TRUE,
    );

    // Add the dropdowns to the form.
    foreach ($mappings as $mapping_key => $mapping) {
      $form['mappings'][$mapping_key] = array(
        '#type' => 'fieldset',
        '#title' => "$mapping ($mapping_key)",
      );
      $form['mappings'][$mapping_key]['mapping'] = array(
        '#type' => 'select',
        '#title' => "Choose a FileMaker field.",
        '#options' => $field_options,
        '#required' => TRUE,
        '#default_value' => isset($this->config['mappings'][$mapping_key]['mapping']) ? $this->config['mappings'][$mapping_key]['mapping'] : '',
      );
      $form['mappings'][$mapping_key]['is_multi_value'] = array(
        '#type' => 'checkbox',
        '#title' => t("Multiple Value field."),
        '#description' => t('If this field has multiple values in FileMaker then tick this box.'),
        '#return_value' => TRUE,
        '#default_value' => isset($this->config['mappings'][$mapping_key]['is_multi_value']) ? $this->config['mappings'][$mapping_key]['is_multi_value'] : FALSE,
      );
    }
    return $form;
  }

  /**
   * Define default configuration.
   */
  public function configDefaults() {
    return array(
      'mappings' => array(),
    );
  }

  /**
   * Build configuration form.
   */
  public function configForm(&$form_state) {
    $config = $this->getConfig();
    return $this->sourceForm($config);
  }
}
