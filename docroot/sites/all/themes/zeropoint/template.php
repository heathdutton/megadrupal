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
function zeropoint_preprocess_maintenance_page(&$vars) {
  if (class_exists('Database', FALSE)) {
    zeropoint_preprocess_html($vars);  // set html vars (html.tpl.php is in maintenance-page.tpl.php)
    zeropoint_preprocess_page($vars);  // set page vars
  }
}


/**
 * HTML preprocessing
 */
function zeropoint_preprocess_html(&$vars) {
  global $theme_key, $user;

// Add variables and paths needed for HTML5 and responsive support.
  $vars['base_path'] = base_path();
  $vars['path_to_zeropoint'] = drupal_get_path('theme', 'zeropoint');

// Attributes for html element.
  $vars['html_attributes_array'] = array(
    'lang' => $vars['language']->language,
    'dir' => $vars['language']->dir,
  );

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
$blockicons = theme_get_setting('blockicons');
  if ($blockicons == '1'){
    $vars['classes_array'][] = 'bi32';
  }
  if ($blockicons == '2'){
    $vars['classes_array'][] = 'bi48';
  }
$navpos = theme_get_setting('navpos');
  if ($navpos == '0'){
    $vars['classes_array'][] = 'ml';
  }
  if ($navpos == '1'){
    $vars['classes_array'][] = 'mc';
  }
  if ($navpos == '2'){
    $vars['classes_array'][] = 'mr';
  }
$fntsize = theme_get_setting('fntsize');
  if ($fntsize == '0'){
	  $vars['classes_array'][] = 'fs0';
  }
  if ($fntsize == '1'){
	  $vars['classes_array'][] = 'fs1';
  }
if (theme_get_setting('grid_responsive') == 1 ){
$mob = theme_get_setting('mobile_blocks');
  if ($mob == '1'){
	  $vars['classes_array'][] = 'nb1';
  }
  if ($mob == '2'){
	  $vars['classes_array'][] = 'nb1 nbl';
  }
  if ($mob == '3'){
	  $vars['classes_array'][] = 'nb1 nb2';
  }
  if ($mob == '4'){
	  $vars['classes_array'][] = 'nb1 nb2 nbl';
  }
  if ($mob == '5'){
	  $vars['classes_array'][] = 'nb1 nb2 nbl nbr';
  }
}
if(theme_get_setting('roundcorners')) {
  $vars['classes_array'][] = 'rnd';
}
if(theme_get_setting('pageicons')) {
  $vars['classes_array'][] = 'pi';
}

// Add language and site ID classes
  $vars['classes_array'][] = ($vars['language']->language) ? 'lg-'. $vars['language']->language : '';        // Page has lang-x
$siteid = check_plain(theme_get_setting('siteid'));
  $vars['classes_array'][] = $siteid;

  $vars['classes_array'] = array_filter($vars['classes_array']);                // Remove empty elements

// Add a unique page id
  $vars['body_id'] = 'pid-' . strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', drupal_get_path_alias(check_plain($_GET['q']))));

// Set grids responsive stylesheets
  drupal_add_css(drupal_get_path('theme','zeropoint').'/css/drupal/drupal-system-min.css', array('group' => CSS_SYSTEM, 'every_page' => TRUE, 'weight' => -1));
  drupal_add_css(drupal_get_path('theme','zeropoint').'/css/drupal/drupal-default-min.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE, 'weight' => -1));

  if(theme_get_setting('css_zone')) {
    drupal_add_css('http://yui.yahooapis.com/pure/0.6.0/pure-min.css', array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => -2, 'preprocess' => FALSE));
  } else {
    drupal_add_css(drupal_get_path('theme','zeropoint').'/css/yui/pure-min.css', array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => -2));
  }
  if(theme_get_setting('grid_responsive') == '1') {
    if(theme_get_setting('css_zone')) {
      drupal_add_css('http://yui.yahooapis.com/pure/0.6.0/grids-responsive-old-ie-min.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 8', '!IE' => FALSE), 'every_page' => TRUE, 'weight' => -1, 'preprocess' => FALSE));
      drupal_add_css('http://yui.yahooapis.com/pure/0.6.0/grids-responsive-min.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'gt IE 8'), 'every_page' => TRUE, 'weight' => -1, 'preprocess' => FALSE));
    } else {
      drupal_add_css(drupal_get_path('theme','zeropoint').'/css/yui/grids-responsive-old-ie-min.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 8', '!IE' => FALSE), 'every_page' => TRUE, 'weight' => -1));
      drupal_add_css(drupal_get_path('theme','zeropoint').'/css/yui/grids-responsive-min.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'gt IE 8'), 'every_page' => TRUE, 'weight' => -1));
    }
  }
