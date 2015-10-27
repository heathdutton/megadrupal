<?php

/**
 * @file
 * Implements hooks for Comment Easy Reply qtip modules.
 */

/**
 * Implements hook_comment_easy_reply_tooltip().
 */
function comment_easy_reply_qtip_comment_easy_reply_tooltip() {
  $tooltips = array();
  $tooltips['cer_qtip'] = array(
    'name' => t('qTip (via Comment Easy Reply qTip submodule)'),
  );
  return $tooltips;
}

/**
 * Implements hook_comment_easy_reply_tooltip_TOOLTIP_number_tips().
 */
function comment_easy_reply_qtip_comment_easy_reply_tooltip_cer_qtip_number_tips(&$variables) {
  return theme('comment_easy_reply_comment_number_tips', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_TOOLTIP_number_link().
 */
function comment_easy_reply_qtip_comment_easy_reply_tooltip_cer_qtip_number_link(&$variables) {
  comment_easy_reply_qtip_add_tooltip();
  return theme('comment_easy_reply_comment_number_link', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_TOOLTIP_referrer_link().
 */
function comment_easy_reply_qtip_comment_easy_reply_tooltip_cer_qtip_referrer_link(&$variables) {
  comment_easy_reply_qtip_add_tooltip();
  $variables['referrer_link'] = theme('comment_easy_reply_comment_referrer_link', $variables);
  return $variables['referrer_link'];
}

/**
 * Implements hook_comment_easy_reply_tooltip_TOOLTIP_referrer_tips().
 */
function comment_easy_reply_qtip_comment_easy_reply_tooltip_cer_qtip_referrer_tips(&$variables) {
  return theme('comment_easy_reply_comment_referrer_tips', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_TOOLTIP_quote().
 */
function comment_easy_reply_qtip_comment_easy_reply_tooltip_cer_qtip_quote(&$variables) {
  comment_easy_reply_qtip_add_tooltip();
  $output = theme('comment_easy_reply_quote', $variables);
  return $output;
}
