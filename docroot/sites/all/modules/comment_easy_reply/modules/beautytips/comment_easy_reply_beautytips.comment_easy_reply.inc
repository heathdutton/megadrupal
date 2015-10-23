<?php

/**
 * @file
 * Implements hooks for Comment Easy Reply BeautyTips modules.
 */

/**
 * Implements hook_comment_easy_reply_tooltip().
 */
function comment_easy_reply_beautytips_comment_easy_reply_tooltip() {
  $tooltips = array();
  $tooltips['cer_beautytips'] = array(
    'name' => t('Beautytips (via Comment Easy Reply Beautytips submodule)'),
  );
  return $tooltips;
}

/**
 * Implements hook_comment_easy_reply_tooltip_TOOLTIP_number_tips().
 */
function comment_easy_reply_beautytips_comment_easy_reply_tooltip_cer_beautytips_number_tips(&$variables) {
  return theme('comment_easy_reply_comment_number_tips', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_TOOLTIP_number_link().
 */
function comment_easy_reply_beautytips_comment_easy_reply_tooltip_cer_beautytips_number_link(&$variables) {
  $variables['class'] = array('beautytips');
  comment_easy_reply_beautytips_add_tooltip($variables);
  return theme('comment_easy_reply_comment_number_link', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_TOOLTIP_referrer_link().
 */
function comment_easy_reply_beautytips_comment_easy_reply_tooltip_cer_beautytips_referrer_link(&$variables) {
  $variables['class'] = array('beautytips');
  comment_easy_reply_beautytips_add_tooltip($variables);
  return theme('comment_easy_reply_comment_referrer_link', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_TOOLTIP_referrer_tips().
 */
function comment_easy_reply_beautytips_comment_easy_reply_tooltip_cer_beautytips_referrer_tips(&$variables) {
  return theme('comment_easy_reply_comment_referrer_tips', $variables);
}

/**
 * Implements hook_comment_easy_reply_tooltip_TOOLTIP_quote().
 */
function comment_easy_reply_beautytips_comment_easy_reply_tooltip_cer_beautytips_quote(&$variables) {
  $variables['attributes']['class'][] = 'beautytips';
  $variables['more_class'] = array('beautytips');
  comment_easy_reply_beautytips_add_tooltip($variables);
  return theme('comment_easy_reply_quote', $variables);
}
