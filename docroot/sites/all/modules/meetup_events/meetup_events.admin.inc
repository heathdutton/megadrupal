<?php // -*- drupal -*-
  /**
   * @file
   * Administration page callbacks for the lending module.
   */

  /**
   * Form builder. Configure lending.
   *
   * @ingroup forms
   * @see system_settings_form().
   */

// ctools_include('dependent');

function meetup_events_get_numeric_fields_for_type($type, $numtype) {
  $fields = array("none" => "none");
  // first get all number fields that are on the type
  $query = db_select('node_type', 'n');
  $query->join('field_config_instance', 'fci', 'fci.bundle=n.type');
  $query->join('field_config', 'fc', 'fci.field_id=fc.id');
  $results = $query->fields('fc', array('field_name'))
    ->condition('fc.type', $numtype)
    ->condition('n.type', $type)
    ->execute();

  foreach ($results as $obj) {
    $fields[$obj->field_name] = $obj->field_name;
  }

  // then get all number fields that are on the nodereferences
  $query = db_select('node_type', 'n');
  $query->join('field_config_instance', 'fci', 'fci.bundle=n.type');
  $query->join('field_config', 'fc', 'fci.field_id=fc.id');
  $results = $query->fields('fc', array('field_name', 'data'))
    ->condition('fc.type', 'node_reference')
    ->condition('n.type', $type)
    ->execute();

  foreach ($results as $obj) {
    $data = unserialize($obj->data);

    foreach ($data["settings"]["referenceable_types"] as $key => $value) {
      if ($value) {
        $query = db_select('node_type', 'n');
        $query->join('field_config_instance', 'fci', 'fci.bundle=n.type');
        $query->join('field_config', 'fc', 'fci.field_id=fc.id');
        $results2 = $query->fields('fc', array('field_name'))
          ->condition('fc.type', $numtype)
          ->condition('n.type', $value)
          ->execute();
        foreach ($results2 as $obj2) {
          $fields[$obj->field_name . ":" . $obj2->field_name] = $obj->field_name . " => " . $obj2->field_name;
        }
      }
    }
  }

  return $fields;
}

function meetup_events_get_date_fields_for_type($type) {
  $fields = array();
  $query = db_select('node_type', 'n');
  $query->join('field_config_instance', 'fci', 'fci.bundle=n.type');
  $query->join('field_config', 'fc', 'fci.field_id=fc.id');

  $results = $query->fields('n', array('name', 'type'))
    ->fields('fc', array('field_name'))
    ->fields('fci', array('data'))
    ->condition('fc.module', 'date')
    ->condition('n.type', $type)
    ->execute();

  foreach ($results as $obj) {
    $fields[$obj->field_name] = $obj->field_name;
  }
  return $fields;
}

function meetup_events_get_allowed_types() {
  $options = array();
  $query = db_select('node_type', 'n');
  $query->join('field_config_instance', 'fci', 'fci.bundle=n.type');
  $query->join('field_config', 'fc', 'fci.field_id=fc.id');

  $results = $query->fields('n', array('name', 'type'))
    ->fields('n', array('name', 'type'))
    ->condition('fc.module', 'date')
    ->execute();

  foreach($results as $obj) {
    $options[$obj->type] = $obj->name;
  }
  return $options;
}

/***
 * Form for main settings page
 */
function meetup_events_get_settings() {
  $settings = variable_get("meetup_events", array());
  $options = meetup_events_get_allowed_types();

  foreach ($options as $type => $name) {
    $key = $type . "_enabled";
    if (!array_key_exists($key, $settings)) {
      $settings[$key] = FALSE;
    }

    if (!array_key_exists($type, $settings)) {
      $settings[$type] = array(
        'date_field' => NULL,
        'body' => t("Come to our awesome meetup!"),
        'venue_field' => NULL,
        'venue_lat' => NULL,
        'venue_lon' => NULL,
      );
    }

  }

  return $settings;
}

