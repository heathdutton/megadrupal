<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup comment_easy_reply_hooks Comment Easy Reply's hooks
 * @{
 * Hooks that can be implemented by other modules in order to extend Comment
 * Easy Reply module functionalities.
 */

/**
 * Create new tags to replace @#NUM in comments.
 *
 * This hook adds one or more tags to the list of available tags of Comment
 * Easy Reply module. Each new tag will be selectable through the module's
 * settings form.
 *
 * @return array
 *   An associative array of tags containing one sub-array for each tag.
 *   Possible attributes for each sub-array are:
 *   - label: The label of the tag. It will be showed on settings form.
 *     Required.
 *   - callback: A function to be called to get the tag to replace @#NUM with.
 *     Required.
 *   - title callback: A function to be called to get the tag to replace @#NUM
 *     with, specific for replacing inside comment subject. Required.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tags() {
  $tags = array();
  $tags['myfirsttag'] = array(
    'callback' => '_my_module_first_tag',
    'title callback' => '_my_module_first_tag_title',
    'label' => t('Display link as myfirsttag (Eg: @myfirsttag)'),
  );
  $tags['mysecondtag'] = array(
    'callback' => '_my_module_second_tag',
    'title callback' => '_my_module_second_tag_title',
    'label' => t('Display link as my#2tag (Eg: #my2ndtag)'),
  );
  return $tags;
}

/**
 * Alter tags added using hook_comment_easy_reply_tags().
 *
 * @param array &$tags
 *   The array of tags to be altered.
 *
 * @return array
 *   The array of tags, altered.
 *
 * @see hook_comment_easy_reply_tags()
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tags_alter(&$tags) {
  if (isset($tags['module_to_be_altered'])) {
    $tags['module_to_be_altered']['label'] = t('My new custom label');
  }
  return $tags;
}

/**
 * Alter quote bbcode added on quote ajax call to comment form.
 *
 * The altered quote bb code will be transformed in a blockquote tag by Quote
 * module's theming functions.
 *
 * @param array &$quote
 *   The quote bbcode to be altered
 *
 * @return array
 *   The context array, containing the following keys:
 *     - nid: the node's id
 *     - cid: the comment's id
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_quote_input_alter(&$quote, $context) {
  $quote = 'My text: ' . $quote;
}

/**
 * Let modules specify their own tooltips to be used by Comment Easy Reply.
 *
 * Usually a module should add one tooltip, but it is possible to add more than
 * one tooltip mode if needed.
 * The specified tooltip will be available as option on Comment Easy Reply
 * settings page.
 *
 * @return array
 *   The array of tooltips.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tooltip() {
  $tooltips = array();
  $tooltips['mymodule_tips'] = array(
    'name' => t('A very short description of my custom tooltip'),
  );
  return $tooltips;
}

/**
 * Alter the tooltips defined in hook_comment_easy_reply_tooltip().
 *
 * @param array &$tooltips
 *   The defined tooltips array, containing the following keys:
 *     - the tooltip name, linking its own array containing:
 *       - name: the tooltip's description (will appear on Comment Easy Reply
 *         settings page).
 *       - module: the module that defined the tooltip.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tooltip_alter(&$tooltips) {
  if (isset($tooltips['mymodule_tips'])) {
    // Override the tooltip module to let my custom functions called instead
    // of hooks of the module that originally defined the tooltip.
    $tooltips['mymodule_tips'] = array(
      'module' => 'my_custom_module',
    );
  }
}

/**
 * Let a module specify the output for its tooltip to be used on permalink.
 *
 * Let a module specify the output and the scripts for its own tooltip to be
 * used on comment permalink.
 * The hook_comment_easy_reply_tooltip_TOOLTIP_number_link() calls the
 * currently selected tooltip module's hook, replacing the word TOOLTIP with
 * the tooltip's name.
 *
 * @param array &$variables
 *   The variables array usually passed as param to
 *   theme_comment_easy_reply_comment_number_link() function.
 *
 * @return string
 *   The permalink html.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tooltip_mymodule_tips_number_link(&$variables) {
  // Disable Comment Easy Reply native tooltips.
  _comment_easy_reply_disable_native_js();
  // If neded, custom javascript tooltips could be defined here.
  return theme('comment_easy_reply_comment_number_link', $variables);
}

/**
 * Let a module specify the default output for its permalink tooltip.
 *
 * Let a module specify the output and the scripts for its own tooltip to be
 * used on comment permalink if no tooltip specific hook is defined.
 * If no hook_comment_easy_reply_tooltip_TOOLTIP_number_link() is defined,
 * the hook_comment_easy_reply_tooltip_number_link() will be called.
 * Only the hook of the module that specified the current selected tooltip
 * will be called.
 *
 * @param array &$variables
 *   The variables array usually passed as param to
 *   theme_comment_easy_reply_comment_number_link() function.
 *
 * @return string
 *   The permalink html.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tooltip_number_link(&$variables) {
  // Disable Comment Easy Reply native tooltips.
  _comment_easy_reply_disable_native_js();
  // If neded, custom javascript tooltips could be defined here.
  return theme('comment_easy_reply_comment_number_link', $variables);
}

/**
 * Let a module specify the output for its own comment permalink's tooltip.
 *
 * Let a module specify the output and the scripts for its own tooltip to be
 * used on comment permalink's tooltip.
 * The hook_comment_easy_reply_tooltip_TOOLTIP_number_tips() calls the
 * currently selected tooltip module's hook, replacing the word TOOLTIP with
 * the tooltip's name.
 *
 * @param array &$variables
 *   The variables array usually passed as param to
 *   theme_comment_easy_reply_comment_number_tips() function.
 *
 * @return string
 *   The permalink's tooltip html.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tooltip_mymodule_tips_number_tips(&$variables) {
  // If neded, custom javascript tooltips could be defined here.
  return theme('comment_easy_reply_comment_number_tips', $variables);
}

/**
 * Let a module specify the output its own default comment permalink's tooltip.
 *
 * Let a module specify the output and the scripts for its own tooltip to be
 * used on comment permalink's tooltip if no tooltip specific hook was defined.
 * If no hook_comment_easy_reply_tooltip_TOOLTIP_number_tips() is defined,
 * the hook_comment_easy_reply_tooltip_number_tips() will be called.
 * Only the hook of the module that specified the current selected tooltip
 * will be called.
 *
 * @param array &$variables
 *   The variables array usually passed as param to
 *   theme_comment_easy_reply_comment_number_tips() function.
 *
 * @return string
 *   The permalink's tooltip html.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tooltip_number_tips(&$variables) {
  // If neded, custom javascript tooltips could be defined here.
  return theme('comment_easy_reply_comment_number_tips', $variables);
}

/**
 * Let a module specify the output for its own comment referrer links tooltip.
 *
 * Let a module specify the output and the scripts for its own tooltip to be
 * used on comment referrer links.
 * The hook_comment_easy_reply_tooltip_TOOLTIP_referrer_link() calls the
 * currently selected tooltip module's hook, replacing the word TOOLTIP with
 * the tooltip's name.
 *
 * @param array &$variables
 *   The variables array usually passed as param to
 *   theme_comment_easy_reply_comment_referrer_link() function.
 *
 * @return string
 *   The comment referrer link html.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_easy_reply_beautytips_comment_easy_reply_tooltip_mymodule_tips_referrer_link(&$variables) {
  // Disable Comment Easy Reply native tooltips.
  _comment_easy_reply_disable_native_js();
  // If neded, custom javascript tooltips could be defined here.
  return = theme('comment_easy_reply_comment_referrer_link', $variables);
}

/**
 * Let a module specify the output for its own default comment referrer link's.
 *
 * Let a module specify the output and the scripts for its own tooltip to be
 * used on comment referrer link's tooltip if no tooltip specific hook was
 * defined.
 * If no hook_comment_easy_reply_tooltip_TOOLTIP_referrer_link() is defined,
 * the hook_comment_easy_reply_tooltip_referrer_link() will be called.
 * Only the hook of the module that specified the current selected tooltip
 * will be called.
 *
 * @param array &$variables
 *   The variables array usually passed as param to
 *   theme_comment_easy_reply_comment_referrer_link() function.
 *
 * @return string
 *   The comment referrer link html.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_easy_reply_beautytips_comment_easy_reply_tooltip_referrer_link(&$variables) {
  // Disable Comment Easy Reply native tooltips.
  _comment_easy_reply_disable_native_js();
  // If neded, custom javascript tooltips could be defined here.
  return = theme('comment_easy_reply_comment_referrer_link', $variables);
}

/**
 * Let a module specify the output for its own comment referrer links' tooltip.
 *
 * Let a module specify the output and the scripts for its own tooltip to be
 * used on comment referrer links' tooltip.
 * The hook_comment_easy_reply_tooltip_TOOLTIP_referrer_tips() calls the
 * currently selected tooltip module's hook, replacing the word TOOLTIP with
 * the tooltip's name.
 *
 * @param array &$variables
 *   The variables array usually passed as param to
 *   theme_comment_easy_reply_comment_referrer_tips() function.
 *
 * @return string
 *   The comment referrer links' tooltip html.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tooltip_mymodule_tips_referrer_tips(&$variables) {
  // If neded, custom javascript tooltips could be defined here.
  return theme('comment_easy_reply_comment_referrer_tips', $variables);
}

/**
 * Let a module specify the output for its own default referrer links tooltip.
 *
 * Let a module specify the output and the scripts for its own tooltip to be
 * used on comment referrer links' tooltip if no tooltip specific hook was
 * defined.
 * If no hook_comment_easy_reply_tooltip_TOOLTIP_referrer_tips() is defined,
 * the hook_comment_easy_reply_tooltip_referrer_tips() will be called.
 * Only the hook of the module that specified the current selected tooltip
 * will be called.
 *
 * @param array &$variables
 *   The variables array usually passed as param to
 *   theme_comment_easy_reply_comment_referrer_tips() function.
 *
 * @return string
 *   The comment referrer links' tooltip html.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tooltip_referrer_tips(&$variables) {
  // If neded, custom javascript tooltips could be defined here.
  return theme('comment_easy_reply_comment_referrer_tips', $variables);
}

/**
 * Let a module specify the output for its own Quote blockquotes tooltips.
 *
 * Let a module specify the output and the scripts for its own tooltip to be
 * used on comment referrer links' tooltip for Quote blockquotes.
 * The hook_comment_easy_reply_tooltip_TOOLTIP_quote() calls the
 * currently selected tooltip module's hook, replacing the word TOOLTIP with
 * the tooltip's name.
 * Only the hook of the module that specified the current selected tooltip
 * will be called.
 * This hook is called only the first time a comment is rendered. Then the
 * comment is cached by Drupal and it's not generated again util next cache
 * clearing.
 *
 * @param array &$variables
 *   The variables array usually passed as param to
 *   theme_comment_easy_reply_quote() function.
 *
 * @return string
 *   The comment referrer links' tooltip for Quote blockquotes html.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_tooltip_mymodule_tips_quote(&$variables) {
  // Disable Comment Easy Reply native tooltips.
  _comment_easy_reply_disable_native_js();
  // If neded, custom javascript tooltips could be defined here.
  return theme('comment_easy_reply_quote', $variables);
}

/**
 * Allows other modules to add their own settings.
 *
 * Allows other modules to add their own settings inside the Comment Easy Reply
 * settings management.
 * Every added setting will be available as general settings and node type
 * overridden value.
 * Every added setting must be specified as a key of a multidimensional array,
 * linking to a callback function giving the value.
 * A callback example can be found on callbacks group below.
 *
 * @return array
 *   An associative array containing the settings names as keys, linking to
 *   sub-arrays with the following keys:
 *     - 'callback': links to the callback function name.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_settings() {
  $settings = array(
    'my_module_setting_1' => array('callback' => '_my_module_setting_1_callback'),
    'my_module_setting_2' => array('callback' => '_my_module_setting_2_callback'),
  );
  return $settings;
}

/**
 * Allows other modules to alter Comment Easy Reply settings.
 *
 * @param array $settings
 *   An associative array containing the settings names as keys, linking to
 *   sub-arrays with the following keys:
 *     - 'callback': links to the callback function name.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_settings_alter(&$settings) {
  $settings['my_module_setting_1']['callback'] = '_my_module_setting_3_callback';
}

/**
 * Allows other modules to alter a comment body or subject.
 *
 * When Comment Easy Reply module replace tags in a comment, the text to be
 * replaced pass through this hook.
 *
 * @param string &$text
 *   The text containing the tags to be replaced.
 * @param int $comment_nid
 *   The nid of the node comment belongs to.
 * @param bool $include_parent
 *   If TRUE, the parent comment body will be displayed inside the tooltip.
 * @param array $options
 *   An associative array of options:
 *   - tag: the tag used to replace a #@NUM tag occurrence.
 *   - tag_link: if FALSE, the tag used to replace a #@NUM tag occurrence
 *     will not be added as a link but as a simple text.
 * @param string $original_body
 *   The original text before having tags replaced.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_text_replace_alter(&$text, $comment_nid, $include_parent, $options, $original_body) {
  $text = str_replace('mytag', 'the_new_output', $text);
}

/**
 * Allows other modules to alter the tags found into comment body or subject.
 *
 * The tags found inside a comment body can be altered using this hook.
 *
 * @param string &$text
 *   The text containing the tags to be found.
 * @param array &$result
 *   An array containing the found tags.
 *
 * @ingroup comment_easy_reply_hooks
 * @ingroup hooks
 */
function hook_comment_easy_reply_text_matches_alter(&$text, &$result) {
  if (isset($result['mytag'])) {
    unset($result['mytag']);
  }
}
/**
 * @}
 */

/**
 * @defgroup comment_easy_reply_callbacks Comment Easy Reply's callbacks
 * @{
 * Examples of callbacks used in Comment Easy Reply module hooks.
 */

/**
 * Example callback used in hook_comment_easy_reply_tags().
 *
 * This callback creates a string used to replace a @#NUM tag.
 * The @#NUM tag is contained in a comment body and it refers to the comment
 * passed as an argument to the callback.
 * This example callback returns a string listing all the comment parents' ids.
 *
 * @param object &$comment
 *   The comment referred by the tag.
 *
 * @return string
 *   The string the @#NUM will be replaced with.
 *
 * @see hook_comment_easy_reply_tags()
 *
 * @ingroup comment_easy_reply_callbacks
 */
function _my_module_first_tag(&$comment) {
  $tag = '#parents:';
  $parents = _comment_easy_reply_comment_get_parents($comment);
  if (!empty($parents)) {
    $tag .= implode(',', array_keys($parents));
  }
  return $tag;
}

/**
 * Example callback used in hook_comment_easy_reply_tags().
 *
 * This callback creates a string used to replace a @#NUM tag.
 * The @#NUM tag is contained in a comment subject and it refers to the comment
 * passed as an argument to the callback.
 *
 * @param object &$comment
 *   The comment referred by the tag.
 *
 * @return string
 *   The string the @#NUM will be replaced with.
 *
 * @see hook_comment_easy_reply_tags()
 *
 * @ingroup comment_easy_reply_callbacks
 */
function _my_module_first_tag_title(&$comment) {
  return '#mytag:' . $comment->name;
}

/**
 * Example callback used in hook_comment_easy_reply_settings().
 *
 * Returns the specified settings value, checking if it is overridden or not.
 *
 * @param string $name
 *   The name of the setting.
 * @param string $node_type
 *   The node type.
 *
 * @return mixed
 *   The setting value.
 *
 * @see hook_comment_easy_reply_settings()
 *
 * @ingroup comment_easy_reply_callbacks
 */
function _my_module_setting_1_callback($name, $node_type) {
  switch ($name) {
    case 'my_module_setting_1':
      if (_comment_easy_reply_is_node_type_override_active($node_type)) {
        return variable_get('my_module_setting_1' . $node_type, MY_MODULE_SETTING_1_DEFAULT_VALUE);
      }
      return variable_get('my_module_setting_1', MY_MODULE_SETTING_1_DEFAULT_VALUE);

  }
  return FALSE;
}

/**
 * @}
 */
