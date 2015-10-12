<?php
/**
 * @file
 * CTools Export UI plugin for Secureshare settings profiles.
 */

$plugin = array(
  'schema' => 'secureshare_profiles',
  'access' => 'administer secureshare',
  'menu' => array(
    'menu prefix' => 'admin/config/content',
    'menu item' => 'secureshare',
    'menu title' => 'SecureShare profiles',
    'menu description' => 'Administer Secureshare settings profiles.',
  ),
  'title singular' => t('profile'),
  'title singular proper' => t('Profile'),
  'title plural' => t('profiles'),
  'title plural proper' => t('Profiles'),
  'form' => array(
    'settings' => 'secureshare_profiles_export_ui_form',
    'submit' => 'secureshare_profiles_export_ui_form_submit',
  ),
);

function secureshare_profiles_export_ui_form(&$form, &$form_state) {
  $config = $form_state['item']->config;

  $form['config'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $form['config']['css_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to css file'),
    '#description' => t('The path to the css file. If no file is provided it will use the default css from heise.'),
    '#default_value' => isset($config['css_path']) ? $config['css_path'] : '',
  );

  $form['config']['info_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer info link'),
    '#description' => t('Link to detailed disclaimer information'),
    '#default_value' => isset($config['info_link']) ? $config['info_link'] : '',
  );

  $form['config']['txt_help'] = array(
    '#type' => 'textfield',
    '#title' => t('MouseOver information'),
    '#description' => t('MouseOver text for the informations icon.'),
    '#default_value' => isset($config['txt_help']) ? $config['txt_help'] : '',
  );

  $form['config']['settings_perma'] = array(
    '#type' => 'textfield',
    '#title' => t('Perma settings heading'),
    '#description' => t('the heading text for the perma settings dialog'),
    '#default_value' => isset($config['settings_perma']) ? $config['settings_perma'] : '',
  );

  $form['config']['cookie_expires'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie expire'),
    '#description' => t('Cookie expire in days'),
    '#default_value' => isset($config['cookie_expires']) ? $config['cookie_expires'] : '',
  );

  $form['config']['services']['facebook'] = array(
    '#type' => 'fieldset',
    '#title' => t('Facebook'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['config']['services']['facebook']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable'),
    '#description' => t('disable facebook button'),
    // heise plugin dont use true/false for this... instead using on/off
    // on is the default behavior... so we disable it here...
    '#return_value' => 'off',
    '#default_value' => isset($config['services']['facebook']['status']) ? $config['services']['facebook']['status'] : '',
  );

  $form['config']['services']['facebook']['dummy_img'] = array(
    '#type' => 'textfield',
    '#title' => t('Imagepath'),
    '#description' => t('path to the displayed image'),
    '#default_value' => isset($config['services']['facebook']['dummy_img']) ? $config['services']['facebook']['dummy_img'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][facebook][status]"]' => array('checked' => TRUE),
      ),
    ),

  );

  $form['config']['services']['facebook']['txt_info'] = array(
    '#type' => 'textfield',
    '#title' => t('MouseOver button text'),
    '#description' => t('MouseOver button text'),
    '#default_value' => isset($config['services']['facebook']['txt_info']) ? $config['services']['facebook']['txt_info'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][facebook][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['facebook']['txt_fb_off'] = array(
    '#type' => 'textfield',
    '#title' => t('Image text off'),
    '#description' => t('alternative text for the image button on off mode'),
    '#default_value' => isset($config['services']['facebook']['txt_fb_off']) ? $config['services']['facebook']['txt_fb_off'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][facebook][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['facebook']['txt_fb_on'] = array(
    '#type' => 'textfield',
    '#title' => t('Image text on'),
    '#description' => t('alternative text for the image button in on mode'),
    '#default_value' => isset($config['services']['facebook']['txt_fb_on']) ? $config['services']['facebook']['txt_fb_on'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][facebook][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['facebook']['perma_option'] = array(
    '#type' => 'checkbox',
    '#title' => t('disable perma option'),
    '#description' => t('disable perma option'),
    // heise plugin dont use true/false for this... instead using on/off
    // on is the default behavior... so we disable it here...
    '#return_value' => 'off',
    '#default_value' => isset($config['services']['facebook']['perma_option']) ? $config['services']['facebook']['perma_option'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][facebook][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['facebook']['display_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Display name'),
    '#description' => t('Name of the displayed social network.'),
    '#default_value' => isset($config['services']['facebook']['display_name']) ? $config['services']['facebook']['display_name'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][facebook][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // @TODO: need to be implemented
  //$form['config']['services']['facebook']['referrer_track'] = array();

  $form['config']['services']['facebook']['language'] = array(
    '#type' => 'select',
    '#title' => t('Language detection'),
    '#description' => t('Language detection mode'),
    '#options' => array(
      'user' => t('Users current language'),
      'system' => t('Default language'),
      'static' => t('static language value'),
    ),
    '#default_value' => isset($config['services']['facebook']['language']) ? $config['services']['facebook']['language'] : 'system',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][facebook][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['facebook']['language_static'] = array(
    '#type' => 'textfield',
    '#title' => t('Language code'),
    '#description' => t('Use this code instead of autodetect. On facebook you need to use format like: en_US or de_DE'),
    '#default_value' => isset($config['services']['facebook']['language_static']) ? $config['services']['facebook']['language_static'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="config[services][facebook][language]"]' => array('value' => 'static'),
      ),
    ),

  );

  $form['config']['services']['facebook']['action'] = array(
    '#type' => 'select',
    '#title' => t('Button style'),
    '#options' => array(
      'recommend' => t('recommend'),
      'like' => t('like'),
    ),
    '#default_value' => isset($config['services']['facebook']['action']) ? $config['services']['facebook']['action'] : 'like',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][facebook][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['config']['services']['twitter']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable'),
    '#description' => t('disable twitter button'),
    // heise plugin dont use true/false for this... instead using on/off
    // on is the default behavior... so we disable it here...
    '#return_value' => 'off',
    '#default_value' => isset($config['services']['twitter']['status']) ? $config['services']['twitter']['status'] : '',
  );

  $form['config']['services']['twitter']['dummy_img'] = array(
    '#type' => 'textfield',
    '#title' => t('Imagepath'),
    '#description' => t('path to the displayed image'),
    '#default_value' => isset($config['services']['twitter']['dummy_img']) ? $config['services']['twitter']['dummy_img'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][twitter][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['twitter']['txt_info'] = array(
    '#type' => 'textfield',
    '#title' => t('MouseOver button text'),
    '#description' => t('MouseOver button text'),
    '#default_value' => isset($config['services']['twitter']['txt_info']) ? $config['services']['twitter']['txt_info'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][twitter][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['twitter']['txt_twitter_off'] = array(
    '#type' => 'textfield',
    '#title' => t('Image text off'),
    '#description' => t('alternative text for the image button on off mode'),
    '#default_value' => isset($config['services']['twitter']['txt_twitter_off']) ? $config['services']['twitter']['txt_twitter_off'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][twitter][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['twitter']['txt_twitter_on'] = array(
    '#type' => 'textfield',
    '#title' => t('Image text on'),
    '#description' => t('alternative text for the image button in on mode'),
    '#default_value' => isset($config['services']['twitter']['txt_twitter_on']) ? $config['services']['twitter']['txt_twitter_on'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][twitter][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['twitter']['perma_option'] = array(
    '#type' => 'checkbox',
    '#title' => t('disable perma option'),
    '#description' => t('disable permanent activation?'),
    // heise plugin dont use true/false for this... instead using on/off
    // on is the default behavior... so we disable it here...
    '#return_value' => 'off',
    '#default_value' => isset($config['services']['twitter']['perma_option']) ? $config['services']['twitter']['perma_option'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][twitter][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['twitter']['display_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Display name'),
    '#description' => t('Name of the displayed social network.'),
    '#default_value' => isset($config['services']['twitter']['display_name']) ? $config['services']['twitter']['display_name'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][twitter][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // @TODO: need to be implemented
  //$form['config']['services']['twitter']['referrer_track'] = array();

  $form['config']['services']['twitter']['tweet_text'] = array(
    '#type' => 'select',
    '#title' => t('Tweet text'),
    '#description' => t('tweet text detection mode'),
    '#options' => array(
      'default' => t('Use script default'),
    ),
    '#default_value' => isset($config['services']['twitter']['tweet_text']) ? $config['services']['twitter']['tweet_text'] : 'default',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][twitter][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['twitter']['language'] = array(
    '#type' => 'select',
    '#title' => t('Language detection'),
    '#description' => t('Language detection mode'),
    '#options' => array(
      'user' => t('User current language'),
      'system' => t('System default language'),
    ),
    '#default_value' => isset($config['services']['twitter']['language']) ? $config['services']['twitter']['language'] : 'system',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][twitter][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['gplus'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google+'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['config']['services']['gplus']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('disable'),
    '#description' => t('disable google+ button'),
    // heise plugin dont use true/false for this... instead using on/off
    // on is the default behavior... so we disable it here...
    '#return_value' => 'off',
    '#default_value' => isset($config['services']['gplus']['status']) ? $config['services']['gplus']['status'] : '',
  );

  $form['config']['services']['gplus']['dummy_img'] = array(
    '#type' => 'textfield',
    '#title' => t('Imagepath'),
    '#description' => t('path to the displayed image'),
    '#default_value' => isset($config['services']['gplus']['dummy_img']) ? $config['services']['gplus']['dummy_img'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][gplus][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['gplus']['txt_info'] = array(
    '#type' => 'textfield',
    '#title' => t('MouseOver button text'),
    '#description' => t('MouseOver button text'),
    '#default_value' => isset($config['services']['gplus']['txt_info']) ? $config['services']['gplus']['txt_info'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][gplus][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['gplus']['txt_gplus_off'] = array(
    '#type' => 'textfield',
    '#title' => t('Image text off'),
    '#description' => t('alternative text for the image button on off mode'),
    '#default_value' => isset($config['services']['gplus']['txt_gplus_off']) ? $config['services']['gplus']['txt_gplus_off'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][gplus][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['gplus']['txt_gplus_on'] = array(
    '#type' => 'textfield',
    '#title' => t('Image text on'),
    '#description' => t('alternative text for the image button in on mode'),
    '#default_value' => isset($config['services']['gplus']['txt_gplus_on']) ? $config['services']['gplus']['txt_gplus_on'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][gplus][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['gplus']['perma_option'] = array(
    '#type' => 'checkbox',
    '#title' => t('disable perma option'),
    '#description' => t('disable permanent activation?'),
    // heise plugin dont use true/false for this... instead using on/off
    // on is the default behavior... so we disable it here...
    '#return_value' => 'off',
    '#default_value' => isset($config['services']['gplus']['perma_option']) ? $config['services']['gplus']['perma_option'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][gplus][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['config']['services']['gplus']['display_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Display name'),
    '#description' => t('Name of the displayed social network.'),
    '#default_value' => isset($config['services']['gplus']['display_name']) ? $config['services']['gplus']['display_name'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][gplus][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  /*
  $form['config']['services']['gplus']['referrer_track'] = array(
    '#type' => 'select',
    '#title' => t('Referrer track code'),
    '#description' => t('will be attached at the end of the url for better tracking'),
    '#default_value' => isset($config['services']['gplus']['referrer_track']) ? $config['services']['gplus']['referrer_track'] : '',
    '#options' => array(
      '' => t('none'),
      'static' => t('use fixed value'),
    ),
  );

  $form['config']['services']['gplus']['referrer_track_static'] = array(
    '#type' => 'textfield',
    '#title' => t('Referrer track code'),
    '#description' => t('will be attached at the end of the url for better tracking'),
    '#default_value' => isset($config['services']['gplus']['referrer_track']) ? $config['services']['gplus']['referrer_track'] : '',
    '#options' => array(
      '' => t('none'),
      'static' => t('use fixed value'),
    ),
  );
  */

  $form['config']['services']['gplus']['language'] = array(
    '#type' => 'select',
    '#title' => t('Language detection'),
    '#description' => t('Language detection mode'),
    '#options' => array(
      'user' => t('User language'),
      'system' => t('Default language'),
    ),
    '#default_value' => isset($config['services']['gplus']['language']) ? $config['services']['gplus']['language'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="config[services][gplus][status]"]' => array('checked' => TRUE),
      ),
    ),
  );
}

function secureshare_profiles_export_ui_form_submit(&$form, &$form_state) {}