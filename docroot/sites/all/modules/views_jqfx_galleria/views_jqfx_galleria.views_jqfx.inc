<?php

/**
 * @file
 * The default options available with Views jQFX: galleria.
 */

/**
 * Implements hook_views_jqfx_jqfx_types().
 */
function views_jqfx_galleria_views_jqfx_jqfx_types() {
  $options = array(
    'views_jqfx_galleria' => t('Galleria'),
  );
  return $options;
}

function views_jqfx_galleria_views_jqfx_option_definition() {
  $options['views_jqfx_galleria'] = array(
    'contains' => array(
      'theme'                     => array('default' => 'classic'),
      'autoplay'                  => array('default' => FALSE),
      'carousel'                  => array('default' => TRUE),
      'carousel_follow'           => array('default' => TRUE),
      'carousel_speed'            => array('default' => 200),
      'carousel_steps'            => array('default' => 'auto'),
      'clicknext'                 => array('default' => FALSE),
      'custom_theme'              => array('default' => NULL),
      'height'                    => array('default' => 400),
      'width'                     => array('default' => 600),
      'data_config'               => array('default' => NULL),
      'data_selector'             => array('default' => NULL),
      'data_source'               => array('default' => NULL),
      'debug'                     => array('default' => FALSE),
      'easing'                    => array('default' => 'galleria'),
      'extend'                    => array('default' => NULL),
      'galleria_loadtheme'        => array('default' => 'galleria'),
      'history'                   => array('default' => FALSE),
      'idle_speed'                => array('default' => 200),
      'idle_time'                 => array('default' => 3000),
      'image_crop'                => array('default' => FALSE),
      'image_margin'              => array('default' => 0),
      'image_pan'                 => array('default' => FALSE),
      'image_pan_smoothness'      => array('default' => 12),
      'image_position'            => array('default' => 'center'),
      'keep_source'               => array('default' => FALSE),
      'lightbox'                  => array('default' => FALSE),
      'lightbox_fade_speed'       => array('default' => 200),
      'lightbox_options'          => array('default' => FALSE),
      'lightbox_transition_speed' => array('default' => 200),
      'max_scale_ratio'           => array('default' => ''),
      'min_scale_ratio'           => array('default' => ''),
      'overlay_background'        => array('default' => '#0b0b0b'),
      'overlay_opacity'           => array('default' => 0.85),
      'pause_on_interaction'      => array('default' => TRUE),
      'popup_links'               => array('default' => FALSE),
      'preload'                   => array('default' => 2),
      'queue'                     => array('default' => TRUE),
      'show'                      => array('default' => 0),
      'show_counter'              => array('default' => TRUE),
      'show_imagenav'             => array('default' => TRUE),
      'show_info'                 => array('default' => TRUE),
      'theme'                     => array('default' => 'classic'),
      'thumb_crop'                => array('default' => TRUE),
      'thumb_fit'                 => array('default' => TRUE),
      'thumb_margin'              => array('default' => 0),
      'thumbnails'                => array('default' => TRUE),
      'transition'                => array('default' => 'fade'),
      'transition_initial'        => array('default' => ''),
      'transition_speed'          => array('default' => 400),
      'advanced_functions'        => array('default' => FALSE),
      'minify_js'                 => array('default' => FALSE),
    ),
  );
  return $options;
}

/**
 * Implements hook_views_jqfx_views_jqfx_type_form().
 */
