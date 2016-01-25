<?php
/**
 * @file
 */

/**
 * Adds a default set of helper variables for variable processors and templates.
 */
function p6_base_preprocess(&$vars, $hook) {
  if (($hook == 'html') || ($hook == 'page')) {
    global $theme_key;

    $vars['base_path'] = base_path();
    // Path to the base theme.
    $vars['base_theme_path'] = drupal_get_path('theme', 'p6_base');
    // Path to the active theme.
    $vars['active_theme_path'] = drupal_get_path('theme', $theme_key);

    $vars['user_roles_array'] = _p6_base_user_roles();

    foreach ($vars['user_roles_array'] as $role) {
      // Add role names as classes for the logged-in user.
      $vars['classes_array'][] = _p6_base_is_variable($role);
      // Add role names as $variables for the logged-in user.
      $vars[_p6_base_is_variable($role)] = TRUE;
    }

    // Path, Level & Section logic:
    // ==
    // i.e. http://example.com/
    // level=1 : no classes for front page.
    // --
    // i.e. http://example.com/somepage
    // level=2 : .level-2, .level-2-somepage
    // --
    // i.e. http://example.com/somepage/anotherpage
    // level=3 : .level-2, .level-2-somepage, .level-3, .level-3-anotherpage
    //
    $vars['is_level_2'] = FALSE;
    $vars['is_level_3'] = FALSE;
    $vars['is_level_4'] = FALSE;
    $vars['is_level_5'] = FALSE;

    $path = drupal_get_path_alias();
    if ($path) {
      if (!drupal_is_front_page()) {
        $level = explode('/', $path);
        $path_request = explode('/', request_uri());

        $i = 2;
        $section_last = end($level);
        foreach ($level as $section) {
          // Body classes.
          $vars['classes_array'][] = drupal_html_class('level-' . $i . '-' . $section);
          if ($section_last == $section) {
            $vars['classes_array'][] = drupal_html_class('level-' . $i);
            $vars['classes_array'][] = drupal_html_class('level-last-' . $section);
          }
          // Variables for until 5th level.
          if (($i > 1) && ($i < 6)) {
            $vars[_p6_base_is_variable('level-' . $i)] = TRUE;
          }
          $i++;
        }
      }
    }
  }
}

/**
 * Preprocess variables for html.tpl.php
 */
