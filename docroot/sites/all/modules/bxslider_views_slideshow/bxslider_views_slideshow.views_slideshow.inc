<?php
/** @file
 * The default options available with Views Slideshow: BxSlider.
 */

/**
 * Define the type of the slideshow (eg.: cycle, imageflow, ddblock).
 *
 * @return Array
 *   Associative array of slideshow type and its information.
 */
function bxslider_views_slideshow_views_slideshow_slideshow_info() {
  $options = array(
    'bxslider_views_slideshow' => array(
      'name' => t('BxSlider'),
      'accepts' => array(
        'goToSlide',
        'nextSlide',
        'previousSlide',
      ),
      'calls' => array(
        'goToSlide',
        'nextSlide',
        'previousSlide',
      ),
    ),
  );
  return $options;
}


/**
 * Implements hook_views_slideshow_options_form().
 */
function bxslider_views_slideshow_views_slideshow_slideshow_type_form(&$form, &$form_state, &$view) {
  if (!$bxslider_path = _bxslider_views_slideshow_library_path()) {
    drupal_set_message(t('You need to install the BxSlider plugin.
     You can find the plugin at !url.',
      array('!url' => l('http://bxslider.com',
        t('http://bxslider.com'),
        array('attributes' => array('target' => '_blank'))))), 'error');
  }

  // Create the form elements for our BxSlider view options.
  $form['bxslider_views_slideshow'] = array(
    '#type' => 'fieldset',
    '#title' => t('BxSlider options'),
    '#description' => t("These options affect the slideshow when it's in the
    !bxslider mode. Please see the !documentation for more information.",
      array('!bxslider' => l(t('BxSlider'), 'http://bxslider.com',
        array('attributes' => array('target' => 'blank'))),
        '!documentation' => l(t('documentation'),
          'http://bxslider.com/options',
          array('attributes' => array('target' => '_blank'))))),
    '#collapsible' => TRUE,
    '#collapsed' => !(isset($view->options['mode'])
      && $view->options['mode'] == 'bxslider_views_slideshow'),
  );
  $form['bxslider_views_slideshow']['fixes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fixes'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['bxslider_views_slideshow']['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['bxslider_views_slideshow']['controlsfieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controls'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['bxslider_views_slideshow']['pagerfieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pager'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['bxslider_views_slideshow']['autofieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Auto'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['bxslider_views_slideshow']['carousel'] = array(
    '#type' => 'fieldset',
    '#title' => t('Carousel'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['bxslider_views_slideshow']['callback'] = array(
    '#type' => 'fieldset',
    '#title' => t('Callbacks'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Fixes.
  $form['bxslider_views_slideshow']['fixes']['height_does_not_dyn_change'] = array(
    '#type' => 'checkbox',
    '#title' => t('Height does not change when resizing window'),
    '#description' => t('For more information visit: https://drupal.org/node/1990682'),
    '#default_value' =>
    isset($view->options['bxslider_views_slideshow']['fixes']['height_does_not_dyn_change'])
      ? $view->options['bxslider_views_slideshow']['fixes']['height_does_not_dyn_change'] : FALSE,
  );
  // Form.
  $form['bxslider_views_slideshow']['general']['mode'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#description' => t('Type of transition between slides'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['mode'])
      ? $view->options['bxslider_views_slideshow']['general']['mode'] : 'horizontal',
    '#options' => array(
      'horizontal' => t('horizontal'),
      'vertical' => t('vertical'),
      'fade' => t('fade'),
    ),
  );
  $form['bxslider_views_slideshow']['general']['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#description' => t('Slide transition duration (in ms)'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['speed'])
      ? $view->options['bxslider_views_slideshow']['general']['speed'] : 500,
  );
  $form['bxslider_views_slideshow']['general']['slideMargin'] = array(
    '#type' => 'textfield',
    '#title' => t('Slide margin'),
    '#description' => t('Margin between each slide.'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['slideMargin'])
      ? $view->options['bxslider_views_slideshow']['general']['slideMargin'] : 0,
  );
  $form['bxslider_views_slideshow']['general']['startSlide'] = array(
    '#type' => 'textfield',
    '#title' => t('Start slide'),
    '#description' => t('Starting slide index (zero-based)'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['startSlide'])
      ? $view->options['bxslider_views_slideshow']['general']['startSlide'] : 0,
  );
  $form['bxslider_views_slideshow']['general']['randomStart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Random start'),
    '#description' => t('Start slider on a random slide'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['randomStart'])
      ? $view->options['bxslider_views_slideshow']['general']['randomStart'] : FALSE,
  );
  $form['bxslider_views_slideshow']['general']['infiniteLoop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inifinite loop'),
    '#description' => t('If true, clicking "Next" while on the last slide will transition to the first slide and vice-versa'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['infiniteLoop']) ?
      $view->options['bxslider_views_slideshow']['general']['infiniteLoop'] : TRUE,
  );
  $form['bxslider_views_slideshow']['general']['hideControlOnEnd'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide controls on end ?'),
    '#description' => t('If true, "Next" control will be hidden on last slide and vice-versa
Note: Only used when infiniteLoop: false'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['hideControlOnEnd'])
      ? $view->options['bxslider_views_slideshow']['general']['hideControlOnEnd'] : FALSE,
  );
  $form['bxslider_views_slideshow']['general']['captions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Captions'),
    '#description' => t('Include image captions. Captions are derived from the image\'s title attribute'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['captions'])
      ? $view->options['bxslider_views_slideshow']['general']['captions'] : FALSE,
  );
  $form['bxslider_views_slideshow']['general']['ticker'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ticker mode'),
    '#description' => t('Use slider in ticker mode (similar to a news ticker)'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['ticker'])
      ? $view->options['bxslider_views_slideshow']['general']['ticker'] : FALSE,
  );
  $form['bxslider_views_slideshow']['general']['tickerHover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ticker hover'),
    '#description' => t('Ticker will pause when mouse hovers over slider. Note: this functionality does NOT work if using CSS transitions!'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['tickerHover'])
      ? $view->options['bxslider_views_slideshow']['general']['tickerHover'] : FALSE,
  );
  $form['bxslider_views_slideshow']['general']['adaptiveHeight'] = array(
    '#type' => 'checkbox',
    '#title' => t('Adaptive height'),
    '#description' => t('Dynamically adjust slider height based on each slide\'s height'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['adaptiveHeight'])
      ? $view->options['bxslider_views_slideshow']['general']['adaptiveHeight'] : FALSE,
  );
  $form['bxslider_views_slideshow']['general']['adaptiveHeightSpeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Adaptive height speed'),
    '#description' => t('Slide height transition duration (in ms). Note: only used if adaptiveHeight: true'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['adaptiveHeightSpeed'])
      ? $view->options['bxslider_views_slideshow']['general']['adaptiveHeightSpeed'] : 500,
  );
  $form['bxslider_views_slideshow']['general']['video'] = array(
    '#type' => 'checkbox',
    '#title' => t('Video'),
    '#description' => t('If any slides contain video, set this to true. Also, include plugins/jquery.fitvids.js
See http://fitvidsjs.com/ for more info'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['video'])
      ? $view->options['bxslider_views_slideshow']['general']['video'] : FALSE,
  );
  $form['bxslider_views_slideshow']['general']['touchEnabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Touch enabled'),
    '#description' => t('If true, slider will allow touch swipe transitions'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['touchEnabled'])
      ? $view->options['bxslider_views_slideshow']['general']['touchEnabled'] : TRUE,
  );
  $form['bxslider_views_slideshow']['general']['preloadImages'] = array(
    '#type' => 'select',
    '#title' => t('Preload all images?'),
    '#description' => t('If all, preloads all images before starting the slider. If visible, preloads only images in the initially visible slides before starting the slider (tip: use visible if all slides are identical dimensions)'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['preloadImages'])
      ? $view->options['bxslider_views_slideshow']['general']['preloadImages'] : 'visisble',
    '#options' => array(
      'all' => t('all'),
      'visible' => t('visible'),
    ),
  );
  $form['bxslider_views_slideshow']['general']['disable_standard_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use own CSS ?'),
    '#description' => t('Do you want to use the standard css or include your own ?'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['disable_standard_css'])
      ? $view->options['bxslider_views_slideshow']['general']['disable_standard_css'] : FALSE,
  );
  $form['bxslider_views_slideshow']['general']['useCSS'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use CSS transitions'),
    '#description' => t('Use hardware-accelerated CSS transitions.  Fallback is jQuery.animate()'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['useCSS'])
      ? $view->options['bxslider_views_slideshow']['general']['useCSS'] : TRUE,
  );
  $form['bxslider_views_slideshow']['general']['align_image'] = array(
    '#type' => 'select',
    '#title' => t('Alignment Image'),
    '#description' => t('Do you want to align the image?'),
    '#options' => array(
      'left' => t('left'),
      'center' => t('center'),
      'right' => t('right'),
    ),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['align_image'])
      ? $view->options['bxslider_views_slideshow']['general']['align_image'] : 'left',
  );
  $form['bxslider_views_slideshow']['general']['align_caption'] = array(
    '#type' => 'select',
    '#title' => t('Alignment caption'),
    '#description' => t('Do you want to align the caption?'),
    '#options' => array(
      'left' => t('left'),
      'center' => t('center'),
      'right' => t('right'),
    ),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['align_caption'])
      ? $view->options['bxslider_views_slideshow']['general']['align_caption'] : 'left',
  );
  $form['bxslider_views_slideshow']['general']['swipeThreshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Swipe treshold default: 50'),
    '#description' => t('Amount of pixels a touch swipe needs to exceed in order to execute a slide transition. Note: only used if touchEnabled: tru.'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['swipeThreshold'])
      ? $view->options['bxslider_views_slideshow']['general']['swipeThreshold'] : 50,
  );
  $form['bxslider_views_slideshow']['general']['oneToOneTouch'] = array(
    '#type' => 'checkbox',
    '#title' => t('One to One touch'),
    '#description' => t('If true, non-fade slides follow the finger as it swipes'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['oneToOneTouch'])
      ? $view->options['bxslider_views_slideshow']['general']['oneToOneTouch'] : TRUE,
  );
  $form['bxslider_views_slideshow']['general']['preventDefaultSwipeX'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prevent swipe X'),
    '#description' => t('If true, touch screen will not move along the x-axis as the finger swipes'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['preventDefaultSwipeX'])
      ? $view->options['bxslider_views_slideshow']['general']['preventDefaultSwipeX'] : TRUE,
  );
  $form['bxslider_views_slideshow']['general']['preventDefaultSwipeY'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prevent swipe Y'),
    '#description' => t('If true, touch screen will not move along the y-axis as the finger swipes'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['preventDefaultSwipeY'])
      ? $view->options['bxslider_views_slideshow']['general']['preventDefaultSwipeY'] : FALSE,
  );
  // Colors.
  $form['bxslider_views_slideshow']['general']['color_caption'] = array(
    '#type' => 'textfield',
    '#title' => t('Color caption'),
    '#description' => t('Please provide me an correct CSS3.0 rgba color code: This consists of 4 numbers separated with an "," '),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['general']['color_caption']) ?
      $view->options['bxslider_views_slideshow']['general']['color_caption'] : "80, 80, 80, 0.75",
  );
  // Pager.
  $form['bxslider_views_slideshow']['pagerfieldset']['pager'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pager'),
    '#description' => t('If true, a pager will be added'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['pagerfieldset']['pager'])
      ? $view->options['bxslider_views_slideshow']['pagerfieldset']['pager'] : FALSE,
  );
  $form['bxslider_views_slideshow']['pagerfieldset']['pagerType'] = array(
    '#type' => 'select',
    '#title' => t('Pager type'),
    '#description' => t("If 'full', a pager link will be generated for each slide. If 'short', a x / y pager will be used (ex. 1 / 5)"),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['pagerfieldset']['pagerType']) ? $view->options['bxslider_views_slideshow']['pagerfieldset']['pagerType'] : 'full',
    '#options' => array(
      'full' => t('full'),
      'short' => t('short'),
      'custom' => t('custom'),
    ),
  );
  $form['bxslider_views_slideshow']['pagerfieldset']['pagerShortSeparator'] = array(
    '#type' => 'textfield',
    '#title' => t('Pager short separator'),
    '#description' => t('If pagerType: short, pager will use this value as the separating character: default /'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['pagerfieldset']['pagerShortSeparator']) ? $view->options['bxslider_views_slideshow']['pagerfieldset']['pagerShortSeparator'] : ' / ',
    '#states' => array(
		  'visible' => array(
		    ':input[name="style_options[bxslider_views_slideshow][pagerfieldset][pagerType]"]' => array('value' => 'short'),
		  ),
		),
  );
  $form['bxslider_views_slideshow']['pagerfieldset']['pagerCustom'] = array(
    '#type' => 'textfield',
    '#title' => t('Pager custom selector'),
    '#description' => t('If pagerType: custom, provide the ID or class (as a jQuery selector) of the pager element. Note that the pager element must contain an &lt;a&gt; tag for each slide. Each &lt;a&gt; tag must have a data-slide-index attribute that references the respecitve slide index (zero-based). <a href="http://bxslider.com/examples/thumbnail-pager-1" target="_blank">See the HTML in this example</a>.'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['pagerfieldset']['pagerCustom'])
      ? $view->options['bxslider_views_slideshow']['pagerfieldset']['pagerCustom'] : '',
    '#states' => array(
		  'visible' => array(
		    ':input[name="style_options[bxslider_views_slideshow][pagerfieldset][pagerType]"]' => array('value' => 'custom'),
		  ),
		  'required' => array(
		    ':input[name="style_options[bxslider_views_slideshow][pagerfieldset][pagerType]"]' => array('value' => 'custom'),
		  ),
    ),
  );
  // Controls.
  $form['bxslider_views_slideshow']['controlsfieldset']['controls'] = array(
    '#type' => 'checkbox',
    '#title' => t('Controls'),
    '#description' => t('If true, "Next" / "Prev" controls will be added'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['controlsfieldset']['controls']) ? $view->options['bxslider_views_slideshow']['controlsfieldset']['controls'] : TRUE,
  );
  $form['bxslider_views_slideshow']['controlsfieldset']['nextText'] = array(
    '#type' => 'textfield',
    '#title' => t('Next Text'),
    '#description' => t('Text to be used for the "Next" control'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['controlsfieldset']['nextText']) ? $view->options['bxslider_views_slideshow']['controlsfieldset']['nextText'] : '',
  );
  $form['bxslider_views_slideshow']['controlsfieldset']['prevText'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous Text'),
    '#description' => t('Text to be used for the "Prev" control'),
    '#default_value' => $view->options['bxslider_views_slideshow']['controlsfieldset']['prevText'],
  );
  $form['bxslider_views_slideshow']['controlsfieldset']['startText'] = array(
    '#type' => 'textfield',
    '#title' => t('Start Text'),
    '#description' => t('Text to be used for the "Start" control'),
    '#default_value' => $view->options['bxslider_views_slideshow']['controlsfieldset']['startText'],
  );
  $form['bxslider_views_slideshow']['controlsfieldset']['stopText'] = array(
    '#type' => 'textfield',
    '#title' => t('Stop Text'),
    '#description' => t('Text to be used for the "Stop" control'),
    '#default_value' => $view->options['bxslider_views_slideshow']['controlsfieldset']['stopText'],
  );
  $form['bxslider_views_slideshow']['controlsfieldset']['autoControls'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto controls'),
    '#description' => t('If true, "Start" / "Stop" controls will be added'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['controlsfieldset']['autoControls']) ? $view->options['bxslider_views_slideshow']['controlsfieldset']['autoControls'] : FALSE,
  );
  $form['bxslider_views_slideshow']['controlsfieldset']['autoControlsCombine'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto controls combine'),
    '#description' => t('When slideshow is playing only "Stop" control is displayed and vice-versa'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['controlsfieldset']['autoControlsCombine']) ? $view->options['bxslider_views_slideshow']['controlsfieldset']['autoControlsCombine'] : FALSE,
  );
  // Auto.
  $form['bxslider_views_slideshow']['autofieldset']['pause'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause'),
    '#description' => t('The amount of time (in ms) between each auto transition'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['autofieldset']['pause'])
      ? $view->options['bxslider_views_slideshow']['autofieldset']['pause'] : 4000,
  );
  $form['bxslider_views_slideshow']['autofieldset']['autoStart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto Start'),
    '#description' => t('If checked, then the slide show will begin rotating after the specified time below.'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['autofieldset']['autoStart'])
      ? $view->options['bxslider_views_slideshow']['autofieldset']['autoStart'] : TRUE,
  );
  $form['bxslider_views_slideshow']['autofieldset']['auto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto'),
    '#description' => t('Slides will automatically transition'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['autofieldset']['auto'])
      ? $view->options['bxslider_views_slideshow']['autofieldset']['auto'] : FALSE,
  );
  $form['bxslider_views_slideshow']['autofieldset']['autoHover'] = array(
    '#type' => 'textfield',
    '#title' => t('Auto hover'),
    '#description' => t('Auto show will pause when mouse hovers over slider'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['autofieldset']['autoHover'])
      ? $view->options['bxslider_views_slideshow']['autofieldset']['autoHover'] : 0,
  );
  $form['bxslider_views_slideshow']['autofieldset']['autoDelay'] = array(
    '#type' => 'textfield',
    '#title' => t('Auto delay'),
    '#description' => t('The direction of auto show slide transitions'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['autofieldset']['autoDelay'])
      ? $view->options['bxslider_views_slideshow']['autofieldset']['autoDelay'] : 0,
  );
  $form['bxslider_views_slideshow']['autofieldset']['autoDirection'] = array(
    '#type' => 'select',
    '#options' => array(
      'next' => t('next'),
      'prev' => t('prev'),
    ),
    '#title' => t('Auto direction'),
    '#description' => t('The direction of auto show slide transitions'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['autofieldset']['autoDirection']) ? $view->options['bxslider_views_slideshow']['autofieldset']['autoDirection'] : 'next',
  );
  // Carrousel.
  $form['bxslider_views_slideshow']['carousel']['minSlides'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum amount of slides'),
    '#description' => t('The minimum number of slides to be shown. Slides will be sized down if carousel becomes smaller than the original size..'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['carousel']['minSlides'])
      ? $view->options['bxslider_views_slideshow']['carousel']['minSlides'] : 1,
  );
  $form['bxslider_views_slideshow']['carousel']['maxSlides'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum slides'),
    '#description' => t('The maximum number of slides to be shown. Slides will be sized up if carousel becomes larger than the original size.'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['carousel']['maxSlides'])
      ? $view->options['bxslider_views_slideshow']['carousel']['maxSlides'] : 1,
  );
  $form['bxslider_views_slideshow']['carousel']['moveSlides'] = array(
    '#type' => 'textfield',
    '#title' => t('Move slides'),
    '#description' => t('The number of slides to move on transition. This value must be >= minSlides, and <= maxSlides. If zero (default), the number of fully-visible slides will be used.'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['carousel']['moveSlides'])
      ? $view->options['bxslider_views_slideshow']['carousel']['moveSlides']: 0,
  );
  $form['bxslider_views_slideshow']['carousel']['slideWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Slide width'),
    '#description' => t('The width of each slide. This setting is required for all horizontal carousels! default 0'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['carousel']['slideWidth'])
      ? $view->options['bxslider_views_slideshow']['carousel']['slideWidth'] : 0,
  );
  // Callbacks.bxSliderInitialize($(this)).
  $form['bxslider_views_slideshow']['callback']['information'] = array(
    '#markup' => t('If you enter javascript below it will be used in the appropriate functions. Be sure to enter javascript !'),
  );
  $form['bxslider_views_slideshow']['callback']['onSliderLoad'] = array(
    '#type' => 'textarea',
    '#title' => t('onSliderLoad'),
    '#description' => t('Executes immediately after the slider is fully loaded: You can utilize the parameter "currentIndex"'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['callback']['onSliderLoad'])
      ? $view->options['bxslider_views_slideshow']['callback']['onSliderLoad'] : NULL,
  );
  $form['bxslider_views_slideshow']['callback']['onSlideBefore'] = array(
    '#type' => 'textarea',
    '#title' => t('onSlideBefore'),
    '#description' => t('Executes immediately before each slide transition.: You can utilize the parameter "$slideElement, oldIndex, newIndex"'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['callback']['onSlideBefore'])
      ? $view->options['bxslider_views_slideshow']['callback']['onSlideBefore'] : NULL,
  );
  $form['bxslider_views_slideshow']['callback']['onSlideAfter'] = array(
    '#type' => 'textarea',
    '#title' => t('onSlideAfter'),
    '#description' => t('Executes immediately after each slide transition. Function verticalargument is the current slide element (when transition completes).: You can utilize the parameter "$slideElement, oldIndex, newIndex"'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['callback']['onSlideAfter'])
      ? $view->options['bxslider_views_slideshow']['callback']['onSlideAfter'] : NULL,
  );
  $form['bxslider_views_slideshow']['callback']['onSlideNext'] = array(
    '#type' => 'textarea',
    '#title' => t('onSlideNext'),
    '#description' => t('Executes immediately before each "Next" slide transition. Function argument is the target (next) slide element.: You can utilize the parameter "$slideElement, oldIndex, newIndex"'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['callback']['onSlideNext'])
      ? $view->options['bxslider_views_slideshow']['callback']['onSlideNext'] : NULL,
  );
  $form['bxslider_views_slideshow']['callback']['onSlidePrev'] = array(
    '#type' => 'textarea',
    '#title' => t('onSlidePrev'),
    '#description' => t('Executes immediately before each "Prev" slide transition. Function argument is the target (prev) slide element.: You can utilize the parameter "$slideElement, oldIndex, newIndex"'),
    '#default_value' => isset($view->options['bxslider_views_slideshow']['callback']['onSlidePrev'])
      ? $view->options['bxslider_views_slideshow']['callback']['onSlidePrev'] : NULL,
  );
}


