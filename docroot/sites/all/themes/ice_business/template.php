<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
*/
function icebusiness_preprocess_node(&$vars, $hook) {
 // Add $unpublished variable.
  $vars['unpublished'] = (!$vars['status']) ? TRUE : FALSE;

  // Add pubdate to submitted variable.
  $vars['pubdate'] = '<time pubdate datetime="' . format_date($vars['node']->created, 'custom', 'c') . '">' . $vars['date'] . '</time>';
  if ($vars['display_submitted']) {
    $vars['submitted'] = t('Submitted by !username on !datetime', array('!username' => $vars['name'], '!datetime' => $vars['pubdate']));
  }

  // Add a class for the view mode.
  if (!$vars['teaser']) {
    $vars['classes_array'][] = 'view-mode-' . $vars['view_mode'];
  }

  // Add a class to show node is authored by current user.
  if ($vars['uid'] && $vars['uid'] == $GLOBALS['user']->uid) {
    $vars['classes_array'][] = 'node-by-viewer';
  }

  $vars['title_attributes_array']['class'][] = 'node-title';
 if(!strstr($vars['title'],'nice-menu')){
  $new_title = '';

  $titles=explode(" ", $vars['title']);
    foreach ($titles as $k=>$v){
      if(($k%2)){
          $new_title.='<span class="blue">'.$v.'</span>&nbsp;';
      }else{
          $new_title.='<span class="black">'.$v.'</span>&nbsp;';
      }
      
    }
  $vars['title']=trim($new_title);
 }
}


/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function icebusiness_preprocess_block(&$vars, $hook) {

 //print '<pre>';print_r($vars['block']);print'</pre>';exit;


  // Use a template with no wrapper for the page's main content.
  if ($vars['block_html_id'] == 'block-system-main') {
    $vars['theme_hook_suggestions'][] = 'block__no_wrapper';
  }

  // Classes describing the position of the block within the region.
  if ($vars['block_id'] == 1) {
    $vars['classes_array'][] = 'first';
  }
  // The last_in_region property is set in icebusiness_page_alter().
  if (isset($vars['block']->last_in_region)) {
    $vars['classes_array'][] = 'last';
  }
  $vars['classes_array'][] = $vars['block_zebra'];

  $vars['title_attributes_array']['class'][] = 'block-title';
$vars['name']='';

if(!empty($vars['block']->subject)) {
$titles=explode(" ", $vars['block']->subject);
$vars['title'] = $new_title = '';
if(isset($titles) && count($titles)>1){
  //print '<pre>';print_r($vars['block']->title);print'</pre>';exit;
  foreach ($titles as $k=>$v){
    if(($k%2)){
        $new_title.='<span class="blue">'.$v.'</span>&nbsp;';
    }else{
        $new_title.='<span class="black">'.$v.'</span>&nbsp;';
    }
    
  }
 $vars['title'] = $vars['block']->subject = trim($new_title);
 }
}


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

}


/**
 * Override or insert variables into the html template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered. This is usually "html", but can
 *   also be "maintenance_page" since icebusiness_preprocess_maintenance_page() calls
 *   this function to have consistent variables.
 */
function icebusiness_preprocess_html(&$variables, $hook) {
  // Add variables and paths needed for HTML5 and responsive support.
  $variables['base_path'] = base_path();
  $variables['path_to_icebusiness'] = drupal_get_path('theme', 'icebusiness');
  $html5_respond_meta = theme_get_setting('ice_html5_respond_meta');
  $variables['add_responsive_meta']='respond';
  if(is_array($html5_respond_meta)){
  $variables['add_responsive_meta'] = in_array('meta', $html5_respond_meta);
  }
   // Attributes for html element.
  $variables['html_attributes_array'] = array(
    'lang' => $variables['language']->language,
    'dir' => $variables['language']->dir,
  );

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  if (!$variables['is_front'] && $hook == 'html') {
    // Add unique class for each page.
    $path = drupal_get_path_alias($_GET['q']);
    // Add unique class for each website section.
    list($section, ) = explode('/', $path, 2);
    $arg = explode('/', $_GET['q']);
    if ($arg[0] == 'node') {
      if ($arg[1] == 'add') {
        $section = 'node-add';
      }
      elseif (isset($arg[2]) && is_numeric($arg[1]) && ($arg[2] == 'edit' || $arg[2] == 'delete')) {
        $section = 'node-' . $arg[2];
      }
    }
    $variables['classes_array'][] = drupal_html_class('section-' . $section);
  }
 
  // Store the menu item since it has some useful information.
  if ($hook == 'html') {
    $variables['menu_item'] = menu_get_item();
    if ($variables['menu_item']) {
      switch ($variables['menu_item']['page_callback']) {
        case 'views_page':
          // Is this a Views page?
          $variables['classes_array'][] = 'page-views';
          break;
        case 'page_manager_page_execute':
        case 'page_manager_node_view':
        case 'page_manager_contact_site':
          // Is this a Panels page?
          $variables['classes_array'][] = 'page-panels';
          break;
      }
    }
  }

}

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
function icebusiness_process_html(&$variables, $hook) {
  // Flatten out html_attributes.
  $variables['html_attributes'] = drupal_attributes($variables['html_attributes_array']);
}

