<?php 
/**
 * @file
 * Admin include file.
 */

/**
 * Displays the form for the Basic Settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function pinned_site_admin_basic_settings() {
  // @todo: Add possibility to change pinned site application icon.
  
  $form['pinned_site_application_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Application Name'),
    '#description' => t('This field defines the name of the Pinned Site application instance. This is the name that appears in a tooltip when hovering over the Pinned Site button on the Windows 7 taskbar. The application name is also appended to the window title of the Pinned Site application instance. Leave the field empty to use the document title instead.') . '<br/>' .
      t('<i>Note: HTML tags and double quotes are not supported.</i>'),
    '#default_value' => variable_get('pinned_site_application_name', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );
  
  $form['pinned_site_msapplication_tooltip'] = array(
    '#type' => 'textfield',
    '#title' => t('Application Tooltip'),
    '#description' => t('This field provides additional tooltip text that appears when you hover over the Pinned Site shortcut in the Windows Start menu or on the desktop.') . '<br/>' .
      t('<i>Note: HTML tags and double quotes are not supported.</i>'),
    '#default_value' => variable_get('pinned_site_msapplication_tooltip', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );
  
  $form['pinned_site_msapplication_starturl'] = array(
    '#type' => 'textfield',
    '#title' => t('Application Start URL'),
    '#description' => t('This field contains the root URL of the application. The start URL can be fully qualified, or relative to the current document. Only HTTP and HTTPS protocols are allowed. Leave the field empty to use the address of the browsed page instead.'),
    '#default_value' => variable_get('pinned_site_msapplication_starturl', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );
  
  $form['pinned_site_msapplication_navbutton_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Navigation Buttons Color'),
    '#description' => t('This field define the custom color of the Back and Forward buttons in the Pinned Site browser window. Please use the color picker below to set a color. If this field is empty, the default color is based on the color palette of the site icon (or, favicon).'),
    '#default_value' => variable_get('pinned_site_msapplication_navbutton_color', '#000000'),
    '#size' => 7,
    '#maxlength' => 7,
    '#suffix' => '<div class="pinned_site_colorpicker" data-target="pinned_site_msapplication_navbutton_color"></div>',
    '#attached' => array(
      // Add Farbtastic color picker.
      'library' => array(
        array('system', 'farbtastic'),
      ),
      'js' => array(
        drupal_get_path('module', 'pinned_site') . '/js/admin.js' =>
        array('type' => 'file'),
      ),
    )
  );
  
  $form['window'] = array(
    '#type' => 'fieldset',
    '#title' => t('Application Window Size'),
    '#description' => t('These fields set the initial size of the Pinned Site window when it is launched for the first time. However, if the user adjusts the size of the window, the Pinned Site retains the new dimensions when it is launched again.'),
    '#collapsible' => FALSE,
  );
  
  $form['window']['pinned_site_msapplication_window_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('The window width in pixels. The minimum value is 800.'),
    '#default_value' => variable_get('pinned_site_msapplication_window_width', ''),
    '#size' => 4,
    '#maxlength' => 4,
  );
  
  $form['window']['pinned_site_msapplication_window_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('The window height in pixels. The minimum value is 600.'),
    '#default_value' => variable_get('pinned_site_msapplication_window_height', ''),
    '#size' => 4,
    '#maxlength' => 4,
  );
  
  $form['pinned_site_use_browserconfig_xml'] = array(
    '#type' => 'select',
    '#title' => t('Browserconfig XML'),
    '#description' => t('Provide an browserconfig.xml with the provided configuration.') . '<br/>' .
      t('IE on Windows 8.1 will load the configuration from this file instead of the metatags. Only using this file will prevent functionality on Versions prior Windows 8.1.'),
    '#options' => array(
      'none' => t('No browserconfig.xml'),
      'xml_with_meta' => t('Use browserconfig.xml and metatags'),
      'xml_without_meta' => t('Use browserconfig.xml only'),
    ),
    '#default_value' => variable_get('pinned_site_use_browserconfig_xml', 'none'),
  );
  
  return system_settings_form($form);
}

/**
 * Displays the form for the Start Tile Settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function pinned_site_admin_start_tile_settings() {

  $form['pinned_site_msapplication_tilecolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Tile Color'),
    '#description' => t('Sets the background color for a live tile.'),
    '#default_value' => variable_get('pinned_site_msapplication_tilecolor', '#ffffff'),
    '#size' => 7,
    '#maxlength' => 7,
    '#suffix' => '<div class="pinned_site_colorpicker" data-target="pinned_site_msapplication_tilecolor"></div>',
    '#attached' => array(
      // Add Farbtastic color picker.
      'library' => array(
        array('system', 'farbtastic'),
      ),
      'js' => array(
        drupal_get_path('module', 'pinned_site') . '/js/admin.js' =>
        array('type' => 'file'),
      ),
    ),
  );

  $form['pinned_site_msapplication_tileimage'] = array(
    '#type' => 'textfield',
    '#title' => t('Tile Image'),
    '#description' => t('Specifies the background image for live tile.') . ' ' .
      t('A fully qualified or relative URI.') . '<br/>' .
      t('Recomended size: %size pixels; Format: %format', array('%size' => '144x144', '%format' => '.PNG')) . '<br/>' .
      t('For example: %example_icon',
        array('%example_icon' => base_path() . path_to_theme() . '/windows_live_tile_144x144.png')),
    '#default_value' => variable_get('pinned_site_msapplication_tileimage', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['pinned_site_msapplication_tile_square70x70logo'] = array(
    '#type' => 'textfield',
    '#title' => t('Tile Image (square, 70x70)'),
    '#description' => t('Specifies the background image for live tile.') . ' ' .
      t('A fully qualified or relative URI.') . '<br/>' .
      t('Recomended size: %size pixels; Format: %format', array('%size' => '70x70', '%format' => '.PNG')) . '<br/>' .
      t('For example: %example_icon',
        array('%example_icon' => base_path() . path_to_theme() . '/windows_live_tile_70x70.png')),
    '#default_value' => variable_get('pinned_site_msapplication_tile_square70x70logo', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['pinned_site_msapplication_tile_square150x150logo'] = array(
    '#type' => 'textfield',
    '#title' => t('Tile Image (square, 150x150)'),
    '#description' => t('Specifies the background image for live tile.') . ' ' .
      t('A fully qualified or relative URI.') . '<br/>' .
      t('Recomended size: %size pixels; Format: %format', array('%size' => '150x150', '%format' => '.PNG')) . '<br/>' .
      t('For example: %example_icon',
        array('%example_icon' => base_path() . path_to_theme() . '/windows_live_tile_150x150.png')),
    '#default_value' => variable_get('pinned_site_msapplication_tile_square150x150logo', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['pinned_site_msapplication_tile_wide310x150logo'] = array(
    '#type' => 'textfield',
    '#title' => t('Tile Image (wide, 310x150)'),
    '#description' => t('Specifies the background image for live tile.') . ' ' .
      t('A fully qualified or relative URI.') . '<br/>' .
      t('Recomended size: %size pixels; Format: %format', array('%size' => '310x150', '%format' => '.PNG')) . '<br/>' .
      t('For example: %example_icon',
        array('%example_icon' => base_path() . path_to_theme() . '/windows_live_tile_310x150.png')),
    '#default_value' => variable_get('pinned_site_msapplication_tile_wide310x150logo', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['pinned_site_msapplication_tile_square310x310logo'] = array(
    '#type' => 'textfield',
    '#title' => t('Tile Image (square, 310x310)'),
    '#description' => t('Specifies the background image for live tile.') . ' ' .
      t('A fully qualified or relative URI.') . '<br/>' .
      t('Recomended size: %size pixels; Format: %format', array('%size' => '310x310', '%format' => '.PNG')) . '<br/>' .
      t('For example: %example_icon',
        array('%example_icon' => base_path() . path_to_theme() . '/windows_live_tile_310x310.png')),
    '#default_value' => variable_get('pinned_site_msapplication_tile_square310x310logo', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['tileimage'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload Image'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );

  $form['tileimage']['image_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload Tile Image'),
    '#maxlength' => 40,
    '#description' => t("If you don't have direct file access to the server, use this field to upload your image."),
  );

  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['#validate'] = array('pinned_site_admin_tileimage_upload');
  $form['#submit'] = array('pinned_site_admin_start_tile_settings_submit');

  return system_settings_form($form);
}

/**
 * Form submit callback for pinned_site_admin_start_tile_settings.
 * @param array $form
 * @param array $form_state
 */
