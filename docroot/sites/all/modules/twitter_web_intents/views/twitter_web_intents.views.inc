<?php

/**
 * @file
 * Provide views data and handlers for twitter_web_intents.module.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data_alter().
 */
function twitter_web_intents_views_data_alter(&$data) {

  $data['twitter']['reply_link'] = array(
    'title' => t('Twitter Web Intents: Reply link'),
    'help' => t('Adds Reply link to the tweet'),
    'field' => array(
      'handler' => 'views_handler_field_twitter_web_intents_reply_link',
    ),
  );
  $data['twitter']['retweet_link'] = array(
    'title' => t('Twitter Web Intents: Retweet link'),
    'help' => t('Adds Retweet link to the tweet'),
    'field' => array(
      'handler' => 'views_handler_field_twitter_web_intents_retweet_link',
    ),
  );
  $data['twitter']['favorite_link'] = array(
    'title' => t('Twitter Web Intents: Favorite link'),
    'help' => t('Adds Favorite link to the tweet'),
    'field' => array(
      'handler' => 'views_handler_field_twitter_web_intents_favorite_link',
    ),
  );
  $data['twitter']['follow_link'] = array(
    'title' => t('Twitter Web Intents: Follow link'),
    'help' => t('Adds Follow link to the author'),
    'field' => array(
      'handler' => 'views_handler_field_twitter_web_intents_follow_link',
    ),
    'area' => array(
      'handler' => 'views_handler_area_twitter_web_intents_follow_link',
    ),
  );

}

/**
 * Implements hook_views_handlers().
 */
function twitter_web_intents_views_handlers() {

  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'twitter_web_intents'),
    ),
    'handlers' => array(
      'views_handler_field_twitter_web_intents_link' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_field_twitter_web_intents_reply_link' => array(
        'parent' => 'views_handler_field_twitter_web_intents_link',
      ),
      'views_handler_field_twitter_web_intents_retweet_link' => array(
        'parent' => 'views_handler_field_twitter_web_intents_link',
      ),
      'views_handler_field_twitter_web_intents_favorite_link' => array(
        'parent' => 'views_handler_field_twitter_web_intents_link',
      ),
      'views_handler_field_twitter_web_intents_follow_link' => array(
        'parent' => 'views_handler_field_twitter_web_intents_link',
      ),
      'views_handler_area_twitter_web_intents_follow_link' => array(
        'parent' => 'views_handler',
      ),
    ),
  );

}
