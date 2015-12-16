<?php
/**
 * @file
 * Admin forms for RoyalSlider.
 */

/**
 * Submit handler for adding a new option set.
 */
function royalslider_form_optionset_add_submit($form, &$form_state) {
  $optionset = royalslider_optionset_create(array('name' => $form_state['values']['name'], 'title' => $form_state['values']['title']));

  $saved = royalslider_optionset_save($optionset, TRUE);

  if ($saved) {
    drupal_set_message(t('Option set %name was created.', array('%name' => $optionset->name)));
    $form_state['redirect'] = 'admin/config/media/royalslider/edit/' . $optionset->name;
  }
  else {
    drupal_set_message(t('Failed to create option set. Please verify your settings.'), 'error');
  }
}


/**
 * Defines the form elements used to edit the RoyalSlider library options
 *
 * @param array $options [optional]
 *  Pass in a set of default values for the options
 * @return array
 *  Returns the option set array
 */
function royalslider_option_elements($options = array()) {
  // Load defaults.
  $defaults = _royalslider_optionset_defaults();

  // For whatever reason, the only way to get #tree behavior to work here is by]
  // explicitly setting the #parents array. Also setting #tree to true in the
  // top-level form is causing a fatal error.
  $form = array();

  $form['intro'] = array(
    '#markup' => t("For more information on the RoyalSlider options, please refer to the <a href=\"@link\">official documentation</a>.", array('@link' => 'http://dimsemenov.com/plugins/royal-slider/documentation/')),
  );

  // General options.
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
  );

  $form['general']['loop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loop'),
    '#description' => t('Makes slider to go from last slide to first.'),
    '#default_value' => isset($options['loop'])
      ? $options['loop']
      : $defaults['loop'],
  );

  $form['general']['loopRewind'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loop rewind'),
    '#description' => t('Makes slider to go from last slide to first with rewind. Overrides previous option.'),
    '#default_value' => isset($options['loopRewind'])
      ? $options['loopRewind']
      : $defaults['loopRewind'],
  );

  $form['general']['randomizeSlides'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize slides'),
    '#description' => t('Randomizes all slides at start.'),
    '#default_value' => isset($options['randomizeSlides'])
      ? $options['randomizeSlides']
      : $defaults['randomizeSlides'],
  );

  $form['general']['usePreloader'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Preloader'),
    '#description' => t('Enables spinning preloader, you may style it via CSS (class rsPreloader)'),
    '#default_value' => isset($options['usePreloader'])
      ? $options['usePreloader']
      : $defaults['usePreloader'],
  );

  $form['general']['numImagesToPreload'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of images to preload'),
    '#description' => t('Number of images to preload on sides. If you set it to 0, only one slide will be kept in the display list at once.'),
    '#size' => 3,
    '#default_value' => isset($options['numImagesToPreload'])
      ? $options['numImagesToPreload']
      : $defaults['numImagesToPreload'],
    '#states' => array(
      'visible' => array(
        'input[name="usePreloader"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['general']['slidesOrientation'] = array(
    '#type' => 'textfield',
    '#title' => t('Slide orientation'),
    '#options' => array(
      'horizontal' => t('Horizontal'),
      'vertical' => t('Vertical'),
    ),
    '#default_value' => isset($options['slidesOrientation'])
      ? $options['slidesOrientation']
      : $defaults['slidesOrientation'],
  );

  $form['general']['slidesSpacing'] = array(
    '#title' => t('Slide spacing'),
    '#description' => t('Spacing between slides in pixels.'),
    '#type' => 'textfield',
    '#default_value' => isset($options['slidesSpacing'])
      ? $options['slidesSpacing']
      : $defaults['slidesSpacing'],
    '#size' => 3,
  );

  $form['general']['globalCaption'] = array(
    '#type' => 'checkbox',
    '#title' => t('Global caption'),
    '#description' => t('Adds global caption element to slider.'),
    '#default_value' => isset($options['globalCaption'])
      ? $options['globalCaption']
      : $defaults['globalCaption'],
  );

  // Full Screen mode.
  $form['fullscreen'] = array(
    '#type' => 'fieldset',
    '#title' => t('Full Screen'),
    '#tree' => TRUE,
  );

  $form['fullscreen']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Enable Full Screen mode.'),
    '#default_value' => isset($options['fullscreen']['enabled'])
      ? $options['fullscreen']['enabled']
      : $defaults['fullscreen']['enabled'],
    '#parents' => array('fullscreen', 'enabled'),
  );

  $form['fullscreen']['keyboardNav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force keyboard arrows nav in fullscreen'),
    '#default_value' => isset($options['fullscreen']['keyboardNav'])
      ? $options['fullscreen']['keyboardNav']
      : $defaults['fullscreen']['keyboardNav'],
    '#states' => array(
      'visible' => array(
        'input[name="fullscreen[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('fullscreen', 'keyboardNav'),
  );

  $form['fullscreen']['buttonFS'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fullscreen button at top right'),
    '#default_value' => isset($options['fullscreen']['buttonFS'])
      ? $options['fullscreen']['buttonFS']
      : $defaults['fullscreen']['buttonFS'],
    '#states' => array(
      'visible' => array(
        'input[name="fullscreen[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('fullscreen', 'buttonFS'),
  );

  $form['fullscreen']['nativeFS'] = array(
    '#type' => 'checkbox',
    '#title' => t('Native browser full screen.'),
    '#description' => t('Enable native browser full screen mode, if available.'),
    '#default_value' => isset($options['fullscreen']['nativeFS'])
      ? $options['fullscreen']['nativeFS']
      : $defaults['fullscreen']['nativeFS'],
    '#states' => array(
      'visible' => array(
        'input[name="fullscreen[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('fullscreen', 'nativeFS'),
  );

  // Navigation.
  $form['navigation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navigation'),
  );

  $form['navigation']['controlNavigation'] = array(
    '#title' => t('Navigation type'),
    '#type' => 'select',
    '#options' => array(
      'none' => t('None'),
      'bullets' => t('Bullets'),
      'tabs' => t('Tabs'),
      'thumbnails' => t('Thumbnails'),
    ),
    '#default_value' => isset($options['controlNavigation'])
      ? $options['controlNavigation']
      : $defaults['controlNavigation'],
  );

  $form['navigation']['controlsInside'] = array(
    '#title' => t('Controls inside'),
    '#description' => t('If set to true adds arrows and fullscreen button inside rsOverflow container, otherwise inside root slider container.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['controlsInside'])
      ? $options['controlsInside']
      : $defaults['controlsInside'],
  );

  $form['navigation']['sliderDrag'] = array(
    '#title' => t('Mouse drag navigation over slider'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['sliderDrag'])
      ? $options['sliderDrag']
      : $defaults['sliderDrag'],
  );

  $form['navigation']['sliderTouch'] = array(
    '#title' => t('Touch navigation of slider'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['sliderTouch'])
      ? $options['sliderTouch']
      : $defaults['sliderTouch'],
  );

  $form['navigation']['keyboardNavEnabled'] = array(
    '#title' => t('Keyboard navigation'),
    '#description' => t('Navigate slider with keyboard left and right arrows.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['keyboardNavEnabled'])
      ? $options['keyboardNavEnabled']
      : $defaults['keyboardNavEnabled'],
  );

  $form['navigation']['navigateByClick'] = array(
    '#title' => t('Click navigation'),
    '#description' => t('Navigates forward by clicking on slide.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['navigateByClick'])
      ? $options['navigateByClick']
      : $defaults['navigateByClick'],
  );

  $form['navigation']['arrowsNav'] = array(
    '#title' => t('Navigation arrows'),
    '#description' => t('Direction arrows navigation'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['arrowsNav'])
      ? $options['arrowsNav']
      : $defaults['arrowsNav'],
  );

  $form['navigation']['arrowsNavAutoHide'] = array(
    '#title' => t('Auto hide arrows'),
    '#description' => t('Auto hide arrows when focus is lost'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['arrowsNavAutoHide'])
      ? $options['arrowsNavAutoHide']
      : $defaults['arrowsNavAutoHide'],
    '#states' => array(
      'visible' => array(
        'input[name="arrowsNav"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['navigation']['arrowsNavHideOnTouch'] = array(
    '#title' => t('Hide on touch'),
    '#description' => t('Hide arrows completely on touch devices'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['arrowsNavHideOnTouch'])
      ? $options['arrowsNavHideOnTouch']
      : $defaults['arrowsNavHideOnTouch'],
    '#states' => array(
      'visible' => array(
        'input[name="arrowsNav"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Thumbnails.
  $form['thumbs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thumbnails'),
    '#description' => t('These settings will only apply if Navigation type is set to "thumbnails".'),
    '#tree' => TRUE,
  );

  $form['thumbs']['drag'] = array(
    '#title' => t('Thumbnails mouse drag'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['thumbs']['drag'])
      ? $options['thumbs']['drag']
      : $defaults['thumbs']['drag'],
    '#parents' => array('thumbs', 'drag'),
  );

  $form['thumbs']['touch'] = array(
    '#title' => t('Thumbnails touch'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['thumbs']['touch'])
      ? $options['thumbs']['touch']
      : $defaults['thumbs']['touch'],
    '#parents' => array('thumbs', 'touch'),
  );

  $form['thumbs']['orientation'] = array(
    '#title' => t('Orientation'),
    '#type' => 'select',
    '#options' => array(
      'horizontal' => t('Horizontal'),
      'vertical' => t('Vertical'),
    ),
    '#default_value' => isset($options['thumbs']['orientation'])
      ? $options['thumbs']['orientation']
      : $defaults['thumbs']['orientation'],
    '#parents' => array('thumbs', 'orientation'),
  );

  $form['thumbs']['arrows'] = array(
    '#title' => t('Thumbnail arrows'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['thumbs']['arrows'])
      ? $options['thumbs']['arrows']
      : $defaults['thumbs']['arrows'],
    '#parents' => array('thumbs', 'arrows'),
  );

  $form['thumbs']['spacing'] = array(
    '#title' => t('Spacing between thumbs'),
    '#type' => 'textfield',
    '#default_value' => isset($options['thumbs']['spacing'])
      ? $options['thumbs']['spacing']
      : $defaults['thumbs']['spacing'],
    '#size' => 3,
    '#parents' => array('thumbs', 'spacing'),
  );

  $form['thumbs']['arrowsAutoHide'] = array(
    '#title' => t('Auto hide arrows'),
    '#description' => t('Auto hide thumbnails arrows on hover.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['thumbs']['arrowsAutoHide'])
      ? $options['thumbs']['arrowsAutoHide']
      : $defaults['thumbs']['arrowsAutoHide'],
    '#parents' => array('thumbs', 'arrowsAutoHide'),
  );

  $form['thumbs']['autoCenter'] = array(
    '#title' => t('Auto center'),
    '#description' => t('Automatically centers container with thumbs if there are small number of items.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['thumbs']['autoCenter'])
      ? $options['thumbs']['autoCenter']
      : $defaults['thumbs']['autoCenter'],
    '#parents' => array('thumbs', 'autoCenter'),
  );

  $form['thumbs']['transitionSpeed'] = array(
    '#title' => t('Transition speed'),
    '#description' => t('Thumbnails transition speed, in milliseconds.'),
    '#type' => 'textfield',
    '#default_value' => isset($options['thumbs']['transitionSpeed'])
      ? $options['thumbs']['transitionSpeed']
      : $defaults['thumbs']['transitionSpeed'],
    '#size' => 5,
    '#parents' => array('thumbs', 'transitionSpeed'),
  );

  $form['thumbs']['fitInViewport'] = array(
    '#title' => t('Fit in viewport'),
    '#description' => t('Reduces size of main viewport area by thumbnails width or height, use it when you set 100% width to slider. This option is always true, when slider is in fullscreen mode.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['thumbs']['fitInViewport'])
      ? $options['thumbs']['fitInViewport']
      : $defaults['thumbs']['fitInViewport'],
    '#parents' => array('thumbs', 'fitInViewport'),
  );

  $form['thumbs']['firstMargin'] = array(
    '#title' => t('First margin'),
    '#description' => t('Margin that equals thumbs spacing for first and last item.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['thumbs']['firstMargin'])
      ? $options['thumbs']['firstMargin']
      : $defaults['thumbs']['firstMargin'],
    '#parents' => array('thumbs', 'firstMargin'),
  );

  $form['thumbs']['arrowLeft'] = array(
    '#title' => t('Replace left thumbnail arrow'),
    '#description' => t('Replaces default thumbnail arrow. Variable accepts jQuery element $(\'This is left arrow\') that will be used as arrow. You have to add it to DOM manually.'),
    '#type' => 'textfield',
    '#default_value' => isset($options['thumbs']['arrowLeft'])
      ? $options['thumbs']['arrowLeft']
      : $defaults['thumbs']['arrowLeft'],
    '#parents' => array('thumbs', 'arrowLeft'),
  );

  $form['thumbs']['arrowRight'] = array(
    '#title' => t('Replace right thumbnail arrow'),
    '#description' => t('Replaces default thumbnail arrow. Variable accepts jQuery element $(\'This is left arrow\') that will be used as arrow. You have to add it to DOM manually.'),
    '#type' => 'textfield',
    '#default_value' => isset($options['thumbs']['arrowRight'])
      ? $options['thumbs']['arrowRight']
      : $defaults['thumbs']['arrowRight'],
    '#parents' => array('thumbs', 'arrowRight'),
  );

  $form['thumbs']['appendSpan'] = array(
    '#title' => t('Append Span'),
    '#description' => t('Adds span element with class "thumbIco" to every thumbnail. Useful for styling.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['thumbs']['appendSpan'])
      ? $options['thumbs']['appendSpan']
      : $defaults['thumbs']['appendSpan'],
    '#parents' => array('thumbs', 'appendSpan'),
  );

  // Transitions.
  $form['transitions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transitions'),
  );

  $form['transitions']['transitionType'] = array(
    '#title' => t('Transition type'),
    '#description' => t('Important note about fade transition, slides must have background as only one image is animating.'),
    '#type' => 'select',
    '#options' => array(
      'move' => t('Move'),
      'fade' => t('Fade'),
    ),
    '#default_value' => isset($options['transitionType'])
      ? $options['transitionType']
      : $defaults['transitionType'],
  );

  $form['transitions']['transitionSpeed'] = array(
    '#title' => t('Transition speed'),
    '#description' => t('Slider transition speed, in milliseconds.'),
    '#type' => 'textfield',
    '#default_value' => isset($options['transitionSpeed'])
      ? $options['transitionSpeed']
      : $defaults['transitionSpeed'],
    '#size' => 5,
  );

  // Built-in easing options.
  $easing_options = array(
    'linear' => 'linear',
    'easeOutSine' => 'easeOutSine',
    'easeInOutSine' => 'easeInOutSine',
  );

  // Support the jquery.easing plugin via the jQuery Easing module.
  if (module_exists('jqeasing')) {
    $easing_options = array_merge($easing_options, _royalslider_jqeasing_options());
  }

  // Sort easing methods.
  ksort($easing_options);

  $form['transitions']['easeInOut'] = array(
    '#title' => t('Easing function for simple transition'),
    '#type' => 'select',
    '#options' => $easing_options,
    '#default_value' => isset($options['easeInOut'])
      ? $options['easeInOut']
      : $defaults['easeInOut'],
  );

  $form['transitions']['easeOut'] = array(
    '#title' => t('Easing function of animation after ending of the swipe gesture'),
    '#type' => 'select',
    '#options' => $easing_options,
    '#default_value' => isset($options['easeOut'])
      ? $options['easeOut']
      : $defaults['easeOut'],
  );

  $form['transitions']['allowCSS3'] = array(
    '#title' => t('Allows usage of CSS3 transitions'),
    '#description' => t('Might be useful if you\'re experiencing font-rendering problems, or other CSS3-related bugs. <strong>Don\'t use enable this if you\'re using the jQuery Easing module.</strong>'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['allowCSS3'])
      ? $options['allowCSS3']
//      : $defaults['allowCSS3'],
      : FALSE,
    // We set this to FALSE by default, despite the RoyalSlider API defaults
    // because we support the jQuery Easing Drupal module and, by using CSS3
    // transitions, we effectively remove the ability to use JS easing.
  );

  $form['transitions']['addActiveClass'] = array(
    '#title' => t('Add active class'),
    '#description' => t('Adds rsActiveSlide class to current slide before transition.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['addActiveClass'])
      ? $options['addActiveClass']
      : $defaults['addActiveClass'],
  );

  $form['transitions']['fadeinLoadedSlide'] = array(
    '#title' => t('Fades in slides'),
    '#description' => t('Fades in slide after it\'s loaded'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['fadeinLoadedSlide'])
      ? $options['fadeinLoadedSlide']
      : $defaults['fadeinLoadedSlide'],
  );

  // Dimensions.
  $form['dimensions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dimensions'),
  );

  $form['dimensions']['autoScaleSlider'] = array(
    '#title' => t('Auto scale slider'),
    '#description' => t('Automatically updates slider height based on base width.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['autoScaleSlider'])
      ? $options['autoScaleSlider']
      : $defaults['autoScaleSlider'],
  );

  $form['dimensions']['drupalAutoSetSliderDimensions'] = array(
    '#title' => t('Automatically calculate slider dimensions'),
    '#description' => t('This is a Drupal-specific option. We can calculate the first image\'s dimensions and set the slider dimensions automatically. Note, if your images have different sizes, this might not work as expected.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['drupalAutoSetSliderDimensions'])
      ? $options['drupalAutoSetSliderDimensions']
      : $defaults['drupalAutoSetSliderDimensions'],
    '#states' => array(
      'visible' => array(
        'input[name="autoScaleSlider"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['dimensions']['autoScaleSliderWidth'] = array(
    '#title' => t('Base slider width'),
    '#description' => t('Slider will autocalculate the ratio based on these values.'),
    '#type' => 'textfield',
    '#default_value' => isset($options['autoScaleSliderWidth'])
      ? $options['autoScaleSliderWidth']
      : $defaults['autoScaleSliderWidth'],
    '#states' => array(
      'visible' => array(
        'input[name="autoScaleSlider"]' => array('checked' => TRUE),
        'input[name="drupalAutoSetSliderDimensions"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['dimensions']['autoScaleSliderHeight'] = array(
    '#title' => t('Base slider height'),
    '#description' => t('Slider will autocalculate the ratio based on these values.'),
    '#type' => 'textfield',
    '#default_value' => isset($options['autoScaleSliderHeight'])
      ? $options['autoScaleSliderHeight']
      : $defaults['autoScaleSliderHeight'],
    '#states' => array(
      'visible' => array(
        'input[name="autoScaleSlider"]' => array('checked' => TRUE),
        'input[name="drupalAutoSetSliderDimensions"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['dimensions']['imageScaleMode'] = array(
    '#title' => t('Scale mode for images'),
    '#type' => 'select',
    '#options' => array(
      'none' => t('None'),
      'fill' => t('Fill'),
      'fit' => t('Fit'),
      'fit-if-smaller' => t('Fit if smaller'),
    ),
    '#default_value' => isset($options['imageScaleMode'])
      ? $options['imageScaleMode']
      : $defaults['imageScaleMode'],
  );

  $form['dimensions']['imageScalePadding'] = array(
    '#title' => t('Scale padding'),
    '#description' => t('Distance between image and edge of slide (doesn\'t work with \'fill\' scale mode).'),
    '#type' => 'textfield',
    '#default_value' => isset($options['imageScalePadding'])
      ? $options['imageScalePadding']
      : $defaults['imageScalePadding'],
    '#size' => 3,
  );

  $form['dimensions']['imageAlignCenter'] = array(
    '#title' => t('Aligns image to center of slide'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['imageAlignCenter'])
      ? $options['imageAlignCenter']
      : $defaults['imageAlignCenter'],
  );
  
  $form['dimensions']['autoHeight'] = array(
    '#title' => t('Auto height'),
    '#description' => t('Auto height option. <em>Please note that autoHeight option has some conflicts with options like imageScaleMode, imageAlignCenter and autoScaleSlider.</em>'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['autoHeight'])
      ? $options['autoHeight']
      : $defaults['autoHeight'],
  );

  $form['dimensions']['drupalAutoSetImageDimensions'] = array(
    '#title' => t('Automatically calculate image dimensions'),
    '#description' => t('This is a Drupal-specific option. We can calculate each image\'s dimensions and set the RoyalSlider attributes automatically.'),
    '#type' => 'checkbox',
    '#default_value' => isset($options['drupalAutoSetImageDimensions'])
      ? $options['drupalAutoSetImageDimensions']
      : $defaults['drupalAutoSetImageDimensions'],
  );

  $form['dimensions']['imgWidth'] = array(
    '#title' => t('Image width'),
    '#description' => t('Adds base width to all images for better-looking loading.'),
    '#type' => 'textfield',
    '#default_value' => isset($options['imgWidth'])
      ? $options['imgWidth']
      : $defaults['imgWidth'],
    '#size' => 5,
    '#states' => array(
      'visible' => array(
        'input[name="drupalAutoSetImageDimensions"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['dimensions']['imgHeight'] = array(
    '#title' => t('Image height'),
    '#description' => t('Adds base height to all images for better-looking loading.'),
    '#type' => 'textfield',
    '#default_value' => isset($options['imgHeight'])
      ? $options['imgHeight']
      : $defaults['imgHeight'],
    '#size' => 5,
    '#states' => array(
      'visible' => array(
        'input[name="drupalAutoSetImageDimensions"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Autoplay.
  $form['autoplay'] = array(
    '#type' => 'fieldset',
    '#title' => t('Autoplay'),
    '#tree' => TRUE,
  );

  $form['autoplay']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Enable autoplay'),
    '#default_value' => isset($options['autoplay']['enabled'])
      ? $options['autoplay']['enabled']
      : $defaults['autoplay']['enabled'],
    '#parents' => array('autoplay', 'enabled'),
  );

  $form['autoplay']['stopAtAction'] = array(
    '#type' => 'checkbox',
    '#title' => t('Stop autoplay at first user action'),
    '#default_value' => isset($options['autoplay']['stopAtAction'])
      ? $options['autoplay']['stopAtAction']
      : $defaults['autoplay']['stopAtAction'],
    '#states' => array(
      'visible' => array(
        'input[name="autoplay[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('autoplay', 'stopAtAction'),
  );

  $form['autoplay']['pauseOnHover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause autoplay on hover'),
    '#default_value' => isset($options['autoplay']['pauseOnHover'])
      ? $options['autoplay']['pauseOnHover']
      : $defaults['autoplay']['pauseOnHover'],
    '#states' => array(
      'visible' => array(
        'input[name="autoplay[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('autoplay', 'pauseOnHover'),
  );

  $form['autoplay']['delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay'),
    '#description' => t('Delay between items, in milliseconds.'),
    '#size' => 5,
    '#default_value' => isset($options['autoplay']['delay'])
      ? $options['autoplay']['delay']
      : $defaults['autoplay']['delay'],
    '#states' => array(
      'visible' => array(
        'input[name="autoplay[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('autoplay', 'delay'),
  );

  // Visible Nearby.
  $form['visibleNearby'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visible-nearby'),
    '#tree' => TRUE,
  );

  $form['visibleNearby']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Enable visible-nearby'),
    '#default_value' => isset($options['visibleNearby']['enabled'])
      ? $options['visibleNearby']['enabled']
      : $defaults['visibleNearby']['enabled'],
    '#parents' => array('visibleNearby', 'enabled'),
  );

  $form['visibleNearby']['centerArea'] = array(
    '#type' => 'textfield',
    '#title' => t('Center area'),
    '#description' => t('Ratio that determines area of center image. For example for 0.6 - 60% of slider area will get center image and 20% for two images on sides.'),
    '#size' => 3,
    '#default_value' => isset($options['visibleNearby']['centerArea'])
      ? $options['visibleNearby']['centerArea']
      : $defaults['visibleNearby']['centerArea'],
    '#states' => array(
      'visible' => array(
        'input[name="visibleNearby[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('visibleNearby', 'centerArea'),
  );

  $form['visibleNearby']['center'] = array(
    '#type' => 'checkbox',
    '#title' => t('Center'),
    '#description' => t('Alignment of center image, if you set it to false center image will be aligned to left.'),
    '#default_value' => isset($options['visibleNearby']['center'])
      ? $options['visibleNearby']['center']
      : $defaults['visibleNearby']['center'],
    '#states' => array(
      'visible' => array(
        'input[name="visibleNearby[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('visibleNearby', 'center'),
  );

  $form['visibleNearby']['navigateByCenterClick'] = array(
    '#type' => 'checkbox',
    '#title' => t('Navigate by center click'),
    '#description' => t('Disables navigation to next slide by clicking on current slide.'),
    '#default_value' => isset($options['visibleNearby']['navigateByCenterClick'])
      ? $options['visibleNearby']['navigateByCenterClick']
      : $defaults['visibleNearby']['navigateByCenterClick'],
    '#states' => array(
      'visible' => array(
        'input[name="visibleNearby[enabled]"]' => array('checked' => TRUE),
        'input[name="navigateByClick"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('visibleNearby', 'navigateByCenterClick'),
  );

  $form['visibleNearby']['breakpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Breakpoint'),
    '#description' => t('Used for responsive design. Changes centerArea value to breakpointCenterArea when width of slider is less then value in this option. Set to 0 to disable. Should be number.'),
    '#size' => 5,
    '#default_value' => isset($options['visibleNearby']['breakpoint'])
      ? $options['visibleNearby']['breakpoint']
      : $defaults['visibleNearby']['breakpoint'],
    '#states' => array(
      'visible' => array(
        'input[name="visibleNearby[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('visibleNearby', 'breakpoint'),
  );

  $form['visibleNearby']['breakpointCenterArea'] = array(
    '#type' => 'textfield',
    '#title' => t('Breakpoint center area'),
    '#description' => t('Same as center area option, just for breakpoint. Can be changed dynamically via \'sliderInstance.st.breakpointCenterArea\'.'),
    '#size' => 5,
    '#default_value' => isset($options['visibleNearby']['breakpointCenterArea'])
      ? $options['visibleNearby']['breakpointCenterArea']
      : $defaults['visibleNearby']['breakpointCenterArea'],
    '#states' => array(
      'visible' => array(
        'input[name="visibleNearby[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('visibleNearby', 'breakpointCenterArea'),
  );


  // Deep linking.
  $form['deeplinking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Deep linking'),
    '#tree' => TRUE,
  );

  $form['deeplinking']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Linking to slides by appending #SLIDE_INDEX to url. Slides count starts from 1. If change is set to false hash is only read once, after page load.'),
    '#default_value' => isset($options['deeplinking']['enabled'])
      ? $options['deeplinking']['enabled']
      : $defaults['deeplinking']['enabled'],
    '#parents' => array('deeplinking', 'enabled'),
  );

  $form['deeplinking']['change'] = array(
    '#type' => 'checkbox',
    '#title' => t('Change'),
    '#description' => t('Automatically change URL after transition and listen for hash change.'),
    '#default_value' => isset($options['deeplinking']['change'])
      ? $options['deeplinking']['change']
      : $defaults['deeplinking']['change'],
    '#states' => array(
      'visible' => array(
        'input[name="deeplinking[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('deeplinking', 'change'),
  );

  $form['deeplinking']['prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#description' => t('Prefix that will be added to hash. For example if you set it to "gallery-", hash would look like this: #gallery-5.'),
    '#size' => 5,
    '#default_value' => isset($options['deeplinking']['prefix'])
      ? $options['deeplinking']['prefix']
      : $defaults['deeplinking']['prefix'],
    '#states' => array(
      'visible' => array(
        'input[name="deeplinking[enabled]"]' => array('checked' => TRUE),
      ),
    ),
    '#parents' => array('deeplinking', 'prefix'),
  );

  // Video.
  $form['video'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video'),
    '#tree' => TRUE,
  );

  $form['video']['autoHideArrows'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto hide arrows'),
    '#description' => t('Auto hide arrows when video is playing.'),
    '#default_value' => isset($options['video']['autoHideArrows'])
      ? $options['video']['autoHideArrows']
      : $defaults['video']['autoHideArrows'],
    '#parents' => array('video', 'autoHideArrows'),
  );

  $form['video']['autoHideControlNav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto hide navigation'),
    '#description' => t('Auto hide navigation when video is playing.'),
    '#default_value' => isset($options['video']['autoHideControlNav'])
      ? $options['video']['autoHideControlNav']
      : $defaults['video']['autoHideControlNav'],
    '#parents' => array('video', 'autoHideControlNav'),
  );

  $form['video']['autoHideBlocks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto hide blocks'),
    '#description' => t('Auto hide animated blocks when video is playing.'),
    '#default_value' => isset($options['video']['autoHideBlocks'])
      ? $options['video']['autoHideBlocks']
      : $defaults['video']['autoHideBlocks'],
    '#parents' => array('video', 'autoHideBlocks'),
  );

  $form['video']['youTubeCode'] = array(
    '#type' => 'textarea',
    '#title' => t('YouTube code'),
    '#description' => t('YouTube embed code. %id% is replaced by video id.'),
    '#size' => 5,
    '#default_value' => isset($options['video']['youTubeCode'])
      ? $options['video']['youTubeCode']
      : $defaults['video']['youTubeCode'],
    '#parents' => array('video', 'youTubeCode'),
  );

  $form['video']['vimeoCode'] = array(
    '#type' => 'textarea',
    '#title' => t('Vimeo code'),
    '#description' => t('Vimeo embed code. %id% is replaced by video id.'),
    '#size' => 5,
    '#default_value' => isset($options['video']['vimeoCode'])
      ? $options['video']['vimeoCode']
      : $defaults['video']['vimeoCode'],
    '#parents' => array('video', 'vimeoCode'),
  );

  return $form;
}

/**
 * Form builder; Form to edit a given option set.
 */
function royalslider_form_optionset_edit($form, &$form_state) {

  if (empty($form_state['optionset'])) {
    $optionset = royalslider_optionset_create();
  }
  else {
    $optionset = $form_state['optionset'];
  }

  // Title
  $form['title'] = array(
    '#type' => 'textfield',
    '#maxlength' => '255',
    '#title' => t('Title'),
    '#description' => t('A human-readable title for this option set.'),
    '#required' => TRUE,
    '#default_value' => $optionset->title,
  );

  // Machine name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => '255',
    '#machine_name' => array(
      'source' => array('title'),
      'exists' => 'royalslider_optionset_exists',
    ),
    '#required' => TRUE,
    '#default_value' => $optionset->name,
  );

  // Skin.
  $skins = array();

  foreach(royalslider_skins() as $name => $info) {
    $skins[$name] = $info['name'];
  }

  $form['skin'] = array(
    '#type' => 'select',
    '#title' => t('Skin'),
    '#options' => $skins,
    '#default_value' => $optionset->skin,
  );

  // Show select boxes for the various image styles.
  $image_style = image_style_options(FALSE);

  $form['image_style'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image styles'),
    '#tree' => TRUE,
  );
  $form['image_style']['fullscreen'] = array(
    '#type' => 'select',
    '#title' => t('Full Screen image style'),
    '#description' => t('Image style for the full screen images.'),
    '#empty_option' => t('None (original image)'),
    '#options' => $image_style,
    '#default_value' => $optionset->imagestyle_fullscreen,
    '#states' => array(
      'visible' => array(
        'input[name="fullscreen[enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['image_style']['normal'] = array(
    '#type' => 'select',
    '#title' => t('Normal image style'),
    '#description' => t('Image style for the main stage images.'),
    '#empty_option' => t('None (original image)'),
    '#options' => $image_style,
    '#default_value' => $optionset->imagestyle_normal,
  );
  $form['image_style']['thumbnail'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnail image style'),
    '#description' => t('Image style for the thumbnail images.'),
    '#empty_option' => t('None (original image)'),
    '#options' => $image_style,
    '#default_value' => $optionset->imagestyle_thumbnail,
    '#states' => array(
      'visible' => array(
        'select[name="controlNavigation"]' => array('value' => 'thumbnails'),
      ),
    ),
  );

  // Options Vertical Tab Group table
  $form['options'] = array(
    '#type' => 'vertical_tabs',
  );

  $default_options = royalslider_option_elements($optionset->options);
  // Add the options to the vertical tabs section
  foreach ($default_options as $key => $value) {
    $form['options'][] = $value;
  }

  return $form;
}

/**
 * Form builder; Form to delete a given option set.
 */
function royalslider_optionset_form_delete($form, &$form_state, $optionset) {
  $form_state['optionset'] = &$optionset;

  // Deleting an export in code will revert it.
  $op = ($optionset->export_type & EXPORT_IN_CODE) ? 'Revert' : 'Delete';

  return confirm_form(
    $form,
    t('Are you sure you want to @action the option set %name?', array('@action' => t(drupal_strtolower($op)), '%name' => $optionset->name)),
    'admin/config/media/royalslider',
    NULL,
    t($op),  t('Cancel')
  );
}

/**
 * Submit handler for deleting an option set.
 */
function royalslider_optionset_form_delete_submit($form, &$form_state) {
  $optionset = &$form_state['optionset'];
  $op = ($optionset->export_type & EXPORT_IN_CODE) ? 'reverted' : 'deleted';

  ctools_include('export');
  ctools_export_crud_delete('royalslider_optionset', $optionset);

  drupal_set_message(t('Option set %name was ' . $op . '.', array('%name' => $optionset->name)));
  $form_state['redirect'] = 'admin/config/media/royalslider';
}
