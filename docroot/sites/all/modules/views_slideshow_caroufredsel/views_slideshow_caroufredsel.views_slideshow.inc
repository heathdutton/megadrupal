<?php

/**
 * @file
 * The default options available with Views Slideshow: cycle.
 */

/**
 * Implements hook_views_slideshow_slideshow_info().
 */
function views_slideshow_caroufredsel_views_slideshow_slideshow_info() {
  $options = array(
    'views_slideshow_caroufredsel' => array(
      'name' => t('CarouFredSel'),
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
        // @todo add support for these (on hover pause etc)
        //'pause',
        //'play',
      ),
    ),
  );
  return $options;
}

/**
 * Implements hook_views_slideshow_option_definition().
 */
function views_slideshow_caroufredsel_views_slideshow_option_definition() {
  $options['views_slideshow_caroufredsel'] = array(
    'contains' => array(
      'circular' => array('default' => TRUE),
      'infinite' => array('default' => TRUE),
      'responsive' => array('default' => TRUE),
      'direction' => array('default' => 'left'),
      'width' => array('default' => 'variable'),
      'height' => array('default' => 'variable'),
      'align' => array('default' => 'center'),
      'padding' => array('default' => NULL),
      'synchronise' => array('default' => NULL),
      'cookie' => array('default' => FALSE),
      'onCreate' => array('default' => NULL),
      'items' => array(
        'visible' => array('default' => NULL),
        'minimum' => array('default' => NULL),
        'start' => array('default' => 0),
        'width' => array('default' => 'variable'),
        'height' => array('default' => 'variable'),
        'filter' => array('default' => NULL)
      ),
      'scroll' => array(
        'items' => array('default' => NULL),
        'fx' => array('default' => 'scroll'),
        'easing' => array('default' => 'swing'),
        'duration' => array('default' => 500),
        'pauseOnHover' => array('default' => FALSE),
        'queue' => array('default' => FALSE),
        'event' => array('default' => "click"),
        'conditions' => array('default' => NULL),
        // @todo Provide support for these without destroying views_slideshow
        //  integration. See js file
        // 'onBefore' => array('default' => NULL),
        // 'onAfter' => array('default' => NULL),
        'onEnd' => array('default' => NULL),
      ),
    )
  );
  return $options;
}

/**
 * Implements hook_views_slideshow_slideshow_type_form().
 */
