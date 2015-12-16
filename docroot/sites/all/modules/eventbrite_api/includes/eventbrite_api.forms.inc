<?php

/**
 * @file
 * Eventbrite forms file.
 */

/**
 * Admin configiguration form.
 */
function eventbrite_api_config_form() {
  // Load the details.
  $ukey = variable_get('eventbrite_api_userkey', '');
  $akey = variable_get('eventbrite_api_appkey', '');
  // Form.
  $form = array();
  $form['auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Eventbrite authentication details'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['auth']['eventbrite_api_userkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Eventbrite user key'),
    '#default_value' => $ukey,
    '#required' => TRUE,
    '#description' => t('Get your user key at <a href="@url">@url</a>.', array('@url' => 'https://www.eventbrite.com/userkeyapi')),
  );
  $form['auth']['eventbrite_api_appkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Eventbrite app key'),
    '#default_value' => $akey,
    '#required' => TRUE,
    '#description' => t('Get your app keys at <a href="@url">@url</a>.', array('@url' => 'https://www.eventbrite.com/api/key/')),
  );
  // Display the rest of the form only when the Eventbrite details are correct.
  if (!empty($ukey) && !empty($akey)) {
    $form['import'] = array(
      '#type' => 'fieldset',
      '#title' => t('Eventbrite data import'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['import']['import_all'] = array(
      '#type' => 'submit',
      '#value' => t('Import all Eventbrite event data'),
      '#submit' => array('eventbrite_api_import_all'),
    );
    $form['import']['import_all_help'] = array(
      '#markup' => '<div>' . t('If you already have Eventbrite data imported,
      this will delete that data before importing the new data. This
      cannot be undone.') . '</div>',
    );
    $form['import_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Event import settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['import_settings']['eventbrite_api_import_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete all existing data before import.'),
      '#default_value' => variable_get('eventbrite_api_import_delete', 1),
    );
    $form['import_settings']['eventbrite_api_cron_import'] = array(
      '#type' => 'checkbox',
      '#title' => t('Import events from Eventbrite during cron runs'),
      '#default_value' => variable_get('eventbrite_api_cron_import', 0),
    );
    $form['import_settings']['eventbrite_api_cron_delay'] = array(
      '#type' => 'select',
      '#title' => t('How often should we run cron'),
      '#default_value' => variable_get('eventbrite_api_cron_delay', 360),
      '#options' => array(
        3600 => 'Every hour',
        7200 => 'Every 2 hours',
        10800 => 'Every 3 hours',
        14400 => 'Every 4 hours',
        18000 => 'Every 5 hours',
        43200 => 'Every 12 hours',
        86400 => 'Once per day',
        172800 => 'Every 2 days',
        259200 => 'Every 3 days',
        432000 => 'Every 5 days',
        604800 => 'Once per week',
        1209600 => 'Every 2 weeks',
      ),
      '#description' => t('Select the gap in hours between each cron run. Remember that syncronising with the server requires resources, so set this value to as high as you believe is feasible to keep your site up to date.', array()),
    );
    $form['import_settings']['eventbrite_api_import_strip'] = array(
      '#type' => 'checkbox',
      '#title' => t('Strip HTML code from the event description (recommended)'),
      '#default_value' => variable_get('eventbrite_api_import_strip', 1),
    );
    $form['general'] = array(
      '#type' => 'fieldset',
      '#title' => t('General settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['general']['eventbrite_api_currency'] = array(
      '#type' => 'select',
      '#title' => t('Default currency'),
      '#default_value' => variable_get('eventbrite_api_currency', 'USD'),
      '#options' => eventbrite_api_currencies(),
    );
    $form['general']['eventbrite_api_country_code'] = array(
      '#type' => 'select',
      '#title' => t('Default country code'),
      '#default_value' => variable_get('eventbrite_api_country_code', 'US'),
      '#options' => eventbrite_api_countries(),
    );
  }
  $form['#validate'][] = 'eventbrite_api_config_form_validate';
  return system_settings_form($form);
}

/**
 * Validation function for the admin configiguration form.
 * Essentially, we're testing to see if the keys work.
 */
function eventbrite_api_config_form_validate($form, &$form_state) {

  if (($library = libraries_load('eventbrite')) && !empty($library['loaded'])) {
    // Get the app key and user key values from the form..
    $akey = $form_state['values']['eventbrite_api_appkey'];
    $ukey = $form_state['values']['eventbrite_api_userkey'];
    // Connect to Eventbrite using the Eventbrite class.
    $eb_client = new Eventbrite(array('app_key' => $akey, 'user_key' => $ukey));
    // Try to get a list of events from Eventbrite with our credentials.
    try {
      // If it works, don't do anything as we want to save the details.
      $events = $eb_client->user_list_events();
    } // If it doesn't work, catch the error.
    catch (Exception $e) {
      // Variable for the form element.
      $element = 'config';
      // Variable for the error message.
      $error = $e->getMessage();
      // If it's the user key.
      if (strpos($error, 'user_key') !== FALSE) {
        $element = 'eventbrite_api_userkey';
      }
      // If it's the app key.
      elseif (strpos($error, 'application key') !== FALSE) {
        $element = 'eventbrite_api_appkey';
      }
      // Then stop the form from saving the details while printing the error.
      form_set_error($element, t('Connection to Eventbrite failed with the following error: @error', array('@error' => $error)));
    }
  }
  else {
    // Check if the eventbrite.php is named Eventbrite.php.
    $lib = getcwd() . '/sites/all/libraries/eventbrite/Eventbrite.php';
    if (file_exists($lib)) {
      form_set_error('form', t('The Eventbrite library file should be renamed from "Eventbrite.php" to "eventbrite.php" (lower case).  Please rename this file.'));
    }
    // Otherwise tell user to install the library.
    else {
      drupal_set_message(t('Please install the Eventbrite API library. Check the !readme for further information.', array('!readme' => l(t('README'), 'admin/help/eventbrite_api'))), 'error');
    }
  }
}

/**
 * Event add form.
 */
function event_add_form($form, &$form_state, $event) {
  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('General information'),
  );

  $form['details']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Event title'),
    '#default_value' => isset($event->title) ? $event->title : '',
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['details']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($event->description) ? $event->description : '',
  );

  $form['details']['capacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Capacity'),
    '#default_value' => isset($event->capacity) ? $event->capacity : '',
    '#size' => 5,
  );

  $form['details']['currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#default_value' => isset($event->currency) ? $event->currency : variable_get('eventbrite_api_currency', 'USD'),
    '#options' => eventbrite_api_currencies(),
  );

  $form['details']['status'] = array(
    '#type' => 'select',
    '#title' => t('Event status'),
    '#default_value' => isset($event->status) ? $event->status : 'draft',
    '#options' => array(
      'draft' => t('Draft'),
      'live' => t('Live'),
    ),
  );

  $form['details']['oid'] = array(
    '#title' => t('Organizer'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'eventbrite/organizer/autocomplete',
    '#default_value' => isset($event->oid) ? eventbrite_api_autocomplete_build($event->oid, 'organizer') : '',
  );

  $form['details']['vid'] = array(
    '#title' => t('Venue'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'eventbrite/venue/autocomplete',
    '#default_value' => isset($event->vid) ? eventbrite_api_autocomplete_build($event->vid, 'venue') : '',
  );

  $form['dates'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date information'),
  );

  $form['dates']['start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Start date'),
    '#default_value' => isset($event->start_date) ? $event->start_date : '',
    '#required' => TRUE,
  );

  $form['dates']['end_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('End date'),
    '#default_value' => isset($event->end_date) ? $event->end_date : '',
    '#required' => TRUE,
  );

  $form['dates']['date_default_timezone'] = array(
    '#type' => 'select',
    '#title' => t('Default time zone'),
    '#default_value' => variable_get('date_default_timezone', date_default_timezone_get()),
    '#options' => system_time_zones(),
    '#required' => TRUE,
  );

  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $submit + array('event_add_form_submit'),
  );

  if (!empty($event->title)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete event'),
      '#suffix' => l(t('Cancel'), 'admin/eventbrite/events'),
      '#submit' => $submit + array('event_form_submit_delete'),
      '#weight' => 45,
    );
  }
  $form['#validate'][] = 'event_add_form_validate';
  // Add the field related form elements.
  $form_state['event'] = $event;
  $form_state['eid'] = $event->eid;

  field_attach_form('event', $event, $form, $form_state);

  return $form;
}

