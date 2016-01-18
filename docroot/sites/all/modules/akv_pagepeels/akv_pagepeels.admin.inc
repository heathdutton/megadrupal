<?php
// $Id:

/**
 * Menu callback for admin settings
 */
function akv_pagepeels_admin_settings($form, &$form_state) {
  $form['type'] = array(
    '#title' => t('Pagepeel type'),
    '#description' => t('Choose the type of collapsed pagepeel'),
    '#type' => 'radios',
    '#default_value' => variable_get('akv_type', 'latest'),
    '#options' => array(
      'latest' => t('Latest pagepeel from AKV, default'),
      'schaeuble' => t('Stasi 2.0 classic'),
      'schaeuble_green' => t('Stasi 2.0 green'),
      'cam' => t('Surveilance camera(2D)'),
      '3d_cam' => t('Surveilance camera(3D)'),
      'ani_cam' => t('Surveilance camera(3D + animated'),
      'trauer' => t('Condolement, 1949 - 2007 t'),
    ),
  );
  $form['info'] = array(
    '#title' => t('Show info'),
    '#description' => t('Show info icon under the pagepeel'),
    '#type' => 'radios',
    '#default_value' => variable_get('akv_info', 'on'),
    '#options' => array(
      'on' => t('Show'),
      'off' => t('Don´t shown'),
    ),
  );
  // Make it a system settings form.
  $form  = system_settings_form($form);
  $form['#submit'][] = 'akv_pagepeels_admin_settings_submit';

  return $form;
}

/**
 * Submit function for admin settings
 */
function akv_pagepeels_admin_settings_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    variable_set('akv_type', $form_state['values']['type']);
    variable_set('akv_info', $form_state['values']['info']);
    switch ($form_state['values']['type']) {
      case "latest":
        variable_set('akv_css', '/css/akv_pagepeels_latest.css');
        break;
      case "schaeuble":
        variable_set('akv_css', '/css/akv_pagepeels_schaeuble.css');
        break;
      case "schaeuble_green":
        variable_set('akv_css', '/css/akv_pagepeels_schaeuble_green.css');
        break;
      case "cam":
        variable_set('akv_css', '/css/akv_pagepeels_cam.css');
        break;
      case "3d_cam":
        variable_set('akv_css', '/css/akv_pagepeels_3d_cam.css');
        break;
      case "ani_cam":
        variable_set('akv_css', '/css/akv_pagepeels_ani_cam.css');
        break;
      case "trauer":
        variable_set('akv_css', '/css/akv_pagepeels_trauer.css');
    }
    if ($form_state['values']['info'] == 'on') {
      variable_set('akv_html', '/html/akv_pagepeels.html');
    }
    else {
      variable_set('akv_html', '/html/akv_pagepeels_no_info.html');
    }
    drupal_set_message(
      t('You are now attending at the !Demo, !here you can create a new entry if you want to get listed at the !attending-sites.',
        array(
      '!Demo' => l(t('Online Demo'), 'http://wiki.vorratsdatenspeicherung.de/Online-Demo',
            array('absolute' => TRUE)
          ),
      '!here' => l(t('here'), 'http://wiki.vorratsdatenspeicherung.de/Diskussion:Online-Demo/Teilnehmende_Websites#Neueintrag',
            array('absolute' => TRUE)
          ),
      '!attending-sites' => l(t('attending sites'), 'http://wiki.vorratsdatenspeicherung.de/Online-Demo/Teilnehmende_Websites',
            array('absolute' => TRUE)
          ),
    )
      )
    );
  }
  else {
    variable_set('akv_type', 'latest');
    variable_set('akv_info', 'on');
    variable_set('akv_html', '/html/akv_pagepeels.html');
    variable_set('akv_css', '/css/akv_pagepeels_latest.css');
    drupal_set_message(t('The configuration options have been resetted.'));
  }
}
