<?php

/**
 * @file
 * Administration settings.
 *
 * @todo, Merge edit & add forms.
 */

/**
 * Menu callback for admin settings.
 *
 * @todo, Cleanup SQL statements, split into indie functions.
 */
function tips_settings_form($form, $form_state) {
  $form['groups'] = array(
    '#type' => 'vertical_tabs',
  );

  // Tooltip groups.
  $form['groups']['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Selectors'),
  );
  $form['groups']['form']['add'] = array(
    '#type' => 'submit',
    '#submit' => array('tips_settings_form_tooltip_submit'),
    '#validate' => array('tips_settings_form_tooltip_validate'),
    '#value' => t('Add tooltip'),
    '#ajax' => array(
      'callback' => 'tips_settings_form_tooltip_callback',
      'effect' => 'fade',
    ),
    '#weight' => 10,
  );
  $form['groups']['form']['tips_form_table'] = array(
    '#theme' => 'tips_admin_settings_form',
    '#tree' => TRUE,
  );
  $results = db_select('tips_registry', 'tr')->fields('tr')->orderBy('tip_id')->execute();
  foreach ($results as $key => $result) {    
    $form['groups']['form']['tips_form_table']['tips_form_fields'][$key] = array();
    $form['groups']['form']['tips_form_table']['tips_form_fields'][$key]['selector'] = array(
      '#type' => 'item',
      '#markup' => $result->selector,
    );
    $form['groups']['form']['tips_form_table']['tips_form_fields'][$key]['pages'] = array(
      '#type' => 'item',
      '#markup' => $result->pages,
    );
    $form['groups']['form']['tips_form_table']['tips_form_fields'][$key]['settings_id'] = array(
      '#type' => 'item',
      '#markup' => $result->settings_id,
    );
    $form['groups']['form']['tips_form_table']['tips_form_fields'][$key]['content_id'] = array(
      '#type' => 'item',
      '#markup' => $result->content_id,
    );
    $form['groups']['form']['tips_form_table']['tips_form_fields'][$key]['delete'] = array(
      '#markup' => implode(", ", array(
        l(t('delete'), "admin/config/user-interface/tips/{$result->tip_id}/delete"),
      )),
    );
  }

  $form['groups']['form']['tips_form_table']['tips_new_form_fields'] = array();
  $form['groups']['form']['tips_form_table']['tips_new_form_fields']['selector'] = array(
    '#type' => 'textfield',
    '#description' => t('Please read these instructions if unaware of how to obtain ') . l(t('HTML selector.'), 'http://www.w3schools.com/jquery/jquery_selectors.asp',
      array('attributes' => array('target' => '_blank'))),
  );
  $form['groups']['form']['tips_form_table']['tips_new_form_fields']['pages'] = array(
    '#type' => 'textfield',
    '#size' => 20,
    '#description' => t('Leave blank or type a comma-separated<br/>list of paths, like <em>node/123, admin/*</em>'),
  );
  $setting_ids = array();
  $settings = db_query('SELECT ts.settings_id FROM {tips_settings_registry} AS ts')->fetchAll();
  foreach ($settings as $setting) {
    $setting_ids[$setting->settings_id] = $setting->settings_id;
  }
  $form['groups']['form']['tips_form_table']['tips_new_form_fields']['settings_id'] = array(
    '#type' => 'select',
    '#options' => $setting_ids,
    '#description' => t('Tooltip settings group.'),
  );

  $content_ids = array();
  $contents = db_query('SELECT tc.content_id FROM {tips_content_registry} AS tc')->fetchAll();
  foreach ($contents as $content_id) {
    $content_ids[$content_id->content_id] = $content_id->content_id;
  }
  $form['groups']['form']['tips_form_table']['tips_new_form_fields']['content_id'] = array(
    '#type' => 'select',
    '#options' => $content_ids,
    '#description' => t('Tooltip content group.'),
  );

  // Settings groups.
  $form['groups']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Setting groups'),
  );
  $form['groups']['settings']['actions'] = array(
    '#type' => 'container',
    '#weight' => 10,
    '#prefix' => '<div id="tips-library-form">',
    '#suffix' => '</div>',
  );

  $tips = tips_info();
  $options = array('none' => t('None'));
  if (empty($tips)) {
    drupal_set_message(t('No Tips library plugins found. Have you enabled at least one Tips submodule?'), 'warning');
  }
  else {
    foreach ($tips as $tip) {
      $options[$tip['library']] = $tip['title'];
    }
  }
  $form['groups']['settings']['actions']['library'] = array(
    '#type' => 'select',
    '#title' => t('Library'),
    '#options' => $options,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'tips_add_settings_group_form_callback',
      'wrapper' => 'tips-library-form',
    ),
    '#description' => t('The parent library.'),
  );

  $library = NULL;
  if (isset($form_state['values']['library'])) {
    $library = $form_state['values']['library'];
  }
  $form['groups']['settings']['actions']['submit'] = array(
    '#type' => 'markup',
    '#markup' => l(t('Add settings group'), 'admin/config/user-interface/tips/settings/add/' . $library, array('attributes' => array('class' => 'button'))),
  );

  $rows = array();
  $results = db_select('tips_settings_registry', 'ts')->fields('ts')->orderBy('settings_id')->execute();
  foreach ($results as $result) {
    $rows[$result->settings_id] = array(
      array('data' => $result->settings_id),
      array('data' => $result->library),
      array('data' => l(t('edit'), "admin/config/user-interface/tips/settings/{$result->settings_id}/edit")),
      array('data' => l(t('delete'), "admin/config/user-interface/tips/settings/{$result->settings_id}/delete")),
    );
  }
  $form['groups']['settings']['tips_form_table'] = array(
    '#theme' => 'table',
    '#header' => array(t('settings_id'), t('library'), t('edit'), t('delete')),
    '#rows' => $rows,
    '#empty' => t('Please create a settings group.'),
  );
  
  // Content groups.
  $form['groups']['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content groups'),
  );
  $rows = array();
  $results = db_select('tips_content_registry', 'tc')->fields('tc')->orderBy('content_id')->execute();
  foreach ($results as $result) {
    $rows[$result->content_id] = array(
      array('data' => $result->content_id),
      array('data' => $result->content),
      array('data' => l(t('edit'), "admin/config/user-interface/tips/content/{$result->content_id}/edit")),
      array('data' => l(t('delete'), "admin/config/user-interface/tips/content/{$result->content_id}/delete")),
    );
  }
  $form['groups']['content']['tips_form_table'] = array(
    '#theme' => 'table',
    '#header' => array(t('content_id'), t('Content'), t('edit'), t('delete')),
    '#rows' => $rows,
    '#empty' => t('Please create a content group.'),
  );
  $form['groups']['content']['actions'] = array(
    '#type' => 'container',
    '#weight' => 10,
  );
  $form['groups']['content']['actions']['submit'] = array(
    '#type' => 'markup',
    '#markup' => l(t('Add content group'), 'admin/config/user-interface/tips/content/add', array('attributes' => array('class' => 'button'))),
  );

  return $form;
}

