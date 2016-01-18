<?php
/**
 * Returns HTML for primary and secondary local tasks.
 */
function boot_press_menu_local_tasks(&$variables) {
  $output = '';
  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] = '<ul class="nav nav-tabs">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['secondary']['#prefix'] = '<ul class="nav nav-pills">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
  return $output;
}

/**
 * Returns HTML for primary and secondary local tasks.
 */
function boot_press_menu_local_task($variables) {
  $link = $variables['element']['#link'];
  $link_text = $link['title'];
  $classes = array();

  if (!empty($variables['element']['#active'])) {
    $active = '<span class="element-invisible">' . t('(active tab)') . '</span>';
    if (empty($link['localized_options']['html'])) {
      $link['title'] = check_plain($link['title']);
    }
    $link['localized_options']['html'] = TRUE;
    $link_text = t('!local-task-title!active', array('!local-task-title' => $link['title'], '!active' => $active));
    $classes[] = 'active';
  }
  return '<li class="' . implode(' ', $classes) . '">' . l($link_text, $link['href'], $link['localized_options']) . "</li>\n";
}

/**
 * Returns HTML for Menu tree.
 */
function boot_press_menu_tree(&$variables) {
  return '<ul class="nav navbar-nav">' . $variables['tree'] . '</ul>';
}

/**
 * Returns HTML for menu link.
 */
function boot_press_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';
  
  if ($element['#below']) {
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
    } else {
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = '<ul class="dropdown-menu">' . drupal_render($element['#below']) . '</ul>';
      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
      $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';
      if ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] > 1)) {
        $element['#attributes']['class'][] = 'dropdown-submenu';
      } else {
        $element['#attributes']['class'][] = 'dropdown';
        $element['#localized_options']['html'] = TRUE;
        $element['#title'] .= ' <span class="caret"></span>';
      }
      $element['#localized_options']['attributes']['data-target'] = '#';
    }
  }
  if (($element['#href'] == $_GET['q'] || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']) || $element['#localized_options']['language']->language == $language_url->language)) {
    $element['#attributes']['class'][] = 'active';
  }
  $menu_output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $menu_output . $sub_menu . "</li>\n";
}

/**
 * Add meta tag to the page header
 * Page alter.
 */
function boot_press_page_alter($page) {
  $mobileoptimized = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
    'name' =>  'MobileOptimized',
    'content' =>  'width'
    )
  );
  $handheldfriendly = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
    'name' =>  'HandheldFriendly',
    'content' =>  'true'
    )
  );
  $viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
    'name' =>  'viewport',
    'content' =>  'width=device-width, initial-scale=1'
    )
  );
  drupal_add_html_head($mobileoptimized, 'MobileOptimized');
  drupal_add_html_head($handheldfriendly, 'HandheldFriendly');
  drupal_add_html_head($viewport, 'viewport');
}

/**
 * Preprocess variables for html.tpl.php
 * Include Font awesome
 * Add IE8 Support
 * Add Javascript for enable/disable Bootstrap 3 Javascript
 */
