<?php

/**
 * @file
 * Menu callbacks and page-specific functions for the Ooyala module.
 */

/**
 * Menu callback; Display global configuration options for Ooyala fields.
 */
function ooyala_settings_form($form, $form_state) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');

  // Display help messages if not yet configured.
  if (empty($form_state['post'])) {
    ooyala_settings_messages();
  }

  $form['ooyala_global_pcode'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('ooyala_global_pcode', ''),
    '#title' => t('Ooyala API Key'),
    '#max_length' => 28,
    '#description' => t('Enter your Ooyala API Key.'),
  );

  $form['ooyala_global_secret'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('ooyala_global_secret', ''),
    '#title' => t('Ooyala API Secret'),
    '#max_length' => 40,
    '#description' => t('Enter your Ooyala API Secret.'),
  );

  $form['ooyala_player_version'] = array(
    '#title' => t('Ooyala player version'),
    '#type' => 'radios',
    '#options' => array(
      'v3' => 'V3 (HTML5 with Flash fallback, more mobile friendly)',
      'v2' => 'V2 (Flash and iOS only)',
    ),
    '#default_value' => variable_get('ooyala_player_version', 'v3'),
    '#description' => t('Ooyala introduced a new "V3" player in late 2012. The new player has superior event handling and better responsive design support (100% width players). If you have legacy code, you may be required to use the V2 player, but if unsure, use the V3 player. See <a href="http://support.ooyala.com/developers/documentation/reference/player_v3_dev_faq.html">the Ooyala V3 FAQ</a> for more information about the V3 player.'),
  );

  $form['ooyala_players']['ooyala_player_id'] = array(
    '#type' => 'radios',
    '#title' => t('Ooyala default player (V3 only)'),
    '#default_value' => variable_get('ooyala_player_id', ''),
    '#options' => ooyala_api_player_list_options(),
    '#description' => t('The default player will be used if no specific player is requested on output. Information can be found in the <a href="http://support.ooyala.com/users/documentation/concepts/chapter_players.html">Ooyala documentation on players.</a>.'),
  );

  $form['ooyala_video_width'] = array(
    '#title' => t('Player width'),
    '#type' => 'textfield', 
    '#description' => t('The width of the video player when viewing content.'),
    '#default_value' => variable_get('ooyala_video_width', 400),
    '#size' => 6,
    '#field_suffix' => ' ' . t('pixels'),
  );

  $form['ooyala_video_height'] = array(
    '#title' => t('Player height'),
    '#type' => 'textfield', 
    '#description' => t('The height of the video player when viewing content.'),
    '#default_value' => variable_get('ooyala_video_height', 300),
    '#size' => 6,
    '#field_suffix' => ' ' . t('pixels'),
  );

  $form['ooyala_thumbnail_path'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('ooyala_thumbnail_path', 'ooyalathumbs'),
    '#title' => t('Thumbnail file path'),
    '#description' => t('The file path where thumbnail images will be saved. Do not include trailing slash.'),
    '#after_build' => array('ooyala_settings_path_validate'),
    '#validate' => array('ooyala_settings_path_validate'),
    '#field_prefix' => file_create_url(file_build_uri('')),
  );

  $form['ooyala_default_thumbnail_path'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('ooyala_default_thumbnail_path', FALSE),
    '#title' => t('Default thumbnail'),
    '#description' => t('If you want to use a default thumbnail, write its path here. This path is relative to the global files path to the site.'),
    '#after_build' => array('ooyala_settings_file_validate'),
    '#validate' => array('ooyala_settings_file_validate'),
    '#field_prefix' => file_create_url(file_build_uri('')),
  );

  // Allow the user to determine what type of node this should be.
  $options = ooyala_type_names();
  $default = variable_get('ooyala_primary_content_type', 'video');
  $form['ooyala_primary_content_type'] = array(
    '#type' => 'select',
    '#description' => t('This content type will be used as the default for the <a href="!url">batch Backlot video import</a>.', array('!url' => url('admin/content/ooyala/import'))),
    '#title' => t('Default content type'),
    '#options' => $options,
    '#default_value' => isset($options[$default]) ? $default : NULL,
  );

  if (empty($options)) {
    $form['ooyala_primary_content_type']['#disabled'] = TRUE;
    $form['ooyala_primary_content_type']['#options'][''] = t('No available types');
    $form['ooyala_primary_content_type']['#description'] = t('This content type will be used as the default for the <a href="!url">batch Backlot video import</a>.', array('!url' => url('admin/content/ooyala/import'))) . ' ' . t('There are currently no content types that contain an Ooyala field. Add an Ooyala field under <em>Manage fields</em> from the <a href="!url">Content type configuration</a>.', array('!url' => url('admin/structure/types')));
  }

  if (module_exists('colorbox')) {
    $form['ooyala_colorbox_height'] = array(
      '#type' => 'textfield', 
      '#description' => t('The height that the colorbox window will have when showing full player video.'),
      '#title' => t('Colorbox video height'),
      '#default_value' => variable_get('ooyala_colorbox_height', 400),
    );

    $form['ooyala_colorbox_width'] = array(
      '#type' => 'textfield', 
      '#description' => t('The width that the colorbox window will have when showing full player video.'),
      '#title' => t('Colorbox video width'),
      '#default_value' => variable_get('ooyala_colorbox_width', 500),
    );
    
    $form['ooyala_colorbox_autoplay'] = array(
      '#type' => 'checkbox',
      '#description' => t('When the video is shown in a colorbox dialog you usually want it to play automatically, uncheck this if you don\'t want this behavior.'),
      '#title' => t('Autoplay video in Colorbox'),
      '#default_value' => TRUE,
    );
    
  }

  $form['ooyala_reporting_level'] = array(
    '#type' => 'radios',
    '#title' => t('Reporting level'),
    '#options' => array(
      '1' => t('Verbose - display errors and status messages'),
      '0' => t('Quiet - log errors to watchdog (recommended for production sites)'),
    ),
    '#default_value' => variable_get('ooyala_reporting_level', 1),
  );

  $ping_help = '';
  $ping_help .= t('The Ooyala Ping is an option that you can use to retrieve thumbnails and publish content after it has completed processing by the Ooyala servers.');
  $ping_help .= ' ' . t('To enable the Ooyala ping, you must set up the "API Ping URL" <strong>@url</strong> in your <a href="http://www.ooyala.com/backlot/web">Ooyala Backlot account</a> (under the "Account" => "Developers" tab).', array('@url' => url('ooyala/ping', array('absolute' => TRUE))));

  $form['ooyala_ping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ooyala ping settings'),
    '#collapsible' => FALSE,
    '#description' => $ping_help,
  );

  $form['ooyala_ping']['ooyala_autopublish'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-publish content when videos finish processing.'),
    '#default_value' => variable_get('ooyala_autopublish', FALSE),
    '#description' => t('Upon receiving a ping, publish nodes that contain the video that has finished processing.'),
  );

  $form['ooyala_ping']['ooyala_ping_reporting_verbose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verbose ping reporting'),
    '#default_value' => t(variable_get('ooyala_ping_reporting_verbose', FALSE)),
    '#description' => t('Log additional messages to the watchdog during each step processing an Ooyala ping including a complete copy of the incoming ping request.'),
  );

  // Add the save button and normal submit handlers.
  $form = system_settings_form($form);
  $form['buttons']['#weight'] = 20;
  return $form;
}