function pinned_site_admin_start_tile_settings_submit(&$form, &$form_state) {
  if (!empty($form_state['values']['image_upload'])) {
    $form_state['values']['pinned_site_msapplication_tileimage'] = $form_state['values']['image_upload'];
  }
}

/**
 * Validation callback; Uploads tile image.
 * @param array $form
 * @param array $form_state
 */
function pinned_site_admin_tileimage_upload(&$form, &$form_state) {

  $validators = array('file_validate_extensions' => array('png'));
  if ($file = file_save_upload('image_upload', $validators)) {

    $destination = file_build_uri($file->filename);

    if (file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
      $absolute_url = file_create_url($destination);
      $url_parts = parse_url($absolute_url);

      $form_state['values']['image_upload'] = $url_parts['path'];
    }
    else {
      form_set_error('image_upload', t("Failed to upload the image."));
    }
  }
}

/**
 * Theme function to render static tasks table with drag & drop ordering by weight.
 * @param array $variables
 * @return string - rendered static tasks table
 */
function theme_pinned_site_admin_static_tasks_settings($variables) {
  $form = $variables['form'];

  $output = drupal_render($form['add_new']);
  
  $rows = array();

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['name'])) {
      $task = &$form[$key];

      $row = array();
      $row[] = drupal_render($task['name']);
      if (isset($task['weight'])) {
        $task['weight']['#attributes']['class'] = array('custom-tasks-weight');
        $row[] = drupal_render($task['weight']);
      }
      $row[] = drupal_render($task['icon']);
      $row[] = drupal_render($task['edit']);
      $row[] = drupal_render($task['delete']);
      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
  }

  $header = array(t('Task'));
  if (isset($form['actions'])) {
    $header[] = t('Weight');
    drupal_add_tabledrag('custom-tasks', 'order', 'sibling', 'custom-tasks-weight');
  }
  $header[] = t('Icon');
  
  $header[] = array('data' => t('Operations'), 'colspan' => '2');
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('There are no tasks yet. !add_new_link', array('!add_new_link' => l(t('Add a static task'), 'admin/config/system/pinned_site/static_tasks/add'))), 'attributes' => array('id' => 'custom-tasks')));
  $output .= drupal_render_children($form);
  
  return $output;
}