/**
 * Validation function for event add form.
 */
function event_add_form_validate($form, &$form_state) {
  $event_submit = (object) $form_state['values'];
  field_attach_form_validate('event', $event_submit, $form, $form_state);
}

/**
 * Submit function for event add form.
 */
function event_add_form_submit($form, &$form_state) {
  // Extract IDs from autocomplete fields.
  $form_state['values']['oid'] = eventbrite_api_autocomplete_filter($form_state['values']['oid']);
  $form_state['values']['vid'] = eventbrite_api_autocomplete_filter($form_state['values']['vid']);
  $event_submit = (object) $form_state['values'];
  // Assign them to an array to send to Eventbrite.
  $eventbrite = $form_state['values'];
  // Format the dates properly.
  $eventbrite['start_date'] = eventbrite_api_datef($eventbrite['start_date']);
  $eventbrite['end_date'] = eventbrite_api_datef($eventbrite['end_date']);
  // Add the vid and oid with correct keys.
  $eventbrite['venue_id'] = $eventbrite['vid'];
  $eventbrite['organizer_id'] = $eventbrite['oid'];
  // Add extra fields.
  field_attach_submit('event', $event_submit, $form, $form_state);
  // Connect to Eventbrite.
  if (($library = libraries_load('eventbrite')) && !empty($library['loaded'])) {
    $auth = array(
      'app_key' => variable_get('eventbrite_api_appkey', ''),
      'user_key' => variable_get('eventbrite_api_userkey', '')
    );
    $eb_client = new Eventbrite($auth);
    try {
      // Should we update an existing event or add a new one?
      if (!empty($form_state['eid'])) {
        // Add the id with the correct key.
        $eventbrite['id'] = $form_state['eid'];
        $response = $eb_client->event_update($eventbrite);
        // If the event is updated.
        if ($response->process->status == 'OK') {
          // Set the event entity values.
          $event = (object) array_merge((array) $form_state['event'], (array) $form_state['values']);
        }
      }
      else {
        $response = $eb_client->event_new($eventbrite);
        // If the event is added.
        if ($response->process->status == 'OK') {
          // Event ID.
          $eid = $response->process->id;
          // Get the full event details.
          $response = $eb_client->event_get(array('id' => $eid));
          // This is the retured event.
          $event = $response->event;
          // Make sure the id is correctly labeled.
          $event->eid = $event->id;
          // Add the oid and vid.
          $event->oid = $event->organizer->id;
          $event->vid = $event->venue->id;
          // Build the event.
          $event = (object) array_merge((array) $form_state['event'], (array) $event);
        }
      }
    } catch (Exception $e) {
      $response = $e->getMessage();
      form_set_error('form', $response);
    }
  }
  else {
    drupal_set_message(t('Please install the Eventbrite API library. Check the !readme for further information.', array('!readme' => l(t('README'), 'admin/help/eventbrite_api'))), 'error');
  }
  $save = event_save($event);
  if (!empty($save)) {
    drupal_set_message(t('Event !title saved to Eventbrite and cached locally.', array('!title' => '<em>' . check_plain($event->title) . '</em>')));
    $form_state['redirect'] = 'admin/eventbrite/event';
  }
}

