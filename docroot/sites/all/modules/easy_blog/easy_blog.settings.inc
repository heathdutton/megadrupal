<?php

/**
 * @file
 * Content type settings.
 */

/**
 * Submission form settings (core functional). Preview before submitting.
 *
 * @param string $node_type
 *   content type
 * @param string $value
 *   0 => Disabled
 *   1 => Optional
 *   2 => Required
 */
function _module_enable_set_node_submission_form_settings($node_type, $value) {
  variable_set("node_preview_{$node_type}", $value);
}


/**
 * Publishing options (core functional). Default options.
 *
 * @param string $node_type
 *   content type
 * @param array $value
 *   array('status', 'promote', 'sticky', 'revision')
 */
function _module_enable_set_node_publishing_settings($node_type, $value) {
  variable_set("node_options_{$node_type}", $value);
}


/**
 * Display settings. Display author and date information.
 *
 * @param string $node_type
 *   content type
 * @param string $value
 *   0 => Hide author and date information
 *   1 => Display author and date information
 */
function _module_enable_set_node_display_settings($node_type, $value) {
  variable_set("node_submitted_{$node_type}", $value);
}

/**
 * Comment settings.
 *
 * @param string $node_type
 *   content type
 * @param array $node_info
 *   Default comment setting for new content
 *    comment => comment-status
 *    values:
 *      0 => hidden
 *      1 => closed
 *      2 => open
 *
 *  Threading
 *    comment => default-mode
 *    values:
 *      0 => don't show
 *      1 => show
 *
 *  Anonymous commenting settings
 *    comment => anonymous
 *    values:
 *      0 => Anonymous posters may not enter contact information
 *      1 => Anonymous posters may leave contact information
 *      2 => Anonymous posters must leave contact information
 *
 *  Comments per page
 *    comment => default-per-page
 *    values:
 *      10, 30, 50, 70, 90, 150, 200, 250, 300
 *
 *  Show reply form on the same page as comments
 *    comment => form-location
 *    values:
 *      0 => hide
 *      1 => show
 *
 *  Preview comment
 *    comment => preview
 *    values:
 *      0 => disabled
 *      1 => optional
 *      2 => required
 *
 *  Allow comment title
 *    comment => subject-field
 *    values:
 *      0 => disallow
 *      1 => allow
 */
function _module_enable_set_node_comment_settings($node_type, $node_info = array()) {
  if (module_exists('comment')) {
    if (isset($node_info['comment'])) {
      if (isset($node_info['comment']['status'])) {
        variable_set("comment_{$node_type}", $node_info['comment']['status']);
      }
      if (isset($node_info['comment']['default-mode'])) {
        variable_set("comment_default_mode_{$node_type}", $node_info['comment']['default-mode']);
      }
      if (isset($node_info['comment']['anonymous'])) {
        variable_set("comment_anonymous_{$node_type}", $node_info['comment']['anonymous']);
      }
      if (isset($node_info['comment']['default-per-page'])) {
        variable_set("comment_default_per_page_{$node_type}", $node_info['comment']['default-per-page']);
      }
      if (isset($node_info['comment']['form-location'])) {
        variable_set("comment_form_location_{$node_type}", $node_info['comment']['form-location']);
      }
      if (isset($node_info['comment']['preview'])) {
        variable_set("comment_preview_{$node_type}", $node_info['comment']['preview']);
      }
      if (isset($node_info['comment']['subject-field'])) {
        variable_set("comment_subject_field_{$node_type}", $node_info['comment']['subject-field']);
      }
    }
  }
}

/**
 * Menu settings.
 *
 * @param string $node_type
 *   content type
 * @param array $node_info
 *   Available menus
 *    menu => options
 *    values:
 *      0 => hidden
 *      1 => closed
 *      2 => open
 *
 *   Default parent item
 *    menu => parent
 *    values:
 *      menu-id:item-id
 *      Example: 'main-menu:0'
 */
function _module_enable_set_node_menu_settings($node_type, $node_info = array()) {
  if (module_exists('menu')) {
    if (isset($node_info['menu'])) {
      if (isset($node_info['menu']['options'])) {
        variable_set("menu_options_{$node_type}", $node_info['menu']['options']);
      }
      if (isset($node_info['menu']['parent'])) {
        variable_set("menu_parent_{$node_type}", $node_info['menu']['parent']);
      }
    }
  }
}
