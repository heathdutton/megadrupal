<?php

/**
 * @file
 * Functions to integrate webforms with insightly API.
 */

/**
 * Helper function from webform_insightly_webform_submission_insert().
 *
 * Integration with Insightly.
 *  Used to map and sent data to Insightly in webform submissions.
 */
function webform_insightly_map_fields($node, $submission) {
  include_once 'webform_insightly.admin.inc';
  $accepted_fields = variable_get('webform_insightly_accepted_fields');
  $results = webform_insightly_get_all_field_mapping();
  foreach ($results as $result) {
    $mapping_id = $result->id;
    $form_id = $result->form_id;
    $mapping_details = json_decode($result->mapping);
    preg_match("/[0-9]+$/", $form_id, $matches);
    $node_id = $matches[0];
    if ($submission->nid == $node_id) {
      $components = $node->webform['components'];
      $data = $submission->data;
      $labels = array();
      foreach ($components as $key => $component) {
        $labels[$key] = $component['form_key'];
      }
      $fields = array();
      $module_details = system_get_info('module', 'webform');
      $version_number = explode('.', substr($module_details['version'], strpos($module_details['version'], "-") + 1));
      foreach ($data as $key => $values) {
        if ($version_number[0] <= 3){
          $fields[$labels[$key]] = $values['value'][0];
        }
        elseif ($version_number[0] >= 4) {
          $fields[$labels[$key]] = $values[0];
        }
      }
      $contact_details = array(
        'FIRST_NAME' => 'To be filled',
        'LAST_NAME' => 'To be filled',
        'BACKGROUND' => 'To be filled',
        'IMAGE_URL' => 'To be filled',
        'EMAIL' => 'To be filled',
        'PHONE' => 'To be filled',
        'BID_AMOUNT' => 0,
      );
      foreach ($mapping_details as $form_field => $insightly_field) {
        $field_values = ($fields[$form_field]) ? $fields[$form_field] : 'To be filled';
        $contact_details[$insightly_field] = $field_values;
      }
    }
  }
  $custom_fields = array_diff_key($contact_details, array_flip($accepted_fields));
  $custom_field_string = '';
  foreach ($custom_fields as $custom_insightly_field => $form_field) {
    $form_field = ($form_field) ? $form_field : 'To be filled';
    $custom_field_string .= '{"CUSTOM_FIELD_ID": "' . $custom_insightly_field . '", "FIELD_VALUE": "' . $form_field . '"}, ';
  }
  $custom_field_string = substr_replace(rtrim($custom_field_string), "", -1);
  $contact_details['custom_field_string'] = $custom_field_string;
  webform_insightly_generate_json($contact_details);
}

/**
 * Custom function used to send and retrive data from Insightly.
 *
 * @param $contact_details
 *   Array of contact details from the webforms.
 */
