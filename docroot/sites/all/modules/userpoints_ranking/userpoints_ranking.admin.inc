<?php 

/**
 * @file
 * Administrative functions to configure userpoints ranking.
 */

/**
 * Callback for admin/config/mass_messaging/configuration.
 */
function userpoints_ranking_settings_form($form, &$form_state) {
  
  $form = array();
    

  $form['userpoints_ranking_cron_percentage'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron Percentage'),
    '#default_value' => variable_get('userpoints_ranking_cron_percentage', USERPOINTS_RANKING_DEFAULT_CRON_PERCENTAGE),
    '#description' => t('The cron percentage defines the percentage of the available time to be used for calculating the ranking. Default 80, so other application get also a chance to process anything.'),
  );

  $form['userpoints_ranking_cron_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron Batch Size'),
    '#default_value' => variable_get('userpoints_ranking_cron_batch_size', USERPOINTS_RANKING_DEFAULT_CRON_BATCH_SIZE),
    '#description' => t('The size of a batch during the cron. The size indicates how many users total points are calculated during one batch.'),
  );
  
  if (module_exists('views')) {
    $views_options = array('' => t('All Users'));
    $views_options += userpoints_ranking_view_list();
  }
  
  $categories = array('total' => t('Settings over all categories'));
  
  // Categories will only appear if the taxonomy module is enabled as
  // the module is required for this functionality but not necessarily
  // a requirement for the module.
  if (module_exists('taxonomy')) {
    $categories += userpoints_get_categories();
  }
  
  foreach ($categories as $tid => $name) {
    if ($tid === 'total') {
      $title = $name;
    }
    else {
      $title = t('Settings for Category %category', array('%category' => $name));
    }
    
    $form[$tid] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => $title,
    );
    
    $form[$tid]['userpoints_ranking_tid__' . $tid . '__length'] = array(
      '#type' => 'textfield',
      '#title' => t('Length of the period'),
      '#default_value' => variable_get('userpoints_ranking_tid__' . $tid . '__length', '24'),
      '#description' => t('Define here the length of the period in hours. For each period a ranking is calculated.'),
    );
    $form[$tid]['userpoints_ranking_tid__' . $tid . '__start_on'] = array(
      '#type' => 'textfield',
      '#title' => t('Period Start On'),
      '#default_value' => variable_get('userpoints_ranking_tid__' . $tid . '__start_on', '0'),
      '#description' => t('Define here date when the periods should start. You could use this also reset the interval start / ends. 0 means that the next interval starts on the next cron run. Format: %time', array('%time' => date_format(date_create(), 'Y-m-d H:i:s'))),
    );
    
    if (module_exists('views')) {
      $form[$tid]['userpoints_ranking_tid__' . $tid . '__view'] = array(
        '#type' => 'select',
        '#title' => t('User Base'),
        '#options' => $views_options,
        '#default_value' => variable_get('userpoints_ranking_tid__' . $tid . '__view', '0'),
        '#description' => t('Define here a view, which defines the user base (subset of all users) on which the ranking should be calculated on. You need to add a special display to the view, to be visible in here. Per default the ranking is done on all users.'),
      );
    }
  }
    
  return system_settings_form($form);
}

function userpoints_ranking_settings_form_validate($form, &$form_state) {
  
  foreach($form_state['values'] as $key => $value) {
    if (strstr($key, '__start_on') && !empty($form_state['values'][$key])) {
      $form_state['values'][$key] = date_format(date_create($form_state['values'][$key]), 'Y-m-d H:i:s');
    }
  }
  
  
}



