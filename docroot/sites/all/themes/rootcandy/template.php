<?php
/**
 * Override or insert variables into the page template for HTML output.
 */
function rootcandy_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

function rootcandy_preprocess_page(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
  // get theme settings
  $vars['hide_header'] = theme_get_setting('rootcandy_header_display');
  $vars['hide_panel'] = theme_get_setting('rootcandy_hide_panel');

  $vars['tabs2'] = array(
    '#theme' => 'menu_local_tasks',
    '#secondary' => $vars['tabs']['#secondary'],
  );
  unset($vars['tabs']['#secondary']);
  //set the links level in settings

  // get users role
  global $user;
  if ($user->uid != 1) {
    // get sorted roles
    $role_menu = _rootcandy_init_role_menu();
    if ($role_menu) {
      $rootcandy_navigation = theme_get_setting('rootcandy_navigation_source_'. $role_menu);
    }
  }
  else {
    $rootcandy_navigation = theme_get_setting('rootcandy_navigation_source_admin');
    if (!isset($rootcandy_navigation)) {
      $rootcandy_navigation = '_rootcandy_default_navigation';
    }
  }

  $rootcandy_nav['menu'] = $rootcandy_navigation;
  if ($rootcandy_navigation == '_rootcandy_default_navigation') {
    $rootcandy_nav['menu'] = 'management';
  }

  $menu_tree = array();
  if (!$rootcandy_navigation) {
    if (!$user->uid) {
      $rootcandy_navigation[] = array('href' => 'user/login', 'title' => t('User login'));
    }
  }
  else if ($rootcandy_navigation == '_rootcandy_default_navigation') {
    if ($user->uid) {
      $rootcandy_navigation = menu_navigation_links($rootcandy_nav['menu'], 1);
    }
    else {
      $rootcandy_navigation = array(array('href' => 'user/login', 'title' => t('User login')));
    }
  }
  else {
    $rootcandy_navigation = menu_navigation_links($rootcandy_navigation);
  }
  $vars['rootcandy_navigation'] = theme('links__rc_main_navigation', array('links' => $rootcandy_navigation));

  $rootcandy_navigation_class = array();
  // detect overlay
  $overlay_mode = FALSE;
  if (module_exists('overlay')) {
    if (overlay_get_mode() == 'child') {
      // we show the panel and header by default
      $vars['hide_panel'] = 0;
      $vars['hide_header'] = 0;

      if (theme_get_setting('rootcandy_header_display_overlay')) {
        $vars['hide_header'] = 1;
      }
      if (theme_get_setting('rootcandy_hide_panel_overlay')) {
        $vars['hide_panel'] = 1;
      }
    }
  }
  if (!$vars['hide_header']) {
    $rootcandy_navigation_class[] = 'i' . theme_get_setting('rootcandy_navigation_icons_size');
    $rootcandy_navigation_class[] = 'header-on';
  }
  $vars['rootcandy_navigation_class'] = '';
  if ($rootcandy_navigation_class) {
    $vars['rootcandy_navigation_class'] .= ' '. implode(' ', $rootcandy_navigation_class);
  }
  if (!theme_get_setting('rootcandy_hide_author')) {
    $vars['legal'] = '<div id="legal-notice">Theme created by <a href="http://sotak.co.uk" rel="external">Marek Sotak</a></div>';
  }

  // fetch user and display welcome message
  if ($user->uid) {
    $user_first_name = $user->name;
    $vars['welcome_user'] = l($user_first_name, 'user', array('attributes' => array('class' => 'user-name'))) . ' | ' . l(t('Logout '), 'user/logout');
  }
  else {
    // TODO: get from settings
    $user_first_name = t('Anonymous');
    $vars['welcome_user'] = l($user_first_name, 'user', array('attributes' => array('class' => 'user-name'))) . ' | ' . l(t('Login '), 'user');
  }
}

