<?php

/**
 * @file
 * Configuration settings for the Evanced Events Importer module.
 */

/**
 * Form builder function for main configuration settings and menu callback.
 *
 * @see evanced_events_importer_admin_settings_submit()
 * @see evanced_events_importer_save_configuration()
 * @ingroup forms
 */
function evanced_events_importer_admin_settings() {

  $form = array();

  // Retrieve any existing configuration settings
  // from the database and load them as an object.
  $main_config_settings = evanced_events_importer_load_configuration('evanced_events_importer');

  // Create an array to hold the input format options.
  $input_format_options = array();

  // Retrieve a list of input formats and set the data as an array.
  $input_formats_array = filter_formats();

  // Make a loop for each input format in the array.
  foreach ($input_formats_array as $input_format_key => $input_format) {
    // Add the appropriate input format data to the options array.
    $input_format_options[$input_format->format] = $input_format->name;
  }

  // Define the form fields for the main configuration screen.

  $form['instructions'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="eei_instructions"> <p>' . t('This module imports an EXML feed from <a href="@external-url-evanced">Evanced Events</a>, which it uses to create and update event nodes in Drupal. Once it is properly configured, event data will be imported every time cron is run.', array('@external-url-evanced' => url('http://evancedsolutions.com/our-solutions/events'))) . '</p> <p>' . t("To learn more about the EXML feed and this module, review the accompanying <a href='@internal-url-readme' target='_blank'>Read Me</a> file. For answers to questions regarding Evanced Events itself, please contact your Evanced representative and/or refer to the Events Manual.", array("@internal-url-readme" => url(drupal_get_path("module", "evanced_events_importer") . "/README.txt"))) . '</p> </div>',
  );

  $form['exml_feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('EXML Feed'),
    '#description' => t('Enter the full URL of the Evanced EXML feed.'),
    '#default_value' => ((empty($main_config_settings)) ? '' : $main_config_settings[0]->exml_feed_url),
    '#size' => 35,
    '#maxlength' => 500,
  );

  $form['content_type_name'] = array(
    '#type' => 'select',
    '#title' => t('Content type'),
    '#options' => node_type_get_names(),
    '#default_value' => ((empty($main_config_settings)) ? array() : array($main_config_settings[0]->content_type_name)),
    '#description' => t('Select the content type into which the EXML data will be imported.'),
  );

  $form['publisher_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('User ID of Publishing Author'),
    '#description' => t('Enter the Drupal user ID of the account that will be credited with publishing these events.'),
    '#default_value' => ((empty($main_config_settings)) ? '' : $main_config_settings[0]->publisher_uid),
    '#size' => 4,
  );

  $form['body_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Body Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['body_settings']['input_format'] = array(
    '#type' => 'select',
    '#title' => t('Input format'),
    '#options' => $input_format_options,
    '#default_value' => ((empty($main_config_settings)) ? array() : array($main_config_settings[0]->input_format)),
    '#description' => t('Select the input format that will be applied to the main body of this event.'),
  );

  $form['body_settings']['relative_links_domain'] = array(
    '#type' => 'hidden',
    '#title' => t('Domain of Relative Links'),
    '#description' => t('In the description, should absolute links for a specific domain be transformed into relative links? If so, enter the appropriate domain above (example: www.mydomain.com). Otherwise, leave this field blank.'),
    '#default_value' => ((empty($main_config_settings)) ? '' : $main_config_settings[0]->relative_links_domain),
    '#size' => 35,
  );

  $form['body_settings']['striplinks_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Strip all links from the event description.'),
    '#default_value' => ((empty($main_config_settings)) ? '' : $main_config_settings[0]->striplinks_flag),
    '#description' => t('By checking this box, all HTML links will be stripped from the body of the event description.'),
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;

}


/**
 * Form builder function for XML Mapper settings and menu callback.
 *
 * @see evanced_events_importer_admin_settings_xmlmapper_submit()
 * @see evanced_events_importer_save_configuration()
 * @ingroup forms
 */