function views_jqfx_galleria_views_jqfx_jqfx_type_form(&$form, &$form_state, &$view) {
  ctools_include('dependent');
  if (!file_exists('sites/all/libraries/galleria/galleria.js')) {
    $form['views_jqfx_galleria']['no_galleria_js'] = array(
      '#markup' => '<div style="color: red">' . t('You need to download the Galleria plugin and copy it to /sites/all/libraries/galleria. The correct directory configuration will have the plugin available at /sites/all/libraries/galleria/galleria.js. You can find the plugin at !url.', array('!url' => l(t('http://galleria.aino.se', array(), array('langcode' => 'en')), 'http://galleria.aino.se', array('attributes' => array('target' => '_blank')))), array('langcode' => 'en')) . '</div>',
    );
  }
  $themes = array(
    FALSE => t('Custom (Enter path below)'),
    'classic' => t('Classic'),
    'dots' => t('Dots'),
    'fullscreen' => t('Fullscreen'),
    'miniml' => t('Miniml'),
    'twelve' => t('Twelve'),
  );
  $form['views_jqfx_galleria']['theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('Galleria theme to load on display. Lately only the classic theme been packaged with the Galleria plugin. If you choose the %custom option, you must specify the path to your custom JavaScript theme below.', array('%custom' => t('Custom theme'))),
    '#default_value' => $view->options['views_jqfx_galleria']['theme'],
    '#options' => $themes,
  );
  $form['views_jqfx_galleria']['custom_theme'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom theme path'),
    '#description' => t("Enter the path to the js file of your custom theme. For example, if the classic theme were custom the path would be 'sites/all/libraries/galleria/themes/dots/galleria.dots.js'."),
    '#default_value' => $view->options['views_jqfx_galleria']['custom_theme'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-theme' => array(FALSE)),
  );
  $form['views_jqfx_galleria']['galleria_loadtheme'] = array(
    '#type' => 'select',
    '#title' => t('Galleria loadTheme'),
    '#description' => t("Choose how you want your theme loaded. This is experimental. You must use only the Galleria loadTheme function if you wish to be able change themes on the fly. Manual load uses the 'drupal_add_js()' and 'drupal_add_css()' functions to load the theme in the head. Manual load by itself does not seem to work with aggregation enabled."),
    '#default_value' => $view->options['views_jqfx_galleria']['galleria_loadtheme'],
    '#options' => array(
      'manual' => t('Load manually'),
      'galleria' => t('Galleria loadTheme'),
      'both' => t('Use both'),
    ),
  );
  $autoplay = array(
    0 => t('False'),
    '200' => t('200'),
    '400' => t('400'),
    '600' => t('600'),
    '800' => t('800'),
    '1000' => t('1000'),
    '1250' => t('1250'),
    '1500' => t('1500'),
    '1750' => t('1750'),
    '2000' => t('2000'),
    '2500' => t('2500'),
    '3000' => t('3000'),
    '4000' => t('4000'),
    '5000' => t('5000'),
    '7500' => t('7500'),
    '10000' => t('10000'),
    '15000' => t('15000'),
    '20000' => t('20000'),
    '30000' => t('30000'),
  );
  $form['views_jqfx_galleria']['autoplay'] = array(
    '#type' => 'select',
    '#title' => t('Autoplay'),
    '#description' => t('If you set this to any number, f.ex 4000, it will start playing with that interval (in milliseconds).'),
    '#default_value' => $view->options['views_jqfx_galleria']['autoplay'],
    '#options' => $autoplay,
  );
  $form['views_jqfx_galleria']['carousel'] = array(
    '#type' => 'select',
    '#title' => t('Carousel'),
    '#description' => t('This will activate the carousel when needed. Otherwise it will not appear at all.'),
    '#default_value' => $view->options['views_jqfx_galleria']['carousel'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['carousel_follow'] = array(
    '#type' => 'select',
    '#title' => t('Carousel follow'),
    '#description' => t('Has the carousel follow the active image.'),
    '#default_value' => $view->options['views_jqfx_galleria']['carousel_follow'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-carousel' => array(TRUE)),
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['carousel_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Carousel speed'),
    '#description' => t('The slide speed of the carousel in milliseconds.'),
    '#default_value' => $view->options['views_jqfx_galleria']['carousel_speed'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-carousel' => array(TRUE)),
  );
  $form['views_jqfx_galleria']['carousel_steps'] = array(
    '#type' => 'textfield',
    '#title' => t('Carousel steps'),
    '#description' => t('The number of "steps" the carousel will slide when navigating between available thumbnails. Specifying %auto will move the carousel as many steps as there are visible thumbnails.', array('%auto' => 'auto')),
    '#default_value' => $view->options['views_jqfx_galleria']['carousel_steps'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-carousel' => array(TRUE)),
  );
  $form['views_jqfx_galleria']['clicknext'] = array(
    '#type' => 'select',
    '#title' => t('Click next'),
    '#description' => t('Click the image to navigate to next item.'),
    '#default_value' => $view->options['views_jqfx_galleria']['clicknext'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('This will set a height to the gallery. If you set this to %auto and no CSS height is found, Galleria will try to automatically add a 16/9 ratio as a fallback. With some themes if this is left to auto the Galleria will not load.', array('%auto' => 'auto')),
    '#default_value' => $view->options['views_jqfx_galleria']['height'],
  );
  $form['views_jqfx_galleria']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('This will set a width to the gallery. If you set this to %auto and no CSS height is found, Galleria will try to automatically add a 16/9 ratio as a fallback.', array('%auto' => 'auto')),
    '#default_value' => $view->options['views_jqfx_galleria']['width'],
  );
  $form['views_jqfx_galleria']['history'] = array(
    '#type' => 'select',
    '#title' => t('History'),
    '#description' => t('This option loads the history plugin. When enabled a url will be created for each image within the gallery, and using the browser back button will navigate to the previous image rather than the previous page. The galleria.history.js file must be located at /sites/all/libraries/galleria/plugins/galleria.history.js. Note that this file has not been part of the recent Galleria 1.2 plugin download package from Aino and is not officially supported, but it does still work with 1.2.'),
    '#default_value' => $view->options['views_jqfx_galleria']['history'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['idle_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Idle speed'),
    '#description' => t("Controls the animation speed of the idle elements. An example is when the thumbnails and captions hide in the fullscreen theme after a period of inactivity."),
    '#default_value' => $view->options['views_jqfx_galleria']['idle_speed'],
  );
  $form['views_jqfx_galleria']['idle_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Idle time'),
    '#description' => t("You can control the delay before Galleria falls into idle mode using this option."),
    '#default_value' => $view->options['views_jqfx_galleria']['idle_time'],
  );
  $image_crop = array(
    FALSE => t('False'),
    TRUE => t('True'),
    'width' => t('Width'),
    'height' => t('Height'),
  );
  $form['views_jqfx_galleria']['image_crop'] = array(
    '#type' => 'select',
    '#title' => t('Image scale/crop'),
    '#options' => $image_crop,
    '#description' => t('If true, all images will be scaled to fill the entire stage, centered and cropped. You can also scale images by either height or width. Otherwise, they will scale down so the entire image fits.'),
    '#default_value' => $view->options['views_jqfx_galleria']['image_crop'],
  );
  $form['views_jqfx_galleria']['image_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Image margin'),
    '#description' => t('Sets a margin between the image and stage. Specify the number of pixels.'),
    '#default_value' => $view->options['views_jqfx_galleria']['image_margin'],
  );
  $form['views_jqfx_galleria']['image_pan'] = array(
    '#type' => 'select',
    '#title' => t('Image pan'),
    '#description' => t('Galleria comes with a built-in panning effect. The effect is sometimes useful if you have cropped images and want to let the users pan across the stage to see the entire image. Enable image pan to apply a mouse-controlled movement of the image to reveal the cropped parts. Note that when images are scaled by height and the slide transition is used, the image may not stay centered in some browsers when this option is enabled.'),
    '#default_value' => $view->options['views_jqfx_galleria']['image_pan'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['image_pan_smoothness'] = array(
    '#type' => 'textfield',
    '#title' => t('Image pan smoothness'),
    '#description' => t('This value sets how "smooth" the image pan movement should be when setting image_pan is set to %true. The higher value, the smoother effect but also CPU consuming.', array('%true' => 'true')),
    '#default_value' => $view->options['views_jqfx_galleria']['image_pan_smoothness'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-image-pan' => array(TRUE)),
  );
  $form['views_jqfx_galleria']['image_position'] = array(
    '#type' => 'textfield',
    '#title' => t('Image position'),
    '#description' => t("Positions the main image. Works like the CSS background-position property; i.e., 'top right' or '20% 100%'. You can use keywords, percents or pixels. The first value is the horizontal position and the second is the vertical. Read more at !read_more.", array('!read_more' => l(t('http://www.w3.org/TR/REC-CSS1/#background-position', array(), array('langcode' => 'en')), 'http://www.w3.org/TR/REC-CSS1/#background-position', array('attributes' => array('target' => '_blank')))), array('langcode' => 'en')),
    '#default_value' => $view->options['views_jqfx_galleria']['image_position'],
  );
  $form['views_jqfx_galleria']['max_scale_ratio'] = array(
    '#type' => 'textfield',
    '#title' => t('Max scale ratio'),
    '#description' => t("Sets the maximum scale ratio for images. If you don't want Galleria to upscale any images, set this to 1. Leaving it blank will allow any scaling of the images."),
    '#default_value' => $view->options['views_jqfx_galleria']['max_scale_ratio'],
  );
  $form['views_jqfx_galleria']['min_scale_ratio'] = array(
    '#type' => 'textfield',
    '#title' => t('Min scale ratio'),
    '#description' => t("Sets the minimum scale ratio for images. If you don't want Galleria to downscale any images, set this to 1. Leaving it blank will allow any scaling of the images."),
    '#default_value' => $view->options['views_jqfx_galleria']['min_scale_ratio'],
  );
  $form['views_jqfx_galleria']['lightbox_options'] = array(
    '#type' => 'checkbox',
    '#title' => t('Lightbox options'),
    '#description' => t('Check to display the lightbox options.'),
    '#default_value' => $view->options['views_jqfx_galleria']['lightbox_options'],
  );
  $form['views_jqfx_galleria']['lightbox'] = array(
    '#type' => 'select',
    '#title' => t('Lightbox'),
    '#description' => t('This option acts as a helper for attaching a lightbox when the user clicks on an image. If you have a link defined for the image, the link will take precedence.'),
    '#default_value' => $view->options['views_jqfx_galleria']['lightbox'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-lightbox-options' => array(TRUE)),
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['lightbox_fade_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Lightbox fade speed'),
    '#description' => t("When calling ``.showLightbox()`` the lightbox will animate and fade the images and captions. This value controls how fast they should fade in milliseconds."),
    '#default_value' => $view->options['views_jqfx_galleria']['lightbox_fade_speed'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-lightbox-options' => array(TRUE)),
  );
  $form['views_jqfx_galleria']['lightbox_transition_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Lightbox transition speed'),
    '#description' => t("When calling ``.showLightbox()`` the lightbox will animate the white square before displaying the image. This value controls how fast it should animate in milliseconds."),
    '#default_value' => $view->options['views_jqfx_galleria']['lightbox_transition_speed'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-lightbox-options' => array(TRUE)),
  );
  $form['views_jqfx_galleria']['overlay_background'] = array(
    '#type' => 'textfield',
    '#title' => t('Overlay background'),
    '#description' => t("This defines the overlay background color when calling ``showLightbox()``."),
    '#default_value' => $view->options['views_jqfx_galleria']['overlay_background'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-lightbox-options' => array(TRUE)),
  );
  $form['views_jqfx_galleria']['overlay_opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Overlay opacity'),
    '#description' => t("This sets how much opacity the overlay should have when calling ``showLightbox()``."),
    '#default_value' => $view->options['views_jqfx_galleria']['overlay_opacity'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-lightbox-options' => array(TRUE)),
  );
  $form['views_jqfx_galleria']['pause_on_interaction'] = array(
    '#type' => 'select',
    '#title' => t('Pause on interaction'),
    '#description' => t('During playback, Galleria will stop the playback if the user presses thumbnails or any other navigational links. If you dont want this behaviour, set this option to %false.', array('%false' => 'false')),
    '#default_value' => $view->options['views_jqfx_galleria']['pause_on_interaction'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['popup_links'] = array(
    '#type' => 'select',
    '#title' => t('Popup links'),
    '#description' => t('This will cause any image links to open in a new window.'),
    '#default_value' => $view->options['views_jqfx_galleria']['popup_links'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['preload'] = array(
    '#type' => 'textfield',
    '#title' => t('Preload'),
    '#description' => t("Defines how many images Galleria should preload in advance. Please note that this only applies when you are using separate thumbnail files. Galleria always cache all preloaded images. <ul><li>%2 preloads the next 2 images in line.</li><li>%all forces Galleria to start preloading all images. This may slow down client.</li><li>%0 will not preload any images</li></ul>", array('%2' => '2', '%all' => 'all', '%0' => '0')),
    '#default_value' => $view->options['views_jqfx_galleria']['preload'],
  );
  $form['views_jqfx_galleria']['queue'] = array(
    '#type' => 'select',
    '#title' => t('Queue'),
    '#description' => t("Galleria queues all activation clicks (next/prev & thumbnails). You can see this effect when, for example, clicking %next many times. If you don't want Galleria to queue, then uncheck the box.", array('%next' => t('next'))),
    '#default_value' => $view->options['views_jqfx_galleria']['queue'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['show'] = array(
    '#type' => 'textfield',
    '#title' => t('Show'),
    '#description' => t("This defines what image index to show at first."),
    '#default_value' => $view->options['views_jqfx_galleria']['show'],
  );
  $form['views_jqfx_galleria']['show_counter'] = array(
    '#type' => 'select',
    '#title' => t('Counter'),
    '#description' => t('Disable if you do not wish to display the counter.'),
    '#default_value' => $view->options['views_jqfx_galleria']['show_counter'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['show_imagenav'] = array(
    '#type' => 'select',
    '#title' => t('Image navigation'),
    '#description' => t('Disable if you do not wish to display the image navigation (next/prev arrows).'),
    '#default_value' => $view->options['views_jqfx_galleria']['show_imagenav'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['show_info'] = array(
    '#type' => 'select',
    '#title' => t('Image captions'),
    '#description' => t('Disable if you do not wish to display the captions.'),
    '#default_value' => $view->options['views_jqfx_galleria']['show_info'],
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $options = array(
    FALSE => t('False'),
    TRUE => t('True'),
    'empty' => t('Empty'),
    'numbers' => t('Numbers'),
  );
  $form['views_jqfx_galleria']['thumbnails'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnails'),
    '#description' => t("Sets the creation of thumbnails. If false, Galleria will not create thumbnails and no carousel. If you set this to 'empty', Galleria will create empty spans with the className img instead of thumbnails. If you set this to 'numbers', Galleria will create empty spans with numbers instead of thumbnails."),
    '#default_value' => $view->options['views_jqfx_galleria']['thumbnails'],
    '#options' => $options,
  );
  $thumb_crop = array(
    FALSE => t('False'),
    TRUE => t('True'),
    'width' => t('Width'),
    'height' => t('Height'),
  );
  $form['views_jqfx_galleria']['thumb_crop'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnail scale/crop'),
    '#options' => $thumb_crop,
    '#description' => t("Same as %image_crop, but for thumbnails.", array('%image_crop' => t('Image crop'))),
    '#default_value' => $view->options['views_jqfx_galleria']['thumb_crop'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-thumbnails' => array(1)),
  );
  $form['views_jqfx_galleria']['thumb_fit'] = array(
    '#type' => 'select',
    '#title' => t('Thumb fit'),
    '#description' => t("If this is enabled, all thumbnail containers will be shrinked to fit the actual thumbnail size. This is useful if you have thumbnails of various sizes and will then float nicely side-by-side.This is only relevant if thumb_crop is set to anything else but 'true'.If you want all thumbnails to fit inside a container with predefined width & height, set this to 'false'."),
    '#default_value' => $view->options['views_jqfx_galleria']['thumb_fit'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-thumbnails' => array(1)),
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['thumb_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumb margin'),
    '#description' => t("Same as %image_margin, but for thumbnails.", array('%image_margin' => t('Image margin'))),
    '#default_value' => $view->options['views_jqfx_galleria']['thumb_margin'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-thumbnails' => array(1)),
  );
  $transitions = array(
    '' => t('Not set'),
    'fade' => t('Fade'),
    'flash' => t('Flash'),
    'pulse' => t('Pulse'),
    'slide' => t('Slide'),
    'fadeslide' => t('Fade/Slide'),
  );
  $form['views_jqfx_galleria']['transition'] = array(
    '#type' => 'select',
    '#title' => t('Transition'),
    '#description' => t("The transition that is used when displaying the images. If a theme comes with a custom transition (like the lightbox theme) leave as not set and Galleria will use the theme default.<ul><li>%fade will fade between images.</li><li>%flash will fade into the background color between images.</li><li>%pulse quickly removes the image into background color, then fades the next image.</li><li>%slide will slide the images using the Galleria easing depending on image position.</li><li>%fadeslide will fade between images and slide slightly at the same time.</li></ul>", array('%fade' => t('Fade'), '%flash' => t('Flash'), '%pulse' => t('Pulse'), '%slide' => t('Slide'), '%fadeslide' => t('Fade/Slide')), array('langcode' => 'en')),
    '#default_value' => $view->options['views_jqfx_galleria']['transition'],
    '#options' => $transitions,
  );
  $transitions_initial = array(
    '' => t('Not set'),
    'fade' => t('Fade'),
    'flash' => t('Flash'),
    'pulse' => t('Pulse'),
    'slide' => t('Slide'),
    'fadeslide' => t('Fade/Slide'),
  );
  $form['views_jqfx_galleria']['transition_initial'] = array(
    '#type' => 'select',
    '#title' => t('Transition initial'),
    '#description' => t("Optionally set a different transition for the first image"),
    '#default_value' => $view->options['views_jqfx_galleria']['transition_initial'],
    '#options' => $transitions_initial,
  );
  $form['views_jqfx_galleria']['transition_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Transition speed'),
    '#description' => t("The milliseconds used when applying the transition."),
    '#default_value' => $view->options['views_jqfx_galleria']['transition_speed'],
  );
  $form['views_jqfx_galleria']['easing'] = array(
    '#type' => 'textfield',
    '#title' => t('Easing'),
    '#description' => t('You can use this option to control the animation easing on a global level in Galleria. Besides the built-in jQuery easings %linear and %swing, Galleria includes the following easings:<ul><li>%galleria</li><li>%galleriaIn</li><li>%galleriaOut</li></ul>You can use any of these easings or any other easing plugin, f.ex the jQuery Easing Plugin.', array('%linear' => t('linear'), '%swing' => t('swing'), '%galleria' => t('galleria'), '%galleriaIn' => t('galleriaIn'), '%galleriaOut' => t('galleriaOut'))),
    '#default_value' => $view->options['views_jqfx_galleria']['easing'],
  );
  $form['views_jqfx_galleria']['advanced_functions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Advanced functions'),
    '#description' => t('Check to display the advanced functions (Caution: experts only!). See: !url for examples.', array('!url' => l(t('http://galleria.aino.se/docs/1.2/', array(), array('langcode' => 'en')), 'http://galleria.aino.se/docs/1.2/', array('attributes' => array('target' => '_blank')))), array('langcode' => 'en')),
    '#default_value' => $view->options['views_jqfx_galleria']['advanced_functions'],
  );
  $form['views_jqfx_galleria']['debug'] = array(
    '#type' => 'select',
    '#title' => t('Debug'),
    '#description' => t('By default, Galleria fails most errors silently, but in development mode you should turn debug on to throw errors when something is not right.'),
    '#default_value' => $view->options['views_jqfx_galleria']['debug'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-advanced-functions' => array(TRUE)),
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['minify_js'] = array(
    '#type' => 'select',
    '#title' => t('Use minified javascript'),
    '#description' => t('When selected the minified version of the galleria plugin will be loaded to improve page speed. Be sure this file has been renamed to galleria.min.js with the release number removed.'),
    '#default_value' => $view->options['views_jqfx_galleria']['minify_js'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-advanced-functions' => array(TRUE)),
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['keep_source'] = array(
    '#type' => 'select',
    '#title' => t('Keep source'),
    '#description' => t('By default, Galleria clears the source container when rendering the gallery. Sometimes you need to apply the gallery some place else, like a lightbox, but still want to keep the thumbnails in the source. This option sets if the source HTML should be left intact. Setting this to ``true`` will also add click events inside the source code, to make your thumbnails clickable. Useful for building custom thumbnails and still have galleria control the gallery.'),
    '#default_value' => $view->options['views_jqfx_galleria']['keep_source'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-advanced-functions' => array(TRUE)),
    '#options' => array(
      FALSE => t('Disabled'),
      TRUE => t('Enabled'),
    ),
  );
  $form['views_jqfx_galleria']['data_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Data selector'),
    '#description' => t("The selector Galleria should look for in the HTML source. If left empty Galleria will default to 'img'. If you are using other elements than images to store data in the HTML, you can set this option to configure Galleria to look for the right elements. You can use any jQuery selector."),
    '#default_value' => $view->options['views_jqfx_galleria']['data_selector'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-advanced-functions' => array(TRUE)),
  );
  $form['views_jqfx_galleria']['data_source'] = array(
    '#type' => 'textfield',
    '#title' => t('Data source'),
    '#description' => t("This is for defining where to look for the Galleria data. By default, Galleria looks for HTML data in the target selector (in this module it's typically '#views-jqfx-galleria-images-1'), but you can tell Galleria to look elsewhere in the document using this option. If you are using JSON data, you can use this option to bring that JSON Array into Galleria."),
    '#default_value' => $view->options['views_jqfx_galleria']['data_source'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-advanced-functions' => array(TRUE)),
  );
  $form['views_jqfx_galleria']['data_config'] = array(
    '#type' => 'textarea',
    '#title' => t('Data config'),
    '#description' => t("This very useful function configures how the data should be extracted from the source. It should return an object that will blend in with the default extractions. Use this option to control how your data should be interpreted. A common case is when you have HTML markup that you would like to implement as captions. This option can tell Galleria how to interpret your HTML code so it collects the right data. The argument is a HTML element selected from the HTML source (most often an Image element). Use this to traverse the source and return the right data into Galleria. If you are using other elements than Images as source, you can change the selctor using the data selector option."),
    '#default_value' => $view->options['views_jqfx_galleria']['data_config'],
    '#rows' => 10,
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-advanced-functions' => array(TRUE)),
  );
  $form['views_jqfx_galleria']['extend'] = array(
    '#type' => 'textarea',
    '#title' => t('Extend'),
    '#description' => t("This function is used to extend the init function of the theme. You will have full access to the Galleria API here. The argument is the cascaded options object, and the scope is always the Galleria gallery instance. Use extend as a method for adding custom modifications such as play/pause without creating a new theme."),
    '#default_value' => $view->options['views_jqfx_galleria']['extend'],
    '#rows' => 10,
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-style-options-views-jqfx-galleria-advanced-functions' => array(TRUE)),
  );
}

/**
 * Implements hook_views_jqfx_options_form_validate().
 */
function views_jqfx_galleria_views_jqfx_options_form_validate(&$form, &$form_state, &$view) {
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_galleria']['carousel_speed'])) {
    form_error($form['views_jqfx_galleria']['carousel_speed'], t('!setting must be numeric!', array('Carousel speed')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_galleria']['idle_speed'])) {
    form_error($form['views_jqfx_galleria']['idle_speed'], t('!setting must be numeric!', array('Idle speed')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_galleria']['idle_time'])) {
    form_error($form['views_jqfx_galleria']['idle_time'], t('!setting must be numeric!', array('Idle time')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_galleria']['image_margin'])) {
    form_error($form['views_jqfx_galleria']['image_margin'], t('!setting must be numeric!', array('Image margin')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_galleria']['image_pan_smoothness'])) {
    form_error($form['views_jqfx_galleria']['image_pan_smoothness'], t('!setting must be numeric!', array('Image pan smoothness')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_galleria']['lightbox_fade_speed'])) {
    form_error($form['views_jqfx_galleria']['lightbox_fade_speed'], t('!setting must be numeric!', array('Lightbox fade speed')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_galleria']['lightbox_transition_speed'])) {
    form_error($form['views_jqfx_galleria']['lightbox_transition_speed'], t('!setting must be numeric!', array('Lightbox transition speed')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_galleria']['show'])) {
    form_error($form['views_jqfx_galleria']['show'], t('!setting must be numeric!', array('Show')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_galleria']['thumb_margin'])) {
    form_error($form['views_jqfx_galleria']['thumb_margin'], t('!setting must be numeric!', array('Thumb margin')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_jqfx_galleria']['transition_speed'])) {
    form_error($form['views_jqfx_galleria']['transition_speed'], t('!setting must be numeric!', array('Transition speed')));
  }
}