/**
 * Print out messages checking the current Ooyala configuration.
 */
function ooyala_settings_messages() {
  if (!variable_get('ooyala_global_pcode', '')) {
    $help = '';
    $help .= t('Welcome to the Ooyala module! Before you can upload any content you need to configure your site for syncronizing with Ooyala. To do this, you need to both enter and retrieve some information to and from Ooyala.');
    $steps = array(
      t('If you haven\'t yet, you need to <a target="_blank" href="http://www.ooyala.com/free_trial">create an Ooyala account</a>. After you have an account, you need to sign-in to the <a target="_blank" href="https://backlot.ooyala.com/backlot/web">Ooyala admin interface (Backlot)</a>.'),
      t('Once logged in to Backlot, click on the "Account" tab. Then click on the "Developers" tab. Enter the following into "API Ping URL":') . '<br /><strong>' . url('ooyala/ping', array('absolute' => TRUE)) . '</strong><br />' . t('Note that your site must be publicly accessible from the internet for this step to work. If your site is not publicly available, some features of the Ooyala module will not work (auto-publishing), while other features will work but be delayed until cron jobs (thumbnail and attribute retreival).'),
      t('On that same page, there are two values for "API Key" And "API Secret". Copy and paste these values from Backlot into the fields below.'),
      t('The Ooyala module is very dependent upon cron jobs, ensure that "Cron maintenance tasks" is set up properly on your site by checking your <a href="!url">site status report</a>.', array('!url' => url('admin/reports/status'))),
    );
    $help .= theme('item_list', array('items' => $steps, 'title' => NULL, 'type' => 'ol'));
    drupal_set_message($help);
  }
  elseif (ooyala_api_available()) {
    $details = array();
    ooyala_api_video_query(array('limit' => 1), $details);
    if ($details) {
      drupal_set_message(t('The Ooyala servers have been contacted and your API Key and Secret code have been confirmed.'), 'status', FALSE);
    }
    else {
      drupal_set_message(t('Check that your API Key and Secret code values are correct. These can be obtained from your <a href="https://backlot.ooyala.com/backlot/web">Ooyala Backlot account</a> under the "Account" => "Developers" sub-tab.'), 'error');
    }
  }
  else {
    drupal_set_message(t('The Ooyala service could not be reached. Check that your server is able to make HTTP requests.'), 'warning');
  }
}