function evanced_events_importer_admin_settings_xmlmapper() {

  $form = array();
  $field_count = 0;
  $secondary_node_name = '';

  // Retrieve any existing configuration settings
  // from the database and load them as objects.
  $main_config_settings = evanced_events_importer_load_configuration('evanced_events_importer');
  $xmlmapper_config_settings = evanced_events_importer_load_configuration('evanced_events_importer_xmlmapper');

  // Ensure an XML feed was specified on the
  // previous configuration screen before continuing.
  if ((!empty($main_config_settings)) && (trim($main_config_settings[0]->exml_feed_url) != '')) {

    // Now add css styling for the admin section.
    drupal_add_css(drupal_get_path('module', 'evanced_events_importer') . '/evanced_events_importer.admin.css');

    // Add the javascript that does the fancy text moving.
    drupal_add_js(drupal_get_path('module', 'evanced_events_importer') . '/evanced_events_importer.admin.js');

    // Reset the array.
    $xml_elements_array = array();

    // Initialize the XML parser (Document Object Model (DOM) library).
    $doc = new DOMDocument();

    // Load the XML file.
    $doc->load(trim($main_config_settings[0]->exml_feed_url));

    // Retrieve the root element in the XML file and set it as an object.
    $root_element = $doc->getElementsByTagName("*");

    // Create an array containing the name of each unique element in the XML feed.
    $xml_element_names_array = evanced_events_importer_xml_tag_names($root_element, $secondary_node_name);

    // Retrieve field data and taxonomy data
    // associated with the specified content type.
    $content_type_fields_array = evanced_events_importer_content_type_fields($main_config_settings[0]->content_type_name);

    // Make a loop for each XML element and update the array, using
    // the element name as both the array key and the array value.
    foreach ($xml_element_names_array as $key => $element) {
      $xml_elements_array[trim($element)] = trim($element);
    }

    // Make a loop for each XML element that was already
    // mapped to a content type field in the database.
    foreach ($xmlmapper_config_settings as $element_object) {

      // Split the string of XML element names into an array, using the comma as a delimiter.
      $elements_array = explode(',', $element_object->xml_elements);

      // Make a loop for each XML element already
      // associated with a content type field.
      foreach ($elements_array as $element) {
        // Remove the current XML element from the
        // master list, since it is already in use
        // and will appear in the righthand column.
        unset($xml_elements_array[trim($element)]);
      }

    }

    // define the form fields for the XML mapper screen.

    $form['instructions_div_open'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="eei_instructions">',
    );

    $form['instructions1'] = array(
      '#type' => 'markup',
      '#markup' => '<p>' . t('Select one of the XML tag names in the lefthand column. Then map it to one of the content type fields in the righthand column using the arrows. Taxonomy fields can have up to three XML elements mapped to them.') . '</p>',
    );

    $form['instructions2'] = array(
      '#type' => 'markup',
      '#markup' => "<p><b>" . t("Helpful hints:") . "</b></p> <ul><li>" . t("Detailed instructions can be found in this module's <a href='@internal-url-readme' target='_blank'>Read Me</a> file.", array("@internal-url-readme" => url(drupal_get_path("module", "evanced_events_importer") . "/README.txt"))) . "</li> <li>" . t("Ensure that you've created a field for storing the Evanced ID as well.") . "</li> <li>" . t("For taxonomy terms to get mapped correctly, the name of the taxonomy term in Drupal needs to <i>exactly match</i> the one found in Evanced.") . "</li> <li>" . t("Review the <i>Suggested Settings</i> below before you start mapping the EXML elements.") . " </li></ul>",
    );

    $form['instructions_div_close'] = array(
      '#type' => 'markup',
      '#markup' => '</div><!-- end-div eei_instructions -->',
    );

    $form['instructions_additional'] = array(
      '#type' => 'fieldset',
      '#title' => t('Suggested Settings'),
      '#description' => '<div><p>' . t('Typically, most event content types contain the same basic fields. With this in mind, the example below shows a common mapping configuration.') . '</p></div> <p><b>' . t('Mapped elements:') . '</b></p>',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['instructions_additional']['suggested_settings_example'] = array(
      '#type' => 'markup',
      '#markup' => _evanced_events_importer_xmlmapper_example_table(),
    );

    $form['selected_options'] = array(
      '#type' => 'hidden',
      '#default_value' => '',
    );

    $form['div1_open'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="xml_tags">',
    );

    $form['exml_list'] = array(
      '#type' => 'select',
      '#title' => t('EXML elements'),
      '#options' =>  array_filter($xml_elements_array),
      '#size' => '28',
      '#default_value' => '',
      '#description' => '',
    );

    $form['div1_close'] = array(
      '#type' => 'markup',
      '#markup' => '</div><!-- end-div xml_tags -->' . "\n",
    );

    $form['div2_open'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="mapped_fields">',
    );

    // Make a loop for each field associated with this content type.
    foreach ($content_type_fields_array as $key => $data) {

      // Set the size of the dropdown menu as a variable.
      $field_size = 1;

      // Determine if this is a taxonomy field.
      if ($data['field_type'] == 'taxonomy') {
        // Update the field size.
        $field_size = 3;
      }

      // Create the field ID for the following form field.
      $field_id = 'edit-evanced-events-importer-field' . $key . '-xml-elements';

      // Set the opening div for this row of content (which
      // will contain fields and toggle buttons) as a variable.
      $content_type_row_opening_div = '<div class="content_type_row">';

      // Determine if this is the very first row.
      if ($key == 0) {
        // Add the section header to the first row's div.
        $content_type_row_opening_div = '<div class="content_type_row"><div><label>' . t('Mapped elements:') . ' </label></div>';
      }

      $form['content_type_row' . $key . '_open'] = array(
        '#type' => 'markup',
        '#markup' => $content_type_row_opening_div,
      );

      // Define the toggle buttons.
      $form['evanced_events_importer_' . preg_replace('/[^a-zA-Z0-9\s]/', '', $data['field_name_alias']) . '_open'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="toggle_buttons"><input type="button" id="edit-evanced-events-importer-button-remove' . $key . '" value="&#060;-" /><input type="button" id="edit-evanced-events-importer-button-add' . $key . '" value="-&#062;" /></div> ',
      );

      $form['float_field' . $key . '_open2'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="content_type_field">',
      );

      // Reset variables.
      $options_array = array();
      $xml_elements_list = '';

      // Make a loop for each XML element that was already
      // mapped to a content type field in the database.
      foreach ($xmlmapper_config_settings as $xmlmapper_object) {

        // Determine if the field alias matches the one currently being processed.
        if (trim($xmlmapper_object->node_field_name_alias) == trim($data['field_name_alias'])) {

          // Set the full comma-delimited list of XML elements as a variable.
          $xml_elements_list = trim($xmlmapper_object->xml_elements);

          // Split the XML element names into an array, using the comma as a delimiter.
          $elements_array = explode(',', $xml_elements_list);

          // Make a loop for each XML element
          // associated with the curent content type field.
          foreach ($elements_array as $element) {
            // Add the current XML element to an array.
            $options_array[$element] = $element;
          }

          break;

        }

      }

      $form['evanced_events_importer_field' . $key . '_xml_elements'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#options' => $options_array,
        '#size' => $field_size,
        '#default_value' => array(''),
        '#description' => '',
      );

      $form['evanced_events_importer_field' . $key . '_xml_elements_list'] = array(
        '#type' => 'hidden',
        '#default_value' => $xml_elements_list,
        '#attributes' => array('id' => 'edit-evanced-events-importer-field' . $key . '-xml-elements-list'),
      );

      $form['evanced_events_importer_field' . $key . '_node_field_label'] = array(
        '#type' => 'hidden',
        '#default_value' => $data['field_label'],
      );

      $form['evanced_events_importer_field' . $key . '_node_field_name'] = array(
        '#type' => 'hidden',
        '#default_value' => $data['field_name'],
      );

      $form['evanced_events_importer_field' . $key . '_node_field_name_alias'] = array(
        '#type' => 'hidden',
        '#default_value' => $data['field_name_alias'],
      );

      $form['evanced_events_importer_field' . $key . '_node_field_type'] = array(
        '#type' => 'hidden',
        '#default_value' => $data['field_type'],
      );

      $form['float_field' . $key . '_close2'] = array(
        '#type' => 'markup',
        '#markup' => '</div><!-- close-div --> ' . "\n",
      );

      $form['evanced_events_importer_' . preg_replace('/[^a-zA-Z0-9\s]/', '', $data['field_name_alias']) . '_close'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="content_type_field_label"> = ' . t('@field-label', array('@field-label' => $data['field_label'])) . '</div><!-- end-div --> ' . "\n",
      );

      $form['content_type_row' . $key . '_close'] = array(
        '#type' => 'markup',
        '#markup' => '</div><!-- end-div -->' . "\n",
      );

      $form['double_breaker' . $key] = array(
        '#type' => 'markup',
        '#markup' => '<div class="clear_float"></div>' . "\n",
      );

      // Set the current key value as a variable in order
      // to keep track of how many field rows there are.
      $field_count = $key;

    }


    $form['evanced_events_importer_fieldcount'] = array(
      '#type' => 'hidden',
      '#default_value' => $field_count,
    );

    $form['div2_close'] = array(
      '#type' => 'markup',
      '#markup' => '</div><!-- end-mapped_fields div -->' . "\n",
    );

    $form['double_break2'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="clear_float"></div>' . "\n",
    );

    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );

  // Otherwise, no XML feed has been specified yet.
  }
  else {

    // Display a message, instructing the user to enter an XML feed first.
    $form['missing_xml_feed_message'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="missing-xml-feed-message"><p>' . t('Before you can start using the XML Mapper, you need to enter the <a href="@internal-url-settings">EXML Feed</a> URL that comes with your Evanced Events installation. To learn more about the EXML feed, please contact your Evanced representative and/or refer to the Events Manual.', array('@internal-url-settings' => url('admin/config/development/evanced-events-importer'))) . '</p></div>',
    );

  }

  return $form;

}