function p6_base_preprocess_html(&$vars) {
  // Generic settings.
  $setting_common = theme_get_setting('p6_base_common');
  if (is_array($setting_common)) {
    // Enable Modernizr for detecting HTML5 and CSS3 features in the user's browser.
    if (in_array('modernizr', $setting_common)) {
      drupal_add_js(
        drupal_get_path('theme', 'p6_base') . '/js/modernizr-2.6.1.min.js',
        array(
          'scope' => 'header',
          'group' => JS_LIBRARY,
          'weight' => -21,
        )
      );
    }
    // Display old browser, upgrade message.
    if (in_array('chromeframe', $setting_common)) {
      $vars['page']['page_top']['chrome_frame'] = array(
        '#weight' => -10,
        '#markup' => '<!--[if lt IE 7]><p class="chromeframe">' .
                     t('You are using an outdated browser. <a href="@upgrade">Upgrade your browser today</a> or <a href="@googleframe">install Google Chrome Frame</a> to better experience this site.', array('@upgrade' => 'http://browsehappy.com/', '@googleframe' => 'http://www.google.com/chromeframe/?redirect=true')) .
                     '</p><![endif]-->',
      );
    }
    // Hide breadcrumb.
    if (in_array('breadcrumb', $setting_common)) {
      // $vars['breadcrumb'] = NULL;
    }
  }

  // Debugging related settings.
  $setting_debug = theme_get_setting('p6_base_debug');
  if (is_array($setting_debug)) {
    // Firebug Lite.
    if (in_array('firebuglite', $setting_debug)) {
      drupal_add_js('https://getfirebug.com/firebug-lite.js#saveCookies=true', array(
        'type' => 'external',
        'scope' => 'header',
        'group' => JS_LIBRARY,
      ));
    }
    // Window size.
    if (in_array('windowsize', $setting_debug)) {
      drupal_add_js(
        drupal_get_path('theme', 'p6_base') . '/js/window_size.js',
        array(
          'group' => JS_THEME,
          'weight' => 100,
          'preprocess' => FALSE,
        )
      );
    }
  }

  // Mobile browser related settings.
  $setting_mobile = theme_get_setting('p6_base_mobile');
  if (is_array($setting_mobile)) {
    if (in_array('meta_handheld', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'meta',
          '#attributes' => array(
            'name' => 'HandheldFriendly',
            'content' => 'True',
          ),
        ),
        'p6_base_meta_handheld'
      );
    }
    if (in_array('meta_mobile', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'meta',
          '#attributes' => array(
            'name' => 'MobileOptimized',
            'content' => '320',
          ),
          '#weight' => 20,
        ),
        'p6_base_meta_mobile'
      );
    }
    if (in_array('meta_viewport', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'meta',
          '#attributes' => array(
            'name' => 'viewport',
            'content' => 'width=device-width',
          ),
          '#weight' => 21,
        ),
        'p6_base_meta_viewport'
      );
    }
    if (in_array('meta_cleartype', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'meta',
          '#attributes' => array(
            'http-equiv' => 'cleartype',
            'content' => 'on',
          ),
          '#weight' => 22,
        ),
        'p6_base_meta_cleartype'
      );
    }
    if (in_array('ios_web_app', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'meta',
          '#attributes' => array(
            'name' => 'apple-mobile-web-app-capable',
            'content' => 'yes',
          ),
          '#weight' => 23,
        ),
        'p6_base_ios_web_app_capable'
      );
      drupal_add_html_head(
        array(
          '#tag' => 'meta',
          '#attributes' => array(
            'name' => 'apple-mobile-web-app-status-bar-style',
            'content' => 'black',
          ),
          '#weight' => 24,
        ),
        'p6_base_ios_web_app_statusbar'
      );
      drupal_add_js(
        drupal_get_path('theme', 'p6_base') . '/js/stay_standalone.js',
        array(
          'scope' => 'header',
          'group' => JS_LIBRARY,
          'weight' => -22,
        )
      );
    }
    // 320x460 for iPhone 3GS.
    if (in_array('appletouch_startup_iphone', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'link',
          '#attributes' => array(
            'rel' => 'apple-touch-startup-image',
            'media' => '(max-device-width: 480px) and not (-webkit-min-device-pixel-ratio: 2)',
            'href' => $vars['base_path'] . $vars['active_theme_path'] . '/images/startup.png',
          ),
          '#weight' => 25,
        ),
        'p6_base_link_startup_iphone'
      );
    }
    // 640x920 for retina display.
    if (in_array('appletouch_startup_retina', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'link',
          '#attributes' => array(
            'rel' => 'apple-touch-startup-image',
            'media' => '(max-device-width: 480px) and (-webkit-min-device-pixel-ratio: 2)',
            'href' => $vars['active_theme_path'] . '/images/startup-retina.png',
          ),
          '#weight' => 26,
        ),
        'p6_base_link_startup_retina'
      );
    }
    // iPad Portrait 768x1004.
    if (in_array('appletouch_startup_ipad_portrait', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'link',
          '#attributes' => array(
            'rel' => 'apple-touch-startup-image',
            'media' => '(min-device-width: 768px) and (orientation: portrait)',
            'href' => $vars['base_path'] . $vars['active_theme_path'] . '/images/startup-tablet-portrait.png',
          ),
          '#weight' => 27,
        ),
        'p6_base_link_startup_ipad_portrait'
      );
    }
    // iPad Landscape 1024x748.
    if (in_array('appletouch_startup_ipad_landscape', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'link',
          '#attributes' => array(
            'rel' => 'apple-touch-startup-image',
            'media' => '(min-device-width: 768px) and (orientation: landscape)',
            'href' => $vars['base_path'] . $vars['active_theme_path'] . '/images/startup-tablet-landscape.png',
          ),
          '#weight' => 28,
        ),
        'p6_base_link_startup_ipad_landscape'
      );
    }
    // For third-generation iPad with high-resolution Retina display.
    if (in_array('appletouch_icon_144', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'link',
          '#attributes' => array(
            'rel' => 'apple-touch-icon-precomposed',
            'sizes' => '144x144',
            'href' => $vars['base_path'] . $vars['active_theme_path'] . '/images/apple-touch-icon-144x144-precomposed.png',
          ),
          '#weight' => 29,
        ),
        'p6_base_link_icon_144'
      );
    }
    // For iPhone with high-resolution Retina display.
    if (in_array('appletouch_icon_114', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'link',
          '#attributes' => array(
            'rel' => 'apple-touch-icon-precomposed',
            'sizes' => '114x114',
            'href' => $vars['base_path'] . $vars['active_theme_path'] . '/images/apple-touch-icon-114x114-precomposed.png',
          ),
          '#weight' => 30,
        ),
        'p6_base_link_icon_114'
      );
    }
    // For first- and second-generation iPad.
    if (in_array('appletouch_icon_72', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'link',
          '#attributes' => array(
            'rel' => 'apple-touch-icon-precomposed',
            'sizes' => '72x72',
            'href' => $vars['base_path'] . $vars['active_theme_path'] . '/images/apple-touch-icon-72x72-precomposed.png',
          ),
          '#weight' => 31,
        ),
        'p6_base_link_icon_72'
      );
    }
    // For non-Retina iPhone, iPod Touch, and Android 2.1+ devices.
    if (in_array('appletouch_icon', $setting_mobile)) {
      drupal_add_html_head(
        array(
          '#tag' => 'link',
          '#attributes' => array(
            'rel' => 'apple-touch-icon-precomposed',
            'href' => $vars['base_path'] . $vars['active_theme_path'] . '/images/apple-touch-icon-precomposed.png',
          ),
          '#weight' => 32,
        ),
        'p6_base_link_icon'
      );
    }
  }

  // Send X-UA-Compatible HTTP header to force IE to use the most recent
  // rendering engine or use Chrome's frame rendering engine if available.
  // This also prevents the IE compatibility mode button to appear when using
  // conditional classes on the html tag.
  if (is_null(drupal_get_http_header('X-UA-Compatible'))) {
    drupal_add_http_header('X-UA-Compatible', 'IE=edge,chrome=1');
  }

  // BODY id.
  $vars['attributes_array']['id'] = drupal_html_id('page-' . str_replace('/', '-', drupal_get_normal_path($path = $_GET['q'])));
}

