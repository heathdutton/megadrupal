<?php
// $Id: pagestyle.block.inc,v 1.3 2009/12/10 23:15:23 christianzwahlen Exp $

/**
 * @file
 * Block, page and theme functions.
 */

/**
 * Returns an array of allowed values.
 *
 * @return
 *   An array of allowed values.
 */
function pagestyle_allowed_values() {
  $pagestyle_allowed = array(
    'black_white',
    'white_black',
    'yellow_blue',
    'standard',
  );
  return $pagestyle_allowed;
}

/**
 * Cookie expires.
 *
 * @return
 *   The cookie expires in seconds.
 */
function pagestyle_cookie_expires($mode = 'php') {
  $pagestyle_cookie_expires = variable_get('pagestyle_cookie_expires', 365) ;
  $output = ($pagestyle_cookie_expires * 24 * 60 * 60);
  if ($mode == 'js') {
    $output = $pagestyle_cookie_expires;
  }
  return $output;
}

/**
 * Menu callback; Set the pagestyle to Black/White, then redirects to the previous page.
 */
function pagestyle_black_white() {
  $pagestyle_cookie_expires = pagestyle_cookie_expires($mode = 'php');
  $pagestyle_cookie_domain = variable_get('pagestyle_cookie_domain', base_path());
  $ps_name = t('Black') . '/' . t('White');
  $pagestyle_message = variable_get('pagestyle_message', 1);
  if (!isset($_COOKIE['pagestyle']) && $pagestyle_message == 1) {
    drupal_set_message(t('The page style have not been saved, because your browser do not accept cookies.'), 'error');
  }
  else {
    $_SESSION['pagestyle'] = 'black_white';
    setcookie("pagestyle", ("black_white"), time()+$pagestyle_cookie_expires, $pagestyle_cookie_domain, "");
    if ($pagestyle_message == 1) {
      drupal_set_message(t('The page style have been saved as %ps_name.', array('%ps_name' => $ps_name)));
    }
  }
  pagestyle_clear_cache();
  drupal_goto();
}

/**
 * Menu callback; Set the pagestyle to White/Black, then redirects to the previous page.
 */
function pagestyle_white_black() {
  $pagestyle_cookie_expires = pagestyle_cookie_expires($mode = 'php');
  $pagestyle_cookie_domain = variable_get('pagestyle_cookie_domain', base_path());
  $ps_name = t('White') . '/' . t('Black');
  $pagestyle_message = variable_get('pagestyle_message', 1);
  if (!isset($_COOKIE['pagestyle']) && $pagestyle_message == 1) {
    drupal_set_message(t('The page style have not been saved, because your browser do not accept cookies.'), 'error');
  }
  else {
    $_SESSION['pagestyle'] = 'white_black';
    setcookie("pagestyle", ("white_black"), time()+$pagestyle_cookie_expires, $pagestyle_cookie_domain, "");
    if ($pagestyle_message == 1) {
      drupal_set_message(t('The page style have been saved as %ps_name.', array('%ps_name' => $ps_name)));
    }
  }
  pagestyle_clear_cache();
  drupal_goto();
}

/**
 * Menu callback; Set the pagestyle to Yellow/Blue, then redirects to the previous page.
 */
function pagestyle_yellow_blue() {
  $pagestyle_cookie_expires = pagestyle_cookie_expires($mode = 'php');
  $pagestyle_cookie_domain = variable_get('pagestyle_cookie_domain', base_path());
  $ps_name = t('Yellow') . '/' . t('Blue');
  $pagestyle_message = variable_get('pagestyle_message', 1);
  if (!isset($_COOKIE['pagestyle']) && $pagestyle_message == 1) {
    drupal_set_message(t('The page style have not been saved, because your browser do not accept cookies.'), 'error');
  }
  else {
    $_SESSION['pagestyle'] = 'yellow_blue';
    setcookie("pagestyle", ("yellow_blue"), time()+$pagestyle_cookie_expires, $pagestyle_cookie_domain, "");
    if ($pagestyle_message == 1) {
      drupal_set_message(t('The page style have been saved as %ps_name.', array('%ps_name' => $ps_name)));
    }
  }
  pagestyle_clear_cache();
  drupal_goto();
}

/**
 * Menu callback; change the pagestyle to Standard, then redirects to the previous page.
 */