/**
 * Deletes all configuration settings from a specified database table.
 *
 * @param $db_table_name
 *   The name of the database table to be emptied.
 */
function evanced_events_importer_delete_settings($db_table_name) {
  $sql = 'DELETE FROM {' . $db_table_name . '}';
  db_query($sql);
}


/**
 * Saves records to the database based upon the schema passed to it.
 *
 * @param $db_table_name
 *   The name of the database table to be updated.
 *
 * @param $config_settings
 *   An object containing the values to be saved to the database table.
 */
function evanced_events_importer_save_configuration($config_settings, $db_table_name) {
  drupal_write_record($db_table_name, $config_settings);
}


/**
 * Form submission handler for evanced_events_importer_admin_settings().
 *
 * Saves the main configuration settings to the database.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @see evanced_events_importer_admin_settings()
 * @see evanced_events_importer_save_configuration()
 * @ingroup forms
 */
function evanced_events_importer_admin_settings_submit($form, &$form_state) {
  // Specify the name of the database table that should be updated.
  $db_table_name = 'evanced_events_importer';
  // Create an object containing the configuration settings.
  $config_settings = (object)$form_state['values'];
  // Clear out any existing settings from the database table.
  evanced_events_importer_delete_settings($db_table_name);
  // Save the configuration settings to the database.
  evanced_events_importer_save_configuration($config_settings, $db_table_name);
  // Ensure that a confirmation message is displayed.
  drupal_set_message(t('The main configuration settings have been saved.'), 'status');
  drupal_set_message(t('Review the <a href="@internal-url-xml-settings">XML Mapper</a> settings to see if these latest changes have affected them.', array('@internal-url-xml-settings' => url('admin/config/development/evanced-events-importer/xmlmapper'))), 'status');
}