/**
 * Overview of static tasks.
 * @param array $form
 * @param array $form_state
 */
function pinned_site_admin_static_tasks_settings($form, &$form_state) {
  
  $tasks = pinned_site_get_static_tasks(PINNED_SITE_STATIC_TASKS_MAX_QUANTITY);
  
  // Check quantity of existing tasks; don't allow add more than PINNED_SITE_STATIC_TASKS_MAX_QUANTITY tasks.
  $form['add_new'] = array('#type' => 'item');
  if (count($tasks) == PINNED_SITE_STATIC_TASKS_MAX_QUANTITY) {
    $form['add_new']['#markup'] = t('You have reached the limit of available task slots. You should delete one of the existing tasks to add another.');
  } 
  else {
    $form['add_new']['#markup'] = l(t('Add a static task'), 'admin/config/system/pinned_site/static_tasks/add');
  }
  
  $form['#tree'] = TRUE;
  foreach ($tasks as $id => $task) {
    $icon = '<img src="' . check_url($task['icon_uri']) . '" />';
    $form[$id]['name'] = array('#markup' => pinned_site_prepare_meta_content($task['name']));
    $form[$id]['icon'] = array('#type' => 'item', '#markup' => $icon);
    $form[$id]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $task['name'])),
      '#title_display' => 'invisible',
      '#delta' => 10,
      '#default_value' => $task['weight'],
    );
    $form[$id]['edit'] = array('#type' => 'link', '#title' => t('edit'), '#href' => 'admin/config/system/pinned_site/static_tasks/' . $task['id'] . '/edit');
    $form[$id]['delete'] = array('#type' => 'link', '#title' => t('delete'), '#href' => 'admin/config/system/pinned_site/static_tasks/' . $task['id'] . '/delete');
  }

  if (count($tasks) > 1) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  }
  elseif (isset($task)) {
    unset($form[$id]['weight']);
  }

  return $form;
}

