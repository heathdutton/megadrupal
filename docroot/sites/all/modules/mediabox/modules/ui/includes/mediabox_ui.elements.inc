<?php

/**
 * @file
 * Implements hook_element_info()
 */
function mediabox_ui_element_info() {
  $types = array();

  // @todo - there is still a lot of things to be done before we can use
  // mediabox element as fapi element - main problem is that everything
  // is heavily binded with field API, see for example
  // mediabox_ui_field_widget_form().
  // All this means that mediabox element can not be used directly in FAPI for
  // now.
  $types['mediabox'] = array(
    '#input' => TRUE,
    '#process' => array('mediabox_element_process'),
    '#element_validate' => array(),
    '#required' => FALSE,
    '#mediabox_options' => array(
      // @todo - there is really no need to add mediabox_ prefix to all this
      // options, it is enough that they are in #mediabox_options array.
      'mediabox_allowed_extensions' => 'gif jpg jpeg png',
      'mediabox_bundle' => 'default',
      'mediabox_display_bundle' => 'display',
      'mediabox_image_styles_to_crop' => array(
        'thumbnail', 'medium', 'large',
      ),
      'mediabox_default_style' => 'thumbnail',
      'mediabox_default_style_can_change' => 0,
      'mediabox_crop_source' => 'crop_image_style',
      'mediabox_default_crop_style' => 'large',
      'mediabox_lock_aspect_ratio' => 0,
      'mediabox_crop_aspect_ratio' => 0,
      'mediabox_show_inline_placeholder_help' => 0,
      'mediabox_show_image_info' => 1,
      'mediabox_show_additional_fields' => 0,
      'mediabox_better_ui' => 1,
      'mediabox_better_ui_settings' => array(
        'item_in_popup' => 1,
        'popup_title' => t('Edit image'),
      ),
      'mediabox_texts' => array(
        'add_button' => t('Add new items'),
        'delete_button' => t('Delete selected item'),
        'restore_button' => t('Restore selected item'),
        'browse_button' => t('Browse all items'),
        'upload_button' => t('Upload new items'),
        'add_image_text' => t('Add image'),
        'add_images_text' => t('Add images'),
      ),
    ),
  );

  return $types;
}

/**
 * #process callback for the mediabox form element.
 */