/**
 * Form API submit callback for the event delete button.
 */
function event_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/eventbrite/event/' . $form_state['eid'] . '/delete';
}

/**
 * Event delete form.
 */
function event_delete_form($form, &$form_state, $event) {
  $form_state['event'] = $event;

  $form['#submit'][] = 'event_delete_form_submit';

  $form = confirm_form($form, t('Are you sure you want to delete event "@title"?', array('@title' => $event->title)), 'admin/content/events/event', '<p>' . t('This will cancel the event on Eventbrite and delete it locally. This action cannot be undone.') . '</p>', t('Delete'), t('Cancel'), 'confirm');

  return $form;
}

/**
 * Submit callback for event_delete_form
 */
function event_delete_form_submit($form, &$form_state) {
  $event = $form_state['event'];
  event_delete($event);
  drupal_set_message(t('The event %name has been deleted.', array('%name' => (!empty($event->name) ? $event->name : ''))));
  $form_state['redirect'] = 'admin/eventbrite/event';
}

/**
 * Submit callback for event_delete_form
 */
function organizer_delete_form_submit($form, &$form_state) {
  $organizer = $form_state['organizer'];
  organizer_delete($organizer);
  drupal_set_message(t('The organizer %name has been deleted.', array('%name' => $organizer->name)));
  $form_state['redirect'] = 'admin/eventbrite/organizer';
}