function views_slideshow_caroufredsel_views_slideshow_slideshow_type_form(&$form, &$form_state, &$view) {
  $conf_link = l(
      t('See plugin documentation'),
      'http://caroufredsel.dev7studios.com/configuration.php', array(
      'attributes' => array(
        'target' => '_blank',
      ),
    ));
  if (!$caroufredsel_path = _views_slideshow_caroufredsel_library_path()) {
    $form['views_slideshow_caroufredsel']['no_caroufredsel_js'] = array(
      '#markup' => '<div style="color: red">' . t('You need to install the jQuery carouFredSel plugin. Create a directory in sites/all/libraries called caroufredsel, and place a copy of the plugin with the name jquery.carouFredSel.min.js. You can find the plugin at !url.', array('!url' => l('http://caroufredsel.dev7studios.com/', 'http://caroufredsel.dev7studios.com/', array('attributes' => array('target' => '_blank'))))) . '</div>',
    );
  }
  $form['views_slideshow_caroufredsel']['description'] = array(
    '#markup' => t('carouFredSel is a complex jQuery plugin. !conf_link in order to understand the options.', array('!conf_link' => $conf_link)),
    '#prefix' => '<div class="messages status">',
    '#suffix' => '</div>',
  );
   // Items section
  $form['views_slideshow_caroufredsel']['items'] = array(
    '#type' => 'fieldset',
    '#title' => t('Items'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('A map of the configuration for the items. !conf_link.', array('!conf_link' => $conf_link)),
  );
  // @todo provide proper support for items.visible see plugin documentation
  $form['views_slideshow_caroufredsel']['items']['visible'] = array(
    '#type' => 'textfield',
    '#title' => t('Visible'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['items']['visible'],
    '#description' => t('The number of visible items. If null, the number will be calculated (and set to "variable" if necessary).'),
  );
  $form['views_slideshow_caroufredsel']['items']['minimum'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Minimum'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['items']['minimum'],
    '#description' => t('The minimum number of items needed to create a carousel. If null, the number for items.visible is inherited and increased by 1.'),
  );
  // @todo provide proper support for items.start Array value
  $form['views_slideshow_caroufredsel']['items']['start'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Start'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['items']['start'],
    '#description' => t('The nth item to start the carousel. Hint: This can also be a negative number.'),
  );
  $form['views_slideshow_caroufredsel']['items']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['items']['width'],
    '#description' => t('The width of the items. If null, the width will be measured (and set to "variable" if necessary).'),
  );
  $form['views_slideshow_caroufredsel']['items']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['items']['height'],
    '#description' => t('The height of the items. If null, the height will be measured (and set to "variable" if necessary).'),
  );
    $form['views_slideshow_caroufredsel']['items']['filter'] = array(
    '#type' => 'textfield',
    '#title' => t('Filter'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['items']['filter'],
    '#description' => t('The selector elements should match to be considered an item. If null, all elements inside the carousel will be considered to be an item. If the carousel contains :hidden-elements, it is set to ":visible". Note: this option is not (yet) compatible with non-circular carousels.'),
  );
  // End Items section

  // Scroll section
  $form['views_slideshow_caroufredsel']['scroll'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scroll'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('A map of the configuration used for general scrolling. !conf_link.', array('!conf_link' => $conf_link)),
  );
  $form['views_slideshow_caroufredsel']['scroll']['items'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 4,
    '#default_value' => $view->options['views_slideshow_caroufredsel']['scroll']['items'],
    '#description' => t('The number of items to scroll. If null, the number of visible items is used.'),
  );
  $form['views_slideshow_caroufredsel']['scroll']['fx'] = array(
    '#type' => 'select',
    '#title' => t('Fx'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['scroll']['fx'],
    '#options' => array(
      "none" => t('None'),
      "scroll",
      "directscroll",
      "fade",
      "crossfade",
      "cover",
      "cover-fade",
      "uncover",
      "uncover-fade"
    ),
    '#description' => t('Indicates which effect to use for the transition.'),
  );
  $form['views_slideshow_caroufredsel']['scroll']['easing'] = array(
    '#type' => 'select',
    '#title' => t('Easing'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['scroll']['easing'],
    '#options' => array(
      "linear",
      "swing",
      "quadratic",
      "cubic",
      "elastic",
    ),
    '#description' => t('Indicates which effect to use for the transition.'),
  );
  $form['views_slideshow_caroufredsel']['scroll']['duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Duration'),
    '#size' => 4,
    '#default_value' => $view->options['views_slideshow_caroufredsel']['scroll']['duration'],
    '#description' => t('Determines the duration of the transition in milliseconds. If less than 10, the number is interpreted as a speed (pixels/millisecond). This is probably desirable when scrolling items with variable sizes.'),
  );
  $form['views_slideshow_caroufredsel']['scroll']['pauseOnHover'] = array(
    '#type' => 'select',
    '#title' => t('Pause on hover'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['scroll']['pauseOnHover'],
    '#options' => array(
      "false" => t('No'),
      "true" => t('Yes'),
      "resume" => t('Resume'),
      "inmediate" => t('Inmediate'),
      "inmediate-resume" => t('Inmediate resume')
    ),
    '#description' => t('Determines whether the timeout between transitions should be paused "onMouseOver" (only applies when the carousel scrolls automatically).'),
  );
  $form['views_slideshow_caroufredsel']['scroll']['queue'] = array(
    '#type' => 'select',
    '#title' => t('Queue'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['scroll']['queue'],
    '#options' => array(
      "false" => t('No'),
      "true" => t('Yes'),
      "first" => t('First'),
      "last" => t('Last'),
    ),
    '#description' => t('Determines whether the scrolling should be queued if the carousel is currently being animated.'),
  );
  $form['views_slideshow_caroufredsel']['scroll']['event'] = array(
    '#type' => 'textfield',
    '#title' => t('Event'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['scroll']['event'],
    '#description' => t('The event to trigger the prev-, next- and pagination-buttons/bullets.'),
  );
  // @todo support onBefore and onAfter without breaking views_slideshow integration
  $form['views_slideshow_caroufredsel']['scroll']['onEnd'] = array(
    '#type' => 'textfield',
    '#title' => t('onEnd'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['scroll']['onEnd'],
    '#description' => t('Function to call when a non-circular carousel reaches its start or end.'),
  );
  // End Scroll section

  $form['views_slideshow_caroufredsel']['circular'] = array(
    '#type' => 'checkbox',
    '#title' => t('Circular'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['circular'],
    '#description' => t('Determines whether the carousel should be circular.'),
  );
  $form['views_slideshow_caroufredsel']['infinite'] = array(
    '#type' => 'checkbox',
    '#title' => t('Infinite'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['infinite'],
    '#description' => t('Determines whether the carousel should be infinite. Note: It is possible to create a non-circular, infinite carousel, but it is not possible to create a circular, non-infinite carousel.'),
  );
  $form['views_slideshow_caroufredsel']['responsive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Responsive'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['responsive'],
    '#description' => t('Determines whether the carousel should be responsive. If true, the items will be resized to fill the carousel.'),
  );
  $form['views_slideshow_caroufredsel']['direction'] = array(
    '#type' => 'select',
    '#title' => t('Direction'),
    '#options' => array("right" => t('Right'), "left" => t('Left'), "up" => t('Up'), "down" => t('Down')),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['direction'],
    '#description' => t('The direction to scroll the carousel, determines whether the carousel scrolls horizontal or vertical and -when the carousel scrolls automatically- in what direction.'),
  );
  $form['views_slideshow_caroufredsel']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['width'],
    '#description' => t('The width of the carousel. Enter "variable" to automatically resize the carousel when scrolling items with variable widths. Enter "auto" to measure the widest item. Enter a percentage to automatically resize (and re-configurate) the carousel onWindowResize. Note: Only applies on horizontal carousels.'),
  );
  $form['views_slideshow_caroufredsel']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['height'],
    '#description' => t('The height of the carousel. Enter "variable" to automatically resize the carousel when scrolling items with variable heights. Enter "auto" to measure the highest item. Enter a percentage to automatically resize (and re-configurate) the carousel onWindowResize. Note: Only applies on vertical carousels.'),
  );
  $form['views_slideshow_caroufredsel']['align'] = array(
    '#type' => 'select',
    '#title' => t('Align'),
    '#options' => array("center" => t('Center'), "left" => t('Left'), "right" => t('Right'), "false" => t('Don\'t align')),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['align'],
    '#description' => t('Whether and how to align the items inside a fixed width/height.'),
  );
  $form['views_slideshow_caroufredsel']['padding'] = array(
    '#type' => 'textfield',
    '#title' => t('Padding'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['padding'],
    '#description' => t('Padding around the carousel (top, right, bottom and left). Padding in an Array. For example: [10, 20, 30, 40] (top, right, bottom, left) or [0, 50] (top/bottom, left/right).'),
  );
  $form['views_slideshow_caroufredsel']['synchronise'] = array(
    '#type' => 'textfield',
    '#title' => t('Synchronise'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['synchronise'],
    '#description' => t('Selector for the carousel to synchronise.'),
  );
  $form['views_slideshow_caroufredsel']['cookie'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cookie'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['cookie'],
    '#description' => t('Determines whether the carousel should start at its last viewed position.'),
  );
  $form['views_slideshow_caroufredsel']['onCreate'] = array(
    '#type' => 'textfield',
    '#title' => t('onCreate'),
    '#default_value' => $view->options['views_slideshow_caroufredsel']['onCreate'],
    '#description' => t('Function that will be called after the carousel has been created.'),
  );
  // @todo Plenty of more configuration options to support
}

/**
 * Form validation callback for the slideshow settings.
 */
function views_slideshow_caroufredsel_views_slideshow_options_form_validate(&$form, &$form_state, &$view) {
  // @todo
}

/**
 * Form submission callback for the slideshow settings.
 */
function views_slideshow_caroufredsel_views_slideshow_options_form_submit($form, &$form_state) {
  // For now we don't need this...
}


/**
 * Define JS methods to be run when a certain slideshow event is fired.
 *
 * Available events:
 *  - viewsSlideshowPause
 *  - viewsSlideshowPlay
 *  - viewsSlideshowNextSlide
 *  - viewsSlideshowPreviousSlide
 *  - viewsSlideshowGoToSlide
 *  - viewsSlideshowTransitionBegin
 *  - viewsSlideshowTransitionEnd
 * @return
 *   array of methods.
 */
function views_slideshow_caroufredsel_views_slideshow_js_method_register() {
  return array(
    'viewsSlideshowCaroufredsel',
  );
}
