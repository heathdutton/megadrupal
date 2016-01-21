<?php

/**
 * @file
 * When cron is run, nodes are added, updated or deleted based on an EXML feed.
 *
 * The following functions are activated when cron runs. Data is extracted from
 * an EXML feed provided by the software package Evanced Events. Based on that
 * data, event nodes are then either added, updated or deleted in Drupal.
 */

/**
 * Determines which term ids should be selected for each taxonomy term field.
 *
 * @param $node_values_array
 *   An array containing event data that will be used to create a Drupal node.
 *
 * @return
 *   An updated array of event data.
 */
function evanced_events_importer_taxonomy_ids($node_values_array) {

  $synonym_field_name = '';

  // Retrieve any taxonomy configuration settings
  // from the database and load them as an object.
  $xmlmapper_config_settings = evanced_events_importer_load_configuration('evanced_events_importer_xmlmapper', 'taxonomy');

  // The following query retrieves any term ID where
  // the term name matches the evanced category name.
  $sql = "SELECT  t.tid FROM {taxonomy_term_data} t WHERE  t.vid = :vocab_id1 AND  LOWER(t.name) = LOWER(:name1) ";

  // Load an array containing all the custom fields associated with taxonomy terms.
  $vocabularies_fields_array = field_info_instances('taxonomy_term');

  // Loop through all the vocabularies in the array.
  foreach ($vocabularies_fields_array as $vocabulary_machine_name => $vocab_field_array) {

    // Loop through all the custom fields in the current vocabulary.
    foreach ($vocab_field_array as $field_name => $value) {

      // Determine if the name of the current field
      // indicates that it is a synonym field. 
      if (strpos($field_name, 'syn') !== FALSE) {
        $synonym_field_name = trim($field_name); // Remember the name of the synonym field.
        break; // We found what we were looking for. Break the current loop.
      }
    }
  }

  // Determine if a synonym field was added to the taxonomy term form.
  if (trim($synonym_field_name) != '') {

    // The following can be joined to the taxonomy term query, so
    // that both the term name and its synonym field are searched.
    $sql_synonym = " UNION " .
                   "SELECT  t.entity_id AS tid " .
                     "FROM {field_data_" . $synonym_field_name . "} t " .
                    "WHERE (t.bundle = :vocab_machine_name OR t.bundle = :vocab_id2)" .
                      "AND  LOWER(t." . $synonym_field_name . "_value) = LOWER(:name2) ";

    // Append the synonym sql to the main sql query.
    $sql .= $sql_synonym;

  }

  // Make a loop for each XML element that has been
  // identified as representing taxonomy data.
  foreach ($xmlmapper_config_settings as $xmlmapper_object) {

    // Reset the array of taxonomy term ids.
    $taxonomy_ids_array = array();

    // Create an object for the current vocabulary.
    $vocab_object = taxonomy_vocabulary_load(str_replace('taxonomy', '', trim($xmlmapper_object->node_field_name_alias)));

    // Split the XML elements into an array, using
    // the comma as a delimiter, and then loop through
    // each XML element that has been mapped.
    foreach (explode(",", $xmlmapper_object->xml_elements) as $object_key => $object_xml_elements) {

      // Determine if a synonym field was added to taxonomy terms.
      if (trim($synonym_field_name) != '') {
        // Run the query, which retrieves term
        // ids associated with the current event.
        $result = db_query($sql, array(':vocab_id1' => trim($vocab_object->vid),
                                       ':name1' => trim($node_values_array[$object_xml_elements]),
                                       ':vocab_machine_name' => trim($vocab_object->machine_name),
                                       ':vocab_id2' => trim($vocab_object->vid),
                                       ':name2' => trim($node_values_array[$object_xml_elements]),
                                       ));
      }
      // Otherwise, no synonym field was found.
	  else {
        // Run the query, which retrieves term
        // ids associated with the current event.
        $result = db_query($sql, array(':vocab_id1' => trim($vocab_object->vid),
                                       ':name1' => trim($node_values_array[$object_xml_elements]),
                                       ));
      }

      // Make loop for each term ID in the query result.
      foreach ($result as $data) {

        // Append the term ID to the master taxonomy array.
        $taxonomy_ids_array[] = $data->tid;

        // Determine if the 'location' element returns more than one record. That
        // means the location vocabulary contains multiple terms with the same name.
        if (($object_xml_elements == 'location') && ($result->rowCount() > 1)) {

          // Retrieve the parents of the current term.
          $parents = taxonomy_get_parents($data->tid);

          // Loop through the list of parent terms.
          foreach($parents as $parent_key => $parent_object) {

            // Determine if this location's parent doesn't match
            // the library name passed through the EXML feed.
            if (strtolower(trim($parent_object->name)) != strtolower(trim($node_values_array['library']))) {
              // Find the matching term ID in the array (and remember the key).
              if(($delete_key = array_search($data->tid, $taxonomy_ids_array)) !== FALSE) {
                // Remove the erroneous term from the array.
                unset($taxonomy_ids_array[$delete_key]);
                break; // Break the current loop.
              }
            }
          }
        }
      }
    }

    // Remove any duplicate taxonomy ids from the array.
    $taxonomy_ids_array = array_keys(array_flip($taxonomy_ids_array));

    // Add the tids associated with this
    // taxonomy term field to the node data.
    $node_values_array[$xmlmapper_object->node_field_name] = $taxonomy_ids_array;

  }

  return $node_values_array;

}


