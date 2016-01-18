<?php
/**
 * @file
 * IMPORTANT WARNING: DO NOT MODIFY THIS FILE.
 *
 * If you need to add or modify preprocess or process functions do it in
 * your sub-theme.
 */

/**
 * Preprocess variables for html.tpl.php
 */
function storefront_preprocess_html(&$vars) {
  global $language;
  global $theme_key;
  $theme_name = 'storefront';
  $path_to_theme = drupal_get_path('theme', $theme_name);

  $vars['doctype'] = '<!DOCTYPE html>' . "\n";
  $vars['rdf_profile'] = '';

  // Clean up the lang attributes.
  $vars['html_attributes'] = 'lang="' . $language->language . '" dir="' .
    $language->dir . '"';

  $mobile_menu = theme_get_setting('mobilemenu');
  if ($mobile_menu == 'slide') {
    drupal_add_js($path_to_theme . '/js/conditional/mobilemenu.js');
    $vars['classes_array'][] = 'menu-slide';
  }
  elseif ($mobile_menu == 'jump') {
    drupal_add_js($path_to_theme . '/js/conditional/jumpmenu.js');
    $vars['classes_array'][] = 'menu-jump';
  }
  if (theme_get_setting('widescreen')) {
    drupal_add_css($path_to_theme . '/css/responsive/widescreen.css');
    $vars['classes_array'][] = 'media-widescreen';
  }
  if (theme_get_setting('tablet')) {
    drupal_add_css($path_to_theme . '/css/responsive/tablet.css');
    $vars['classes_array'][] = 'media-tablet';
  }
  if (theme_get_setting('smartphone')) {
    drupal_add_css($path_to_theme . '/css/responsive/smartphone.css');
    $vars['classes_array'][] = 'media-smartphone';
  }
  if (theme_get_setting('dlayout_framing') == 'framed') {
    $vars['classes_array'][] = 'format-framed';
  }
  elseif (theme_get_setting('dlayout_framing') == 'float') {
    $vars['classes_array'][] = 'format-float';
  }
  else {
    $vars['classes_array'][] = 'format-full';
  }

  // Add extra class if set.
  if (theme_get_setting('extra_page_classes')) {
    if (function_exists('locale')) {
      $vars['classes_array'][] = 'lang-' . $vars['language']->language;
    }

    // Add a class for the active color scheme.
    if (module_exists('color')) {
      $class = check_plain(get_color_scheme_name($theme_key));
      $vars['classes_array'][] = 'color-scheme-' . drupal_html_class($class);
    }

    // Classes for body element. Allows advanced theming based on context.
    if (!$vars['is_front']) {
      $path = drupal_get_path_alias($_GET['q']);
      list($section,) = explode('/', $path, 2);
      $arg = explode('/', $_GET['q']);
      if ($arg[0] == 'node') {
        if ($arg[1] == 'add') {
          $section = 'node-add';
        }
        elseif (isset($arg[2]) && is_numeric($arg[1]) && ($arg[2] == 'edit' || $arg[2] == 'delete')) {
          $section = 'node-' . $arg[2];
        }
      }
      $vars['classes_array'][] = drupal_html_class('section-' . $section);
    }


    // Set classes for views and panels pages.
    $vars['menu_item'] = menu_get_item();
    switch ($vars['menu_item']['page_callback']) {
      case 'views_page':
        $vars['classes_array'][] = 'page-views';
        break;

      case 'page_manager_page_execute':
      case 'page_manager_node_view':
      case 'page_manager_contact_site':
        $vars['classes_array'][] = 'page-panels';
        break;

    }
  }

  // Load the media queries layout CSS files (from the default files folder.
  global $theme;
  $filename = $theme . '_mediaqueries_css';
  $file = variable_get($filename);
  $path = "public://sf_css";
  if (!empty($file)) {
    $filepath = $path . '/' . $file;
     drupal_add_css($filepath, array(
      'preprocess' => variable_get('preprocess_css', '') == 1 ? TRUE : FALSE,
      'group' => CSS_THEME,
      'media' => 'screen',
      'every_page' => TRUE,
      )
    );
  }

  // Set container class for content displays.
  $show_frontpage_grid = theme_get_setting('content_display_grids_frontpage') == 1 ? TRUE : FALSE;
  $show_taxopage_grid = theme_get_setting('content_display_grids_taxonomy_pages') == 1 ? TRUE : FALSE;
  if ($show_frontpage_grid == TRUE) {
    $cols_fpg = theme_get_setting('content_display_grids_frontpage_colcount');
    $vars['classes_array'][] = 'fpcc-' . $cols_fpg;
    drupal_add_js($path_to_theme . '/js/conditional/eq.fp.grid.js');
  }
  if ($show_taxopage_grid == TRUE) {
    $cols_tpg = theme_get_setting('content_display_grids_taxonomy_pages_colcount');
    $vars['classes_array'][] = $cols_tpg;
    drupal_add_js($path_to_theme . '/js/conditional/eq.tp.grid.js');
  }

}

