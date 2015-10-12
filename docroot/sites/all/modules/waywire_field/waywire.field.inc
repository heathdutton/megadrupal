<?php

/**
 * @file
 * Waywire field functions.
 */

/**
 * Implements hook_field_info().
 */
function waywire_field_info() {
  return array(
    'waywire' => array(
      'label' => t('Waywire video iframe'),
      'description' => t('A video hosted on Waywire.'),
      'default_widget' => 'waywire',
      'default_formatter' => 'waywire_video_iframe',
      'property_type' => 'waywire',
      'property_callbacks' => array('waywire_property_info_callback'),
    ),
  );
}

/**
 * Callback to alter the property info of waywire fields.
 *
 * @see hook_field_info()
 */
function waywire_property_info_callback(&$info, $entity_type, $field, $instance, $field_type) {
  $name = $field['field_name'];
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$name];

  $property['type'] = ($field['cardinality'] != 1) ? 'list<waywire>' : 'waywire';
  $property['getter callback'] = 'entity_metadata_field_verbatim_get';
  $property['setter callback'] = 'entity_metadata_field_verbatim_set';
  $property['property info'] = waywire_field_data_property_info();

  unset($property['query callback']);
}

/**
 * Defines info for the properties of Waywire field data.
 */
function waywire_field_data_property_info($name = NULL) {
  return array(
    'video_id' => array(
      'label' => t('Waywire Video ID'),
      'description' => t('The ID assigned to the Waywire video'),
      'type' => 'text',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );
}

/**
 * Implements hook_field_validate().
 */
function waywire_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    // If not empty Waywire id, verify that it is a valid Waywire Id.
    if (!empty($item['video_id'])) {
      $waywire = new Waywire(variable_get('waywire_api_key'), variable_get('waywire_channel'));
      $result = $waywire->show($item['video_id']);

      if (!$result) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'waywire_invalid',
          'message' => t('Please provide a valid Waywire ID.'),
        );
      }
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function waywire_field_is_empty($item, $field) {
  return empty($item['video_id']);
}

/**
 * Implements hook_field_formatter_info().
 */