/**
 * Processes offsite address data and adds it to the main array.
 *
 * @param $node_values_array
 *   An array containing all the event data that
 *   will be used to create a single Drupal node.
 *
 * @return
 *   An updated array of event data.
 */
function evanced_events_importer_offsite_address($node_values_array) {

  $element_address = '';
  $street2_flag = FALSE;

  // Retrieve any address configuration settings
  // from the database and load them as an object.
  $xmlmapper_config_settings = evanced_events_importer_load_configuration('evanced_events_importer_xmlmapper', 'addressfield');

  // Set the address-related XML elements as a variable.
  $element_address = ((count($xmlmapper_config_settings) > 0) ? trim($xmlmapper_config_settings[0]->xml_elements) : "");

  // Determine if any addresses data exists.
  if ((trim($element_address) != '') && (trim($node_values_array[$element_address]) != '')) {

    // Replace any incorrectly-formatted line break tags with the correct version.
    $node_values_array[$element_address] = str_replace(chr(60) . 'br' . chr(62),  '<br />', $node_values_array[$element_address]);
    $node_values_array[$element_address] = str_replace('<br/>', '<br />', $node_values_array[$element_address]);

    // Split the address string into an array,
    // using the html line breaks as a delimiter.
    $address_offsite_array = explode('<br />', $node_values_array[$element_address]);

    // Determine if there is a 'street 2' value in this address.
    if ((count($address_offsite_array) >= 2) &&
        (trim($address_offsite_array[1]) != trim($address_offsite_array[(count($address_offsite_array) - 2)]))) {
      $street2_flag = TRUE;
    }

    // Separate the city from the last line of the address.
    $city_state_zip_array = explode(',', trim($address_offsite_array[(count($address_offsite_array) - 1)]));

    // Separate the state and zip from each other.
    $state_zip_array = explode(' ', trim($city_state_zip_array[1]));

    // Set the address components as variables.
    $node_values_array['offsite_location_name'] = trim($address_offsite_array[0]);
    $node_values_array['street1']               = $address_offsite_array[1];
    $node_values_array['street2']               = (($street2_flag) ? trim($address_offsite_array[(count($address_offsite_array) - 2)]) : '');
    $node_values_array['city']                  = $city_state_zip_array[0];
    $node_values_array['state']                 = $state_zip_array[0];
    $node_values_array['zip']                   = $state_zip_array[1];
    $node_values_array['country']               = 'US';

  // Otherwise, the address-related field is empty.
  }
  else {

    // Ensure that any offsite location information that
    // previously existed in the database gets cleared out.
    $node_values_array['offsite_location_name'] = '';
    $node_values_array['street1']               = '';
    $node_values_array['street2']               = '';
    $node_values_array['city']                  = '';
    $node_values_array['state']                 = '';
    $node_values_array['zip']                   = '';
    $node_values_array['country']               = '';

  }

  return $node_values_array;

}


/**
 * Processes the event's date/time values and adds them to the main array.
 *
 * @param $node_values_array
 *   An array containing all the event data that
 *   will be used to create a single Drupal node.
 *
 * @return
 *   An updated array of event data.
 */