/**
 * Preprocess variables for page.tpl.php
 */
function p6_base_preprocess_page(&$vars) {
  // Set better page titles for "User Account" pages.
  if (arg(0) === 'user') {
    if (arg(1) === 'login' || arg(1) == '') {
      drupal_set_title(t('User login'));
    }
    if (arg(1) === 'password') {
      drupal_set_title(t('Request new password'));
    }
    if (arg(1) === 'register') {
      drupal_set_title(t('Create new account'));
    }
  }
}

/**
 * Process variables for page.tpl.php
 */
function p6_base_process_page(&$vars) {
  // Generic settings.
  $setting_common = theme_get_setting('p6_base_common');
  if (is_array($setting_common)) {
    // Hide breadcrumb.
    if (in_array('breadcrumb', $setting_common)) {
      $vars['breadcrumb'] = NULL;
    }
  }

}

/**
 * Maintenance page preprocessing
 */
// function p6_base_preprocess_maintenance_page(&$vars) {
//   p6_base_preprocess_page($vars);
// }

/**
 * Process variables for node.tpl.php
 *
 * @see node.tpl.php
 */
function p6_base_preprocess_node(&$vars) {
  // Add $unpublished variable.
  $vars['unpublished'] = (!$vars['status']) ? TRUE : FALSE;

  // Add a class for the view mode.
  $vars['classes_array'][] = 'node-' . $vars['view_mode'];

  $vars['title_attributes_array']['class'][] = 'node-title';

  // Add pubdate to submitted variable.
  $vars['pubdate'] = '<time pubdate datetime="' . format_date($vars['node']->created, 'custom', 'c') . '">' . format_date($vars['node']->created, 'custom', 'F j, Y') . '</time>';
  if ($vars['display_submitted']) {
    $vars['submitted'] = t('by !username on !datetime', array('!username' => $vars['name'], '!datetime' => $vars['pubdate']));
  }

  $vars['permalink'] = l('¶', 'node/' . $vars['node']->nid, array('fragment' => 'node-' . $vars['node']->nid, 'attributes' => array('class' => array('permalink'), 'title' => t('Permalink to this headline'))));

  // More link.
  // @see p6_base_more_link
  // @see views-more.tpl.php
  $node_title_stripped = strip_tags($vars['node']->title);
  if (isset($vars['content']['links']['node']['#links']['node-readmore'])) {
    $vars['content']['links']['node']['#links']['node-readmore']['title'] = t('<span class="element-invisible">Read </span><em>more</em><span class="element-invisible"> about @title</span>', array('@title' => $node_title_stripped));
    $vars['content']['links']['node']['#links']['node-readmore']['attributes']['class'][] = 'more';
    $vars['content']['links']['#attributes']['class'] = array('links', 'clearfix');
  }
}

/**
 * Implements theme_more_link().
 */
function p6_base_more_link($variables) {
  return '<div class="more-link">' . l('<em>' . t('More') . '</em>', $variables['url'], array('html' => TRUE, 'attributes' => array('title' => $variables['title'], 'class' => 'more'))) . '</div>';
}

/**
 * Processes variables for block.tpl.php.
 *
 * @see block.tpl.php
 */