/**
 * Process custom tasks configuration form submission.
 * @param array $form
 * @param array $form_state
 */
function pinned_site_admin_static_tasks_settings_submit($form, &$form_state) {
  
  foreach ($form_state['values'] as $id => $task) {
    if (!is_numeric($id)) {
      continue;
    }
    
    db_update('pinned_site_tasks')
      ->fields(array('weight' => $task['weight']))
      ->condition('id', $id)
      ->execute();
  }
  drupal_set_message(t('The custom tasks ordering has been saved.'));
}

/**
 * Confirmation form for deletion of static task.
 * @param array $form
 * @param array $form_state
 * @param int $id
 */
function pinned_site_admin_static_task_delete($form, $form_state, $id) {
  $task = pinned_site_get_static_task($id);
  
  $form = array();
  
  $form['#task'] = $task;
  
  return 
    confirm_form($form, 
      t('Are you sure you want to delete the task %task?', 
      array('%task' => $task['name'])), 
      'admin/config/system/pinned_site/static_tasks', 
      t('This action cannot be undone.'), 
      t('Delete'), 
      t('Cancel')
    );
}

/**
 * Form submit handler for 'Delete' button on static task delete confirmation form.
 * @param array $form
 * @param array $form_state
 */
function pinned_site_admin_static_task_delete_submit($form, &$form_state) {
  $task = $form['#task'];
  
  db_delete('pinned_site_tasks')
    ->condition('id', $task['id'])
    ->execute();
  
  drupal_set_message(t('Task %task has been deleted.', array('%task' => $task['name'])));
  
  $form_state['redirect'] = 'admin/config/system/pinned_site/static_tasks';
}

/**
 * The static task edit form.
 * @param array $form
 * @param array $form_state
 * @param int $id
 */
function pinned_site_admin_static_task_edit($form, $form_state, $id = NULL) {
 
  // Load the existing task if user wants to edit it.
  if ($id) {
    $task = pinned_site_get_static_task($id);
  }
 
  $form = array();
  
  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $id
  );
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Task Name'),
    '#description' => t('The task name that appears in the Jump List.') . '<br/>' . t('<i>Note: HTML tags and double quotes are not supported.</i>'),
    '#default_value' => isset($task['name']) ? $task['name'] : '',
    '#size' => 60,
    '#maxlength' => PINNED_SITE_STATIC_TASK_NAME_MAX_LENGTH,
    '#required' => TRUE
  );
  
  $form['action_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Action URI'),
    '#description' => t('The address that is launched when the item is clicked.') . ' ' . 
      t('A fully qualified or relative URI.'),
    '#default_value' => isset($task['action_uri']) ? $task['action_uri'] : '',
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE
  );

  $form['icon_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon URI'),
    '#description' => t('The icon resource that appears next to the task in the Jump List.') . ' ' . 
      t('A fully qualified or relative URI.') . '<br/>' .
      t('Recomended size: %size pixels; Format: %format', array('%size' => '16x16', '%format' => '.ICO')) . '<br/>' .
      t('For example: %example_icon', 
        array('%example_icon' => base_path() . path_to_theme() . '/favicon.ico')),
    '#default_value' => isset($task['icon_uri']) ? $task['icon_uri'] : '',
    '#size' => 60,
    '#maxlength' => 255,
  );
  
  $form['icon'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload Icon'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE
  );
  $form['icon']['icon_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload icon image'),
    '#maxlength' => 40,
    '#description' => t("If you don't have direct file access to the server, use this field to upload your icon.")
  );
  
  $options = array(
    'tab' => 'tab',
    'self' => 'self',
    'window' => 'window'
  );
  $form['window_type'] = array(
    '#type' => 'select',
    '#title' => t('Window Type'),
    '#description' => t('tab (default) — a new tab in the current window; self — the current tab; window — a new Pinned Site window'),
    '#default_value' => isset($task['window_type']) ? $task['window_type'] : 'tag',
    '#options' => $options,
  );
  
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  // If user wants to edit existing task, give him possibility to delete it.
  if (!empty($id)) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }
  
  $form['#attributes']['enctype'] = 'multipart/form-data';
  
  $form['#validate'] = array('pinned_site_admin_icon_upload');
  
  return $form;
}