function waywire_field_formatter_info() {
  $formatters = array(
    // This formatter displays your waywire video iframe.
    'waywire_video_iframe' => array(
      'label' => t('Waywire video iframe'),
      'field types' => array('waywire'),
      'settings' => array(
        'waywire_size' => '416x290',
        'waywire_width' => NULL,
        'waywire_height' => NULL,
        'waywire_player' => 'svp',
        'waywire_layout' => NULL,
        'waywire_template' => NULL,
        'waywire_init_autoplay' => NULL,
        'waywire_embed_hidden' => NULL,
        'waywire_no_tabs' => NULL,
        'waywire_mute' => NULL,
        'waywire_bottom' => NULL,
        'waywire_companion' => NULL,
        'waywire_sync_ad' => NULL,
        'waywire_manual_controls' => NULL,
        'waywire_title_height' => NULL,
        'waywire_show_description' => NULL,
        'waywire_hp' => NULL,
        'waywire_read_more' => NULL,
        'waywire_link_target' => NULL,
      ),
    ),
    // This formatter just displays a thumbnail for your video.
    'waywire_thumbnail' => array(
      'label' => t('Waywire thumbnail'),
      'field types' => array('waywire'),
      'settings' => array(
        'image_style' => 'thumbnail',
        'image_link' => '',
      ),
    ),
  );

  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function waywire_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $waywire = new Waywire();

  if ($display['type'] == 'waywire_video_iframe') {
    $element['waywire_size'] = array(
      '#type' => 'select',
      '#title' => t('Waywire video size'),
      '#options' => $waywire->get_size_options(),
      '#default_value' => $settings['waywire_size'],
    );
    $element['waywire_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#size' => 10,
      '#default_value' => $settings['waywire_width'],
      '#states' => array(
        'visible' => array(
          ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][waywire_size]"]' => array('value' => 'custom'),
        ),
      ),
    );
    $element['waywire_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#size' => 10,
      '#default_value' => $settings['waywire_height'],
      '#states' => array(
        'visible' => array(
          ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][waywire_size]"]' => array('value' => 'custom'),
        ),
      ),
    );
    $element['waywire_player'] = array(
      '#type' => 'select',
      '#title' => t('Player type'),
      '#options' => $waywire->get_player_options(),
      '#default_value' => $settings['waywire_player'],
    );
    $element['waywire_layout'] = array(
      '#type' => 'select',
      '#title' => t('Player Layout'),
      '#options' => $waywire->get_layout_options(),
      '#default_value' => $settings['waywire_layout'],
    );
    $element['waywire_template'] = array(
      '#type' => 'textfield',
      '#title' => t('Player skin'),
      '#description' => t('A template CID. Defaults to the value selected in global configration configiration'),
      '#default_value' => $settings['waywire_template'],
    );
    $element['waywire_init_autoplay'] = array(
      '#type' => 'checkbox',
      '#title' => t('Play video automatically when loaded (autoplay).'),
      '#default_value' => $settings['waywire_init_autoplay'],
    );
    $element['waywire_embed_hidden'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove the ability to grab an embed code'),
      '#default_value' => $settings['waywire_embed_hidden'],
    );
    $element['waywire_no_tabs'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove the tab bars.'),
      '#default_value' => $settings['waywire_no_tabs'],
    );
    $element['waywire_mute'] = array(
      '#type' => 'checkbox',
      '#title' => t('Mute video (Magnify hosted videos only)'),
      '#default_value' => $settings['waywire_mute'],
    );
    $element['waywire_bottom'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show or hide a bottom banner ad (Multi View Player only)'),
      '#default_value' => $settings['waywire_bottom'],
    );
    $element['waywire_companion'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show or hide the companion ad overlay (Default layout only)'),
      '#default_value' => $settings['waywire_companion'],
    );
    $element['waywire_sync_ad'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show or hide companion ad overlay only when the video plays. Do not show on player load. (Default layout only)'),
      '#default_value' => $settings['waywire_sync_ad'],
    );
    $element['waywire_manual_controls'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show or hide manual controls (Thumbnail widget only)'),
      '#default_value' => $settings['waywire_manual_controls'],
    );
    $element['waywire_title_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Title height'),
      '#description' => t('Show or hide the title bar (Defaults to 31 if blank).'),
      '#default_value' => $settings['waywire_title_height'],
    );
    $element['waywire_show_description'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show the video description below the player (Viral layout only)'),
      '#default_value' => $settings['waywire_show_description'],
    );
    $element['waywire_hp'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide the playlist panel (Multi-View player only)'),
      '#default_value' => $settings['waywire_hp'],
    );
    $element['waywire_read_more'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show a “read more” link for the video, if feature is activated for your channel
                     and one is configured forthe video.'),
      '#default_value' => $settings['waywire_read_more'],
    );
    $element['waywire_link_target'] = array(
      '#type' => 'select',
      '#title' => t('Thumbnail widget only. Choose where thumbnail links open.'),
      '#options' => array(
        NULL => '',
        '_blank' => t('_blank'),
        '_top' => t('_top'),
      ),
      '#default_value' => $settings['waywire_link_target'],
    );
  }
  if ($display['type'] == 'waywire_thumbnail') {
    $element['image_style'] = array(
      '#type' => 'select',
      '#title' => t('Image style'),
      '#options' => image_style_options(FALSE),
      '#default_value' => $settings['image_style'],
      '#empty_option' => t('None (original image)'),
    );

    // Option to link the thumbnail to its original node or Waywire video page.
    $element['image_link'] = array(
      '#title' => t('Link image to'),
      '#type' => 'select',
      '#default_value' => $settings['image_link'],
      '#empty_option' => t('Nothing'),
      '#options' => $waywire->get_link_types(),
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function waywire_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $waywire = new Waywire();

  // Summary for the video style.
  if ($display['type'] == 'waywire_video_iframe') {
    $video_sizes = $waywire->get_size_options();
    if (isset($video_sizes[$settings['waywire_size']])) {
      $summary = t('Waywire video: @size', array('@size' => $video_sizes[$settings['waywire_size']]));
    }
    else {
      $summary = t('Waywire video: 416px by 290px');
    }

    $parameters = array(
      $settings['waywire_layout'],
      $settings['waywire_template'],
      $settings['waywire_autoplay'],
      $settings['waywire_embed_hidden'],
      $settings['waywire_no_tabs'],
      $settings['waywire_mute'],
      $settings['waywire_bottom'],
      $settings['waywire_companion'],
      $settings['waywire_sync_ad'],
      $settings['waywire_manual_controls'],
      $settings['waywire_title_height'],
      $settings['waywire_show_description'],
      $settings['waywire_hp'],
      $settings['waywire_read_more'],
      $settings['waywire_link_target'],
    );

    // If it has one of these parameters, change the summary text.
    foreach ($parameters as $parameter) {
      if ($parameter) {
        $summary .= t(', custom parameters');
        break;
      }
    }
    return $summary;
  }

  // Summary for the thumbnail style.
  if ($display['type'] == 'waywire_thumbnail') {
    $image_styles = image_style_options(FALSE);
    // Unset possible 'No defined styles' option.
    unset($image_styles['']);
    if (isset($image_styles[$settings['image_style']])) {
      $summary = t('Image style: @style.', array('@style' => $image_styles[$settings['image_style']]));
    }
    else {
      $summary = t('Original image.');
    }

    // Display this setting only if image is linked.
    $link_types = $waywire->get_link_types();
    if (isset($settings['image_link']) && isset($link_types[$settings['image_link']])) {
      $summary .= '<br/>' . t('Linked to:') . ' ' . $link_types[$settings['image_link']] . '.';
    }

    return $summary;
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function waywire_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    // This formatter outputs the waywire embed code.
    case 'waywire_video_iframe':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'waywire_video_iframe',
          '#video_id' => $item['video_id'],
          '#video_url' => $item['video_url'],
          '#player' => array_key_exists('waywire_player', $display['settings']) ? $display['settings']['waywire_player'] : NULL,
          '#size' => array_key_exists('waywire_size', $display['settings']) ? $display['settings']['waywire_size'] : NULL,
          '#width' => array_key_exists('waywire_width', $display['settings']) ? $display['settings']['waywire_width'] : NULL,
          '#height' => array_key_exists('waywire_height', $display['settings']) ? $display['settings']['waywire_height'] : NULL,
          '#layout' => array_key_exists('waywire_layout', $display['settings']) ? $display['settings']['waywire_layout'] : NULL,
          '#template' => array_key_exists('waywire_template', $display['settings']) ? $display['settings']['waywire_template'] : NULL,
          '#autoplay' => array_key_exists('waywire_init_autoplay', $display['settings']) ? $display['settings']['waywire_init_autoplay'] : NULL,
          '#embed_hidden' => array_key_exists('waywire_embed_hidden', $display['settings']) ? $display['settings']['waywire_embed_hidden'] : NULL,
          '#no_tabs' => array_key_exists('waywire_no_tabs', $display['settings']) ? $display['settings']['waywire_no_tabs'] : NULL,
          '#mute' => array_key_exists('waywire_mute', $display['settings']) ? $display['settings']['waywire_mute'] : NULL,
          '#bottom' => array_key_exists('waywire_bottom', $display['settings']) ? $display['settings']['waywire_bottom'] : NULL,
          '#companion' => array_key_exists('waywire_companion', $display['settings']) ? $display['settings']['waywire_companion'] : NULL,
          '#sync_ad' => array_key_exists('waywire_sync_ad', $display['settings']) ? $display['settings']['waywire_sync_ad'] : NULL,
          '#manual_controls' => array_key_exists('waywire_manual_controls', $display['settings']) ? $display['settings']['waywire_manual_controls'] : NULL,
          '#title_height' => array_key_exists('waywire_title_height', $display['settings']) ? $display['settings']['waywire_title_height'] : NULL,
          '#lb' => $item['lb'],
          '#lb_playlist' => $item['lb_playlist'],
          '#lb_counter' => $item['lb_counter'],
          '#show_description' => array_key_exists('waywire_show_description', $display['settings']) ? $display['settings']['waywire_show_description'] : NULL,
          '#hp' => array_key_exists('waywire_hp', $display['settings']) ? $display['settings']['waywire_hp'] : NULL,
          '#read_more' => array_key_exists('waywire_read_more', $display['settings']) ? $display['settings']['waywire_read_more'] : NULL,
          '#link_target' => array_key_exists('waywire_link_target', $display['settings']) ? $display['settings']['waywire_link_target'] : NULL,
        );
      }

      // Allow other modules to alter the video to be displayed.
      foreach (module_implements('waywire_video_alter') as $module) {
        $function = $module . '_waywire_video_alter';
        $function($entity, $element);
      }
      break;

    // This formatter uses an imagecache preset to generate a thumbnail.
    case 'waywire_thumbnail':
      // Check if the formatter involves a link.
      if (isset($display['settings']['image_link'])) {
        switch ($display['settings']['image_link']) {
          case 'content':
            $uri = entity_uri($entity_type, $entity);
            $uri['options']['html'] = TRUE;
            break;

          case 'waywire':
            $uri['path'] = isset($items[0]['video_url']) ? $items[0]['video_url'] : '#';
            break;

        }
      }

      foreach ($items as $delta => $item) {
        // Load the image.
        $file = file_load($item['thumbnail_fid']);

        if ($file) {
          $item['uri'] = $file->uri;
          $item['alt'] = $entity->title;
          $item['title'] = '';
          $item['attributes'] = array();

          $element[$delta] = array(
            '#theme' => 'image_formatter',
            '#item' => $item,
            '#image_style' => $display['settings']['image_style'],
            '#path' => isset($uri) ? $uri : '',
          );
        }
      }
      break;

  }

  return $element;
}

