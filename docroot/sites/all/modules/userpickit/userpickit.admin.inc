<?php

/**
 * @file
 * Pages for User Pic Kit.
 */

/**
 * Implements hook_form_BASE_FORM_ID_alter().
 *
 * Hook into user_profile_form().
 *
 * @see hook_form_BASE_FORM_ID_alter()
 */
function userpickit_form_user_profile_form_alter(&$form, &$form_state, $form_id) {
  drupal_add_css(drupal_get_path('module', 'userpickit') . '/css/userpickit.css');

  $picture_types = userpickit_picture_type_info();
  $form['#submit'][] = 'userpickit_form_user_submit';
  $account = $form['#user'];

  $pictures = array();
  $messages = array();

  $options_active = array();
  $options_inactive = array();

  foreach ($picture_types as $picture_type => $info) {
    if (user_access('userpickit use ' . $picture_type . ' type', $account)) {
      $path = userpickit_picture_path_cache($picture_type, $account);
      $cache = userpickit_cache_get($picture_type, $account->uid);

      $option = $info['title'];

      // Image
      $style = _userpickit_picture_type_ui_style();

      $messages[$picture_type] = !empty($cache->message) ? t($cache->message, $cache->message_variables) : '';

      $default_image = userpickit_picture_type_path_default($picture_type, $account);
      if ($path->fid || $path->uri || $default_image) {
        $picture_uri = userpickit_picture_uri($picture_type, $account);
        if (userpickit_valid_uri($picture_uri)) {
          $pictures[$picture_type] = userpickit_render_picture($account, $picture_uri, $style);
        }
        $options_active[$picture_type] = $info['title'];
      }
      else {
        $pictures[$picture_type] = $default_image;
        $options_inactive[$picture_type] = $info['title'];
      }
    }
  }

  $form['picture']['picture_type'] = array(
    '#title' => t('Picture'),
    '#description' => t('Choose a picture to associate with your account.'),
    '#type' => 'radios',
    '#theme' => 'userpickit_picture_configuration_active',
    '#options' => $options_active,
    '#default_value' => userpickit_picture_type_user_get($account),
    '#weight' => -20,
    '#access' => !empty($options_active),
    '#userpickit_pictures' => $pictures,
    '#userpickit_messages' => $messages,
  );

  $form['picture']['picture_type_disabled'] = array(
    '#title' => t('Inactive picture types'),
    '#theme' => 'userpickit_picture_configuration_inactive',
    '#items' => $options_inactive,
    '#weight' => -15,
    '#access' => count($options_inactive),
    '#userpickit_pictures' => $pictures,
    '#userpickit_messages' => $messages,
  );

  $form['picture']['#title'] = t('Picture');
  $form['picture']['#type'] = 'fieldset';

  $form['picture']['picture_current']['#access'] = FALSE;
  $form['picture']['picture_delete']['#access'] = FALSE;

  $form['picture']['userpickit_user_delete'] = array(
    '#title' => t('Delete uploaded picture'),
    '#type' => 'checkbox',
    '#access' => userpickit_user_picture_upload_get($account->uid),
    '#description' => t('Check this box to delete your uploaded picture.'),
    '#weight' => 20,
  );
}

/**
 * Form submission callback.
 *
 * @see userpickit_form_user_profile_form_alter()
 */
function userpickit_form_user_submit($form, $form_state) {
  $account = $form['#user'];

  // Set preference
  userpickit_picture_type_user_set($form_state['values']['picture_type'], $account->uid);

  // Delete uploaded picture
  if ($form_state['values']['userpickit_user_delete']) {
    // Minor quirk if 'user' is a users selected picture type:
    // Picture will not get removed until a new picture type is selected,
    // due to file_usage of Drupal core.
    // Do not force delete as the file may be managed by other modules.
    if (userpickit_picture_type_user_get($account) != 'user') {
      userpickit_user_picture_upload_set($account->uid, FALSE);
    }
    else {
      drupal_set_message(t('Please change your picture preference to something other than your uploaded picture before attempting to delete your uploaded picture.'), 'warning');
    }
  }

  userpickit_user_picture_reset($account->uid);
}

/**
 * Returns HTML for the user picture configuration form.
 */
function theme_userpickit_picture_configuration_active(&$variables) {
  $form = &$variables['form'];
  $result = '<div class="picture-configuration">';

  // Loop through each rendered option element
  foreach (element_children($form) as $picture_type) {
    $result .= '<div class="picture-type active">';

    // Picture
    $result .= '<label for="' . $form[$picture_type]['#id'] . '">';
    if (isset($form['#userpickit_pictures'][$picture_type])) {
      // Sometimes there is no picture, such as 'user' type with an invalid
      // fallback in 'user_picture_default'.
      $result .= $form['#userpickit_pictures'][$picture_type];
    }
    $result .= '</label>';

    // Title
    $result .= '<h3>' . render($form[$picture_type]) . '</h3>';

    // Description
    $result .= '<div class="picture-type-description">';
    $result .= $form['#userpickit_messages'][$picture_type];
    $result .= '</div>';

    $result .= '</div>';
  }

  $result .= '</div>';
  return $result;
}

