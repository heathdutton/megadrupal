<?php
/**
 * @file
 * varbase_editor.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function varbase_editor_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ace_editor_autowrap';
  $strongarm->value = 1;
  $export['ace_editor_autowrap'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ace_editor_default_syntax';
  $strongarm->value = 'html';
  $export['ace_editor_default_syntax'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ace_editor_filter_formats';
  $strongarm->value = array(
    'html_editor' => 'html_editor',
  );
  $export['ace_editor_filter_formats'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ace_editor_fontsize';
  $strongarm->value = '9pt';
  $export['ace_editor_fontsize'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ace_editor_linehighlighting';
  $strongarm->value = 1;
  $export['ace_editor_linehighlighting'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ace_editor_printmargin';
  $strongarm->value = 1;
  $export['ace_editor_printmargin'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ace_editor_theme';
  $strongarm->value = 'clouds';
  $export['ace_editor_theme'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_file__image';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'filename' => array(
          'weight' => '0',
        ),
        'preview' => array(
          'weight' => '1',
        ),
        'redirect' => array(
          'weight' => '5',
        ),
      ),
      'display' => array(
        'file' => array(
          'wysiwyg' => array(
            'weight' => '0',
            'visible' => TRUE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_file__image'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_backgroundcolor';
  $strongarm->value = '#000000';
  $export['media_backgroundcolor'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_dialogclass';
  $strongarm->value = 'media-wrapper';
  $export['media_dialogclass'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_draggable';
  $strongarm->value = '1';
  $export['media_draggable'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_height';
  $strongarm->value = '280';
  $export['media_height'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_minwidth';
  $strongarm->value = '500';
  $export['media_minwidth'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_modal';
  $strongarm->value = '1';
  $export['media_modal'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_opacity';
  $strongarm->value = '0.4';
  $export['media_opacity'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_position';
  $strongarm->value = 'center';
  $export['media_position'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_resizable';
  $strongarm->value = '0';
  $export['media_resizable'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_width';
  $strongarm->value = '670';
  $export['media_width'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_default_render';
  $strongarm->value = 'file_entity';
  $export['media_wysiwyg_default_render'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_audio_file_wysiwyg_view_mode';
  $strongarm->value = 'wysiwyg';
  $export['media_wysiwyg_view_mode_audio_file_wysiwyg_view_mode'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_audio_file_wysiwyg_view_mode_status';
  $strongarm->value = 0;
  $export['media_wysiwyg_view_mode_audio_file_wysiwyg_view_mode_status'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_audio_wysiwyg_restricted_view_modes';
  $strongarm->value = array(
    'wysiwyg' => 'wysiwyg',
    'default' => 0,
    'teaser' => 0,
    'full' => 0,
    'preview' => 0,
    'rss' => 0,
    'token' => 0,
  );
  $export['media_wysiwyg_view_mode_audio_wysiwyg_restricted_view_modes'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_audio_wysiwyg_restricted_view_modes_status';
  $strongarm->value = 1;
  $export['media_wysiwyg_view_mode_audio_wysiwyg_restricted_view_modes_status'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_document_file_wysiwyg_view_mode';
  $strongarm->value = 'wysiwyg';
  $export['media_wysiwyg_view_mode_document_file_wysiwyg_view_mode'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_document_file_wysiwyg_view_mode_status';
  $strongarm->value = 0;
  $export['media_wysiwyg_view_mode_document_file_wysiwyg_view_mode_status'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_document_wysiwyg_restricted_view_modes';
  $strongarm->value = array(
    'wysiwyg' => 'wysiwyg',
    'default' => 0,
    'teaser' => 0,
    'full' => 0,
    'preview' => 0,
    'rss' => 0,
    'token' => 0,
  );
  $export['media_wysiwyg_view_mode_document_wysiwyg_restricted_view_modes'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_document_wysiwyg_restricted_view_modes_status';
  $strongarm->value = 1;
  $export['media_wysiwyg_view_mode_document_wysiwyg_restricted_view_modes_status'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_image_file_wysiwyg_view_mode';
  $strongarm->value = 'wysiwyg';
  $export['media_wysiwyg_view_mode_image_file_wysiwyg_view_mode'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_image_file_wysiwyg_view_mode_status';
  $strongarm->value = 1;
  $export['media_wysiwyg_view_mode_image_file_wysiwyg_view_mode_status'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_image_wysiwyg_restricted_view_modes';
  $strongarm->value = array(
    'default' => 'default',
    'teaser' => 'teaser',
    'preview' => 'preview',
  );
  $export['media_wysiwyg_view_mode_image_wysiwyg_restricted_view_modes'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_image_wysiwyg_restricted_view_modes_status';
  $strongarm->value = 1;
  $export['media_wysiwyg_view_mode_image_wysiwyg_restricted_view_modes_status'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_video_file_wysiwyg_view_mode';
  $strongarm->value = 'wysiwyg';
  $export['media_wysiwyg_view_mode_video_file_wysiwyg_view_mode'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_video_file_wysiwyg_view_mode_status';
  $strongarm->value = 0;
  $export['media_wysiwyg_view_mode_video_file_wysiwyg_view_mode_status'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_video_wysiwyg_restricted_view_modes';
  $strongarm->value = array(
    'wysiwyg' => 'wysiwyg',
    'default' => 0,
    'teaser' => 0,
    'full' => 0,
    'preview' => 0,
    'rss' => 0,
    'token' => 0,
  );
  $export['media_wysiwyg_view_mode_video_wysiwyg_restricted_view_modes'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_wysiwyg_view_mode_video_wysiwyg_restricted_view_modes_status';
  $strongarm->value = 1;
  $export['media_wysiwyg_view_mode_video_wysiwyg_restricted_view_modes_status'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_zindex';
  $strongarm->value = '10005';
  $export['media_zindex'] = $strongarm;

  return $export;
}