/**
 * Implements hook_field_widget_info().
 */
function waywire_field_widget_info() {
  return array(
    'waywire' => array(
      'label' => t('waywire'),
      'field types' => array('waywire'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function waywire_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element += array(
    '#type' => 'fieldset',
  );

  $required = $element['#required'];
  $item =& $items[$delta];

  $element['video_id'] = array(
    '#title' => t('Waywire Video Id'),
    '#type' => 'textfield',
    '#description' => t('Enter the Waywire ID'),
    '#required' => $required,
    '#default_value' => isset($item['video_id']) ? $item['video_id'] : '',
  );
  $element['leave_behind'] = array(
    '#title' => 'Leave Behind',
    '#type' => 'fieldset',
  );
  $element['leave_behind']['lb'] = array(
    '#title' => t('Enable Leave Behind'),
    '#type' => 'checkbox',
    '#description' => t('Load a leave behind wit related video, default is Most Recent Videos'),
    '#default_value' => isset($item['lb']) ? $item['lb'] : FALSE,
  );

  $element['leave_behind']['lb_counter'] = array(
    '#title' => t('Playlist Counter'),
    '#type' => 'textfield',
    '#description' => t('Any positive number sets the countdown time to that number of seconds, 0 will skip the leave behind entirely and move onto the next video immediately, -1 will disable the counter and the player will notauto-advance to the next video'),
    '#required' => $required,
    '#default_value' => isset($item['lb_counter']) ? $item['lb_counter'] : 10,
    '#states' => array(
      'visible' => array(
        ':input[name="' . $field['field_name'] . '[' . $langcode . '][' . $delta . '][leave_behind][lb]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $element['leave_behind']['lb_playlist'] = array(
    '#title' => t('Playlist CID'),
    '#type' => 'textfield',
    '#description' => t('Load a specific playlist into the leave behind panel, separate from a playlist loaded into the Multi-View Player'),
    '#required' => $required,
    '#default_value' => isset($item['lb_playlist']) ? $item['lb_playlist'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="' . $field['field_name'] . '[' . $langcode . '][' . $delta . '][leave_behind][lb]"]' => array('checked' => TRUE),
      ),
    ),
  );

  return $element;
}

/**
 * Implements hook_field_insert().
 */
function waywire_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  waywire_add_video($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_update().
 */
function waywire_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  waywire_add_video($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Adds video thumbnail to field.
 *
 * @see hook_field_update()
 */
function waywire_add_video($entity_type, $entity, $field, $instance, $langcode, &$items) {
  $waywire = new Waywire(variable_get('waywire_api_key'), variable_get('waywire_channel'));
  $media = new MediaInternetFileHandler('');
  foreach ($items as &$item) {
    // Move the fieldset data outside the array so it saves propery.
    if (isset($item['leave_behind'])) {
      foreach ($item['leave_behind'] as $name => $value) {
        $item[$name] = $value;
      }
    }

    $waywire->show($item['video_id']);
    // Get Thumnail and save it.
    $thumbnail_path = $waywire->thumbnail;
    $file_obj = file_uri_to_object($thumbnail_path);
    $media->preSave($file_obj);
    $media->postSave($file_obj);

    $item['thumbnail_url'] = $thumbnail_path;
    $item['thumbnail_fid'] = $file_obj->fid;
    $item['video_url'] = $waywire->video_url;
  }
}