function pagestyle_standard() {
  $pagestyle_cookie_expires = pagestyle_cookie_expires($mode = 'php');
  $pagestyle_cookie_domain = variable_get('pagestyle_cookie_domain', base_path());
  $ps_name = t('Standard');
  $pagestyle_message = variable_get('pagestyle_message', 1);
  if (!isset($_COOKIE['pagestyle']) && $pagestyle_message == 1) {
    drupal_set_message(t('The page style have not been saved, because your browser do not accept cookies.'), 'error');
  }
  else {
    $_SESSION['pagestyle'] = 'standard';
    setcookie("pagestyle", ("standard"), time()+$pagestyle_cookie_expires, $pagestyle_cookie_domain, "");
    if ($pagestyle_message == 1) {
      drupal_set_message(t('The page style have been saved as %ps_name.', array('%ps_name' => $ps_name)));
    }
  }
  pagestyle_clear_cache();
  drupal_goto();
}

/**
 * Menu callback; change the pagestyle to the cookie value, then redirects to the previous page.
 */
function pagestyle_set() {
  $pagestyle_cookie_expires = pagestyle_cookie_expires($mode = 'php');
  $pagestyle_cookie_domain = variable_get('pagestyle_cookie_domain', base_path());
  $pagestyle_normal = variable_get('pagestyle_normal', 'standard');
  $pagestyle_text = array(
    'black_white' => t('Black') . '/' . t('White'),
    'white_black' => t('White') . '/' . t('Black'),
    'yellow_blue' => t('Yellow') . '/' . t('Blue'),
    'standard' => t('Standard'),
  );
  $pagestyle_message = variable_get('pagestyle_message', 1);
  $pagestyle_post = array();
  $pagestyle_cookie = array();
  // allowed values
  $pagestyle_allowed = pagestyle_allowed_values();
  // check the type and the content
  if (in_array($_COOKIE['pagestyle'], $pagestyle_allowed)) {
    $pagestyle_cookie['pagestyle'] = (string) filter_xss($_COOKIE['pagestyle']);
  }
  // set session/cookie
  if (!isset($_COOKIE['pagestyle']) && $pagestyle_message == 1) {
    drupal_set_message(t('The page style have not been saved, because your browser do not accept cookies.'), 'error');
  }
  elseif (isset($_COOKIE['pagestyle'])) {
    $_SESSION['pagestyle'] = $pagestyle_cookie['pagestyle'];
    setcookie("pagestyle", $pagestyle_cookie['pagestyle'], time()+$pagestyle_cookie_expires, $pagestyle_cookie_domain, "");
    if ($pagestyle_message == 1) {
      drupal_set_message(t('The page style have been saved as %pagestyle.', array('%pagestyle' => $pagestyle_text[$pagestyle_cookie['pagestyle']])));
    }
  }
  pagestyle_clear_cache();
  drupal_goto();
}

/**
 * Clear defined cache tables
 */
function pagestyle_clear_cache() {
  $tables = array(
    'cache_page',
    'cache_block',
  );
  foreach ($tables as $table) {
    cache_clear_all('*', $table, TRUE);
  }
}

/**
 * Block Title
 *
 * @return
 *   "Page Style" and "Current Style" or "Theme" and "Current Theme".
 *
 * @see pagestyle_admin_settings()
 */
function pagestyle_block_title($title = 'title') {
  $pagestyle_block_title = variable_get('pagestyle_block_title', 'page_style');
  $pagestyle_block_title_text = t('Page Style');
  $pagestyle_current_pagestyle_text = t('Current Style');
  switch ($pagestyle_block_title) {
    case 'theme':
    $pagestyle_block_title_text = t('Theme');
    $pagestyle_current_pagestyle_text = t('Current Theme');
    break;
  }
  if ($title == 'title') {
    return $pagestyle_block_title_text;
  }
  elseif ($title == 'current') {
    return $pagestyle_current_pagestyle_text;
  }
}

/**
 * Returns an array of available values for the select element in the block page style.
 *
 * @return
 *   An array of available values, for selecting the page style.
 *
 * @see pagestyle_form()
 */
