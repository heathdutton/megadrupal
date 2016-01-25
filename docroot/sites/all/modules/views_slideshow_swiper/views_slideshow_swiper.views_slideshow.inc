<?php

/**
 * @file
 * Views Slideshow hook implementations.
 */

/**
 * Implements hook_views_slideshow_slideshow_info().
 */
function views_slideshow_swiper_views_slideshow_slideshow_info() {
  $options = array(
    'views_slideshow_swiper' => array(
      'name' => t('Swiper'),
      'accepts' => array(
        'goToSlide',
        'nextSlide',
        'pause',
        'play',
        'previousSlide',
      ),
      'calls' => array(
        'transitionBegin',
        'transitionEnd',
        'goToSlide',
        'pause',
        'play',
        'nextSlide',
        'previousSlide',
      ),
    ),
  );

  // @todo figure out which options to add

  return $options;
}

/**
 * Implements hook_views_slideshow_option_definition().
 */
function views_slideshow_swiper_views_slideshow_option_definition() {
  // Default options for Swiper
  $options['views_slideshow_swiper'] = array(
    'contains' => array(
      'initialSlide' => array('default' => 0),
      'direction' => array('default' => 'horizontal'),
      'speed' => array('default' => 300),
      'setWrapperSize' => array('default' => false),
      'virtualTranslate' => array('default' => false),
      // Autoplay.
      'autoplay' => array('default' => 1000),
      'autoplayDisableOnInteraction' => array('default' => true),
      // Progress.
      'watchSlidesProgress' => array('default' => false),
      'watchSlidesVisibility' => array('default' => false),
      // Freemode.
      'freeMode' => array('default' => false),
      'freeModeMomentum' => array('default' => true),
      'freeModeMomentumRatio' => array('default' => 1),
      'freeModeMomentumBounce' => array('default' => true),
      'freeModeMomentumBounceRatio' => array('default' => 1),
      // Effects.
      'effect' => array('default' => 'slide'),
      // Parallax.
      'parallax' => array('default' => false),
      // Slides grid.
      'spaceBetween' => array('default' => 0),
      'slidesPerView' => array('default' => 1),
      'slidesPerColumn' => array('default' => 1),
      'slidesPerColumnFill' => array('default' => 'column'),
      'slidesPerGroup' => array('default' => 1),
      'centeredSlides' => array('default' => false),
      // Grab cursor.
      'grabCursor' => array('default' => false),
      // Touches.
      'touchRatio' => array('default' => 1),
      'touchAngle' => array('default' => 45),
      'simulateTouch' => array('default' => true),
      'shortSwipes' => array('default' => true),
      'longSwipes' => array('default' => true),
      'longSwipesRatio' => array('default' => 0.5),
      'longSwipesMs' => array('default' => 300),
      'followFinger' => array('default' => true),
      'onlyExternal' => array('default' => false),
      'threshold' => array('default' => 0),
      'touchMoveStopPropagation' => array('default' => true),
      // Touch resistance.
      'resistance' => array('default' => true),
      'resistanceRatio' => array('default' => 0.85),
      // Clicks
      'preventClicks' => array('default' => true),
      'preventClicksPropagation' => array('default' => true),
      'slideToClickedSlide' => array('default' => false),
      // Swiping / no swiping.
      'allowSwipeToPrev' => array('default' => true),
      'allowSwipeToNext' => array('default' => true),
      'noSwiping' => array('default' => true),
      'noSwipingClass' => array('default' => 'swiper-no-swiping'),
      'swipeHandler' => array('default' => null),
      // Pagination.
      'pagination' => array('default' => null),
      'paginationHide' => array('default' => true),
      'paginationClickable' => array('default' => false),
      'paginationBulletRender' => array('default' => null),
      // Navigation Buttons.
      'nextButton' => array('default' => 'views-slideshow-controls-text-next a'),
      'prevButton' => array('default' => 'views-slideshow-controls-text-previous a'),
      // Accessibility.
      'a11y' => array('default' => false),
      'prevSlideMessage' => array('default' => 'Previous slide'),
      'nextSlideMessage' => array('default' => 'Next slide'),
      'firstSlideMessage' => array('default' => 'This is the first slide'),
      'lastSlideMessage' => array('default' => 'This is the last slide'),
      // Scrollbar.
      'scrollbar' => array('default' => null),
      'scrollbarHide' => array('default' => true),
      // Keyboard / mousewheel.
      'keyboardControl' => array('default' => false),
      'mousewheelControl' => array('default' => false),
      'mousewheelForceToAxis' => array('default' => false),
      // Hash navigation.
      'hashnav' => array('default' => false),
      // Images
      'preloadImages' => array('default' => true),
      'updateOnImagesReady' => array('default' => true),
      'lazyLoading' => array('default' => false),
      'lazyLoadingInPrevNext' => array('default' => false),
      'lazyLoadingOnTransitionStart' => array('default' => false),
      // Loop.
      'loop' => array('default' => false),
      'loopAdditionalSlides' => array('default' => 0),
      'loopedSlides' => array('default' => null),
      // Control
      'control' => array('default' => ''), // Named reference to Swiper instances 
      'controlInverse' => array('default' => false),
      // Observer.
      'observer' => array('default' => false),
      'observeParents' => array('default' => false),
      // Callbacks.
      'runCallbacksOnInit' => array('default' => true),
      'onInit' => array('default' => ''),
      'onSlideChangeStart' => array('default' => ''),
      'onSlideChangeEnd' => array('default' => ''),
      'onTransitionStart' => array('default' => ''),
      'onTransitionEnd' => array('default' => ''),
      'onTouchStart' => array('default' => ''),
      'onTouchMove' => array('default' => ''),
      'onTouchMoveOpposite' => array('default' => ''),
      'onSliderMove' => array('default' => ''),
      'onTouchEnd' => array('default' => ''),
      'onClick' => array('default' => ''),
      'onTap' => array('default' => ''),
      'onDoubleTap' => array('default' => ''),
      'onImagesReady' => array('default' => ''),
      'onProgress' => array('default' => ''),
      'onReachBeginning' => array('default' => ''),
      'onReachEnd' => array('default' => ''),
      'onDestroy' => array('default' => ''),
      'onSetTranslate' => array('default' => ''),
      'onSetTransition' => array('default' => ''),
      'onAutoplayStart' => array('default' => ''),
      'onAutoplayStop' => array('default' => ''),
      'onLazyImageLoad' => array('default' => ''),
      'onLazyImageReady' => array('default' => ''),
      // Namespace.
      'slideClass' => array('default' => 'swiper-slide'),
      'slideActiveClass' => array('default' => 'swiper-slide-active'),
      'slideVisibleClass' => array('default' => 'swiper-slide-visible'),
      'slideDuplicateClass' => array('default' => 'swiper-slide-duplicate'),
      'slideNextClass' => array('default' => 'swiper-slide-next'),
      'slidePrevClass' => array('default' => 'swiper-slide-prev'),
      'wrapperClass' => array('default' => 'swiper-wrapper'),
      'bulletClass' => array('default' => 'swiper-pagination-bullet'),
      'bulletActiveClass' => array('default' => 'swiper-pagination-bullet-active'),
      'paginationHiddenClass' => array('default' => 'swiper-pagination-hidden'),
      'buttonDisabledClass' => array('default' => 'swiper-button-disabled'),
    ),
  );
  // @todo figure out which options to define
  // @todo integrate with "optionsets"

  return $options;
}