function evanced_events_importer_event_dates($node_values_array) {

  $element_date      = '';
  $element_starttime = '';
  $element_endtime   = '';
  $date_fields_array = array();

  // Create an array specifying how the date field should be
  // formatted. This is based on whether the user created the field
  // in datetime format, ISO format, or as a Unix timestamp.
  $date_format_array = array('datetime' => 'Y-m-d H:i:s',  // Datetime format
                             'varchar'  => 'Y-m-d\TH:i:s', // ISO format
                             'int'      => 'U',            // Unix timestamp
                            );

  // Retrieve any date configuration settings
  // from the database and load them as an object.
  $xmlmapper_config_settings = evanced_events_importer_load_configuration('evanced_events_importer_xmlmapper', 'date');

  // Make a loop for each date XML element already
  // mapped to a content type field in the database.
  foreach ($xmlmapper_config_settings as $xmlmapper_object) {

    // Determine if the current XML element pertains to a date.
    if (preg_match('/\_date$/', trim($xmlmapper_object->node_field_name_alias))) {
      // Set the current XML element as the date key.
      $date_fields_array[trim($xmlmapper_object->node_field_name)]['element_date'] = $xmlmapper_object->xml_elements;
    } 
    // Otherwise, determine if the current XML element pertains to the start time.
    elseif (preg_match('/\_starttime$/', trim($xmlmapper_object->node_field_name_alias))) {
      // set the current XML element as the start time key.
      $date_fields_array[trim($xmlmapper_object->node_field_name)]['element_starttime'] = $xmlmapper_object->xml_elements;
    } 
    // Otherwise, determine if the current XML element pertains to the end time.
    elseif (preg_match('/\_endtime$/', trim($xmlmapper_object->node_field_name_alias))) {
      // Set the current XML element as the end time key.
      $date_fields_array[trim($xmlmapper_object->node_field_name)]['element_endtime'] = $xmlmapper_object->xml_elements;
    }

  }

  // Make a loop for each CCK date field that was mapped.
  foreach ($date_fields_array as $date_field_name => $date_field_values) {

    // Determine if the current event lasts all day, with no specific start or end time.
    if ((trim($date_field_values['element_starttime']) != '') &&
        ($node_values_array[$date_field_values['element_starttime']] == 'All Day')) {
      // Clear the start and end time values.
      $node_values_array[$date_field_values['element_starttime']] = '';
      $node_values_array[$date_field_values['element_endtime']]   = '';
    }

    // Assemble the start date/time and ending date/time,
    // using the date components from the XML feed.
    $datetime_start = trim(((trim($date_field_values['element_date'])     != '') ? $node_values_array[$date_field_values['element_date']]      : '') . ' ' .
                          ((trim($date_field_values['element_starttime']) != '') ? $node_values_array[$date_field_values['element_starttime']] : ''));

    $datetime_end   = trim(((trim($date_field_values['element_date'])   != '') ? $node_values_array[$date_field_values['element_date']]    : '') . ' ' .
                          ((trim($date_field_values['element_endtime']) != '') ? $node_values_array[$date_field_values['element_endtime']] : ''));

    // Set the starting and ending dates as variables, formatting them for the database.
    $node_values_array[$date_field_name . '_event_date_from']   = ((trim($datetime_start) != "") ? date('Y-m-d\TH:i:s', strtotime($datetime_start)) : "");
    $node_values_array[$date_field_name . '_event_date_to']     = ((trim($datetime_end)   != "") ? date('Y-m-d\TH:i:s', strtotime($datetime_end))   : "");
    $node_values_array[$date_field_name . '_event_date_format'] = $date_format_array[trim(str_replace('date ', '', $xmlmapper_object->node_field_type))];

    // Determine if the current field represents the start date of the event.
    if ($date_field_values['element_date'] == 'date1') {
      $node_values_array['event_date_fieldname'] = $date_field_name;
    }

  }

  return $node_values_array;

}


/**
 * Determines if a date/time value is larger than any processed yet so far.
 *
 * @param $datetime_start
 *   The starting date of the current event.
 *
 * @param $xml_maxdate
 *   The maximum date that has been processed so far.
 *
 * @return
 *   The largest date value that was found.
 */
function evanced_events_importer_find_maximum_feed_date($datetime_start, $date_format, $xml_maxdate) {

  // Determine if the current start date happens
  // later than any other date processed so far.
  if (strtotime($datetime_start) > strtotime($xml_maxdate)) {
    // Update the value of the maximum date.
    $xml_maxdate = date($date_format, strtotime($datetime_start));
  }

  return $xml_maxdate;

}


/**
 * Determines if a date/time value is smaller than any processed yet so far.
 *
 * @param $datetime_start
 *   The starting date of the current event.
 *
 * @param $xml_mindate
 *   The minimum date that has been processed so far.
 *
 * @return
 *   The smallest date value that was found.
 */
function evanced_events_importer_find_minimum_feed_date($datetime_start, $date_format, $xml_mindate) {

  // Determine if the new start date happens
  // earlier than any other date processed so far.
  if (strtotime($datetime_start) < strtotime($xml_mindate)) {
    // Update the value of the minimum date.
    $xml_mindate = date($date_format, strtotime($datetime_start));
  }

  return $xml_mindate;

}


/**
 * Uses an array of event data to create a new node or edit an existing one.
 *
 * @param $nid
 *   The id of the node that needs to be updated. 
 *   If it's zero, a new node will be created.
 *
 * @param $node_values_array
 *   An array containing all the event data that
 *   will be used to create a single Drupal node.
 */