/**
 * Form submit handler for 'Save' and 'Delete' button on static task add/edit form.
 * @param array $form
 * @param array $form_state
 */
function pinned_site_admin_static_task_edit_submit($form, &$form_state) {
  $task = $form_state['values'];
  
  if ($task['op'] == t('Delete')) {
    $form_state['redirect'] = 'admin/config/system/pinned_site/static_tasks/' . $task['id'] . '/delete';
    return;
  }
  
  // Replace old icon URI with URI of uploaded icon.
  if (!empty($task['icon_upload'])) {
    $task['icon_uri'] = $task['icon_upload'];
  }

  db_merge('pinned_site_tasks')
    ->key(array('id' => $task['id']))
    ->fields(array(
      'name' => $task['name'],
      'action_uri' => $task['action_uri'],
      'icon_uri' => $task['icon_uri'],
      'window_type' => $task['window_type']
    ))
    ->execute();
  
  drupal_set_message(t('Task %task has been saved.', array('%task' => pinned_site_prepare_meta_content($task['name']))));
  
  $form_state['redirect'] = 'admin/config/system/pinned_site/static_tasks';
}

/**
 * Displays the form for the Recent Posts tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function pinned_site_admin_recent_posts_settings() {
 
  $form = array();
  
  $form['pinned_site_recent_posts_jump_list_category_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Jump List Category Name'),
    '#description' => t('The name of the Jump List Category for grouping of recent posts.') . '<br/>' . t('<i>Note: HTML tags are not supported.</i>'),
    '#default_value' => variable_get('pinned_site_recent_posts_jump_list_category_name', ''),
    '#size' => 60,
    '#maxlength' => 255
  );
  
  $node_types = array();
  foreach (node_type_get_types() as $key => $type) {
    $node_types[$key] = $type->name;
  }
  
  $form['pinned_site_recent_posts_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content Types'),
    '#default_value' => variable_get('pinned_site_recent_posts_node_types', array()),
    '#options' => $node_types,
    '#description' => t('Select the content types available for displaying in this Jump List Category. You should select at least one content type to turn on the displaying of recent posts.')
  );
  
  $options = array();
  for ($i = 1; $i <= PINNED_SITE_JUMP_LIST_ITEMS_MAX_QUANTITY; $i++) {
    $options[$i] = $i;
  }
 
  $form['pinned_site_recent_posts_count'] = array(
    '#type' => 'select',
    '#title' => t('Quantity'),
    '#description' => t("Indicates how many recent posts will be shown."),
    '#options' => $options,
    '#default_value' => variable_get('pinned_site_recent_posts_count', 5),
  );
  
  $form['pinned_site_recent_posts_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon URI'),
    '#description' => t('The icon resource that appears next to the task in the Jump List.') . ' ' . 
      t('A fully qualified or relative URI.') . '<br/>' .
      t('Recomended size: %size pixels; Format: %format', array('%size' => '16x16', '%format' => '.ICO')) . '<br/>' .
      t('For example: %example_icon', 
        array('%example_icon' => base_path() . path_to_theme() . '/favicon.ico')),
       '#default_value' => variable_get('pinned_site_recent_posts_icon', ''),
    '#size' => 60,
    '#maxlength' => 255
  );
  
  $form['icon'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload Icon'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE
  );

  $form['icon']['icon_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload icon image'),
    '#maxlength' => 40,
    '#description' => t("If you don't have direct file access to the server, use this field to upload your icon.")
  );
  
  $form['#attributes']['enctype'] = 'multipart/form-data';
  
  $form['#validate'] = array('pinned_site_admin_icon_upload');
  $form['#submit'] = array('pinned_site_admin_recent_posts_settings_submit');
  
  return system_settings_form($form);
}

/**
 * Form submit callback for pinned_site_admin_recent_posts_settings.
 * @param array $form
 * @param array $form_state
 */
function pinned_site_admin_recent_posts_settings_submit(&$form, &$form_state) {
  if (!empty($form_state['values']['icon_upload'])) {
    $form_state['values']['pinned_site_recent_posts_icon'] = $form_state['values']['icon_upload'];
  }
}

/**
 * Validation callback; Uploads icon.
 * @param array $form
 * @param array $form_state
 */
