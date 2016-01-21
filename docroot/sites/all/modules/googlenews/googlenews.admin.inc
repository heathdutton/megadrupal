<?php

/**
 * Form builder; administration settings.
 */
function googlenews_admin_settings() {
  $node_types = node_type_get_names();

  $form['help'] = array(
    '#markup' => '<p>' . t('Settings for controlling the <a href="@news-sitemap">Google News sitemap file</a>.', array('@news-sitemap' => url('googlenews.xml'))) . "</p>\n",
  );
  $form['count'] = array(
    '#markup' => '<p>' . t('There are currently @count node(s) suitable for output.', array('@count' => count(googlenews_list_nodes()))) . "</p>\n",
  );

  $form['googlenews_publication_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Publication name'),
    '#default_value' => variable_get('googlenews_publication_name', ''),
    '#description' => t("Leave blank to use the site's name instead: :site_name", array(':site_name' => variable_get('site_name', 'Drupal'))),
  );

  $form['googlenews_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select the content types to include'),
    '#default_value' => variable_get('googlenews_node_types', array_keys($node_types)),
    '#options' => $node_types,
  );
  $form['array_filter'] = array(
    '#type' => 'value',
    '#value' => TRUE,
  );

  $form['googlenews_cache_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache timeout (minutes)'),
    '#default_value' => variable_get('googlenews_cache_timeout', 15),
    '#description' => t('The number of minutes that the sitemap file will be cached for before it is regenerated.'),
  );

  $form['googlenews_content_hours'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum content age (hours)'),
    '#default_value' => intval(variable_get('googlenews_content_hours', 48)),
    '#description' => t('All content (nodes) created within this number of hours will be included in the sitemap file. It is recommended to leave this at the default of 48 hours.'),
  );

  // Optional tags.
  $form['tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Optional tags'),
    '#description' => t('Enter values to use for any of the optional GoogleNews tags. Tokens are supported, see below for a list of available tokens that may be used.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  foreach (googlenews_tag_list() as $tag_name => $tag) {
    $var_name = 'googlenews_tag_' . $tag_name;
    $form['tags'][$var_name] = array(
      '#type' => 'textfield',
      '#title' => $tag['title'],
      '#default_value' => variable_get($var_name, ''),
      '#description' => $tag['description'],
    );
    // If an 'options' value was present, display the field as a selector.
    if (!empty($tag['options'])) {
      $form['tags'][$var_name]['#type'] = 'select';
      $form['tags'][$var_name]['#options'] = $tag['options'];
      // Some selectors allow multiple values so display them as checkboxes.
      if (!empty($tag['multiple'])) {
        $form['tags'][$var_name]['#type'] = 'checkboxes';
        $form['tags'][$var_name]['#multiple'] = TRUE;
      }
    }
  }

  // Display the list of available placeholders if token module is installed.
  if (module_exists('token')) {
    $form['tags']['token_help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
      '#dialog' => TRUE,
    );
  }

  return system_settings_form($form);
}