function event_add_wrapper() {
  $event = event_create();
  return drupal_get_form('event_add_form', $event);
}

function event_edit_wrapper($event) {
  return drupal_get_form('event_add_form', $event);
}

function event_delete_wrapper($event) {
  return drupal_get_form('event_delete_form', $event);
}

/**
 * Organizer add form.
 */
function organizer_add_form($form, &$form_state, $organizer) {
  // Add the OID into the $form_state array.
  $form_state['oid'] = !empty($organizer->oid) ? $organizer->oid : '';

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Organizer name'),
    '#default_value' => isset($organizer->name) ? $organizer->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($organizer->description) ? $organizer->description : '',
  );

  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $submit + array('organizer_add_form_submit'),
  );
  $form['#validate'][] = 'organizer_add_form_validate';
  // Add the field related form elements.
  $form_state['organizer'] = $organizer;
  $form_state['oid'] = $organizer->oid;

  field_attach_form('organizer', $organizer, $form, $form_state);
  return $form;
}

/**
 * Validation function for event add form.
 */
function organizer_add_form_validate($form, &$form_state) {
  $organizer_submit = (object) $form_state['values'];
  field_attach_form_validate('event', $organizer_submit, $form, $form_state);
}

/**
 * Submit function for organizer add form.
 */
function organizer_add_form_submit($form, &$form_state) {
  $organizer_submit = (object) $form_state['values'];
  // Assign them to an array to send to Eventbrite.
  $eventbrite = $form_state['values'];
  // Add extra fields.
  field_attach_submit('organizer', $organizer_submit, $form, $form_state);
  // Connect to Eventbrite.
  if (($library = libraries_load('eventbrite')) && !empty($library['loaded'])) {
    $auth = array(
      'app_key' => variable_get('eventbrite_api_appkey', ''),
      'user_key' => variable_get('eventbrite_api_userkey', '')
    );
    $eb_client = new Eventbrite($auth);
    try {
      // Should we update an existing organizer or add a new one?
      if (!empty($form_state['oid'])) {
        // Add the id with the correct key.
        $eventbrite['id'] = $form_state['oid'];
        $response = $eb_client->organizer_update($eventbrite);
        // If the event is updated.
        if ($response->process->status == 'OK') {
          // Set the organizer entity values.
          $organizer = (object) array_merge((array) $form_state['organizer'], (array) $form_state['values']);
        }
      }
      else {
        $response = $eb_client->organizer_new($eventbrite);
        // If the event is added.
        if ($response->process->status == 'OK') {
          // Organizer ID.
          $oid = $response->process->id;
          // Get the full event details.
          $response = $eb_client->organizer_get(array('id' => $oid));
          // This is the returned organizer.
          $organizer = $response->organizer[0];
          // Make sure the id is correctly labeled.
          $organizer->oid = $organizer->id;
          // Get rid of the ID element.
          unset($organizer->id);
          // Set the organizer.
          $organizer = (object) array_merge((array) $form_state['organizer'], (array) $organizer);
        }
      }
    } catch (Exception $e) {
      $response = $e->getMessage();
      form_set_error('form', $response);
    }
  }
  else {
    drupal_set_message(t('Please install the Eventbrite API library. Check the !readme for further information.', array('!readme' => l(t('README'), 'admin/help/eventbrite_api'))), 'error');
  }
  $save = organizer_save($organizer);
  if (!empty($save)) {
    drupal_set_message(t('Organizer !name saved to Eventbrite and cached locally.', array('!name' => '<em>' . $organizer->name . '</em>')));
    $form_state['redirect'] = 'admin/eventbrite/organizer';
  }
}

/**
 * Form API submit callback for the organizer delete button.
 */
function organizer_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/eventbrite/organizer/' . $form_state['oid'] . '/delete';
}

/**
 * Organizer delete form.
 */
function organizer_delete_form($form, &$form_state, $organizer) {
  $form_state['organizer'] = $organizer;

  $form['#submit'][] = 'organizer_delete_form_submit';

  $form = confirm_form($form, t('Are you sure you want to delete organizer "@name"?', array('@name' => $organizer->name)), 'admin/content/events/event', '<p>' . t('This will delete the organizer on Eventbrite and delete it locally. This action cannot be undone.') . '</p>', t('Delete'), t('Cancel'), 'confirm');

  return $form;
}