/**
 * AJAX callback, add new tooltip
 */
function tips_settings_form_tooltip_callback($form, $form_state) {
  // Trigger validation message.
  $message = drupal_set_message(t(''));

  if (!empty($form_state['values']['tips_form_table']['tips_new_form_fields']['selector'])) {
    $message = drupal_set_message(t('New tooltip added with @selector.', array('@selector' => $form_state['values']['tips_form_table']['tips_new_form_fields']['selector'])));
  }

  $commands = array();
  // @todo, Insert selector row via AJAX, looks more swish :).
  $commands[] = ajax_command_invoke('div.messages', 'remove', array('.error'));
  $commands[] = ajax_command_prepend('div#page', theme('status_messages', $message));

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * AJAX callback for development_form
 */
function tips_add_settings_group_form_callback($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  return $form['groups']['settings']['actions'];
}

/**
 * Validation handler for new tooltip collection.
 */
function tips_settings_form_tooltip_validate($form, $form_state) {
  if (empty($form_state['values']['tips_form_table']['tips_new_form_fields']['selector'])) {
    form_set_error('selector', t('Please enter a HTML selector.'));
  }
}

/**
 * Submit hanlder for new tooltip collection.
 */
function tips_settings_form_tooltip_submit($form, $form_state) {
  module_load_include('inc', 'tips', 'includes/tips.database');

  tips_add_tooltip_group($form_state['values']['tips_form_table']['tips_new_form_fields']);
}

/**
 * Add new settings group form.
 */
function tips_add_settings_group_form($form, $form_state) {
  $form = drupal_build_form('tips_' . $form_state['build_info']['args'][0] . '_settings_form', $form_state);
  $form['#submit'][] = 'tips_add_settings_group_form_submit';

  $form['actions'] = array(
    '#type' => 'container',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Submit handler for new settings group forms.
 */
function tips_add_settings_group_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/user-interface/tips';
  $settings_id = $form_state['build_info']['args'][0];

  // Cleanup & save new settings instance.
  tips_form_save_instance_settings($settings_id, $form_state, 'input');

  // Alert user.
  drupal_set_message(t('Settings have been saved.'), 'status');
}

/**
 * Edit TIP library settings form.
 */
function tips_form_settings_form($form, $form_state) {
  module_load_include('inc', 'tips', 'includes/tips.database');

  $tips = tips_load_settings($form_state['build_info']['args'][0]);

  if ($tips) {
    // Tip library form.
    $form = drupal_build_form('tips_' . $tips['library'] . '_settings_form', $form_state);
    tips_attach_default_values($form, $tips['settings']);

    // Attach submit handler for all tip settings forms.
    $form['#submit'][] = 'tips_form_setttings_form_submit';

    // @todo, Add back button.
    $form['actions'] = array(
      '#type' => 'container',
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save settings'),
    );
  }

  return $form;
}

/**
 * Submit handler for TIP settings forms.
 */
function tips_form_setttings_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/user-interface/tips';
  $settings_id = $form_state['build_info']['args'][0];

  // Cleanup form attr.
  tips_form_save_instance_settings($settings_id, $form_state, 'input');

  // Alert user.
  drupal_set_message(t('Settings have been updated.'), 'status');
}

/**
 * Edit TIP content group form.
 */
function tips_form_content_form($form, $form_state) {
  module_load_include('inc', 'tips', 'includes/tips.database');

  if (isset($form_state['build_info']['args'][0])) {
    $content = tips_load_content($form_state['build_info']['args'][0]);
  }

  $form['content'] = array(
    '#type' => 'text_format',
    '#default_value' => isset($content) ? $content : '',
    '#format' => 'tips',
  );
  
  if (module_exists('token')) {
    $form['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node', 'term', 'vocabulary', 'user', 'menu-link', 'date', 'file'),
      '#global_types' => TRUE,
      '#click_insert' => TRUE,
      '#suffix' => '<p><em>' . t('Please note, the token must be available on the page implementing the tooltip.') . '</em></p>',
    );
  }
  else {
    $form['tokens'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available token browser.', array('@drupal-token' => 'http://drupal.org/project/token')) . '</p>',
    );
  }
  
  // We don't want the format changed.
  $form['#attached']['css'][] = array(
    'data' => '.filter-wrapper {display:none;}', 
    'type' => 'inline'
  );

  // Attach submit handler for all tip settings forms.
  $form['#submit'][] = 'tips_form_content_form_submit';

  // @todo, Add back button.
  $form['actions'] = array(
    '#type' => 'container',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Tooltip content form submit handler.
 */
function tips_form_content_form_submit($form, &$form_state) {
  module_load_include('inc', 'tips', 'includes/tips.database');
  $content_id = isset($form_state['build_info']['args'][0]) ? $form_state['build_info']['args'][0] : NULL;

  // Save content.
  tips_update_content($content_id, $form_state['values']['content']['value']);

  // Alert user.
  drupal_set_message(t('Content has been updated.'), 'status');
  $form_state['redirect'] = 'admin/config/user-interface/tips';
}

/**
 * Clean form of admin fields & create new or update settings.
 */
function tips_form_save_instance_settings($settings_id, $values, $key) {
  module_load_include('inc', 'tips', 'includes/tips.database');

  $attributes = array('form_build_id', 'form_token', 'form_id', 'op');
  foreach ($attributes as $attr) {
    unset($values[$key][$attr]);
  }

  // Include type cast.
  // @todo, This needs it's own col.
  foreach ($values['complete form'] as $setting => $data) {
    if (isset($data['#value_type']) && (isset($values[$key][$setting]))) {
      $values[$key][$setting] .= '::' . $data['#value_type'];
    }
  }

  if (is_numeric($settings_id)) {
    // Update settings group.
    tips_update_settings($settings_id, $values[$key]);
  }
  else {
    // Create new settings group.
    tips_add_settings_group($settings_id, $values[$key]);
  }
}

/**
 * Return form with values set.
 */
function tips_attach_default_values(&$form, $values) {
  foreach ($form as $key => $form_element) {
    if (isset($form_element['#theme_wrappers'])) {
      if (is_array($form_element['#theme_wrappers']) && in_array('form_element', $form_element['#theme_wrappers'])) {
        // Remove setting type cast.
        $type = substr($values[$key], stripos($values[$key] ,'::'));
        $values[$key] = str_replace($type, '', $values[$key]);

        // Replace.
        $form[$key]['#value'] = $values[$key];
      }
    }
  }
}

/**
 * Menu callback, delete settings group.
 */
function tips_delete_settings_group_form($form, $form_state) {
  if (is_numeric($form_state['build_info']['args'][0])) {
    return confirm_form($form, t('Are you sure you want to delete group id %id?', array(
      '%id' => $form_state['build_info']['args'][0])), 'admin/config/user-interface/tips', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
  }
}

/**
 * Delete settings group submit handler.
 */
function tips_delete_settings_group_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    module_load_include('inc', 'tips', 'includes/tips.database');

    $settings_id = $form_state['build_info']['args'][0];
    tips_delete_settings_group($settings_id);
    // @todo, Clear tips cache.

    drupal_set_message(t('Settings group @settings_id has been deleted', array('@settings_id' => $settings_id)), 'status');
  }
  
  $form_state['redirect'] = 'admin/config/user-interface/tips';
}

/**
 * Menu callback, delete settings group.
 */
function tips_delete_content_group_form($form, $form_state) {
  if (is_numeric($form_state['build_info']['args'][0])) {
    return confirm_form($form, t('Are you sure you want to delete group id %id?', array(
      '%id' => $form_state['build_info']['args'][0])), 'admin/config/user-interface/tips', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
  }
}

/**
 * Delete settings group submit handler.
 */
function tips_delete_content_group_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    module_load_include('inc', 'tips', 'includes/tips.database');

    $content_id = $form_state['build_info']['args'][0];
    tips_delete_content_group($content_id);
    // @todo, Clear tips cache.

    drupal_set_message(t('Content group @content_id has been deleted', array('@content_id' => $content_id)), 'status');
  }

  $form_state['redirect'] = 'admin/config/user-interface/tips';
}

