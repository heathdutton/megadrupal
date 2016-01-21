<?php

/**
 * Menu callback: content administration.
 */
function inlinemanual_admin_list($form, $form_state) {
  $form['admin'] = inlinemanual_admin_topics();
  return $form;
}

/**
 * Form builder: Builds the node administration overview.
 */
function inlinemanual_admin_topics() {
  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'title'),
    'description' => t('Description'),
    'status' => array('data' => t('Status'), 'field' => 'status'),
    'version' => array('data' => t('Updated'), 'field' => 'version'),
    'operations' => t('Operations'),
    'weight' => array('data' => t('Weight'), 'field' => 'weight', 'sort' => 'asc'),
  );

  $query = db_select('inm_topics', 't')->extend('PagerDefault')->extend('TableSort');

  $topics = $query
    ->fields('t',array('tid', 'title', 'description', 'status', 'version', 'weight'))
    ->limit(50)
    ->orderByHeader($header)
    ->execute()
    ->fetchAll();
  
  // Prepare the list of topics.
  $options = array();
  $selection = array();
  foreach ($topics as $topic) {
    $operations = array(
      l(t('edit permissions'), 'admin/config/services/inlinemanual/topic/'. $topic->tid .'/permissions'),
      l(t('remove'), 'admin/config/services/inlinemanual/topic/'. $topic->tid .'/remove'),
    );
    $options[$topic->tid] = array(
      'title' => check_plain($topic->title),
      'description' => check_plain($topic->description),
      'status' => $topic->status ? t('enabled') : t('disabled'),
      'version' => $topic->version,
      'weight' => $topic->weight,
      'operations' => implode(' | ', $operations), 
    );
    $selection[$topic->tid] = $topic->status;
  }
  
  // Only use a tableselect when the current user is able to perform any
  // operations.
  $form['topics'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content available.'),
    '#default_value' => $selection,
  );

  $form['pager'] = array('#markup' => theme('pager'));
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('inlinemanual_admin_topics_submit'),
  );
  
  return $form;
}

function inlinemanual_admin_topics_submit($form, &$form_state) {
  $topics = $form_state['values']['topics'];
  
  // Enable checked topics 
  $enable = array_filter($topics);
  if (!empty($enable)) {
    inlinemanual_topics_update_status(1, array_keys($enable));
  }
  
  // Disable unchecked topics
  $disable = array_diff_assoc($topics, $enable);
  if (!empty($disable)) {
    inlinemanual_topics_update_status(0, array_keys($disable));
  }
  
  drupal_set_message(t('Topic updated.'));  
}

/**
 * Page callback for "Refresh topics"
 */