function rootcandy_links__rc_main_navigation($variables) {
  global $language_url;
  $links = $variables['links'];
  $attributes = $variables['attributes'];
  $heading = $variables['heading'];
  $output = '';

  if (count($links) > 0) {
    $output = '';
    // Treat the heading first if it is present to prepend it to the
    // list of links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading
        // is a string.
        $heading = array(
          'text' => $heading,
          // Set the default level of the heading. 
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . drupal_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 1;

    $size = theme_get_setting('rootcandy_navigation_icons_size');
    $icons_disabled = theme_get_setting('rootcandy_navigation_icons');
    $list_class = 'i' . $size;

    // custom icons
    $custom_icons = _rootcandy_custom_icons();
    if (!isset($custom_icons)) {
      $custom_icons = '';
    }
    $match = _rootcandy_besturlmatch($_GET['q'], $links);
    $items = array();

    foreach ($links as $key => $link) {
      $class = array($key);
      $id = '';
      $icon = '';
      $class= '';

      // icons
      if (!$icons_disabled) {
        $arg = explode("/", $link['href']);
        $icon = _rootcandy_icon($arg, $size, 'admin', $custom_icons);
        if ($icon) $icon = $icon . '<br />';
        $link['html'] = TRUE;
      }
      if ($key == $match) {
        $id = 'current';
        if (!$icons_disabled && $size) {
          $id = 'current-'. $size;
        }
      }
      // add a class to li
      if (isset($arg) AND is_array($arg)) {
        $class[] = implode($arg, '-');
      }

      // Add first, last and active classes to the list of links to help out themers.
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))
           && (empty($link['language']) || $link['language']->language == $language_url->language)) {
        $class[] = 'active';
      }
      $output .= '<li' . drupal_attributes(array('class' => $class, 'id' => $id)) . '>';

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $output .= l($icon . $link['title'], $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }

      $i++;
      $output .= "</li>\n";
    }

    $output .= '</ul>';
  }

  return $output;
}

function _rootcandy_icon($name, $size = '16', $subdir = '', $icons = '') {
  $url = implode("/", $name);
  $alias = drupal_get_path_alias($url);
  $name = implode("-", $name);
  $path = path_to_theme();
  if ($subdir) {
    $subdir = $subdir . '/';
  }

  if (isset($icons[$url])) {
    $icon = $icons[$url];
  }
  else if (isset($icons[$alias])) {
    $icon = $icons[$alias];
  }
  else {
    $icon = $path . '/images/icons/i' . $size . '/' . $subdir . $name . '.png';
  }
  $output = theme('image', array('path' => $icon));

  if (!$output) {

    $icon = $path . '/images/icons/i' . $size . '/misc/unknown.png';
    $output = theme('image', array('path' => $icon));
  }

  return $output;
}

function _rootcandy_custom_icons() {
  $custom_icons = theme_get_setting('rootcandy_navigation_custom_icons');
  if (isset($custom_icons)) {
    $list = explode("\n", $custom_icons);
    $list = array_map('trim', $list);
    $list = array_filter($list, 'strlen');
    foreach ($list as $opt) {
      // Sanitize the user input with a permissive filter.
      $opt = _rootcandy_filter_xss($opt);
      if (strpos($opt, '|') !== FALSE) {
        list($key, $value) = explode('|', $opt);
        $icons[$key] = $value ? $value : $key;
      }
      else {
        $icons[$opt] = $opt;
      }
    }
  }
  if (isset($icons)) {
    return $icons;
  }
}

function _rootcandy_filter_xss($string) {
  return filter_xss($string);
}

function _rootcandy_besturlmatch($needle, $menuitems) {
  $needle = drupal_get_path_alias($needle);
  $lastmatch = NULL;
  $lastmatchlen = 0;
  $urlparts = explode('/', $needle);
  $partcount = count($urlparts);

  foreach ($menuitems as $key => $menuitem) {
    $href = $menuitem['href'];
    $menuurlparts = explode('/', $href);
    $matches = _rootcandy_countmatches($urlparts, $menuurlparts);
    if (($matches > $lastmatchlen) || (($matches == $lastmatchlen) && (($lastmatch && drupal_strlen($menuitems[$lastmatch]['href'])) > drupal_strlen($href)) )) {
      $lastmatchlen = $matches;
      $lastmatch = $key;
    }
  }
  return $lastmatch;
}

