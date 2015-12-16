<?php
/**
 * @file
 * The theme system that controls the display of Views Slideshow: Bxslider.
 */

/**
 *  Process variables for views-bxslider-main-frame.tpl.php.
 */
function template_preprocess_bxslider_views_slideshow_main_frame(&$vars) {
  _bxslider_views_slideshow_set_extra_css($vars['settings']['general'], $vars['id']);

  _bxslider_views_slideshow_add_js($vars['settings'], 'views-slideshow-bxslider-images-' . $vars['id']);

  $vars['classes_array'][] = 'bxslider';
  $rendered_rows = '';
  $current_row = 0;
  foreach ($vars['rows'] as $count => $item) {
    // Replace the width and height with zero. for fix.
    if (isset($vars['settings']["fixes"])
      && isset($vars['settings']["fixes"]["height_does_not_dyn_change"])
      && $vars['settings']["fixes"]["height_does_not_dyn_change"] == TRUE
    ) {
      $item = _bxslider_views_slideshow_theme_filter_out_width_and_heigth_attribute($item);
    }

    $rendered_rows[] = theme('bxslider_views_slideshow_main_frame_row', array(
      'vss_id' => $vars['id'],
      'items' => $item,
      'count' => count($vars['rows']),
      'current_row' => $current_row,
      'view' => $vars['view']
    ));
    $current_row++;
  }
  $vars['rendered_rows'] = $rendered_rows;
}

/**
 * Filters out the height and with from the image tag.
 * This is a fix that needs to be enabled in the admin screen.
 * @param $general_settings
 */
function _bxslider_views_slideshow_theme_filter_out_width_and_heigth_attribute($item) {
  $dom = new DOMDocument();
  $dom->loadHTML(mb_convert_encoding($item, 'HTML-ENTITIES', 'UTF-8'));

  $domElement = $dom->documentElement;
  $img = $domElement->getElementsByTagName('img');

  if ($img->length == 0) {
    return $item;
  }

  $img = $img->item(0);

  $img->removeAttribute('width');
  $img->removeAttribute('height');

  $dom->removeChild($dom->doctype);
  $dom->replaceChild($dom->firstChild->firstChild->firstChild, $dom->firstChild);

  return $dom->saveHTML();
}

/**
 * This function sets extra cSS setti,gs that are configurable at the admin screen
 * of this module. (Views slideshow bx slider).
 * @param $general_settings
 */
function _bxslider_views_slideshow_set_extra_css($general_settings, $id_of_slider) {
  $align_image = isset($general_settings['align_image']) ?
    $general_settings['align_image'] : NULL;
  $color_caption = isset($general_settings['color_caption']) ?
    $general_settings['color_caption'] : NULL;
  $align_caption = isset($general_settings['align_caption']) ?
    $general_settings['align_caption'] : NULL;;
  // Overlay bug fix.
  $selector_overlay =
    "#views-slideshow-bxslider-" . $id_of_slider
    . ".views-slideshow-bxslider .bx-wrapper .bx-controls-direction a";
  drupal_add_css($selector_overlay . ' { z-index:499; }', 'inline');

  $selector = "#views-slideshow-bxslider-" . $id_of_slider
    . ".views-slideshow-bxslider li img";
  switch ($align_image) {
    case 'center':
      drupal_add_css($selector . ' { margin-left:auto; margin-right:auto; }', 'inline');
      break;

    case 'right':
      drupal_add_css($selector . ' { margin-left:auto; }', 'inline');
      break;

    default:
      drupal_add_css($selector . ' { margin-right:auto; }', 'inline');
  }
  // Caption alignment.
  $selector_caption_alignment = "#views-slideshow-bxslider-" . $id_of_slider
    . ".views-slideshow-bxslider .bx-caption";
  if (!empty($align_caption)) {
    switch ($align_caption) {
      case 'center':
        drupal_add_css($selector_caption_alignment . ' { text-align: center; }', 'inline');
        break;

      case 'right':
        drupal_add_css($selector_caption_alignment . ' { text-align: right; }', 'inline');
        break;

      default:
        // Nothing.
    }
  }
  // Second part color caption overruling.
  $selector_caption = "#views-slideshow-bxslider-" . $id_of_slider
    . ".views-slideshow-bxslider .bx-wrapper .bx-caption";
  if (!empty($color_caption)) {
    drupal_add_css($selector_caption . ' { background: rgba(' . $color_caption . ') }', 'inline');
  }
}

/**
 * Helper function to build and pass BxSlider settings to Drupal.
 */
