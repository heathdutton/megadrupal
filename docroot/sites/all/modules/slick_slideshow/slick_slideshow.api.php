<?php

/**
 * @file
 * API documentation for Slick Slideshow.
 */

/**
 * Developers may want to create custom Slick Slideshows in their own code.
 *
 * Calling slick_slideshow_create($field_name, $settings) allows devs to bypass
 * the admin ui and add all of Slick's setting variables directly to the function.
 *
 * This makes sense since there are a few settings that won't be added to the
 * web interface such as "onBeforeLoad" or "responsive".
 *
 * @param string $field_selector
 *  The selector of the field that you want to turn into a slideshow.
 *  Can be an id or class.
 *
 * @param array $settings
 *  Slick Slideshow settings passed to the javascript init.
 *
 * @param string $target
 *  (optional) If the slideshow slide elements aren't the direct children
 *  of the $field_selector, specify a target selector.
 */

$field_selector = '#slick-slideshow-field-test';
$target = '.slick-slideshow-target';
$settings = array(
  'accessibility' => true, // Default true
  'autoplay' => true, // Default false
  'autoplaySpeed' => 2500, // Default 3000
  'arrows' => false, // Default false
  'centerMode' => false, // Default false
  'centerPadding' => '20px', // Default 50px
  'cssEase' => 'ease', // Default ease
  'customPaging' => 'function(){}',
  'dots' => false, // Default false
  'draggable' => false, // Default true
  'easing' => 'linear', // Default linear
  'fade' => true, // Default false
  'infinite' => false, // Default true
  'lazyLoad' => 'ondemand', // Default ondemand
  'onBeforeChange' => 'function(this, index){}',
  'onAfterChange' => 'function(this, index){}',
  'onInit' => 'function(this){}',
  'onReInit' => 'function(this){}',
  'pauseOnHover' => true, // Default true
  'responsive' => '[]',
  'slide' => 'div', // Default div
  'slidesToShow' => 1, // Default 1
  'slidesToScroll' => 1, // Default 1
  'speed' => 300, // Default 300
  'swipe' => true, // Default true
  'touchMove' => true, // Default true
  'touchThreshold' => 5, // Default 5
  'useCSS' => true, // Default true
  'vertical' => false, // Default false
);

/**
 * Implements slick_slideshow_create().
 *
 * @param string $field_selector
 *  The selector of the field that you want to turn into a slideshow.
 *  Can be an id or class.
 *
 * @param array $settings
 *  Slick Slideshow settings passed to the javascript init.
 *
 * @param string $target
 *  (optional) If the slideshow slide elements aren't the direct children
 *  of the $field_selector, specify a target selector.
 */
slick_slideshow_create($field_selector, $settings, $target);

/*
 * Implements slick_slideshow_update().
 *
 * @param string $field_selector
 *  Field element selector that the Slick slideshow belongs to.
 * @param array $settings
 *  Settings array with options to update.
 * @param string $target (optional)
 *  If the $field_selector isn't the element that Slick should be created on,
 *  specify the $target as a child element of the $field_selector.
 */
slick_slideshow_update($field_selector, $settings, $target);