function inlinemanual_admin_topics_refresh() {
  if (inlinemanual_topics_fetch_all()) {
    drupal_set_message(t('Topics were successfully refreshed.'));
  }
  else {
    drupal_set_message(t('Topics refresh failed. Please try again or see the <a href="@url">last reports</a> to get more info.', array('@url' => url('admin/reports/dblog'))), 'error');
  }
  
  drupal_goto('admin/config/services/inlinemanual');
} 

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function inlinemanual_admin_settings_form($form_state) {
  $form['inlinemanual_site_key'] = array(
    '#title' => t('Site API Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('inlinemanual_site_key', ''),
    '#size' => 60,
    '#maxlength' => 120,
    '#required' => TRUE,
    '#description' => t('This ID is unique to each site and can be found on inline manual portal.'),
  );

  $form['inlinemanual_widget_title'] = array(
    '#title' => t('Widget title'),
    '#type' => 'textfield',
    '#default_value' => variable_get('inlinemanual_widget_title', 'Get support'),
    '#size' => 60,
    '#maxlength' => 120,
    '#required' => FALSE,
    '#description' => t('The title of the widget that end-users see.'),
  );

  $form['inlinemanual_widget_footer'] = array(
    '#title' => t('Widget footer'),
    '#type' => 'textfield',
    '#default_value' => variable_get('inlinemanual_widget_footer', ''),
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => FALSE,
    '#description' => t('Set your own footer of the widget. Only text with an anchor tag is allowed.'),
  );

  $positions = array(
    'left' => 'Left',
    'left_top' => 'Left Top',
    'left_bottom' => 'Left Bottom',
    'right' => 'Right',
    'right_top' => 'Right Top',
    'right_bottom' => 'Right Bottom',
    'top' => 'Top',
    'top_left' => 'Top Left',
    'top_right' => 'Top Right',
    'bottom' => 'Bottom',
    'bottom_left' => 'Bottom Left',
    'bottom_right' => 'Bottom Right'
  );

  $form['inlinemanual_widget_position'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#options' => $positions,
       '#default_value' => variable_get('inlinemanual_widget_position', 'bottom_right'),
       '#description' => t('Position of the widget on the screen.'),
   );

  $form['inlinemanual_widget_z_index'] = array(
    '#title' => t('Widget z-index'),
    '#type' => 'textfield',
    '#default_value' => variable_get('inlinemanual_widget_z_index', 1000),
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => FALSE,
    '#description' => t('If the player is covered by other elements on the page, you can bring it to top by setting the z-index to higher value. Default is 1000.'),
  );

  // add option to change the colour of the widget
  drupal_add_css('misc/farbtastic/farbtastic.css');
  drupal_add_js('misc/farbtastic/farbtastic.js');

  $form ['inlinemanual_widget_color'] = array(
   '#type' => 'textfield',
   '#title' => t('Widget Color'),
   '#default_value' => variable_get('inlinemanual_widget_color', '#222222'),
   '#description' => '<div id="inlinemanual_widget_colorpicker"></div>',
   '#size' => 9,
   '#maxlength' => 7,
   '#suffix' => "<script type='text/javascript'>
     jQuery(document).ready(function() {
       jQuery('#inlinemanual_widget_colorpicker').farbtastic('#edit-inlinemanual-widget-color');
     });
   </script>"
  );
  $form ['inlinemanual_widget_text_color'] = array(
   '#type' => 'textfield',
   '#title' => t('Widget Text Color'),
   '#default_value' => variable_get('inlinemanual_widget_text_color', '#ffffff'),
   '#description' => '<div id="inlinemanual_widget_text_colorpicker"></div>',
   '#size' => 9,
   '#maxlength' => 7,
   '#suffix' => "<script type='text/javascript'>
     jQuery(document).ready(function() {
       jQuery('#inlinemanual_widget_text_colorpicker').farbtastic('#edit-inlinemanual-widget-text-color');
     });
   </script>"
  );
  return system_settings_form($form);
}

/**
 * Page callback for topic permissions editing 
 */
function inlinemanual_admin_topic_permissions_form($form, &$form_state, $tid) {
  // Set breadcrumbs
  drupal_set_breadcrumb(array(l(t('Home'), NULL), l(t('Administration'), 'admin'), l(t('Configuration'), 'admin/config'), l(t('Web services'), 'admin/config/services'), l(t('Inline Manual'), 'admin/config/services/inlinemanual')));

  // Retrieve role names for columns.
  $role_names = user_roles();

  // Fetch permissions for all roles or the one selected role.
  $role_permissions = user_role_permissions($role_names);
  
  // Store $role_names for use when saving the data.
  $form['role_names'] = array(
    '#type' => 'value', 
    '#value' => $role_names,
  );
  
  $options = array();
  $hide_descriptions = system_admin_compact_mode();
  
  if ($permissions = inlinemanual_permission()) {
    foreach ($permissions as $perm => $perm_item) {
      if ($perm == 'view inlinemanual_topic_'. $tid) {
        // Fill in default values for the permission.
        $perm_item += array(
          'description' => '', 
          'restrict access' => FALSE, 
          'warning' => !empty($perm_item['restrict access']) ? t('Warning: Give to trusted roles only; this permission has security implications.') : '',
        );
        $options[$perm] = '';
        $form['permission'][$perm] = array(
          '#type' => 'item', 
          '#markup' => $perm_item['title'], 
          '#description' => theme('user_permission_description', array('permission_item' => $perm_item, 'hide' => $hide_descriptions)),
        );
        foreach ($role_names as $rid => $name) {
          // Builds arrays for checked boxes for each role
          if (isset($role_permissions[$rid][$perm])) {
            $status[$rid][] = $perm;
          }
        }
      }
    }
  }
  
  // Have to build checkboxes here after checkbox arrays are built
  foreach ($role_names as $rid => $name) {
    $form['checkboxes'][$rid] = array(
      '#type' => 'checkboxes', 
      '#options' => $options, 
      '#default_value' => isset($status[$rid]) ? $status[$rid] : array(), 
      '#attributes' => array('class' => array('rid-' . $rid)),
    );
    $form['role_names'][$rid] = array(
      '#markup' => check_plain($name),
      '#tree' => TRUE,
    );
  }
  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save permissions'),
  );

  $form['#attached']['js'][] = drupal_get_path('module', 'user') . '/user.permissions.js';
  
  return $form;
}

function inlinemanual_admin_topic_permissions_form_submit($form, &$form_state) {
  module_load_include('inc', 'user', 'user.admin');
  user_admin_permissions_submit($form, $form_state);
}

function theme_inlinemanual_admin_topic_permissions_form($variables) {
  if (isset($variables['form']['permission']) && count($variables['form']['permission'])) {
    return theme('user_admin_permissions', $variables);
  }
  else {
    return t('There are no topics associated with this ID.');
  }
}

function inlinemanual_admin_topic_delete_confirm_form($form, $form_state, $tid) {
  return confirm_form(
    array(
      'tid' => array('#type' => 'hidden', '#value' => $tid)
    ),
    t('Are you sure you want to remove the topic?'),
    'admin/config/services/inlinemanual',
    t('The topic will be removed from yours website database, not from the Inline Manual portal. Refetching topics will add the topic again if it has been assigned to the site.')
  );
}

function inlinemanual_admin_topic_delete_confirm_form_submit($form, &$form_state) {
  $tid = $form_state['values']['tid'];
  $tid_deleted = db_delete('inm_topics')->condition('tid', $tid)->execute();
  drupal_set_message(t('The topic has been removed from your database.'));
  $form_state['redirect'] = 'admin/config/services/inlinemanual';
}
