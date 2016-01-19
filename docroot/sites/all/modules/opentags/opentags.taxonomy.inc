<?php
/**
 * @file
 * Create vocalularies and terms as widget data source.
 */

/**
 * Create a vocabulary.
 *
 * @param array $settings
 *   An associative array with vocabulary settings. It should have the following
 *   keys:
 *
 *   - name: Required. The display name of the vocabulary.
 *   - machine_name: Required. The machine name of the vocabulary.
 *   - description: Optional. The description of the vocabulary.
 *
 * @return object
 *   The created vocalulary's vid.
 */
function opentags_taxonomy_create_vocabulary(array $settings) {
  $vocab = new stdClass();
  $vocab->name = $settings['name'];
  $vocab->machine_name = $settings['machine_name'];
  if (isset($settings['description'])) {
    $vocab->description = $settings['description'];
  }
  taxonomy_vocabulary_save($vocab);
  return $vocab->vid;
}

/**
 * Create an associative array from a vocabulary xml file.
 *
 * @param string $vocab_name
 *   Vocabulary machine name, which is also the xml file name.
 *
 * @return array
 *   Arrary created from the xml file.
 */
function opentags_taxonomy_load_data_xml($vocab_name) {
  $xml = simplexml_load_file(OPENTAGS_MODULE_PATH . '/data/' . $vocab_name . '.xml');
  return json_decode(json_encode($xml), TRUE);
}
