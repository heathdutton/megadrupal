<?php
/**
 * @file
 * Context hooks file.
 */

/**
 * Implements hook_context_plugins().
 */
function social_network_user_detection_context_plugins() {
  // CTools plugin API hook for Context. Note that a proper entry in.
  // Social_network_user_detectionctools_plugin_api() must exist for this hook.
  // To be called.
  $plugins = array();
  $plugins['social_network_user_detection_social_action_fb'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'social_network_user_detection') . '/plugins',
      'file' => 'social_network_user_detection.context.inc',
      'class' => 'SocialNetworkUserDetectionSocialActionFb',
      'parent' => 'context_condition',
    ),
  );
  $plugins['social_network_user_detection_social_action_twitter'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'social_network_user_detection') . '/plugins',
      'file' => 'social_network_user_detection.context.inc',
      'class' => 'SocialNetworkUserDetectionSocialActionTwitter',
      'parent' => 'context_condition',
    ),
  );
  $plugins['social_network_user_detection_social_action_gmail'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'social_network_user_detection') . '/plugins',
      'file' => 'social_network_user_detection.context.inc',
      'class' => 'SocialNetworkUserDetectionSocialActionGmail',
      'parent' => 'context_condition',
    ),
  );
  $plugins['social_network_user_detection_social_action_gplus'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'social_network_user_detection') . '/plugins',
      'file' => 'social_network_user_detection.context.inc',
      'class' => 'SocialNetworkUserDetectionSocialActionGplus',
      'parent' => 'context_condition',
    ),
  );
  $plugins['social_network_user_detection_social_action_hyves'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'social_network_user_detection') . '/plugins',
      'file' => 'social_network_user_detection.context.inc',
      'class' => 'SocialNetworkUserDetectionSocialActionHyves',
      'parent' => 'context_condition',
    ),
  );
  return $plugins;
}

/**
 * Implements hook_context_registry().
 *
 * Each entry associates a condition or reaction with the CTools plugin to be
 * used as its plugin class.
 */
function social_network_user_detection_context_registry() {
  return array(
    'conditions' => array(
      'social_network_user_detection_context_action_fb' => array(
        'title' => t('Social network user detection - FB'),
        'plugin' => 'social_network_user_detection_social_action_fb',
      ),
      'social_network_user_detection_context_action_twitter' => array(
        'title' => t('Social network user detection - Twitter'),
        'plugin' => 'social_network_user_detection_social_action_twitter',
      ),
      'social_network_user_detection_context_action_gmail' => array(
        'title' => t('Social network user detection - Gmail'),
        'plugin' => 'social_network_user_detection_social_action_gmail',
      ),
      'social_network_user_detection_context_action_gplus' => array(
        'title' => t('Social network user detection - GooglePlus'),
        'plugin' => 'social_network_user_detection_social_action_gplus',
      ),
      'social_network_user_detection_context_action_hyves' => array(
        'title' => t('Social network user detection - Hyves'),
        'plugin' => 'social_network_user_detection_social_action_hyves',
      ),
    ),
  );
}

/**
 * Function called on hook_init().
 */
function _social_network_user_detection_context_plugins_load() {
  $plugin_fb = context_get_plugin('condition', 'social_network_user_detection_context_action_fb');
  if ($plugin_fb) {
    $plugin_fb->execute();
  }
  $plugin_twitter = context_get_plugin('condition', 'social_network_user_detection_context_action_twitter');
  if ($plugin_twitter) {
    $plugin_twitter->execute();
  }
  $plugin_gmail = context_get_plugin('condition', 'social_network_user_detection_context_action_gmail');
  if ($plugin_gmail) {
    $plugin_gmail->execute();
  }
  $plugin_hyves = context_get_plugin('condition', 'social_network_user_detection_context_action_hyves');
  if ($plugin_hyves) {
    $plugin_hyves->execute();
  }
  $plugin_gplus = context_get_plugin('condition', 'social_network_user_detection_context_action_gplus');
  if ($plugin_gplus) {
    $plugin_gplus->execute();
  }
}