function webform_insightly_generate_json($contact_details) {
  date_default_timezone_set('UTC');
  $date = date('Y-m-d H:m:s');

  // Checking whether the contact already exists in Insightly.
  $url = 'https://api.insight.ly/v2.1/Contacts?email=' . $contact_details['EMAIL'];
  $method = 'GET';
  $type = 'contact';
  $contact_data = webform_insightly_transfer_data($type, $url, $method);
  $contact_id = $contact_data['contact_id'];
  $owner_id = $contact_data['owner_id'];
  // Create a new contact if contact not exist in insightly.
  $email_contact = '{"CONTACT_INFO_ID":1,"TYPE":"EMAIL","SUBTYPE":"Work","LABEL":null,"DETAIL":"' . $contact_details['EMAIL'] . '"}';
  $phone_contact = '{"CONTACT_INFO_ID":2,"TYPE":"PHONE","SUBTYPE":"Work","LABEL":null,"DETAIL":"' . $contact_details['PHONE'] . '"}';
  if (!$contact_id) {
    $url = 'https://api.insight.ly/v2.1/Contacts';
    $method = 'POST';
    $contact = '{"CONTACT_ID": 0,
      "SALUTATION": null,
      "FIRST_NAME": "' . $contact_details['FIRST_NAME'] . '",
      "LAST_NAME": "' . $contact_details['LAST_NAME'] . '",
      "BACKGROUND": "' . $contact_details['BACKGROUND'] . '",
      "IMAGE_URL": "' . $contact_details['IMAGE_URL'] . '",
      "DEFAULT_LINKED_ORGANISATION": null,
      "DATE_CREATED_UTC": "' . $date . '",
      "DATE_UPDATED_UTC": "' . $date . '",
      "VISIBLE_TO": "EVERYONE",
      "VISIBLE_TEAM_ID": null,
      "VISIBLE_USER_IDS": null,
      "CUSTOMFIELDS": [' . $contact_details['custom_field_string'] . '],
      "ADDRESSES": [],
      "CONTACTINFOS": [' . $email_contact . ', ' . $phone_contact . '],
      "DATES": [],
      "TAGS": [],
      "LINKS": [],
      "CONTACTLINKS": [],
      "EMAILLINKS": null
      }';
    $contact_id  = webform_insightly_transfer_data($type, $url, $method, $contact);
  }

  // Create opportunity against a contact.
  $url = 'https://api.insight.ly/v2.1/Opportunities';
  $method = 'POST';
  $type = 'opportunity';
  $opportunity = '{
    "OPPORTUNITY_ID": 0,
    "OPPORTUNITY_NAME": "To be filled",
    "OPPORTUNITY_DETAILS": "' . $contact_details['BACKGROUND'] . '",
    "PROBABILITY": 50,
    "BID_CURRENCY": "USD",
    "BID_AMOUNT": ' . $contact_details['BID_AMOUNT'] . ',
    "BID_TYPE": "Fixed Bid",
    "BID_DURATION": 1,
    "FORECAST_CLOSE_DATE": "' . $date . '",
    "CATEGORY_ID": null,
    "PIPELINE_ID": null,
    "STAGE_ID": null,
    "OPPORTUNITY_STATE": "OPEN",
    "IMAGE_URL": null,
    "RESPONSIBLE_USER_ID": "' . $owner_id . '",
    "OWNER_USER_ID": "' . $owner_id . '",
    "DATE_CREATED_UTC": "' . $date . '",
    "DATE_UPDATED_UTC": "' . $date . '",
    "VISIBLE_TO": "EVERYONE",
    "VISIBLE_TEAM_ID": null,
    "VISIBLE_USER_IDS": null,
    "CUSTOMFIELDS": [],
    "TAGS": [],
    "LINKS": [
      {
        "LINK_ID": null,
        "CONTACT_ID": ' . $contact_id . ',
        "OPPORTUNITY_ID": null,
        "ORGANISATION_ID": null,
        "PROJECT_ID": null,
        "SECOND_PROJECT_ID": null,
        "SECOND_OPPORTUNITY_ID": null,
        "ROLE": null,
        "DETAILS": null
      },
    ],
    "EMAILLINKS": null
  }';
  webform_insightly_transfer_data($type, $url, $method, $opportunity);
}

/**
 * Custom function to transfer data from and to Insightly.
 *
 * @param $type
 *   Request category.
 * @param $url
 *   The url responsible for the action.
 * @param $method
 *   Values will be GET or POST.
 * @param $data
 *   Json array to create contacts or opportunity.
 */
function webform_insightly_transfer_data($type, $url, $method, $data = NULL) {
  $ch = curl_init($url);
  $api_key = variable_get('webform_insightly_api_key', '');
  $user = base64_encode($api_key);
  curl_setopt($ch,
    CURLOPT_HTTPHEADER,
    array('Content-Type: application/json', 'Authorization: Basic ' . $user));
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_TIMEOUT, 30);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  if ($method == 'GET') {
    curl_setopt($ch, CURLOPT_POST, 0);
  }
  elseif ($method == 'POST') {
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
  }
  $output = curl_exec($ch);
  $result = json_decode($output);
  curl_error($ch);
  curl_close($ch);
  if ($type == 'contact') {
    if ($method == 'GET') {
      $contact_id = $result[0]->CONTACT_ID;
      $owner_id = $result[0]->OWNER_USER_ID;
    }
    elseif ($method == 'POST') {
      $contact_id = $result->CONTACT_ID;
      $owner_id = $result[0]->OWNER_USER_ID;
    }
    return array('contact_id' => $contact_id, 'owner_id' => $owner_id);
  }
}
