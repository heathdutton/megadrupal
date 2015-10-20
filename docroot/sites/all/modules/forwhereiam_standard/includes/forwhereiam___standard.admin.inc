<?php
/**
 * @file
 * This file contains administration functions implementation.
 */

/**
 * ForWhereiAm Standard settings form.
 */
function forwhereiam___standard_admin_form($block_name = '') {

  $form = array();
  $form['forwhereiam___standard_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => variable_get('forwhereiam___standard_client_id'),
    '#maxlength' => 60,
    '#description' => t("The client ID to use for making the requests. This can be obtained from a forWhereiAm Enterprise account."),
    '#required' => TRUE,
    '#element_validate' => array('forwhereiam___standard_admin_validate'),
  );

  $form['forwhereiam___standard_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Secret'),
    '#default_value' => variable_get('forwhereiam___standard_client_secret'),
    '#maxlength' => 60,
    '#description' => t("The client secret to use for making the requests. This can be obtained from a forWhereiAm Enterprise account."),
    '#required' => TRUE,
    '#element_validate' => array('forwhereiam___standard_admin_validate'),
  );

  $form['forwhereiam___standard_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height of block (in pixels)'),
    '#default_value' => variable_get('forwhereiam___standard_height', 280),
    '#maxlength' => 4,
    '#size' => 6,
    '#description' => t("The height of the widget in pixels (internal content area). The 'pixels' unit is implicit and should not be entered above. Default height is assumed to be 280px."),
    '#required' => FALSE,
    '#element_validate' => array('forwhereiam___standard_admin_validate'),
  );

  $form['forwhereiam___standard_refresh_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Refresh interval (in seconds)'),
    '#default_value' => variable_get('forwhereiam___standard_refresh_interval', 60),
    '#maxlength' => 4,
    '#size' => 6,
    '#description' => t("Optionally specify the number of seconds after which to reload. The 'seconds' unit is implicit and should not be entered above. Minimum interval is 60 seconds."),
    '#required' => FALSE,
    '#element_validate' => array('forwhereiam___standard_admin_validate'),
  );

  $form['forwhereiam___standard_geolocate'] = array(
    '#type' => 'checkbox',
    '#title' => t("Show 'guess my postcode' link on the widget"),
    '#default_value' => variable_get('forwhereiam___standard_geolocate', TRUE),
    '#element_validate' => array('forwhereiam___standard_admin_validate'),
  );

  $form['forwhereiam___standard_signup'] = array(
    '#type' => 'checkbox',
    '#title' => t("Show 'signup for alerts' link on the widget"),
    '#default_value' => variable_get('forwhereiam___standard_signup', TRUE),
    '#element_validate' => array('forwhereiam___standard_admin_validate'),
  );

  $form['forwhereiam___standard_initial_screen'] = array(
    '#title' => t('Content to display on initial screen'),
    '#type' => 'textarea',
    '#description' => t('Content to display on initial screen if we have no user location to perform a search with. Can add plain text or HTML code here.'),
    '#default_value' => variable_get('forwhereiam___standard_initial_screen', '<p>Enter your postcode to see all relevant announcements for you.</p>'),
    '#rows' => 4,
    '#resizable' => TRUE,
  );

  $form['forwhereiam___standard_show_map'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show maps where applicable'),
    '#description' => t("Display an embedded map in an announcement's detailed view, if any coordinates have been set for that announcement."),
    '#default_value' => variable_get('forwhereiam___standard_show_map', FALSE),
    '#element_validate' => array('forwhereiam___standard_admin_validate'),
  );

  $form['forwhereiam___standard_map_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Maps (v3) Key'),
    '#description' => t("Google Maps v3 API is used by this module for displaying maps. Please enter your unique API key here. You can obtain a key by following instructions given <a href='@google-maps'>here</a>", array("@google-maps" => "https://developers.google.com/maps/documentation/javascript/tutorial#api_key")),
    '#default_value' => variable_get('forwhereiam___standard_map_key', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="forwhereiam___standard_show_map"]' => array('checked' => TRUE),
      ),
      '#element_validate' => array('forwhereiam___standard_admin_validate'),
    ),
  );

  $form['forwhereiam___standard_show_sharing_buttons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show AddThis social sharing buttons where applicable'),
    '#description' => t("Display AddThis social sharing buttons on an announcement's detailed view, if sharing of an announcement is enabled."),
    '#default_value' => variable_get('forwhereiam___standard_show_sharing_buttons', FALSE),
    '#element_validate' => array('forwhereiam___standard_admin_validate'),
  );

  $form['forwhereiam___standard_addthis_pubid'] = array(
    '#type' => 'textfield',
    '#title' => t('AddThis Publisher Profile ID'),
    '#description' => t("AddThis social sharing buttons are used if an announcement is enabled to be shared. Please enter your unique AddThis Publisher Profile ID here. You can obtain an ID by following instructions given <a href='@addthis'>here</a>", array("@addthis" => "http://www.addthis.com/settings/publisher")),
    '#default_value' => variable_get('forwhereiam___standard_addthis_pubid', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="forwhereiam___standard_show_sharing_buttons"]' => array('checked' => TRUE),
      ),
      '#element_validate' => array('forwhereiam___standard_admin_validate'),
    ),
  );

  $form['forwhereiam___standard_show_ratings'] = array(
    '#type' => 'checkbox',
    '#title' => t("Show rating stars for announcements which have ratings enabled"),
    '#default_value' => variable_get('forwhereiam___standard_show_ratings', TRUE),
    '#element_validate' => array('forwhereiam___standard_admin_validate'),
  );

  return system_settings_form($form);
}

