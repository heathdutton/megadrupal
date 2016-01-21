<?php

/**
 * @file
 * Contains the available hooks of the Stanbol Enhancer.
 */

/**
 * Define the Stanbol Enhancer result processors.
 *
 * This hook is required to add a new result processors to the Stanbol Enhancer.
 *
 * @return array
 *   An array of information about the module's provided Stanbol Enhancer result
 *   processors. The array contains sub-arrays for each result processor with
 *   the result processor name as the key.
 *   Possible attributes of each sub-arrays are:
 *   - label: The label of the processor.
 *   - description: The description of the processor.
 *   - type: The data types of the entity properties which could store the
 *     returned value of the processor. @see https://www.drupal.org/node/905632
 *   - conditions: Array of additional data type property conditions for the
 *     selected data type. @see entity_get_property_info()
 *   - group identifier: (optional) Describes which Stanbol Enhancer fields
 *     should be used as an input to the process callback. The fields in the
 *     Stanbol's (raw) result will be re-grouped according to this values.
 *   - processor callback: The function which will be used to retrieve the
 *     values from the Stanbol results.
 *   - processor callback settings: Array of additional settings for the
 *     process callback. This will be passed to the function.
 *     - ignore confidence: If this is TRUE, than the processors will be run
 *       before the un-sure values will be removed from the Stanbol Enhancer
 *       results, which means this processor will ignore the minimum confidence
 *       level settings.
 *   - value callback: The function witch provides the proper values to the
 *     assigned entity property. (For example: If the assigned field is a
 *     term reference field, then the tids of the terms. So the processor
 *     callback will extract the name of terms and the value callback will
 *     find the matching term ids of these term names or create a new term
 *     with the missing term name.)
 */
function hook_stanbol_enhancer_result_processor_info() {
  $processors = [];

  $processors['language'] = [
    'label' => t('Language'),
    'description' => t("Set the entity's language property to the detected language. Only works on those entities which have language attribute."),
    'type' => 'token',
    'conditions' => [
      'schema field' => 'language',
    ],
    'processor callback' => '_stanbol_enhancer_language_process_callback',
    'processor callback settings' => [
      'ignore confidence' => TRUE,
    ],
    'value callback' => '_stanbol_enhancer_language_value_callback',
  ];
  $processors['organizations_term'] = [
    'label' => t('Organizations'),
    'description' => t('Store the identified organizations in a taxonomy term field.'),
    'type' => 'list<taxonomy_term>',
    'group identifier' => STANBOL_ENHANCER_FIELD_TYPE_ORGANIZATION,
    'processor callback' => '_stanbol_enhancer_organizations_process_callback',
    'processor callback settings' => [
      'capitalize filter limit' => 4,
    ],
    'value callback' => '_stanbol_enhancer_term_reference_value_callback',
  ];

  return $processors;
}

/**
 * Alter the available Stanbol Enhancer result processors.
 *
 * @param array $processors
 *   Associative array of available result processors.
 *
 * @see hook_stanbol_enhancer_result_processor_info()
 */
function hook_stanbol_enhancer_result_processor_info_alter(array &$processors) {
  unset($processors['language']);
}
