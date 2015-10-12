<?php

/**
 * @file
 * Pjirc module admin section
 */


/**
 * Menu callback. Shows PJIRC module settigns.
 */
function pjirc_admin($form, &$form_state) {

  $form['pjirc_general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pjirc general settings'),
    '#weight' => -6,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['pjirc_general_settings']['pjirc_allow_nick_selection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Nick selection for guests'),
    '#default_value' => variable_get('pjirc_allow_nick_selection', 0),
    '#description' => t('Allow anonymous users to choose a nick before connecting to IRC.'),
  );
  $form['pjirc_general_settings']['pjirc_allow_nick_selection_registered'] = array(
    '#type' => 'checkbox',
    '#title' => t('Nick selection for registered users'),
    '#default_value' => variable_get('pjirc_allow_nick_selection_registered', 0),
    '#description' => t('Allow registered users to choose a nick before connecting to IRC.'),
  );
  $form['pjirc_general_settings']['note'] = array(
    '#value' => 'Note: if none of this checkbox is enabled, registered users will connect to IRC with their username if possible, while guests will connect with the nick configured in the next section.',
  );

  $form['pjirc_connection_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('IRC connection settings'),
    '#weight' => -4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['pjirc_connection_settings']['pjirc_nick'] = array(
    '#type' => 'textfield',
    '#title' => t('Guest Nick Name'),
    '#default_value' => variable_get('pjirc_nick', 'Guest'),
    '#size' => 20,
    '#maxlength' => 30,
    '#description' => t('The username assigned to guests who have not logged into Drupal'),
  );
  $form['pjirc_connection_settings']['pjirc_realname'] = array(
    '#type' => 'textfield',
    '#title' => t('Real Name'),
    '#default_value' => variable_get('pjirc_realname', 'Java User'),
    '#size' => 40,
    '#maxlength' => 255,
    '#description' => t('The Real Name that will be show on whois of the user'),
  );
  $form['pjirc_connection_settings']['pjirc_quitmessage'] = array(
    '#type' => 'textfield',
    '#title' => t('Quit message'),
    '#default_value' => variable_get('pjirc_quitmessage', 'Java User'),
    '#size' => 50,
    '#maxlength' => 255,
    '#description' => t('This message will be shown when the user disconnects from IRC'),
  );
  $form['pjirc_connection_settings']['pjirc_server'] = array(
    '#type' => 'textfield',
    '#title' => t('IRC Server'),
    '#default_value' => variable_get('pjirc_server', 'irc.freenode.net'),
    '#size' => 20,
    '#maxlength' => 255,
    '#description' => t('The IP address or hostname of the IRC server you are connecting to'),
  );
  $form['pjirc_connection_settings']['pjirc_serverport'] = array(
    '#type' => 'textfield',
    '#title' => t('IRC Server'),
    '#default_value' => variable_get('pjirc_serverport', '6667'),
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The port to use when connecting to the irc server'),
  );
  $form['pjirc_connection_settings']['pjirc_room'] = array(
    '#type' => 'textfield',
    '#title' => t('IRC Room'),
    '#default_value' => variable_get('pjirc_room', ''),
    '#size' => 20,
    '#maxlength' => 255,
    '#description' => t('The IRC room you want to join initially. Should start with a # (eg: #myroom).'),
  );

  //**************************************************
  $form['pjirc_color_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colors'),
    '#weight' => -3,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attached' => array(
      'library' => array(array('system', 'farbtastic')),
      'css' => array(drupal_get_path('module', 'pjirc') . '/css/pjirc-admin.css'),
      'js' => array(drupal_get_path('module', 'pjirc') . '/js/pjirc-color.js'),
    ),
  );

  $form['pjirc_color_settings']['pjirc_color_selector'] =  array(
    '#type' => 'markup',
    '#markup' => '<div id="pickerholder"></div>',
  );

  $colorsdef = _pjirc_get_colorsdef_admin();

  foreach ($colorsdef as $colorname => $colordef) {
    $variablename = 'pjirc_' . $colorname;
    $form['pjirc_color_settings'][$variablename] = array(
      '#type' => 'textfield',
      '#title' => t($colordef['title']),
      '#default_value' => variable_get($variablename, $colordef['defvalue']),
      '#size' => 8,
      '#maxlength' => 128,
      '#description' => t($colordef['description']),
      '#attributes' => array('class' => array('color_fieldset')),
    );
  }


  //**************************************************

  $form['pjirc_pixx_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('pixx GUI settings'),
    '#weight' => -2,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['pjirc_pixx_settings']['size'] = array(
    '#type' => 'fieldset',
    '#title' => t('Size settings'),
    '#weight' => -3,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['pjirc_pixx_settings']['size']['pjirc_room_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Applet Width'),
    '#default_value' => variable_get('pjirc_room_width', '500'),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Specify the width (in px) of the applet.'),
  );
  $form['pjirc_pixx_settings']['size']['pjirc_room_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Applet Height'),
    '#default_value' => variable_get('pjirc_room_height', '400'),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Specify the height (in px) of the applet.'),
  );
  $form['pjirc_pixx_settings']['pjirc_font_color'] = array(
    '#type' => 'select',
    '#title' => t('Font color selection'),
    '#default_value' => variable_get('pjirc_font_color', 'true'),
    '#options' => array(
      'false' => t('disabled'),
      'true' => t('enabled'),
    ),
  );
  $form['pjirc_pixx_settings']['pjirc_font_type'] = array(
    '#type' => 'select',
    '#title' => t('Font type selection'),
    '#default_value' => variable_get('pjirc_font_type', 'true'),
    '#options' => array(
      'false' => t('disabled'),
      'true' => t('enabled'),
    ),
    '#description' => t('Font color must be enabled'),
  );
  $form['pjirc_pixx_settings']['pjirc_enablesmileys'] = array(
    '#type' => 'select',
    '#title' => t('Enable or disable smiley substitution'),
    '#default_value' => variable_get('pjirc_enablesmileys', 'true'),
    '#options' => array(
      'false' => t('disabled'),
      'true' => t('enabled'),
    ),
    '#description' => t('With this set to enabled, text smileys will be substituted by image smileys'),
  );
  $form['pjirc_pixx_settings']['pjirc_nickfield'] = array(
    '#type' => 'select',
    '#title' => t('Display a field for nickname changing'),
    '#default_value' => variable_get('pjirc_nickfield', 'true'),
    '#options' => array(
      'false' => t('disabled'),
      'true' => t('enabled'),
    ),
  );

  return system_settings_form($form);
}

function _pjirc_get_colorsdef_admin() {
  require_once drupal_get_path('module', 'pjirc') . '/' . 'pjirc.functions.inc.php';
  
  return _pjirc_get_colorsdef();
}
