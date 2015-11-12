<?php

/**
 * @file
 * Helper functions for Drush Entity Scaffold module.
 */

/**
 * Returns a list containing the available tokens.
 *
 * @return array
 *   Vector of tokens.
 */
function entity_scaffold_get_tokens() {
  return array(
    '{module_extension}',
    '{module_name}',
    '{entity_label}',
    '{entity_label_lowercase}',
    '{entity_class}',
  );
}

/**
 * Returns a list containing the model file names.
 *
 * @return array
 *   List of model files.
 */
function entity_scaffold_get_model_files() {
  $files = drush_scan_directory(rtrim(ENTITY_SCAFFOLD_MODEL_MODULE_PATH, '/'), '/.*\\' . ENTITY_SCAFFOLD_MODEL_FILE_EXTENSION . '$/');

  // Adds relative path information to result.
  foreach ($files as $file_key => $file) {
    $files[$file_key]->relative_path = str_replace(ENTITY_SCAFFOLD_MODEL_MODULE_PATH, '', $file->filename);
  }

  return $files;
}

/**
 * Replaces the tokens inside the given file content.
 *
 * @param object $file_uri
 *   The file uri to be processed.
 *
 * @param string $module_name
 *   Name of the module where the Entity will reside.
 *
 * @return string
 *   Returns the generated file contents after replacing tokens.
 */
function entity_scaffold_file_get_replaced_content($file_uri, $module_name) {
  $model_file_contents = file_get_contents($file_uri);

  return entity_scaffold_string_replace($model_file_contents, $module_name);
}

/**
 * Returns an array containing the token replaced values.
 *
 * @param string $module_name
 *   Name of the module where the Entity will reside.
 *
 * @return array
 *   An array containing the token replaced values.
 */
function entity_scaffold_get_replaced_token_values($module_name) {
  // Normalizes entity label and class string.
  $entity_name = explode('_', $module_name);
  foreach ($entity_name as $key => $name) {
    $entity_name[$key] = ucfirst(strtolower($name));
  }

  $entity_label = implode(" ", $entity_name);
  $entity_label_lowercase = strtolower($entity_label);
  $entity_class = implode("", $entity_name);

  // Replacements values, using the same token list order.
  $replacements = array(
    ENTITY_SCAFFOLD_MODULE_EXTENSION,
    $module_name,
    $entity_label,
    $entity_label_lowercase,
    $entity_class,
  );

  return $replacements;
}

/**
 * Replaces the given value against the scaffold tokens.
 *
 * @param string $string
 *   The related string to be replaced against token values.
 *
 * @param string $module_name
 *   Name of the module where the Entity will reside.
 *
 * @return string
 *   Returns the processed string.
 */
function entity_scaffold_string_replace($string, $module_name) {
  $tokens = entity_scaffold_get_tokens();
  $replacements = entity_scaffold_get_replaced_token_values($module_name);

  return str_replace($tokens, $replacements, $string);
}

/**
 * Save the given file contents to the given module destination path.
 *
 * @param string $file_contents
 *   Contents of the generated file.
 *
 * @param string $path
 *   File path to be used when saving the file.
 *
 * @return mixed
 *   Return follow the file_put_contents() definition.
 */
function entity_scaffold_file_put_contents($file_contents, $path) {
  $file_parts = explode('/', $path);
  $file_name = array_pop($file_parts);
  $file_path = reset($file_parts);
  unset($file_parts[0]);

  foreach ($file_parts as $part) {
    $file_path .= "/$part";

    if (!is_dir($file_path)) {
      drush_mkdir($file_path);
    }
  }

  return file_put_contents("$file_path/$file_name", $file_contents);
}

/**
 * Returns the path of the new module.
 *
 * @param string $module_name
 *   The given module name that will be generated.
 *
 * @param string $module_destination
 *   The path where the new module will be generated.
 *
 * @return string
 *   The new module path.
 */
function entity_scaffold_get_new_module_path($module_name, $module_destination) {
  return rtrim($module_destination, '/') . '/' . $module_name;
}

/**
 * Returns the new file path related to the new module.
 *
 * @param string $module_name
 *   The new module name.
 *
 * @param string $module_destination
 *   The new module destination path.
 *
 * @param string $relative_file_uri
 *   The given file relative path.
 *
 * @return string
 *   Returns the new replaced file path.
 */
function entity_scaffold_get_new_file_path($module_name, $module_destination, $relative_file_uri) {
  $file_path = entity_scaffold_string_replace($module_destination . '/' . $relative_file_uri, $module_name);

  return str_replace(ENTITY_SCAFFOLD_MODEL_FILE_EXTENSION, '', $file_path);
}

/**
 * Helper function to validate a Drupal module_name.
 *
 * @param string $module_name
 *   The given module name to be validated.
 *
 * @return bool
 *   TRUE on case of success, FALSE otherwise.
 */
function entity_scaffold_is_valid_module_name($module_name) {
  $success = TRUE;

  if (preg_match('/[^a-z0-9_]/', $module_name)) {
    $success = FALSE;
  }

  return $success;
}

/**
 * Processing all of the model files according to user given information.
 *
 * @param string $module_name
 *   Name of the module where the Entity will reside.
 *
 * @param string $module_destination
 *   Place where the module will be saved.
 *
 * @return mixed
 *   Echoes the SUCCESS or FAILURE of saving the files.
 */
function entity_scaffold_generate($module_name, $module_destination) {
  $new_module_path = entity_scaffold_get_new_module_path($module_name, $module_destination);

  if (!drush_mkdir($new_module_path)) {
    drush_log(dt('Unable to create the new module on !path', array('!path' => $new_module_path)), ENTITY_SCAFFOLD_LOG_ERROR);
  }

  // Processing all model files.
  $model_files = entity_scaffold_get_model_files();
  foreach ($model_files as $file) {
    $file_contents = entity_scaffold_file_get_replaced_content($file->filename, $module_name);
    $file_path = entity_scaffold_get_new_file_path($module_name, $new_module_path, $file->relative_path);

    if (entity_scaffold_file_put_contents($file_contents, $file_path) === FALSE) {
      drush_log(dt('File !path could NOT be created.', array('!path' => $file_path)), ENTITY_SCAFFOLD_LOG_ERROR);
    }

    drush_log(dt('File !path created.', array('!path' => $file_path)), ENTITY_SCAFFOLD_LOG_SUCCESS);
  }
}