function mediabox_element_process(&$element, &$form_state, &$form) {
  $item = $element['#item'];
  $delta = $element['#delta'];
  $language = $element['#language'];
  $element_name = $element['#parents'][0];
  $mediabox_options = $element['#mediabox_options'];

  $data = array();

  $mediabox_display = entity_create('mediabox_display', array('type' => $mediabox_options['mediabox_display_bundle']));
  $data['mediabox_display'] = $mediabox_display;

  $mid = NULL;
  if (isset($form_state['values'][$element_name][$language][$delta]['mid'])) {
    $mid = $form_state['values'][$element_name][$language][$delta]['mid'];
  }

  // Load source mediabox if mid available.
  if (!empty($mid) || !empty($item['mid'])) {
    $mid = !empty($item['mid']) ? $item['mid'] : $mid;
    $mediabox = mediabox_load($mid);

    if (isset($item['display_mid']) && !empty($item['display_mid'])) {
      $mediabox_display = mediabox_display_load($item['display_mid']);
    }

    $data = array(
      'display' => $mediabox,
      'mediabox_display' => $mediabox_display,
    );
  }

  // Store our entities data.
  $element['entity'] = array(
    '#type' => 'value',
    '#value' => $data,
  );

  // Display empty message if no media.
  if (empty($mid)) {
    $element['no_media_message'] = array(
      '#markup' => '<div class="mediabox-message">' . t('No media file choosen') . '</div>',
    );
  } 
  elseif (!empty($mid) && $mediabox_display) {

    // Inline placeholder help text.
    if (!empty($mediabox_options['mediabox_show_inline_placeholder_help'])) {
      if (!empty($item['display_mid'])) {
        $placeholder = '[mediabox:id:' . $item["display_mid"] . ']';
      }
      else {
        $placeholder = '[mediabox:' . $delta . ']';
      }

      $element['placeholder'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="mediabox-placeholder">',
        '#suffix' => '</div>',
        '#markup' => "<label>" . t('Inline placeholder:') . "</label>" . $placeholder .
          " " . t("(Copy-paste this placeholder to textarea in order to display entity. Full html filter required)"),
      );
    }

    // Display image thumbnail. This thumbnail is by no way connected with
    // resulting mediabox display object
    $fid = !empty($item['fid']) ? $item['fid'] : $mediabox->fid;

    $element['fid'] = array(
      '#type' => 'value',
      '#value' => $fid,
    );

    $element['add_fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Additional info'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#weight' => 50,
      '#access' => !empty($mediabox_options['mediabox_show_additional_fields']),
    );

    // Set #parents to 'top-level' by default.
    $form += array('#parents' => array($delta));

    // If no language is provided use the default site language.

    $options = array('language' => field_valid_language($language));
    $add_fields = (array) _field_invoke_default('form', 'mediabox_display', $mediabox_display, $element, $form_state, $options);
    $element['add_fields'] += $add_fields;

    // Add custom weight handling.
    list($id, $vid, $bundle) = entity_extract_ids('form', 'mediabox_display');

    $element['add_fields']['#pre_render'][] = '_field_extra_fields_pre_render';
    $element['add_fields']['#entity_type'] = 'mediabox_display';
    $element['add_fields']['#bundle'] = $mediabox_options['mediabox_display_bundle'];

    if (count($add_fields) <= 1) {
      $element['add_fields']['#attributes']['class'][] = 'hidden';
    }

    $element['mid'] = array(
      '#type' => 'hidden',
      '#title' => t('Title'),
      '#default_value' => $mid,
      '#value' => $mid,
    );

    // Display mid is always zero.
    // We don't want to create actual mediabox_display entities until node is
    // submitted. Field is updated on entity presave action and all required
    // references are created.
    $element['display_mid'] = array(
      '#type' => 'value',
      '#value' => !empty($item['display_mid']) ? $item['display_mid'] : 0,
    );

    // Crop form elements.
    if (!empty($mediabox_display->crop_id)) {
      $crops = mediabox_crop_load($mediabox_display->crop_id);
    }

    $crop_wrapper_id = $element_name . '-' . $language . '-' . $delta . '-crop';
    $crop_wrapper_set_id = $crop_wrapper_id . '-fieldset';

    // Image info elements.
    $element['image_info'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#title' => t('Image info'),
      '#attributes' => array('class' => array('mediabox-image-info')),
      '#access' => !empty($mediabox_options['mediabox_show_image_info']),
    );

    $element['image_info']['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => !empty($mediabox_display->title) ? $mediabox_display->title : $mediabox->title,
    );

    $element['image_info']['alternate_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Alternate text'),
      '#default_value' => !empty($mediabox_display->alternate_text) ? $mediabox_display->alternate_text : $mediabox->alternate_text,
    );

    $image_styles = array_filter($mediabox_options['mediabox_image_styles_to_crop']);

    if (!empty($mediabox_options['mediabox_default_style_can_change'])) {
      $element['image_info']['image_style'] = array(
        '#title' => t('Display from image style'),
        '#type' => 'select',
        '#options' => $image_styles,
        '#default_value' => !empty($mediabox_display->style_name) ? $mediabox_display->style_name : $mediabox_options['mediabox_default_style'],
      );
    }
    else {
      $element['image_info']['image_style'] = array('#type' => 'value', '#value' => !empty($mediabox_display->style_name) ? $mediabox_display->style_name : $mediabox_options['mediabox_default_style']);
    }

    // Crop part.

    $file = file_load($fid);
    $image = image_load($file->uri);
    $default_crops = array(
      'x' => 0,
      'y' => 0,
      'x1' => $image->info['width'],
      'y1' => $image->info['height'],
      'width' => $image->info['width'],
      'height' => $image->info['height'],
    );

    $element['crop'] = array(
      '#type' => 'container',
    );

    $element['crop'] = array(
      '#type' => 'fieldset',
      '#title' => t('Crop image'),
      '#id' => $crop_wrapper_set_id,
      '#attributes' => array('class' => array('crop-image-set')),
      '#collapsible' => FALSE,
    );

    if (!empty($mediabox_options['mediabox_default_crop_style_can_change'])) {
      $element['crop']['crop_image_style'] = array(
        '#title' => t('Crop from image style'),
        '#type' => 'select',
        '#options' => $image_styles,
        '#ajax' => array(
          'callback' => 'mediabox_ui_crop',
          'wrapper' => $crop_wrapper_id,
          'method' => 'html',
        ),
        '#default_value' => !empty($crops->origin_image_style) ? $crops->origin_image_style : $mediabox_options['mediabox_default_crop_style'],
      );
      $element['crop']['ratio'] = array(
        '#type' => 'checkbox',
        '#title' => t('Lock aspect ratio'),
        '#ajax' => array(
          'callback' => 'mediabox_ui_crop',
          'wrapper' => $crop_wrapper_id,
          'method' => 'html',
        ),
        '#attributes' => array(
          'class' => array('ratio')
        ),
        '#default_value' => !empty($crops->ratio) ? $crops->ratio : $mediabox_options['mediabox_lock_aspect_ratio'],
      );
    }
    else {
      $element['crop']['crop_image_style'] = array(
        '#type' => 'hidden',
        '#value' => !empty($crops->origin_image_style) ? $crops->origin_image_style : $mediabox_options['mediabox_default_crop_style'],
      );
      $element['crop']['ratio'] = array(
        '#type' => 'hidden',
        '#value' => !empty($crops->ratio) ? $crops->ratio : $mediabox_options['mediabox_lock_aspect_ratio'],
        '#attributes' => array(
          'class' => array('ratio')
        ),
      );
    }

    $element['crop']['id'] = array(
      '#type' => 'hidden',
      '#value' => !empty($crops->id) ? $crops->id : 0,
    );

    // Crop coordinates form elements.
    foreach (array('x', 'y', 'x1', 'y1', 'height', 'width') as $coord) {
      $element['crop'][$coord] = array(
        '#type' => 'textfield',
        '#title' => strtoupper($coord),
        '#size' => 15,
        // Bigger maxlenght for jcrop textfields so we can accept longer numbers
        // that are coming from Jcrop operations.
        // @see https://www.drupal.org/node/2155295 for more info.
        '#maxlength' => 25,
        '#prefix' => '<div class=jcrop-attribute>',
        '#suffix' => '</div>',
        '#attributes' => array(
          'class' => array('jcrop-' . $coord, 'hidden'),
        ),
        '#default_value' => !empty($crops->$coord) ? $crops->$coord : $default_crops[$coord],
      );
    }

    $element['crop']['min-height'] = array(
      '#type' => 'hidden',
      '#prefix' => '<div class=jcrop-attribute>',
      '#suffix' => '</div>',
      '#value' => !empty($crops->height) ? $crops->height : 10,
      '#attributes' => array(
        'class' => array('jcrop-min-height', 'hidden'),
      ),
    );

    $element['crop']['min-width'] = array(
      '#type' => 'hidden',
      '#prefix' => '<div class=jcrop-attribute>',
      '#suffix' => '</div>',
      '#value' => !empty($crops->width) ? $crops->width : 10,
      '#attributes' => array(
        'class' => array('jcrop-min-width', 'hidden'),
      ),
    );

    $file = file_load($mediabox->fid);
    $image_properties = array(
      'path' => $file->uri,
      'attributes' => array(
        'class' => array('jcrop-image', 'mediabox-image'),
      ),
      'width' => '',
      'height' => '',
      'style_name' => empty($mediabox_options['mediabox_default_crop_style_can_change']) ?  $mediabox_options['mediabox_default_crop_style'] : (!empty($crops->origin_image_style) ? $crops->origin_image_style : $mediabox_options['mediabox_default_crop_style']),
    );

    if ($image_properties['style_name'] != 'origin') {
      $image = theme('image_style', $image_properties);
    }
    else {
      $image = theme('image', $image_properties);
    }
    $element['crop']['image'] = array(
      '#type' => 'markup',
      '#prefix' => "<div id=" . $crop_wrapper_id . ">",
      '#suffix' => "</div>",
      '#markup' => $image,
    );
  }

  $element['actions'] = array(
    '#weight' => 100,
  );

  return $element;
}