function organizer_add_wrapper() {
  $organizer = organizer_create();
  return drupal_get_form('organizer_add_form', $organizer);
}

function organizer_edit_wrapper($organizer) {
  return drupal_get_form('organizer_add_form', $organizer);
}

function organizer_delete_wrapper($organizer) {
  return drupal_get_form('organizer_delete_form', $organizer);
}

/**
 * Ticket add form.
 */
function ticket_add_form($form, &$form_state, $ticket) {
  // Ability to clone the ticket to another event.
  $arg = arg(4);
  if (empty($ticket->tid) && !empty($arg)) {
    $entity = entity_load('ticket', array($arg));
    if (!empty($entity)) {
      $ticket = array_shift($entity);
      // Get rid of the event and ticket IDs.
      $ticket->tid = $ticket->eid = '';
    }
  }
  // Add the TID into the $form_state array.
  $form_state['tid'] = $ticket->tid;

  $form['eid'] = array(
    '#type' => 'textfield',
    '#title' => t('Event'),
    '#description' => t('A ticket can only be associated with one event so once chosen, the associated event cannot be changed.'),
    '#default_value' => isset($ticket->eid) ? eventbrite_api_autocomplete_build($ticket->eid, 'event') : '',
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  // If we're editing an event we can't edit the event ID.
  if (!empty($ticket->eid)) {
    $form['eid']['#disabled'] = TRUE;
  }
  // If it's not being edited, add autocomplete to the field.
  else {
    $form['eid']['#autocomplete_path'] = 'eventbrite/event/autocomplete';
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($ticket->name) ? $ticket->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      0 => 'Fixed price',
      1 => 'Donation',
    ),
    '#default_value' => isset($ticket->type) ? $ticket->type : 0,
    '#required' => TRUE,
  );

  $form['min'] = array(
    '#type' => 'textfield',
    '#title' => t('Min'),
    '#description' => t('The minimum number of this ticket a person can purchase. The default is 1.'),
    '#default_value' => !empty($ticket->min) ? $ticket->min : 1,
    '#required' => TRUE,
    '#maxlength' => 255,
  );

  $form['max'] = array(
    '#type' => 'textfield',
    '#title' => t('Max'),
    '#description' => t('The maximum number of this ticket a person can purchase. This is optional and if left blank, the default will be whatever value is entered into the "Quantity available" field.'),
    '#default_value' => (!empty($ticket->max) ? $ticket->max : (isset($ticket->quantity_available) ? $ticket->quantity_available : '')),
    '#maxlength' => 255,
  );

  $form['price'] = array(
    '#type' => 'textfield',
    '#title' => t('Price'),
    '#default_value' => isset($ticket->price) ? $ticket->price : '',
  );

  $form['quantity_available'] = array(
    '#type' => 'textfield',
    '#title' => t('Quantity available'),
    '#description' => t('The number of this ticket available.'),
    '#default_value' => isset($ticket->quantity_available) ? $ticket->quantity_available : '',
  );

  $form['currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#options' => eventbrite_api_currencies(),
    '#default_value' => !empty($ticket->currency) ? $ticket->currency : variable_get('eventbrite_api_currency', 'USD'),
    '#required' => TRUE,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($ticket->description) ? $ticket->description : '',
  );

  $form['start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Start date'),
    '#default_value' => isset($ticket->start_date) ? $ticket->start_date : '',
    '#required' => TRUE,
  );

  $form['end_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('End date'),
    '#default_value' => isset($ticket->end_date) ? $ticket->end_date : '',
    '#required' => TRUE,
  );

  $form['include_fee'] = array(
    '#type' => 'select',
    '#title' => t('Include fee'),
    '#description' => t('Should the Eventbrite fee be added onto the ticket price or included as part of the ticket price.'),
    '#options' => array(
      0 => 'Add to ticket price',
      1 => 'Include in ticket price',
    ),
    '#default_value' => isset($ticket->include_fee) ? $ticket->include_fee : 0,
    '#required' => TRUE,
  );

  if (!empty($ticket->eid)) {
    $form['hide'] = array(
      '#type' => 'select',
      '#title' => t('Show or hide'),
      '#description' => t('Display this ticket on the Eventbrite booking page or hide it from view.'),
      '#options' => array(
        'n' => 'Show',
        'y' => 'Hide',
      ),
      '#default_value' => isset($ticket->hide) ? $ticket->hide : 'n',
      '#required' => TRUE,
    );
  }

  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $submit + array('ticket_add_form_submit'),
  );
  $form['#validate'][] = 'ticket_add_form_validate';
  // Add the field related form elements.
  $form_state['ticket'] = $ticket;
  $form_state['tid'] = $ticket->tid;

  field_attach_form('ticket', $ticket, $form, $form_state);
  return $form;
}

