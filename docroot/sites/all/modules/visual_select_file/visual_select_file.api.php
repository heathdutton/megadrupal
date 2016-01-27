<?php

/**
 * Implements hook_visual_select_file_formatter_options_alter().
 *
 * $options contains #options-like options for a select element.
 * $context contains:
 * - 'form' -- the full form array of views_exposed_form().
 * - 'form_state' -- the full form_state array of views_exposed_form().
 * - 'view' -- the full View object for the current View.
 * - 'field' -- the 'vsf_field' string from $_GET, with format 'entity_type.bundle.field_name'.
 * - 'default_formatter' (by ref) -- the default selected formatter, defaults to empty (- Choose -).
 */
function hook_visual_select_file_formatter_options_alter(&$options, &$context) {
  // No huge allowed!
  unset($options['huge'], $options['huge_2']);

  // Move 'pagewide' to the very top and select it.
  $options = array('pagewide' => 'pagewide') + $options;
  $context['default_formatter'] = 'pagewide';
}

/**
 * Implements hook_visual_select_file_results_alter().
 *
 * $results contains:
 * - 'results' -- a 2D array with array($fid, $path, $name and $original).
 * $context contains:
 * - 'view' -- the full View object, with raw results etc.
 * - 'field' -- the 'vsf_field' string from $_GET, with format 'entity_type.bundle.field_name'
 */
function hook_visual_select_file_results_alter(&$results, $context) {

}



/**
 * Some custom form on your website, maybe global, or per-domain, or not.
 */
function YOURMODULE_custom_form($form, &$form_state) {
  $form['test_wysiwyg3'] = array(
    '#type' => 'text_format',
    '#title' => t('Pretty text 3'),
    '#default_value' => $value['value'],
    '#format' => $value['format'],

    // The type of file usage/form/config we are now. For entities, this is the entity type, but
    // that's not so for custom forms. A common practice is using the form name.
    '#vsf_file_usage_type' => 'YOURMODULE_custom_form',

    // The instance of the above type you defined. For entities, this is the entity id, but any
    // ID will do. A few examples:
    // Note. {file_usage} only saves 32 bit ints, so strings will be converted using crc32().

    // This form has many many WYSIWYG fields, so I want per-field usage.
    '#vsf_file_usage_id' => 'test_wysiwyg3',

    // This form is global, once, entire site, so ANY ID will do, so keep it simple.
    '#vsf_file_usage_id' => 1,

    // This very form is used for a few Domains, so defaults and values are domain-specific. Kind of
    // like an entity id, but per-domain.
    '#vsf_file_usage_id' => $domain['domain_id'],
  );

  // Custom forms that save into variables use this. Use whatever submit handler you want, but ADD
  // MINE TOO!
  $form = system_settings_form($form);

  // If you want automatic file_usage, YOU NEED THIS SUBMIT HANDLER.
  $form['#submit'][] = 'vsf_wysiwyg_save_file_usage_submit';

  // Note. If you don't care about {file_usage} at all (you silly you!), you don't need #vsf_file_usage_type
  // and #vsf_file_usage_id, but you will need #entity_type, #bundle and #field_name.
  // @see vsf_wysiwyg_pre_render_wysiwyg_element()

  return $form;
}