function evanced_events_importer_process_node($nid, $node_values_array) {

  // Retrieve any existing configuration settings
  // from the database and load them as an object.
  $main_config_settings = evanced_events_importer_load_configuration('evanced_events_importer');
  $xmlmapper_config_settings = evanced_events_importer_load_configuration('evanced_events_importer_xmlmapper');

  // Determine if a node ID was found. That means an
  // existing event will be updated (rather than added).
  if (($nid != 0) && ($nid != '')) {
    // Load the event node and set it as an object.
    $node = node_load($nid);
  // Otherwise, no node ID was retrieved. This event
  // will be added to Drupal for the first time.
  }
  else {
    // Create an object for the new node.
    $node = new stdClass();
  }

  // Retrieve the content type to which this data will be
  // added, as well as the ID of the publishing author.
  $node->type = $main_config_settings[0]->content_type_name;

  // Prepare defaults.
  node_object_prepare($node);

  // Define language (currently language neutral).
  $node->language = LANGUAGE_NONE;

  $node->uid  = $main_config_settings[0]->publisher_uid;

  // Ensure that the current node is published.
  $node->status  = 1;
  $node->promote = 0;
  $node->sticky  = 0;

  // Ensure the 'created' date matches the
  // evanced 'last updated' value (note: Drupal
  // won't allow the 'changed' date to be overwritten).
  // $node->created = strtotime(str_replace('Z', '', $node_values_array['lastupdated']));
  // drupal_set_message('lastupdated = ' . $node_values_array['lastupdated'] . '; ' . $node->created, 'status');

  // Make a loop for each date XML element already
  // mapped to a content type field in the database.
  foreach ($xmlmapper_config_settings as $xmlmapper_object) {

    // Determine if this is a date field.
    if (substr($xmlmapper_object->node_field_type, 0, 4) == 'date') {
      // Add the start and end dates to the node object.
      $node->{$xmlmapper_object->node_field_name}[$node->language][0]['value']  = date($node_values_array[$xmlmapper_object->node_field_name . '_event_date_format'],
                                                                                       strtotime($node_values_array[$xmlmapper_object->node_field_name . '_event_date_from']));
      $node->{$xmlmapper_object->node_field_name}[$node->language][0]['value2'] = date($node_values_array[$xmlmapper_object->node_field_name . '_event_date_format'],
                                                                                       strtotime($node_values_array[$xmlmapper_object->node_field_name . '_event_date_to']));
      // drupal_set_message($xmlmapper_object->node_field_name . ' = ' . $node->{$xmlmapper_object->node_field_name}[$node->language][0]['value'] . ' - ' . $node->{$xmlmapper_object->node_field_name}[$node->language][0]['value2'] . '; ', 'status');
    } 
    // Otherwise, determine if this is an address field.
    elseif ($xmlmapper_object->node_field_type == 'addressfield') {

      // Ensure the address field contains usable data.
      if ((trim($node_values_array['street1']) != '') &&
          (trim($node_values_array['city'])    != '')) {

        // Retrieve the configuration settings for this instance of the address field.
        $field_config_instance = field_read_instances(array('field_name' => $xmlmapper_object->node_field_name, 'bundle' => $node->type), array());

        // Determine if the "Organisation (single line)" field is enabled.
        if ((!empty($field_config_instance[0]['widget']['settings']['format_handlers']['organisation'])) &&
            (trim($field_config_instance[0]['widget']['settings']['format_handlers']['organisation']) != '0')) {
          $node->{$xmlmapper_object->node_field_name}[$node->language][0]['organisation_name'] = trim($node_values_array['offsite_location_name']);
        }

        // Determine if the "Name (single line)" field is enabled.
        if ((!empty($field_config_instance[0]['widget']['settings']['format_handlers']['name-oneline'])) &&
            (trim($field_config_instance[0]['widget']['settings']['format_handlers']['name-oneline']) != '0')) {
          $node->{$xmlmapper_object->node_field_name}[$node->language][0]['name_line']         = trim($node_values_array['offsite_location_name']);
        }

        // Add the offsite address data to the node object.
        $node->{$xmlmapper_object->node_field_name}[$node->language][0]['thoroughfare']        = trim($node_values_array['street1']);
        $node->{$xmlmapper_object->node_field_name}[$node->language][0]['premise']             = trim($node_values_array['street2']);
        $node->{$xmlmapper_object->node_field_name}[$node->language][0]['locality']            = trim($node_values_array['city']);
        $node->{$xmlmapper_object->node_field_name}[$node->language][0]['administrative_area'] = trim($node_values_array['state']);
        $node->{$xmlmapper_object->node_field_name}[$node->language][0]['postal_code']         = trim($node_values_array['zip']);
        $node->{$xmlmapper_object->node_field_name}[$node->language][0]['country']             = trim($node_values_array['country']);

      }

    }
    // Otherwise, determine if this is a taxonomy field.
    elseif ($xmlmapper_object->node_field_type == 'taxonomy') {

      // Determine if the current taxonomy term field is already populated with term ids.
      // This code block was provided by Sean Fitzpatrick, Issue #1691414. Thanks, Sean!
      if (isset($node->{$xmlmapper_object->node_field_name})) {
        // Remove any existing taxonomy terms from the current field.
        unset($node->{$xmlmapper_object->node_field_name});
      }

      // Loop through the array of term ids associated with this taxonomy term field.
      foreach ($node_values_array[$xmlmapper_object->node_field_name] as $tid) {
        // Add the current tid to the taxonomy term field.
        // Optimized by Sean Fitzpatrick, Issue #1691414. Thanks, Sean!
        $node->{$xmlmapper_object->node_field_name}[$node->language][]['tid'] = $tid;
      }

    }
    // Otherwise, this is probably a less 'widget-y' field.
    else {

      // Determine if this field is the node title.
      if (($xmlmapper_object->node_field_type == 'text') &&
          ($xmlmapper_object->node_field_name == 'title')) {
        // Add the node title to the node object.
        $node->{$xmlmapper_object->node_field_name} = trim($node_values_array[$xmlmapper_object->xml_elements]);
      }
      // Otherwise, determine if this field is the node body.
      elseif (($xmlmapper_object->node_field_type == 'text') &&
              ($xmlmapper_object->node_field_name == 'body')) {

        // Convert any Windows-1252 glyphs (which won't
        // display) into alpha character codes (ex. &rdquo;).
        $node_values_array[$xmlmapper_object->xml_elements] = evanced_events_importer_convert_char_encoding($node_values_array[$xmlmapper_object->xml_elements]);

        // Determine if the user wants all the html
        // links stripped from the event description.
        if ($main_config_settings[0]->striplinks_flag == 1) {
          // Remove all html links. instead, show the URL next to the link text.
          $node_values_array[$xmlmapper_object->xml_elements] = preg_replace('/<a[^>]*?href=[\'"](.*?)[\'"][^>]*?>(.*?)<\/a>/si', '$2', $node_values_array[$xmlmapper_object->xml_elements]);
        }

        // Add the node body to the node object.
        $node->{$xmlmapper_object->node_field_name}[$node->language][0]['value'] = trim($node_values_array[$xmlmapper_object->xml_elements]);

        // Specify the correct input type for the node body.
        $node->{$xmlmapper_object->node_field_name}[$node->language][0]['format'] = $main_config_settings[0]->input_format;
		// drupal_set_message($xmlmapper_object->node_field_name . ' = ' . $node->{$xmlmapper_object->node_field_name}[$node->language][0]['format'] . '; ', 'status');

        // Add a summary (teaser) to the node object.
        $node->{$xmlmapper_object->node_field_name}[$node->language][0]['summary'] = text_summary(trim($node_values_array[$xmlmapper_object->xml_elements]),
                                                                                                  isset($main_config_settings[0]->input_format) ?
                                                                                                  $main_config_settings[0]->input_format : NULL);

      }
      // Otherwise, this is probably a basic CCK field.
      else {
        // Ensure that an XML element was actually mapped to this CCK field.
        if (trim($xmlmapper_object->xml_elements) != '') {
          // Add the current field to the node object.
          $node->{$xmlmapper_object->node_field_name}[$node->language][0]['value'] = trim($node_values_array[$xmlmapper_object->xml_elements]);
          // drupal_set_message('||' . $xmlmapper_object->xml_elements . '|| ' . $xmlmapper_object->node_field_name . ' = ' . $node->{$xmlmapper_object->node_field_name}[$node->language][0]['value']  . '; ', 'status');
        }
      }

    }

  }

  // Prepare the node for saving.
  node_submit($node);

  // Ensure the 'created' date matches the evanced 'last updated' value.
  // Note 1: Drupal won't allow the 'changed' date to be overwritten.
  // Note 2: The 'created' date has to come after 'node_submit';
  // otherwise, the function will override it.
  $node->created = strtotime(str_replace('Z', '', $node_values_array['lastupdated']));

  // Update Drupal with the changes, whether a new
  // node is being added or an old one is being edited.
  node_save($node);

  // Set the node ID as a variable.
  $nid = $node->nid;

}


