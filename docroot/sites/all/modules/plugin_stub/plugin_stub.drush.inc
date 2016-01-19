<?php
/**
 * @file
 * Drush plugin for generating ctools plugins.
 */

/**
 * Implementation of hook_drush_help().
 */
function plugin_stub_drush_help($section) {
  switch ($section) {
    case 'drush:plugin-stub':
      return dt('Creates a plugin file with stub code in the module dir you are in when issuing the command. Currently supports content_types for Panels.');
  }
}

/**
 * Implements hook_drush_command().
 */
function plugin_stub_drush_command() {
  $items['plugin-stub'] = array(
    'callback' => 'plugin_stub_create_ctools_plugin',
    'description' => 'Create ctools plugins.',
    'arguments' => array(
      'type' => 'The type of ctools plugin. Mandatory.',
      'name' => 'The name of the ctools plugin to be created. Mandatory.',
    ),
    'required-arguments' => 2,
    'options' => array(
      'regions' => 'If you are generating a layout you can pass region names to this. Use comma separation. Eg. --regions="top,middle,bottom". Defaults to top, center, bottom.',
      'w' => 'Write implementations of hook_ctools_plugin_dierctory() and hook_ctools_plugin_api() to the module file.',
    ),
    'examples' => array(
      'plugin-stub content_type my_type' => 'Creates a content_type called my_type in the module\'s folder.',
    ),
    'aliases' => array('cps'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  return $items;
}

/**
 * Command callback. Validates the arguments and generates the stub.
 */
function plugin_stub_create_ctools_plugin() {
  $module = plugin_stub_get_module_or_theme_name();
  if (!$module) {
    return FALSE;
  }
  $arguments = func_get_args();
  if (empty($arguments[0])) {
    return drush_set_error(dt('No plugin type specified.'));
  }
  if (empty($arguments[1])) {
    return drush_set_error(dt('No name specified.'));
  }

  $plugin_name = $arguments[1];
  $plugin_type = $arguments[0];

  if (!in_array($plugin_type, array('content_type', 'layout'))) {
    return drush_set_error(dt('I do not know the plugin type you specified.'));
  }

  // Get the naming logic.
  $prefix = $module . '_' . $plugin_name;

  require_once 'plugins/' . $plugin_type . '.inc';

  $file_code = call_user_func('plugin_stub_generate_plugin_' . $plugin_type, $plugin_name, $prefix);
  $filename = plugin_stub_create_plugin_file_name($plugin_type, $plugin_name, TRUE);

  plugin_stub_write($filename, $file_code);
  if (drush_get_option('w')) {
    plugin_stub_write_include_in_module($module, $plugin_type, $plugin_name);
  }
  // Finalize.
  call_user_func('plugin_stub_finalize_' . $plugin_type, $plugin_name, $filename);
}

/**
 * Get the filename for the plugin and create the the plugins folder and
 * subfolder if necessary.
 *
 * @return
 *   Filename of the plugin to be generated.
 */
function plugin_stub_create_plugin_file_name($plugin_type, $plugin_name) {
  // Change to the current module dir.
  chdir(drush_cwd());
  if (!file_exists('plugins')) {
    drush_op('mkdir', 'plugins');
  }
  // The file folders for ctools plugins have the plugin type pluralized.
  $dir = 'plugins/'. $plugin_type .'s';
  if (!file_exists($dir)) {
    drush_op('mkdir', $dir);
  }
  // Content types go in their own folder in the plugins dir, so make that dir.
  $dir .= '/'. $plugin_name;
  if (!file_exists($dir)) {
   drush_op('mkdir', $dir);
  }
  return $dir .'/'. $plugin_name .'.inc';
}

/**
 * Finds the name of the module or theme in the folder the user is in - or
 * returns an error if the user is not in a module  or theme directory.
 *
 * @return string
 *   The name of the module or theme in the folder the user is in.
 */
function plugin_stub_get_module_or_theme_name() {
  $old = getcwd();
  chdir(drush_cwd());
  $modules = glob('*.info');
  chdir($old);
  $candidates = array();
  foreach ($modules as $name) {
    $candidates[] = basename($name, '.info');
  }

  if (sizeof($candidates) == 1) {
    return $candidates[0];
  }
  else {
    return drush_set_error(dt('You have to be in a module or theme folder to issue this command. Change directories to the module or theme folder you would like to be responsible for the plugin.'));
  }
}

/**
 * Writes file content to file. If the file already exists, the old one is
 * moved to a file with the same name - but a ~ appended.
 */
function plugin_stub_write($filename, $content) {
  file_put_contents($filename . '.new', $content);
  if (file_exists($filename)) {
    drush_op('rename', $filename, $filename . '~');
  }
  drush_op('rename', $filename . '.new', $filename);
}

/**
 * Writes the implementation of the ctools hooks hook_ctools_plugin_directory()
 * and hook_ctools_plugin_api() to the module file. It tries its best, but will
 * give up and give a message to the user if it is not sure where to place the
 * code.
 */
function plugin_stub_write_include_in_module($module, $plugin_type, $plugin_name) {
  $plugin_code = "  if ((\$module == 'ctools' || \$module == 'panels') && (\$plugin == '{$plugin_type}s')) {\n";
  if ('task' == $plugin_type) {
    $plugin_type = "  if (\$module == 'page_manager') {\n";
  }

  if (!function_exists($module .'_ctools_plugin_directory')) {
    $filename = drush_cwd() .'/'. $module .'.module';
    $code = file($filename);
    if ('?>' == trim(end($code))) {
      array_pop($code);
    }
    $code[] = "\n/**\n";
    $code[] = " * Implements hook_ctools_plugin_directory().\n";
    $code[] = " *\n";
    $code[] = " * Tells CTools (and thus Panels) where to look for plugin code.\n";
    $code[] = " */\n";
    $code[] = "function {$module}_ctools_plugin_directory(\$module, \$plugin) {\n";
    $code[] = $plugin_code;
    $code[] = "    return 'plugins/' . \$plugin;\n";
    $code[] = "  }\n";
    $code[] = "}\n";

    // Write the new function to the module.
    file_put_contents($filename, $code);
  }
  else {
    drush_print(t("It looks like the implementation of ctools hooks are already done, but if you have problems, make sure these lines of code line are added to the function\n!hook():\n", array('!hook' => $module .'_ctools_plugin_directory')));
    drush_print($plugin_code ." return 'plugins/' . \$plugin;\n}");
  }

  if (function_exists($module .'_ctools_plugin_api')) {
    $func = new ReflectionFunction($module .'_ctools_plugin_api');
    $filename = $func->getFileName();
    $code = file($filename);

    $exists = FALSE;
    $start = $func->getStartLine();
    $end = $func->getEndLine() - 2;
    for ($i = $start; $i < $end; $i++) {
      if (strpos($code[$i], '($module == \'page_manager\' && $api == \'pages_default\')')) {
        $exists = TRUE;
        break;
      }
    }
    if (!$exists) {
      $code[$end] .= "
  if (\$module == 'page_manager' && \$api == 'pages_default') {
    return array('version' => 1);
  }\n";
    file_put_contents($filename, $code);
    }
  }
  else {
    $filename = drush_cwd() .'/'. $module .'.module';
    $code = file($filename);
    if ('?' == trim(end($code))) {
      array_pop($code);
    }
    $code[] = "\n/**\n";
    $code[] = " * Implements ctools_plugin_api().\n";
    $code[] = " */\n";
    $code[] = "function {$module}_ctools_plugin_api() {\n";
    $code[] = "  \$args = func_get_args();\n";
    $code[] = "  \$module = array_shift(\$args);\n";
    $code[] = "  \$api = array_shift(\$args);\n";
    $code[] = "  if (\$module == 'page_manager' && \$api == 'pages_default') {\n";
    $code[] = "    return array('version' => 1);\n";
    $code[] = "  }\n";
    $code[] = "}\n";
    // Write the new function to the module.
    file_put_contents($filename, $code);
  }
}

/**
 * Copies a default icon to the plugin.
 *
 * @param string $plugin_name
 *   Name of the plugin.
 * @param string $filename
 *   Filename for the plugin.
 */
function plugin_stub_copy_icon($plugin_name, $filename) {
  $src_path = dirname(__FILE__);
  // Get the destination path.
  $pathinfo = pathinfo($filename);
  $dst = drush_cwd() . '/' . $pathinfo['dirname'] . '/ctools_stub.png';
  drush_op('copy', $src_path . '/ctools_stub.png', $dst);
}
