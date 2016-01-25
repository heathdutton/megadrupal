<?php

/**
 * @file
 * Content field formatter functions for Image Matrix module.
 */

/**
 * Implements hook_field_formatter_info().
 */
function imagematrix_field_formatter_info() {
  $formatters = array(
    'imagematrix' => array(
      'label' => t('Image Matrix'),
      'field types' => array('image', 'media', 'video'),
      'settings' => array(
        'block_width' => 600,
        'block_height' => NULL,
        'block_images_count' => 7,
        'image_padding' => 3,
        'image_link' => NULL,
        'target_image_style' => NULL,
        'randomize_layout' => FALSE,
        'cache_layout' => FALSE,
        'custom_class' => NULL,
        'custom_rel' => NULL,
        'custom_rel_id' => FALSE,
        'add_alt_element' => FALSE,
        'add_title_element' => FALSE,
      ),
    ),
  );
  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function imagematrix_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  // If formatter settings form was called from the view, let's return only
  // "Link image to" form element, as in this case all settings are configured
  // from Views' style plugin options form.
  if ($view_mode != '_dummy' && $view_mode != '_custom') {

    $element['block_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width of one block'),
      '#default_value' => $settings['block_width'],
      '#required' => TRUE,
      '#element_validate' => array('element_validate_integer_positive'),
      '#field_suffix' => 'px',
      '#size' => 5,
    );

    $element['block_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height of one block'),
      '#description' => t('This value is optional. If you leave it empty, you will most probably <br>get blocks with different heights, but images will not be cropped.<br />If you specify it on the other hand, all blocks will have the same height<br />(which would be desirable for example when displaying them next to<br />each other), but images could (and most probably will) be partially cropped.'),
      '#default_value' => $settings['block_height'],
      '#field_suffix' => 'px',
      '#size' => 5,
    );

    $element['block_images_count'] = array(
      '#type' => 'select',
      '#title' => t('Number of images in one block'),
      '#options' => drupal_map_assoc(range(2, 8)),
      '#default_value' => $settings['block_images_count'],
      '#multiple' => FALSE,
    );

    $element['randomize_layout'] = array(
      '#type' => 'checkbox',
      '#title' => t('Randomize generated layout'),
      '#description' => t('Enabling this option will try to randomize each block layout each time<br />it is generated. This option is exclusive with "Cache generated layout".'),
      '#default_value' => $settings['randomize_layout'],
    );

    $element['cache_layout'] = array(
      '#type' => 'checkbox',
      '#title' => t('Cache generated layout'),
      '#description' => t('Enabling this option will save once generated layout to the cache,<br />which could slightly improve page loading time. On the other hand<br />it will also cause images to appear in exactly the same places on each<br />page load (which could be slightly randomized otherwise). This option<br />is exclusive with "Randomize generated layout".'),
      '#default_value' => $settings['cache_layout'],
    );

    $element['image_padding'] = array(
      '#type' => 'textfield',
      '#title' => t('Image padding'),
      '#description' => t('Padding around each image.'),
      '#default_value' => $settings['image_padding'],
      '#element_validate' => array('element_validate_integer'),
      '#field_suffix' => 'px',
      '#size' => 5,
    );

  }

  $link_types = array(
    'content' => t('Content'),
    'file' => t('File'),
  );
  $element['image_link'] = array(
    '#title' => t('Link image to'),
    '#type' => 'select',
    '#default_value' => $settings['image_link'],
    '#empty_option' => t('Nothing'),
    '#options' => $link_types,
  );

  // We don't want to display target image style selection
  //  for any other field type different from "image" (do we?).
  if ($field['type'] == 'image') {
    $image_styles = image_style_options(FALSE);
    $element['target_image_style'] = array(
      '#title' => t('Target image style'),
      '#type' => 'select',
      '#default_value' => $settings['target_image_style'],
      '#empty_option' => t('None (original image)'),
      '#options' => $image_styles,
      '#states' => array(
        'invisible' => array(
          // View field configuration.
          ':input[name="options[settings][image_link]"]' => array('!value' => 'file'),
          // Content type field configuration.
          ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][image_link]"]' => array('!value' => 'file'),
        ),
      ),
    );
  }

  $element['custom_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom link class'),
    '#description' => t('Custom class to add to all image links.'),
    '#default_value' => $settings['custom_class'],
    '#states' => array(
      'visible' => array(
        // View field configuration.
        ':input[name="options[settings][image_link]"]' => array('!value' => ''),
        // Content type field configuration.
        ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][image_link]"]' => array('!value' => ''),
      ),
    ),
  );

  $element['custom_rel'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom link <em>rel</em> attribute'),
    '#description' => t('Custom <em>rel</em> attribute to add to all image links.'),
    '#default_value' => $settings['custom_rel'],
    '#states' => array(
      'visible' => array(
        // View field configuration.
        ':input[name="options[settings][image_link]"]' => array('!value' => ''),
        // Content type field configuration.
        ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][image_link]"]' => array('!value' => ''),
      ),
    ),
  );

  $element['custom_rel_id'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add content ID to <em>rel</em> attribute'),
    '#description' => t('Adds additional content ID to <em>rel</em> attribute provided above,<br />to enable separate galleries for separate content entities.'),
    '#default_value' => $settings['custom_rel_id'],
    '#states' => array(
      'visible' => array(
        // Field "custom_rel" is visible (based on "image_link" value).
        ':input[name="options[settings][image_link]"]' => array('!value' => ''),
        ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][image_link]"]' => array('!value' => ''),
        // Field "custom_rel" has value.
        ':input[name="options[settings][custom_rel]"]' => array('!value' => ''),
        ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][custom_rel]"]' => array('!value' => ''),
      ),
    ),
  );

  // We don't want to display target image style selection
  // for "media" or "video" type fields (for videos - it's pretty obvious,
  // and for media (old media, v1.x) fields - it's just not supported).
  if ($field['type'] == 'image') {
    $element['add_alt_element'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add <em>alt</em> element'),
      '#description' => t('Adds extra &lt;div&gt; element with displaying value of image <em>alt</em> attribute.'),
      '#default_value' => $settings['add_alt_element'],
    );

    $element['add_title_element'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add <em>title</em> element'),
      '#description' => t('Adds extra &lt;div&gt; element with displaying value of image <em>title</em> attribute.'),
      '#default_value' => $settings['add_title_element'],
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function imagematrix_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  $summary[] = t('Block width: !block_width px', array('!block_width' => $settings['block_width']));

  if (!empty($settings['block_heigth'])) {
    $summary[] = t('Block height: !block_height px', array('!block_height' => $settings['block_height']));
  }

  $summary[] = t('Images in one block: !block_images_count', array('!block_images_count' => $settings['block_images_count']));

  $image_padding = $settings['image_padding'] ? $settings['image_padding'] : 0;
  $summary[] = t('Image padding: !image_padding px', array('!image_padding' => $image_padding));

  $link_types = array(
    'content' => t('Images linked to content'),
    'file' => t('Images linked to file'),
  );
  // Display this setting only if image is linked.
  if (isset($link_types[$settings['image_link']])) {
    $summary[] = $link_types[$settings['image_link']];
    if ($settings['image_link'] == 'file' && !empty($settings['target_image_style'])) {
      $image_styles = image_style_options(FALSE);
      $summary[] = t('Target image style: @style', array('@style' => $image_styles[$settings['target_image_style']]));
    }
  }
  else {
    $summary[] = t('Images without links');
  }

  $cache_layout = !empty($settings['cache_layout']) ? 'yes' : 'no';
  $summary[] =  t('Cache layout: !value', array('!value' => $cache_layout));

  if (!empty($settings['custom_class'])) {
    $summary[] =  t('Custom link class');
  }

  if (!empty($settings['custom_rel'])) {
    $summary[] =  t('Custom link <em>rel</em> attribute');
  }

  if ($settings['add_alt_element']) {
    $summary[] =  t('Add <em>alt</em> element');
  }

  if ($settings['add_title_element']) {
    $summary[] =  t('Add <em>title</em> element');
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function imagematrix_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $entity_info = entity_get_info($entity_type);
  $entity_id_key = $entity_info['entity keys']['id'];
  $entity_id = $entity->$entity_id_key;

  // Check if the formatter involves a link.
  if ($display['settings']['image_link'] == 'content') {
    $uri = entity_uri($entity_type, $entity);
  }
  elseif ($display['settings']['image_link'] == 'file') {
    $link_file = TRUE;
  }

  // For media fields use child image object to display in the matrix.
  if ($field['type'] == 'media') {
    foreach ($items as $delta => $item) {
      // There could be any type of file in the "media" field,
      // and we don't want any processing for anything that is not an image.
      if ($item['file']->type == 'image') {
        $image_info = image_load($item['file']->uri);
        $items[$delta] = array_merge(
          array(
            'field_type' => 'media',
            'media' => $item,
            'title' => '',
            'alt' => '',
          ),
          (array) $item['file'],
          $image_info->info
        );
      }
    }
  }

  // For video fields use child thumbnail image object to display in the matrix.
  if ($field['type'] == 'video') {
    foreach ($items as $delta => $item) {
      // Make sure that we really have a thumbnail here.
      if (isset($item['thumbnailfile']->uri)) {
        $image_info = image_load($item['thumbnailfile']->uri);
        $items[$delta] = array_merge(
          array(
            'field_type' => 'video',
            'video' => $item,
            'title' => '',
            'alt' => '',
          ),
          (array) $item['thumbnailfile'],
          $image_info->info
        );
      }
    }
  }

  // If formatter was called from the view, let's just return $items array,
  // as in such case we don't want to do any calculations here, as we don't
  // have all images available to arrange them properly. The only thing to
  // do here would be to add links to the content the image comes from
  // if we need them later.
  if (isset($display['views_view'])) {
    $element = array();

    foreach ($items as $delta => $item) {

      // Now, we'll see if this is going to break anything,
      // but actually if we don't have an image inside $item here,
      // we shouldn't really process any further (as this could happen,
      // as old type "media" fields (v1.x) could contain any file.
      if (isset($item['type']) && $item['type'] != 'image') {
        continue;
      }

      if (isset($link_file)) {
        $uri = array(
          'path' => file_create_url($item['uri']),
          'options' => array(),
        );
      }
      $content_uri = entity_uri($entity_type, $entity);
      $item['links'] = array(
        'content' => $content_uri['path'],
        'file' => file_create_url($item['uri']),
      );

      $item['custom_class'] = $display['settings']['custom_class'];
      $item['custom_rel'] = $display['settings']['custom_rel'];
      $item['custom_rel_id'] = $display['settings']['custom_rel_id'];
      $item['entity_id'] = $entity_id;

      $element[$delta] = array(
        '#theme' => 'image_formatter',
        '#item' => $item,
        '#path' => isset($link_file) ? $uri : '',
      );

    }
    return $element;
  }

  $settings = $display['settings'];

  // Settings for MagazineLayout class.
  $mag_options = array(
    'block_width' => $settings['block_width'],
    'block_height' => $settings['block_height'],
    'block_images_count' => $settings['block_images_count'],
    'image_padding' => $settings['image_padding'],
    'randomize_layout' => $settings['randomize_layout'],
  );

  // Before initializing MagazineLayout class, let's check
  // if we don't already have block definitions stored in cache.
  if (!empty($settings['cache_layout'])) {
    $fids = array();
    foreach ($items as $item) {
      $fids[] = $item['fid'];
    }
    $cid_elements = $mag_options + array('image_link' => $settings['image_link']) + $fids;

    // Number of $cid_elements could get quite long (longer than 255 chars
    // allowed for cids), so we cannot use it directly, let's then sha1 it.
    $cid = 'imagematrix:' . sha1(implode(':', $cid_elements));

    $cached = cache_get($cid, 'cache_views');
    if (is_object($cached) && isset($cached->data)) {
      $blocks = $cached->data;
    }
  }
  // Block definitions were not found in cache,
  // so we need to regenerate them unfortunately.
  if (empty($settings['cache_layout']) || empty($cached->data)) {

    // Allow other modules to change MagazineLayout options.
    $context = array(
      'function' => __FUNCTION__,
      'entity_type' => $entity_type,
      'entity' => $entity,
      'field' => $field,
      'instance' => $instance,
      'langcode' => $langcode,
      'items' => $items,
      'display' => $display,
    );
    drupal_alter('imagematrix_layout_options', $mag_options, $context);

    // Initialize MagazineLayout class.
    $mag = new MagazineLayout($mag_options);

    foreach ($items as $id => $item) {

      // Prepare array of image attributes to be passed
      // to MagazineLayout class, and, well, pass them.
      $image_attributes = array(
        'fid'       => $item['fid'],
        'path'      => drupal_realpath($item['uri']),
        'width'     => $item['width'],
        'height'    => $item['height'],
        'title'     => $item['title'],
        'alt'       => $item['alt'],
        'entity_id' => $entity_id,
      );

      // Check if the formatter involves a link.
      if (isset($link_file)) {
        $target_path = file_create_url($item['uri']);
        if ($settings['image_link'] == 'file') {
          if ($field['type'] == 'video') {
            $target_path = file_create_url($item['video']['uri']);
          }
          else if (!empty($settings['target_image_style']) && $image_style = image_style_load($settings['target_image_style'])) {
            $target_path = image_style_url($settings['target_image_style'], $item['uri']);
          }
        }
        $uri = array(
          'path' => $target_path,
          'options' => array(),
        );
      }
      if (!empty($uri)) {
        $image_attributes['uri'] = $uri['path'];
      }

      $mag->addImage($image_attributes);

    }

    // Get array of already processed blocks from MagazineLayout class.
    $blocks = $mag->getProcessedBlocks();

    // Save blocks to cache.
    if (!empty($settings['cache_layout'])) {
      cache_set($cid, $blocks, 'cache_views');
    }

  }

  $element = array(
    '#settings' => $settings,
  );
  if (!empty($blocks)) {
    foreach ($blocks as $block_id => $images) {
      $element[$block_id] = array(
        '#theme' => 'field_imagematrix_block',
        '#images' => $images,
        '#settings' => $settings,
      );
    }
  }

  return $element;
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adds 'Flush generated images' button to Image Matrix field's format settings.
 */
function imagematrix_form_field_ui_display_overview_form_alter(&$form, &$form_state) {
  if (!empty($form_state['values'])) {
    foreach ($form_state['values']['fields'] as $field => $value) {
      if ($value['type'] == 'imagematrix') {
        $form['fields'][$field]['format']['settings_edit_form']['actions']['flush'] = array(
          '#type' => 'submit',
          '#value' => t('Flush generated images'),
          '#submit' => array('imagematrix_flush_style'),
        );
      }
    }
  }
}
