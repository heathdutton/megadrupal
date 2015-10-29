<?php

/**
 * @file
 * Contains generator classes for .info files.
 */

namespace ModuleBuider\Generator;

/**
 * Generator base class for module info file.
 *
 * This needs to go last in the order of subcomponents, so that it can see
 * all the files requested so far, and if required, declare them in the info
 * file.
 */
class Info extends File {

  /**
   * Build the code files.
   */
  function collectFiles(&$files) {
    $files['info'] = array(
      'path' => '', // Means base folder.
      'filename' => $this->base_component->component_data['module_root_name'] . '.info',
      // We pass $files in to check for files containing classes.
      'body' => $this->file_body($files),
      // We join the info lines with linebreaks, as they (currently!) do not
      // come with their own lineends.
      // TODO: fix this!
      'join_string' => "\n",
    );
  }

}

/**
 * Abstract parent class for .ini syntax info files.
 */
class InfoIni extends Info {

  /**
   * Process a structured array of info files lines to a flat array for merging.
   *
   * @param $lines
   *  An array of lines keyed by label.
   *  Place grouped labels (eg, dependencies) into an array of
   *  their own, keyed numerically.
   *  Eg:
   *    name => module name
   *    dependencies => array(foo, bar)
   *
   * @return
   *  An array of lines for the .info file.
   */
  function process_info_lines($lines) {
    foreach ($lines as $label => $data) {
      if (is_array($data)) {
        foreach ($data as $data_piece) {
          $merged_lines[] = $label . "[] = $data_piece"; // Urgh terrible variable name!
        }
      }
      else {
        $merged_lines[] = "$label = $data";
      }
    }

    // Add final empty line so the file has a closing linebreak.
    $merged_lines[] = '';

    //drush_print_r($merged_lines);
    return $merged_lines;
  }

}

/**
 * Generator class for module info file for Drupal 5.
 */
class Info5 extends InfoIni {

  /**
   * Create lines of file body for Drupal 5.
   */
  function file_body() {
    $module_data = $this->base_component->component_data;

    $lines = array();
    $lines['name'] = $module_data['module_readable_name'];
    $lines['description'] = $module_data['module_short_description'];

    if (!empty($module_data['module_dependencies'])) {
      $lines['dependencies'] = $module_data['module_dependencies'];
    }

    if (!empty($module_data['module_package'])) {
      $lines['package'] = $module_data['module_package'];
    }

    $info = $this->process_info_lines($lines);
    return $info;
  }

}

/**
 * Generator class for module info file for Drupal 6.
 */
class Info6 extends InfoIni {

  /**
   * Create lines of file body for Drupal 6.
   */
  function file_body() {
    $module_data = $this->base_component->component_data;

    $lines = array();
    $lines['name'] = $module_data['module_readable_name'];
    $lines['description'] = $module_data['module_short_description'];
    if (!empty($module_data['module_dependencies'])) {
      // For lines which form a set with the same key and array markers,
      // simply make an array.
      foreach (explode(' ', $module_data['module_dependencies']) as $dependency) {
        $lines['dependencies'][] = $dependency;
      }
    }

    if (!empty($module_data['module_package'])) {
      $lines['package'] = $module_data['module_package'];
    }
    $lines['core'] = "6.x";

    $info = $this->process_info_lines($lines);
    return $info;
  }

}

/**
 * Generator class for module info file for Drupal 7.
 */
class Info7 extends InfoIni {

  /**
   * Create lines of file body for Drupal 7.
   */
  function file_body() {
    $args = func_get_args();
    $files = array_shift($args);

    $module_data = $this->base_component->component_data;
    //print_r($module_data);

    $lines = array();
    $lines['name'] = $module_data['module_readable_name'];
    $lines['description'] = $module_data['module_short_description'];
    if (!empty($module_data['module_dependencies'])) {
      // For lines which form a set with the same key and array markers,
      // simply make an array.
      foreach (explode(' ', $module_data['module_dependencies']) as $dependency) {
        $lines['dependencies'][] = $dependency;
      }
    }

    if (!empty($module_data['module_package'])) {
      $lines['package'] = $module_data['module_package'];
    }

    $lines['core'] = "7.x";

    // Files containing classes need to be declared in the .info file.
    foreach ($files as $file) {
      if (!empty($file['contains_classes'])) {
        $lines['files'][] = $file['filename'];
      }
    }

    $info = $this->process_info_lines($lines);
    return $info;
  }

}

/**
 * Generator class for module info file for Drupal 8.
 */
class Info8 extends Info {

  /**
   * Build the code files.
   */
  function collectFiles(&$files) {
    parent::collectFiles($files);

    $files['info']['filename'] = $this->base_component->component_data['module_root_name'] . '.info.yml';
  }

  /**
   * Create lines of file body for Drupal 8.
   */
  function file_body() {
    $args = func_get_args();
    $files = array_shift($args);

    $module_data = $this->base_component->component_data;
    print_r($module_data);

    $lines = array();
    $lines['name'] = $module_data['module_readable_name'];
    $lines['type'] = $module_data['base'];
    $lines['description'] = $module_data['module_short_description'];
    if (!empty($module_data['module_dependencies'])) {
      // For lines which form a set with the same key and array markers,
      // simply make an array.
      foreach (explode(' ', $module_data['module_dependencies']) as $dependency) {
        $lines['dependencies'][] = $dependency;
      }
    }

    if (!empty($module_data['module_package'])) {
      $lines['package'] = $module_data['module_package'];
    }

    $lines['core'] = "8.x";

    // Files containing classes need to be declared in the .info file.
    foreach ($files as $file) {
      if (!empty($file['contains_classes'])) {
        $lines['files'][] = $file['filename'];
      }
    }

    $info = $this->process_info_lines($lines);
    return $info;
  }

  /**
   * Process a structured array of info files lines to a flat array for merging.
   *
   * @param $lines
   *  An array of lines keyed by label.
   *  Place grouped labels (eg, dependencies) into an array of
   *  their own, keyed numerically.
   *  Eg:
   *    name => module name
   *    dependencies => array(foo, bar)
   *
   * @return
   *  An array of lines for the .info file.
   */
  function process_info_lines($lines) {
    $yaml_parser = new \Symfony\Component\Yaml\Yaml;
    $yaml = $yaml_parser->dump($lines, 2, 2);
    //drush_print_r($yaml);

    // Because the yaml is all built for us, this is just a singleton array.
    return array($yaml);
  }

}
