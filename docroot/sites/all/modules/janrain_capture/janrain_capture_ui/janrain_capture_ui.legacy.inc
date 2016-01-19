<?php

/**
 * @file
 * Admin settings menu
 */

/**
 * Sets up the admin settings form for this module.
 */
function janrain_capture_ui_settings() {
  $janrain_capture_main = variable_get('janrain_capture_main', array());
  $form['janrain_capture_main'] = array(
    '#type' => 'fieldset',
    '#title' => t('Application Settings'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['janrain_capture_main']['capture_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => !empty($janrain_capture_main['capture_client_id']) ? $janrain_capture_main['capture_client_id'] : '',
  );
  $form['janrain_capture_main']['capture_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Secret'),
    '#default_value' => !empty($janrain_capture_main['capture_client_secret']) ? $janrain_capture_main['capture_client_secret'] : '',
  );
  $form['janrain_capture_main']['capture_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Capture URL'),
    '#default_value' => !empty($janrain_capture_main['capture_address']) ? $janrain_capture_main['capture_address'] : '',
    '#description' => t('For example, myapp.janraincapture.com.'),
  );

  $janrain_capture_fields = variable_get('janrain_capture_fields', array());
  $form['janrain_capture_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field Settings'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['janrain_capture_fields']['capture_enforce_verification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require email verification'),
    '#default_value' => !empty($janrain_capture_fields['capture_enforce_verification']) ? $janrain_capture_fields['capture_enforce_verification'] : 0,
    '#description' => t('Prompt users who have not verified their email address to do so before completing authentication'),
  );
  $form['janrain_capture_fields']['capture_match_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Match users by email address'),
    '#default_value' => !empty($janrain_capture_fields['capture_match_email']) ? $janrain_capture_fields['capture_match_email'] : 0,
    '#description' => t('Associate user and Capture accounts if both share the same email address.'),
    '#states' => array(
      'enabled' => array(
        ':input[name="janrain_capture_fields[capture_enforce_verification]"]' => array('checked' => TRUE),
      )
    ),
  );
  $form['janrain_capture_fields']['capture_map_profile_pic'] = array(
    '#type' => 'checkbox',
    '#title' => t('Map profile pic'),
    '#default_value' => isset($janrain_capture_fields['capture_map_profile_pic']) ? $janrain_capture_fields['capture_map_profile_pic'] : 1,
    '#description' => t('Automatically map the Capture profile photo to the Drupal user profile picture.'),
  );
  $form['janrain_capture_fields']['capture_preferred_photo_variant'] = array(
    '#type' => 'select',
    '#title' => t('Preferred photo variant'),
    '#options' => array(
      'small' => t('Small'),
      'normal' => t('Normal'),
      'large' => t('Large'),
      'square_thumbnail' => t('Square Thumbnail'),
      'original' => t('Original')
    ),
    '#default_value' => isset($janrain_capture_fields['capture_preferred_photo_variant']) ? $janrain_capture_fields['capture_preferred_photo_variant'] : 'small',
    '#description' => t('Select the preferred photo variant to use from the Capture profile. When it\'s not available, the "other" variant will be used as fallback'),
  );
  $janrain_capture_optional = variable_get('janrain_capture_optional', array());
  $form['janrain_capture_optional'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Settings (optional)'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['janrain_capture_optional']['capture_sso_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Single sign on domain'),
    '#default_value' => !empty($janrain_capture_optional['capture_sso_address']) ? $janrain_capture_optional['capture_sso_address'] : '',
    '#description' => t('For example, myapp.janrainsso.com.'),
  );

  $form['janrain_capture_optional']['capture_sso_segment_name'] = array(
    '#type' => 'textfield',
    '#title' => t('SSO Segment Name'),
    '#default_value' => !empty($janrain_capture_optional['capture_sso_segment_name']) ? $janrain_capture_optional['capture_sso_segment_name'] : '',
    '#description' => t('A segment is used to identify a group of sites within your federated web properties to enable a more granular SSO experience. For more information, see http://developers.janrain.com/documentation/federate/segments/'),
  );
  $form['janrain_capture_optional']['capture_sso_supported_segment_names'] = array(
    '#type' => 'textarea',
    '#title' => t('SSO Supported Segment Names'),
    '#default_value' => !empty($janrain_capture_optional['capture_sso_supported_segment_names']) ? $janrain_capture_optional['capture_sso_supported_segment_names'] : '',
    '#description' => t('This is a list of segments, outside of the SSO Segment Name, that are permitted to access your site via SSO. Segment names should be separated by commas. Example: one,two,three,four'),
  );
  
  $form['janrain_capture_optional']['backplane_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Backplane server base URL'),
    '#default_value' => !empty($janrain_capture_optional['backplane_server']) ? $janrain_capture_optional['backplane_server'] : '',
    '#description' => t('For example, http://backplane1.janrainbackplane.com/v1.1.'),
  );
  $form['janrain_capture_optional']['backplane_bus_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Backplane bus name'),
    '#default_value' => !empty($janrain_capture_optional['backplane_bus_name']) ? $janrain_capture_optional['backplane_bus_name'] : '',
    '#description' => t('For example, domain.com.'),
  );
  $form['janrain_capture_optional']['backplane_js_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Backplane JavaScript URL'),
    '#default_value' => !empty($janrain_capture_optional['backplane_js_path']) ? $janrain_capture_optional['backplane_js_path'] : '',
    '#description' => t('The URL to your site\'s backplane.js file.'),
  );

  $group = 'janrain_capture_share';
  $conf = variable_get($group, array());
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Share Settings (optional)'),
    '#group' => 'settings',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('To configure social sharing providers visit <a href="@social_link" target="_blank">Janrain\'s rpxnow site</a>.', array('@social_link' => 'https://rpxnow.com')),
  );
  $form[$group]['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable social share.'),
    '#default_value' => isset($conf['enabled']) ? $conf['enabled'] : 0,
    '#description' => t('Enable social sharing for the site. Don\'t forget to edit the social sharing settings for individual <a href="!content_types_link" target="_blank">content types</a>.', array('!content_types_link' => url('admin/structure/types/'))),
  );
  $janrain_capture_main = variable_get('janrain_capture_main2', array());
  $form[$group]['engage_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Engage URL'),
    '#default_value' => !empty($janrain_capture_main['engage_address']) ? $janrain_capture_main['engage_address'] : '',
    '#description' => t('For example, https://myapp.rpxnow.com.'),
  );

  $sgroup = 'nodes';
  $sconf = isset($conf[$sgroup]) ? $conf[$sgroup] : array();
  $form[$group][$sgroup] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Social sharing settings for content'),
  );
  $form[$group][$sgroup]['default_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Default message for shared content'),
    '#default_value' => isset($sconf['default_message']) ? $sconf['default_message'] : JANRAIN_CAPTURE_SHARE_MESSAGE_NODES,
    '#description' => t('Specify the default message text (including tokens) to be used when sharing content. This setting can be overridden on the content type edit page.')
  );
  $form[$group][$sgroup]['default_linktext'] = array(
    '#type' => 'textfield',
    '#title' => t('Default link text for shared content'),
    '#default_value' => isset($sconf['default_linktext']) ? $sconf['default_linktext'] : JANRAIN_CAPTURE_SHARE_LINKTEXT_NODES,
    '#description' => t('Specify the default action description (including tokens) to be used when sharing content.  This setting can be overridden on the content type edit page.')
  );
  $form[$group][$sgroup]['default_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default title for shared content'),
    '#default_value' => isset($sconf['default_title']) ? $sconf['default_title'] : JANRAIN_CAPTURE_SHARE_TITLE_NODES,
    '#description' => t('Specify the default title (including tokens) to be used when sharing content. This setting can be overridden on the content type edit page.')
  );
  $form[$group][$sgroup]['default_summary'] = array(
    '#type' => 'textfield',
    '#title' => t('Default description for shared content'),
    '#default_value' => isset($sconf['default_summary']) ? $sconf['default_summary'] : JANRAIN_CAPTURE_SHARE_SUMMARY_NODES,
    '#description' => t('Specify the default description (including tokens) to be used when sharing content. This setting can be overridden on the content type edit page.')
  );
  $form[$group][$sgroup]['providers'] = array(
    '#type' => 'textfield',
    '#title' => t('Comma-separated list of providers'),
    '#default_value' => isset($sconf['providers']) ? implode(', ', $sconf['providers']) : '',
    '#description' => t('Specify a comma-separated list of providers whose icons should be displayed on the share button. For example: facebook, twitter, yahoo, linkedin.')
  );

  $sgroup = 'comments';
  $sconf = isset($conf[$sgroup]) ? $conf[$sgroup] : array();
  $form[$group][$sgroup] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Social sharing settings for comments'),
  );
  $form[$group][$sgroup]['default_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Default message for shared comments'),
    '#default_value' => isset($sconf['default_message']) ? $sconf['default_message'] : JANRAIN_CAPTURE_SHARE_MESSAGE_COMMENTS,
    '#description' => t('Specify the default message text (including tokens) to be used when sharing comments. This setting can be overridden on the content type edit page.')
  );
  $form[$group][$sgroup]['default_linktext'] = array(
    '#type' => 'textfield',
    '#title' => t('Default link text for shared comments'),
    '#default_value' => isset($sconf['default_linktext']) ? $sconf['default_linktext'] : JANRAIN_CAPTURE_SHARE_LINKTEXT_COMMENTS,
    '#description' => t('Specify the default action description (including tokens) to be used when sharing comments.  This setting can be overridden on the content type edit page.')
  );
  $form[$group][$sgroup]['default_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default title for shared comments'),
    '#default_value' => isset($sconf['default_title']) ? $sconf['default_title'] : JANRAIN_CAPTURE_SHARE_TITLE_COMMENTS,
    '#description' => t('Specify the default title (including tokens) to be used when sharing comments. This setting can be overridden on the content type edit page.')
  );
  $form[$group][$sgroup]['default_summary'] = array(
    '#type' => 'textfield',
    '#title' => t('Default description for shared comments'),
    '#default_value' => isset($sconf['default_summary']) ? $sconf['default_summary'] : JANRAIN_CAPTURE_SHARE_SUMMARY_COMMENTS,
    '#description' => t('Specify the default description (including tokens) to be used when sharing comments. This setting can be overridden on the content type edit page.')
  );

  // Add the token tree UI.
  if (module_exists('token')) {
    $form[$group]['token_help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('user', 'node', 'comment', 'site'),
      '#global_types' => FALSE,
    );
  }
  else {
    $form[$group]['token_help'] = array(
      '#markup' => t('<a href="@token_module_link" target="_blank">Token</a> module is not enabled. Enable it if you want to use the token browser here.', array('@token_module_link' => 'http://drupal.org/project/token')),
    );
  }

  $form['janrain_capture_enforce'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enforce the use of Janrain Capture.'),
    '#default_value' => variable_get('janrain_capture_enforce', 0),
    '#description' => t('Make it impossible to login to this site outside of Janrain Capture. <br/>WARNING: DO NOT enable this setting until you have made at least one Capture user a Drupal Administrator and all of your user accounts have been migrated to Janrain Capture.'),
  );
  $form['#validate'] = array(
    'janrain_capture_ui_settings_validate',
  );
  $form['#submit'] = array(
    'janrain_capture_ui_settings_submit',
  );

  return system_settings_form($form);
}

