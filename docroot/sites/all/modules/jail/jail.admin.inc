<?php

/**
 * @file
 * Configuration options for JAIL Formatter settings
 */

/**
 * Menu callback; Settings administration.
 */
function jail_admin_settings() {
  $form['jail_users'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Lazy Load User Images'),
    '#description'   => t('If enabled, user pictures will "lazy load"'),
    '#default_value' => variable_get('jail_users', 0),
  );

  $form['jail_noscript'] = array(
     '#title' => t('No Script images'),
     '#type' => 'checkbox',
     '#description' => t('When checked, a &lt;noscript&gt; tag will be added containing a regular version of the file.  This will increase markup.'),
    '#default_value'  => variable_get('jail_noscript', 0),
   );
  $form['jail_place'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Place Holder Image'),
    '#description'   => t('Path to place holder image'),
    '#default_value' => variable_get('jail_place', drupal_get_path('module', 'jail') . '/white.gif'),
  );
  $form['jail_loader'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Loader Image'),
    '#description'   => t('Path to loader image'),
    '#default_value' => variable_get('jail_loader', drupal_get_path('module', 'jail') . '/white.gif'),
  );
  $form['jail_effect'] = array(
    '#type'           => 'select',
    '#title'          => t('User Picture Effect'),
    '#description'    => t('Select the effect to use for user pictures'),
    '#default_value'  => variable_get('jail_effect','show'),
    '#options'        => array(
      'show'  => 'None',
      'fadeIn' => 'Fade in',
      'slideDown'  => 'Slide down',
    ),
  );
  return system_settings_form($form);
}