/**
 * Parses the EXML feed, creating an array of event data for a single node.
 *
 * @param $item
 *   An object containing data for a single 
 *   event, retrieved from the EXML feed.
 *
 * @param $xml_nodes_array
 *   An array containing the unique XML
 *   tag names found in the EXML feed.
 *
 * @return
 *   An array containing all the event data that
 *   will be used to create a single Drupal node.
 */
function evanced_events_importer_populate_node_array($item, $xml_nodes_array) {

  // Make a loop for each element in the array.
  foreach ($xml_nodes_array as $node_key => $node_name) {

    // Retrieve data associated with this XML element.
    $node_element = $item->getElementsByTagName($node_name);
    // Add the value of the current element to an
    // array, using the XML tag to dynamically name it.
    $node_values_array[$node_name] = $node_element->item(0)->nodeValue;

  }

  return $node_values_array;

}


/**
 * Determines if an existing event node should be updated with new data.
 *
 * @param $id
 *   The unique ID of the event in Evanced.
 *
 * @param $lastupdated
 *   The date that the event was last updated in Evanced.
 *
 * @param $nid
 *   The id of the event node in Drupal.
 *
 * @param $update_database_flag
 *   A flag to signal whether or not the node should be updated.
 *
 * @return
 *   TRUE if the node should be updated; FALSE if it should be skipped.
 */