/**
 * Returns HTML for disabled picture types on picture configuration form.
 */
function theme_userpickit_picture_configuration_inactive(&$variables) {
  $form = &$variables['form'];
  $items = array();
  $result = '<div class="picture-configuration">';

  foreach ($form['#items'] as $picture_type => $title) {
    $item = '<div class="picture-type inactive">';
    $item .= '<div class="title">' . $title . '</div>';
    $item .= '<div class="message">' . $form['#userpickit_messages'][$picture_type] . '</div>';
    $item .= '</div>';
    $items[$picture_type] = $item;
  }

  $result .= theme('item_list', array(
    'title' => t('Inactive Pictures'),
    'items' => $items,
  ));

  $result .= '</div>';
  return $result;
}

/**
 * Global settings form for User Pic Kit.
 */
function userpickit_settings_form($form, $form_state) {
  $options = array();
  $picture_types = userpickit_picture_type_info();
  foreach ($picture_types as $picture_type => $info) {
    if (!empty($info['default callback'])) {
      $options[$picture_type] = $info['title'];
    }
  }

  $default_user_image = userpickit_userpickit_picture_default();
  if (!empty($default_user_image) && !file_valid_uri($default_user_image)) {
    drupal_set_message(t('Drupal cannot perform image style operations on the site <a href="@url">default image</a> because it has incorrect URL scheme.
Instead of a path like `http://example.com/image.png` or `sites/my_site/files/default.png. Try using a path such as `public://default.png`', array(
      '@url' => url('admin/config/people/accounts', array('fragment' => 'edit-user-picture-default')),
    )), 'warning');
  }

  $form['picture_type_default'] = array(
    '#title' => t('Default picture type preference'),
    '#description' => t('Picture type to use when a user is anonymous, or has not set a preference.'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('userpickit_default', '')
  );

  if (module_exists('image')) {
    $form['picture_type_ui_style'] = array(
      '#type' => 'select',
      '#title' => t('Picture list display style'),
      '#options' => image_style_options(TRUE),
      '#default_value' => _userpickit_picture_type_ui_style(),
      '#description' => t('The style of image to use on the picture configuration form. Styles may be configured in the <a href="!url">Image styles</a> administration area.', array('!url' => url('admin/config/media/image-styles'))),
    );
  }

  // @see userpickit_user_picture_reset()
  $form['behavior_fail_load'] = array(
    '#title' => t('New picture error behavior'),
    '#description' => t('Behavior to use when a users new picture cannot be loaded.'),
    '#type' => 'radios',
    '#options' => array(
      USERPICKIT_BEHAVIOR_FAIL_LOAD_NEW_KEEP_LAST => t('Use last picture'),
      USERPICKIT_BEHAVIOR_FAIL_LOAD_NEW_ERASE_LAST => t('Erase last picture, reset to default')
    ),
    '#default_value' => userpickit_behavior_fail_load_get(),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission callback
 *
 * @see userpickit_settings_form()
 */
function userpickit_settings_form_submit($form, &$form_state) {
  userpickit_behavior_fail_load_set($form_state['values']['behavior_fail_load']);

  variable_set('userpickit_default', $form_state['values']['picture_type_default']);
  variable_set('userpickit_picture_type_configuration_style', $form_state['values']['picture_type_ui_style']);
}

/**
 * Configure settings for individual picture type.
 */
function userpickit_picture_type_form($form, $form_state, $picture_type) {
  drupal_set_title(t('@picture_type settings', array('@picture_type' => $picture_type['title'])));

  $form['#picture_type'] = $picture_type;
  $settings = userpickit_picture_type_settings_get($picture_type['picture_type']);

  $cache_lifetime_options = array(
    43200 => t('12 hours'),
    86400 => t('24 hours'),
    604800 => t('7 days'),
    1209600 => t('14 days'),
    2419200 => t('28 days'),
    4838400 => t('56 days'),
  );

  $form['cache_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Cache lifetime'),
    '#description' => t('Length of time to cache picture.'),
    '#options' => $cache_lifetime_options,
    '#default_value' => $settings->info['cache lifetime'],
  );

  $form['cache_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache locally'),
    '#description' => t('Download the picture to the server. This option is required for picture styles to operate on remote pictures. Clear the cache for this picture type after changing this option.'),
    '#default_value' => $settings->info['cache locally'],
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Configure settings for individual picture type.
 */
function userpickit_picture_type_form_submit($form, $form_state) {
  $picture_type = $form['#picture_type'];

  $settings = array(
    'cache lifetime' => (int)$form_state['values']['cache_lifetime'],
    'cache locally' => (int)$form_state['values']['cache_locally'],
  );

  userpickit_picture_type_settings_set($picture_type['module'], $picture_type['picture_type'], $settings);
}

/**
 * Configure settings for all picture types.
 */
function userpickit_picture_types_form($form, $form_state) {
  $picture_types = userpickit_picture_type_info();
  $form['#picture_types_all'] = $picture_types;

  $header = array(t('Picture type'), t('Files cached'), t('Total cached'), t('Usage'), t('Operations'));

  $options = array();

  foreach ($picture_types as $picture_type => $info) {
    $title = '<label for="edit-picture-types-' . drupal_html_class($picture_type) . '">' . $info['title'] . '</label>';
    if (!empty($info['description'])) {
      $title .= $info['description'];
    }

    $count_files = db_select('userpickit_pictures', 'f')
      ->fields('f')
      ->condition('type', $picture_type)
      ->isNotNull('fid')
      ->countQuery()->execute()->fetchField();

    $count_users = db_select('userpickit_user_preferences', 'p')
      ->fields('p')
      ->condition('picture_type', $picture_type)
      ->countQuery()->execute()->fetchField();

    $count_all = _userpickit_picture_type_cache_count($picture_type);

    $row = array(
      $title,
      $count_files,
      $count_all,
      $count_users,
      l(t('settings'), 'admin/config/people/userpickit/picture_types/manage/' . $picture_type),
    );

    $options[$picture_type] = $row;
  }

  $form['picture_types'] = array(
    '#type' => 'tableselect',
    '#multiple' => TRUE,
    '#header' => $header,
    '#options' => $options,
    '#default_value' => array(),
    '#empty' => t('No picture types found.'),
  );

  $form['delete_note']['#markup'] = t('Warning: clearing local cache will cause performance to degrade temporarily while local caches are re-downloaded.');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['clear_cache'] = array(
    '#type' => 'submit',
    '#value' => t('Clear caches'),
  );

  return $form;
}

/**
 * Form submission callback
 *
 * @see userpickit_picture_types_form()
 */
function userpickit_picture_types_form_submit($form, &$form_state) {
  $picture_types = $form['#picture_types_all'];

  foreach ($form_state['values']['picture_types'] as $picture_type => $checked) {
    $info = $picture_types[$picture_type];
    if (!$checked) {
      continue;
    }

    $batch = array(
      'title' => t('Processing @picture_type', array('@picture_type' => $info['title'])),
      'file' => drupal_get_path('module', 'userpickit') . '/userpickit.admin.inc',
      'finished' => 'userpickit_picture_types_batch_finished',
      'operations' => array(),
    );

    $count_all = _userpickit_picture_type_cache_count($picture_type);
    $operations_per_page = 10;
    $operations_count = ceil($count_all / $operations_per_page);
    for ($i = 0; $i < $operations_count; $i++) {
      $batch['operations'][] = array('userpickit_picture_types_batch_delete', array(
        $picture_type,
        $i,
      ));
    }

    batch_set($batch);
  }
}

/**
 * Batch API 'operation' callback.
 *
 * @see userpickit_picture_types_form_submit()
 */
function userpickit_picture_types_batch_delete($picture_type, $operation, &$context) {
  $operations_per_page = 10;
  $result = db_select('userpickit_pictures', 'f')
    ->fields('f', array('uid', 'fid'))
    ->condition('type', $picture_type)
    ->range(0, $operations_per_page)
    ->execute();

  foreach ($result as $cache) {
    _userpickit_cache_delete($picture_type, $cache->uid, $cache->fid);
  }
}

/**
 * Batch API 'finished' callback.
 *
 * @see userpickit_picture_types_form_submit()
 */
function userpickit_picture_types_batch_finished() {
  //drupal_set_message(t('Finished clearing cache for @picture_type'))
}

function userpickit_maintenance_form($form, $form_state) {
  $form['migrate_user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User preferences'),
  );

  $form['migrate_user']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Migrate user preferences'),
    '#submit' => array('userpickit_maintenance_migrate_user_preferences'),
  );

  if (variable_get(USERPICKIT_VARIABLE_MIGRATION_USER_PREFERENCES_NOT_DONE, NULL) === NULL) {
    $form['migrate_user']['submit']['#disabled'] = TRUE;
    $form['migrate_user']['submit']['#value'] = t('Migration complete');
  }

  $form['migrate_user']['message']['#markup'] = '<p>' . t('Set picture type preference of all users to "Picture uploaded to Site Install" if users have uploaded a picture. This migration should be run once after installing User Pic Kit. Users who have not uploaded a picture will use the default picture type.') . '</p>';

  return $form;
}

/**
 * Get current image style for picture configuration form.
 */
function _userpickit_picture_type_ui_style() {
  return variable_get(
    'userpickit_picture_type_configuration_style',
    variable_get('user_picture_style', USERPICKIT_IMAGE_STYLE_PICTURE_CONFIGURATION)
  );
}

function _userpickit_picture_type_cache_count($picture_type) {
  return db_select('userpickit_pictures', 'f')
    ->fields('f')
    ->condition('type', $picture_type)
    ->countQuery()
    ->execute()
    ->fetchField();
}