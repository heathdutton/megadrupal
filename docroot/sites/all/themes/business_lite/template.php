<?php
include_once('theme.inc');

function business_lite_preprocess_page(&$vars) {
  // Useful for devel default banners, remove before commit
  //   variable_del('theme_marinelli_first_install');
  // Chcek if is first setup of marinelli and install banners.
  if (variable_get('theme_business_lite_first_install', TRUE)) {
    include_once('theme-settings.php');
    _business_lite_install();
  }
  //to print the banners
  $banners = business_lite_show_banners();
  $vars['banner'] = $banners;
  if(drupal_is_front_page()){
    drupal_add_js(path_to_theme() .'/js/slide.js', 'file');
    
  }
  //$vars['main_links_tree'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
  //print render ( $vars['main_links_tree']);
   if (isset($vars['main_menu'])) {
    $vars['main_menu'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('links', 'main-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['main_menu'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menu'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'secondary-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['secondary_menu'] = FALSE;
  }
}

/**
 *
 * @return <array>
 *    html markup to show banners
 */
function business_lite_show_banners() {
  $banners = business_lite_get_banners(FALSE);
  $output = ' <div id="orbitDemo">';
  
  for ($i=0;$i<count($banners);$i++) { 
    if (empty($banners[$i]['image_title'])) {
      $output .=  l('<img class="img-sl" src="' . file_create_url($banners[$i]['image_path']) . '" alt="slider image' . $i . '" />', $banners[$i]['image_url'],array('html' => TRUE));
    }
    else{
   $output .= '<div data-caption="#caption' . $i . '">' . l('<img class="img-sl" src="' . file_create_url($banners[$i]['image_path']) . '" alt="slider image' . $i . '" />', $banners[$i]['image_url'],array('html' => TRUE)) . '</div>';
   }
  } 
  $output .= '</div>';
  for($i=0;$i<count($banners);$i++){
  if (!empty($banners[$i]['image_title'])) {
    $output .= '<span class="orbit-caption" id="caption' . $i . '">' . $banners[$i]['image_title']  . '</span>';
    }
  }
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
function business_lite_get_banners($all = TRUE) { 
  // Get all banners
  $banners = variable_get('theme_business_lite_banner_settings', array());
  $delay = theme_get_setting('banner_delay');
  drupal_add_js('var delay = "'.$delay.'"', 'inline');
    $animation_speed = theme_get_setting('animation_speed');
  $caption_animation_speed = theme_get_setting('caption_animation_speed');
  drupal_add_js('var delay = "'.$delay.'"', 'inline');
   drupal_add_js('var animation_speed = "'.$animation_speed.'"', 'inline');
    drupal_add_js('var caption_animation_speed  = "'.$caption_animation_speed.'"', 'inline');
  $vars['banner_text'] = '';
  // Create list of banner to return
  $banners_value = array();
  foreach ($banners as $banner) {     
      // Add weight param to use `drupal_sort_weight`
      $banner['weight'] = $banner['image_weight'];   
      $banners_value[] = $banner;
  }
  //print_r($banner['weight']);die;
 /* $a = theme_get_setting('banner_showtext');
  if ($banners && theme_get_setting('banner_showtext')) {
    // Banner text markup
    $vars['banner_text'] = theme('mbanner_text');
  }*/
  usort($banners_value, 'drupal_sort_weight');
  return $banners_value;
}

/**
 * Set banner settings.
 *
 * @param <array> $value
 *    Settings to save
 */
function business_lite_set_banners($value) {
  variable_set('theme_business_lite_banner_settings', $value);
}

function business_lite_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
  		
  		$form['search_block_form']['#attributes']['placeholder'] = t('Search');
  		//$form['search_block_form']['#value'] = 'Search';  // define size of the textfield
  		$form['actions']['submit']['#value'] = ''; // Change the text on the submit button
  		$form['actions']['submit'] = array('#type' => 'image_button', '#src' => base_path() . path_to_theme() . '/images/magnify.png');
  		
  		/*$form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = 'Search';}";
  		$form['search_block_form']['#attributes']['onfocus'] = "if (this.value == 'Search') {this.value = '';}";*/
  }
}
/*function business_lite_preprocess_page(&$vars) {
  $vars['main_links_tree'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
}*/

/*menu*/


/*function business_lite_preprocess_page(&$vars) {
  if (isset($vars['main_menu'])) {
    $vars['main_menu'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('links', 'main-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['main_menu'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menu'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'secondary-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['secondary_menu'] = FALSE;
  }
}*/

/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function business_lite_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
  return $output;
}