function evanced_events_importer_select_node_action($id, $lastupdated, &$nid, &$update_database_flag) {

  $nid = 0;
  $last_updated_db = '';
  $fieldname_evanced_id = '';
  $update_database_flag = TRUE;

  // Retrieve any existing configuration settings
  // from the database and load them as an object.
  $main_config_settings = evanced_events_importer_load_configuration('evanced_events_importer');
  $xmlmapper_config_settings = evanced_events_importer_load_configuration('evanced_events_importer_xmlmapper');

  // Make a loop for each row of mapped-field data.
  foreach ($xmlmapper_config_settings as $xmlmapper_object) {

    // Determine if the XML element associated with this field is the evanced id.
    if (drupal_strtolower(trim($xmlmapper_object->xml_elements)) == 'id') {
      // Set the evanced id's CCK field name as a variable.
      $fieldname_evanced_id = $xmlmapper_object->node_field_name;
      break;
    }

  }

  // Ensure that the evanced id's CCK field
  // name was found before continuing.
  if (trim($fieldname_evanced_id) != '') {

    // This sql query retrieves an event that
    // matches the evanced ID currently in play.
    $sql = "SELECT  n.nid, n.created " .
           "  FROM  {node} n, {field_data_" . trim($fieldname_evanced_id) . "} eid " .
           " WHERE  eid.entity_id = n.nid " .
           "   AND  eid.bundle = :bundle " .
           "   AND  eid." . trim($fieldname_evanced_id) . "_value = :evanced_id ";

    // Run the query.
    $result = db_query($sql, array(':bundle' => trim($main_config_settings[0]->content_type_name),
                                   ':evanced_id' => trim($id)));

    // Make loop for each record in the query results.
    foreach ($result as $data) {
      $nid = $data->nid;
      $last_updated_db = date('Y-m-d\TH:i:s\Z', $data->created);
      // $last_updated_db = date('Y-m-d\TH:i:s\Z', strtotime('+1 hour', $data->created));
	  // drupal_set_message('id #' . $id . ': DB: ' . $last_updated_db . '; Evanced: ' . $lastupdated, 'status');
    }

    // Determine if the 'last updated' date for this
    // record matches in both Drupal and evanced.
    if ($last_updated_db == $lastupdated) {
      // This record hasn't changed. Skip it.
      $update_database_flag = FALSE;
    }

  }
  // Otherwise, the evanced ID wasn't found
  // and the database query didn't run.
  else {
    // There's not enough valid
    // information to update this record.
    $update_database_flag = FALSE;
  }

}


/**
 * Deletes event nodes in Drupal that should exist in the EXML feed, but don't.
 *
 * @param $valid_evanced_ids
 *   A comma-delimited string of Evanced IDs found in the EXML feed.
 *
 * @param $xml_mindate
 *   The minimum event date found in the EXML feed.
 *
 * @param $xml_maxdate
 *   The maximum event date found in the EXML feed.
 *
 * @todo Sometimes, this function erroneously deletes event nodes,
     which exist the day before the earliest event in EXML feed.
 */