/**
 * Override or insert variables in the html_tag theme function.
 */
function icebusiness_process_html_tag(&$variables) {
  $tag = &$variables['element'];

  if ($tag['#tag'] == 'style' || $tag['#tag'] == 'script') {
    // Remove redundant type attribute and CDATA comments.
    unset($tag['#attributes']['type'], $tag['#value_prefix'], $tag['#value_suffix']);

    // Remove media="all" but leave others unaffected.
    if (isset($tag['#attributes']['media']) && $tag['#attributes']['media'] === 'all') {
      unset($tag['#attributes']['media']);
    }
  }
}

/**
 * Implement hook_html_head_alter().
 */
function icebusiness_html_head_alter(&$head) {
  // Simplify the meta tag for character encoding.
  $head['system_meta_content_type']['#attributes'] = array('charset' => str_replace('text/html; charset=', '', $head['system_meta_content_type']['#attributes']['content']));
}

/*
 *  Preprocess page.tpl.php to inject the $search_box variable back into D7.
 */

function icebusiness_preprocess_page(&$variables) {
  // Get the entire main menu tree
  $main_menu_tree = menu_tree_all_data('main-menu');
$variables['name']=''; 
  // Add the rendered output to the $main_menu_expanded variable
  $main_menu_tree = menu_tree_all_data_with_active_trail('main-menu');
  $variables['main_menu_expanded'] = menu_tree_output($main_menu_tree);
  $variables['main_menu_tree'] = $main_menu_tree;
}

function menu_tree_all_data_with_active_trail($menu_name) {
function menu_tree_active_trail(&$tree_data, $menu_item) {
  foreach($tree_data as &$item) {
    if(!empty($item['link']) && (($item['link']['href'] == $menu_item['href']) || ($item['link']['href']=='<front>' && drupal_is_front_page()) )  && (empty($item['link']['language']) || $item['link']['language']->language == $language_url->language)) {
      $item['link']['in_active_trail'] = TRUE;
      return TRUE;
    } elseif(!empty($item['below'])) {
      if(menu_tree_active_trail($item['below'], $menu_item)) {
      $item['link']['in_active_trail'] = TRUE;
      return TRUE;
      }
    }
  }
return FALSE;
}

$menu_item = menu_get_item();
$menu_tree = menu_tree_all_data($menu_name);
menu_tree_active_trail($menu_tree, $menu_item);

return $menu_tree;
}








function _menu_tree_output($tree){
$output = '';
foreach($tree as $k=>$link){
  $links=array($k=>$link);
   $output .= theme("links",array('links'=>$link));
}
//print '<pre>';print_r($output); print '</pre>';exit;
return $output;
}
/**
 * Implements hook_links().
 */
/*function icebusiness_links($variables) {
  global $language_url;
//print '<pre>';print_r($vars); print '</pre>';exit;
  $links = $vars['links'];
  $attributes = $vars['attributes'];
  $heading = $vars['heading'];
  $output = '';

  if (count($links) > 0) {
    // Treat the heading first if it is present to prepend it to the list of links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading is a string.
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
if(!empty($links)){
   

    $num_links = count($links);
    $i = 1;
//print '<pre>';print_r($links); print '</pre>';//exit;
    foreach ($links as $key => $lk) {
     if(isset($lk['link'])) {
    $link = $lk['link'];
      $link['below'] = $lk['below'];
      $class = array($key);
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
      
      // Add a class for has a sub-menu
      if(isset($link['below'])) {
        $class[] = 'expanded';
      }
      
      $output .= '<li ' . drupal_attributes(array('class' => $class)) . '>';

      if (isset($link['href'])) {
          // Pass in $link as $options, they share the same keys.
        $output .= l(trim($link['title']), $link['href']);
      }elseif (!empty($link['title'])) {
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

      // Theme in nested links in the UL
      if(isset($link['below'])) { 
        $output .= theme('links', array(
          'links' => $link['below'],
          'attributes' => array(
            'class' => array('child'),
          ),
        ));
      }
      
      $i++;
      $output .= "</li>";
  }
    }
  
    
  }//if
  $output .= '</ul>';
  }

  return $output;
}
*/