/**
 * Form submission handler: evanced_events_importer_admin_settings_xmlmapper().
 *
 * Saves the XML Mapper configuration settings to the database.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @see evanced_events_importer_admin_settings_xmlmapper()
 * @see evanced_events_importer_save_configuration()
 * @ingroup forms
 */
function evanced_events_importer_admin_settings_xmlmapper_submit($form, &$form_state) {

  $date_field_name   = '';
  $date_field_format = '';

  // Specify the name of the database table that should be updated.
  $db_table_name = 'evanced_events_importer_xmlmapper';

  // Clear out any existing settings from the database table.
  evanced_events_importer_delete_settings($db_table_name);

  // Make a loop for each field that needs to be mapped to one or more XML elements.
  for ($i = 0; $i <= $form_state['values']['evanced_events_importer_fieldcount']; $i++) {

    // Set the field type as a variable.
    $field_type = trim($form_state['values']['evanced_events_importer_field' . $i . '_node_field_type']);

    // Determine if this is a date field.
    if ($field_type == 'date') {

      // Set the name of the date field as a variable.
      $date_field_name = $form_state['values']['evanced_events_importer_field' . $i . '_node_field_name'];

      // Ensure the date field database table exists.
      if (db_table_exists('field_data_' . $date_field_name)) {

        // Retrieve the schema of this table.
        $date_dbtable_array = drupal_get_schema('field_data_' . $date_field_name);
 
        // Set the date field's field-type as a
        // variable (i.e. int, varchar or datetime).
        $date_field_format = $date_dbtable_array['fields'][$date_field_name . '_value']['type'];

      }

    }

    // Create an array containing data, which maps one or
    // more XML elements to a specific content type field.
    $config_settings_array = array('xml_elements'          => $form_state['values']['evanced_events_importer_field' . $i . '_xml_elements_list'],
                                   'node_field_label'      => $form_state['values']['evanced_events_importer_field' . $i . '_node_field_label'],
                                   'node_field_name'       => $form_state['values']['evanced_events_importer_field' . $i . '_node_field_name'],
                                   'node_field_name_alias' => $form_state['values']['evanced_events_importer_field' . $i . '_node_field_name_alias'],
                                   'node_field_type'       => $field_type .
                                                             (((trim($field_type) == 'date') &&
                                                               (trim($date_field_format) != '')) ? ' ' . trim($date_field_format) : ''),
                                   'last_updated'          => REQUEST_TIME,
    );

    // Create an object that contains the
    // configuration settings for this particular row.
    $config_settings = (object)$config_settings_array;

    // Save the configuration settings to the database.
    evanced_events_importer_save_configuration($config_settings, $db_table_name);

  }

  // Ensure that a confirmation message is displayed.
  drupal_set_message(t('The XML Mapper configuration settings have been saved.'), 'status');
  drupal_set_message(t('These settings will be applied the next time cron is run.'), 'status');

}


