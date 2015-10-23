<?php

/**
 * @file
 * Administative form for the twitter profile widget module.
 */

/**
 * Page callback
 * Return form with twitter profile settings
 */
function twitter_profile_widget_admin() {
  $form = array();

  // Main settings
  $form['main_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Main settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );  
  $form['main_settings']['twitter_profile_widget_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#size' => 25,
    '#default_value' => variable_get('twitter_profile_widget_username', 'Twitter'),
    '#description' => t('Your username on twitter.com'),
  );

   // Widget type
  $form['widget_type'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Widget type'),
    '#collapsible'      => TRUE,
    '#collapsed'        => FALSE,
  );
  $form['widget_type']['twitter_profile_widget_type'] = array(
    '#type'             => 'select',
    '#title'            => t('Please select widget behavior'),
    '#options'          => array(
      'profile'         => t("Load user's profile"),
      'list'            => t("Load user's list"),
      'faves'           => t("Load user's favorite tweets"),
    ),
    '#default_value'    => variable_get('twitter_profile_widget_type', 'profile'),
    '#description'      => t('Select widget behavior'),
  );
  $form['widget_type']['twitter_profile_widget_twitterlist'] = array(
    '#type'             => 'textfield',
    '#title'            => t('List name'),
    '#size'             => 25,
    '#default_value'    => variable_get('twitter_profile_widget_twitterlist', ''),
    '#description'      => t("User's list name from twitter.com"),
    '#prefix'           => '<div class = "twitter-list-setting" style = "display: none;">',
  );
  $form['widget_type']['twitter_profile_widget_twitterlist_title'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Twitter list title'),
    '#size'             => 25,
    '#default_value'    => variable_get('twitter_profile_widget_twitterlist_title', t('Everything we do at')),
    '#description'      => t('Widget description'),
  );
  $form['widget_type']['twitter_profile_widget_twitterlist_subject'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Twitter list subject'),
    '#size'             => 25,
    '#default_value'    => variable_get('twitter_profile_widget_twitterlist_subject', t('the twoffice')),
    '#description'      => t('Widget subject'),
    '#suffix'           => '</div>',
  );
  $form['widget_type']['twitter_profile_widget_faves_title'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Favorite widget title'),
    '#size'             => 25,
    '#default_value'    => variable_get('twitter_profile_widget_faves_title', t('Best twitts according to')),
    '#description'      => t('Widget description'),
    '#prefix'           => '<div class = "twitter-faves-setting" style = "display: none;">',
  );
  $form['widget_type']['twitter_profile_widget_faves_subject'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Favorite widget subject'),
    '#size'             => 25,
    '#default_value'    => variable_get('twitter_profile_widget_faves_subject', t('ME')),
    '#description'      => t('Widget subject'),
    '#suffix'           => '</div>',
  );

  if (variable_get('twitter_profile_widget_type', 'profile') == 'list') {
    $form['widget_type']['twitter_profile_widget_twitterlist']['#prefix'] = '<div class = "twitter-list-setting">';
  }
  elseif (variable_get('twitter_profile_widget_type', 'profile') == 'faves') {
    $form['widget_type']['twitter_profile_widget_faves_title']['#prefix'] = '<div class = "twitter-faves-setting">';
  }

  // Preferences
  $form['preferences'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preferences'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  $form['preferences']['twitter_profile_widget_post_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of Tweets'),
    '#size' => 2,
    '#default_value' => variable_get('twitter_profile_widget_post_amount', '5'),
  );  
  $form['preferences']['twitter_profile_widget_behavior'] = array(
    '#type' => 'radios',
    '#title' => t('Behavior'),
    '#default_value' => variable_get('twitter_profile_widget_behavior', 'load_all_tweets'),
    '#options' => array(
      'timed_interval' => t('Timed interval'),
      'load_all_tweets' => t('Load all tweets')
    ),    
  );  
  $form['preferences']['twitter_profile_widget_loop_results'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loop results'),
    '#default_value' => variable_get('twitter_profile_widget_loop_results', FALSE),
  );  
  $form['preferences']['twitter_profile_widget_tweet_intervals'] = array(
    '#type' => 'textfield',
    '#title' => t('Tweet interval'),
    '#default_value' => variable_get('twitter_profile_widget_tweet_intervals', 5),
    '#size' => 2,
    '#description' => t('Set time interval (in seconds) for tweets updating'),
  );
  $form['preferences']['twitter_profile_widget_poll_for_new_results'] = array(
    '#type' => 'checkbox',
    '#title' => t('Poll for new results'),
    '#default_value' => variable_get('twitter_profile_widget_poll_for_new_results', FALSE),
  );  
  $form['preferences']['twitter_profile_widget_include_scrollbar'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include scrollbar'),
    '#default_value' => variable_get('twitter_profile_widget_include_scrollbar', FALSE),
  );
  $form['preferences']['twitter_profile_widget_show_avatars'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show avatars'),
    '#default_value' => variable_get('twitter_profile_widget_show_avatars', FALSE),
  );  
  $form['preferences']['twitter_profile_widget_show_timestamp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show timestamp'),
    '#default_value' => variable_get('twitter_profile_widget_show_timestamp', TRUE),
  );  
  $form['preferences']['twitter_profile_widget_show_hashtags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show hashtags'),
    '#default_value' => variable_get('twitter_profile_widget_show_hashtags', TRUE),
  );

  //Appearance
  $form['apprearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['apprearance']['twitter_profile_widget_shell_bg_color_type'] = array(
    '#type'           => 'select',
    '#title'          => t('Shell background type'),
    '#options'        => array(
      'color'         => t('Selected color'),
      'transparent'   => t('Transparent')
    ),
    '#default_value'  => variable_get('twitter_profile_widget_shell_bg_color_type', 'color'),
  );
  $form['apprearance']['twitter_profile_widget_shell_bg_color'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Shell background color'),
    '#size'           => 6,
    '#field_prefix'   => '#',
    '#required'       => TRUE,
    '#attributes'     => array('class' => array('colorselect')),
    '#prefix'         => '<div>',
    '#suffix'         => '</div>',
    '#default_value'  => variable_get('twitter_profile_widget_shell_bg_color', 'FFFFFF'),
  );
  $form['apprearance']['twitter_profile_widget_shell_text_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Shell text color'),
    '#size' => 6,
    '#field_prefix' => '#',
    '#required' => TRUE,
    '#attributes' => array('class' => array('colorselect')),
    '#default_value' => variable_get('twitter_profile_widget_shell_text_color', '425367'),
  );
  $form['apprearance']['twitter_profile_widget_tweet_bg_color_type'] = array(
    '#type'           => 'select',
    '#title'          => t('Tweet background type'),
    '#options'        => array(
      'color'         => t('Selected color'),
      'transparent'   => t('Transparent')
    ),
    '#default_value'  => variable_get('twitter_profile_widget_tweet_bg_color_type', 'color'),
  );
  $form['apprearance']['twitter_profile_widget_tweet_bg_color'] = array(
     '#type'           => 'textfield',
    '#title'          => t('Tweet background color'),
    '#size'           => 6,
    '#field_prefix'   => '#',
    '#required'       => TRUE,
    '#attributes'     => array('class' => array('colorselect')),
    '#prefix'         => '<div>',
    '#suffix'         => '</div>',
    '#default_value'  => variable_get('twitter_profile_widget_tweet_bg_color', 'FFFFFF'),
  );  
  $form['apprearance']['twitter_profile_widget_tweet_text_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Tweet text color'),
    '#size' => 6,
    '#field_prefix' => '#',
    '#required' => TRUE,
    '#attributes' => array('class' => array('colorselect')),
    '#default_value' => variable_get('twitter_profile_widget_tweet_text_color', '3E465B'),
  );  
  $form['apprearance']['twitter_profile_widget_tweet_links_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Tweet links color'),
    '#size' => 6,
    '#field_prefix' => '#',
    '#required' => TRUE,
    '#attributes' => array('class' => array('colorselect')),
    '#default_value' => variable_get('twitter_profile_widget_tweet_links_color', '0178b4'),
  );
  if (variable_get('twitter_profile_widget_shell_bg_color_type', 'color') == 'transparent') {
    $form['apprearance']['twitter_profile_widget_shell_bg_color']['#prefix'] = '<div style = "display: none;">';
  }
  if (variable_get('twitter_profile_widget_tweet_bg_color_type', 'color') == 'transparent') {
    $form['apprearance']['twitter_profile_widget_tweet_bg_color']['#prefix'] = '<div style = "display: none;">';
  }

  //Dimensions
  $form['dimensions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dimensions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  $form['dimensions']['twitter_profile_widget_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Widget width'),
    '#size' => 4,
    '#default_value' => variable_get('twitter_profile_widget_width', '180'),
    '#description' => t('Field allows only digital values'),
    '#field_suffix' => 'px',
  );  
  $form['dimensions']['twitter_profile_widget_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Widget height'),
    '#size' => 4,
    '#default_value' => variable_get('twitter_profile_widget_height', '300'),
    '#description' => t('Field allows only digital values'),
    '#field_suffix' => 'px',
  );  
  $form['dimensions']['twitter_profile_widget_autowidth'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto width'),
    '#default_value' => variable_get('twitter_profile_widget_autowidth', FALSE),
  );

  // Follow us button
  $form['follow_us_button'] = array(
    '#type' => 'fieldset',
    '#title' => t('Follow us button'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  $form['follow_us_button']['twitter_profile_widget_follow_us_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Follow Us button'),
    '#default_value' => variable_get('twitter_profile_widget_follow_us_enable', TRUE),
  );  
  $form['follow_us_button']['twitter_profile_widget_follow_us_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Follow Us button text'),
    '#default_value' => variable_get('twitter_profile_widget_follow_us_text', 'Follow us on twitter'),
  );  
  $form['follow_us_button']['twitter_profile_widget_follow_us_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Follow Us button link'),
    '#default_value' => variable_get('twitter_profile_widget_follow_us_link', 'http://twitter.com/twitter'),
    '#description' => t('You can enter link on your twitter page (link might start width "http://")'),
  );

  // Add css and js to form
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'twitter_profile_widget') . '/colorpicker/colorpicker.css',
  );
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'twitter_profile_widget') . '/colorpicker/colorpicker.js',
    drupal_get_path('module', 'twitter_profile_widget') . '/twitter_profile_widget.js',
  );

  // Add validation function
  $form['#validate'][] = 'twitter_profile_widget_admin_validate';
  return system_settings_form($form);
}

