<?php

/**
 * @file
 * Plugin API provided by the conditional_text module.
 */

/**
 * @defgroup conditional_text_plugin Conditional text plugin types.
 * @{
 * Condition text uses the CTools plugin system, so if you want to implement a
 * plugin, read the CTools plugin documentation first. Basically you have to
 * implement hook_ctools_plugin_directory(), which tells the CTools plugin
 * system about the plugin files. Every plugin file must contain an associative
 * array, called $plugin.
 *
 * Conditional text uses two kind of plugins: display plugins and condition
 * plugins. For examples, see the existing plugins in the plugins directory.
 *
 * Display plugins control how the conditional text will be displayed, by
 * defining what theme functions are called. A display plugin array contains
 * the following elements:
 * - title: Translated human-readable name of the plugin.
 * - description: Translated longer description of the plugin.
 * - theme: Theme function(s) to call. There are two possibilities:
 *   - Array of two function names, with keys 'true' and 'false', to be used for
 *     displaying condtions that evaluate to TRUE and FALSE. Each theme
 *     function has two variables ($text, $reason), where $text is the
 *     conditional content, and $reason is text from the condition plugin
 *     explaining why the text is hidden or shown (suitable for use as a
 *     fieldset label). If you omit one of the functions, the conditional text
 *     will just be output as if there was no condition. See
 *     plugins/display/filter.inc for an example.
 *   - One function nane. The theme function variables are ($text, $reason,
 *     $result), where $text and $reason are as above, and $result is the result
 *     of the evaluation (TRUE or FALSE).
 * - js: Array of additional JavaScript files to load.
 * - css: Array of additional CSS files to load.
 *
 * Example with two theme functions:
 * @code
 * $plugin = array(
 *   'title' => t('Display plugin title'),
 *   'description' => t('Display plugin description.'),
 *   'theme' => array(
 *     'false' => 'false_theme_function_name',
 *     'true' => 'true_theme_function_name',
 *   ),
 *   'js' => array(
 *      drupal_get_path('module', 'mymodule') . '/plugins/display/foo.js',
 *   ),
 *   'css' => array(
 *     drupal_get_path('module', 'mymodule') . '/plugins/display/bar.css',
 *   ),
 * );
 * @endcode
 * Example with one theme function:
 * @code
 * $plugin = array(
 *   'title' => t('Display plugin title'),
 *   'description' => t('Display plugin description.'),
 *   'theme' => 'theme_function_name',
 *   'js' => array(),
 *   'css' => array(),
 * );
 * @endcode
 *
 * Condition plugins evaluate the conditions. There are several components:
 * - A settings form.
 * - An evaluator function, which evaluates the conditions.
 * - A reason callback, which explains the result.
 * A condition plugin array contains the following elements:
 * - title: Translated human-readable name of the plugin.
 * - description: Translated longer description of the plugin.
 * - identifier token: The identifier token of the condition plugin (normally
 *   the same as the name of the plugin). This is the identifier used in
 *   conditional text editing to identify that this plugin is used to evaluate
 *   the condition.
 * - settings: Settings form function for the plugin. The function signature is
 *   (&$form_state, $filter), where $form_state is the usual Form API form
 *   state, and $filter is an array of settings. Returns a form array.
 * - evaluate: Evaluation function used to evaluate the condition. The function
 *   signature is ($expression, $context), where $expression is a list of tokens
 *   from parsing the condition line, and $context is the settings array from
 *   the settings form. Returns TRUE or FALSE.
 * - reason: Function that returns the "reason" string (suitable for display as
 *   a conditional fieldset label) for the condition (same parameters as the
 *   evaluation function).
 * - settings include: Specifies an additional file to load with the settings
 *   form. This can be useful if the settings form has callbacks (e.g., a submit
 *   or element validate callback). Leave empty if you don't need such a file.
 * - short help: Function that generates short filter help text to be displayed
 *   in the Filter Tips section of the editing form. The arguments are the same
 *   as for hook_filter_FILTER_tips().
 * - long help: Function that generates the long filter help text to be
 *   displayed on the "More information about text formats" page. The arguments
 *   are the same as for hook_filter_FILTER_tips().
 *
 * Example:
 * @code
 * $plugin = array(
 *   'title' => t('Condition plugin title.'),
 *   'description' => t('Condition plugin description.'),
 *   'identifier token' => 'idtoken',
 *   'settings' => function (&$form_state, $filter) {},
 *   'evaluate' => function ($expression, $context = NULL) {},
 *   'reason' => function ($expression, $context = NULL) {},
 *   'settings include' => '',
 *   'short help' => function ($filter, $format) {},
 *   'long help' => function ($filter, $format) {},
 * );
 * @endcode
 *
 * @}
 */

/**
 * @addtogroup conditional_text_plugin
 * @{
 */

/**
 * Define conditional text operators.
 *
 * @return array
 *   Associative array of the operators. The key is the operator itself,
 *   and the value is an associative array where:
 *   - arity: The prefix arity, either CONDITIONAL_TEXT_ARITY_PREFIX or
 *     CONDITIONAL_TEXT_ARITY_INFIX. In a case of a prefix arity, the operator
 *     must be written before the expression. In a case of an infix arity, the
 *     operator must be written between two expressions.
 *   - evaluate: A function that takes 'arity' number of boolean arguments,
 *     and returns the boolean result of the operator.
 */
function hook_conditional_text_operators() {
  return array(
    'not' => array(
      'arity' => CONDITIONAL_TEXT_ARITY_PREFIX,
      'evaluate' => function($exp) { return !$exp; },
    ),
    'and' => array(
      'arity' => CONDITIONAL_TEXT_ARITY_INFIX,
      'evaluate' => function($exp0, $exp1) { return $exp0 && $exp1; },
    ),
  );
}

/**
 * @}
 */