/**
 * Form ajah callback for crop ui part.
 */
function mediabox_ui_crop($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $triggering_element = $form_state['triggering_element'];
  $aspect_ratio = 0;

  $element_name = end($triggering_element['#array_parents']);
  $crop_image_style = NULL;

  //@todo refactor this
  switch ($element_name) {
    case 'crop_image_style':
      $crop_image_style = $triggering_element['#value'];
      $nested_values = drupal_array_get_nested_value($form, array_slice($triggering_element['#array_parents'], 0, -2));
      $aspect_ratio = $nested_values['crop']['ratio']['#value'];
      $style_name = $nested_values['image_info']['image_style']['#value'];
      break;

    case 'ratio':
      $nested_values = drupal_array_get_nested_value($form, array_slice($triggering_element['#array_parents'], 0, -2));
      $style_name = $nested_values['image_info']['image_style']['#value'];
      $aspect_ratio = $triggering_element['#value'];
      $crop_image_style = $nested_values['crop']['crop_image_style']['#value'];
      break;
    break;
  }

  if ($crop_image_style == 'origin') {
    $crop_image_style = NULL;
  }
  // Unfortunately we have no way to discover on which field/language/item was trigger submitted
  // We are parsing wrapper_id to get the required values
  // This is sad day for developers all across the world.
  $wrapper_id = $triggering_element['#ajax']['wrapper'];
  $data = explode('-', $wrapper_id);

  $field_name = $data[0];
  $language = $data[1];
  $delta = $data[2];

  if (empty($style_name) || $style_name == 'origin') {
    // No style selected, we are croping on original image.
    $file = file_load($form_state['values'][$field_name][$language][$delta]['fid']);
    $image = image_load($file->uri);

    $init_coords = array(
      'x' => 0,
      'y' => 0,
      'x1' => $image->info['width'],
      'y1' => $image->info['height'],
      'w' => $image->info['width'],
      'h' => $image->info['height'],
    );
  } 
  else {
    // style selected
    $style = image_style_load($style_name);
    $init_coords = mediabox_crop_get_default_coords_from_style($style);
  }

  drupal_add_js(array(
    'crop' => array(
      'ratio' => $aspect_ratio,
      'coords' => $init_coords,
      'wrapper_id' => $wrapper_id,
    )
  ), 'setting');

  $mediabox = mediabox_load($form_state['values'][$field_name][$language][$delta]['mid']);

  module_load_include('inc', 'mediabox_crop', 'includes/mediabox_crop.forms');
  return mediabox_crop_form_style($mediabox, $crop_image_style);
}
