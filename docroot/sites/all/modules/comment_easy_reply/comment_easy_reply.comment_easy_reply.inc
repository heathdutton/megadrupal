<?php

/**
 * @file
 * Implements hooks for Comment Easy Reply modules.
 */

/**
 * Implements hook_comment_easy_reply_tooltip().
 */
function comment_easy_reply_comment_easy_reply_tooltip() {
  $tooltips = array();
  $tooltips[COMMENT_EASY_REPLY_DEFAULT_TOOLTIP_MODULE] = array(
    'name' => 'Comment Easy Reply',
  );
  return $tooltips;
}

/**
 * Implements hook_comment_easy_reply_tags().
 */
function comment_easy_reply_comment_easy_reply_tags() {
  $tags = array();
  $tags['number'] = array(
    'callback' => '_comment_easy_reply_tag_number',
    'title callback' => '_comment_easy_reply_tag_number_title',
    'label' => t('Display link as comment number (Eg: #45)'),
  );
  $tags['author'] = array(
    'callback' => '_comment_easy_reply_tag_username',
    'title callback' => '_comment_easy_reply_tag_username_title',
    'label' => t('Display link as username (Eg: @username)'),
  );
  return $tags;
}

/**
 * Implements hook_comment_easy_reply_tooltip_number_tips().
 */
function comment_easy_reply_comment_easy_reply_tooltip_number_tips(&$variables) {
  return theme('comment_easy_reply_comment_number_tips', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_number_link().
 */
function comment_easy_reply_comment_easy_reply_tooltip_number_link(&$variables) {
  comment_easy_reply_add_native_tooltip_js();
  return theme('comment_easy_reply_comment_number_link', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_referrer_link().
 */
function comment_easy_reply_comment_easy_reply_tooltip_referrer_link(&$variables) {
  comment_easy_reply_add_native_tooltip_js();
  return theme('comment_easy_reply_comment_referrer_link', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_referrer_tips().
 */
function comment_easy_reply_comment_easy_reply_tooltip_referrer_tips(&$variables) {
  return theme('comment_easy_reply_comment_referrer_tips', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_quote().
 */
function comment_easy_reply_comment_easy_reply_tooltip_quote(&$variables) {
  return theme('comment_easy_reply_quote', $variables);
}

/**
 * Implements hook_comment_easy_reply_settings().
 */
function comment_easy_reply_comment_easy_reply_settings() {
  $settings = array();
  $default = array(
    'callback' => '_comment_easy_reply_settings_value_callback',
    'report callback' => '_comment_easy_reply_status_report_callback',
  );
  $names = array(
    'comment_easy_reply_override' => array(),
    'comment_easy_reply_reply_from_numlink' => array(
      'report label' => t('Allow users to reply to a comment by clicking on comment #NUM link:'),
    ),
    'comment_easy_reply_replytip_activated' => array(
      'report label' => t('Reply tooltip on comment number permalink:'),
    ),
    'comment_easy_reply_fragment_activated' => array(
      'report label' => t('Custom url fragment:'),
    ),
    'comment_easy_reply_link_mode' => array(
      'report label' => t('Referrer link type:'),
    ),
    'comment_easy_reply_filter' => array(
      'report label' => t('Tooltip filter:'),
    ),
    'comment_easy_reply_replytip_text' => array(
      'report label' => t('Text inside comment reply tooltip:'),
    ),
    'comment_easy_reply_referrertip_text' => array(
      'report label' => t('Text inside comment referrer tooltip:'),
    ),
    'comment_easy_reply_fragment_text' => array(
      'report label' => t('Url fragment custom text:'),
    ),
    'comment_easy_reply_comment_title' => array(
      'report label' => t('Predefined comment reply title:'),
    ),
    'comment_easy_reply_tooltip_name' => array(
      'report label' => t('Tooltip style:'),
    ),
  );
  if (module_exists('quote')) {
    $names['comment_easy_reply_quote_tooltip'] = array(
      'report label' => t('Quote reply inside a tooltip:'),
    );
  }
  foreach ($names as $name => $value) {
    if (is_array($value)) {
      $settings[$name] = array_merge($default, $value);
    }
    else {
      $settings[$value] = $default;
    }
  }
  return $settings;
}