/**
 * Validate and set the user's configuration settings for the widget.
 */
function forwhereiam___standard_admin_validate($form, &$form_state) {

  $client_id = $form_state['values']['forwhereiam___standard_client_id'];
  if (!is_string($client_id) || strlen($client_id) < 30 || strlen($client_id) > 60) {
    form_set_error('forwhereiam___standard_client_id', t('Please check your client ID again. It appears incorrect.'));
  }

  $client_secret = $form_state['values']['forwhereiam___standard_client_secret'];
  if (!is_string($client_secret) || strlen($client_secret) < 10 || strlen($client_secret) > 60) {
    form_set_error('forwhereiam___standard_client_secret', t('Please check your client secret again. It appears incorrect.'));
  }

  $height = $form_state['values']['forwhereiam___standard_height'];
  if (!empty($height)) {
    if (!is_numeric($height) || $height < 60 || $height > 2000) {
      form_set_error('forwhereiam___standard_height', t('Please check you specified a valid height value (min. 60). It can only be a numerical value.'));
    }
  }

  $refresh_interval = $form_state['values']['forwhereiam___standard_refresh_interval'];
  if (!empty($refresh_interval)) {
    if (!is_numeric($refresh_interval) || $refresh_interval < 60) {
      form_set_error('forwhereiam___standard_refresh_interval', t('Please check you specified a valid refresh interval (min. 60). It can only be a numerical value.'));
    }
  }

  $show_map = (bool) $form_state['values']['forwhereiam___standard_show_map'];
  if ($show_map == TRUE) {

    $map_key = $form_state['values']['forwhereiam___standard_map_key'];
    if (empty($map_key)) {
      form_set_error('forwhereiam___standard_map_key', t('You must provide a Google Maps v3 API key to enable the rendering of coordinates on a map with an announcement, where appropriate.'));
    }
    else {
      if (!is_string($map_key) || strlen($map_key) < 30 || strlen($map_key) > 60) {
        form_set_error('forwhereiam___standard_map_key', t('Please check your Google Map API key. It appears incorrect.'));
      }
    }
  }

  $show_sharing_buttons = (bool) $form_state['values']['forwhereiam___standard_show_sharing_buttons'];
  if ($show_sharing_buttons == TRUE) {

    $addthis_pubid = $form_state['values']['forwhereiam___standard_addthis_pubid'];
    if (empty($addthis_pubid)) {
      form_set_error('forwhereiam___standard_addthis_pubid', t('You must provide an addThis Publisher Profile ID if you want to use social sharing buttons in your widget.'));
    }
    else {
      if (!is_string($addthis_pubid) || strlen($addthis_pubid) < 10 || strlen($addthis_pubid) > 40) {
        form_set_error('forwhereiam___standard_addthis_pubid', t('Please check your addThis Publisher Profile ID. It appears incorrect.'));
      }
    }
  }

}