function evanced_events_importer_delete_events($valid_evanced_ids, $xml_mindate, $xml_maxdate) {

  $fieldname_date = '';
  $fieldname_evanced_id = '';

  // Retrieve any existing configuration settings
  // from the database and load them as an object.
  $main_config_settings = evanced_events_importer_load_configuration('evanced_events_importer');
  $xmlmapper_config_settings = evanced_events_importer_load_configuration('evanced_events_importer_xmlmapper');

  // Make a loop for each row of mapped-field data.
  foreach ($xmlmapper_config_settings as $xmlmapper_object) {

    // Determine if the XML element associated with this field is the evanced id.
    if (drupal_strtolower(trim($xmlmapper_object->xml_elements)) == 'id') {
      // Set the evanced id's CCK field name as a variable.
      $fieldname_evanced_id = $xmlmapper_object->node_field_name;
    }

    // Determine if this is a CCK date field.
    if (drupal_strtolower(trim(substr($xmlmapper_object->node_field_type, 0, 4))) == 'date') {
      // Set the CCK date field name as a variable.
      $fieldname_date = $xmlmapper_object->node_field_name;
    }

    // Determine if both field names were found.
    if ((trim($fieldname_evanced_id) != '') &&
        (trim($fieldname_date)       != '')) {
      break;
    }

  }

  // Ensure the CCK field names for both the evanced id
  // and the date field were found before continuing.
  if ((trim($fieldname_evanced_id) != '') &&
      (trim($fieldname_date)       != '')) {

    // Explode the string of valid Evanced IDs into an array.
    $valid_evanced_ids_array = explode(',', trim($valid_evanced_ids));

    // This sql query retrieves any events that have a
    // start date falling within the timespan of the xml
    // feed, with an evanced ID that does *not* exist in the
    // XML file. This signals that the event should be deleted.
    $sql = "SELECT  n.nid, n.created, eid." . trim($fieldname_evanced_id) . "_value " .
           "  FROM  {node} n, " .
                   "{field_data_" . trim($fieldname_date) . "} dt, " . 
                   "{field_data_" . trim($fieldname_evanced_id) . "} eid " .
           " WHERE  dt.entity_id = n.nid " .
           "   AND  eid.entity_id = n.nid " .
           "   AND  dt.bundle = :bundle1 " .
           "   AND  eid.bundle = :bundle2 " .
           "   AND (eid." . trim($fieldname_evanced_id) . "_value <> '' " .
           "   AND  eid." . trim($fieldname_evanced_id) . "_value IS NOT NULL) " .
           "   AND (eid." . trim($fieldname_evanced_id) . "_value NOT IN (:valid_evanced_ids)) " .
           "   AND (dt."  . trim($fieldname_date) . "_value BETWEEN :timespan_start AND :timespan_end) ";

    // Run the query.
    $result = db_query($sql, array(':bundle1' => trim($main_config_settings[0]->content_type_name),
                                   ':bundle2' => trim($main_config_settings[0]->content_type_name),
                                   ':valid_evanced_ids' => $valid_evanced_ids_array,
                                   ':timespan_start' => $xml_mindate,
                                   ':timespan_end' => $xml_maxdate));

    // Make loop for each record in the query results.
    foreach ($result as $data) {
      // Delete the current event node from Drupal.
      node_delete($data->nid);
    }

  }

}


/**
 * Reads the EXML feed, parses the data and uses it to create/update nodes.
 */
