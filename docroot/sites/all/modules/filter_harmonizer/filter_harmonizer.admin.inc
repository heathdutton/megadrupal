<?php
/**
 * @file
 * filter_harmonizer.admin.module
 *
 * Configuration form for global and advanced filter harmonizer settings.
 */

/**
 * Menu callback for admin settings.
 */
function filter_harmonizer_admin_config() {
  $form['filter_harmonizer_settings_functional'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter Harmonizer functional settings'),
  );
  $form['filter_harmonizer_settings_functional']['filter_harmonizer_always'] = array(
    '#type' => 'checkbox',
    '#title' => t('<em>After</em> initial page load, ignore <strong>any</strong> <em>contextual</em> filter that also has a companion <em>exposed</em> filter for the same field'),
    '#default_value' => variable_get('filter_harmonizer_always', FALSE),
    '#description' => t("This is a shortcut to checking all the per-field check boxes on the contextual and companion exposed filter configuration pages. All these will be considered ticked and <em>won't</em> appear as a choice for any field."),
  );
  $form['filter_harmonizer_settings_functional']['filter_harmonizer_merge_filter_values'] = array(
    '#type' => 'checkbox',
    '#title' => t('On initial page load, merge rather than replace, exposed filter values with contextual filter values'),
    '#default_value' => variable_get('filter_harmonizer_merge_filter_values', FALSE),
    '#description' => t('Applies only to exposed filters that have <strong>Allow multiple selections</strong> checked.'),
  );

  $form['filter_harmonizer_settings_display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter Harmonizer display settings'),
    '#descripton' => t('These options affect the display only.')
  );
  $form['filter_harmonizer_settings_display']['filter_harmonizer_fill_exposed'] = array(
    '#type' => 'checkbox',
    '#title' => t('On initial page load, auto-fill the exposed filter form with the contextual filter values applied to the page'),
    '#default_value' => variable_get('filter_harmonizer_fill_exposed', TRUE),
    '#description' => t('While not affecting the filter behaviour itself, this is a useful visual reflection of what is happening.'),
  );
  $form['filter_harmonizer_settings_display']['filter_harmonizer_adjust_browser_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('When the user changes the exposed filter, auto-reflect the selection in the address bar of the browser'),
    '#default_value' => variable_get('filter_harmonizer_adjust_browser_url', FALSE),
    '#description' => t('While the URL in address bar changes, there is no additional server request or page refresh. Like the previous option, this is merely a visual reflection of what is happening.<br/>This option is somewhat experimental and may not display correctly in some cases.'),
  );

  return system_settings_form($form);
}