global $language;
$lang_dir = $language->dir;
if(theme_get_setting('headerimg')) {
  $vars['classes_array'][] = 'himg';
  if($lang_dir == 'rtl') {
    drupal_add_css(drupal_get_path('theme','zeropoint').'/_custom/headerimg-rtl/rotate.php', array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 2, 'preprocess' => FALSE));
  } else {
    drupal_add_css(drupal_get_path('theme','zeropoint').'/_custom/headerimg/rotate.php', array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 2, 'preprocess' => FALSE));
  }
}
  drupal_add_css(drupal_get_path('theme','zeropoint').'/css/style-zero.css', array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 3));
  drupal_add_css(drupal_get_path('theme','zeropoint').'/css/'.get_zeropoint_style().'.css', array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 4));
  drupal_add_css(drupal_get_path('theme','zeropoint').'/_custom/custom-style.css', array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 6));
  drupal_add_css(drupal_get_path('theme','zeropoint').'/css/print.css', array('group' => CSS_THEME, 'media' => 'print', 'every_page' => TRUE, 'weight' => 7));

// Add javascript and CSS files for jquery slideshow.
if (theme_get_setting('slideshow_display')) {
  if (drupal_is_front_page() || theme_get_setting('slideshow_all')) {
    drupal_add_css(drupal_get_path('theme','zeropoint').'/css/slider.css', array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => 5));
    drupal_add_js(drupal_get_path('theme', 'zeropoint').'/js/jquery.flexslider-min.js', array('group' => JS_THEME));
    drupal_add_js(drupal_get_path('theme', 'zeropoint').'/js/slide.js', array('group' => JS_THEME));
  }
}

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


// Get css styles
function get_zeropoint_style() {
  $style = theme_get_setting('style');
  return $style;
}


/**
 * Implements RDFa_preprocess_hook().
 */
function zeropoint_preprocess_username(&$vars) {
// xml:lang alone is invalid in HTML5. Use the lang attribute instead.
  if (empty($vars['attributes_array']['lang'])) {
    $vars['attributes_array']['lang'] = '';
  }
  unset($vars['attributes_array']['xml:lang']);
  unset($vars['attributes_array']['property']);
  unset($vars['attributes_array']['rel']);
}


/**
 * HTML processing
 */
function zeropoint_process_html(&$vars) {
// Flatten out html_attributes.
  $vars['html_attributes'] = drupal_attributes($vars['html_attributes_array']);

// Serialize RDF Namespaces into an RDFa 1.1 prefix attribute.
  if ($vars['rdf_namespaces']) {
    $prefixes = array();
    foreach (explode("\n  ", ltrim($vars['rdf_namespaces'])) as $namespace) {
      // Remove xlmns: and ending quote and fix prefix formatting.
      $prefixes[] = str_replace('="', ': ', substr($namespace, 6, -1));
    }
    $vars['rdf_namespaces'] = ' prefix="' . implode(' ', $prefixes) . '"';
  }
}


/**
 * Page preprocessing
 */
function zeropoint_preprocess_page(&$vars) {
// Hide breadcrumb on all pages
  if (theme_get_setting('breadcrumb_display') == 0) {
    $vars['breadcrumb'] = '';
  }
}


/**
 * Breadcrumb override
 */