/**
 * Element #validate function to ensure the entered directory is writable.
 */
function ooyala_settings_path_validate($element) {
  if (!empty($element['#value'])) {
    $uri = file_build_uri($element['#value']);
    $path = file_prepare_directory($uri, FILE_CREATE_DIRECTORY);
    if (!$path) {
      form_error($element, t("The directory %path is not writable or could not be created. Check your server's file system permissions.", array('%path' => $path)));
    }
  }
  return $element;
}

/**
 * Element #validate function to ensure a file exists.
 */
function ooyala_settings_file_validate($element) {
  $uri = file_build_uri($element['#value']);
  $path = file_prepare_directory($uri, FILE_CREATE_DIRECTORY);
  if (!empty($element['#value']) && !is_file($path)) {
    form_error($element, t('The image at %path could not be found.', array('%path' => $path)));
  }
  return $element;
}

/**
 * Menu callback; Retrieve a new thumbnail from the Ooyala servers.
 */
function ooyala_refresh_thumbnail() {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');
  $token = drupal_get_token();

  $data = array();
  if (TRUE || isset($_GET['token']) && $_GET['token'] == $token) {
    if (isset($_GET['embed_code']) && $_GET['embed_code']) {
      $embed_code = $_GET['embed_code'];
      // Try to retrieve a new thumbnail.
      $thumb_path = ooyala_api_fetch_image($embed_code);
      if ($thumb_path != FALSE) {
        $data['content'] = theme('ooyala_upload_preview', array('embed_code' => $embed_code));
        $data['message'] = '';
        $data['error'] = 0;

        // Flush out all existing image styles.
        $styles = image_styles();
        foreach ($styles as $style) {
          image_style_flush($style);
        }
      }
      else {
        $data['content'] = theme('ooyala_upload_preview');
        $data['message'] = t('A thumbnail was not able to be retrieved. Check that the video code is correct and that the video has finished processing.');
        $data['error'] = 0;
      }

      $data['content'] .= theme('ooyala_video_data', array('data_type' => 'length', 'data' => ooyala_api_video_property($embed_code, 'length')));
      $data['content'] .= theme('ooyala_video_data', array('data_type' => 'status', 'data' =>  ooyala_api_video_property($embed_code, 'status')));

    }
    else {
      $data['message'] = t('A video code must be provided before it can be previewed.');
      $data['error'] = 1;
    }
  }
  else {
    $data['message'] = t('Invalid token.');
    $data['error'] = 1;
  }

  $data['field_id'] = isset($_GET['field_id']) ? $_GET['field_id'] : '';

  drupal_json_output($data);
}
