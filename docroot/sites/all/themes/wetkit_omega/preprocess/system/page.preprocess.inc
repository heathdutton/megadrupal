<?php
/**
 * @file
 * page.preprocess.inc
 */


/**
 * Implements hook_preprocess_page().
 */
function wetkit_omega_preprocess_page(&$variables, $hook) {

  global $language;

  // Build variables for Primary and Secondary local tasks.
  $variables['primary_local_tasks'] = menu_primary_local_tasks();
  $variables['secondary_local_tasks'] = menu_secondary_local_tasks();

  // Header attributes.
  $variables['header_attributes_array']['id'][] = 'header';
  $variables['header_attributes_array']['class'][] = 'clearfix';
  $variables['header_attributes_array']['role'][] = 'banner';

  // Logo attributes.
  $variables['logo_class'] = '';

  // Build variable(s) for svg logo.
  $variables['logo_svg'] = '';

  // Site name.
  if (!empty($variables['site_name'])) {
    $variables['site_name_title'] = filter_xss(variable_get('site_name', 'Drupal'));
    $variables['site_name'] = l($variables['site_name_title'], variable_get('site_frontpage', 'node'), array('attributes' => array('title' => t('Home page'))));
    $variables['site_name_unlinked'] = $variables['site_name_title'];
  }

  if (module_exists('page_manager')) {
    // Page template suggestions for Panels pages.
    $panel_page = page_manager_get_current_page();
    if (!empty($panel_page)) {
      if (variable_get('wetkit_wetboew_theme', 0) == 1) {
        $suggestions[] = 'page__panels__internet';
      }
      elseif (variable_get('wetkit_wetboew_theme', 0) == 2) {
        $suggestions[] = 'page__panels__intranet';
      }
      elseif (variable_get('wetkit_wetboew_theme', 0) == 3) {
        $suggestions[] = 'page__panels__base';
      }
      elseif (variable_get('wetkit_wetboew_theme', 0) == 4) {
        $suggestions[] = 'page__panels__ogpl';
      }
      else {
        $suggestions[] = 'page__panels__wet';
      }
      // Add the panel page machine name to the template suggestions.
      $suggestions[] = 'page__' . $panel_page['name'];
      // Merge the suggestions in to the existing suggestions
      // (as more specific than the existing suggestions).
      $variables['theme_hook_suggestions'] = array_merge($variables['theme_hook_suggestions'], $suggestions);
    }
    // Page template suggestions for normal pages.
    else {
      if (variable_get('wetkit_wetboew_theme', 0) == 1) {
        $suggestions[] = 'page__internet';
      }
      elseif (variable_get('wetkit_wetboew_theme', 0) == 2) {
        $suggestions[] = 'page__intranet';
      }
      elseif (variable_get('wetkit_wetboew_theme', 0) == 3) {
        $suggestions[] = 'page__base';
      }
      elseif (variable_get('wetkit_wetboew_theme', 0) == 4) {
        $suggestions[] = 'page__ogpl';
      }
      else {
        $suggestions[] = 'page__wet';
      }
      // Merge the suggestions in to the existing suggestions (as more specific
      // than the existing suggestions).
      $variables['theme_hook_suggestions'] = array_merge($variables['theme_hook_suggestions'], $suggestions);
    }
  }

  // Custom Logic Based on Theme Selection
  $toggle_logo = theme_get_setting('toggle_logo', 'wetkit_omega');
  $default_logo = theme_get_setting('default_logo', 'wetkit_omega');
  $default_svg_logo = theme_get_setting('wetkit_theme_svg_default_logo', 'wetkit_omega');

  // INTRANET THEME.
  if (variable_get('wetkit_wetboew_theme', 0) == 1) {
    $theme_prefix = 'gcwu';
    $theme_menu_prefix = 'gcwu-gcnb';

    // Default Logo logic.
    if (($default_logo == 0) && ($default_svg_logo == 1)) {
      if ($language->language == 'en') {
        $variables['logo_svg'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-fegc/images/sig-eng.svg';
        $variables['logo'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-fegc/images/sig-eng.png';
      }
      if ($language->language == 'fr') {
        $variables['logo_svg'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-fegc/images/sig-fra.svg';
        $variables['logo'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-fegc/images/sig-fra.png';
      }

      // Toggle Logo.
      if (!$toggle_logo) {
        $variables['logo_class'] = drupal_attributes(array('class' => 'no-logo'));
      }
    }

    // WMMS Logic.
    $variables['wmms'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-fegc/images/wmms.svg';
    $variables['wmms_alt'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-fegc/images/wmms.png';
  }
  // INTERNET THEME.
  elseif (variable_get('wetkit_wetboew_theme', 0) == 2) {
    $theme_prefix = 'gcwu';
    $theme_menu_prefix = 'gcwu-gcnb';

    // Default Logo logic.
    if (($default_logo == 0) && ($default_svg_logo == 1)) {
      if ($language->language == 'en') {
        $variables['logo_svg'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-intranet/images/sig-blk-eng.svg';
        $variables['logo'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-intranet/images/sig-eng.png';
      }
      if ($language->language == 'fr') {
        $variables['logo_svg'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-intranet/images/sig-blk-fra.svg';
        $variables['logo'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-intranet/images/sig-fra.png';
      }

      // Toggle Logo.
      if (!$toggle_logo) {
        $variables['logo_class'] = drupal_attributes(array('class' => 'no-logo'));
      }
    }

    // WMMS Logic.
    $variables['wmms'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-intranet/images/wmms-intra.svg';
    $variables['wmms_alt'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-gcwu-intranet/images/wmms.png';
  }
  // BASE THEME.
  elseif (variable_get('wetkit_wetboew_theme', 0) == 3) {
    $theme_prefix = 'base';
    $theme_menu_prefix = 'base-gcnb';

    // Toggle Logo.
    if (!$toggle_logo) {
      $variables['logo_class'] = drupal_attributes(array('class' => 'no-logo'));
    }
  }
  // OGPL Theme.
  elseif (variable_get('wetkit_wetboew_theme', 0) == 4) {
    $theme_prefix = 'ogpl';
    $theme_menu_prefix = 'ogpl-fullhd';

    // Default Logo logic.
    if ($default_logo == 0) {
      $variables['logo'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-ogpl/images/theme103_logo.png';
    }

    // Toggle Logo.
    if (!$toggle_logo) {
      $variables['logo'] = '';
      $variables['logo_svg'] = '';
      $variables['logo_class'] = drupal_attributes(array('class' => 'no-logo'));
    }
  }
  // WXT THEME.
  else {
    $theme_prefix = 'wet';
    $theme_menu_prefix = 'wet-fullhd';

    // Default Logo logic.
    if (($default_logo == 0) && ($default_svg_logo == 1)) {
      $variables['logo_svg'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-wet-boew/images/logo.svg';
      $variables['logo'] = libraries_get_path('wet-boew', TRUE) . '/dist/theme-wet-boew/images/logo.png';
    }

    // Toggle Logo.
    if (!$toggle_logo) {
      $variables['logo'] = '';
      $variables['logo_svg'] = '';
      $variables['logo_class'] = drupal_attributes(array('class' => 'no-logo'));
    }
  }

  // Site Name logic.
  if (!empty($variables['site_name'])) {
    $variables['site_name_url'] = url(variable_get('site_frontpage', 'node'));
    $variables['site_name'] = trim($variables['site_name_title']);
  }

  // Internationalization Settings.
  $is_multilingual = 0;
  if (module_exists('i18n_menu') && drupal_multilingual()) {
    $is_multilingual = 1;
  }

  // Assign an alternate Site URL.
  $alternate_site_url = omega_theme_get_setting('wetkit_sub_site_alt_url');
  if (!empty($alternate_site_url)) {
    $variables['site_name_url'] = $alternate_site_url;
  }

  // Assign the search indexing settings.
  $search_indexing_start = omega_theme_get_setting('wetkit_search_indexing_start');
  $search_indexing_stop = omega_theme_get_setting('wetkit_search_indexing_stop');
  if ((!empty($search_indexing_start) && !empty($search_indexing_stop))) {
    $variables['search_indexing_start'] = $search_indexing_start;
    $variables['search_indexing_stop'] = $search_indexing_stop;
  }

  // Set Sub Site Name.
  $variables['wetkit_sub_site'] = omega_theme_get_setting('wetkit_sub_site');

  // Header Navigation + Language Switcher.
  $menu = ($is_multilingual) ? i18n_menu_navigation_links('menu-wet-header') : menu_navigation_links('menu-wet-header');
  $nav_bar_markup = theme('links__menu_menu_wet_header', array(
    'links' => $menu,
    'attributes' => array(
      'id' => 'menu',
      'class' => array('links', 'clearfix'),
    ),
    'heading' => array(),
  ));
  $nav_bar_markup = strip_tags($nav_bar_markup, '<li><a>');

  // Language Handling.
  if (module_exists('wetkit_language')) {
    $language_link_markup = '<li id="' . $theme_menu_prefix . '-lang">' . strip_tags($variables['menu_lang_bar'], '<a><span>') . '</li>';
    if (module_exists('edit')) {
      $quick_edit = edit_trigger_link();
      $variables['menu_bar'] = '<ul>' . $nav_bar_markup . $language_link_markup . '<li>' . drupal_render($quick_edit) . '</li>' . '</ul>';
    }
    else {
      $variables['menu_bar'] = '<ul>' . $nav_bar_markup . $language_link_markup . '</ul>';
    }
  }
  else {
    $variables['menu_bar'] = '<ul>' . $nav_bar_markup . '</ul>';
  }

  // Search Region.
  if (module_exists('custom_search')) {
    // Custom Search.
    $variables['custom_search'] = drupal_get_form('custom_search_blocks_form_1');
    $variables['custom_search']['#id'] = 'search-form';
    $variables['custom_search']['#prefix'] = '<div id="gcwu-srchbx-in">';
    $variables['custom_search']['custom_search_blocks_form_1']['#id'] = $theme_prefix . '-srch';
    $variables['custom_search']['#suffix'] = '</div>';
    $variables['custom_search']['actions']['submit']['#id'] = $theme_prefix . '-srch-submit';
    $variables['custom_search']['actions']['submit']['#attributes']['data-icon'] = 'search';
    $variables['custom_search']['actions']['submit']['#attributes']['value'] = t('Search');
    $variables['search_box'] = render($variables['custom_search']);
    $variables['search_box'] = str_replace('type="text"', 'type="search"', $variables['search_box']);

    // Remove empty attributes.
    if (empty($variables['custom_search']['custom_search_blocks_form_1']['#attributes']['title'])) {
      unset($variables['custom_search']['custom_search_blocks_form_1']['#attributes']['title']);
    }
    if (empty($variables['custom_search']['custom_search_blocks_form_1']['#attributes']['placeholder'])) {
      unset($variables['custom_search']['custom_search_blocks_form_1']['#attributes']['placeholder']);
    }
  }

  // Mega Menu Region.
  if (module_exists('menu_block') && empty($variables['page']['mega_menu'])) {
    $config = menu_block_get_config('main_menu');
    $data = menu_tree_build($config);

    $data['content']['#cache'] = array(
      'keys' => array('wetkit_mega_menu_region_content'),
      'expire' => CACHE_TEMPORARY,
      'granularity' => DRUPAL_CACHE_PER_PAGE, // unset this to cache globally
    );

    $variables['page']['mega_menu'] = $data['content'];
  }

  // Terms Navigation.
  $menu = ($is_multilingual) ? i18n_menu_navigation_links('menu-wet-terms') : menu_navigation_links('menu-wet-terms');
  $terms_bar_markup = theme('links__menu_menu_wet_terms', array(
    'links' => $menu,
    'attributes' => array(
      'class' => array('links', 'clearfix'),
    ),
    'heading' => array(),
  ));
  $variables['menu_terms_bar'] = $terms_bar_markup;

  // Mid Footer Region.
  if (module_exists('menu_block')) {
    $config = menu_block_get_config('mid_footer_menu');
    $data = menu_tree_build($config);

    $data['content']['#cache'] = array(
      'keys' => array('wetkit_mid_footer_region_content'),
      'expire' => CACHE_TEMPORARY,
      'granularity' => DRUPAL_CACHE_PER_PAGE, // unset this to cache globally
    );


    unset($variables['page']['footer']['system_powered-by']);
    $variables['page']['footer']['minipanel'] = $data['content'];
  }

  // Footer Navigation.
  $menu = ($is_multilingual) ? i18n_menu_navigation_links('menu-wet-footer') : menu_navigation_links('menu-wet-footer');
  $footer_bar_markup = theme('links__menu_menu_wet_footer', array(
    'links' => $menu,
    'attributes' => array(
      'id' => 'menu',
      'class' => array('links', 'clearfix'),
    ),
    'heading' => array(),
  ));
  $variables['menu_footer_bar'] = $footer_bar_markup;

  // Footer attributes.
  $variables['footer_attributes_array']['id'][] = 'footer';
  $variables['footer_attributes_array']['class'][] = 'clearfix';
  $variables['footer_attributes_array']['role'][] = 'contentinfo';
}