/**
 * Validation function for event add form.
 */
function ticket_add_form_validate($form, &$form_state) {
  $ticket_submit = (object) $form_state['values'];
  field_attach_form_validate('event', $ticket_submit, $form, $form_state);
}

/**
 * Submit function for ticket add form.
 */
function ticket_add_form_submit($form, &$form_state) {
  // Extract IDs from autocomplete fields.
  $form_state['values']['eid'] = eventbrite_api_autocomplete_filter($form_state['values']['eid']);
  $ticket_submit = (object) $form_state['values'];
  // Assign them to an array to send to Eventbrite.
  $eventbrite = $form_state['values'];
  // Add extra fields.
  field_attach_submit('ticket', $ticket_submit, $form, $form_state);
  // Format the dates correctly.
  $eventbrite['start_date'] = eventbrite_api_datef($eventbrite['start_date']);
  $eventbrite['end_date'] = eventbrite_api_datef($eventbrite['end_date']);
  // Connect to Eventbrite.
  if (($library = libraries_load('eventbrite')) && !empty($library['loaded'])) {
    $auth = array(
      'app_key' => variable_get('eventbrite_api_appkey', ''),
      'user_key' => variable_get('eventbrite_api_userkey', '')
    );
    $eb_client = new Eventbrite($auth);
    try {
      // Ensure empty max field doesn't cause an error.
      if (empty($eventbrite['max'])) {
        $eventbrite['max'] = $eventbrite['quantity_available'];
      }
      // Should we update an existing ticket or add a new one?
      if (!empty($form_state['tid'])) {
        // Add the id with the correct key.
        $eventbrite['id'] = $form_state['tid'];
        $response = $eb_client->ticket_update($eventbrite);
        // If the event is updated.
        if ($response->process->status == 'OK') {
          // Set the ticket entity values.
          $ticket = (object) array_merge((array) $form_state['ticket'], (array) $form_state['values']);
        }
      }
      else {
        // Add the event id.
        $eventbrite['event_id'] = $eventbrite['eid'];
        $response = $eb_client->ticket_new($eventbrite);
        // If the event is added.
        if ($response->process->status == 'OK') {
          // Ticket ID.
          $tid = $response->process->id;
          // We have to call the actual event to get the full ticket details.
          $response = $eb_client->event_get(array('id' => $eventbrite['eid']));
          // Get the list of tickets.
          $tickets = $response->event->tickets;
          // Add some IDs.
          $ticket = array('tid' => $tid, 'eid' => $eventbrite['eid']);
          // Find the ticket we just added.
          foreach ($tickets as $t) {
            if ($t->ticket->id == $tid) {
              $ticket = $ticket + (array) $t->ticket;
              break;
            }
          }
          // Set the ticket.
          $ticket = (object) array_merge((array) $form_state['ticket'], (array) $ticket);
        }
      }
    } catch (Exception $e) {
      $response = $e->getMessage();
      form_set_error('form', $response);
    }
  }
  else {
    drupal_set_message(t('Please install the Eventbrite API library. Check the !readme for further information.', array('!readme' => l(t('README'), 'admin/help/eventbrite_api'))), 'error');
  }
  $save = ticket_save($ticket);
  if (!empty($save)) {
    drupal_set_message(t('Ticket !name saved to Eventbrite and cached locally.', array('!name' => '<em>' . $ticket->name . '</em>')));
    $form_state['redirect'] = 'admin/eventbrite/ticket';
  }
}

/**
 * Ticket clone form.
 */
function ticket_clone_form($form, &$form_state, $ticket) {
  $form_state['ticket'] = $ticket;

  $form['#submit'][] = 'ticket_clone_form_submit';

  $form = confirm_form($form, t('Are you sure you want to clone ticket "@name"?', array('@name' => $ticket->name)), 'admin/content/events/event', '<p>' . t('This will clone the ticket on Eventbrite and clone it locally.') . '</p>', t('Clone'), t('Cancel'), 'confirm');

  return $form;
}

