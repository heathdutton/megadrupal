<?php
// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('rebuild_registry') && !defined('MAINTENANCE_MODE')) {
  // Rebuild .info data.
  system_rebuild_theme_data();
  // Rebuild theme registry.
  drupal_theme_rebuild();
}


/**
 * Maintenance page preprocessing
 */
function black_hole_preprocess_maintenance_page(&$vars) {
  if (class_exists('Database', FALSE)) {
    black_hole_preprocess_html($vars);  // set html vars (html.tpl.php is in maintenance-page.tpl.php)
    black_hole_preprocess_page($vars);  // set page vars
  }
}


/**
 * HTML preprocessing
 */
function black_hole_preprocess_html(&$vars) {
  global $theme_key, $user;

// Attributes for html element.
  $vars['html_attributes_array'] = array(
    'lang' => $vars['language']->language,
    'dir' => $vars['language']->dir,
  );

// Send X-UA-Compatible HTTP header to force IE to use the most recent rendering engine or use Chrome's frame rendering engine if available.
  if (is_null(drupal_get_http_header('X-UA-Compatible'))) {
    drupal_add_http_header('X-UA-Compatible', 'IE=edge,chrome=1');
  }

// Serialize RDF Namespaces into an RDFa 1.1 prefix attribute.
  if ($vars['rdf_namespaces']) {
    $prefixes = array();
    foreach (explode("\n  ", ltrim($vars['rdf_namespaces'])) as $namespace) {
      // Remove xlmns: and ending quote and fix prefix formatting.
      $prefixes[] = str_replace('="', ': ', substr($namespace, 6, -1));
    }
    $vars['rdf_namespaces'] = ' prefix="' . implode(' ', $prefixes) . '"';
  }

// Add to array of helpful body classes
  $vars['classes_array'][] = ($vars['is_admin']) ? 'admin' : 'not-admin';                                     // Page user is admin
  if (isset($vars['node'])) {
    $vars['classes_array'][] = ($vars['node']) ? 'full-node' : '';                                            // Full node
    $vars['classes_array'][] = (($vars['node']->type == 'forum') || (arg(0) == 'forum')) ? 'forum' : '';      // Forum page
  }
  else {
    $vars['classes_array'][] = (arg(0) == 'forum') ? 'forum' : '';                                            // Forum page
  }
  if (module_exists('panels') && function_exists('panels_get_current_page_display')) {                        // Panels page
    $vars['classes_array'][] = (panels_get_current_page_display()) ? 'panels' : '';
  }

// Add unique classes for each page and website section
  if (!$vars['is_front']) {
    $path = drupal_get_path_alias(check_plain($_GET['q']));
    list($section, ) = explode('/', $path, 2);
    $vars['classes_array'][] = ('section-' . $section);
    $vars['classes_array'][] = ('page-' . check_plain($path));
  }

// Build array of additional body classes and retrieve custom theme settings
$layoutwidth = theme_get_setting('layout-width');
  if ($layoutwidth == '0'){ 
    $vars['classes_array'][] = 'layout-jello';
  }
  if ($layoutwidth == '1'){ 
    $vars['classes_array'][] = 'layout-fluid';
  }
  if ($layoutwidth == '2'){ 
    $vars['classes_array'][] = 'layout-fixed';
  }
$sidebarslayout = theme_get_setting('sidebarslayout');
  if ($sidebarslayout == '0'){ 
    $vars['classes_array'][] = 'var';
  }
  if ($sidebarslayout == '1'){ 
    $vars['classes_array'][] = 'fix';
  }
  if ($sidebarslayout == '2'){ 
    $vars['classes_array'][] = 'var1';
  }
  if ($sidebarslayout == '3'){ 
    $vars['classes_array'][] = 'fix1';
  }
  if ($sidebarslayout == '4'){ 
    $vars['classes_array'][] = 'eq';
  }
$blockicons = theme_get_setting('blockicons');
  if ($blockicons == '1'){ 
    $vars['classes_array'][] = 'bicons32';
  }
  if ($blockicons == '2'){ 
    $vars['classes_array'][] = 'bicons48';
  }
  if ($blockicons == '3'){ 
    $vars['classes_array'][] = 'bnicons32';
  }
  if ($blockicons == '4'){ 
    $vars['classes_array'][] = 'bnicons48';
  }
$pageicons = theme_get_setting('pageicons');
  if ($pageicons == '1'){ 
    $vars['classes_array'][] = 'picons';
  }
$headerimg = theme_get_setting('headerimg');
  if ($headerimg == '1'){ 
    $vars['classes_array'][] = 'himg';
  }
$fntsize = theme_get_setting('fntsize');
  if ($fntsize == '0'){ 
	  $vars['classes_array'][] = 'fs0';
  }
  if ($fntsize == '1'){ 
	  $vars['classes_array'][] = 'fs1';
  }

// Add language and site ID classes
  $vars['classes_array'][] = ($vars['language']->language) ? 'lg-'. $vars['language']->language : '';        // Page has lang-x
$siteid = check_plain(theme_get_setting('siteid'));
  $vars['classes_array'][] = $siteid;

  $vars['classes_array'] = array_filter($vars['classes_array']);                // Remove empty elements

// Add a unique page id
  $vars['body_id'] = 'pid-' . strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', drupal_get_path_alias(check_plain($_GET['q']))));

// Set IE6 & IE7 stylesheets
  drupal_add_css(drupal_get_path('theme','black_hole').'/css/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(drupal_get_path('theme','black_hole').'/css/ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(drupal_get_path('theme','black_hole').'/css/style-bh.css', array('group' => CSS_THEME, 'every_page' => TRUE));
  drupal_add_css(drupal_get_path('theme','black_hole') . '/css/' . get_black_hole_style() . '.css', array('group' => CSS_THEME, 'every_page' => TRUE));
  drupal_add_css(drupal_get_path('theme','black_hole').'/_custom/custom-style.css', array('group' => CSS_THEME, 'every_page' => TRUE));

$roundcorners = theme_get_setting('roundcorners');
  if ($roundcorners == '1'){ 
  drupal_add_css(drupal_get_path('theme','black_hole').'/css/round.css', array('group' => CSS_THEME, 'every_page' => TRUE));
}
  drupal_add_css(drupal_get_path('theme','black_hole').'/css/print.css', array('group' => CSS_THEME, 'media' => 'print', 'every_page' => TRUE));

$devlink = theme_get_setting('devlink');
  if ($devlink == '0'){ 
	  $dvlk = 'byy';
  }
  if ($devlink == '1'){ 
	  $dvlk = 'by';
  }
  $node = menu_get_object();
  if(isset($node->type)) {
    $nt = ucfirst($node->type).' | ';
  }
  else {
    $nt='';
  }
  $vars['page_b'] = '<div class="'.$dvlk.'"><a href="http://www.radut.net">'.$nt.'by Dr. Radut</a></div>';
}


/**
 * HTML processing
 */
function black_hole_process_html(&$vars, $hook) {
// Flatten out html_attributes.
  $vars['html_attributes'] = drupal_attributes($vars['html_attributes_array']);
}


// Get css styles 
function get_black_hole_style() {
$style = theme_get_setting('style');
return $style;
}


/**
 * Page preprocessing
 */
function black_hole_preprocess_page(&$vars) {
// Hide breadcrumb on all pages
  if (theme_get_setting('breadcrumb_display') == 0) {
    $vars['breadcrumb'] = '';
  }
}


/**
 * Breadcrumb override
 */
function black_hole_breadcrumb($vars) {
  $breadcrumb = $vars['breadcrumb'];
  if (!empty($breadcrumb)) {
// Provide a navigational heading to give context for breadcrumb links to screen-reader users. Make the heading invisible with .element-invisible.
    $breadcrumb[] = drupal_get_title();
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $lastitem = sizeof($breadcrumb);
    $output .= '<ul class="breadcrumb">';
    $a=1;
    foreach($breadcrumb as $value) {
        if ($a!=$lastitem){
  $output .= '<li class="breadcrumb-'.$a.'">'. $value . t(' &raquo; ') . '</li>';
          $a++;
        }
        else {
            $output .= '<li class="breadcrumb-last">'.$value.'</li>';
        }
      }
     $output .= '</ul>';
    return $output;
  }
}


/**
 * Block preprocessing
 */
function black_hole_preprocess_block(&$vars) {
  global $theme_info, $user;
// Add regions with themed blocks to $themed_regions array and retrieve custom theme settings
$themedblocks = theme_get_setting('themedblocks');
  if ($themedblocks == '0'){ 
    $themed_regions = array('sidebar_first','sidebar_second');
  }
  if ($themedblocks == '1'){ 
    $themed_regions = array('sidebar_first','sidebar_second','user1','user2','user3','user4','user5','user6','user7','user8');
  }
  if ($themedblocks == '2'){ 
    $themed_regions = array('user1','user2','user3','user4','user5','user6','user7','user8');
  }
  if (isset($themed_regions) && is_array($themed_regions))
    $vars['themed_block'] = (in_array($vars['block']->region, $themed_regions)) ? TRUE : FALSE;
  else $vars['themed_block'] = FALSE;
}


/**
 * Node preprocessing
 */
function black_hole_preprocess_node(&$vars) {
// Build array of handy node classes
  $vars['classes_array'][] = $vars['zebra'];                                     // Node is odd or even
  $vars['classes_array'][] = (!$vars['node']->status) ? 'node-unpublished' : ''; // Node is unpublished
  $vars['classes_array'][] = ($vars['sticky']) ? 'sticky' : '';                  // Node is sticky
  $vars['classes_array'][] = ($vars['teaser']) ? 'teaser' : 'full-node';         // Node is teaser or full-node
  $vars['classes_array'][] = 'node-type-'. $vars['node']->type;                  // Node is type-x, e.g., node-type-page
// Change "Submitted by" display on all nodes, site-wide
$postedby = theme_get_setting('postedby');
  if ($postedby == '0'){ 
    $vars['submitted'] = t('!username - !datetime', array('!username' => $vars['name'], '!datetime' => $vars['date']));
  }
  if ($postedby == '1'){ 
    $vars['submitted'] = t('!username', array('!username' => $vars['name']));
  }
  if ($postedby == '2'){ 
    $vars['submitted'] = t('!datetime', array('!datetime' => $vars['date']));
    $vars['user_picture'] = '';
  }
}


/**
 * Comment preprocessing
 */
function black_hole_preprocess_comment(&$vars) {
  static $comment_odd = TRUE;                                                    // Comment is odd or even
  
// Build array of handy comment classes
  $vars['classes_array'][] = $comment_odd ? 'odd' : 'even';
  $comment_odd = !$comment_odd;
}


/**
 * Views preprocessing - Add view type class (e.g., node, teaser, list, table)
 */
function black_hole_preprocess_views_view(&$vars) {
  $vars['css_name'] = $vars['css_name'] .' view-style-'. drupal_clean_css_identifier(strtolower($vars['view']->plugin_name));
}


/**
 * Implements theme_field__field_type().
 */
function black_hole_field__taxonomy_term_reference($vars) {
  $output = '';

// Render the label, if it's not hidden.
  if (!$vars['label_hidden']) {
    $output .= '<div class="field-label">' . $vars['label'] . ': </div>';
  }

// Render the items.
  $output .= ($vars['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($vars['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $vars['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

// Render the top-level DIV.
  $output = '<div class="' . $vars['classes'] . (!in_array('clearfix', $vars['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';

  return $output;
}


/**
 * Implements RDFa_preprocess_hook().
 */
function black_hole_preprocess_username(&$vars) {
// xml:lang alone is invalid in HTML5. Use the lang attribute instead.
  if (empty($vars['attributes_array']['lang'])) {
    $vars['attributes_array']['lang'] = '';
  }
  unset($vars['attributes_array']['xml:lang']);
  unset($vars['attributes_array']['property']);
  unset($vars['attributes_array']['rel']);
}


/**
 * Social links
 */
function black_hole_social_links() {
  $social = '';
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
      'myother' => 'social_links_display_links_myother',
    );
    foreach ($displays_possible as $key => $display_possible) {
      $link_possible = $display_possible . '_link';
      if (theme_get_setting($display_possible) && $link = theme_get_setting($link_possible)) {
        $url = check_url($link);
        $nofollow = 'nofollow';
        $classes = 'sociallinks ' . $key;
        $social .= l('', $url, array('attributes' => array('rel' => $nofollow, 'class' => $classes)));
      }
    }
  }
  return $social;
}


/**
 * Other theme settings 
 */

function menupos() {
  $navpos = theme_get_setting('navpos'); // Primary & secondary links position 
    if ($navpos == '0'){ 
      return 'navleft';
  }
    if ($navpos == '1'){ 
      return 'navcenter';
  }
    if ($navpos == '2'){ 
      return 'navright';
  }
}

function black_hole_login(){
  global $user;
  $loginlinks = theme_get_setting('loginlinks');
  if ($loginlinks == '1'){ 
    if ($user->uid != 0) { 
      print '<ul class="links inline"><li class="first"><a href="' .url('user/'.$user->uid). '">' .$user->name. '</a></li><li><a href="' .url('user/logout'). '">' .t('Logout'). '</a></li></ul>'; 
    } 
    else { 
      print '<ul class="links inline"><li class="first"><a href="' .url('user'). '" rel="nofollow">' .t('Login'). '</a></li><li><a href="' .url('user/register'). '" rel="nofollow">' .t('Register'). '</a></li></ul>'; 
    }
  }
}

function divider() {
  $divider = theme_get_setting('themedblocks');
    if ($divider == '0' || $divider == '4') { 
      return 'divider';
  }
}



/**
 * CUSTOM
 */

/**
 * Use toplinks function to return links or whatever
 */

//function toplinks() {
//  return '
//<ul class="links">
//<li class="first"> LINK1 </li>
//<li> LINK2 </li>
//</ul>
//'; }
