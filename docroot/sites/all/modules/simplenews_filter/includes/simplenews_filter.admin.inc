<?php

/**
 * @file
 * Simplenews filter settings.
 *
 * @ingroup simplenews_filter
 */

/**
 * Menu callback: Simplenews filter admin settings form.
 */
function simplenews_filter_admin_settings_form($form, &$form_state) {
  $form = array();

  $form['simplenews_filter_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default simplenews filter options'),
    '#collapsible' => FALSE,
  );
  $form['simplenews_filter_defaults']['simplenews_filter_default_view'] = array(
    '#type' => 'select',
    '#title' => t('Which view should be selected by default?'),
    '#options' => simplenews_filter_get_views_as_options(),
    '#description' => t('View must be of type <em>simplenews_subscriber</em>. Any exposed filters can be configured on each newsletter send page.'),
    '#default_value' => variable_get('simplenews_filter_default_view', NULL),
  );

  return system_settings_form($form);
}