/**
 * Validate callback for the settings form.
 */
function janrain_capture_ui_settings_validate($form, &$form_state) {
  if ($form_state['values']['janrain_capture_enforce']) {
    // Do not allow the "enforce" setting to be set if the basic application
    // settings have not been set.
    if (empty($form_state['values']['janrain_capture_main']['capture_client_id']) ||
        empty($form_state['values']['janrain_capture_main']['capture_client_secret']) ||
        empty($form_state['values']['janrain_capture_main']['capture_address'])) {
      form_set_error('janrain_capture_enforce', t('You cannot enforce Janrain Capture for login because you have not configured the application settings. You must at least provide a Janrain Capture Client ID, Janrain Capture Client Secret and Janrain Capture Address'));
    }
  }

  if ($form_state['values']['janrain_capture_fields']['capture_match_email'] && !$form_state['values']['janrain_capture_fields']['capture_enforce_verification']) {
    form_set_error('janrain_capture_fields][capture_match_email', t('You must enable the Enforce Email Verification setting in order to enable the Match Users on Email setting.'));
  }

  // Do not allow to enable social share if no engage URL is provided.
  if ($form_state['values']['janrain_capture_share']['enabled'] && empty($form_state['values']['janrain_capture_share']['engage_address'])) {
    form_set_error('janrain_capture_share][engage_address', t('You must provide an Engage URL in order to enable social share.'));
  }
}

/**
 * Submit callback for the settings form.
 */
function janrain_capture_ui_settings_submit($form, &$form_state) {
  // Store the Engage URL in the new Capture config, so that we don't have
  // several copies of the same setting.
  $conf2 = variable_get('janrain_capture_main2', array());
  $conf2['engage_address'] = $form_state['values']['janrain_capture_share']['engage_address'];
  variable_set('janrain_capture_main2', $conf2);
  unset($form_state['values']['janrain_capture_share']['engage_address']);

  // Store the comma-separated list of share providers as an array.
  if(!empty($form_state['values']['janrain_capture_share']['nodes']['providers'])) {
    $providers = $form_state['values']['janrain_capture_share']['nodes']['providers'];
    $providers = str_replace(' ', '', $providers);
    $providers = explode(',', $providers);
  } else {
    $providers = array();
  }
  $form_state['values']['janrain_capture_share']['nodes']['providers'] = $providers;

  // Rebuild the menu in case the "enforce" setting changed.
  variable_set('menu_rebuild_needed', TRUE);
}
