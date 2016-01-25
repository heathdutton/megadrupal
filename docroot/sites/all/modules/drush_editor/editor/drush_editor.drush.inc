<?php

/**
 * @file
 * Drush integration for WYSIWYG editors.
 */

$current_path = dirname(__FILE__);

include_once $current_path . '/lib/DrushEditorInterface.php';
include_once $current_path . '/lib/DrushEditorBase.php';
include_once $current_path . '/lib/DrushEditorManager.php';

/**
 * Implements COMMANDFILE_drush_command().
 */
function drush_editor_drush_command() {
  $current_path = dirname(__FILE__);

  $items['editor-download'] = array(
    'commandfile' => $current_path . DIRECTORY_SEPARATOR . 'download.editor.inc',
    'description' => dt('Downloads popular WYSIWYG editors from their servers or other sources.'),
    'examples'    => array(
      'drush eddl ckeditor'                => 'Download latest full package release of CKEditor (non-beta).',
      'drush eddl ckeditor-standard'       => 'Download latest standard package release of CKEditor (non-beta).',
      'drush eddl ckeditor-4'              => 'Download latest full package release of CKEditor 4 (non-beta).',
      'drush eddl ckeditor-4-standard'     => 'Download latest standard package release of CKEditor 4.',
      'drush eddl ckeditor-4.1.2'          => 'Download full package of a specfic version of CKEditor.',
      'drush eddl ckeditor-4.1.2-standard' => 'Download standard package of a specfic version of CKEditor.',
      'drush eddl ckeditor-4.x'            => 'Download latest 4.x version of CKEditor (including betas).',
      'drush eddl ckeditor tinymce'        => 'Download latest versions of CKEditor and TinyMCE editors.',
      'drush eddl ckeditor --select'       => 'Show a list of recent releases of the CKEditor project, prompt for which one to download.',
    ),
    'arguments' => array(
      'editors' => "A space delimited list of editor names, with optional version. Defaults to 'ckeditor'",
    ),
    'options' => array(
      'destination' => array(
        'description'   => "Path to which the editor will be copied. If you're providing a relative path, note it is relative to the drupal root (if bootstrapped).",
        'example-value' => 'path',
      ),
      'use-site-dir' => 'Force to use the site specific directory. It will create the directory if it doesn\'t exist. If --destination is also present this option will be ignored.',
      'select'       => "Select the version to download interactively from a list of available releases.",
      'cache'        => 'Cache the downloaded file.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_MAX,
    'aliases' => array('eddl'),
    'engines' => array(
      'editor_package_handler',
    ),
  );

  return $items;
}

/**
 * Implementation of COMMANDFILE_drush_engine_type_info().
 */
function drush_editor_drush_engine_type_info() {
  return array(
    'editor_package_handler' => array(
      'option'      => 'package-handler',
      'description' => 'Determine how to fetch editor from their servers or other sources.',
      'default'     => 'wget',
      'options'     => array(),
    ),
  );
}

/**
 * Implements COMMANDFILE_drush_engine_ENGINE_TYPE().
 */
function drush_editor_drush_engine_editor_package_handler() {
  return array(
    'wget' => array(
      'description' => 'Download editors packages using wget or curl.',
      'options' => array(
        // 'cache' => 'Cache the downloaded file.',
        // TODO: Version 7.x-1.2-alpha1.
        // 'build' => "Build your own editor with latest release.",
      ),
    ),
    // TODO: Version 7.x-1.1-alpha1.
    // 'git' => array(
    //   'description' => 'Use git to checkout and update editors.',
    //   'options' => array(
    //     'gitsubmodule'      => 'Use git submodules for checking out new editors. Existing git checkouts are unaffected, and will continue to (not) use submodules regardless of this setting.',
    //     'no-source-control' => 'Delete source control folder (.git) after the download is finished.',
    //   ),
    //   'sub-options' => array(
    //     'gitsubmodule' => array(
    //       'gitsubmoduleaddparams' => 'Add options to the `git submodule add` command.',
    //     ),
    //   ),
    // ),
    // 'svn' => array(
    //   'description' => 'Use svn to checkout and update editors.',
    //   'options'     => array(
    //     'no-source-control' => 'Delete source control folder (.svn) after the download is finished.',
    //   ),
    // ),
  );
}

/**
 * Implements hook_drush_editor().
 */
function drush_editor_drush_editor() {
  $path = dirname(__FILE__);

  $editors['ckeditor'] = array(
    'class'  => 'DrushEditorCKEditor',
    'file'   => $path  . '/lib/editor/ckeditor/DrushEditorCKEditor.php',
    'preset' => array('basic', 'standard', 'full'),
  );

  $editors['tinymce'] = array(
    'class'  => 'DrushEditorTinyMCE',
    'file'   => $path  . '/lib/editor/tinymce/DrushEditorTinyMCE.php',
    'preset' => array('jquery'),
  );

  return $editors;
}

/**
 * Sanitize user provided arguments to several pm commands.
 *
 * Return an array of arguments off a space and/or comma separated values.
 *
 * @param array $args
 *   Arguments.
 *
 * @param bool $dashes_to_underscores
 *   Convert dash to underscore.
 *
 * @return array
 *   Parsed arguments.
 */
function drush_editor_parse_arguments($args, $dashes_to_underscores = TRUE) {
  $arguments = _convert_csv_to_array($args);

  foreach ($arguments as $key => $argument) {
    $argument = ($dashes_to_underscores) ? strtr($argument, '-', '_') : $argument;
  }

  return $arguments;
}

/**
 * Parse out the project name and version and return as a structured array.
 *
 * @param array $requests
 *   List of project names.
 *
 * @return array
 *   List of requested editors.
 */
function drush_editor_parse_version($requests) {
  $requestdata      = array();
  $drupal_bootstrap = drush_get_context('DRUSH_BOOTSTRAP_PHASE') > 0;

  foreach ($requests as $request) {
    $editor        = drupal_strtolower($request);
    $major_version = NULL;
    $version       = NULL;
    $preset        = NULL;
    $pattern       = '/(?:-((\d+)((?:(?:\.\d+){1,})|(?:\.x))*))?(?:-([a-z0-9_-]+))?$/';

    // Detect ckeditor-4.x or ckeditor-4.1.2.
    preg_match($pattern, $editor, $matches);

    $editor = trim(preg_replace($pattern, '', $editor));

    if (!empty($matches[1])) {
      // The project is whatever we have prior to the version part of the
      // request.
      $major_version = $matches[2] | 0;

      // Becareful with 3.0 and 3, we should use ===.
      if ($matches[1] === $matches[2]) {
        $version = 'head';
      }
      elseif ($matches[3] == '.x') {
        $version = 'dev';
      }
      else {
        $version = $matches[1];
      }
    }

    if (!empty($matches[4])) {
      $preset = $matches[4];
    }

    $requestdata[$editor] = array(
      'name'          => $editor,
      'major_version' => $major_version,
      'version'       => $version,
      'preset'        => $preset,
    );
  }

  return $requestdata;
}
