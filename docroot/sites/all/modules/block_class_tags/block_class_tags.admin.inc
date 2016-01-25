<?php
/**
 * @file
 * Administration page callbacks for the block_class_tags module.
 *
 * @ingroup block_class_tags
 * @{
 */

/**
 * Page callback/form for admin interface.
 */
function block_class_tags_admin_settings() {
  $form = array();

  $form['default'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default settings'),
    '#description' => t("Enable various plugins for Block Class Tags."),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['default']['block_class_tags_default_tags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable default Block Class Tags'),
    '#default_value' => variable_get('block_class_tags_default_tags', FALSE),
    '#description' => t("Enable default class tags defined by the Block Class Tags module. This is helpful to show how implementing your own class tags would work."),
    '#required' => FALSE,
  );
  $form['default']['block_class_tags_defined_tags'] = array(
    '#type' => 'textarea',
    '#title' => t('Predefined Classes'),
    '#description' => t('Add predefined classes here for use across all blocks. Add multiple classes by adding each class on a new line. All values will be processed through !function.', array('!function' => l('drupal_clean_css_identifier()', 'https://api.drupal.org/api/drupal/includes%21common.inc/function/drupal_clean_css_identifier/7'))),
    '#default_value' => variable_get('block_class_tags_defined_tags'),
  );
  $form['plugins'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tagging Plugins'),
    '#description' => t("Enable various plugins for Block Class Tags."),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['plugins']['block_class_tags_plugin_tags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Plugin: Tags'),
    '#default_value' => variable_get('block_class_tags_plugin_tags'),
    '#description' => t("Tags plugin brings in the traditional tag functionality where user can assemble and edit list of tags. Tags plugin works especially well together with Autocomplete, Filter, Suggestions and Ajax plugins to provide full spectrum of features. It can also work on its own and just do one thing – tags."),
    '#required' => FALSE,
  );
  $form['plugins']['block_class_tags_plugin_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Plugin: Autocomplete'),
    '#default_value' => variable_get('block_class_tags_plugin_autocomplete'),
    '#description' => t("Autocomplete plugin brings the classic autocomplete functionality to the TextExt ecosystem. The gist of functionality is when user starts typing in, for example a term or a tag, a dropdown would be presented with possible suggestions to complete the input quicker."),
    '#required' => FALSE,
  );
  // $form['plugins']['block_class_tags_plugin_arrow'] = array(
  //   '#type' => 'checkbox',
  //   '#title' => t('Plugin: Arrow'),
  //   '#default_value' => variable_get('block_class_tags_plugin_arrow'),
  //   '#description' => t("Displays a dropdown style arrow button. The TextExtArrow works together with the TextExtAutocomplete plugin and whenever clicked tells the autocomplete plugin to display its suggestions."),
  //   '#required' => FALSE,
  //   '#states' => array(
  //     'visible' => array(
  //       ':input[name="block_class_tags_plugin_autocomplete"]' => array('checked' => TRUE),
  //     ),
  //   ),
  // );
  $form['plugins']['block_class_tags_plugin_clear'] = array(
    '#type' => 'checkbox',
    '#title' => t('Plugin: Clear'),
    '#default_value' => variable_get('block_class_tags_plugin_clear'),
    '#description' => t("Displays a \"clear search\" button."),
    '#required' => FALSE,
  );
  $form['plugins']['block_class_tags_plugin_focus'] = array(
    '#type' => 'checkbox',
    '#title' => t('Plugin: Focus'),
    '#default_value' => variable_get('block_class_tags_plugin_focus'),
    '#description' => t("Focus plugin displays a visual effect whenever user sets focus into the text area."),
    '#required' => FALSE,
  );
  $form['plugins']['block_class_tags_plugin_prompt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Plugin: Prompt'),
    '#default_value' => variable_get('block_class_tags_plugin_prompt'),
    '#description' => t("Prompt plugin displays a visual user prompt in the text input area. If user focuses on the input, the prompt is hidden and only shown again when user focuses on another element and text input doesn’t have a value."),
    '#required' => FALSE,
  );

  $form['#validate'][] = 'block_class_tags_admin_validate';

  return system_settings_form($form);
}

/**
 * Validate input for Block Class Tags settings.
 */
function block_class_tags_admin_validate($form, &$form_state) {
  $classes = explode("\n", $form_state['input']['block_class_tags_defined_tags']);
  foreach ($classes as $key => $value) {
    $classes[$key] = drupal_clean_css_identifier(trim($value));
  }
  $form_state['values']['block_class_tags_defined_tags'] = implode("\n", $classes);
}
