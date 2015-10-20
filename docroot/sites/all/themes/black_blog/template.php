
<?php


function black_blog_preprocess_page(&$vars) {
  // Useful for devel default banners, remove before commit
  //   variable_del('theme_marinelli_first_install');
  // Chcek if is first setup of marinelli and install banners.
// Checking selected color to switch css  
 $theme_path=path_to_theme();
 $color_setting=variable_get('theme_black_blog_color_settings');
 switch($color_setting){
    case 'red':      
      drupal_add_css($theme_path.'/css/red.css');
      break;
    case 'green':      
      drupal_add_css($theme_path.'/css/green.css');
      break;
    case 'orange':      
      drupal_add_css($theme_path.'/css/orange.css');
      break;
    case 'default':      
      break;
 }
 
  if (variable_get('theme_black_blog_first_install', TRUE)) {
    include_once('theme-settings.php');
    _black_blog_install();
  }
  //to print the banners
  $banners = black_blog_show_banners();
  $vars['banner'] = $banners;
  if(drupal_is_front_page()){
    drupal_add_js(path_to_theme() .'/js/slide.js', 'file');
    
  }
  //$vars['main_links_tree'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
  //print render ( $vars['main_links_tree']);
}

/**
 *
 * @return <array>
 *    html markup to show banners
 */
function black_blog_show_banners() {
  $banners = black_blog_get_banners(FALSE);

  $output = ' <div id="orbitDemo">';
  
  for($i=0;$i<count($banners);$i++){
    if (empty($banners[$i]['image_title'])) {
      $output .=  l('<img class="img-sl" src="' . file_create_url($banners[$i]['image_path']) . '" alt="slider image' . $i . '" />', $banners[$i]['image_url'],array('html' => TRUE));
    }
    else{
   $output .= '<div data-caption="#caption' . $i . '">' . l('<img class="img-sl" src="' . file_create_url($banners[$i]['image_path']) . '" alt="slider image' . $i . '" />', $banners[$i]['image_url'],array('html' => TRUE)) . '</div>';
   }
  }  
  $output .= '</div>';
  for($i=0;$i<count($banners);$i++){
    $output .= '<span class="orbit-caption" id="caption' . $i . '">' . $banners[$i]['image_title']  . '</span>';
  }
  //print l('sdfsd','dfsdf');
  return $output;
}


/**
 * Get banner settings.
 *
 * @param <bool> $all
 *    Return all banners or only active.
 *
 * @return <array>
 *    Settings information
 */
function black_blog_get_banners($all = TRUE) {
  // Get all banners
  $banners = variable_get('theme_black_blog_banner_settings', array());
  $delay = theme_get_setting('banner_delay');
  $animation_speed = theme_get_setting('animation_speed');
  $caption_animation_speed = theme_get_setting('caption_animation_speed');
  drupal_add_js('var delay = "'.$delay.'"', 'inline');
   drupal_add_js('var animation_speed = "'.$animation_speed.'"', 'inline');
    drupal_add_js('var caption_animation_speed  = "'.$caption_animation_speed.'"', 'inline');
  // Create list of banner to return
  $banners_value = array();
  foreach ($banners as $banner) {   
  $banner['weight'] = $banner['image_weight'];  
      $banners_value[] = $banner;   
  }

  // Sort image by weigh
  usort($banners_value, 'drupal_sort_weight');

  return $banners_value;
}

/**
 * Set banner settings.
 *
 * @param <array> $value
 *    Settings to save
 */
function black_blog_set_banners($value) {
  variable_set('theme_black_blog_banner_settings', $value);

}

function black_blog_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
  		
  		$form['search_block_form']['#attributes']['placeholder'] = t('Search');
  		//$form['search_block_form']['#value'] = 'Search';  // define size of the textfield
  		$form['actions']['submit']['#value'] = ''; // Change the text on the submit button
  		$form['actions']['submit'] = array('#type' => 'image_button', '#src' => base_path() . path_to_theme() . '/images/search_icon.png');
  		
  		/*$form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = 'Search';}";
  		$form['search_block_form']['#attributes']['onfocus'] = "if (this.value == 'Search') {this.value = '';}";*/
  }
}

/*function black_blog_preprocess_page(&$vars) {
  $vars['main_links_tree'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
}*/
function black_blog_preprocess_node(&$variables) {
$variables['thumb'] = '';
global $base_url;
if(!empty($variables['node']->field_image)) {
 	        $filename = $variables['node']->field_image['und'][0]['uri'];
  	      $newimage= image_style_url('medium', $filename);
  	      $variables['thumb'] = $newimage;
     }
      $variables['display_submitted']=FALSE;
     
}
function black_blog_preprocess_comment(&$variables) {
  $variables['comment_created']   = $variables['comment']->created;
  $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
  $variables['default_photo'] = '<img src="' . base_path( ) . $theme_path . '/images/user.jpg" />';
}

