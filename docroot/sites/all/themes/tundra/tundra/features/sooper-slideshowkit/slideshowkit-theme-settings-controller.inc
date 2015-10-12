<?php
if (theme_get_setting('flexslider_enable')) {
  drupal_add_js(array('slideshowKit' => array('flexInvoke' => theme_get_setting('flexslider_invoke'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexAnimation' => theme_get_setting('flexslider_animation'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexTimeout' => theme_get_setting('flexslider_timeout'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexSpeed' => theme_get_setting('flexslider_speed'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexRandom' => theme_get_setting('flexslider_random'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexPause' => theme_get_setting('flexslider_pause'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexShowPager' => theme_get_setting('flexslider_showPager'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexPrevNext' => theme_get_setting('flexslider_prevnext'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexItemWidth' => theme_get_setting('flexslider_itemwidth'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexItemMargin' => theme_get_setting('flexslider_itemmargin'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexMinItems' => theme_get_setting('flexslider_minitems'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexMaxItems' => theme_get_setting('flexslider_maxitems'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('flexMove' => theme_get_setting('flexslider_move'))), 'setting');
  if (is_file($tundra_theme_path . '/features/sooper-slideshowkit/jquery.flexslider-min.js')) {
    drupal_add_js($tundra_theme_path . '/features/sooper-slideshowkit/jquery.flexslider-min.js');
  }
  if (is_file($tundra_theme_path . '/features/sooper-slideshowkit/invoke-flexslider.js')) {
    drupal_add_js($tundra_theme_path . '/features/sooper-slideshowkit/invoke-flexslider.js');
  }
}
if (theme_get_setting('slideshowkit_enable')) {
  drupal_add_js(array('slideshowKit' => array('cycleInvoke' => theme_get_setting('cycle_invoke'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cycleFx' => theme_get_setting('cycle_fx'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cycleTimeout' => theme_get_setting('cycle_timeout'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cycleContinuous' => theme_get_setting('cycle_continuous'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cycleSpeed' => theme_get_setting('cycle_speed'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cycleEasing' => theme_get_setting('cycle_easing'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cycleRandom' => theme_get_setting('cycle_random'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cyclePause' => theme_get_setting('cycle_pause'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cycleShowPager' => theme_get_setting('cycle_showPager'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cyclePagerEvent' => theme_get_setting('cycle_pagerEvent'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cyclePauseOnPagerHover' => theme_get_setting('cycle_pauseOnPagerHover'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cycleDelay' => theme_get_setting('cycle_delay'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cycleImgPager' => theme_get_setting('cycle_img_pager'))), 'setting');
  drupal_add_js(array('slideshowKit' => array('cyclePrevNext' => theme_get_setting('cycle_prevnext'))), 'setting');

  if ((theme_get_setting('cycle_easing')) && (theme_get_setting('cycle_easing') != 'linear') && (theme_get_setting('cycle_easing') != 'swing')) {
    drupal_add_js($tundra_theme_path . '/scripts/jquery.easing-sooper.js');
  }
  if (is_file($tundra_theme_path . '/features/sooper-slideshowkit/jquery.cycle.all.min.js')) {
    drupal_add_js($tundra_theme_path . '/features/sooper-slideshowkit/jquery.cycle.all.min.js');
  }
  if (is_file($tundra_theme_path . '/features/sooper-slideshowkit/invoke-SlideshowKit.js')) {
    drupal_add_js($tundra_theme_path . '/features/sooper-slideshowkit/invoke-SlideshowKit.js');
  }
}