function ticket_add_wrapper() {
  $ticket = ticket_create();
  return drupal_get_form('ticket_add_form', $ticket);
}

function ticket_edit_wrapper($ticket) {
  return drupal_get_form('ticket_add_form', $ticket);
}

function ticket_delete_wrapper($ticket) {
  return drupal_get_form('ticket_delete_form', $ticket);
}

function ticket_clone_wrapper($ticket) {
  return drupal_get_form('ticket_clone_form', $ticket);
}

/**
 * Submit function for ticket clone form.
 */
function ticket_clone_form_submit($form, &$form_state) {
  // Assign them to an array to send to Eventbrite.
  $eventbrite = (array) $form_state['ticket'];
  // Ensure empty max field doesn't cause an error.
  if (empty($eventbrite['max'])) {
    $eventbrite['max'] = $eventbrite['quantity_available'];
  }
  // Connect to Eventbrite.
  if (($library = libraries_load('eventbrite')) && !empty($library['loaded'])) {
    $auth = array(
      'app_key' => variable_get('eventbrite_api_appkey', ''),
      'user_key' => variable_get('eventbrite_api_userkey', '')
    );
    $eb_client = new Eventbrite($auth);
    try {
      // Add the event id.
      $eventbrite['event_id'] = $eventbrite['eid'];
      $response = $eb_client->ticket_new($eventbrite);
      // If the event is added.
      if ($response->process->status == 'OK') {
        // Ticket ID.
        $tid = $response->process->id;
        // We have to call the actual event to get the full ticket details.
        $response = $eb_client->event_get(array('id' => $eventbrite['eid']));
        // Get the list of tickets.
        $tickets = $response->event->tickets;
        // Add some IDs.
        $ticket = array('tid' => $tid, 'eid' => $eventbrite['eid']);
        // Find the ticket we just added.
        foreach ($tickets as $t) {
          if ($t->ticket->id == $tid) {
            $ticket = $ticket + (array) $t->ticket;
            break;
          }
        }
        // Set the ticket.
        $ticket = (object) array_merge((array) $form_state['ticket'], (array) $ticket);
      }
    } catch (Exception $e) {
      $response = $e->getMessage();
      form_set_error('form', $response);
    }
    // Save the entity.
    $save = ticket_save($ticket);
    if (!empty($save)) {
      drupal_set_message(t('Ticket !name saved to Eventbrite and cached locally.', array('!name' => '<em>' . $ticket->name . '</em>')));
      $form_state['redirect'] = 'admin/eventbrite/ticket';
    }
  }
  else {
    drupal_set_message(t('Please install the Eventbrite API library. Check the !readme for further information.', array('!readme' => l(t('README'), 'admin/help/eventbrite_api'))), 'error');
  }
}

/**
 * Venue add form.
 */
function venue_add_form($form, &$form_state, $venue) {

  // Add the VID into the $form_state array.
  $form_state['vid'] = !empty($venue->vid) ? $venue->vid : '';

  // If we're editing an event we don't show the organizer ID.
  if (empty($venue->name)) {
    $form['oid'] = array(
      '#type' => 'textfield',
      '#title' => t('Organizer'),
      '#default_value' => isset($venue->oid) ? $venue->oid : '',
      '#maxlength' => 255,
      '#required' => TRUE,
      '#autocomplete_path' => 'eventbrite/organizer/autocomplete',
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($venue->name) ? $venue->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#default_value' => isset($venue->address) ? $venue->address : '',
    '#maxlength' => 255,
  );

  $form['address_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 2'),
    '#default_value' => isset($venue->address_2) ? $venue->address_2 : '',
    '#maxlength' => 255,
  );

  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => isset($venue->city) ? $venue->city : '',
    '#maxlength' => 255,
  );

  $form['region'] = array(
    '#type' => 'textfield',
    '#title' => t('Region'),
    '#default_value' => isset($venue->region) ? $venue->region : '',
    '#maxlength' => 255,
    '#description' => t('Please note that if your event is in the US, this field is mandatory and requires a 2 digit US state code. You can find a list of US state codes at !url.', array('!url' => l('https://en.wikipedia.org/wiki/List_of_U.S._state_abbreviations', 'https://en.wikipedia.org/wiki/List_of_U.S._state_abbreviations'))),
  );

  $form['postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal code'),
    '#default_value' => isset($venue->postal_code) ? $venue->postal_code : '',
    '#maxlength' => 255,
  );

  $form['country_code'] = array(
    '#type' => 'select',
    '#title' => t('Country code'),
    '#default_value' => !empty($venue->country_code) ? $venue->country_code : variable_get('eventbrite_api_country_code', 'US'),
    '#options' => eventbrite_api_countries(),
  );

  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $submit + array('venue_add_form_submit'),
  );

  $form['#validate'][] = 'venue_add_form_validate';
  // Add the field related form elements.
  $form_state['venue'] = $venue;
  $form_state['vid'] = $venue->vid;

  field_attach_form('venue', $venue, $form, $form_state);
  return $form;
}

