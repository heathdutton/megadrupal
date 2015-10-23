<?php

/**
 * @file
 * Social Feed module admin inc file.
 *
 * Contains admin configuration page callback functions.
 */

/**
 * Form builder; Configure social feeds settings for this site.
 *
 * @ingroup forms
 *
 * @see system_settings_form()
 */
function socialfeed_facebook_settings($form, &$form_state) {
  $form['socialfeed_facebook_page_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Page Name'),
    '#default_value' => variable_get('socialfeed_facebook_page_name'),
    '#description' => t('eg. If your Facebook page URL is this http://www.facebook.com/YOUR_PAGE_NAME, <br />then you just need to add this YOUR_PAGE_NAME above.'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_facebook_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook App ID'),
    '#default_value' => variable_get('socialfeed_facebook_app_id'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_facebook_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Secret Key'),
    '#default_value' => variable_get('socialfeed_facebook_secret_key'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_facebook_no_feeds'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of Feeds'),
    '#default_value' => variable_get('socialfeed_facebook_no_feeds', 10),
    '#size' => 60,
    '#maxlength' => 60,
  );
  $form['socialfeed_facebook_all_types'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show all post types'),
    '#default_value' => variable_get('socialfeed_facebook_all_types', TRUE),
  );
  $form['socialfeed_facebook_post_type'] = array(
    '#type' => 'select',
    '#title' => 'Select your post type(s) to show',
    '#default_value' => variable_get('socialfeed_facebook_post_type', 0),
    '#options' => array('None', 'link', 'status', 'photo', 'video'),
    // '#multiple' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="socialfeed_facebook_all_types"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['socialfeed_facebook_display_pic'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Post Picture'),
    '#default_value' => variable_get('socialfeed_facebook_display_pic', FALSE),
    '#states' => array(
      'visible' => array(
        ':input[name="socialfeed_facebook_all_types"]' => array('checked' => FALSE),
        ':input[name="socialfeed_facebook_post_type"]' => array('value' => 3),
      ),
    ),
  );
  $form['socialfeed_facebook_trim_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Trim Length'),
    '#default_value' => variable_get('socialfeed_facebook_trim_length', 120),
    '#size' => 60,
    '#maxlength' => 60,
  );
  $form['socialfeed_facebook_time_stamp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Date/Time'),
    '#default_value' => variable_get('socialfeed_facebook_time_stamp', FALSE),
  );
  $form['socialfeed_facebook_time_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Date/Time format'),
    '#default_value' => variable_get('socialfeed_facebook_time_format', 'd-M-Y'),
    '#description' => t('You can check for PHP Date Formats <a href="@datetime" target="@blank">here</a>', array('@datetime' => 'http://php.net/manual/en/datetime.formats.date.php#datetime.formats.date', '@blank' => '_blank')),
    '#size' => 60,
    '#maxlength' => 100,
    '#states' => array(
      'visible' => array(
        ':input[name="socialfeed_facebook_time_stamp"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}

/**
 * Form builder; Configure social feeds settings for this site.
 *
 * @ingroup forms
 *
 * @see system_settings_form()
 */
function socialfeed_twitter_settings($form, &$form_state) {
  $form['socialfeed_twitter_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Access Token'),
    '#default_value' => variable_get('socialfeed_twitter_access_token'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_twitter_access_token_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Access Token Secret'),
    '#default_value' => variable_get('socialfeed_twitter_access_token_secret'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_twitter_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Consumer Key'),
    '#default_value' => variable_get('socialfeed_twitter_consumer_key'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_twitter_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Consumer Secret'),
    '#default_value' => variable_get('socialfeed_twitter_consumer_secret'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter User Name'),
    '#default_value' => variable_get('socialfeed_twitter_username'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_twitter_tweets_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Tweetes Count'),
    '#default_value' => variable_get('socialfeed_twitter_tweets_count', 3),
    '#size' => 60,
    '#maxlength' => 100,
  );
  $form['socialfeed_twitter_time_stamp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Date/Time'),
    '#default_value' => variable_get('socialfeed_twitter_time_stamp', FALSE),
  );
  $form['socialfeed_twitter_time_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Date/Time format'),
    '#default_value' => variable_get('socialfeed_twitter_time_format', 'd-M-Y'),
    '#description' => t('You can check for PHP Date Formats <a href="@datetime" target="@blank">here</a>', array('@datetime' => 'http://php.net/manual/en/datetime.formats.date.php#datetime.formats.date', '@blank' => '_blank')),
    '#size' => 60,
    '#maxlength' => 100,
    '#states' => array(
      'visible' => array(
        ':input[name="socialfeed_twitter_time_stamp"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}

/**
 * Form builder; Configure social feeds settings for this site.
 *
 * @ingroup forms
 *
 * @see system_settings_form()
 */
function socialfeed_instagram_settings($form, &$form_state) {
  $form['socialfeed_instagram_user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('User ID'),
    '#description' => t('Fetch the Instagram User ID from <a href="@jelled" target="@blank">here</a>.', array('@jelled' => 'http://jelled.com/instagram/lookup-user-id', '@blank' => '_blank')),
    '#default_value' => variable_get('socialfeed_instagram_user_id'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_instagram_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('socialfeed_instagram_api_key'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_instagram_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('API Secret'),
    '#default_value' => variable_get('socialfeed_instagram_api_secret'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_instagram_redirect_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URI'),
    '#default_value' => variable_get('socialfeed_instagram_redirect_uri'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialfeed_instagram_picture_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Picture Count'),
    '#default_value' => variable_get('socialfeed_instagram_picture_count', 1),
    '#size' => 60,
    '#maxlength' => 100,
  );
  $form['socialfeed_instagram_picture_resolution'] = array(
    '#type' => 'select',
    '#title' => t('Picture Resolution'),
    '#default_value' => variable_get('socialfeed_instagram_picture_resolution', 'thumbnail'),
    '#options' => array(
      'thumbnail' => t('Thumbnail'),
      'low_resolution' => t('Low Resolution'),
      'standard_resolution' => t('Standard Resolution'),
    ),
  );

  return system_settings_form($form);
}
