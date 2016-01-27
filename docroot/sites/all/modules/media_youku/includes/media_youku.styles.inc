<?php

/**
 * @file media_youku/includes/media_youku.styles.inc
 * Styles definitions for Media: YouKu.
 */

/**
 * Implementation of Styles module hook_styles_register().
 */
function media_youku_styles_register() {
  return array(
    'MediaYouKuStyles' => array(
      'field_types' => 'file',
      'name' => t('MediaYouKu'),
      'description' => t('Media YouKu styles.'),
      'path' => drupal_get_path('module', 'media_youku') .'/includes',
      'file' => 'media_youku.styles.inc',
    ),
  );
}

/**
 *  Implements hook_styles_containers(). (Deprecated in version 2)
 */
function media_youku_styles_containers() {
  return array(
    'file' => array(
      'containers' => array(
        'media_youku' => array(
          'label' => t('YouKu Styles'),
          'data' => array(
            'streams' => array(
              'youku',
            ),
            'mimetypes' => array(
              'video/youku',
            ),
          ),
          'weight' => 0,
          'filter callback' => 'media_youku_formatter_filter',
          'themes' => array(
            'field_formatter_styles' => 'media_youku_field_formatter_styles',
            'styles' => 'media_youku_styles',
            'preview' => 'media_youku_preview_style',
          ),
          'description' => t('YouKu Styles will display embedded YouKu videos and thumbnails to your choosing, such as by resizing, setting colors, and autoplay. You can !manage.', array('!manage' => l(t('manage your YouKu styles here'), 'admin/config/media/media-youku-styles'))),
        ),
      ),
    ),
  );
}

function media_youku_formatter_filter($variables) {
  if (isset($variables['object'])) {
    $object = $variables['object'];
    return (file_uri_scheme($object->uri) == 'youku') && ($object->filemime == 'video/youku');
  }
}

/**
 * Implementation of the File Styles module's hook_file_styles_filter().
 */
function media_youku_file_styles_filter($object) {
  if ((file_uri_scheme($object->uri) == 'youku') && ($object->filemime == 'video/youku')) {
    return 'media_youku';
  }
}

/**
 *  Implements hook_styles_styles().
 */
function media_youku_styles_styles() {
  $styles = array(
    'file' => array(
      'containers' => array(
        'media_youku' => array(
          'styles' => array(
            'youku_thumbnail' => array(
              'name' => 'youku_thumbnail',
              'effects' => array(
                array('label' => t('Thumbnail'), 'name' => 'thumbnail', 'data' => array('thumbnail' => 1)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 100, 'height' => 75)),
              ),
            ),
            'youku_preview' => array(
              'name' => 'youku_preview',
              'effects' => array(
                array('label' => t('Autoplay'), 'name' => 'autoplay', 'data' => array('autoplay' => 1)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 220, 'height' => 165)),
              ),
            ),
            'youku_full' => array(
              'name' => 'youku_full',
              'effects' => array(
                array('label' => t('Autoplay'), 'name' => 'autoplay', 'data' => array('autoplay' => 0)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 640, 'height' => 480)),
                array('label' => t('Full screen'), 'name' => 'fullscreen', 'data' => array('fullscreen' => 1)),
              ),
            ),
          ),
        ),
      ),
    ),
  );

  // Allow any image style to be applied to the thumbnail.
  foreach (image_styles() as $style_name => $image_style) {
    $styles['file']['containers']['media_youku']['styles']['youku_thumbnail_' . $style_name] = array(
      'name' => 'youku_thumbnail_' . $style_name,
      'image_style' => $style_name,
      'effects' => array(
        array('label' => t('Thumbnail'), 'name' => 'thumbnail', 'data' => array('thumbnail' => 1)),
      ),
    );
  }

  return $styles;
}

/**
 *  Implements hook_styles_presets().
 */
function media_youku_styles_presets() {
  $presets = array(
    'file' => array(
      'square_thumbnail' => array(
        'media_youku' => array(
          'youku_thumbnail_square_thumbnail',
        ),
      ),
      'thumbnail' => array(
        'media_youku' => array(
          'youku_thumbnail',
        ),
      ),
      'small' => array(
        'media_youku' => array(
          'youku_preview',
        ),
      ),
      'large' => array(
        'media_youku' => array(
          'youku_full',
        ),
      ),
      'original' => array(
        'media_youku' => array(
          'youku_full',
        ),
      ),
    ),
  );
  return $presets;
}

/**
 * Implementation of Styles module hook_styles_default_containers().
 */
function media_youku_styles_default_containers() {
  // We append YouKu to the file containers.
  return array(
    'file' => array(
      'containers' => array(
        'media_youku' => array(
          'class' => 'MediaYouKuStyles',
          'name' => 'media_youku',
          'label' => t('YouKu'),
          'preview' => 'media_youku_preview_style',
        ),
      ),
    ),
  );
}


/**
 * Implementation of Styles module hook_styles_default_presets().
 */
function media_youku_styles_default_presets() {
  $presets = array(
    'file' => array(
      'containers' => array(
        'media_youku' => array(
          'default preset' => 'unlinked_thumbnail',
          'styles' => array(
            'original' => array(
              'default preset' => 'video',
            ),
            'thumbnail' => array(
              'default preset' => 'linked_thumbnail',
            ),
            'square_thumbnail' => array(
              'default preset' => 'linked_square_thumbnail',
            ),
            'medium' => array(
              'default preset' => 'linked_medium',
            ),
            'large' => array(
              'default preset' => 'large_video',
            ),
          ),
          'presets' => array(
            'video' => array(
              array(
                'name' => 'video',
                'settings' => array(),
              ),
            ),
            'large_video' => array(
              array(
                'name' => 'resize',
                'settings' => array(
                  'width' => 640,
                  'height' => 390,
                ),
              ),
              array(
                'name' => 'video',
                'settings' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
  );
  // Allow any image style to be applied to the thumbnail.
  foreach (image_styles() as $style_name => $image_style) {
    $presets['file']['containers']['media_youku']['presets']['linked_' . $style_name] = array(
      array(
        'name' => 'linkToMedia',
        'settings' => array(),
      ),
      array(
        'name' => 'imageStyle',
        'settings' => array(
          'image_style' => $style_name,
        ),
      ),
      array(
        'name' => 'thumbnail',
        'settings' => array(),
      ),
    );
    $presets['file']['containers']['media_youku']['presets']['unlinked_' . $style_name] = $presets['file']['containers']['media_youku']['presets']['linked_' . $style_name];
    array_shift($presets['file']['containers']['media_youku']['presets']['unlinked_' . $style_name]);
  }
  return $presets;
}