/**
 * Validation function for event add form.
 */
function venue_add_form_validate($form, &$form_state) {
  $venue_submit = (object) $form_state['values'];
  field_attach_form_validate('event', $venue_submit, $form, $form_state);
}

/**
 * Submit function for venue add form.
 */
function venue_add_form_submit($form, &$form_state) {
  $venue_submit = (object) $form_state['values'];
  // Assign them to an array to send to Eventbrite.
  $eventbrite = $form_state['values'];
  // Add extra fields.
  field_attach_submit('venue', $venue_submit, $form, $form_state);
  // Connect to Eventbrite.
  $auth = array(
    'app_key' => variable_get('eventbrite_api_appkey', ''),
    'user_key' => variable_get('eventbrite_api_userkey', '')
  );
  $eb_client = new Eventbrite($auth);
  try {
    // Should we update an existing venue or add a new one?
    if (!empty($form_state['vid'])) {
      // Add the id with the correct key.
      $eventbrite['id'] = $form_state['vid'];
      $response = $eb_client->venue_update($eventbrite);
      // If the event is updated.
      if ($response->process->status == 'OK') {
        // Set the venue entity values.
        $venue = (object) array_merge((array) $form_state['venue'], (array) $form_state['values']);
      }
    }
    else {
      // Add the organizer id.
      $eventbrite['organizer_id'] = eventbrite_api_autocomplete_filter($eventbrite['oid']);
      $response = $eb_client->venue_new($eventbrite);
      // If the event is added.
      if ($response->process->status == 'OK') {
        // venue ID.
        $vid = $response->process->id;
        // Get the full venue details from Eventbrite.
        $response = $eb_client->venue_get(array('id' => $vid));
        // Assign details of the venue.
        $venue = $response->venue[0];
        // VID.
        $venue->vid = $vid;
        // Lat/long
        $venue->lat_long = $venue->latitude . ', ' . $venue->longitude;
        // Set the venue.
        $venue = (object) array_merge((array) $form_state['venue'], (array) $venue);
      }
    }
  } catch (Exception $e) {
    $response = $e->getMessage();
    form_set_error('form', $response);
  }
  $save = venue_save($venue);
  if (!empty($save)) {
    drupal_set_message(t('Venue !name saved to Eventbrite and cached locally.', array('!name' => '<em>' . check_plain($venue->name) . '</em>')));
    $form_state['redirect'] = 'admin/eventbrite/venue';
  }
}

/**
 * Form API submit callback for the venue delete button.
 */
function venue_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/eventbrite/venue/' . $form_state['vid'] . '/delete';
}

/**
 * Organizer delete form.
 */
function venue_delete_form($form, &$form_state, $venue) {
  $form_state['venue'] = $venue;

  $form['#submit'][] = 'venue_delete_form_submit';

  $form = confirm_form($form, t('Are you sure you want to delete venue "@name"?', array('@name' => $venue->name)), 'admin/content/events/event', '<p>' . t('This will delete the venue on Eventbrite and delete it locally. This action cannot be undone.') . '</p>', t('Delete'), t('Cancel'), 'confirm');

  return $form;
}

function venue_add_wrapper() {
  $venue = venue_create();
  return drupal_get_form('venue_add_form', $venue);
}

function venue_edit_wrapper($venue) {
  return drupal_get_form('venue_add_form', $venue);
}

function venue_delete_wrapper($venue) {
  return drupal_get_form('venue_delete_form', $venue);
}