function pagestyle_block_form_options() {
  $pagestyle_black_white = variable_get('pagestyle_black_white', 1);
  $pagestyle_white_black = variable_get('pagestyle_white_black', 1);
  $pagestyle_yellow_blue = variable_get('pagestyle_yellow_blue', 1);
  $pagestyle_standard = variable_get('pagestyle_standard', 1);
  $ps_options = array(
    'black_white' => array(
      'class' => 'black_white',
      'name' => t('Black/White'),
      'display' => $pagestyle_black_white,
      ),
    'white_black' => array(
      'class' => 'white_black',
      'name' => t('White/Black'),
      'display' => $pagestyle_white_black,
      ),
    'yellow_blue' => array(
      'class' => 'yellow_blue',
      'name' => t('Yellow/Blue'),
      'display' => $pagestyle_yellow_blue,
      ),
    'standard' => array(
      'class' => 'standard',
      'name' => t('Standard'),
      'display' => $pagestyle_standard,
      ),
    );
  $options = array();
  foreach ($ps_options as $name) {
    if ($name['display'] == 1) {
    $options[$name['class']] = $name['name'];
    }
  }
  return $options;
}

/**
 * Implement hook_form().
 *
 * Generate the select form for the block.
 *
 * @see pagestyle_block()
 */
function pagestyle_form() {
  $pagestyle_cookie_expires = pagestyle_cookie_expires($mode = 'php');
  $pagestyle_cookie_domain = variable_get('pagestyle_cookie_domain', base_path());
  $pagestyle_block_title = pagestyle_block_title($title = 'title');
  $pagestyle_normal = variable_get('pagestyle_normal', 'standard');
  $pagestyle = pagestyle_get_current($value = 'int');
  $options = pagestyle_block_form_options();
  $pagestyle_text = array(
    'black_white' => t('Black') . '/' . t('White'),
    'white_black' => t('White') . '/' . t('Black'),
    'yellow_blue' => t('Yellow') . '/' . t('Blue'),
    'standard' => t('Standard'),
  );
  $pagestyle_message = variable_get('pagestyle_message', 1);
  $pagestyle_post = array();
  $pagestyle_cookie = array();
  // allowed values
  $pagestyle_allowed = pagestyle_allowed_values();
  // check the type and the content
  if (isset($_COOKIE['pagestyle']) && isset($_POST['pagestyle_select'])) {
    if (in_array($_POST['pagestyle_select'], $pagestyle_allowed)) {
      $pagestyle_post['pagestyle'] = (string) filter_xss($_POST['pagestyle_select']);
    }
    // set session/cookie
    setcookie("pagestyle", $pagestyle_post['pagestyle'], time()+$pagestyle_cookie_expires, $pagestyle_cookie_domain, "");
    $_SESSION['pagestyle'] = $pagestyle_post['pagestyle'];
    if ($pagestyle_message == 1) {
      drupal_set_message(t('The page style have been saved as %pagestyle_post.', array('%pagestyle_post' => $pagestyle_text[$pagestyle_post['pagestyle']])));
    }
    pagestyle_clear_cache();
  }
  elseif (!isset($_COOKIE['pagestyle']) && isset($_POST['pagestyle_select']) && $pagestyle_message == 1) {
    drupal_set_message(t('The page style have not been saved, because your browser do not accept cookies.'), 'error');
  }
  $subtitle = NULL;
  if (variable_get('pagestyle_display_subtitle', 0) == 1) {
    $subtitle = $pagestyle_block_title;
  }
  $form = array();
  $form['pagestyle_select'] = array(
    '#type' => 'select',
    '#title' => $subtitle,
    '#name' => 'pagestyle_select',
    '#options' => $options,
    '#default_value' => variable_get('pagestyle_select', $pagestyle),
    '#weight' => 0,
    // '#description' => t('Select the page style.'),
  );
  $form['#attributes']['class'][] = 'pagestyle';
  $form['pagestyle_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 1,
  );
  $form['#skip_duplicate_check'] = TRUE;
  return $form;
}

/**
 * Implement theme().
 *
 * @see pagestyle_form()
 */
function theme_pagestyle_form($form) {
  $output = drupal_render($form);
  return $output;
}

/**
 * Implement hook_theme().
 */
function pagestyle_theme() {
  $variables = array(
    'dest' => NULL,
    'block_title' => NULL,
    'list_inline' => NULL,
    'current_inline' => NULL,
    'subtitle' => NULL,
    'subtitle_text' => NULL,
    'display_links' => NULL,
    'black_white' => NULL,
    'white_black' => NULL,
    'yellow_blue' => NULL,
    'standard' => NULL,
    'current_pagestyle' => NULL,
    'display_current_pagestyle' => NULL,
    'display_current_pagestyle_text' => NULL,
    'current_pagestyle_text' => NULL,
    'pagestyle' => NULL,
  );
  return array(
    'pagestyle_text' => array(
      'variables' => $variables,
      'template' => 'pagestyle-text',
    ),
    'pagestyle_image' => array(
      'variables' => $variables,
      'template' => 'pagestyle-image',
    ),
    'pagestyle_form' => array(
      'render element' => 'form',
      'template' => 'pagestyle-form',
    ),
  );
}