function p6_base_preprocess_block(&$vars) {
  // Use a template with no wrapper for the page's main content.
  if ($vars['block_html_id'] == 'block-system-main') {
    $vars['theme_hook_suggestions'][] = 'block__no_wrapper';
  }
  // Classes describing the position of the block within the region.
  if ($vars['block_id'] == 1) {
    $vars['classes_array'][] = 'first';
  }
  // The last_in_region property is set in zen_page_alter().
  if (isset($vars['block']->last_in_region)) {
    $vars['classes_array'][] = 'last';
  }
  $vars['classes_array'][] = $vars['block_zebra'];
  $vars['title_attributes_array']['class'][] = 'block-title';
  // Add Aria Roles via attributes.
  switch ($vars['block']->module) {
    case 'system':
      switch ($vars['block']->delta) {
        case 'main':
          // Note: the "main" role goes in the page.tpl, not here.
          break;
        case 'help':
        case 'powered-by':
          $vars['attributes_array']['role'] = 'complementary';
          break;
        default:
          // Any other "system" block is a menu block.
          $vars['attributes_array']['role'] = 'navigation';
          break;
      }
      break;
    case 'menu':
    case 'menu_block':
    case 'superfish':
    case 'blog':
    case 'book':
    case 'comment':
    case 'forum':
    case 'shortcut':
    case 'statistics':
      $vars['attributes_array']['role'] = 'navigation';
      break;
    case 'search':
      $vars['attributes_array']['role'] = 'search';
      break;
    case 'help':
    case 'aggregator':
    case 'locale':
    case 'poll':
    case 'profile':
      $vars['attributes_array']['role'] = 'complementary';
      break;
    case 'node':
      switch ($vars['block']->delta) {
        case 'syndicate':
          $vars['attributes_array']['role'] = 'complementary';
          break;
        case 'recent':
          $vars['attributes_array']['role'] = 'navigation';
          break;
      }
      break;
    case 'user':
      switch ($vars['block']->delta) {
        case 'login':
          $vars['attributes_array']['role'] = 'form';
          break;
        case 'new':
        case 'online':
          $vars['attributes_array']['role'] = 'complementary';
          break;
      }
      break;
  }

  $vars['block_content_tag'] = 'div';
  // if (($vars['block']->module == 'menu') || ($vars['block']->module == 'menu_block')) {
  if (isset($vars['attributes_array']['role']) && ('navigation' == $vars['attributes_array']['role']) ){
    $vars['block_content_tag'] = 'nav';
    $vars['title_attributes_array']['class'][] = 'element-invisible';
  }
}

/**
 * Process variables for block.tpl.php.
 *
 * @see block.tpl.php
 */
function p6_base_process_block(&$vars) {
  $vars['title'] = $vars['block']->subject;
}

/**
 * Implements hook_page_alter().
 *
 * Look for the last block in the region. This is impossible to determine from
 * within a preprocess_block function.
 *
 * @param $page
 *   Nested array of renderable elements that make up the page.
 */
function p6_base_page_alter(&$page) {
  // Look in each visible region for blocks.
  foreach (system_region_list($GLOBALS['theme'], REGIONS_VISIBLE) as $region => $name) {
    if (!empty($page[$region])) {
      // Find the last block in the region.
      $blocks = array_reverse(element_children($page[$region]));
      while ($blocks && !isset($page[$region][$blocks[0]]['#block'])) {
        array_shift($blocks);
      }
      if ($blocks) {
        $page[$region][$blocks[0]]['#block']->last_in_region = TRUE;
      }
    }
  }
}

/**
 * Returns HTML for a breadcrumb trail.
 *
 * @param $variables
 *   An associative array containing:
 *   - breadcrumb: An array containing the breadcrumb links.
 */
function p6_base_breadcrumb($vars) {
  if (!empty($vars['breadcrumb'])) {
    $vars['breadcrumb'][] = drupal_get_title();
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    $output .= '<nav class="breadcrumb">' . implode('<span class="divider">/</span>', $vars['breadcrumb']) . '</nav>';
    return $output;
  }
}

/**
 * Retrieve an array of roles excluding Drupal core's for the logged-in user.
 *
 * @return array
 */
function _p6_base_user_roles() {
  global $user;

  $user_roles = $user->roles;
  $all_roles  = user_roles(FALSE);

  // Remove core roles from the list.
  unset($user_roles[1]);
  unset($user_roles[2]);

  $user_roles_array = array();

  foreach (array_intersect($all_roles, $user_roles) as $role_name) {
    $user_roles_array[] = $role_name;
  }

  return $user_roles_array;
}

/**
 * Formats strings compatible to be used as PHP variables.
 *
 * @return string
 */
function _p6_base_is_variable($var = NULL) {
  return @str_replace('-', '_', 'is_' . drupal_html_class($var));
}