/**
 * Retrieves field and taxonomy data for a specific content type.
 *
 * @param $content_type_name
 *   The name of a specific content type.
 *
 * @return
 *   An array populated with field and
 *   taxonomy data for a specific content type.
 */
function evanced_events_importer_content_type_fields($content_type_name) {

  $counter = 0;

  // Create an array that will hold all of the
  // fields associated with this content type.
  $fields_array = array();

  // Retrieve title, body and other fields associated with this content type.
  // $content_type_info_array = _content_type_info();
  $content_type_info_array = field_info_instances('node', $content_type_name);
  // Retrieve taxonomy data associated with this content type.
  // $content_type_taxonomy_array = taxonomy_get_vocabularies($content_type_name);

  // Create an object for referencing the field data.
  // $content_type_object = $content_type_info_array['content types'][$content_type_name];

  // Determine if a title is associated with this content type.
  // if ($content_type_info_array['has_title'] == 1) {

  // Append the title data to the array.
  $fields_array[$counter]['field_label'] = t('@title-label', array('@title-label' => 'Title'));
  $fields_array[$counter]['field_name']  = 'title';
  $fields_array[$counter]['field_name_alias'] = 'title';
  $fields_array[$counter]['field_type']  = 'text';

  $counter++;

  // }

  // Make a loop for each field associated with this content type.
  foreach ($content_type_info_array as $field_name => $data) {

    // Set the field label as a variable.
    $field_label = $data['label'];

    // Set the field type as a variable.
    $field_type = (isset($data['display']['default']['module']) ? $data['display']['default']['module'] : $data['widget']['module']);

    // Determine if this is a date field.
    if ($field_type == 'date') {

      // Create an array containing all the components
      // that the following date will be split into.
      $date_components_array = array('date'      => 'Start and End Date',
                                     'starttime' => 'Start Time',
                                     'endtime'   => 'End Time');

      // Make a loop for each date component.
      foreach ($date_components_array as $field_suffix => $label_alias) {

        // Append the date data to the array.
        $fields_array[$counter]['field_label'] = t('@label-alias (@field-label)', array('@label-alias' => $label_alias, '@field-label' => $field_label));
        $fields_array[$counter]['field_name']  = $field_name;
        $fields_array[$counter]['field_name_alias'] = $field_name . '_' . $field_suffix;
        $fields_array[$counter]['field_type']  = $field_type;

        $counter++;

      }

    }
    // Otherwise, determine if this is a taxonomy term field.
    else if ($field_type == 'taxonomy') {

      // The following query retrieves configuration
      // data that's specific to this taxonomy term field.
      $sql = "SELECT data from {field_config} WHERE field_name = :taxonomy_term_field_name ";

      // Run the query.
      $result = db_query($sql, array(':taxonomy_term_field_name' => $field_name));

      // Loop through the data in the query result.
      foreach ($result as $data) {
        // The configuration info for the field is saved
        // as serialized data. Restore it back to an array.
        $taxonomy_field_data_array = unserialize($data->data);
      }

      // Using the machine name (pulled from the query
      // data), retrieve information about the vocabulary
      // associated with this taxonomy term field.
      $vocab_array = taxonomy_vocabulary_machine_name_load($taxonomy_field_data_array['settings']['allowed_values']['0']['vocabulary']);

      // Append the taxonomy data to the array.
      $fields_array[$counter]['field_label'] = t('Taxonomy: @field-label', array('@field-label' => $vocab_array->name));
      $fields_array[$counter]['field_name']  = $field_name;
      $fields_array[$counter]['field_name_alias'] = 'taxonomy' . $vocab_array->vid;
      $fields_array[$counter]['field_type']  = 'taxonomy';

      $counter++;

    }
    // Otherwise, this isn't a date or taxonomy field.
    else {

      // Append the field data to the array.
      $fields_array[$counter]['field_label'] = t('@field-label', array('@field-label' => $field_label));
      $fields_array[$counter]['field_name']  = $field_name;
      $fields_array[$counter]['field_name_alias'] = $field_name;
      $fields_array[$counter]['field_type']  = $field_type;

      $counter++;

    }

  }

  // Return the array of field data.
  return $fields_array;

}