/**
 * Process variables for pagestyle-text.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $dest: The drupal function drupal_get_destination().
 * - $block_title: Block title: "Page Style" or "Theme".
 * - $list_inline: Display the ul list "inline" or not.
 * - $current_inline: Display the current page style "inline" or not.
 * - $subtitle: The subtitle.
 * - $subtitle_text: "Page Style"/"Theme" or inline "Page Style: "/"Theme: ".
 * - $display_links: Show or hide ("display_hidden") the text in the links.
 * - $black_white: The pagestyle Black/White "black_white".
 * - $white_black: The pagestyle White/Black "white_black".
 * - $yellow_blue: The pagestyle Yellow/Blue "yellow_blue".
 * - $standard: The pagestyle Standard "standard".
 * - $current_pagestyle: The current page style.
 * - $display_current_pagestyle: Show or hide ("display_hidden") the text of the current page style.
 * - $display_current_pagestyle_text: Show or hide ("display_hidden");
 * - $current_pagestyle_text: The current pagestyle text "Current Style" or "Current Theme".
 * - $pagestyle: The current page style (default Standard).
 *
 * @see pagestyle_theme(), pagestyle-text.tpl.php
 */
function template_preprocess_pagestyle_text(&$variables) {
  $pagestyle_block_title = pagestyle_block_title($title = 'title');
  $pagestyle_current_pagestyle_text = pagestyle_block_title($title = 'current');
  $pagestyle_block_type = variable_get('pagestyle_block_type', 'text');
  $pagestyle_black_white = variable_get('pagestyle_black_white', 1);
  $pagestyle_white_black = variable_get('pagestyle_white_black', 1);
  $pagestyle_yellow_blue = variable_get('pagestyle_yellow_blue', 1);
  $pagestyle_standard = variable_get('pagestyle_standard', 1);
  if (isset($variables['block'])) {
    $variables['template_files'][] = 'pagestyle-text';
  }
  $variables['block_' . $pagestyle_block_type] = 'block_' . $pagestyle_block_type;
  $variables['dest'] = drupal_get_destination();
  $variables['block_title'] = $pagestyle_block_title;
  $variables['list_inline'] = "list";
  $subtitle_add = NULL;
  if (variable_get('pagestyle_display_list_inline', 0) == 1) {
    $variables['list_inline'] = "inline";
    $subtitle_add = ": ";
  }
  $variables['current_inline'] = "pagestyle_current_list";
  if (variable_get('pagestyle_display_current_inline', 0) == 1) {
    $variables['current_inline'] = "pagestyle_current_inline";
  }
  $variables['subtitle'] = NULL;
  $variables['subtitle_text'] = NULL;
  if (variable_get('pagestyle_display_subtitle', 0) == 1) {
    $variables['subtitle'] = "subtitle";
    $variables['subtitle_text'] = $pagestyle_block_title . $subtitle_add;
  }
  $variables['display_links'] = "display";
  if (variable_get('pagestyle_display_links', 1) == 0) {
    $variables['display_links'] = "display_hidden";
  }
  $variables['black_white'] = NULL;
  if ($pagestyle_black_white == 1) {
    $variables['black_white'] = "black_white";
  }
  $variables['white_black'] = NULL;
  if ($pagestyle_white_black == 1) {
    $variables['white_black'] = "white_black";
  }
  $variables['yellow_blue'] = NULL;
  if ($pagestyle_yellow_blue == 1) {
    $variables['yellow_blue'] = "yellow_blue";
  }
  $variables['standard'] = NULL;
  if ($pagestyle_standard == 1) {
    $variables['standard'] = "standard";
  }
  $variables['current_pagestyle'] = NULL;
  $variables['display_current_pagestyle'] = "display";
  $variables['display_current_pagestyle_text'] = "display";
  switch (variable_get('pagestyle_display_current_text_value', 'text_value')) {
    case "text_value":
      $variables['current_pagestyle'] = "current_pagestyle";
      $variables['display_current_pagestyle'] = "display";
      $variables['display_current_pagestyle_text'] = "display";
    break;
    case "value":
      $variables['current_pagestyle'] = "current_pagestyle";
      $variables['display_current_pagestyle'] = "display";
      $variables['display_current_pagestyle_text'] = "display_hidden";
    break;
    case "hidden":
      $variables['current_pagestyle'] = "current_pagestyle";
      $variables['display_current_pagestyle'] = "display_hidden";
      $variables['display_current_pagestyle_text'] = "display_hidden";
    break;
    case "remove":
      $variables['display_current_pagestyle'] = "display_hidden";
      $variables['display_current_pagestyle_text'] = "display_hidden";
    break;
  }
  $variables['current_pagestyle_text'] = $pagestyle_current_pagestyle_text;
  $variables['pagestyle'] = pagestyle_get_current($value = 'name');
}