function pinned_site_admin_icon_upload(&$form, &$form_state) {

  $validators = array('file_validate_extensions' => array('ico'));
  if ($file = file_save_upload('icon_upload', $validators)) {

    $destination = file_build_uri($file->filename);

    if (file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
      $absolute_url = file_create_url($destination);
      $url_parts = parse_url($absolute_url);
   
      $form_state['values']['icon_upload'] = $url_parts['path'];
    }
    else {
      form_set_error('icon_upload', t("Failed to upload the icon."));
    }
  }
}

/**
 * Displays the form for the Notification & Badge Settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function pinned_site_admin_notification_badge_settings() {
  
  $form['notification'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notification'),
    '#description' => t('Specifies URIs to be polled for notification markup.'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE
  );
  
  $form['notification']['polling_uris'] = array(
    '#type' => 'fieldset',
    '#title' => t('Polling URIs'),
    '#description' => t('Specifies URIs to be polled for notification markup.'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE
  );

  $form['notification']['polling_uris']['pinned_site_notification_polling_uri1'] = array(
    '#type' => 'textfield',
    '#title' => t('Polling URI 1'),
    '#default_value' => variable_get('pinned_site_notification_polling_uri1', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['notification']['polling_uris']['pinned_site_notification_polling_uri2'] = array(
    '#type' => 'textfield',
    '#title' => t('Polling URI 2'),
    '#default_value' => variable_get('pinned_site_notification_polling_uri2', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['notification']['polling_uris']['pinned_site_notification_polling_uri3'] = array(
    '#type' => 'textfield',
    '#title' => t('Polling URI 3'),
    '#default_value' => variable_get('pinned_site_notification_polling_uri3', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['notification']['polling_uris']['pinned_site_notification_polling_uri4'] = array(
    '#type' => 'textfield',
    '#title' => t('Polling URI 4'),
    '#default_value' => variable_get('pinned_site_notification_polling_uri4', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['notification']['polling_uris']['pinned_site_notification_polling_uri5'] = array(
    '#type' => 'textfield',
    '#title' => t('Polling URI 5'),
    '#default_value' => variable_get('pinned_site_notification_polling_uri5', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );
  
  $form['notification']['pinned_site_notification_freqency'] = array(
    '#type' => 'select',
    '#title' => t('Frequency'),
    '#description' => t('Defines the frequency, in minutes, between poll requests.'),
    '#options' => array(
      '30' => t('30'),
      '60' => t('60'),
      '360' => t('360'),
      '720' => t('720'),
      '1440' => t('1440'),
    ),
    '#default_value' => variable_get('pinned_site_notification_freqency', 30),
  );
  
  $form['notification']['pinned_site_notification_cycle'] = array(
    '#type' => 'select',
    '#title' => t('Cycle'),
    '#description' => t('Control notification cycling.'),
    '#options' => array(
      '0' => t('No tiles show notifications'),
      '1' => t('Notifications cycle for all tile sizes'),
      '2' => t('Notifications cycle for medium tiles only'),
      '3' => t('Notifications cycle for wide tiles only'),
      '4' => t('Notifications cycle for large tiles only'),
      '5' => t('Notifications cycle for medium and wide tiles'),
      '6' => t('Notifications cycle for medium and large tiles'),
      '7' => t('Notifications cycle for large and wide tiles'),
    ),
    '#default_value' => variable_get('pinned_site_notification_cycle', 1),
  );
  
  $form['badge'] = array(
    '#type' => 'fieldset',
    '#title' => t('Badge'),
    '#description' => t('Defines a badge for a live tile.'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE
  );

  $form['badge']['pinned_site_badge_polling_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Polling URI'),
    '#description' => t('Specifies the URI for an XML file containing badge schema.'),
    '#default_value' => variable_get('pinned_site_badge_polling_uri', ''),
    '#size' => 60,
    '#maxlength' => 255,
  );
  
  $form['badge']['pinned_site_badge_freqency'] = array(
    '#type' => 'select',
    '#title' => t('Frequency'),
    '#description' => t('Defines the frequency, in minutes, between poll requests.'),
    '#options' => array(
      '30' => t('30'),
      '60' => t('60'),
      '360' => t('360'),
      '720' => t('720'),
      '1440' => t('1440'),
    ),
    '#default_value' => variable_get('pinned_site_badge_freqency', 30),
  );

  return system_settings_form($form);
}
