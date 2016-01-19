<?php
/**
 * @file
 * Views Slideshow integration.
 */

/**
 * Implements hook_views_slideshow_slideshow_info().
 */
function views_slideshow_royalslider_views_slideshow_slideshow_info() {
  $options = array(
    'views_slideshow_royalslider' => array(
      'name' => t('RoyalSlider'),
      'accepts' => array(
//        'goToSlide',
//        'nextSlide',
//        'pause',
//        'play',
//        'previousSlide',
      ),
      'calls' => array(
//        'transitionBegin',
//        'transitionEnd',
//        'goToSlide',
//        'pause',
//        'play',
//        'nextSlide',
//        'previousSlide',
      ),
    ),
  );
  return $options;
}

/**
 * Implements hook_views_slideshow_skin_info().
 *
 * NOTE: Even though this would be really cool to implement like this, we're
 * persisting theme info in the optionsets so that it can be re-used by field
 * formatters and any other RoyalSlider implementation.
 */
//function views_slideshow_royalslider_views_slideshow_skin_info() {
//  return array(
//    'default' => array(
//      'name' => t('Default'),
//      'default-inverted' => t('Default Inverted'),
//      'minimal-white' => t('Minimal White'),
//      'universal' => t('Universal'),
//    ),
//  );
//}

/**
 * Implements hook_views_slideshow_options_form().
 */
function views_slideshow_royalslider_views_slideshow_slideshow_type_form(&$form, &$form_state, &$view) {
  // Check for library and display warning message if not configured properly.
  $library_info = libraries_detect('royalslider');
  if (!isset($library_info) || $library_info['installed'] !== TRUE) {
    drupal_set_message(t('You need to install the RoyalSlider plugin. You can find the plugin at !url.', array('!url' => l('http://dimsemenov.com/plugins/royal-slider', 'http://dimsemenov.com/plugins/royal-slider', array('attributes' => array('target' => '_blank'))))), 'error');
  }

  $form['views_slideshow_royalslider']['intro'] = array(
    '#markup' => '<strong>' . t('Important') . ':</strong> ' . t('The RoyalSlider Views Slideshow plugin requires an image field to be added to the display (image styles will be overridden). Any extra fields will be joined together and treated as the caption. I realized this isn\'t ideal so please feel free to propose improvements in the issue queue.'),
  );

  // All settings stored in option sets.
  $form['views_slideshow_royalslider']['optionset'] = array(
    '#title' => t('Option Set'),
    '#description' => t('All RoyalSlider settings are stored in Option Sets.'),
    '#type' => 'select',
    '#options' => _royalslider_optionsets(),
    '#default_value' => $view->options['views_slideshow_royalslider']['optionset'],
  );

  // Allow the skin to be overridden per views slideshow instance.
  $form['views_slideshow_royalslider']['skin'] = array(
    '#title' => t('Skin'),
    '#description' => t('Override the Option Set\'s skin.'),
    '#type' => 'select',
    '#options' => _royalslider_skins(),
    '#default_value' => $view->options['views_slideshow_royalslider']['skin'],
  );

  $field_options = array('' => t('- None -'));
  $field_labels = $view->display->handler->get_field_labels(TRUE);
  $field_options += $field_labels;

  // Allow the user to select a single field as the main image.
  // @TODO: beter describe the effect of selecting a main image field? (for example thumbs, fullscreen)
  $form['views_slideshow_royalslider']['main_image'] = array(
    '#title' => t('Image'),
    '#description' => t('Field used as the slide\'s main image.'),
    '#type' => 'select',
    '#default_value' => $view->options['views_slideshow_royalslider']['main_image'],
    '#options' => $field_options,
  );

  // Allow user to select one caption field.
  $form['views_slideshow_royalslider']['caption'] = array(
    '#title' => t('Caption'),
    '#description' => t('Field used as slide caption.'),
    '#type' => 'select',
    '#default_value' => $view->options['views_slideshow_royalslider']['caption'],
    '#options' => $field_options,
  );
}

/**
 * Implements hook_views_slideshow_option_definition().
 */
function views_slideshow_royalslider_views_slideshow_option_definition() {
  $options['views_slideshow_royalslider'] = array(
    'contains' => array(
      'optionset' => array('default' => 'default'),
      'skin' => array('default' => 'default'),
      'main_image' => array('default' => ''),
      'caption' => array('default' => ''),
    ),
  );

  return $options;
}

/**
 * Adds js to the option form.
 */
function views_slideshow_royalslider_form_options_js($element, &$form_state, &$form) {
  ctools_add_js('formoptions', 'views_slideshow_royalslider');
  return $element;
}