/**
 * Process variables for pagestyle-image.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $dest: The drupal function drupal_get_destination().
 * - $block_title: Block title: "Page Style" or "Theme".
 * - $list_inline: Display the ul list "inline" or not.
 * - $current_inline: Display the current page style "inline" or not.
 * - $subtitle: The subtitle.
 * - $subtitle_text: "Page Style"/"Theme" or inline "Page Style: "/"Theme: ".
 * - $display_links: Show or hide ("display_hidden") the text in the links.
 * - $black_white: The pagestyle Black/White "black_white".
 * - $white_black: The pagestyle White/Black "white_black".
 * - $yellow_blue: The pagestyle Yellow/Blue "yellow_blue".
 * - $standard: The pagestyle Standard "standard".
 * - $current_pagestyle: The current page style.
 * - $display_current_pagestyle: Show or hide ("display_hidden") the text of the current page style.
 * - $display_current_pagestyle_text: Show or hide ("display_hidden");
 * - $current_pagestyle_text: The current pagestyle text "Current Style" or "Current Theme".
 * - $pagestyle: The current page style (default Standard).
 *
 * @see pagestyle_theme(), pagestyle-image.tpl.php
 */
function template_preprocess_pagestyle_image(&$variables) {
  $pagestyle_block_title = pagestyle_block_title($title = 'title');
  $pagestyle_current_pagestyle_text = pagestyle_block_title($title = 'current');
  $pagestyle_block_type = variable_get('pagestyle_block_type', 'text');
  $pagestyle_black_white = variable_get('pagestyle_black_white', 1);
  $pagestyle_white_black = variable_get('pagestyle_white_black', 1);
  $pagestyle_yellow_blue = variable_get('pagestyle_yellow_blue', 1);
  $pagestyle_standard = variable_get('pagestyle_standard', 1);
  if (isset($variables['block'])) {
    $variables['template_files'][] = 'pagestyle-image';
  }
  $variables['block_' . $pagestyle_block_type] = 'block_' . $pagestyle_block_type;
  $variables['dest'] = drupal_get_destination();
  $variables['block_title'] = $pagestyle_block_title;
  $variables['list_inline'] = "list";
  $subtitle_add = NULL;
  if (variable_get('pagestyle_display_list_inline', 0) == 1) {
    $variables['list_inline'] = "inline";
    $subtitle_add = ": ";
  }
  $variables['current_inline'] = "pagestyle_current_list";
  if (variable_get('pagestyle_display_current_inline', 0) == 1) {
    $variables['current_inline'] = "pagestyle_current_inline";
  }
  $variables['subtitle'] = NULL;
  $variables['subtitle_text'] = NULL;
  if (variable_get('pagestyle_display_subtitle', 0) == 1) {
    $variables['subtitle'] = "subtitle";
    $variables['subtitle_text'] = $pagestyle_block_title . $subtitle_add;
  }
  $variables['display_links'] = "display";
  if (variable_get('pagestyle_display_links', 1) == 0) {
    $variables['display_links'] = "display_hidden";
  }
  $variables['black_white'] = NULL;
  if ($pagestyle_black_white == 1) {
    $variables['black_white'] = "black_white";
  }
  $variables['white_black'] = NULL;
  if ($pagestyle_white_black == 1) {
    $variables['white_black'] = "white_black";
  }
  $variables['yellow_blue'] = NULL;
  if ($pagestyle_yellow_blue == 1) {
    $variables['yellow_blue'] = "yellow_blue";
  }
  $variables['standard'] = NULL;
  if ($pagestyle_standard == 1) {
    $variables['standard'] = "standard";
  }
  $variables['current_pagestyle'] = NULL;
  $variables['display_current_pagestyle'] = "display";
  $variables['display_current_pagestyle_text'] = "display";
  switch (variable_get('pagestyle_display_current_text_value', 'text_value')) {
    case "text_value":
      $variables['current_pagestyle'] = "current_pagestyle";
      $variables['display_current_pagestyle'] = "display";
      $variables['display_current_pagestyle_text'] = "display";
    break;
    case "value":
      $variables['current_pagestyle'] = "current_pagestyle";
      $variables['display_current_pagestyle'] = "display";
      $variables['display_current_pagestyle_text'] = "display_hidden";
    break;
    case "hidden":
      $variables['current_pagestyle'] = "current_pagestyle";
      $variables['display_current_pagestyle'] = "display_hidden";
      $variables['display_current_pagestyle_text'] = "display_hidden";
    break;
    case "remove":
      $variables['display_current_pagestyle'] = "display_hidden";
      $variables['display_current_pagestyle_text'] = "display_hidden";
    break;
  }
  $variables['current_pagestyle_text'] = $pagestyle_current_pagestyle_text;
  $variables['pagestyle'] = pagestyle_get_current($value = 'name');
}