/**
 * Generates a table showing the most common XML Mapper configuration settings.
 *
 * @return
 *   A themed HTML table containing the suggested configuration settings.
 */
function _evanced_events_importer_xmlmapper_example_table() {

  // Populate an array with the table headers.
  $headers = array("exml_element" => t('EXML element'), "separator" => ' ', "ct_field" => t('Content type field'));

  // Populate an array with the data that will appear in the table.
  $rows = array();
  $rows[] = array("exml_element" => "title", "separator" => "=", "ct_field" => t("Node title"));
  $rows[] = array("exml_element" => "description", "separator" => "=", "ct_field" => t("Node body"));
  $rows[] = array("exml_element" => "date1", "separator" => "=", "ct_field" => t("<i>Start &amp; End Date</i>"));
  $rows[] = array("exml_element" => "time", "separator" => "=", "ct_field" => t("<i>Start Time</i>"));
  $rows[] = array("exml_element" => "endtime", "separator" => "=", "ct_field" => t("<i>End Time</i>"));
  $rows[] = array("exml_element" => "id", "separator" => "=", "ct_field" => t("Evanced ID field"));
  $rows[] = array("exml_element" => "library<br /> location", "separator" => "=", "ct_field" => t("Taxonomy: Location"));
  $rows[] = array("exml_element" => "agegroup1<br /> agegroup2<br /> agegroup3", "separator" => "=", "ct_field" => t("Taxonomy: Age group"));
  $rows[] = array("exml_element" => "eventtype1<br /> eventtype2<br /> eventtype3", "separator" => "=", "ct_field" => t("Taxonomy: Event type"));

  // generate the HTML table.
  // $table = theme('table', $headers, $rows);
  $table = theme('table', array('header' => $headers, 'rows' => $rows));

  return $table;

}