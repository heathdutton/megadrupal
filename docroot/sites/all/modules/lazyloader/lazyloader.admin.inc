<?php
/**
 * @file
 * Lazyloader Admin
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Admin Form: UI
 *
 */
function lazyloader_admin() {
  $form = array();

  $form['lazyloader_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('lazyloader_enabled', LAZYLOADER_ENABLED),
    '#description' => t('Enable/Disable Lazyloader (Useful for testing)'),
    '#prefix' => '<div class="lazy-columns clearfix"><div class="lazy-column lazy-column-1">',
  );

  $form['lazyloader_distance'] = array(
    '#type' => 'textfield',
    '#title' => t('Distance'),
    '#default_value' => variable_get('lazyloader_distance', LAZYLOADER_DISTANCE),
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The distance (in pixels) of image to the viewable browser window before it loads the actual image.'),
  );

  $form['lazyloader_placeholder'] = array(
    '#type' => 'textfield',
    '#title' => t('Placeholder Image'),
    '#default_value' => variable_get('lazyloader_placeholder', LAZYLOADER_PLACEHOLDER),
    '#description' => t('Path to your placeholder image, ex. sites/default/files/placeholder_image.gif. Leave it blank to use the default image.'),
  );

  $loader_dir = base_path() . drupal_get_path('module', 'lazyloader') . '/loader/';
  $loader = array(0 => t('None'));
  for ($i = 1; $i <= 8; $i++) {
    $loader[$i] = '<img alt="Lazyloader Icon" src="' . $loader_dir . 'loader-' . $i . '.gif" />';
  }

  $form['lazyloader_icon'] = array(
    '#type' => 'radios',
    '#title' => t('Loader Icon'),
    '#options' => $loader,
    '#default_value' => variable_get('lazyloader_icon', LAZYLOADER_ICON),
    '#description' => t('This icon will show when the actual image is not yet fully loaded.'),
    '#suffix' => '</div>',
  );

  $form['lazyloader_exclude_option'] = array(
    '#type'          => 'radios',
    '#title'         => t('Enable on specific pages'),
    '#default_value' => variable_get('lazyloader_exclude_option', LAZYLOADER_EXCLUDE_OPTION),
    '#options'       => array(
      'exclude' => t('All pages except those listed'),
      'include' => t('Only the listed pages'),
    ),
    '#prefix' => '<div class="lazy-column lazy-column-2">',
  );

  $form['lazyloader_paths'] = array(
    '#type' => 'textarea',
    '#title_display' => 'invisible',
    '#title' => t('Pages'),
    '#default_value' => variable_get('lazyloader_paths', LAZYLOADER_PATHS),
    '#description'   => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog. <front> is the front page."),
    '#suffix' => '</div></div>',
  );

  $form['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'), '#weight' => 10);
  $form['#submit'][] = 'lazyloader_admin_submit';

  return system_settings_form($form);
}


/**
 * Submit handler for the lazyloader settings form.
 */
function lazyloader_admin_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Reset')) {
    // Reset form values.
    $form_state['values']['lazyloader_enable'] = LAZYLOADER_ENABLED;
    $form_state['values']['lazyloader_distance'] = LAZYLOADER_DISTANCE;
    $form_state['values']['lazyloader_icon'] = LAZYLOADER_ICON;
    $form_state['values']['lazyloader_placeholder'] = LAZYLOADER_PLACEHOLDER;
    $form_state['values']['lazyloader_paths'] = LAZYLOADER_PATHS;
    $form_state['values']['lazyloader_exclude_option'] = LAZYLOADER_EXCLUDE_OPTION;
  }

  // Rebuild the theme registry if the module was enabled/disabled.
  if ($form['lazyloader_enabled']['#default_value'] !== $form_state['values']['lazyloader_enabled']) {
    drupal_theme_rebuild();
  }
}
