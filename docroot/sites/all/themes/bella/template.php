<?php
define('THEME_SHORTCUT', 'bella');
// Allow theme overrides by checking for the file "bella.custom.inc" inside the "bella" directory.
bella_customize();

/**
 * Hide "No front page content has been created yet" message (if the option is selected).
 */
function bella_toggle_frontpagenocontent_message(&$page) {
  // Avoid notices wherever possible by checking isset().
  if (theme_get_setting('frontpage_hide_no_content_message') && drupal_is_front_page() && isset($page['content']) && isset($page['content']['system_main']) && isset($page['content']['system_main']['default_message'])) {
    hide($page['content']['system_main']['default_message']);
  }
}

/**
 * Allow for theme customizations
 */
function bella_customize() {
  // Check to make sure the functions exist (i.e. that the file itself is not being viewed) to prevent error messages.
  if (function_exists('theme_get_setting') && function_exists('drupal_get_path')) {
    if (theme_get_setting('custom_includes')) {
      $include_file = drupal_get_path('theme', THEME_SHORTCUT) . '/' . THEME_SHORTCUT . '.custom.inc';
      if (file_exists($include_file)) {
        include_once($include_file);
      }
    }
    if (theme_get_setting('custom_stylesheets')) {
      bella_get_custom_styles();
    }
  }
  if (function_exists('drupal_add_js') && function_exists('base_path') && function_exists('drupal_get_path')) {
    drupal_add_js(array('themeInfo' => array('pathToTheme' => base_path() . drupal_get_path('theme', THEME_SHORTCUT))), 'setting');
  }
}

/**
 * Checks to see if the title should be displayed on the front page or not.
 */