/**
 * Menu callback, delete selector.
 */
function tips_delete_selector_form($form, $form_state) {
  if (is_numeric($form_state['build_info']['args'][0])) {
    return confirm_form($form, t('Are you sure you want to delete group id %id?', array(
      '%id' => $form_state['build_info']['args'][0])), 'admin/config/user-interface/tips/settings', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
  }
}

/**
 * Delete selector submit handler.
 */
function tips_delete_selector_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    module_load_include('inc', 'tips', 'includes/tips.database');

    $tip_id = $form_state['build_info']['args'][0];
    tips_delete_selector($tip_id);
    // @todo, Clear tips cache.

    drupal_set_message(t('Selector @selector has been deleted', array('@selector' => $tip_id)), 'status');
  }

  $form_state['redirect'] = 'admin/config/user-interface/tips/settings';
}

/**
 * Theme form fields settings table.
 */
function theme_tips_admin_settings_form_fields($variables) {
  $rows = array();
  $form = $variables['form'];
  $header = array(t('ID'), t('Field Name'), t('Tooltip'), t('Operations'));

  foreach (element_children($form['tips_form_fields']) as $key) {
    $row = array();
    $row[] = drupal_render($form['tips_form_fields'][$key]['id']);
    $row[] = drupal_render($form['tips_form_fields'][$key]['field_name']);
    $row[] = drupal_render($form['tips_form_fields'][$key]['body']);
    $row[] = drupal_render($form['tips_form_fields'][$key]['operations']);
    $rows[] = $row;
  }
  $row = array();
  $row[] = drupal_render($form['tips_new_form_fields']['id']);
  $row[] = drupal_render($form['tips_new_form_fields']['field_name']);
  $row[] = drupal_render($form['tips_new_form_fields']['body']);
  $row[] = '';
  $rows[] = $row;

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Theme form settings table.
 */
function theme_tips_admin_settings_form($variables) {
  $rows = array();
  $form = $variables['form'];
  $header = array(t('Selector'), t('Pages'), t('Settings ID'), t('Content ID'), t('Delete'));

  if (!empty($form['tips_form_fields'])) {
    foreach (element_children($form['tips_form_fields']) as $key) {
      $row = array();
      $row[] = drupal_render($form['tips_form_fields'][$key]['selector']);
      $row[] = drupal_render($form['tips_form_fields'][$key]['pages']);
      $row[] = drupal_render($form['tips_form_fields'][$key]['settings_id']);
      $row[] = drupal_render($form['tips_form_fields'][$key]['content_id']);
      $row[] = drupal_render($form['tips_form_fields'][$key]['delete']);
      $rows[] = $row;
    }
  }
  $row = array();
  $row[] = drupal_render($form['tips_new_form_fields']['selector']);
  $row[] = drupal_render($form['tips_new_form_fields']['pages']);
  $row[] = drupal_render($form['tips_new_form_fields']['settings_id']);
  $row[] = drupal_render($form['tips_new_form_fields']['content_id']);
  $row[] = '';
  $rows[] = $row;

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}