function evanced_events_importer_parse_xml() {

  global $user;

  $nid = 0;

  // Set the default max and min dates. Make sure they're extreme
  // enough to get immediately overwritten by dates in the EXML feed.
  $xml_maxdate = '1971-01-01T00:00:00';
  $xml_mindate = '2037-01-19T11:59:59';

  $evanced_ids_string   = '';
  $secondary_node_name  = '';
  $update_database_flag = TRUE;

  // Retrieve any existing configuration settings
  // from the database and load them as an object.
  $main_config_settings = evanced_events_importer_load_configuration('evanced_events_importer');

  // User-impersonation code pulled from http://drupal.org/node/218104

  // Set the current user object as a variable.
  $user_original = $user;

  // Save the current session data as a variable.
  $session_original = drupal_save_session();

  // Temporarily disable the writing of additional session
  // data while the user object has been overwritten.
  drupal_save_session(FALSE);

  // Load the user that should be credited with publishing
  // the job nodes and set it as the current user object.
  $user = user_load(trim($main_config_settings[0]->publisher_uid));

  // Ensure an XML feed url has been specified before continuing.
  if ((count($main_config_settings) > 0) && (trim($main_config_settings[0]->exml_feed_url) != '')) {

    // Initialize the XML parser (Document Object Model (DOM) library).
    $doc = new DOMDocument();

    // Load the XML file.
    $doc->load($main_config_settings[0]->exml_feed_url);

    // Retrieve the root element in the XML file and set it as an object.
    $root_element = $doc->getElementsByTagName("*");

    // Populate an array with each unique
    // XML tag name found in the EXML feed.
    $xml_nodes_array = evanced_events_importer_xml_tag_names($root_element, $secondary_node_name);

    // Get each 'item' XML element.
    $items = $doc->getElementsByTagName($secondary_node_name);

    // Loop through all data contained within the <item> element.
    foreach ($items as $item) {

      // Populate an array with data from the EXML feed,
      // using the XML element name as the array key.
      $node_values_array = evanced_events_importer_populate_node_array($item, $xml_nodes_array);

      // Process event date data in the xml
      // feed and add it to the main array.
      $node_values_array = evanced_events_importer_event_dates($node_values_array);

      // Process offsite address data in the
      // XML feed and add it to the main array.
      $node_values_array = evanced_events_importer_offsite_address($node_values_array);

      // Determine which term ids should be selected for each
      // taxonomy term field; then add them to the main array.
      $node_values_array = evanced_events_importer_taxonomy_ids($node_values_array);

      // Determine if a domain was specified on the configuration page, one that should
      // be converted into a relative link when found within the body of the event node.
      if (trim($main_config_settings[0]->relative_links_domain) != '') {
        // Convert any full links in the body of this document into relative links.
        $node_values_array['description'] = preg_replace('/href=("|\')http:\/\/' .
                                                          trim($main_config_settings[0]->relative_links_domain) . '\//si',
                                                         'href=$1<?php print base_path(); ?>',
                                                          $node_values_array['description']);
      }

      // Determine whether the current date happens
      // later than any other date processed so far.
      $xml_maxdate = evanced_events_importer_find_maximum_feed_date($node_values_array[$node_values_array['event_date_fieldname'] . '_event_date_from'],
                                                                    $node_values_array[$node_values_array['event_date_fieldname'] . '_event_date_format'],
                                                                    $xml_maxdate);

      // Determine whether the current date happens
      // earlier than any other date processed so far.
      $xml_mindate = evanced_events_importer_find_minimum_feed_date($node_values_array[$node_values_array['event_date_fieldname'] . '_event_date_from'],
                                                                    $node_values_array[$node_values_array['event_date_fieldname'] . '_event_date_format'],
                                                                    $xml_mindate);

      // Append the current evanced ID to the comma-delimited string.
      $evanced_ids_string .= ',' . $node_values_array['id'];

      // Determine whether an existing event node in
      // Drupal needs to be updated or should be skipped.
      evanced_events_importer_select_node_action($node_values_array['id'], $node_values_array['lastupdated'], $nid, $update_database_flag);

      // Ensure the database should be updated before continuing.
      if ($update_database_flag) {
        // Update Drupal with the processed EXML
        // data by either adding or editing a node.
        evanced_events_importer_process_node($nid, $node_values_array);
      }

    }

    // Remove the first occurrence of the comma in the string.
    $evanced_ids_string = ltrim(trim($evanced_ids_string), ',');

    // Reformat any existing dates so that they are compatible with the database.
    // $today_date_object = mktime(0, 0, 0, date("m"), date("d"), date("Y"));
    // $xml_mindate = strtotime(date('Y-m-d\TH:i:s', $today_date_object));
    // $xml_maxdate = strtotime(date('Y-m-d\T11:59:59', $today_date_object) . ' +365 days');

    // Ensure at least one evanced ID exists
    // within the string before continuing.
    if (trim($evanced_ids_string) != '') {
      // Delete any event nodes that should exist
      // within the current XML feed, but don't.
      evanced_events_importer_delete_events($evanced_ids_string, $xml_mindate, $xml_maxdate);
    }

  }

  // Reinstate the original user object. Once $user is back
  // to normal, so it's okay for the session to be saved again.
  $user = $user_original;

  // Reinstate the original session data.
  drupal_save_session($session_original);

}


/**
 * Converts commonly-used Windows-1252 glyphs into alpha character codes.
 *
 * The EXML feed is encoded in ISO-8859-1 format. It's similar to
 * Windows-1252 format, except for a few extra commonly-used
 * glyphs. These glyphs won't display in Drupal unless they're
 * converted into alpha character codes first.
 *
 * @param $text_string
 *   Text that potentially contains Windows-1252 glyphs to convert.
 */
function evanced_events_importer_convert_char_encoding($text_string) {

  // Create an array of the most common Windows-1252 character entities:
  // Specifically, numeric character codes and alpha character codes.
  $glyphs_array = array("&#133;" => "&hellip;", // …
                        "&#145;" => "&lsquo;",  // ‘
                        "&#146;" => "&rsquo;",  // ’
                        "&#147;" => "&ldquo;",  // “
                        "&#148;" => "&rdquo;",  // ”
                        "&#149;" => "&bull;",   // •
                        "&#150;" => "&ndash;",  // –
                        "&#151;" => "&mdash;",  // —
                        "&#152;" => "&tilde;",  // ˜
                        "&#153;" => "&trade;",  // ™
                       );

  // Convert glyphs in the text body into HTML special character entities. While
  // most glyphs will be changed into alpha character codes (&quot;), Windows-1252
  // glyphs will be transformed into numeric character codes (&#148;) instead.
  $text_string = mb_convert_encoding($text_string, "HTML-ENTITIES", "UTF-8");

  // Replace any Windows-1252 numeric character entities (&#148;) with alpha
  // character entities instead (&rdquo;), since the latter will be displayed
  // regardless of the text format. Not true of the numeric character entities.
  $text_string = strtr($text_string, $glyphs_array);

  return $text_string;

}