function boot_press_preprocess_html(&$variables) {
  drupal_add_css(path_to_theme() . '/css/ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => '(lt IE 9)', '!IE' => FALSE), 'preprocess' => FALSE));
  $directory = drupal_get_path('theme', 'boot_press');
  if (theme_get_setting('enable_jquery_appear')) {
    drupal_add_js($directory . '/js/libs/jquery.appear.js');
    drupal_add_js($directory . '/js/jquery.appear.settings.js', array('group' => JS_THEME));
    drupal_add_css($directory . '/css/animations.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  }
  if (theme_get_setting('sticky_main_navigation')) {
    drupal_add_js($directory . '/js/enable-sticky-navigation.js', array('group' => JS_THEME));
  }
  
  global $base_path; global $base_root;
  $path_to_theme = $base_root . $base_path . path_to_theme();
  if (theme_get_setting('responsive_respond', 'boot_press')) {
    $respondjs = array(
      '#tag' => 'script',
      '#attributes' => array( 
        'src' => $path_to_theme . '/js/libs/respond.min.js', 
      ),
      '#prefix' => '<!--[if lte IE 9]>',
      '#suffix' => '</script><![endif]-->',
    );
    drupal_add_html_head($respondjs, 'respond');
  }
  
  $html5 = array(
    '#tag' => 'script',
    '#attributes' => array(
      'src' => '//html5shiv.googlecode.com/svn/trunk/html5.js', 
    ),
    '#prefix' => '<!--[if lte IE 9]>',
    '#suffix' => '</script><![endif]-->',
  );
  drupal_add_html_head($html5, 'html5');
}

/**
 * Preprocess variables for page template.
 * Add bootstrap class to page content and sidebar.
 */
function boot_press_preprocess_page(&$variables) {
  if ($variables['page']['sidebar_first'] && $variables['page']['sidebar_second']) { 
    $variables['sidebar_grid_class'] = 'col-md-3';
    $variables['main_grid_class'] = 'col-md-6';
  } elseif ($variables['page']['sidebar_first'] || $variables['page']['sidebar_second']) {
    $variables['sidebar_grid_class'] = 'col-md-4';
    $variables['main_grid_class'] = 'col-md-8';		
  } else {
    $variables['main_grid_class'] = 'col-md-12';			
  }
  
  $variables['primary_nav'] = FALSE;
  if ($variables['main_menu']) {
    $variables['primary_nav'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
  }

  $variables['secondary_nav'] = FALSE;
  if ($variables['secondary_menu']) {
    $variables['secondary_nav'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__secondary');
  }
  
  /* BootStrap Message */
  if (!module_exists('bootstrap_library')) {
    $bootstrap_download_link = l('Download Bootstrap 3', 'https://github.com/twbs/bootstrap/releases/download/v3.3.1/bootstrap-3.3.1-dist.zip', array('external' => TRUE, 'attributes' => array('target' => '_blank')));
    drupal_set_message(t('!module module was not found. Boot Press requires Bootstrap Library module. Please install !module, or Boot Press may not work correctly. After install the !module you should ' .$bootstrap_download_link. ' files on /sites/all/libraries/bootstrap.',
    array(
      '!module' => l('Bootstrap Library', 'https://drupal.org/project/bootstrap_library', array('external' => TRUE, 'attributes' => array('target' => '_blank'))),
      )
    ), 'warning', FALSE);
  }
  
   /* jQuery Module Message */
    $jquery_version = variable_get('jquery_update_jquery_version', '1.5');
  if (!module_exists('jquery_update') || !version_compare($jquery_version, '1.10', '>=')) {
    drupal_set_message(t('!module module was not found, or your version of jQuery does not meet the minimum version requirement. Boot Press requires jQuery 1.10 or higher. Please install !module, or Boot Press may not work correctly.',
      array(
        '!module' => l('jQuery Update', 'https://drupal.org/project/jquery_update', array('external' => TRUE, 'attributes' => array('target' => '_blank'))),
      )
    ), 'warning', FALSE);
  }
  
  /* Font Awesome Message */
  if (!module_exists('fontawesome')) {
    $fontawesome_download_link = l('Download Font Awesome Library', 'http://fortawesome.github.io/Font-Awesome/assets/font-awesome-4.2.0.zip', array('external' => TRUE, 'attributes' => array('target' => '_blank')));
    drupal_set_message(t('!module module was not found. Boot Press requires Font Awesome Icons. Please install !module, or Boot Press may not work correctly. After install the !module you should ' .$fontawesome_download_link. ' files on /sites/all/libraries/fontawesome.',
    array(
      '!module' => l('Font Awesome Icons', 'https://www.drupal.org/project/fontawesome', array('external' => TRUE, 'attributes' => array('target' => '_blank'))),
      )
     ), 'warning', FALSE);
  }
  
  /* variable for the socialmedia links */
  $variables['boot_press_social_media_links'] = array(
    theme_get_setting('twitter') => 'fa fa-twitter',
    theme_get_setting('facebook') => 'fa fa-facebook',
    theme_get_setting('linkedin') => 'fa fa-linkedin',
    theme_get_setting('dribbble') => 'fa fa-dribbble',
    theme_get_setting('flickr') => 'fa fa-flickr',
    theme_get_setting('skype') => 'fa fa-skype',
    theme_get_setting('digg') => 'fa fa-digg',
    theme_get_setting('googleplus') => 'fa fa-google-plus',
    theme_get_setting('instagram') => 'fa fa-instagram',
    theme_get_setting('vimeo') => 'fa fa-vimeo-square',
    theme_get_setting('tumblr') => 'fa fa-tumblr',
    theme_get_setting('youtube') => 'fa fa-youtube',
    theme_get_setting('pinterest') => 'fa fa-pinterest',
    theme_get_setting('xing') => 'fa fa-xing',
    theme_get_setting('delicious') => 'fa fa-delicious',
  );
}

/**
 * Bootstrap menu theme wrapper function for the primary menu links
 */
function boot_press_menu_tree__primary(&$variables) {
  return '<ul class="nav navbar-nav">' . $variables['tree'] . '</ul>';
}

/**
 * Bootstrap menu theme wrapper function for the secondary menu links
 */
function boot_press_menu_tree__secondary(&$variables) {
  return '<ul class="nav navbar-nav pull-right">' . $variables['tree'] . '</ul>';
}


/**
 * Preprocess variables for block.tpl.php
 * Add 'clearfix' class
 */
function boot_press_preprocess_block(&$variables) {
  $variables['classes_array'][] = 'clearfix';
}

/**
 * Override theme_breadrumb().
 * Print breadcrumbs as a list, with separators.
 */
function boot_press_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    $breadcrumb[] = drupal_get_title();
    $breadcrumbs = '<ol class="breadcrumb">';

    $count = count($breadcrumb) - 1;
    foreach ($breadcrumb as $key => $value) {
      $breadcrumbs .= '<li>' . $value . '</li>';
    }
    $breadcrumbs .= '</ol>';
    return $breadcrumbs;
  }
}

/**
 * Search block form alter.
 * Customize the search block form
 */
function boot_press_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    unset($form['search_block_form']['#title']);
    $form['search_block_form']['#title_display'] = 'invisible';
    $form_default = t('Search this website...');
    $form['search_block_form']['#default_value'] = $form_default;

    $form['actions']['submit']['#attributes']['value'][] = '';
    $form['search_block_form']['#attributes'] = array('onblur' => "if (this.value == '') {this.value = '{$form_default}';}", 'onfocus' => "if (this.value == '{$form_default}') {this.value = '';}" );
  }
}

/**
 * Include theme templates
 * @$tempate_file_name handel the template file name
 * 
 */
function _boot_press_include_template($tempate_file_name) {
  $theme_path = drupal_get_path('theme', 'boot_press');
  $theme_path .= '/templates/includes/' . $tempate_file_name;
  return $theme_path;
}

/**
 * Return total comment count number.
 * @param $comments
 */
function _boot_press_comment_count($comments) {
  if ($comments) {
    $comment_cids = array();
    foreach ($comments as $comment_obj ) {
      if (!empty($comment_obj['#comment'])) {
        $comment_cids[] = $comment_obj['#comment']->cid;
      }
    }
    return (count($comment_cids));
  }
}

/**
* Return absolute theme path.
* @param $theme_name 
*/
function _boot_press_get_absolute_theme_path($theme_name) {
  $absurl = url("", array('absolute' => true)) . drupal_get_path('theme', $theme_name);
  return $absurl;
}