function meetup_events_admin_settings() {
  ctools_include('dependent');

  $options = meetup_events_get_allowed_types();

  $settings = meetup_events_get_settings();

  $form["meetup_events"] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => 'Enabled types',
    '#description' => 'Select node types below to enable for meetup synchronization'
  );

  foreach ($options as $type => $name) {

    $form["meetup_events"][$type . "_enabled"] = array(
      '#type' => 'checkboxes',
      '#options' => array($type => $name),
      '#default_value' => $settings[$type . "_enabled"],
    );

    $dates = meetup_events_get_date_fields_for_type($type);

    $dashtype = str_replace("_", "-", $type);
    $form["meetup_events"][$type] = array(
      '#type' => 'fieldset',
      '#input' => TRUE,
      '#id' => 'group-' . $dashtype,
      '#prefix' => '<div id="group-' . $dashtype. '-wrapper">',
      '#suffix' => '</div>',
      '#dependency' => array("edit-meetup-events-" . $dashtype . "-enabled-" . $dashtype => array(TRUE))
    );

    $form["meetup_events"][$type]["date_field"] = array(
      '#title' => t("Date Field"),
      '#description' => 'Which date field should we use for the event date / time',
      '#type' => 'select',
      '#options' => meetup_events_get_date_fields_for_type($type),
      '#default_value' => $settings[$type]["date_field"],
    );

    $form["meetup_events"][$type]["venue_field"] = array(
      '#title' => t("Venue Id Field"),
      '#description' => 'Which numeric field should we use for numeric venue id for meetup (can be on a referenced node)',
      '#type' => 'select',
      '#options' => meetup_events_get_numeric_fields_for_type($type, 'number_integer'),
      '#default_value' => $settings[$type]["venue_field"],
    );

    $form["meetup_events"][$type]["venue_lat"] = array(
      '#title' => t("Venue Latitude Field"),
      '#description' => 'Which numeric field should we use for Latitude to force the position of the map marker (optional)',
      '#type' => 'select',
      '#options' => meetup_events_get_numeric_fields_for_type($type, 'number_float'),
      '#default_value' => $settings[$type]["venue_lat"],
    );

    $form["meetup_events"][$type]["venue_lon"] = array(
      '#title' => t("Venue Longitude Field"),
      '#description' => 'Which numeric field should we use for Longitude to force the position of the map marker (optional)',
      '#type' => 'select',
      '#options' => meetup_events_get_numeric_fields_for_type($type, 'number_float'),
      '#default_value' => $settings[$type]["venue_lon"],
    );

    $form["meetup_events"][$type]["body"] = array(
      '#title' => t("Body Contents"),
      '#description' => 'The body content which will be synced to meetup (can use tokens if enabled)',
      '#type' => 'textarea',
      '#default_value' => ($settings[$type]["body"]) ? $settings[$type]["body"] : t("Come to our awesome meetup!")
    );

    if (module_exists('token')) {
      $form["meetup_events"][$type]['view']['token_help'] =  array(
        '#title' => t('Replacement patterns'),
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $form["meetup_events"][$type]['view']['token_help']['help'] = array(
        '#markup' => theme('token_tree', array('token_types' => 'all', 'click_insert' => FALSE, 'show_restricted' => TRUE))
      );
    }
  }

  $form['meetup_api'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Meetup.com API information'),
  );

  $api = variable_get('meetup_api', array(
           'key' => "",
           'group_url' => "",
           'oauth_key' => "",
           'opts' => array(),
         ));

  $form['meetup_api']['key'] = array(
    '#type' => 'textfield',
    '#title' => t('Meetup API Key'),
    '#default_value' => $api["key"],
    '#description' => t('The <a href="http://www.meetup.com/meetup_api/key/">meetup API key</a> for your user.')
  );

  $form['meetup_api']['group_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Meetup Group URL'),
    '#default_value' => $api["group_url"],
    '#description' => t('The meetup group url. This will be the name of your group after http://meetup.com/<u><b>Group-URL</b></u>'),
  );

  return system_settings_form($form);
}

function meetup_events_admin_settings_validate($form, &$form_state) {
  $values = $form_state["values"];

  $request_url = "https://api.meetup.com/2/groups?";
  $request_headers = array(
    'Content-Type' => 'application/x-www-form-urlencoded',
    'Accept-Charset' => 'utf-8');
  $request_method = 'GET';

  $rawdata = array(
    "key" => $values['meetup_api']['key'],
    "group_urlname" => $values['meetup_api']['group_url'],
    "page" => 20,
  );
  $request_url .= http_build_query($rawdata, '', '&');

  // TODO: validate the key seperately than group_url?

  $res = drupal_http_request($request_url, $request_headers, $request_method, '');
  $resp = json_decode($res->data);

  foreach ($resp->results as $result) {
    if ($result->urlname == $values['meetup_api']['group_url']) {
      $form_state["values"]["meetup_api"]["group"] = $result->id;
      break;
    }
  }

  // TODO: other validations we should make?
  // if we didn't find an id, assume things are wrong
  if (!$form_state["values"]["meetup_api"]["group"]) {
    form_set_error('meetup_api][group_url', t("Must specify valid group url"));
  }
}