function _views_slideshow_swiper_validate_integer_non_negative($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && (!is_numeric($value) || intval($value) != $value || $value < 0)) {
    form_error($element, t('%name must be a non-negative integer.', array('%name' => $element['#title'])));
  }
}

function _views_slideshow_swiper_validate_directions($element, &$form_state) {
  $value = $element['#value'];
  $directions = array('horizontal', 'vertical');
  if (!in_array($value, $directions)) {
    form_error($element, t('%name must be either %dirs.', array(
      '%name' => $element['#title'],
      '%dirs' => "'" . implode("' or '", $directions) . "'",
    )));
  }
}

/**
 * Configuration form within the Views Slideshow settings pane
 */
function views_slideshow_swiper_views_slideshow_slideshow_type_form(&$form, &$form_state, &$view) {
  // For a full list of options
  // @see http://www.idangero.us/swiper/api

  $form['views_slideshow_swiper']['initialSlide'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t('Index number of initial slide.'),
  );
  $form['views_slideshow_swiper']['direction'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_directions'),
    '#description' => t("Could be 'horizontal' or 'vertical' (for vertical slider)."),
  );
  $form['views_slideshow_swiper']['speed'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Duration of transition between slides (in ms).'),
  );
  $form['views_slideshow_swiper']['setWrapperSize'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enabled this option and plugin will set width/height on swiper wrapper equal to total size of all slides. Mostly should be used as compatibility fallback option for browser that don't support flexbox layout well."),
  );
  $form['views_slideshow_swiper']['virtualTranslate'] = array(
    '#type' => 'checkbox',
    '#description' => t(''),
  );
  $form['views_slideshow_swiper']['autoplay'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t('Delay between transitions (in ms). If set to zero or blank, auto play will be disabled.'),
  );
  $form['views_slideshow_swiper']['autoplayDisableOnInteraction'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to false and autoplay will not be disabled after user interactions (swipes), it will be restarted every time after interaction.'),
  );
  // Progress
  $form['views_slideshow_swiper']['watchSlidesProgress'] = array(
    '#type' => 'checkbox',
    '#description' => t('Enable this feature to calculate each slides progress.'),
  );
  $form['views_slideshow_swiper']['watchSlidesVisibility'] = array(
    '#type' => 'checkbox',
    '#description' => t("'Watch slides progress' should be enabled. Enable this option and slides that are in viewport will have additional visible class."),
  );
  // Freemode
  $form['views_slideshow_swiper']['freeMode'] = array(
    '#type' => 'checkbox',
    '#description' => t('If true then slides will not have fixed positions.'),
  );
  $form['views_slideshow_swiper']['freeModeMomentum'] = array(
    '#type' => 'checkbox',
    '#description' => t('If true, then slide will keep moving for a while after you release it.'),
  );
  $form['views_slideshow_swiper']['freeModeMomentumRatio'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_number'),
    '#description' => t('Higher value produces larger momentum distance after you release slider.'),
  );
  $form['views_slideshow_swiper']['freeModeMomentumBounce'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to false if you want to disable momentum bounce in free mode.'),
  );
  $form['views_slideshow_swiper']['freeModeMomentumBounceRatio'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_number'),
    '#description' => t('Higher value produces larger momentum bounce effect.'),
  );
  // Effects
  $form['views_slideshow_swiper']['effect'] = array(
    '#type' => 'textfield',
    '#description' => t('Could be "slide", "fade", "cube" or "coverflow".'),
  );
  // Parallax
  $form['views_slideshow_swiper']['parallax'] = array(
    '#type' => 'checkbox',
    '#description' => t('Enable, if you want to use "parallaxed" elements inside of slider.'),
  );
  // Slides Grid
  $form['views_slideshow_swiper']['spaceBetween'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t('Distance between slides in px.'),
  );
  $form['views_slideshow_swiper']['slidesPerView'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Number of slides per view (slides visible at the same time on slider's container) or zero to set automatic."),
  );
  $form['views_slideshow_swiper']['slidesPerColumn'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Number of slides per column, for multirow layout.'),
  );
  $form['views_slideshow_swiper']['slidesPerColumnFill'] = array(
    '#type' => 'textfield',
    '#description' => t("Could be 'column' or 'row'. Defines how slides should fill rows, by column or by row."),
  );
  $form['views_slideshow_swiper']['slidesPerGroup'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Set numbers of slides to define and enable group sliding. Useful to use with slidesPerView > 1.'),
  );
  $form['views_slideshow_swiper']['centeredSlides'] = array(
    '#type' => 'checkbox',
    '#description' => t('If true, then active slide will be centered, not always on the left side.'),
  );
  // Grab Cursor
  $form['views_slideshow_swiper']['grabCursor'] = array(
    '#type' => 'checkbox',
    '#description' => t('This option may a little improve desktop usability. If true, user will see the "grab" cursor when hover on Swiper.'),
  );
  // Touches
  $form['views_slideshow_swiper']['touchRatio'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_number'),
  );
  $form['views_slideshow_swiper']['touchAngle'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Allowable angle (in degrees) to trigger touch move.'),
  );
  $form['views_slideshow_swiper']['simulateTouch'] = array(
    '#type' => 'checkbox',
    '#description' => t('If true, Swiper will accept mouse events like touch events (click and drag to change slides).'),
  );
  $form['views_slideshow_swiper']['shortSwipes'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to false if you want to disable short swipes.'),
  );
  $form['views_slideshow_swiper']['longSwipes'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to false if you want to disable long swipes.'),
  );
  $form['views_slideshow_swiper']['longSwipesRatio'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_number'),
    '#description' => t('Ratio to trigger swipe to next/previous slide during long swipes.'),
  );
  $form['views_slideshow_swiper']['longSwipesMs'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Minimal duration (in ms) to trigger swipe to next/previous slide during long swipes.'),
  );
  $form['views_slideshow_swiper']['followFinger'] = array(
    '#type' => 'checkbox',
    '#description' => t('If disabled, then slider will be animated only when you release it, it will not move while you hold your finger on it.'),
  );
  $form['views_slideshow_swiper']['onlyExternal'] = array(
    '#type' => 'checkbox',
    '#description' => t('If true, then the only way to switch the slide is use of external API functions like slidePrev or slideNext.'),
  );
  $form['views_slideshow_swiper']['threshold'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t('Threshold value in px. If "touch distance" will be lower than this value then swiper will not move.'),
  );
  $form['views_slideshow_swiper']['touchMoveStopPropagation'] = array(
    '#type' => 'checkbox',
    '#description' => t('If enabled, then propagation of "touchmove" will be stopped.'),
  );
  // Touch Resistance
  $form['views_slideshow_swiper']['resistance'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to false if you want to disable resistant bounds.'),
  );
  $form['views_slideshow_swiper']['resistanceRatio'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_number'),
    '#description' => t('This option allows you to control resistance ratio.'),
  );
  // Clicks
  $form['views_slideshow_swiper']['preventClicks'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to true to prevent accidental unwanted clicks on links during swiping.'),
  );
  $form['views_slideshow_swiper']['preventClicksPropagation'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to true to stop clicks event propagation on links during swiping.'),
  );
  $form['views_slideshow_swiper']['slideToClickedSlide'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to true and click on any slide will produce transition to this slide.'),
  );
  // Swiping / No swiping
  $form['views_slideshow_swiper']['allowSwipeToPrev'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to false to disable swiping to previous slide direction (to left or top).'),
  );
  $form['views_slideshow_swiper']['allowSwipeToNext'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to false to disable swiping to next slide direction (to right or bottom).'),
  );
  $form['views_slideshow_swiper']['noSwiping'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to false to disable swiping to next slide direction (to right or bottom).'),
  );
  $form['views_slideshow_swiper']['noSwipingClass'] = array(
    '#type' => 'textfield',
    '#description' => t("If true, then you can add noSwipingClass class to swiper's slide to prevent/disable swiping on this element."),
  );
  $form['views_slideshow_swiper']['swipeHandler'] = array(
    '#type' => 'textfield',
    '#description' => t('String with CSS selector or HTML element of the container with pagination that will work as only available handler for swiping.'),
  );
  // Pagination
  $form['views_slideshow_swiper']['pagination'] = array(
    '#type' => 'textfield',
    '#description' => t('String with CSS selector or HTML element of the container with pagination.'),
  );
  $form['views_slideshow_swiper']['paginationHide'] = array(
    '#type' => 'checkbox',
    '#description' => t("Toggle (hide/true) pagination container visibility when click on Slider's container."),
  );
  $form['views_slideshow_swiper']['paginationClickable'] = array(
    '#type' => 'checkbox',
    '#description' => t('If true then clicking on pagination button will cause transition to appropriate slide.'),
  );
  $form['views_slideshow_swiper']['paginationBulletRender'] = array(
    '#type' => 'textfield',
    '#description' => t('This parameter allows totally customize pagination bullets, you need to pass here the name of a function that accepts index number of pagination bullet and required element class name (className).'),
  );
  // Navigation Buttons
  $form['views_slideshow_swiper']['nextButton'] = array(
    '#type' => 'textfield',
    '#description' => t('String with CSS selector or HTML element of the element that will work like "next" button after click on it.'),
  );
  $form['views_slideshow_swiper']['prevButton'] = array(
    '#type' => 'textfield',
    '#description' => t('String with CSS selector or HTML element of the element that will work like "prev" button after click on it.'),
  );
  // Accessibility
  $form['views_slideshow_swiper']['a11y'] = array(
    '#type' => 'checkbox',
    '#description' => t('Option to enable keyboard accessibility to provide foucsable navigation buttons and basic ARIA for screen readers.'),
  );
  $form['views_slideshow_swiper']['prevSlideMessage'] = array(
    '#type' => 'textfield',
    '#description' => t('Message for screen readers for previous button.'),
  );
  $form['views_slideshow_swiper']['nextSlideMessage'] = array(
    '#type' => 'textfield',
    '#description' => t('Message for screen readers for next button.'),
  );
  $form['views_slideshow_swiper']['firstSlideMessage'] = array(
    '#type' => 'textfield',
    '#description' => t('Message for screen readers for previous button when swiper is on first slide.'),
  );
  $form['views_slideshow_swiper']['lastSlideMessage'] = array(
    '#type' => 'textfield',
    '#description' => t('Message for screen readers for previous button when swiper is on last slide.'),
  );
  // Scollbar
  $form['views_slideshow_swiper']['scrollbar'] = array(
    '#type' => 'textfield',
    '#description' => t('String with CSS selector or HTML element of the container with scrollbar.'),
  );
  $form['views_slideshow_swiper']['scrollbarHide'] = array(
    '#type' => 'checkbox',
    '#description' => t('Hide scrollbar automatically after user interaction.'),
  );
  // Keyboard / Mousewheel
  $form['views_slideshow_swiper']['keyboardControl'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to true to enable navigation through slides using keyboard right and left (for horizontal mode), top and borrom (for vertical mode) keyboard arrows.'),
  );
  $form['views_slideshow_swiper']['mousewheelControl'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to true to enable navigation through slides using mouse wheel.'),
  );
  $form['views_slideshow_swiper']['mousewheelForceToAxis'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to true to force mousewheel swipes to axis. So in horizontal mode mousewheel will work only with horizontal mousewheel scrolling, and only with vertical scrolling in vertical mode.'),
  );
  // Hash Navigation
  $form['views_slideshow_swiper']['hashnav'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to true to enable hash url navigation to for slides.'),
  );
  // Images
  $form['views_slideshow_swiper']['preloadImages'] = array(
    '#type' => 'checkbox',
    '#description' => t('When enabled Swiper will force to load all images.'),
  );
  $form['views_slideshow_swiper']['updateOnImagesReady'] = array(
    '#type' => 'checkbox',
    '#description' => t('When enabled Swiper will be reinitialized after all inner images (<img> tags) are loaded. Required preloadImages: true.'),
  );
  $form['views_slideshow_swiper']['lazyLoading'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to "true" to enable images lazy loading. Note that preloadImages should be disabled.'),
  );
  $form['views_slideshow_swiper']['lazyLoadingInPrevNext'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to "true" to enable lazy loading for the closest slides images (for previous and next slide images).'),
  );
  $form['views_slideshow_swiper']['lazyLoadingOnTransitionStart'] = array(
    '#type' => 'checkbox',
    '#description' => t('By default, Swiper will load lazy images after transition to this slide, so you may enable this parameter if you need it to start loading of new image in the beginning of transition.'),
  );
  $form['views_slideshow_swiper']['loop'] = array(
    '#type' => 'checkbox',
    '#description' => t('.'),
  );
  $form['views_slideshow_swiper']['loopAdditionalSlides'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t('Addition number of slides that will be cloned after creating of loop.'),
  );
  $form['views_slideshow_swiper']['loopedSlides'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t("If you use slidesPerView:'auto' with loop mode you should tell to Swiper how many slides it should loop (duplicate) using this parameter."),
  );
  // Controller
  //~ $form['views_slideshow_swiper']['control'] = array(
    //~ '#type' => '[Swiper Instance]',
    //~ '#description' => t('Pass here another Swiper instance or array with Swiper instances that should be controlled by this Swiper.'),
  //~ );
  $form['views_slideshow_swiper']['controlInverse'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to true and controlling will be in inverse direction.'),
  );
  // Observer
  $form['views_slideshow_swiper']['observer'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to true to enable Mutation Observer on Swiper and its elements. In this case Swiper will be updated (reinitialized) each time if you change its style (like hide/show) or modify its child elements (like adding/removing slides).'),
  );
  $form['views_slideshow_swiper']['observeParents'] = array(
    '#type' => 'checkbox',
    '#description' => t('Set to true if you also need to watch Mutations for Swiper parent elements.'),
  );
  // Callbacks
  $form['views_slideshow_swiper']['runCallbacksOnInit'] = array(
    '#type' => 'checkbox',
    '#description' => t('Run on[Transition/SlideChange][Start/End] callbacks on swiper initialization. Such callbacks will be fired on initialization in case of your initialSlide is not 0, or you use loop mode.'),
  );
  $form['views_slideshow_swiper']['onInit'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed right after Swiper initialization'),
  );
  $form['views_slideshow_swiper']['onSlideChangeStart'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed in the beginning of animation to other slide (next or previous). Receives swiper instance as an argument.'),
  );
  $form['views_slideshow_swiper']['onSlideChangeEnd'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed after animation to other slide (next or previous). Receives slider instance as an argument.'),
  );
  $form['views_slideshow_swiper']['onTransitionStart'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed in the beginning of transition. Receives swiper instance as an argument.'),
  );
  $form['views_slideshow_swiper']['onTransitionEnd'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed after transition. Receives slider instance as an argument.'),
  );
  $form['views_slideshow_swiper']['onTouchStart'] = array(
    '#type' => 'textarea',
    '#description' => t("Callback function with arguments, swiper, event, will be executed when user touch Swiper. Receives swiper instance and 'touchstart' event as an arguments."),
  );
  $form['views_slideshow_swiper']['onTouchMove'] = array(
    '#type' => 'textarea',
    '#description' => t("Callback function with arguments, swiper, event, will be executed when user touch and move finger over Swiper. Receives swiper instance and 'touchmove' event as an arguments."),
  );
  $form['views_slideshow_swiper']['onTouchMoveOpposite'] = array(
    '#type' => 'textarea',
    '#description' => t("Callback function with arguments, swiper, event, will be executed when user touch and move finger over Swiper in direction opposite to direction parameter. Receives swiper instance and 'touchmove' event as an arguments."),
  );
  $form['views_slideshow_swiper']['onSliderMove'] = array(
    '#type' => 'textarea',
    '#description' => t("Callback function with arguments, swiper, event, will be executed when user touch and move finger over Swiper and move it. Receives swiper instance and 'touchmove' event as an arguments."),
  );
  $form['views_slideshow_swiper']['onTouchEnd'] = array(
    '#type' => 'textarea',
    '#description' => t("Callback function with arguments, swiper, event, will be executed when user release Swiper. Receives swiper instance and 'touchend' event as an arguments."),
  );
  $form['views_slideshow_swiper']['onClick'] = array(
    '#type' => 'textarea',
    '#description' => t("Callback function with arguments, swiper, event, will be executed when user click/tap on Swiper after 300ms delay. Receives swiper instance and 'touchend' event as an arguments."),
  );
  $form['views_slideshow_swiper']['onTap'] = array(
    '#type' => 'textarea',
    '#description' => t("Callback function with arguments, swiper, event, will be executed when user click/tap on Swiper. Receives swiper instance and 'touchend' event as an arguments."),
  );
  $form['views_slideshow_swiper']['onDoubleTap'] = array(
    '#type' => 'textarea',
    '#description' => t("Callback function with arguments, swiper, event, will be executed when user double tap on Swiper's container. Receives swiper instance and 'touchend' event as an arguments"),
  );
  $form['views_slideshow_swiper']['onImagesReady'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed right after all inner images are loaded. updateOnImagesReady should be also enabled'),
  );
  $form['views_slideshow_swiper']['onProgress'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, progress, will be executed when Swiper progress is changed, as second arguments it receives progress that is always from 0 to 1'),
  );
  $form['views_slideshow_swiper']['onReachBeginning'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed when Swiper reach its beginning (initial position)'),
  );
  $form['views_slideshow_swiper']['onReachEnd'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed when Swiper reach last slide'),
  );
  $form['views_slideshow_swiper']['onDestroy'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed when you destroy Swiper'),
  );
  $form['views_slideshow_swiper']['onSetTranslate'] = array(
    '#type' => 'textarea',
    '#description' => t("Callback function with arguments, swiper, translate, will be executed when swiper's wrapper change its position. Receives swiper instance and current translate value as an arguments"),
  );
  $form['views_slideshow_swiper']['onSetTransition'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, transition, will be executed everytime when swiper starts animation. Receives swiper instance and current transition duration (in ms) as an arguments'),
  );
  $form['views_slideshow_swiper']['onAutoplayStart'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed when when autoplay started'),
  );
  $form['views_slideshow_swiper']['onAutoplayStop'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, will be executed when when autoplay stopped'),
  );
  $form['views_slideshow_swiper']['onLazyImageLoad'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, slide, image, will be executed in the beginning of lazy loading of image'),
  );
  $form['views_slideshow_swiper']['onLazyImageReady'] = array(
    '#type' => 'textarea',
    '#description' => t('Callback function with arguments, swiper, slide, image, will be executed when lazy loading image will be loaded'),
  );
  // Namespace
  $form['views_slideshow_swiper']['slideClass'] = array(
    '#type' => 'textfield',
    '#description' => t('CSS class name of slide.'),
  );
  $form['views_slideshow_swiper']['slideActiveClass'] = array(
    '#type' => 'textfield',
    '#description' => t('CSS class name of currently active slide.'),
  );
  $form['views_slideshow_swiper']['slideVisibleClass'] = array(
    '#type' => 'textfield',
    '#description' => t('CSS class name of currently visible slide.'),
  );
  $form['views_slideshow_swiper']['slideDuplicateClass'] = array(
    '#type' => 'textfield',
    '#description' => t('CSS class name of slide duplicated by loop mode.'),
  );
  $form['views_slideshow_swiper']['slideNextClass'] = array(
    '#type' => 'textfield',
    '#description' => t('CSS class name of slide which is right after currently active slide.'),
  );
  $form['views_slideshow_swiper']['slidePrevClass'] = array(
    '#type' => 'textfield',
    '#description' => t('CSS class name of slide which is right before currently active slide.'),
  );
  $form['views_slideshow_swiper']['wrapperClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of slides' wrapper."),
  );
  $form['views_slideshow_swiper']['bulletClass'] = array(
    '#type' => 'textfield',
    '#description' => t('CSS class name of single pagination bullet.'),
  );
  $form['views_slideshow_swiper']['bulletActiveClass'] = array(
    '#type' => 'textfield',
    '#description' => t('CSS class name of currently active pagination bullet.'),
  );
  $form['views_slideshow_swiper']['paginationHiddenClass'] = array(
    '#type' => 'textfield',
    '#description' => t('CSS class name of pagination when it becomes inactive.'),
  );
  $form['views_slideshow_swiper']['buttonDisabledClass'] = array(
    '#type' => 'textfield',
    '#description' => t('CSS class name of next/prev button when it becomes disabled.'),
  );

  $options = views_slideshow_swiper_views_slideshow_option_definition()['views_slideshow_swiper']['contains'];
  foreach (array_keys($options) as $key) {
    $title = ucfirst(strtolower(preg_replace('/([A-Z])/', ' $1', $key)));
    $current = $view->options['views_slideshow_swiper'][$key];
    $default = $options[$key]['default'];
    $form['views_slideshow_swiper'][$key]['#title'] = $title;
    $form['views_slideshow_swiper'][$key]['#default_value'] = (!isset($current) || is_null($current)) ? $default : $current;
  }
}

/**
 * Implements hook_views_slideshow_options_form_validate().
 */
function views_slideshow_swiper_views_slideshow_options_form_validate(&$form, &$form_state, &$view) {
  _views_slideshow_swiper_remove_default_values($form_state['values']['style_options']['views_slideshow_swiper']);
}

function _views_slideshow_swiper_remove_default_values(&$values) {
  // Get default values.
  $default_values = views_slideshow_swiper_views_slideshow_option_definition()['views_slideshow_swiper']['contains'];
  foreach ($values as $key => $value) {
    // Check if default value exists for current key.
    if (isset($default_values[$key]) && isset($default_values[$key]['default'])) {
      $default_value = $default_values[$key]['default'];
      // Typecast user-inputted value to default value's type for same-type equality comparison.
      settype($value, gettype($default_value));
      if ($value === $default_value) {
        // Unset user-inputted value if it is the same as the default value.
        unset($values[$key]);
      }
    }
  }
  return $values;
}
