<?php
/**
 * @file
 * Riles integration for the Yandex.Webmaster Original texts module.
 */

/**
 * Implements hook_rules_action_info().
 * @ingroup rules
 */
function yandex_webmaster_original_texts_rules_action_info() {
  return array(
    'yandex_webmaster_original_texts_rules_add_original_text' => array(
      'label' => t('Add original text'),
      'group' => t('Yandex.Webmaster'),
      'parameter' => array(
        'host' => array(
          'label' => t('Site'),
          'type' => 'list<integer>',
          'options list' => 'yandex_webmaster_original_texts_rules_host_selector',
          'restriction' => 'input',
          'description' => t('Select the site this text belongs to.'),
          'default value' => yandex_webmaster_get_host_id(),
        ),
        'text' => array(
          'label' => t('Original text'),
          'type' => 'text',
          'description' => t('Original text to add to Yandex.'),
        ),
      ),
      'provides' => array(
        'code' => array(
          'label' => t('Response code'),
          'type' => 'integer',
        ),
      ),
    ),
  );
}

/**
 * Rules action.
 */
function yandex_webmaster_original_texts_rules_add_original_text($host_id, $text) {
  $result = yandex_webmaster_original_texts_add_original_text(array_pop($host_id), $text);
  return array('code' => $result->code);
}

/**
 * Returns array of verified hosts.
 */
function yandex_webmaster_original_texts_rules_host_selector() {
  $options = array();
  foreach (yandex_webmaster_get_hosts() as $host_id => $host) {
    if ($host['verification'] == 'VERIFIED') {
      $options[$host_id] = $host['name'];
    }
  }
  return $options;
}