function _rootcandy_countmatches($arrayone, $arraytwo) {
  $matches = 0;
  foreach ($arraytwo as $i => $part) {
    if (!isset($arrayone[$i])) break;
    if ($arrayone[$i] == $part) $matches = $i+1;
  }
  return $matches;
}


function _rootcandy_system_settings_form($form) {
  $themes = list_themes();
  $enabled_theme = arg(4);
  $form = $form['form'];
  if ($form['#id'] == 'system-theme-settings' AND ($enabled_theme == 'rootcandy' || $themes[$enabled_theme]->base_theme == 'rootcandy')) {

    foreach ($form['theme_specific']['rows'] as $rid => $row) {
      //we are only interested in numeric keys
      if (intval($rid)) {
        $this_row = $row['data']['#value'];
        //Add the weight field to the row
        $weight = $form['theme_specific']['rows'][$rid]['role-weight-'. $rid]['#value'];
        $this_row[] = drupal_render($form['theme_specific']['navigation']['nav-by-role']['rootcandy_navigation_source_'. $rid]);
        $this_row[] = drupal_render($form['theme_specific']['rows'][$rid]['role-weight-'. $rid]);
        //Add the row to the array of rows
        $table_rows[$weight] = array('data' => $this_row, 'class' => 'draggable');
      }
    }
    ksort($table_rows);

    $header = array(
      "Role", "Navigation menu", "Order"
    );

    $form['theme_specific']['navigation']['role-weights']['content']['#value'] = theme('table', $header, $table_rows, array('id' => 'rootcandy-settings-table'));
    $output = drupal_render($form);

   // drupal_add_tabledrag('rootcandy-settings-table', 'order', 'sibling', 'weight');
  }
  else {
    $output = drupal_render_children($form);
  }
  return $output;
}

function _rootcandy_init_role_menu() {
  global $theme_key;
  global $user;

  $i = 100;
  //$settings = theme_get_setting($theme_key);
  $menu = array();

  $roles = user_roles(FALSE);

  foreach ($user->roles as $rid => $role) {
    if (!$weight = theme_get_setting('role-weight-'. $rid)) {
      $weight = $i++;
    }
    $menu[$weight] = $rid;
  }

  ksort($menu);
  return $menu[key($menu)];
}

function rootcandy_page_alter(&$page) {
  // TODO: ideally we should merge these from preprocess_page
  // to page_alter and have just one instance of code
  // this is a hack at the moment, but works as expected

  // get users role
  global $user;
  if ($user->uid != 1) {
    // get sorted roles
    $role_menu = _rootcandy_init_role_menu();
    if ($role_menu) {
      $rootcandy_navigation = theme_get_setting('rootcandy_navigation_source_'. $role_menu);
    }
  }
  else {
    $rootcandy_navigation = theme_get_setting('rootcandy_navigation_source_admin');
    if (!isset($rootcandy_navigation)) {
      $rootcandy_navigation = '_rootcandy_default_navigation';
    }
  }

  $rootcandy_nav['menu'] = $rootcandy_navigation;
  if ($rootcandy_navigation == '_rootcandy_default_navigation') {
    $rootcandy_nav['menu'] = 'management';
  }

  // get admin links into the region
  $rootcandy_nav['level'] = 2;

  $menu = menu_navigation_links($rootcandy_nav['menu'], $rootcandy_nav['level']);
  $menu_links = theme('links', array('links' => $menu, 'attributes' => array('id' => 'content-menu')));
  global $language;
  if (!empty($menu_links)) {
    if ($language->direction) {
      $page['sidebar_second']['nav']['#markup'] = $menu_links;
    }
    else {
      $page['sidebar_first']['nav']['#markup'] = $menu_links;
      $page['sidebar_first']['nav']['#weight'] = -100;
      $page['sidebar_first']['#sorted'] = FALSE;
    }
  }
}