function zeropoint_breadcrumb($vars) {
  $breadcrumb = $vars['breadcrumb'];
  if (!empty($breadcrumb)) {
// Provide a navigational heading to give context for breadcrumb links to screen-reader users. Make the heading invisible with .element-invisible.
    $breadcrumb[] = drupal_get_title();
    $output = '<div class="element-invisible">' . t('You are here') . '</div>';
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
function zeropoint_preprocess_block(&$vars) {
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
function zeropoint_preprocess_node(&$vars) {
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
function zeropoint_preprocess_comment(&$vars) {
  static $comment_odd = TRUE;                                                    // Comment is odd or even

// Build array of handy comment classes
  $vars['classes_array'][] = $comment_odd ? 'odd' : 'even';
  $comment_odd = !$comment_odd;
}


/**
 * Views preprocessing - Add view type class (e.g., node, teaser, list, table)
 */
function zeropoint_preprocess_views_view(&$vars) {
  $vars['css_name'] = $vars['css_name'] .' view-style-'. drupal_clean_css_identifier(strtolower($vars['view']->plugin_name));
}


/**
 * Implements theme_field__field_type().
 */
function zeropoint_field__taxonomy_term_reference($vars) {
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
 * Social links
 */
function social_links() {
  $social = '';
  if (theme_get_setting('social_links_display')) {
    $displays_possible = array(
      'facebook' => 'facebook',
      'googleplus' => 'googleplus',
      'twitter' => 'twitter',
      'instagram' => 'instagram',
      'pinterest' => 'pinterest',
      'linkedin' => 'linkedin',
      'youtube' => 'youtube',
      'vimeo' => 'vimeo',
      'flickr' => 'flickr',
      'tumblr' => 'tumblr',
      'skype' => 'skype',
      'myother' => 'myother',
    );
    foreach ($displays_possible as $key => $display_possible) {
      $link_possible = $display_possible . '_link';
      if (theme_get_setting($display_possible) && $link = theme_get_setting($link_possible)) {
        $url = check_url($link);
        $nofollow = 'nofollow';
        $classes = 'sociallinks ' . $key;
        $social .= l('.', $url, array('attributes' => array('class' => $classes, 'rel' => $nofollow, 'title' => $key)));
      }
    }
  }
  return $social;
}


/**
 * Pure Grid settings
 */
function wrapper_width() {
  $wrapper = check_plain(theme_get_setting('wrapper'));
    return ' style="max-width:' . $wrapper . ';"';
}

function section_class($page, $onefour=true){
  if($onefour) {
    $cols = (bool) $page['user1'] + (bool) $page['user2'] + (bool) $page['user3'] + (bool) $page['user4'];
  } else {
    $cols = (bool) $page['user5'] + (bool) $page['user6'] + (bool) $page['user7'] + (bool) $page['user8'];
  }
  if((theme_get_setting('grid_responsive') == '1') && ((preg_match('/(?i)msie [2-7]/',$_SERVER['HTTP_USER_AGENT']))) == FALSE) {
    if ($cols == '1') {
      return 'pure-u-1';
    }
    if ($cols == '2') {
      return 'pure-u-1 pure-u-sm-1-2';
    }
    if ($cols == '3') {
      return 'pure-u-1 pure-u-md-1-3';
    }
    if ($cols == '4') {
      return 'pure-u-1 pure-u-sm-1-2 pure-u-md-1-4';
    }
  } else {
      return 'pure-u-1-'.$cols;
    }
}

function first_class(){
  $w1 = (theme_get_setting('first_width'));
  if((theme_get_setting('grid_responsive') == '1') && ((preg_match('/(?i)msie [2-7]/',$_SERVER['HTTP_USER_AGENT']))) == FALSE) {
    return 'pure-u-1 pure-u-md-'.$w1.'-24';
  } else {
    return 'pure-u-'.$w1.'-24';
  }
}

function second_class(){
  $w2 = (theme_get_setting('second_width'));
  if((theme_get_setting('grid_responsive') == '1') && ((preg_match('/(?i)msie [2-7]/',$_SERVER['HTTP_USER_AGENT']))) == FALSE) {
    return 'pure-u-1 pure-u-md-'.$w2.'-24';
  } else {
    return 'pure-u-'.$w2.'-24';
  }
}

function cont_class($page){
  $cols = (bool) $page['sidebar_first'] + (bool) $page['sidebar_second'];
  $w1 = (theme_get_setting('first_width'));
  $w2 = (theme_get_setting('second_width'));
  $cont1 = 24 - $w1;
  $cont2 = 24 - $w2;
  $cont0 = 24 - ($w1+$w2);
  if (($page['sidebar_first']) && (!$page['sidebar_second'])) {
    if((theme_get_setting('grid_responsive') == '1') && ((preg_match('/(?i)msie [2-7]/',$_SERVER['HTTP_USER_AGENT']))) == FALSE) {
      return 'pure-u-1 pure-u-md-'.$cont1.'-24';
    } else {
      return 'pure-u-'.$cont1.'-24';
    }
  }
  if ((!$page['sidebar_first']) && ($page['sidebar_second'])) {
    if((theme_get_setting('grid_responsive') == '1') && ((preg_match('/(?i)msie [2-7]/',$_SERVER['HTTP_USER_AGENT']))) == FALSE) {
      return 'pure-u-1 pure-u-md-'.$cont2.'-24';
    } else {
      return 'pure-u-'.$cont2.'-24';
    }
  }
  if (($page['sidebar_first']) && ($page['sidebar_second'])) {
    if((theme_get_setting('grid_responsive') == '1') && ((preg_match('/(?i)msie [2-7]/',$_SERVER['HTTP_USER_AGENT']))) == FALSE) {
      return 'pure-u-1 pure-u-md-'.$cont0.'-24';
    } else {
      return 'pure-u-'.$cont0.'-24';
    }
  } else {
    if((theme_get_setting('grid_responsive') == '1') && ((preg_match('/(?i)msie [2-7]/',$_SERVER['HTTP_USER_AGENT']))) == FALSE) {
      return 'pure-u-1 pure-u-md-24-24';
    } else {
      return 'pure-u-24-24';
    }
  }
}

function resp_class(){
  if(theme_get_setting('grid_responsive') == '1') {
    return 'pure-u-1 pure-u-md-';
  } else {
    return 'pure-u-';
  }
}


/**
 * Add pure-img class to images to make them fit within their fluid parent wrapper while maintaining aspect ratio.
 */
function zeropoint_image($vars) {
  $attributes = $vars['attributes'];
  $attributes['src'] = file_create_url($vars['path']);

  foreach (array('width', 'height', 'alt', 'title') as $key) {
    if (isset($vars[$key])) {
      $attributes[$key] = $vars[$key];
    }
  }
  return '<img class="pure-img"' . drupal_attributes($attributes) . ' />';
}


/**
 * Returns HTML for a form element and buttons.
 */
function zeropoint_form($vars) {
  $element = $vars ['element'];
  if (isset($element ['#action'])) {
    $element ['#attributes']['action'] = drupal_strip_dangerous_protocols($element ['#action']);
  }
  //element_set_attributes($element, array('method', 'id'));
  $element ['#attributes']['class'][] = 'pure-form' . element_set_attributes($element, array('method', 'id'));
  if (empty($element ['#attributes']['accept-charset'])) {
    $element ['#attributes']['accept-charset'] = "UTF-8";
  }
  // Anonymous DIV to satisfy XHTML compliance.
  return '<form' . drupal_attributes($element ['#attributes']) . '><div>' . $element ['#children'] . '</div></form>';
}

function zeropoint_button($vars) {
  $element = $vars ['element'];
  $element ['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element ['#attributes']['class'][] = 'pure-button form-' . $element ['#button_type'];
  if (!empty($element ['#attributes']['disabled'])) {
    $element ['#attributes']['class'][] = 'pure-button pure-button-disabled form-button-disabled';
  }
  return '<input' . drupal_attributes($element ['#attributes']) . ' />';
}

function zeropoint_image_button($vars) {
  $element = $vars ['element'];
  $element ['#attributes']['type'] = 'image';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element ['#attributes']['src'] = file_create_url($element ['#src']);
  if (!empty($element ['#title'])) {
    $element ['#attributes']['alt'] = $element ['#title'];
    $element ['#attributes']['title'] = $element ['#title'];
  }

  $element ['#attributes']['class'][] = 'pure-button form-' . $element ['#button_type'];
  if (!empty($element ['#attributes']['disabled'])) {
    $element ['#attributes']['class'][] = 'pure-button pure-button-disabled form-button-disabled';
  }
  return '<input' . drupal_attributes($element ['#attributes']) . ' />';
}


/**
 * Theme's pager
 */
function zeropoint_item_list($vars){
  if(!empty($vars['attributes']['class']) && is_array($vars['attributes']['class']) && in_array('pager', $vars['attributes']['class'])){
    $vars['attributes']['class'][]='pure-paginator';
    foreach($vars['items'] as $i=>$item){
      if(!empty($item['class']) && in_array('pager-current', $item['class'])){
        $vars['items'][$i]['data']='<a href="#" class="pure-button pure-button-selected">'.$vars['items'][$i]['data'].'</a>';
      }
    }
  }
  return theme_item_list($vars);
}

function zeropoint_pager_link($vars){
  $vars['attributes']['class'][]='pure-button';

  if($vars['text']==t('« first'))
    $vars['attributes']['class'][]='prev';
  elseif($vars['text']==t('last »'))
    $vars['attributes']['class'][]='next';

  return theme_pager_link($vars);
}


/**
 * Overrides theme_menu_tree().
 */
/*
function zeropoint_menu_tree($vars) {
  return '<ul class="pure-menu-list">' . $vars['tree'] . '</ul>';
}*/


/*
* Theme's main navigation menu
*/
function zeropoint_links__system_main_menu($vars, $is_child=false){

  $vars['#attributes']['class'][] = $is_child ? 'pure-menu-children': 'pure-menu-list';
  $html = '<ul '.drupal_attributes($vars['#attributes']).'>';

  foreach($vars['links'] as $link){
    // Test for localization options and apply them if they exist.
    if (isset($link['#localized_options']['attributes']) && is_array($link['#localized_options']['attributes'])) {
      $link['#attributes'] = $link['#localized_options']['attributes'] + $link['#attributes'];
    }
    // Output html for drop-down menu.
    if(empty($link['#title']))
      continue;
    else{
      $link['#attributes']['class'][] = 'pure-menu-link';
      $link['#attributes']['class'][] = 'menu-' . $link['#original_link']['mlid'];

      if(!empty($link['#below'])){
        if(theme_get_setting('dropdown') == '1') {
          $html .= '<li class="pure-menu-item pure-menu-has-children pure-menu-allow-hover">';
        }
        if(theme_get_setting('dropdown') == '0') {
          $html .= '<li class="pure-menu-item">';
        }
        $html .= l($link['#title'], $link['#href'], array('attributes' => $link['#attributes']));
        if(theme_get_setting('dropdown') == '1') {
          $html .= zeropoint_links__system_main_menu(array('links' => $link['#below']), true);
        }
        $html .= '</li>';
      }
      else
        $html .= '<li class="pure-menu-item">'.l($link['#title'], $link['#href'], array('attributes' => $link['#attributes'])).'</li>';
    }
  }

  $html .= "</ul>\r\n";
  return $html;
}


/**
 * Other theme settings
 */
function login_links(){
  global $user;
  $loginlinks = theme_get_setting('loginlinks');
  if ($loginlinks == '1'){
    if ($user->uid != 0) {
      print '<div class="element-invisible">'.t('Login links').'</div><ul class="links inline"><li class="uin first"><a href="' .url('user/'.$user->uid). '">' .$user->name. '</a></li><li class="uout"><a href="' .url('user/logout'). '">' .t('Logout'). '</a></li></ul>';
    }
    else {
      print '<div class="element-invisible">'.t('Login links').'</div><ul class="links inline"><li class="ulog first"><a href="' .url('user'). '" rel="nofollow">' .t('Login'). '</a></li><li class="ureg"><a href="' .url('user/register'). '" rel="nofollow">' .t('Register'). '</a></li></ul>';
    }
  }
}

function divider() {
  $divider = theme_get_setting('themedblocks');
    if ($divider == '0' || $divider == '3') {
      return 'divider';
  }
}

function zeropoint_css_alter(&$css) {
  $exclude = array(
  // drupal-system.css
    'modules/system/system.base.css' => FALSE,
    'modules/system/system.menus.css' => FALSE,
    'modules/system/system.messages.css' => FALSE,
    'modules/system/system.theme.css' => FALSE,
    'modules/contextual/contextual.css' => FALSE,
  // drupal-system-rtl.css
    'modules/system/system.base-rtl.css' => FALSE,
    'modules/system/system.menus-rtl.css' => FALSE,
    'modules/system/system.messages-rtl.css' => FALSE,
    'modules/system/system.theme-rtl.css' => FALSE,
    'modules/contextual/contextual-rtl.css' => FALSE,
  // drupal-default.css
    'modules/aggregator/aggregator.css' => FALSE,
    'modules/book/book.css' => FALSE,
    'modules/comment/comment.css' => FALSE,
    'modules/field/theme/field.css' => FALSE,
    'modules/node/node.css' => FALSE,
    'modules/poll/poll.css' => FALSE,
    'modules/search/search.css' => FALSE,
    'modules/user/user.css' => FALSE,
    'modules/forum/forum.css' => FALSE,
    'modules/shortcut/shortcut.css' => FALSE,
  // drupal-default-rtl.css
    'modules/aggregator/aggregator-rtl.css' => FALSE,
    'modules/book/book.css' => FALSE,
    'modules/comment/comment-rtl.css' => FALSE,
    'modules/field/theme/field-rtl.css' => FALSE,
    'modules/node/node-rtl.css' => FALSE,
    'modules/poll/poll-rtl.css' => FALSE,
    'modules/search/search-rtl.css' => FALSE,
    'modules/user/user-rtl.css' => FALSE,
    'modules/forum/forum-rtl.css' => FALSE,
    'modules/shortcut/shortcut-rtl.css' => FALSE,

  );
  $css = array_diff_key($css, $exclude);
}
