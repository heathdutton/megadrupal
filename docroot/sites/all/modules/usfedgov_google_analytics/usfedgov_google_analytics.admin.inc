<?php

/**
 * @file
 * Callback functions for the administration page.
 */


/**
 * Page callback: Federal Google Analytics settings.
 *
 * These are the optional configurable fields for the library:
 *   agency
 *   subagency
 *   sp
 *   exts
 *   yt
 *   sdor
 *   dclink
 *   aip
 *   pua
 *   enhlink
 *   autotracker
 *   forcessl
 *   optout
 *   fedagencydim
 *   fedsubagencydim
 *   fedversiondim
 *   palagencydim
 *   palsubagencydim
 *   palversiondim
 *   maincd
 *   parallelcd
 *   cto
 *
 * @see usfedgov_google_analytics_menu()
 *
 * @param $form - Array() this is the container for all the form elements
 * @param $submittable - Should the for be able to be submitted? Since the form
 *        is also used to pass specific information on to
 *        @see usfedgov_google_analytics_process_html() we don't want
 *        system_settings_form() to always be called.
 */
function usfedgov_google_analytics_form($form = array(), $submittable = TRUE) {
  $default_settings = usfedgov_google_analytics_default_settings();
  $configured_settings = variable_get('usfedgov_google_analytics__settings', array());
  $settings = array_replace_recursive($default_settings, $configured_settings);

  $form['library'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Library'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );/*
  $form['library']['usfedgov_google_analytics__load_from_cdn'] = array(
    '#type'             => 'checkbox',
    '#title'            => t('Load Analytics library from CDN'),
    '#description'      => t('This will use the DAP analytics library from http://analytics.usa.gov hosted by their CloudFlare CDN. Currently this is the only option.'),
    '#default_value'    => variable_get('usfedgov_google_analytics__load_from_cdn', TRUE),
    '#disabled'         => TRUE,
  );*/

  $form['library']['usfedgov_google_analytics__load_minified'] = array(
    '#type'             => 'radios',
    '#title'            => t('Library Version'),
    '#description'      => t('Which version of the library should be loaded?'),
    '#default_value'    => variable_get('usfedgov_google_analytics__load_minified', TRUE),
    '#options'          => array(
      FALSE         => t('Original'),
      TRUE          => t('Minified'),
    ),
    '#states'            => array(
      'visible'     => array(
        ':input[name="usfedgov_google_analytics__load_from_cdn"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['usfedgov_google_analytics__settings'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Settings'),
    '#description'  => t('More information about the settings can be found in <a href="!analytics_instructions_url">the DAP analytics tool instructions</a> or <a href="!quick_guide_url">the GSA DAP UA code quick guide</a>.', array('!analytics_instructions_url' => 'http://www.digitalgov.gov/services/dap/analytics-tool-instructions/', '!quick_guide_url' => 'https://www.digitalgov.gov/files/2015/02/GSA-DAP-UA-Code-Quick-Guide-15-01-30-v1-02_mvf.pdf')),
    '#collapsible'  => FALSE,
    '#tree'         => TRUE,
  );

  $form['usfedgov_google_analytics__settings']['agency'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Agency'),
    '#description'      => t('The main Federal agency which this website is part of. i.e. DHS'),
    '#default_value'    => $settings['agency'],
    '#required'         => TRUE,
  );
  $form['usfedgov_google_analytics__settings']['subagency'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Sub-Agency'),
    '#description'      => t('The sub-agency, if any, which this website is part of. i.e. FEMA'),
    '#default_value'    => $settings['subagency'],
  );
  $form['usfedgov_google_analytics__settings']['extra'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Extra Settings'),
    '#description'  => t('These all come with default settings which will be used if left blank.'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );

  $form['usfedgov_google_analytics__settings']['extra']['sp'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Search Params'),
    '#description'      => t('Default: "!default"', array('!default' => 'q|querytext|nasaInclude|k|QT|')),
    '#default_value'    => $settings['extra']['sp'],
  );
  $form['usfedgov_google_analytics__settings']['extra']['exts'] = array(
    '#type'             => 'textfield',
    '#title'            => t('File Extensions'),
    '#description'      => t('Default: "!default"', array('!default' => 'doc|docx|xls|xlsx|xlsm|ppt|pptx|exe|zip|pdf|js|txt|csv|dxf|dwgd|rfa|rvt|dwfx|dwg|wmv|jpg|msi|7z|gz|tgz|wma|mov|avi|mp3|mp4|csv|mobi|epub|swf|rar')),
    '#default_value'    => $settings['extra']['exts'],
  );
  $form['usfedgov_google_analytics__settings']['extra']['yt'] = array(
    '#type'             => 'radios',
    '#title'            => t('Youtube'),
    '#default_value'    => $settings['extra']['yt'],
    '#options'          => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  $form['usfedgov_google_analytics__settings']['extra']['sdor'] = array(
    '#type'             => 'radios',
    '#title'            => t('Subdomain Based'),
    '#default_value'    => $settings['extra']['sdor'],
    '#options'          => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  $form['usfedgov_google_analytics__settings']['extra']['dclink'] = array(
    '#type'             => 'radios',
    '#title'            => t('DoubleClick Link'),
    '#default_value'    => $settings['extra']['dclink'],
    '#options'          => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  $form['usfedgov_google_analytics__settings']['extra']['aip'] = array(
    '#type'             => 'radios',
    '#title'            => t('Anonymize IP'),
    '#default_value'    => $settings['extra']['aip'],
    '#options'          => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  $form['usfedgov_google_analytics__settings']['extra']['pua'] = array(
    '#type'             => 'textfield',
    '#title'            => t('PUA_NAME'),
    '#description'      => t('Default: "!default"', array('!default' => 'GSA_CP')),
    '#default_value'    => $settings['extra']['pua'],
  );
  $form['usfedgov_google_analytics__settings']['extra']['enhlink'] = array(
    '#type'             => 'radios',
    '#title'            => t('Enhanced Link'),
    '#default_value'    => $settings['extra']['enhlink'],
    '#options'          => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  $form['usfedgov_google_analytics__settings']['extra']['autotracker'] = array(
    '#type'             => 'radios',
    '#title'            => t('Autotracker'),
    '#default_value'    => $settings['extra']['autotracker'],
    '#options'          => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  $form['usfedgov_google_analytics__settings']['extra']['forcessl'] = array(
    '#type'             => 'radios',
    '#title'            => t('Force SSL'),
    '#default_value'    => $settings['extra']['forcessl'],
    '#options'          => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  $form['usfedgov_google_analytics__settings']['extra']['optout'] = array(
    '#type'             => 'radios',
    '#title'            => t('Opt Out Page'),
    '#default_value'    => $settings['extra']['optout'],
    '#options'          => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  $form['usfedgov_google_analytics__settings']['extra']['fedagencydim'] = array(
    '#type'             => 'textfield',
    '#title'            => t('MAIN_AGENCY_CUSTOM_DIMENSION_SLOT'),
    '#description'      => t('Default: "!default"', array('!default' => 'dimension1')),
    '#default_value'    => $settings['extra']['fedagencydim'],
  );
  $form['usfedgov_google_analytics__settings']['extra']['fedsubagencydim'] = array(
    '#type'             => 'textfield',
    '#title'            => t('MAIN_SUBAGENCY_CUSTOM_DIMENSION_SLOT'),
    '#description'      => t('Default: "!default"', array('!default' => 'dimension2')),
    '#default_value'    => $settings['extra']['fedsubagencydim'],
  );
  $form['usfedgov_google_analytics__settings']['extra']['fedversiondim'] = array(
    '#type'             => 'textfield',
    '#title'            => t('MAIN_CODEVERSION_CUSTOM_DIMENSION_SLOT'),
    '#description'      => t('Default: "!default"', array('!default' => 'dimension3')),
    '#default_value'    => $settings['extra']['fedversiondim'],
  );
  $form['usfedgov_google_analytics__settings']['extra']['palagencydim'] = array(
    '#type'             => 'textfield',
    '#title'            => t('PARALLEL_AGENCY_CUSTOM_DIMENSION_SLOT'),
    '#description'      => t('Default: "!default"', array('!default' => 'dimension1')),
    '#default_value'    => $settings['extra']['palagencydim'],
  );
  $form['usfedgov_google_analytics__settings']['extra']['palsubagencydim'] = array(
    '#type'             => 'textfield',
    '#title'            => t('PARALLEL_SUBAGENCY_CUSTOM_DIMENSION_SLOT'),
    '#description'      => t('Default: "!default"', array('!default' => 'dimension2')),
    '#default_value'    => $settings['extra']['palsubagencydim'],
  );
  $form['usfedgov_google_analytics__settings']['extra']['palversiondim'] = array(
    '#type'             => 'textfield',
    '#title'            => t('PARALLEL_CODEVERSION_CUSTOM_DIMENSION_SLOT'),
    '#description'      => t('Default: "!default"', array('!default' => 'dimension3')),
    '#default_value'    => $settings['extra']['palversiondim'],
  );
  $form['usfedgov_google_analytics__settings']['extra']['maincd'] = array(
    '#type'             => 'radios',
    '#title'            => t('USE_MAIN_CUSTOM_DIMENSIONS'),
    '#default_value'    => $settings['extra']['maincd'],
    '#options'          => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  $form['usfedgov_google_analytics__settings']['extra']['parallelcd'] = array(
    '#type'             => 'radios',
    '#title'            => t('USE_PARALLEL_CUSTOM_DIMENSIONS'),
    '#default_value'    => $settings['extra']['parallelcd'],
    '#options'          => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  $form['usfedgov_google_analytics__settings']['extra']['cto'] = array(
    '#type'             => 'textfield',
    '#title'            => t('COOKIE_TIMEOUT'),
    '#description'      => t('Default: "!default"', array('!default' => '24')),
    '#default_value'    => $settings['extra']['cto'],
  );
  if ($submittable) {
    return system_settings_form($form);
  }
  else {
    return $form;
  }
}
