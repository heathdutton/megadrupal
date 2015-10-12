<?php
drupal_add_js('$(function () {$("#edit-slideshowkit").fieldset_tabs();});', 'inline');

$form['arctica_settings']['slideshowkit'] = array(
  '#title' => t('Sliders'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
  '#weight' => 3,
);

$form['arctica_settings']['slideshowkit']['flexslider'] = array(
  '#title' => t('jQ Flexslider slideshows'),
  '#type' => 'fieldset',
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_enable'] = array(
  '#type'          => 'checkbox',
  '#title'         => t('Enable jQuery Flexslider'),
  '#default_value' => theme_get_setting('flexslider_enable'),
);


$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_invoke'] = array(
  '#type' => 'textfield',
  '#title' => t('Element for Flexslider invocation'),
  '#default_value' => theme_get_setting('flexslider_invoke'),
  '#description' => t('Enter a jQuery- or CSS selector to mark the element(s) containing slides. For Flexslider this must be a parent element of a ul element.'),
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_speed'] = array(
  '#type' => 'textfield',
  '#title' => t('flexslider speed'),
  '#default_value' => theme_get_setting('flexslider_speed'),
  '#size' => 5,
  '#maxlength' => 5,
  '#description' => t('Enter the speed of the transition in milliseconds.'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_timeout'] = array(
  '#type' => 'textfield',
  '#title' => t('flexslider Timeout'),
  '#default_value' => theme_get_setting('flexslider_timeout'),
  '#size' => 5,
  '#maxlength' => 5,
  '#description' => t('Enter time in milliseconds between slide transitions (0 to disable auto advance).'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_animation'] = array(
  '#type' => 'select',
  '#title' => t('flexslider animation'),
  '#default_value' => theme_get_setting('flexslider_animation'),
  '#description' => t('Select the animation for transitions.'),
  '#options' => array(
    '0' => t('fade'),
    '1' => t('slide horizontal'),
    '2' => t('slide vertical'),
  ),
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_random'] = array(
  '#type' => 'select',
  '#title' => t('flexslider Slides ordering'),
  '#default_value' => theme_get_setting('flexslider_random'),
  '#description' => t('Select either sequentially ordered slides or play in random order.'),
  '#options' => array(
    '0' => t('Sequential'),
    '1' => t('Random'),
  ),
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_pause'] = array(
  '#type' => 'select',
  '#title' => t('flexslider Pause on hover'),
  '#default_value' => theme_get_setting('flexslider_pause'),
  '#description' => t('Select if you want to pause the slideshow when any element inside it is hovered.'),
  '#options' => array(
    '0' => t('No'),
    '1' => t('Yes'),
  ),
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_showPager'] = array(
  '#type' => 'select',
  '#title' => t('flexslider Pager'),
  '#default_value' => theme_get_setting('flexslider_showPager'),
  '#description' => t('Adds a pager to the slideshow.'),
  '#options' => array(
    '0' => t('No'),
    '1' => t('Yes'),
  ),
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_prevnext'] = array(
  '#type' => 'select',
  '#title' => t('flexslider Previous / Next links'),
  '#default_value' => theme_get_setting('flexslider_prevnext'),
  '#description' =>  t('Adds previous / next links to the slideshow.'),
  '#options' => array(
    '0' => t('No'),
    '1' => t('Yes'),
  ),
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_itemwidth'] = array(
  '#type' => 'textfield',
  '#title' => t('flexslider item width'),
  '#prefix' => '<h3>Carousel options</h3><div class="description" style="width:auto;margin:0 0 10px 0;float:none;">Give your ul tag containing the slides a .carousel class to enable carousel mode</div>',
  '#default_value' => theme_get_setting('flexslider_itemwidth'),
  '#description' => t('Box-model width of individual carousel items.'),
  '#size' => 5,
  '#maxlength' => 5,
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_itemmargin'] = array(
  '#type' => 'textfield',
  '#title' => t('flexslider item margin'),
  '#default_value' => theme_get_setting('flexslider_itemmargin'),
  '#description' => t('Margin between carousel items.'),
  '#size' => 5,
  '#maxlength' => 5,
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_minitems'] = array(
  '#type' => 'textfield',
  '#title' => t('flexslider minimum items'),
  '#default_value' => theme_get_setting('flexslider_minitems'),
  '#description' => t('Minimum number of carousel items that should be visible.'),
  '#size' => 5,
  '#maxlength' => 5,
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_maxitems'] = array(
  '#type' => 'textfield',
  '#title' => t('flexslider max items'),
  '#default_value' => theme_get_setting('flexslider_maxitems'),
  '#description' => t('Maxmimum number of carousel items that should be visible.'),
  '#size' => 5,
  '#maxlength' => 5,
);

$form['arctica_settings']['slideshowkit']['flexslider']['flexslider_move'] = array(
  '#type' => 'textfield',
  '#title' => t('flexslider move items'),
  '#default_value' => theme_get_setting('flexslider_move'),
  '#description' => t('Number of carousel items that should move on animation.'),
);

$form['arctica_settings']['slideshowkit']['cycle'] = array(
  '#title' => t('jQ Cycle slideshows'),
  '#type' => 'fieldset',
);

$form['arctica_settings']['slideshowkit']['cycle']['slideshowkit_enable'] = array(
  '#type'          => 'checkbox',
  '#title'         => t('Enable jQuery cycle'),
  '#default_value' => theme_get_setting('slideshowkit_enable'),
);


$form['arctica_settings']['slideshowkit']['cycle']['cycle_invoke'] = array(
  '#type' => 'textfield',
  '#title' => t('Element for cycle invocation'),
  '#default_value' => theme_get_setting('cycle_invoke'),
  '#description' => t('Enter a jQuery- or CSS selector to mark the element(s) containing slides.'),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_fx'] = array(
  '#type' => 'select',
  '#title' => t('Cycle Transition Animation'),
  '#default_value' => theme_get_setting('cycle_fx'),
  '#description' => t('Select transition effects for slide interchanges.'),
  '#options'       => array(
  'blindX' => t('blindX'),
  'blindY' => t('blindY'),
  'blindZ' => t('blindZ'),
  'cover' => t('cover'),
  'curtainX' => t('curtainX'),
  'curtainY' => t('curtainY'),
  'fade' => t('fade'),
  'fadeZoom' => t('fadeZoom'),
  'growX' => t('growX'),
  'growY' => t('growY'),
  'scrollUp' => t('scrollUp'),
  'scrollDown' => t('scrollDown'),
  'scrollLeft' => t('scrollLeft'),
  'scrollRight' => t('scrollRight'),
  'scrollHorz' => t('scrollHorz'),
  'scrollVert' => t('scrollVert'),
  'scrollLeft,scrollDown,scrollRight,scrollUp' => t('scrollAll'),
  'shuffle' => t('shuffle'),
  'slideX' => t('slideX'),
  'slideY' => t('slideY'),
  'toss' => t('toss'),
  'turnUp' => t('turnUp'),
  'turnDown' => t('turnDown'),
  'turnLeft' => t('turnLeft'),
  'turnRight' => t('turnRight'),
  'uncover' => t('uncover'),
  'wipe' => t('wipe'),
  'zoom' => t('zoom'),
  ),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_speed'] = array(
  '#type' => 'textfield',
  '#title' => t('Cycle speed'),
  '#default_value' => theme_get_setting('cycle_speed'),
  '#size' => 5,
  '#maxlength' => 5,
  '#description' => t('Enter the speed of the transition in milliseconds.'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_timeout'] = array(
  '#type' => 'textfield',
  '#title' => t('Cycle Timeout'),
  '#default_value' => theme_get_setting('cycle_timeout'),
  '#size' => 5,
  '#maxlength' => 5,
  '#description' => t('Enter time in milliseconds between slide transitions (0 to disable auto advance).'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_delay'] = array(
  '#type' => 'textfield',
  '#title' => t('Cycle Delay'),
  '#default_value' => theme_get_setting('cycle_delay'),
  '#size' => 5,
  '#maxlength' => 5,
  '#description' => t('Enter the additional delay (in ms) for first transition (Can be negative).'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_easing'] = array(
  '#type' => 'select',
  '#title' => t('Cycle Animation Easing'),
  '#default_value' => theme_get_setting('cycle_easing'),
  '#options' => $easing_options,
  '#description' => t('Select the function by which the animation gains speed.'),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_random'] = array(
  '#type' => 'select',
  '#title' => t('Cycle Slides ordering'),
  '#default_value' => theme_get_setting('cycle_random'),
  '#description' => t('Select either sequentially ordered slides or play in random order.'),
  '#options' => array(
    '0' => t('Sequential'),
    '1' => t('Random'),
  ),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_continuous'] = array(
  '#type' => 'select',
  '#title' => t('Cycle Continous play'),
  '#default_value' => theme_get_setting('cycle_continuous'),
  '#description' => t('Select if you want the next transition to start as soon as the current transition ends.'),
  '#options' => array(
    '0' => t('No'),
    '1' => t('Yes'),
  ),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_pause'] = array(
  '#type' => 'select',
  '#title' => t('Cycle Pause on hover'),
  '#default_value' => theme_get_setting('cycle_pause'),
  '#description' => t('Select if you want to pause the slideshow when any element inside it is hovered.'),
  '#options' => array(
    '0' => t('No'),
    '1' => t('Yes'),
  ),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_showPager'] = array(
  '#type' => 'select',
  '#title' => t('Cycle Pager'),
  '#default_value' => theme_get_setting('cycle_showPager'),
  '#description' => t('Adds a pager to the slideshow'),
  '#options' => array(
    '0' => t('No'),
    '1' => t('Yes'),
  ),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_pagerEvent'] = array(
  '#type' => 'select',
  '#title' => t('Cycle Pager Event'),
  '#default_value' => theme_get_setting('cycle_pagerEvent'),
  '#description' => t('Select the event that signifies a pager action.'),
  '#options' => array(
    'mouseover' => t('hover'),
    'click' => t('click'),
  ),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_pauseOnPagerHover'] = array(
  '#type' => 'select',
  '#title' => t('Cycle Pause on pager hover'),
  '#default_value' => theme_get_setting('cycle_pauseOnPagerHover'),
  '#description' =>  t('Select if you want to pause the slideshow when the pager is hovered.'),
  '#options' => array(
    '0' => t('No'),
    '1' => t('Yes'),
  ),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_img_pager'] = array(
  '#type' => 'select',
  '#title' => t('Cycle image pager. Only works if slides are image tags.'),
  '#default_value' => theme_get_setting('cycle_img_pager'),
  '#description' =>  t('Select if you want to automatically created thumbnails from the slides. This only works if the slides are images.'),
  '#options' => array(
    '0' => t('No'),
    '1' => t('Yes'),
  ),
);

$form['arctica_settings']['slideshowkit']['cycle']['cycle_prevnext'] = array(
  '#type' => 'select',
  '#title' => t('Cycle Previous / Next links'),
  '#default_value' => theme_get_setting('cycle_prevnext'),
  '#description' =>  t('Adds previous / next links to the slideshow.'),
  '#options' => array(
    '0' => t('No'),
    '1' => t('Yes'),
  ),
);
