<?php

/**
 * @file
 * Smartling log.
 */

namespace Drupal\smartling\Wrappers;

/**
 * Class SmartlingUtils.
 */
class SmartlingUtils {
  protected $entity_api_wrapper;
  protected $field_api_wrapper;

  public function __construct($entity_api_wrapper, $field_api_wrapper) {
    $this->entity_api_wrapper = $entity_api_wrapper;
    $this->field_api_wrapper = $field_api_wrapper;
  }

  /**
   * Checks node method.
   *
   * @param string $bundle
   *   Node content type.
   *
   * @return bool
   *   Return TRUE if this node type set in nodes method translate.
   */
  public function isNodesMethod($bundle) {
    return variable_get('language_content_type_' . $bundle, NULL) == SMARTLING_NODES_METHOD_KEY;
  }

  /**
   * Checks fields method.
   *
   * @param string $bundle
   *   Node content type.
   *
   * @return bool
   *   Return TRUE if this node type set in fields method translate.
   */
  public function isFieldsMethod($bundle) {
    return  variable_get('language_content_type_' . $bundle, NULL) != SMARTLING_NODES_METHOD_KEY;
  }

  /**
   * Checks translatable field by field name.
   *
   * @param string $field_name
   *   Field name.
   *
   * @param string $entity_type
   *   Entity type machine name.
   * @return bool
   *   Return TRUE if field is translatable.
   */
  public function fieldIsTranslatable($field_name, $entity_type) {
    $field = $this->field_api_wrapper->fieldInfoField($field_name);
    return $this->field_api_wrapper->fieldIsTranslatable($entity_type, $field);
  }


  public function hookEntityUpdate($entity, $entity_type) {
    smartling_entity_update($entity, $entity_type);
  }

  /**
   * Checks any required configuration parameters are missing.
   *
   * @return bool
   *   Return TRUE if configuration parameters is set.
   */
  public function isConfigured() {
    $required_variables = array(
      'smartling_api_url',
      'smartling_key',
      'smartling_project_id',
      'smartling_target_locales',
    );
    foreach ($required_variables as $required_variable) {
      $val = variable_get($required_variable, NULL);
      if (empty($val)) {
        return FALSE;
      }
    }
    // All required configuration variables are set.
    return TRUE;
  }

  /**
   * @todo Add new 'Debug' option to smartling settings and save files to disk only if debugging is enabled
   * Otherwise skip saving. Other function must not require a phisical file on a disk
   *
   * Save xml document.

   * @param string $file_name
   *   File name.
   * @param string $content
   *   Xml document.
   *
   * @return bool
   *   Was file creation successful or not.
   */
  public function saveFile($file_name, $content) {
    $log = smartling_log_get_handler();

    if (empty($file_name)) {
      throw new SmartlingGenericException('Filename is empty.');
    }

    $file_name = file_munge_filename(preg_replace('@^.*/@', '', $file_name), '', TRUE);
    $directory = smartling_get_dir();
    $path = $directory . '/' . $this->cleanFileName($file_name);

    if (file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      file_put_contents(drupal_realpath($path), $content);

      return TRUE;
    }

    $log->error('Smartling file was not saved because of some errors. Filename: @file_name, related entity - @rid, directory: @dir.',
      array('@file_name' => $file_name, '@dir' => $directory), TRUE);

    return FALSE;
  }

  /**
   * Return clean filename, sanitized for path traversal vulnerability.
   *
   * Url (https://code.google.com/p/teenage-mutant-ninja-turtles
   * /wiki/AdvancedObfuscationPathtraversal).
   *
   * @param string $filename
   *   File name.
   * @param bool $allow_dirs
   *   TRUE if allow dirs. FALSE by default.
   *
   * @return string
   *   Return clean filename.
   */
  public function cleanFileName($filename, $allow_dirs = FALSE) {
    // Prior to PHP 5.5, empty() only supports variables.
    // (http://www.php.net/manual/en/function.empty.php).
    $trim_filename = trim($filename);
    if (empty($trim_filename)) {
      return '';
    }

    $pattern = '/[^a-zA-Z0-9_\-\:]/i';
    $info = pathinfo(trim($filename));
    $filename = preg_replace($pattern, '_', $info['filename']);
    if (isset($info['extension']) && !empty($info['extension'])) {
      $filename .= '.' . preg_replace($pattern, '_', $info['extension']);
    }

    if ($allow_dirs && isset($info['dirname']) && !empty($info['dirname'])) {
      $filename = preg_replace('/[^a-zA-Z0-9_\/\-\:]/i', '_', $info['dirname']) . '/' . $filename;
    }

    return (string) $filename;
  }


}