/**
 * Validates twitter_profile_widget_admin form
 */
function twitter_profile_widget_admin_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (drupal_strlen($values['twitter_profile_widget_username']) < 2) {
    form_set_error('twitter_profile_widget_username', t('Please, enter correct username'));
  }
  
  if (!ctype_digit($values['twitter_profile_widget_post_amount']) || $values['twitter_profile_widget_post_amount'] < 1 || $values['twitter_profile_widget_post_amount'] > 30) {
    form_set_error('twitter_profile_widget_post_amount', t('Please, enter only digits from 1 to 30 in Post amount field'));
  }

  if (!ctype_digit($values['twitter_profile_widget_tweet_intervals']) || $values['twitter_profile_widget_tweet_intervals'] < 1 || $values['twitter_profile_widget_tweet_intervals'] > 20) {
    form_set_error('twitter_profile_widget_tweet_intervals', t('Please, enter only digits from 1 to 20 in Twitter interval field'));
  }
  
  if (drupal_strlen($values['twitter_profile_widget_shell_bg_color']) != 6) {
    form_set_error('twitter_profile_widget_shell_bg_color', t('Shell background color field must contain 6 characters'));
  }
  
  if (drupal_strlen($values['twitter_profile_widget_shell_text_color']) != 6) {
    form_set_error('twitter_profile_widget_shell_text_color', t('Shell text color field must contain 6 characters'));
  }
  
  if (drupal_strlen($values['twitter_profile_widget_tweet_bg_color']) != 6) {
    form_set_error('twitter_profile_widget_tweet_bg_color', t('Tweet background color field must contain 6 characters'));
  }
  
  if (drupal_strlen($values['twitter_profile_widget_tweet_text_color']) != 6) {
    form_set_error('twitter_profile_widget_tweet_text_color', t('Tweet text color field must contain 6 characters'));
  }
  
  if (drupal_strlen($values['twitter_profile_widget_tweet_links_color']) != 6) {
    form_set_error('twitter_profile_widget_tweet_links_color', t('Tweet links color field must contain 6 characters'));
  }
  
  if (drupal_substr(trim($values['twitter_profile_widget_follow_us_link']), 0, 7) != 'http://') {
    form_set_error('twitter_profile_widget_follow_us_link', t('Follow us link should start with "http://"'));
  }
  
  if (!ctype_digit($values['twitter_profile_widget_width']) || $values['twitter_profile_widget_width'] < 1) {
    form_set_error('twitter_profile_widget_width', t('Please, enter only digits in Twitter widget width field'));
  }
  
  if (!ctype_digit($values['twitter_profile_widget_height']) || $values['twitter_profile_widget_height'] < 1) {
    form_set_error('twitter_profile_widget_height', t('Please, enter only digits in Twitter widget height field'));
  }
}