function _bxslider_views_slideshow_add_js($options, $id) {
  // Make an correct id.
  $id = str_replace('-', '_', $id);

  $bxslider_path = _bxslider_views_slideshow_library_path();

  // Load Bxslider.
  drupal_add_js($bxslider_path, array('group' => JS_LIBRARY));
  if (!$options['general']['disable_standard_css']) {
    drupal_add_css(libraries_get_path('bxslider') . '/jquery.bxslider.css');
  }

  // Load our Bxslider js.
  drupal_add_js(drupal_get_path('module', 'bxslider_views_slideshow') . '/bxslider_views_slideshow.js');

  // Process Bxslider settings.

  // Add support for PagerCustom option.
  if ($options['pagerfieldset']['pagerType'] == 'custom') {
    unset($options['pagerfieldset']['pagerType']);
  }
  else {
    unset($options['pagerfieldset']['pagerCustom']);
  }

  // Callbacks.
  if (!empty($options['callback']['onSliderLoad'])) {
    $options['callback']['onSliderLoad'] = "function(currentIndex){" .
      bxslider_views_slideshow_filter_xss($options['callback']['onSliderLoad']) . "}";
  }
  else {
    unset($options['callback']['onSliderLoad']);
  }
  if (!empty($options['callback']['onSlideBefore'])) {
    $options['callback']['onSlideBefore'] = "function(slideElement, oldIndex, newIndex){" .
      bxslider_views_slideshow_filter_xss($options['callback']['onSlideBefore']) . "}";
  }
  else {
    unset($options['callback']['onSlideBefore']);
  }
  if (!empty($options['callback']['onSlideAfter'])) {
    $options['callback']['onSlideAfter'] = "function(slideElement, oldIndex, newIndex){" .
      bxslider_views_slideshow_filter_xss($options['callback']['onSlideAfter']) . "}";
  }
  else {
    unset($options['callback']['onSlideAfter']);
  }
  if (!empty($options['callback']['onSlideNext'])) {
    $options['callback']['onSlideNext'] = "function(slideElement, oldIndex, newIndex){" .
      bxslider_views_slideshow_filter_xss($options['callback']['onSlideNext']) . "}";
  }
  else {
    unset($options['callback']['onSlideNext']);
  }
  if (!empty($options['callback']['onSlidePrev'])) {
    $options['callback']['onSlidePrev'] = "function(slideElement, oldIndex, newIndex){" .
      bxslider_views_slideshow_filter_xss($options['callback']['onSlidePrev']) . "}";
  }
  else {
    unset($options['callback']['onSlidePrev']);
  }

  // Cast all numbers that are currently strings into a numeric type and pass
  // all string values through filter_xss.
  array_walk_recursive($options, '_bxslider_views_slideshow_prep_settings');

  // Add bxslider settings
  drupal_add_js(array(
    'viewsSlideshowBxslider' => array($id => $options),
  ), 'setting');
}

/**
 * A helper function that will cast all numbers that are currently strings into
 * a numeric type.  It'll also pass all string values through filter_xss.
 */
function _bxslider_views_slideshow_prep_settings(&$value) {
  if (is_numeric($value)) {
    $value = 0 + $value;
  }
  else {
    $value = bxslider_views_slideshow_filter_xss($value);
  }
}

///**
// * This functions returns me the admin JS of the bxSlider.
// * @param array $autofieldset
// * autofieldset settings
// * @param array $general
// * general settings
// * @param array $carousel
// * carrousel settings
// * @param array $pagerfieldset
// * pager settings
// * @param array $controlsfieldset
// * controls settings
// * @param array $carousel
// * carrousel settings.
// * @return mixed|string
// * a string to be used to initialize bxslider js.
// */
//function _bxslider_views_slideshow_theme_get_admin_js($autofieldset,
//                                                      $general,
//                                                      $carousel,
//                                                      $pagerfieldset,
//                                                      $controlsfieldset,
//                                                      $carousel) {
//  $bx_slider_js = <<<BXSLIDERJSCONTENT
//    mode: '{$general['mode']}',
//    speed: {$general['speed']},
//    slideMargin: {$general['slideMargin']},
//    startSlide: {$general['startSlide']},
//    randomStart: {$general['randomStart']},
//    infiniteLoop: {$general['infiniteLoop']},
//    hideControlOnEnd: {$general['hideControlOnEnd']},
//    captions: {$general['captions']},
//    pager: {$pagerfieldset['pager']},
//BXSLIDERJSCONTENT;
//
//  // Add support for PagerCustom option.
//  if ($pagerfieldset['pagerType'] == 'custom') {
//    $bx_slider_js .= "\n    pagerCustom: '{$pagerfieldset['pagerCustom']}',";
//  }
//  else {
//    $bx_slider_js .= "\n    pagerType: '{$pagerfieldset['pagerType']}',";
//  }
//
//  $bx_slider_js .= <<<BXSLIDERJSCONTENT
//    controls: {$controlsfieldset['controls']},
//    auto: {$autofieldset['auto']},
//    pause: {$autofieldset['pause']},
//    minSlides: {$carousel['minSlides']},
//    maxSlides: {$carousel['maxSlides']},
//    ticker: {$general['ticker']},
//    tickerHover: {$general['tickerHover']},
//    adaptiveHeight: {$general['adaptiveHeight']},
//    adaptiveHeightSpeed: {$general['adaptiveHeightSpeed']},
//    video: {$general['video']},
//    touchEnabled: {$general['touchEnabled']},
//    preloadImages: '{$general['preloadImages']}',
//    swipeThreshold: {$general['swipeThreshold']},
//    oneToOneTouch: {$general['oneToOneTouch']},
//    preventDefaultSwipeX: {$general['preventDefaultSwipeX']},
//    preventDefaultSwipeY: {$general['preventDefaultSwipeY']},
//    pagerShortSeparator: '{$pagerfieldset['pagerShortSeparator']}',
//    nextText: '{$controlsfieldset['nextText']}',
//    prevText: '{$controlsfieldset['prevText']}',
//    startText: '{$controlsfieldset['startText']}',
//    stopText: '{$controlsfieldset['stopText']}',
//    autoControls: {$controlsfieldset['autoControls']},
//    autoControlsCombine: {$controlsfieldset['autoControlsCombine']},
//    autoHover: {$autofieldset['autoHover']},
//    autoDelay: {$autofieldset['autoDelay']},
//    moveSlides: {$carousel['moveSlides']},
//    slideWidth: {$carousel['slideWidth']},
//    useCSS: {$general['useCSS']},
//    autoDirection: '{$autofieldset['autoDirection']}',
//BXSLIDERJSCONTENT;
//  return filter_xss(trim($bx_slider_js));
//}
