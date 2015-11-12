<?php
/**
 * @file
 * Filters and text formats for Social Content: Twitter.
 */

/**
 * Implements hook_filter_default_formats().
 */
function social_content_twitter_filter_default_formats() {
  $formats = array();

  // Exported format: Tweet.
  $formats['tweet'] = array(
    'format' => 'tweet',
    'name' => 'Tweet',
    'cache' => 1,
    'status' => 1,
    'weight' => -8,
    'filters' => array(
      'filter_url' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
      'social_content_twitter_hashtag' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(),
      ),
      'social_content_twitter_links' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(),
      ),
      'social_content_twitter_username' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}

/**
 * Implements hook_filter_info().
 */
function social_content_twitter_filter_info() {
  $filters['social_content_twitter_username'] = array(
    'title' => t('Twitter @username converter'),
    'description' => t('Converts Twitter-style @usernames into links to Twitter account pages.'),
    'process callback' => 'social_content_twitter_filterusername',
    'tips callback' => 'social_content_twitter_filtertip_username',
  );
  $filters['social_content_twitter_hashtag'] = array(
    'title' => t('Twitter #hashtag converter'),
    'description' => t('Converts Twitter-style #hashtags into links to twitter.com.'),
    'process callback' => 'social_content_twitter_filterhashtag',
    'tips callback' => 'social_content_twitter_filtertip_hashtag',
  );
  $filters['social_content_twitter_links'] = array(
    'title' => t('Twitter link converter'),
    'description' => t("Makes links to be opened in new windows and adds rel='nofollow'."),
    'process callback' => 'social_content_twitter_filterlink',
    'tips callback' => 'social_content_twitter_filtertip_link',
  );

  return $filters;
}

/**
 * Filter tips callback function for Twitter usernames.
 */
function social_content_twitter_filtertip_username($filter, $format, $long = FALSE) {
  return t('Twitter-style @usernames are linked to their Twitter account pages.');
}

/**
 * Filter tips callback function for Twitter hashtags.
 */
function social_content_twitter_filtertip_hashtag($format, $long = FALSE) {
  return t('Twitter-style #hashtags are linked to !url.', array(
    '!url' => '<a href="http://search.twitter.com/">search.twitter.com</a>')
  );
}

/**
 * Filter tips callback function for Twitter links.
 */
function social_content_twitter_filtertip_link($filter, $format, $long = FALSE) {
  return t('Twitter message links are opened in new windows and rel="nofollow" is added.');
}

/**
 * Callback for twitter @username converter.
 */
function social_content_twitter_filterusername($text, $filter) {
  $prefix = '@';
  $destination = SOCIAL_CONTENT_TWITTER_HOST . '/';
  return social_content_twitter_filtertext($text, $prefix, $destination);
}

/**
 * Callback for twitter #hashtag converter.
 */
function social_content_twitter_filterhashtag($text, $filter) {
  $prefix = '#';
  $destination = 'http://twitter.com/search?q=%23';
  return social_content_twitter_filtertext($text, $prefix, $destination);
}

/**
 * Converts Twitter-style @usernames and #hashtags into actual links.
 */
function social_content_twitter_filtertext($text, $prefix, $destination) {
  $matches = array(
    '/\>' . $prefix . '(\w+)/ui',
    '/^' . $prefix . '(\w+)/ui',
    '/(\s+)' . $prefix . '(\w+)/ui',
  );
  $replacements = array(
    '><a href="' . $destination . '${1}">' . $prefix . '${1}</a>',
    '<a href="' . $destination . '${1}">' . $prefix . '${1}</a>',
    '${1}<a href="' . $destination . '${2}">' . $prefix . '${2}</a>',
  );
  return preg_replace($matches, $replacements, $text);
}

/**
 * Callback for twitter link converter.
 */
function social_content_twitter_filterlink($text, $filter) {
  return str_replace("<a ", '<a target="_blank" rel="nofollow" ', $text);
}
