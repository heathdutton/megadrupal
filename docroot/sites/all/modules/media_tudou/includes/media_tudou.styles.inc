<?php

/**
 * @file media_tudou/includes/media_tudou.styles.inc
 * Styles definitions for Media: TuDou.
 */

/**
 * Implementation of Styles module hook_styles_register().
 */
function media_tudou_styles_register() {
  return array(
    'MediaTuDouStyles' => array(
      'field_types' => 'file',
      'name' => t('MediaTuDou'),
      'description' => t('Media TuDou styles.'),
      'path' => drupal_get_path('module', 'media_tudou') .'/includes',
      'file' => 'media_tudou.styles.inc',
    ),
  );
}

/**
 *  Implements hook_styles_containers(). (Deprecated in version 2)
 */
function media_tudou_styles_containers() {
  return array(
    'file' => array(
      'containers' => array(
        'media_tudou' => array(
          'label' => t('TuDou Styles'),
          'data' => array(
            'streams' => array(
              'tudou',
            ),
            'mimetypes' => array(
              'video/tudou',
            ),
          ),
          'weight' => 0,
          'filter callback' => 'media_tudou_formatter_filter',
          'themes' => array(
            'field_formatter_styles' => 'media_tudou_field_formatter_styles',
            'styles' => 'media_tudou_styles',
            'preview' => 'media_tudou_preview_style',
          ),
          'description' => t('TuDou Styles will display embedded TuDou videos and thumbnails to your choosing, such as by resizing, setting colors, and autoplay. You can !manage.', array('!manage' => l(t('manage your TuDou styles here'), 'admin/config/media/media-tudou-styles'))),
        ),
      ),
    ),
  );
}

function media_tudou_formatter_filter($variables) {
  if (isset($variables['object'])) {
    $object = $variables['object'];
    return (file_uri_scheme($object->uri) == 'tudou') && ($object->filemime == 'video/tudou');
  }
}

/**
 * Implementation of the File Styles module's hook_file_styles_filter().
 */
function media_tudou_file_styles_filter($object) {
  if ((file_uri_scheme($object->uri) == 'tudou') && ($object->filemime == 'video/tudou')) {
    return 'media_tudou';
  }
}

/**
 *  Implements hook_styles_styles().
 */
function media_tudou_styles_styles() {
  $styles = array(
    'file' => array(
      'containers' => array(
        'media_tudou' => array(
          'styles' => array(
            'tudou_thumbnail' => array(
              'name' => 'tudou_thumbnail',
              'effects' => array(
                array('label' => t('Thumbnail'), 'name' => 'thumbnail', 'data' => array('thumbnail' => 1)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 100, 'height' => 75)),
              ),
            ),
            'tudou_preview' => array(
              'name' => 'tudou_preview',
              'effects' => array(
                array('label' => t('Autoplay'), 'name' => 'autoplay', 'data' => array('autoplay' => 1)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 220, 'height' => 165)),
              ),
            ),
            'tudou_full' => array(
              'name' => 'tudou_full',
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
    $styles['file']['containers']['media_tudou']['styles']['tudou_thumbnail_' . $style_name] = array(
      'name' => 'tudou_thumbnail_' . $style_name,
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
function media_tudou_styles_presets() {
  $presets = array(
    'file' => array(
      'square_thumbnail' => array(
        'media_tudou' => array(
          'tudou_thumbnail_square_thumbnail',
        ),
      ),
      'thumbnail' => array(
        'media_tudou' => array(
          'tudou_thumbnail',
        ),
      ),
      'small' => array(
        'media_tudou' => array(
          'tudou_preview',
        ),
      ),
      'large' => array(
        'media_tudou' => array(
          'tudou_full',
        ),
      ),
      'original' => array(
        'media_tudou' => array(
          'tudou_full',
        ),
      ),
    ),
  );
  return $presets;
}

/**
 * Implementation of Styles module hook_styles_default_containers().
 */
function media_tudou_styles_default_containers() {
  // We append TuDou to the file containers.
  return array(
    'file' => array(
      'containers' => array(
        'media_tudou' => array(
          'class' => 'MediaTuDouStyles',
          'name' => 'media_tudou',
          'label' => t('TuDou'),
          'preview' => 'media_tudou_preview_style',
        ),
      ),
    ),
  );
}


/**
 * Implementation of Styles module hook_styles_default_presets().
 */
function media_tudou_styles_default_presets() {
  $presets = array(
    'file' => array(
      'containers' => array(
        'media_tudou' => array(
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
    $presets['file']['containers']['media_tudou']['presets']['linked_' . $style_name] = array(
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
    $presets['file']['containers']['media_tudou']['presets']['unlinked_' . $style_name] = $presets['file']['containers']['media_tudou']['presets']['linked_' . $style_name];
    array_shift($presets['file']['containers']['media_tudou']['presets']['unlinked_' . $style_name]);
  }
  return $presets;
}