/**
 * Process variables for html.tpl.php file.
 */
function storefront_process_html(&$vars) {
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}

/**
 * Preprocess variables for page.tpl.php file.
 */
function storefront_preprocess_page(&$vars) {
  $vars['logo_alt_text'] = check_plain(variable_get('site_name', '')) . ' ' .
    t('logo');
  $vars['logo_img'] = $vars['logo'] ? '<img src="' . check_url($vars['logo']) .
    '" alt="' . $vars['logo_alt_text'] . '"/>' : '';
  $vars['linked_site_logo'] = $vars['logo_img'] ? l($vars['logo_img'], '<front>',
    array(
      'attributes' => array(
        'title' => t('Home page'),
      ),
      'html' => TRUE,
    )
  ) : '';

  // If they turn off the site name, still print it, but hide it.
  if (theme_get_setting('toggle_name') == FALSE) {
    $vars['visibility'] = 'element-invisible';
    $vars['hide_site_name'] = TRUE;
  }
  else {
    $vars['visibility'] = '';
    $vars['hide_site_name'] = FALSE;
  }

  // Get sidebar grid settings based on theme setting selections.
  global $theme_info;

  $grid = array();
  $grid['container'] = theme_get_setting('grid_columns');
  $grid['columns']['count'] = str_replace('container-', '',
    theme_get_setting('grid_columns'));
  $grid['width'] = theme_get_setting('dlayout_max_width') . theme_get_setting('dlayout_max_width_unit');
  $grid['layout'] = theme_get_setting('dlayout_layout');

  // Set grid sizes based on the container settings.
  switch ($grid['container']) {
    case 'container-24':
      $tgrid_first = theme_get_setting('grid_24_first');
      $tgrid_second = theme_get_setting('grid_24_second');
      break;

    case 'container-16':
      $tgrid_first = theme_get_setting('grid_16_first');
      $tgrid_second = theme_get_setting('grid_16_second');
      break;

    case 'container-12':
      $tgrid_first = theme_get_setting('grid_12_first');
      $tgrid_second = theme_get_setting('grid_12_second');
      break;

    default:
      $tgrid_first = $theme_info->info['settings']['grid_24_sidebar_first'];
      $tgrid_second = $theme_info->info['settings']['grid_24_sidebar_second'];
  }

  // Set default grid number.
  if ($tgrid_first == '') {
    $tgrid_first = 6;
  }
  if ($tgrid_second == '') {
    $tgrid_second = 6;
  }
  $grid['columns']['first']['grid'] = $tgrid_first;
  $grid['columns']['second']['grid'] = $tgrid_second;

  $tgrid_count = $grid['columns']['count'];
  $grid['columns']['main']['grid']
    = $tgrid_count - ($tgrid_first + $tgrid_second);

  // Set pull settings based on layout chosen.
  switch ($grid['layout']) {
    case 'three-col-grail':
      $grid['columns']['first']['pull']
        = $tgrid_count;
      $grid['columns']['second']['pull']
        = $grid['columns']['second'];
      break;

    case 'three-col-left':
      $grid['columns']['first']['pull']
        = $tgrid_count;
      $grid['columns']['second']['pull']
        = $grid['columns']['second'];
      break;

    case 'three-col-right':
      $grid['columns']['first']['pull']
        = $tgrid_count;
      $grid['columns']['second']['pull']
        = $grid['columns']['second'];
      break;

  }

  // Set variable for layout class for better control of page layouts.
  $vars['theme_layout'] = $grid['layout'];

  // Define the variables that will be used from on page.
  $vars['container'] = $grid['container'];
  $grid_columns = $grid['columns']['count'];
  $grid_first = $grid['columns']['first']['grid'];
  $grid_second = $grid['columns']['second']['grid'];
  $grid_layout = $grid['layout'];

  // Set the Framing settings.
  if (theme_get_setting('dlayout_framing') == 'framed') {
    $vars['frame'] = $vars['container'];
    $vars['container'] = '';
  }
  elseif (theme_get_setting('dlayout_framing') == 'float') {
    $vars['frame'] = $vars['container'];
    $vars['container'] = '';
  }
  else {
    $vars['frame'] = '';
  }

  // If nothing is configured yet, set the defaults.
  if ($grid_first == '') {
    $grid_first = $theme_info->info['settings']['grid_24_sidebar_first'];
  }
  if ($grid_second == '') {
    $grid_second = $theme_info->info['settings']['grid_24_sidebar_second'];
  }

  // Get Sidebear Variables.
  $sf_sbf = $vars['page']['sidebar_first'];
  $sf_sbs = $vars['page']['sidebar_second'];

  // If no sidebar, set sidebar # to zero when calculating main content.
  if (!empty($sf_sbf)) {
    $number_first = $grid_first;
  }
  else {
    $number_first = 0;
  }

  if (!empty($sf_sbs)) {
    $number_second = $grid_second;
  }
  else {
    $number_second = 0;
  }
  $number_width = ($grid_columns) - ($number_first + $number_second);
  $main_width = 'grid-' . $number_width;
  $vars['sidebar_first_grid'] = 'grid-' . $number_first;
  $vars['sidebar_second_grid'] = 'grid-' . $number_second;

  // Set push of main content based on layout settings & regions present.
  switch (theme_get_setting('dlayout_layout')) {
    case 'three-col-grail':
      $main_left = 'push-' . $number_first;
      $vars['sidebar_first_pull'] = 'pull-' . $grid_columns;
      $vars['sidebar_second_pull'] = 'pull-' . $number_second;
      break;

    case 'three-col-left':
      $main_left = 'push-' . ($number_first + $number_second);
      $vars['sidebar_first_pull'] = 'pull-' . $grid_columns;
      $vars['sidebar_second_pull'] = 'pull-' . ($number_width + $number_second);
      break;

    case 'three-col-right':
      $main_left = '';
      $vars['sidebar_first_pull'] = 'pull-' . ($number_first + $number_second);
      $vars['sidebar_second_pull'] = 'pull-' . $number_second;
      break;

    default:
      $main_left = 'push-' . $number_first;
  }

  if (!empty($sf_sbf) || theme_get_setting('dlayout_layout') == 'three-col-left') {
    $vars['main_grid_classes'] = $main_width . ' ' . $main_left;
  }
  else {
    $vars['main_grid_classes'] = $main_width;
  }

  // Setting Main Menu block links to Primary Navigation variable.
  if (isset($vars['main_menu'])) {
    $text = block_get_blocks_by_region('menu_bar') ? t('Navigation') : t('Main menu');

    $vars['primary_navigation'] = theme('links', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('menu', 'primary-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => $text,
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }

  // Add Wrapper From template.helpers.inc file.
  if (!empty($vars['primary_navigation'])) {
    $vars['primary_navigation'] = storefront_menu_variables($vars['primary_navigation'], 'primary');
  }

  // Hide title and add classes in SECONDARY NAVIGATION.
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_navigation'] = theme('links', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('menu', 'secondary-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Secondary navigation'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }

  // Add Wrapper From template.helpers.inc file.
  if (!empty($vars['secondary_navigation'])) {
    $vars['secondary_navigation'] = storefront_menu_variables(
      $vars['secondary_navigation'], 'secondary'
    );
  }

  // Primary & Secondary Tabs.
  $vars['primary_local_tasks'] = menu_primary_local_tasks();
  $vars['secondary_local_tasks'] = menu_secondary_local_tasks();

  if (!empty($vars['tabs']['#primary']) || !empty($vars['tabs']['#secondary'])) {
    $vars['tab_class'] = 'content-tab';
  }
  else {
    $vars['tab_class'] = '';
  }

  // Work-around a stupid bug in Drupal 7 (thanks AT).
  global $user;
  if (arg(0) == 'user' && arg(1) == 'login' || arg(0) == 'user' && $user->uid == 0) {
    drupal_set_title(t('User login'));
  }
  if (arg(0) == 'user' && arg(1) == 'password') {
    drupal_set_title(t('Request new password'));
  }
  if (arg(0) == 'user' && arg(1) == 'register') {
    drupal_set_title(t('Create new account'));
  }
}

/**
 * Preprocess variables for page.tpl.php
 */
function storefront_process_page(&$vars) {
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}

/**
 * Preprocess variables for node.tpl.php
 */
function storefront_preprocess_node(&$vars) {
  global $theme, $user;

  global $theme_info;

  $theme_name = 'storefront';
  $path_to_theme = drupal_get_path('theme', $theme_name);

  // If content grid, display teasers with grid numbers.
  $show_frontpage_grid = theme_get_setting('content_display_grids_frontpage') == 1 ? TRUE : FALSE;

  // Collect the sidebars and main content settings.
  if ($show_frontpage_grid == TRUE) {

    $grid_columns = theme_get_setting('grid_columns');
    $cols_fpg = theme_get_setting('content_display_grids_frontpage_colcount');
    $vars['classes_array'][] = 'fpcc-' . $cols_fpg;
  }

  if (theme_get_setting('extra_article_classes')) {
    if (module_exists('translation')) {
      if ($vars['node']->language) {
        global $language;
        $vars['classes_array'][] = 'article-lang-' . $vars['node']->language;
      }
    }
  }

  if (theme_get_setting('extra_article_classes')) {
    $vars['classes_array'][] = $vars['zebra'];
    if ($vars['display_submitted'] && !empty($vars['picture'])) {
      $vars['classes_array'][] = 'article-with-picture';
    }
  }

  if (!$vars['teaser'] && theme_get_setting('extra_article_classes')) {
    $vars['classes_array'][] = drupal_html_class('article-' . $vars['view_mode']);
  }

  $vars['attributes_array']['role'][] = 'article';
  $vars['title_attributes_array']['class'][] = 'article-title';
  $vars['content_attributes_array']['class'][] = 'article-content';

  // PHP 'c' format is not proper ISO8601, we need to build it.
  $vars['datetime'] = format_date($vars['created'], 'custom', 'Y-m-d\TH:i:s\Z');

  if (variable_get('node_submitted_' . $vars['node']->type, TRUE)) {
    $vars['submitted'] = t('Submitted by !username on !datetime',
      array(
        '!username' => $vars['name'],
        '!datetime' => '<time datetime="' . $vars['datetime'] .
        '" pubdate="pubdate">' . $vars['date'] . '</time>',
      )
    );
  }
  else {
    $vars['submitted'] = '';
  }

  $vars['unpublished'] = '';
  if (!$vars['status']) {
    $vars['unpublished'] = '<div class="unpublished">' . t('Unpublished') . '</div>';
  }

  if (theme_get_setting('node_image_float')) {
    $vars['classes_array'][] = theme_get_setting('node_image_float');
  }
  if ($vars['page']) {
    $vars['classes_array'][] = 'node-full';
  }
  if ($vars['type'] == 'product_display') {
    if (!empty($vars['elements']['product:field_image'][0])) {
      $vars['image_class'] = 'with-image';
      drupal_add_js($path_to_theme . '/js/conditional/eq.prod.node.js');
    }
    else {
      $vars['image_class'] = 'no-image';
    }
  }
  if ($vars['type'] == 'product_detail') {
    if (!empty($vars['field_images'][0])) {
      $vars['image_class'] = 'with-image';
    }
    else {
      $vars['image_class'] = 'no-image';
    }
  }
}

/**
 * Preprocess variables for region.tpl.php file.
 */
function storefront_preprocess_region(&$vars) {
  $theme_name = 'storefront';
  $path_to_theme = drupal_get_path('theme', $theme_name);
  if ($vars['region'] == 'toolbar') {
    drupal_add_js($path_to_theme . '/js/conditional/script.toolbar.js');
  }
}

/**
 * Preprocess variables for block.tpl.php file.
 */
function storefront_preprocess_block(&$vars) {

  if (theme_get_setting('extra_block_classes')) {
    $vars['classes_array'][] = 'block-' . $vars['block_zebra'];
    $vars['classes_array'][] = 'block-count-' . $vars['id'];
    $vars['classes_array'][] = drupal_html_class('block-region-' .
      $vars['block']->region);
  }
  if (theme_get_setting('horizontal_login_block')
    && $vars['block']->module == 'user' && $vars['block']->delta == 'login') {
    $vars['classes_array'][] = 'lb-h';
  }

  $vars['title_attributes_array']['class'][] = 'block-title';
  $vars['content_attributes_array']['class'][] = 'block-content';
  $vars['content_attributes_array']['class'][] = 'content';
  $nav_blocks = array('navigation', 'main-menu', 'management', 'user-menu');

  if (in_array($vars['block']->delta, $nav_blocks)) {
    $vars['theme_hook_suggestions'][] = 'block__menu';
  }
  $nav_modules = array('superfish', 'nice_menus');
  if (in_array($vars['block']->module, $nav_modules)) {
    $vars['theme_hook_suggestions'][] = 'block__menu';
  }
  if ($vars['block']->region == 'menu_bar') {
    $vars['theme_hook_suggestions'][] = 'block__menu_bar';
    $vars['title_attributes_array']['class'][] = 'element-invisible';
  }
  if ($vars['block']->region == 'header') {
    $vars['theme_hook_suggestions'][] = 'block__header';
    $vars['title_attributes_array']['class'][] = 'element-invisible';
  }

  // Provide template suggestions such as block--header--search.
  $vars['theme_hook_suggestions'][] = 'block__' . $vars['block']->region .
    '__' . $vars['block']->module;

  // Add EqualHeights if the SF Browse Products block is available.
  $theme_name = 'storefront';
  $path_to_theme = drupal_get_path('theme', $theme_name);
  if ($vars['block_html_id'] == 'block-quicktabs-browse-products' ||
      $vars['block_html_id'] == 'block-quicktabs-browse-products-full') {
    drupal_add_js($path_to_theme . '/js/conditional/script.sftools.js');
  }

  // Add CSS if the blocks are present.
  if ($vars['block_html_id'] == 'block-views-promos-block' || $vars['block_html_id'] == 'block-quicktabs-browse-products') {
    drupal_add_css($path_to_theme . '/css/sftools.css');
  }

}
