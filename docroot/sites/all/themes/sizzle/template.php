<?php
/**
 * @file
 * Theme functions
 */

// Include all files from the includes directory.
$includes_path = dirname(__FILE__) . '/includes/*.inc';
foreach (glob($includes_path) as $filename) {
  require_once dirname(__FILE__) . '/includes/' . basename($filename);
}

/**
 * Implements hook_admin_links_info_alter().
 */
function sizzle_admin_links_info_alter(&$info) {
  // Add contextual links to header.
  $info['header'] = restaurant_admin_links_build_links(array(
    'edit_logo' => array(
      'title' => t('Edit logo'),
      'href' => 'admin/appearance/settings',
    ),
    'edit_site_name' => array(
      'title' => t('Edit site information'),
      'href' => 'admin/config/restaurant/general',
    ),
  ), '.header');

  // Add contextual links to address.
  $info['address'] = restaurant_admin_links_build_links(array(
    'edit' => array(
      'title' => t('Edit contact information'),
      'href' => 'admin/config/restaurant/contact',
    ),
  ), '.header .address');

  // Add contextual links to footer.
  $info['footer'] = restaurant_admin_links_build_links(array(
    'edit_logo' => array(
      'title' => t('Edit footer'),
      'href' => 'admin/appearance/settings',
    ),
  ), '.footer');

  // Add contextual links to main menu.
  $info['navbar'] = restaurant_admin_links_build_links(array(
    'add' => array(
      'title' => t('Add Link'),
      'href' => 'admin/structure/menu/manage/main-menu/add',
    ),
    'edit' => array(
      'title' => t('Edit Menu'),
      'href' => 'admin/structure/menu/manage/main-menu',
    ),
  ), '.navbar > .container');

  // Add contextual links to footer menu.
  $info['footer_nav'] = restaurant_admin_links_build_links(array(
    'add' => array(
      'title' => t('Add Link'),
      'href' => 'admin/structure/menu/manage/menu-footer-menu/add',
    ),
    'edit' => array(
      'title' => t('Edit Menu'),
      'href' => 'admin/structure/menu/manage/menu-footer-menu',
    ),
  ), '.footer-nav');

  // Add contextual links to copyright.
  $info['copyright'] = restaurant_admin_links_build_links(array(
    'edit' => array(
      'title' => t('Edit copyright'),
      'href' => 'admin/appearance/settings',
      'options' => array(
        'fragment' => 'footer'
      ),
    ),
  ), '.footer .copyright');

  // Add contextual links to copyright.
  $info['footer_text'] = restaurant_admin_links_build_links(array(
    'edit' => array(
      'title' => t('Edit footer text'),
      'href' => 'admin/appearance/settings',
      'options' => array(
        'fragment' => 'footer'
      ),
    ),
  ), '.footer .footer-text');

  // Add contextual links to copyright.
  $info['menu_categories'] = restaurant_admin_links_build_links(array(
    'edit' => array(
      'title' => t('Edit categories'),
      'href' => 'admin/structure/taxonomy/menu_categories',
    ),
  ), '.pane-menu-categories-menu-categories');
}

/**
 * Implements hook_css_alter().
 */
function sizzle_css_alter(&$css) {
  $radix_path = drupal_get_path('theme', 'radix');

  // Radix now includes compiled stylesheets for demo purposes.
  // We remove these from our subtheme since they are already included 
  // in compass_radix.
  unset($css[$radix_path . '/assets/stylesheets/radix-style.css']);
  unset($css[$radix_path . '/assets/stylesheets/radix-print.css']);
}

/**
 * Implements template_preprocess_html().
 */
function sizzle_preprocess_html(&$variables, $hook) {
  // Add site width to classes for theming.
  $variables['classes_array'][] = theme_get_setting('site_width');
}

/**
 * Implements template_preprocess_page().
 */
function sizzle_preprocess_page(&$variables) {
  $theme_path = drupal_get_path('theme', 'sizzle');

  // Add a menu link.
  $variables['menu_link'] = l(t('The Menu'), 'menus', array(
    'attributes' => array(
      'class' => array('btn'),
    ),
  ));

  // Add a reservation link.
  $variables['reservation_link'] = '';
  if (module_exists('restaurant_reservation')) {
    $variables['reservation_link'] = l(t('Book a Table'), 'reservation', array(
      'attributes' => array(
        'class' => array('btn'),
      ),
    ));
  }

  // Add the footer menu to the template.
  $show_footer_nav = theme_get_setting('show_footer_nav');
  if ($show_footer_nav) {
    $footer_nav_data = menu_build_tree('menu-footer-menu', array(
      'min_depth' => 1,
      'max_depth' => 2,
    ));
    $variables['footer_nav'] = menu_tree_output($footer_nav_data);
    $variables['footer_nav']['#theme_wrappers'] = array();
  }

  // Add the site background image.
  if ($site_background_image_fid = theme_get_setting('site_background_image')) {
    $site_background_image = file_load($site_background_image_fid);
    $site_background_image_url = file_create_url($site_background_image->uri);
    drupal_add_css('body { background-image: url("' . $site_background_image_url . '") }', array('type' => 'inline'));
  }

  // Add the footer background image.
  if ($footer_background_image_fid = theme_get_setting('footer_background_image')) {
    $footer_background_image = file_load($footer_background_image_fid);
    $footer_background_image_url = file_create_url($footer_background_image->uri);
  }
  else {
    $footer_background_image_url = '/' . $theme_path . '/assets/images/bg/bg-footer-default.jpg';
  }
  drupal_add_css('.footer { background-image: url("' . $footer_background_image_url . '") }', array('type' => 'inline'));

  // Add copyright to theme.
  $copyright = theme_get_setting('copyright');
  $variables['copyright'] = check_markup($copyright['value'], $copyright['format']);

  // Add footer text.
  $footer_text = theme_get_setting('footer_text');
  $variables['footer_text'] = check_markup($footer_text['value'], $footer_text['format']);

  $variables['address'] = panopoly_config_get('address');
  $variables['phone'] = panopoly_config_get('phone');
  $variables['opening_hours'] = panopoly_config_get('opening_hours');
}
