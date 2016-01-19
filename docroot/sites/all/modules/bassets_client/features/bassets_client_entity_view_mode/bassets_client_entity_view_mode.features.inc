<?php
/**
 * @file
 * bassets_client_entity_view_mode.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function bassets_client_entity_view_mode_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "file_entity" && $api == "file_default_displays") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function bassets_client_entity_view_mode_image_default_styles() {
  $styles = array();

  // Exported image style: large_copyright.
  $styles['large_copyright'] = array(
    'label' => 'Large with copyright (480x480)',
    'effects' => array(
      21 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 480,
          'height' => 480,
          'upscale' => 0,
        ),
        'weight' => -10,
      ),
      22 => array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '#fff',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '',
            'height' => '',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => 40,
          ),
        ),
        'weight' => -9,
      ),
      18 => array(
        'name' => 'image_effects_text',
        'data' => array(
          'size' => 10,
          'xpos' => 'left+10',
          'ypos' => 'bottom-24',
          'halign' => 'left',
          'valign' => 'bottom',
          'RGB' => array(
            'HEX' => '#000000',
          ),
          'alpha' => 100,
          'angle' => 0,
          'fontfile' => 'module://imagecache_actions/image_effects_text/Komika_display.ttf',
          'text_source' => 'text',
          'text' => '[file:field_asset_caption]',
          'php' => '',
        ),
        'weight' => -8,
      ),
      19 => array(
        'name' => 'image_effects_text',
        'data' => array(
          'size' => 10,
          'xpos' => 'left+10',
          'ypos' => 'bottom-12',
          'halign' => 'left',
          'valign' => 'bottom',
          'RGB' => array(
            'HEX' => '#000000',
          ),
          'alpha' => 100,
          'angle' => 0,
          'fontfile' => 'module://imagecache_actions/image_effects_text/Komika_display.ttf',
          'text_source' => 'text',
          'text' => '[file:field_asset_license]',
          'php' => '',
        ),
        'weight' => -7,
      ),
      20 => array(
        'name' => 'image_effects_text',
        'data' => array(
          'size' => 10,
          'xpos' => 'left+10',
          'ypos' => 'bottom',
          'halign' => 'left',
          'valign' => 'bottom',
          'RGB' => array(
            'HEX' => '#000000',
          ),
          'alpha' => 100,
          'angle' => 0,
          'fontfile' => 'module://imagecache_actions/image_effects_text/Komika_display.ttf',
          'text_source' => 'text',
          'text' => '[file:field-asset-license-url:url]',
          'php' => '',
        ),
        'weight' => -6,
      ),
    ),
  );

  // Exported image style: medium_copyright.
  $styles['medium_copyright'] = array(
    'label' => 'Medium with copyright (220x220)',
    'effects' => array(
      26 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 220,
          'height' => 220,
          'upscale' => 1,
        ),
        'weight' => -10,
      ),
      27 => array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '#fff',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '',
            'height' => '',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => 40,
          ),
        ),
        'weight' => -9,
      ),
      23 => array(
        'name' => 'image_effects_text',
        'data' => array(
          'size' => 6,
          'xpos' => 'left+10',
          'ypos' => 'bottom-24',
          'halign' => 'left',
          'valign' => 'bottom',
          'RGB' => array(
            'HEX' => '#000000',
          ),
          'alpha' => 100,
          'angle' => 0,
          'fontfile' => 'module://imagecache_actions/image_effects_text/Komika_display.ttf',
          'text_source' => 'text',
          'text' => '[file:field_asset_caption]',
          'php' => '',
        ),
        'weight' => -8,
      ),
      24 => array(
        'name' => 'image_effects_text',
        'data' => array(
          'size' => 6,
          'xpos' => 'left+10',
          'ypos' => 'bottom-12',
          'halign' => 'left',
          'valign' => 'bottom',
          'RGB' => array(
            'HEX' => '#000000',
          ),
          'alpha' => 100,
          'angle' => 0,
          'fontfile' => 'module://imagecache_actions/image_effects_text/Komika_display.ttf',
          'text_source' => 'text',
          'text' => '[file:field_asset_license]',
          'php' => '',
        ),
        'weight' => -7,
      ),
      25 => array(
        'name' => 'image_effects_text',
        'data' => array(
          'size' => 6,
          'xpos' => 'left+10',
          'ypos' => 'bottom',
          'halign' => 'left',
          'valign' => 'bottom',
          'RGB' => array(
            'HEX' => '#000000',
          ),
          'alpha' => 100,
          'angle' => 0,
          'fontfile' => 'module://imagecache_actions/image_effects_text/Komika_display.ttf',
          'text_source' => 'text',
          'text' => '[file:field-asset-license-url:url]',
          'php' => '',
        ),
        'weight' => -6,
      ),
    ),
  );

  // Exported image style: thumbnail_copyright.
  $styles['thumbnail_copyright'] = array(
    'label' => 'Thumbnail with copyright (100x100)',
    'effects' => array(
      31 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 100,
          'height' => 100,
          'upscale' => 1,
        ),
        'weight' => -10,
      ),
      32 => array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '#fff',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '',
            'height' => '',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => 40,
          ),
        ),
        'weight' => -9,
      ),
      28 => array(
        'name' => 'image_effects_text',
        'data' => array(
          'size' => 4,
          'xpos' => 'left+10',
          'ypos' => 'bottom-24',
          'halign' => 'left',
          'valign' => 'bottom',
          'RGB' => array(
            'HEX' => '#000000',
          ),
          'alpha' => 100,
          'angle' => 0,
          'fontfile' => 'module://imagecache_actions/image_effects_text/Komika_display.ttf',
          'text_source' => 'text',
          'text' => '[file:field_asset_caption]',
          'php' => '',
        ),
        'weight' => -8,
      ),
      29 => array(
        'name' => 'image_effects_text',
        'data' => array(
          'size' => 4,
          'xpos' => 'left+10',
          'ypos' => 'bottom-12',
          'halign' => 'left',
          'valign' => 'bottom',
          'RGB' => array(
            'HEX' => '#000000',
          ),
          'alpha' => 100,
          'angle' => 0,
          'fontfile' => 'module://imagecache_actions/image_effects_text/Komika_display.ttf',
          'text_source' => 'text',
          'text' => '[file:field_asset_license]',
          'php' => '',
        ),
        'weight' => -7,
      ),
      30 => array(
        'name' => 'image_effects_text',
        'data' => array(
          'size' => 4,
          'xpos' => 'left+10',
          'ypos' => 'bottom',
          'halign' => 'left',
          'valign' => 'bottom',
          'RGB' => array(
            'HEX' => '#000000',
          ),
          'alpha' => 100,
          'angle' => 0,
          'fontfile' => 'module://imagecache_actions/image_effects_text/Komika_display.ttf',
          'text_source' => 'text',
          'text' => '[file:field-asset-license-url:url]',
          'php' => '',
        ),
        'weight' => -6,
      ),
    ),
  );

  return $styles;
}
