<?php

/**
 * @file
 * API documentation for the Mechanical Tutor module.
 */

/**
 * Declares plugins providing scripted questions.
 *
 * This function only declares information about the classes that describe
 * scripted question. The actual question, as a class declaration, should be
 * stored in a separate file.
 *
 * An alternative way of declaring plugins is to have plugin class *and* plugin
 * information in the plugin file itself, and implement
 * hook_ctools_plugin_directory() to tell CTools to look for plugins in a
 * certain folder.
 *
 * @see hook_ctools_plugin_directory()
 *
 * @return
 *   An associative array with plugin information. Each key should be the
 *   identier for the plugin (no longer than 128 characters), and the value
 *   should be an array with optional extra information:
 *   - path: The path to the directory with the plugin file, to the module base
 *     folder. Do not open or close with slash. Defaults to NULL, which means
 *     that CTools will look for the plugin in the root folder.
 *   - file: The name of the file with the plugin. Defaults to PLUGIN_ID.inc.
 *   - handler: The name of the class for your question. Defaults to the
 *     question ID converted to CamelCase, i.e. YourQuestionID.
 *   - settings: If one plugin class manages more than one type of question,
 *     this entry may be used to pass settings to the question object when
 *     created. Normally it is better to create separate classes for each
 *     question, but this can be used for questions that depend on site
 *     settings. Defaults to an empty array.
 */
function hook_tutor_question() {
  // Two 'normal' question plugins, using default settings: class and file names
  // are inherited from the plugin ID.
  $questions = array(
    'mymodule_question_1' => array(),
    'mymodule_question_2' => array(),
  );

  // Declare a dynamic number of questions, based on information collected by
  // mymodule_plugin_variants().
  foreach(mymodule_plugin_variants() as $id => $settings) {
    $questions[$id] = array(
      'file' => 'mymodule_question_3.inc',
      'handler' => 'mymodule_question_3',
      'settings' => $settings,
    );
  }

  return $questions;
}