function bella_show_title() {
  if (theme_get_setting('full_pages_hide_titles') || (drupal_is_front_page() && !theme_get_setting('frontpage_show_title'))) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Allow the administrator to set the site's footer message automatically
 * @notes
 *   This is primarily for convenience purposes so that the administrator doesn't have to update the site's
 *   copyright date every year. Use case: a site that's in a (wise) situation where the PHP Filter module is not enabled.
 * @return
 *   string An automatically formatted (translateable) footer message whose year displayed should always be the current one.
 */
function bella_override_footer_message() {
  return t('&copy; @year <a href="@basepath" title="@branding">@sitename</a>&trade; - All rights reserved', array('@year' => date('Y'), '@basepath' => base_path(), '@branding' => bella_brandinginfo(), '@sitename' => check_plain(variable_get('site_name', 'Drupal'))));
}

/**
 * Get a sanitized version of the site's info
 * @return
 *   string Site info in the format "My Site Name: My Site Slogan"
 */
function bella_brandinginfo() {
  $name = check_plain(variable_get('site_name', ''));
  $slogan = check_plain(variable_get('site_slogan', ''));
  $text = '';
  if (!empty($name) && !empty($slogan)) {
    $branding[] = $name;
    $branding[] = $slogan;
    $text = implode(': ', $branding);
  }
  else if (!empty($slogan)) {
    $text = $slogan;
  }
  else if (!empty($name)) {
    $text = $name;
  }
  return $text;
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function bella_breadcrumb($vars) {
  $out = '';
  $breadcrumb = $vars['breadcrumb'];
  if (!empty($breadcrumb)) {
    $separator_exists = theme_get_setting('breadcrumb_separator');
    $separator = $separator_exists ? $separator_exists :  ' &#8674; ';
    $separator = filter_xss($separator, array('div', 'span'));
    $count = count($breadcrumb);
    if ($count >= 2) {
      $out .= '<div class="breadcrumb">' . implode($separator, $breadcrumb) . '</div>';
    }
  }
  return $out;
}

/**
 * Verify if the current user has permission to search and if the theme has been
 * set to display the search box, and if so, print the search box.
 */
function bella_searchbox() {
  $out = '';
  if (theme_get_setting('search_box_display') && user_access('search content')) {
    $out .= '<div class="block">' . drupal_render(drupal_get_form('search_block_form')) . '</div>';
  }
  return $out;
}

/**
 * Include custom CSS files, if they exist.
 * Example: to add a custom stylesheet for handheld devices, create a file inside this theme's
 * directory called custom-style-handheld.css
 *
 * See README.txt or notes below for more in-depth explanations.
 */
function bella_get_custom_styles() {
  global $base_url, $language;
  $output = '';
  $theme_shortcut = THEME_SHORTCUT;
  $site_base = substr($_SERVER['HTTP_HOST'] . base_path(), 0, -1);
  $customcss_types = array('all', 'aural', 'braille', 'handheld', 'projection', 'print', 'screen', 'tty', 'tv');
  $theme_path = drupal_get_path('theme', $theme_shortcut) . '/';
  $default_css_file = $theme_path . 'css/style.css';
  $customcss = array();

  // First we check for a stylesheet called "custom-style.css". If this file is found 
  // inside this theme's directory, we'll add it to every site of this Drupal
  // installation that uses this theme. Anything found here will be able to override
  // anything found inside the theme's default stylesheet.
  $file_to_add_all_sites = $theme_path . 'custom-style.css';
  if (file_exists($file_to_add_all_sites)) {
    $customcss[] = array('file' => $file_to_add_all_sites, 'type' => 'all', 'browsers' => array(), 'weight' => 10, 'preprocess' => TRUE);
  }

  // Next we check for stylesheets based on domain name alone.
  // Assuming your domain name is www.example.com, if there is a file named "custom-style-www.example.com.css"
  // inside this theme's directory, add that file (stylesheet) ONLY when the user visits www.example.com
  // (so if they visit www.example.org, the file custom-style-www.example.com.css will NOT appear).
  // Anything found here will be able to override what may have been found inside the "style.css" file
  // (above) as well as anything inside the theme's default stylesheet.
  $file_to_add_this_site = $theme_path . 'custom-style-' . $site_base . '.css';
  if (file_exists($file_to_add_this_site)) {
    $customcss[] = array('file' => $file_to_add_this_site, 'type' => 'all', 'browsers' => array(), 'weight' => 20, 'preprocess' => TRUE);
  }

  // Regardless of whether the above file(s) were found or not, now we check for more customized files
  // so that site admins can add custom stylesheets based on other criteria.

  // Now we check for stylesheets based on media type alone.
  // Each of these files will be able to override what may have been found so far in any 
  // of the stylesheets above and/or anything inside the theme's default stylesheet.
  // Example: custom-style-handheld.css
  foreach ($customcss_types as $type) {
    $file_to_add_by_type = $theme_path . 'custom-style-' . $type . '.css';
    if (file_exists($file_to_add_by_type)) {
      $customcss[] = array('file' => $file_to_add_by_type, 'type' => $type, 'browsers' => array(), 'weight' => 30, 'preprocess' => TRUE);
    }
  }

  // Now we check for stylesheets based on a combination of the site name as well as the media type.
  // Each of these files will be able to further override what may have been found so far in any 
  // of the stylesheets above and/or anything inside the theme's default stylesheet.
  foreach ($customcss_types as $type) {
    // Regardless of whether those file(s) were found or not, now we check for even deeper customization files
    // so that each site in a multi-site installation can use its own specific file(s).
    $file_to_add_by_site = $theme_path . 'custom-style-' . $type . '-' . $site_base . '.css';
    if (file_exists($file_to_add_by_site)) {
      $customcss[] = array('file' => $file_to_add_by_site, 'type' => $type, 'browsers' => array(), 'weight' => 40, 'preprocess' => TRUE);
    }
  }

  // Almost done. Now we check for stylesheets based on language.
  if ($language->direction == LANGUAGE_RTL) {
    // To add a RTL stylesheet for all sites, simply create a file called custom-style-rtl.css and place it inside this theme's directory.
    $file_to_add_all_sites_rtl = $theme_path . 'custom-style-rtl.css';
    if (file_exists($file_to_add_all_sites_rtl)) {
      $customcss[] = array('file' => $file_to_add_all_sites_rtl, 'type' => 'all', 'browsers' => array(), 'weight' => 50, 'preprocess' => TRUE);
    }
    foreach ($customcss_types as $type) {
      // To add site-specific stylesheets, use the same format as above ($file_to_add_by_site).
      // Example usage: custom-style-all-www.example.com-rtl.css
      //   where "all" is replaceable by the media type, and "www.example.com" is replaced by the specific site's domain name.
      $file_to_add_by_site_rtl = $theme_path . 'custom-style-' . $type . '-' . $site_base . '-rtl.css';
      if (file_exists($file_to_add_by_site_rtl)) {
        $customcss[] = array('file' => $file_to_add_by_site_rtl, 'type' => $type, 'browsers' => array(), 'weight' => 60, 'preprocess' => TRUE);
      }
    }
  }

  // Finally, no matter how hard we try, we can't forget about IE. ;)
  // To add a stylesheet for IE8, create a file named custom-style-ie8.css and place it in this theme's directory.
  // Or, to add a stylesheet for IE8 that should only be loaded for right-to-left languages, create a file
  // named custom-style-ie8-rtl.css and place it in this theme's directory.
  // Follow this same pattern as needed for IE7 and IE6, replacing the number 8 with the number 7 or 6.
  $ie_versions = array(8, 7, 6);
  $weight_ie_base = 100;
  $weight_ie_ltr_base = 200;
  $weight_ie_rtl_base = 300;
  foreach ($ie_versions as $ie_version) {
    $file_to_add_ie = $theme_path . 'custom-style-ie' . $ie_version . '.css';
    $file_to_add_ie_ltr = $theme_path . 'custom-style-ie' . $ie_version . '-ltr.css';
    $file_to_add_ie_rtl = $theme_path . 'custom-style-ie' . $ie_version . '-rtl.css';
    $weight_ie = $weight_ie_base - $ie_version;
    $weight_ie_ltr = $weight_ie_ltr_base - $ie_version;
    $weight_ie_rtl = $weight_ie_rtl_base - $ie_version;
    if (file_exists($file_to_add_ie)) {
      $ie_val = 'lte IE ' . $ie_version;
      $customcss[] = array('file' => $file_to_add_ie, 'type' => 'all', 'browsers' => array('IE' => $ie_val, '!IE' => FALSE), 'weight' => $weight_ie, 'preprocess' => FALSE);
    }
    if (file_exists($file_to_add_ie_ltr) && $language->direction == LANGUAGE_LTR) {
      $ie_val_ltr = 'lte IE ' . $ie_version;
      $customcss[] = array('file' => $file_to_add_ie_ltr, 'type' => 'all', 'browsers' => array('IE' => $ie_val_ltr, '!IE' => FALSE), 'weight' => $weight_ie_ltr, 'preprocess' => FALSE);
    }
    if (file_exists($file_to_add_ie_rtl) && $language->direction == LANGUAGE_RTL) {
      $ie_val_rtl = 'lte IE ' . $ie_version;
      $customcss[] = array('file' => $file_to_add_ie_rtl, 'type' => 'all', 'browsers' => array('IE' => $ie_val_rtl, '!IE' => FALSE), 'weight' => $weight_ie_rtl, 'preprocess' => FALSE);
    }
  }

  if (!empty($customcss)) {
    foreach ($customcss as $css) {
      $css_data = $css['file'];
      $css_opts = array(
        'type' => 'file',
        'group' => CSS_THEME,
        'browsers' => $css['browsers'],
        'every_page' => TRUE,
        'weight' => $css['weight'],
        'media' => $css['type'],
        'preprocess' => $css['preprocess']
      );
      drupal_add_css($css_data, $css_opts);
    }
  }
}

/**
 * Social links - top
 */
function bella_social_links() {
  $out = '';
  if (theme_get_setting('social_links_display')) {
    $displays_possible = array(
      'facebook' => 'social_links_display_links_facebook',
      'googleplus' => 'social_links_display_links_googleplus',
      'twitter' => 'social_links_display_links_twitter',
      'instagram' => 'social_links_display_links_instagram',
      'pinterest' => 'social_links_display_links_pinterest',
      'linkedin' => 'social_links_display_links_linkedin',
      'youtube' => 'social_links_display_links_youtube',
      'vimeo' => 'social_links_display_links_vimeo',
      'flickr' => 'social_links_display_links_flickr',
      'tumblr' => 'social_links_display_links_tumblr',
      'skype' => 'social_links_display_links_skype',
      'rss' => 'social_links_display_links_rss',
    );
    foreach ($displays_possible as $key => $display_possible) {
      $link_possible = $display_possible . '_link';
      if (theme_get_setting($display_possible) && $link = theme_get_setting($link_possible)) {
        $url = check_url($link);
        $classes = 'sociallinks ' . $key;
        $out .=   l('', $url, array('attributes' => array('class' => $classes)));
      }
    }
  }
  return $out;
}

/**
 * Primary and seconday menus.
 */
function bella_nav_menu($menu, $menu_id = 'main-menu', $html_id = 'navlist', $html_classes = array('menu'), $heading = '') {
  $menu_id = check_plain($menu_id);
  $out = '';
  if (theme_get_setting('menus_show_expanded_main_menu') && $menu_id == 'main-menu') {
    $menu_source = variable_get('menu_main_links_source', $menu_id);
    if (module_exists('i18n_menu') && function_exists('i18n_menu_translated_tree')) {
      $tree = i18n_menu_translated_tree($menu_source);
    }
    else {
      $tree = menu_tree($menu_source); 
    }
    $out .= '<div id="main-menu-wrap"><ul id="main-menu-outer-ul">' . drupal_render($tree) . '</ul></div>';
  }
  else {
    $theme_arg = 'links__system_' . str_replace('-', '_', $menu_id);
    $out .= theme($theme_arg, array('links' => $menu, 'attributes' => array('id' => $html_id, 'class' => $html_classes), 'heading' => $heading));
  }
  return $out;
}

/**
 * Sets the wrapper tag class.
 */
function bella_wrapper_class($sidebar_left, $sidebar_right) {
  $out = '';
  $class = 'sidebars-none';
  if (!empty($sidebar_left) && !empty($sidebar_right)) {
    $class = 'sidebars';
  }
  else {
    if (!empty($sidebar_left)) {
      $class = 'sidebar-left';
    }
    if (!empty($sidebar_right)) {
      $class = 'sidebar-right';
    }
  }
  if (isset($class)) {
    $out .= ' class="'. $class .'"';
  }
  return $out;
}
