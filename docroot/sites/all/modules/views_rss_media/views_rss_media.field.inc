<?php

/**
 * Implements hook_field_formatter_info().
 */
function views_rss_media_field_formatter_info() {
  $formatters = array(
    'media_content' => array(
      'label' => t('RSS <media:content> element'),
      'field types' => array('image', 'file', 'video', 'video_embed_field'),
      'settings' => array(
        'image_style' => '',
        'group_multiple_values' => 0,
        'medium' => '',
        'expression' => '',
        'generate_hash' => 0,
        'hash_algo' => 'md5',
      ),
    ),
    'media_thumbnail' => array(
      'label' => t('RSS <media:thumbnail> element'),
      'field types' => array('image', 'file', 'video'),
      'settings' => array(),
    ),
    'media_category' => array(
      'label' => t('RSS <media:category> element'),
      'field types' => array('taxonomy_term_reference'),
      'settings' => array(),
    ),
  );
  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function views_rss_media_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  if ($field['type'] == 'image' || $field['type'] == 'file') {
    $image_styles = image_style_options(FALSE);
    $element['image_style'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#default_value' => $settings['image_style'],
      '#empty_option' => t('None (original image)'),
      '#options' => $image_styles,
    );
  }

  if ($display['type'] == 'media_content') {

    $element['group_multiple_values'] = array(
      '#title' => t('Group multiple field values in one &lt;media:group&gt; element'),
      '#type' => 'checkbox',
      '#default_value' => $settings['group_multiple_values'],
      '#description' => t('&lt;media:group&gt; element allows grouping of &lt;media:content&gt; elements that are effectively the same content, yet different representations. For instance: the same song recorded in both the WAV and MP3 format. It is an optional element that must only be used for this purpose. !more_link', array(
        '!more_link' => l('[?]', 'http://www.rssboard.org/media-rss#media-group', array('attributes' => array('title' => t('Need more information?')))),
      )),
    );

    $element['medium'] = array(
      '#title' => t('Medium'),
      '#type' => 'select',
      '#default_value' => $settings['medium'],
      '#empty_option' => t('None (do not display)'),
      '#options' => drupal_map_assoc(array('image', 'audio', 'video', 'document', 'executable')),
      '#description' => t('<em>medium</em> attribute of &lt;media:content&gt; element is the type of object. While this attribute can at times seem redundant if type is supplied, it is included because it simplifies decision making on the reader side, as well as flushes out any ambiguities between MIME type and object type. It is an optional attribute. !more_link', array(
        '!more_link' => l('[?]', 'http://www.rssboard.org/media-rss#media-content', array('attributes' => array('title' => t('Need more information?')))),
      )),
    );

    $element['expression'] = array(
      '#title' => t('Expression'),
      '#type' => 'select',
      '#default_value' => $settings['expression'],
      '#empty_option' => t('None (do not display)'),
      '#options' => drupal_map_assoc(array('sample', 'full', 'nonstop')),
      '#description' => t('<em>expression</em> attribute of &lt;media:content&gt; element determines if the object is a sample or the full version of the object, or even if it is a continuous stream. Default value is "full". It is an optional attribute. !more_link', array(
        '!more_link' => l('[?]', 'http://www.rssboard.org/media-rss#media-content', array('attributes' => array('title' => t('Need more information?')))),
      )),
    );

    // hash_file() function used to generate file hashes
    // is available only in PHP versions >= 5.1.2.
    // @see http://www.php.net/manual/en/function.hash-file.php
    // Also, no hash support for Video Embed fields, as these are not
    // stored locally, not possible then to calculate hashes for them.
    if ($field['type'] != 'video_embed_field' && version_compare(phpversion(), '5.1.2') >= 0) {

      $element['generate_hash'] = array(
        '#title' => t('Generate hash values for files'),
        '#type' => 'checkbox',
        '#default_value' => $settings['generate_hash'],
        '#description' => t('Enabling this will generate &lt;media:hash&gt; subelement for each &lt;media:content&gt; element. !more_link', array(
          '!more_link' => l('[?]', 'http://www.rssboard.org/media-rss#media-hash', array('attributes' => array('title' => t('Need more information?')))),
        )),
      );

      $element['hash_algo'] = array(
        '#title' => t('Hashing algorithm'),
        '#type' => 'select',
        '#default_value' => $settings['hash_algo'],
        '#options' => drupal_map_assoc(array('md5', 'sha1')),
        '#description' => t('Indicates the algorithm used to create the hash. Will be added as <em>algo</em> attribute of &lt;media:hash&gt; element. !more_link', array(
          '!more_link' => l('[?]', 'http://www.rssboard.org/media-rss#media-hash', array('attributes' => array('title' => t('Need more information?')))),
        )),
      );
      // Add #states value for showing/hiding "Hashing algorithm"
      // field based on state of "generate_hash" field value.
      $name = ($view_mode == '_dummy') ? 'options[settings][generate_hash]' : 'fields[' . $field['field_name'] . '][settings_edit_form][settings][generate_hash]';
      $element['hash_algo']['#states']['invisible'][':input[name="' . $name . '"]'] = array('checked' => FALSE);

    }

  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function views_rss_media_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  if ($field['type'] == 'image' || $field['type'] == 'file') {
    $image_styles = image_style_options(FALSE);
    // Unset possible 'No defined styles' option.
    unset($image_styles['']);
    // Styles could be lost because of enabled/disabled modules that defines
    // their styles in code.
    if (isset($settings['image_style']) && isset($image_styles[$settings['image_style']])) {
      $summary[] = t('Image style: @style', array('@style' => $image_styles[$settings['image_style']]));
    }
    else {
      $summary[] = t('Original image');
    }
  }

  if ($display['type'] == 'media_content') {

    if (!empty($display['settings']['group_multiple_values'])) {
      $summary[] = t('Group multiple values');
    }

    $value = (!empty($display['settings']['medium'])) ? $display['settings']['medium'] : t('none');
    $summary[] = t('Medium: !medium', array('!medium' => $value));

    $value = (!empty($display['settings']['expression'])) ? $display['settings']['expression'] : t('none');
    $summary[] = t('Expression: !expression', array('!expression' => $value));

    if (!empty($display['settings']['generate_hash'])) {
      $summary[] = t('Generate hash: !algo', array('!algo' => $display['settings']['hash_algo']));
    }

  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function views_rss_media_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  foreach ($items as $delta => $item) {

    if ($field['type'] == 'video_embed_field') {
      $url = $item['video_url'];
      $item['video_data'] = unserialize($item['video_data']);
    }

    // When generating a thumbnail for a video file,
    // let's process only thumbnail image details.
    if ($item['type'] == 'video' && $display['type'] == 'media_thumbnail') {
      $item = (array) $item['thumbnailfile'];
    }

    if (($display['type'] == 'media_content' || $display['type'] == 'media_thumbnail') && $field['type'] != 'video_embed_field') {

      // Inside a view item may contain NULL data. In that case, just return.
      if (empty($item['fid'])) {
        unset($items[$delta]);
        continue;
      }

      // Default file URL.
      $item_uri = $item['uri'];
      $url = file_create_url($item_uri);

      // For images provide styled image URL if required.
      if (!empty($display['settings']['image_style']) && $image_style = image_style_load($display['settings']['image_style'])) {
        // Get full image URI based on provided image style.
        $url = image_style_url($display['settings']['image_style'], $item_uri);
        // Make sure that image style file has already been created.
        $path = image_style_path($display['settings']['image_style'], $item_uri);
        if (!file_exists(drupal_realpath($path))) {
          image_style_create_derivative($image_style, $item_uri, $path);
        }
        $item_uri = $path;
      }

    }

    if ($display['type'] == 'media_content') {

      $rss_element = array(
        'key' => 'media:content',
        'attributes' => array(
          'url' => $url,
        ),
      );
      if (!empty($item['filesize'])) {
        $rss_element['attributes']['fileSize'] = $item['filesize'];
      }
      if (!empty($item['filemime'])) {
        $rss_element['attributes']['type'] = $item['filemime'];
      }
      if (!empty($display['settings']['medium'])) {
        $rss_element['attributes']['medium'] = $display['settings']['medium'];
      }
      if (!empty($display['settings']['expression'])) {
        $rss_element['attributes']['expression'] = $display['settings']['expression'];
      }
      if ($langcode != LANGUAGE_NONE) {
        $rss_element['attributes']['lang'] = $langcode;
      }

      // Additional properties for images.
      if ($item['type'] == 'image') {
        $media_info = image_load($item_uri);
        if (!empty($media_info)) {
          $item['info'] = $media_info;
          $rss_element['attributes']['width'] = $media_info->info['width'];
          $rss_element['attributes']['height'] = $media_info->info['height'];
        }
      }

      // Additional properties for audio files.
      if ($item['type'] == 'audio' && module_exists('getid3')) {
        $media_info = getid3_analyze(drupal_realpath($item_uri));
        if (!empty($media_info)) {
          $item['info'] = $media_info;
          $rss_element['attributes']['bitrate'] = round($media_info['audio']['bitrate'] / 1000);
          $rss_element['attributes']['samplingrate'] = $media_info['audio']['sample_rate'];
          $rss_element['attributes']['channels'] = $media_info['audio']['channels'];
          $rss_element['attributes']['duration'] = round($media_info['playtime_seconds']);
        }
      }

      // Additional properties for video files.
      if ($item['type'] == 'video' && module_exists('getid3')) {
        $media_info = getid3_analyze(drupal_realpath($item_uri));
        if (!empty($media_info)) {
          $item['info'] = $media_info;
          $rss_element['attributes']['bitrate'] = round($media_info['video']['bitrate'] / 1000);
          $rss_element['attributes']['framerate'] = $media_info['video']['frame_rate'];
          $rss_element['attributes']['duration'] = round($media_info['playtime_seconds']);
          $rss_element['attributes']['width'] = $media_info['video']['resolution_x'];
          $rss_element['attributes']['height'] = $media_info['video']['resolution_y'];
        }
      }

      // Additional properties for Video Embed fields.
      if ($field['type'] == 'video_embed_field') {
        if (!empty($item['video_data']['media$group']['media$content'][0])) {
          $rss_element['attributes'] += $item['video_data']['media$group']['media$content'][0];
        }
        if (!empty($item['video_data']['media$group']['media$title'])) {
          $rss_element['value'][] = array(
            'key' => 'media:title',
            'value' => $item['video_data']['media$group']['media$title'],
          );
        }
        if (!empty($item['video_data']['media$group']['media$description'])) {
          $rss_element['value'][] = array(
            'key' => 'media:description',
            'value' => $item['video_data']['media$group']['media$description'],
          );
        }
        if (!empty($item['video_data']['media$group']['media$thumbnail'][0])) {
          $rss_element['value'][] = array(
            'key' => 'media:thumbnail',
            'attributes' => $item['video_data']['media$group']['media$thumbnail'][0],
          );
        }
        if (!empty($item['video_data']['media$group']['media$category'])) {
          foreach ($item['video_data']['media$group']['media$category'] as $category) {
            $rss_element['value'][] = array(
              'key' => 'media:category',
              'value' => $category,
            );
          }
        }
        if (!empty($item['video_data']['media$group']['media$keywords'])) {
          $rss_element['value'][] = array(
            'key' => 'media:keywords',
            'value' => implode(', ', $item['video_data']['media$group']['media$keywords']),
          );
        }
      }

      // Generate file hash for <media:hash> element.
      // hash_file() function used to generate file hashes
      // is available only in PHP versions >= 5.1.2.
      // @see http://www.php.net/manual/en/function.hash-file.php
      if (version_compare(phpversion(), '5.1.2') >= 0 && !empty($display['settings']['generate_hash'])) {
        $hash = hash_file($display['settings']['hash_algo'], $item_uri);
        $rss_element['value'][] = array(
          'key' => 'media:hash',
          'value' => $hash,
          'attributes' => array(
            'algo' => strtr($display['settings']['hash_algo'], array('sha1' => 'sha-1')),
          ),
        );
      }

      // Other additional sub-elements.
      $extra_fields = array('title', 'description');
      foreach ($extra_fields as $extra_field) {
        $subelement = array();
        $field_extra_field = 'field_' . $extra_field;
        // Images could have 'title' and 'description' properties.
        if (isset($item[$extra_field]) && $item[$extra_field]) {
          $subelement = array(
            'key' => 'media:' . $extra_field,
            'value' => $item[$extra_field],
          );
        }
        // Other fieldable entities (think media) could have, well, fields.
        if (isset($item[$field_extra_field][LANGUAGE_NONE][0]['value']) && $item[$field_extra_field][LANGUAGE_NONE][0]['value']) {
          $subelement = array(
            'key' => 'media:' . $extra_field,
            'value' => $item[$field_extra_field][LANGUAGE_NONE][0]['value'],
          );
        }
        // Check whether original value format is text or HTML.
        if ($subelement) {
          $type = 'plain';
          $value_decoded = htmlspecialchars_decode($subelement['value'], ENT_QUOTES);
          if ($value_decoded != strip_tags($value_decoded)) {
            $type = 'html';
          }
          $subelement['attributes']['type'] = $type;
          $rss_element['value'][] = $subelement;
        }
      }

    }

    if ($display['type'] == 'media_thumbnail') {
      // Main RSS element.
      $rss_element = array(
        'key' => 'media:thumbnail',
        'attributes' => array(
          'url' => $url,
        ),
      );
      // Attributes for images: width and height.
      if ($item['type'] == 'image') {
        $media_info = image_load($item_uri);
        if (!empty($media_info)) {
          $item['info'] = $media_info;
          $rss_element['attributes']['width'] = $media_info->info['width'];
          $rss_element['attributes']['height'] = $media_info->info['height'];
        }
      }
      // Attribute: time.
      if (isset($item['field_time'][LANGUAGE_NONE][0]['value']) && $item['field_time'][LANGUAGE_NONE][0]['value']) {
        $rss_element['attributes']['time'] = $item['field_time'][LANGUAGE_NONE][0]['value'];
      }
    }

    if ($display['type'] == 'media_category') {
      // Main RSS element.
      $term = taxonomy_term_load($item['tid']);
      $rss_element = array(
        'key' => 'media:category',
        'value' => $term->name,
      );
      // Attributes: scheme and label.
      $attributes = array('scheme', 'label');
      foreach ($attributes as $attribute) {
        $field_name = 'field_' . $attribute;
        $field_items = field_get_items('taxonomy_term', $term, $field_name);
        if (!empty($field_items[0]['value'])) {
          $rss_element['attributes'][$attribute] = $field_items[0]['value'];
        }
      }
    }

    $element[$delta] = array(
      '#item' => $item,
      '#markup' => format_xml_elements(array($rss_element)),
      '#rss_element' => $rss_element,
      '#settings' => $display['settings'],
    );

  }

  return $element;
}
