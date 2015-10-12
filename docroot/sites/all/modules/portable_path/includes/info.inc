<?php
/**
 * @file
 * Provides info-type hook implementations that are infrequently called.
 *
 * @author Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Implements hook_help().
 */
function _portable_path_help($path, $arg) {
  switch ($path) {
    case 'admin/help#portable_path':
      $output = '<h3>' . t('About') . '</h3>';
      $module_settings = 'the ' . l('module settings', 'admin/config/content/portable') . ' page';
      break;

    case 'admin/config/content/portable':
      $output = '';
      $module_settings = 'this page';
      break;

    default:
      return;
  }

  $output .= '<p>' . t('The Portable path module defines input/output filters to store/restore paths using either stream wrapper notation or a managed file ID token.
    When content is saved, the output filter will store URLs in text fields in a portable fashion.
    When content is rendered, the input filter will replace the stream wrapper URL or file ID token with an absolute or relative URL.<br></br>
    On !module-settings, specify the stream_wrappers and fields to apply the input/output filters to.
    Visit the !format-settings page to enable and configure the filters provided by this module.',
    array('!module-settings' => $module_settings, '!format-settings' => l('text formats', 'admin/config/content/formats'))) . '</p>';
  return $output;
}

/**
 * Implements hook_menu().
 */
function _portable_path_menu() {
  $items['admin/config/content/portable'] = array(
    'title' => 'Portable path',
    'description' => 'Configure which fields and paths are converted to portable paths before saving an entity.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('portable_path_settings_form'),
    'access arguments' => array('administer filters'),
    'file' => 'includes/settings.inc',
  );
  return $items;
}

/**
 * Implements hook_filter_info().
 *
 * Added custom key of "encode callback."
 */
function _portable_path_filter_info() {
  $filters['file_id'] = array(
    'title' => t('Convert file ID tokens to URLs'),
    'description' => t('Replaces a token in file ID format (i.e., "[portable:fid:style:query]") with the full absolute or relative URL.'),
    'process callback'  => 'portable_path_file_id_process',
    'settings callback' => 'portable_path_file_id_settings',
    'tips callback' => 'portable_path_file_id_tips',
    'encode callback'  => 'portable_path_file_id_encode',
    'default settings' => array('absolute_path' => 0),
    'weight' => -30,
  );
  $filters['stream_wrapper'] = array(
    'title' => t('Convert stream wrapper URLs to URLs'),
    'description' => t('Replaces a URL in a local stream wrapper format (e.g. "public://") with the full absolute or relative URL.'),
    'process callback'  => 'portable_path_stream_wrapper_process',
    'settings callback' => 'portable_path_stream_wrapper_settings',
    'tips callback' => 'portable_path_stream_wrapper_tips',
    'encode callback'  => 'portable_path_stream_wrapper_encode',
    'default settings' => array('absolute_path' => 0),
    'weight' => -29,
  );

  if (!module_exists('remote_content')) {
    // @todo If remote_content module is not enabled on remote site, then these
    // filters will not be invoked on local site (due to query run in
    // filter_list_format() which will use the remote connection).
    // If we ignore this condition, then the filters can be enabled on remote.
//     return $filters;
  }

  $filters['remote_file_id'] = array(
    'title' => t('Convert remote file ID tokens to absolute URLs'),
    'description' => t('Replaces a remote token in file ID format (i.e., "[portable:fid:style:query]") with the absolute URL.'),
    'process callback'  => 'portable_path_remote_file_id_process',
    'settings callback' => 'portable_path_remote_file_id_settings',
    'tips callback' => 'portable_path_remote_file_id_tips',
    'encode callback'  => 'portable_path_remote_file_id_encode',
    'default settings' => array(),
    'weight' => -40,
  );
  $filters['remote_stream_wrapper'] = array(
    'title' => t('Convert remote stream wrapper URLs to absolute URLs'),
    'description' => t('Replaces a remote URL in a local stream wrapper format (e.g. "public://") with the absolute URL.'),
    'process callback'  => 'portable_path_remote_stream_wrapper_process',
    'settings callback' => 'portable_path_remote_stream_wrapper_settings',
    'tips callback' => 'portable_path_remote_stream_wrapper_tips',
    'encode callback'  => 'portable_path_remote_stream_wrapper_encode',
    'default settings' => array(
      'stream_wrappers' => array(),
    ),
    'weight' => -39,
  );
  return $filters;
}
