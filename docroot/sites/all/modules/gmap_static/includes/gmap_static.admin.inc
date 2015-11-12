<?php

/**
 * @file
 * Provides the Google Map Static Module administrative interface.
 */

/**
 * Form builder for the "gmap_static" configuration page.
 */
function gmap_static_config($form, &$form_state) {
  $replace_iframe_to = array(
    'computer' => t('Desktop preferences'),
    'mobile' => t('Mobile preferences'),
  );
  $replace_iframe_options = array(
    'none' => t('Do nothing.'),
    'popup_map' => t('Change iframe to static map. Switchable to fullscreen popup.'),
    'change_condition' => t('Change iframe to static map. Switchable back to iframe.'),
    'in_new_window' => t('Change iframe to static map. Display Iframe in a new window.'),
  );
  $table_headers = array("View Mode") + $replace_iframe_to;
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $table_headers,
  );
  foreach ($replace_iframe_options as $option => $option_description) {
    $cell = array(
      'data' => array(
        '#type' => 'item',
        '#description' => $option_description,
        '#title' => ucfirst(str_replace("_", " ", $option)),
      ),
    );
    $form['table']['#rows'][$option][] = $cell;
    $options = variable_get('gmap_static_view_mode', array());
    foreach ($replace_iframe_to as $device => $device_description) {
      $cell = array(
        'data' => array(
          '#type' => 'radio',
          '#name' => $device,
          '#attributes' => array('value' => $option),
        ),
      );
      $default_option = (isset($options[$device])) ? $options[$device] : 'none';
      if ($default_option == $option) {
        $cell['data']['#attributes']['checked'] = 'checked';
      }
      $form['table']['#rows'][$option][] = $cell;
    }
  }
  $form['visibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility settings'),
  );
  $options = array(
    BLOCK_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    BLOCK_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description_text = 'Specify pages by using their paths. Enter one path per ';
  $description_text .= 'line. The "*" character is a wildcard. Example paths ';
  $description_text .= 'are %blog for the blog page and %blog-wildcard for ';
  $description_text .= 'every personal blog. %front is the front page.';
  $description_extra = array(
    '%blog' => 'blog',
    '%blog-wildcard' => 'blog/*',
    '%front' => '<front>',
  );
  $description = t($description_text, $description_extra);
  $form['visibility']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Change iframe to static map on specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('gmap_static_visibility', 0),
  );
  $form['visibility']['pages'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('gmap_static_pages', ''),
    '#description' => $description,
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

/**
 * Process the "gmap_static" configuration form 'save'.
 */
function gmap_static_config_submit($form, &$form_state) {
  $options = array();
  foreach ($form_state['input'] as $device => $device_description) {
    $options[$device] = $device_description;
  }
  variable_set('gmap_static_view_mode', $options);
  variable_set('gmap_static_visibility', (int) $form_state['values']['visibility']);
  variable_set('gmap_static_pages', trim($form_state['values']['pages']));
  drupal_set_message(t('The configuration options has been saved.'));
}