/**
 *  Implements hook_views_slideshow_option_definition().
 */
function bxslider_views_slideshow_views_slideshow_option_definition() {
  // easing
  // pagerCustom

  // Set our default options.
  $options['bxslider_views_slideshow'] = array(
    'contains' => array(
      'general' => array(
        'mode' => array('default' => 'horizontal'),
        'speed' => array('default' => 500),
        'slideMargin' => array('default' => 0),
        'startSlide' => array('default' => 0),
        'randomStart' => array('default' => FALSE),
        'infiniteLoop' => array('default' => TRUE),
        'hideControlOnEnd' => array('default' => FALSE),
        'captions' => array('default' => False),
        'useCSS' => array('default' => TRUE),
        'disable_standard_css' => array('default' => FALSE),
        'align_image' => array('default' => 'left'),
        'align_caption' => array('default' => 'left'),
        'ticker' => array('default' => FALSE),
        'tickerHover' => array('default' => FALSE),
        'adaptiveHeight' => array('default' => FALSE),
        'adaptiveHeightSpeed' => array('default' => 500),
        'video' => array('default' => FALSE),
        'touchEnabled' => array('default' => TRUE),
        'preloadImages' => array('default' => 'visible'),
        'swipeThreshold' => array('default' => 50),
        'oneToOneTouch' => array('default' => TRUE),
        'preventDefaultSwipeX' => array('default' => TRUE),
        'preventDefaultSwipeY' => array('default' => FALSE),
        'color_caption' => array('default' => "80, 80, 80, 0.75"),
      ),
      'controlsfieldset' => array(
        'startText' => array('default' => ''),
        'stopText' => array('default' => ''),
        'nextText' => array('default' => ''),
        'prevText' => array('default' => ''),
        'autoControls' => array('default' => FALSE),
        'autoControlsCombine' => array('default' => FALSE),
        'controls' => array('default' => TRUE),
      ),
      'pagerfieldset' => array(
        'pager' => array('default' => FALSE),
        'pagerType' => array('default' => 'short'),
        'pagerShortSeparator' => array('default' => ' / '),
        'pagerCustom' => array('default' => ''),
      ),
      'autofieldset' => array(
        'autoStart' => array('default' => TRUE),
        'pause' => array('default' => 4000),
        'auto' => array('default' => FALSE),
        'autoHover' => array('default' => 0),
        'autoDelay' => array('default' => 0),
        'autoDirection' => array('default' => 'next'),
      ),
      'carousel' => array(
        'minSlides' => array('default' => 1),
        'maxSlides' => array('default' => 1),
        'slideWidth' => array('default' => 0),
        'moveSlides' => array('default' => 0),
      ),
      'callback' => array(
        'onSliderLoad' => array('default' => NULL),
        'onSlideBefore' => array('default' => NULL),
        'onSlideAfter' => array('default' => NULL),
        'onSlideNext' => array('default' => NULL),
        'onSlidePrev' => array('default' => NULL),
      ),
    ),
  );
  return $options;
}