/**
 * Process variables for pagestyle-form.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $list_inline: Display the ul list "inline" or not.
 * - $current_inline: Display the current page style "inline" or not.
 * - $current_pagestyle: The current page style.
 * - $display_current_pagestyle: Show or hide ("display_hidden") the text of the current page style.
 * - $display_current_pagestyle_text: Show or hide ("display_hidden").
 * - $pagestyle_form: The form.
 * - $current_pagestyle_text: The current page style text "Current Style" or "Current Theme".
 * - $pagestyle: The current page style (default Standard).
 *
 * @see pagestyle_theme(), pagestyle-form.tpl.php
 */
function template_preprocess_pagestyle_form(&$variables) {
  $pagestyle_current_pagestyle_text = pagestyle_block_title($title = 'current');
  $variables['pagestyle'] = array();
  $hidden = array();
  // Provide variables named after form keys so themers can print each element independently.
  foreach (element_children($variables['form']) as $key) {
    $type = $variables['form'][$key]['#type'];
    if ($type == 'hidden' || $type == 'token') {
      $hidden[] = drupal_render($variables['form'][$key]);
    }
    else {
      $variables['pagestyle'][$key] = drupal_render($variables['form'][$key]);
    }
  }
  $variables['list_inline'] = "list";
  if (variable_get('pagestyle_display_list_inline', 0) == 1) {
    $variables['list_inline'] = "inline";
  }
  $variables['current_inline'] = "pagestyle_current_list";
  if (variable_get('pagestyle_display_current_inline', 0) == 1) {
    $variables['current_inline'] = "pagestyle_current_inline";
  }
  $variables['current_pagestyle'] = NULL;
  $variables['display_current_pagestyle'] = "display";
  $variables['display_current_pagestyle_text'] = "display";
  switch (variable_get('pagestyle_display_current_text_value', 'text_value')) {
    case "text_value":
      $variables['current_pagestyle'] = "current_pagestyle";
      $variables['display_current_pagestyle'] = "display";
      $variables['display_current_pagestyle_text'] = "display";
    break;
    case "value":
      $variables['current_pagestyle'] = "current_pagestyle";
      $variables['display_current_pagestyle'] = "display";
      $variables['display_current_pagestyle_text'] = "display_hidden";
    break;
    case "hidden":
      $variables['current_pagestyle'] = "current_pagestyle";
      $variables['display_current_pagestyle'] = "display_hidden";
      $variables['display_current_pagestyle_text'] = "display_hidden";
    break;
    case "remove":
      $variables['display_current_pagestyle'] = "display_hidden";
      $variables['display_current_pagestyle_text'] = "display_hidden";
    break;
  }
  // Hidden form elements have no value to themers. No need for separation.
  // $variables['pagestyle']['hidden'] = implode($hidden);
  // Collect all form elements to make it easier to print the whole form.
  $variables['pagestyle_form'] = implode($variables['pagestyle']);
  $variables['current_pagestyle_text'] = $pagestyle_current_pagestyle_text;
  $variables['pagestyle'] = pagestyle_get_current($value = 'name');
}
