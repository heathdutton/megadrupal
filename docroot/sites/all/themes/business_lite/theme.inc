<?php // $Id$

/**
 * Overrides of theme implementations
 */

function business_lite_theme() {
  return array(
    'custom_links' => array( // function that renders classic primary menu with <h2> 
      'variables' => array('links' => NULL, 'attributes' => NULL, 'heading' => NULL),
    ),
    'mega_menu' => array(
      'variables' => array('menu' => NULL),
    ),
    'mbanner_text' => array(
      // 'variables' => array('text' => NULL),
    ),
    'mbanner_nav'  => array(
      'variables' => array('prev' => NULL, 'next' => NULL),
    ),
  );
}



function business_lite_mbanner_text() { 
  $banner_text  =  '<div id="header-image-text" class="business_lite-hide-no-js">';
  $banner_text .=  '<div id="header-image-text-data">';
  $banner_text .=  '<'.OUTTAG.' id="header-image-title"><a href="#" class="bannerlink" title="'.t('See this content').'">title</a></'.OUTTAG.'>';
  $banner_text .=  '<p id="header-image-description"><a href="#" class="bannerlink" title="'.t('See this content').'">description</a></p>';
  $banner_text .=  '</div>';
  $banner_text .=  '</div>';

  return $banner_text;
}
/*function business_lite_banners_add_js($num_banners) {
  // in this two cases we do not need cycle, we use a small script to fill banner text
  if ($num_banners <= 1) { // banners match only url
     drupal_add_js(path_to_theme() . 'business_lite_banner_text.js');
  }
  // adds cycle passing some parameters
  else {
    // add the required javascript
    ///drupal_add_js(path_to_theme() . '/js/cycle/cycle.js');
    //drupal_add_js(path_to_theme() . '/js/banner/business_lite_configure_cycle.js');

    // pass the text variables to javascript
    drupal_add_js(
      array(
        'business_lite' => array(
          'banner_effect' => theme_get_setting('banner_effect'),
          'banner_speed'  => theme_get_setting('banner_speed'),
          'banner_delay'  => theme_get_setting('banner_delay'),
          'banner_pause'  => theme_get_setting('banner_pause') == 1 ? 1 : 0,
        )
      ),
      array('type' => 'setting')
    );
  }
}*/

