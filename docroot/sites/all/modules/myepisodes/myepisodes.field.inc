<?php

/**
 * @file
 * My Episodes field related hooks.
 */

/**
 * Implements hook_field_access().
 */
function myepisodes_field_access($op, $field, $entity_type, $entity, $account) {
  if (module_exists('field_permissions')) {
    return TRUE;
  }

  if ('myepisodes' !== $field['type']) {
    return TRUE;
  }

  $user_is_owner = !isset($entity) || (isset($entity->uid) && $entity->uid == $account->uid);

  switch ($op) {

    case 'edit':
      return ($user_is_owner && user_access('myepisode edit', $account)) || user_access('myepisode bypass', $account);

    case 'view':
      return ($user_is_owner || user_access('myepisode view', $account)) || user_access('myepisode bypass', $account);
  }
}

/**
 * Implements hook_field_info().
 */
function myepisodes_field_info() {
  return array(
    'myepisodes' => array(
      'label' => t("My Episodes credentials"),
      'description' => t("Stores My Episode site's user RSS feed credentials."),
      'settings' => array(),
      'instance_settings' => array(
        'refresh_enabled' => FALSE,
        'manual_refresh_enabled' => FALSE,
        'public_display' => FALSE,
        'allowed_links' => array(),
      ),
      'default_widget' => 'myepisodes_form',
      'default_formatter' => 'myepisodes_advert',
    ),
  );
}

/**
 * Implements hook_field_widget_info().
 */
function myepisodes_field_widget_info() {
  return array(
    'myepisodes_form' => array(
      'label' => t('My Episodes user credentials'),
      'field types' => array('myepisodes'),
      'settings' => array(),
    ),
  );
}

/**
 * Implements hook_field_formatter_info().
 */
function myepisodes_field_formatter_info() {
  return array(
    'myepisodes_advert' => array(
      'label' => t("Advert"),
      'field types' => array('myepisodes'),
    ),
    'myepisodes_uid_only' => array(
      'label' => t("UID only"),
      'field types' => array('myepisodes'),
    ),
    'myepisodes_mylist' => array(
      'label' => t("My episodes list"),
      'field types' => array('myepisodes'),
      'settings' => array(
        'download_links' => 0,
        'site_link' => 0,
      ),
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function myepisodes_field_is_empty($item, $field) {
  return !isset($item['uid']) || empty($item['uid']);
}

/**
 * Implements hook_field_formatter_settings_form().
 * 
 * FIXME: Does not work.
 */
function myepisodes_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  if ('myepisodes_mylist' === $display['type']) {
    $element['download_links'] = array(
      '#title' => t("Show download links"),
      '#type' => 'checkbox',
      '#default_value' => $settings['download_links'],
    );
    $element['site_link'] = array(
      '#title' => t("Show My Episodes site link"),
      '#type' => 'checkbox',
      '#default_value' => $settings['site_link'],
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function myepisodes_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = '';

  /*
   * FIXME: Display boolean settings (display links).
   * 
  if ($instance['settings']) {
    $summary = t('Trim length') . ': ' . $settings['trim_length'];
  }
   */

  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function myepisodes_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'myepisodes_advert':
      foreach ($items as $delta => $item) {
        if ($entity_type === 'user') {
          $element[$delta] = array('#markup' => t("This user uses My Episodes!"));
        } else {
          $element[$delta] = array('#markup' => t("This content carries My Episodes information!"));
        }
        break;
      }
      break;

    case 'myepisodes_uid_only':
      foreach ($items as $delta => $item) {
        $element[$delta] = array('#markup' => check_plain($items[$delta]['uid']));
      }
      break;

    case 'myepisodes_mylist':
      foreach ($items as $delta => $item) {
        $element[$delta] = myepisodes_mylist_render($item);
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_widget_settings_form().
 */ 
function myepisodes_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  $form = array();

  $form['refresh_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow user to enable their feed automatic refresh"),
    '#default_value' => isset($settings['refresh_enabled']) ? $settings['refresh_enabled'] : NULL,
  );
  $form['manual_refresh_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow users to manually trigger a refresh"),
    '#default_value' => isset($settings['manual_refresh_enabled']) ? $settings['manual_refresh_enabled'] : NULL,
  );
  $form['public_display'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow users to enable public display of their fields"),
    '#default_value' => isset($settings['public_display']) ? $settings['public_display'] : NULL,
  );
  $form['allowed_links'] = array(
    '#title' => t("Allowed links"),
    '#type' => 'checkboxes',
    '#options' => myepisodes_get_links(),
    '#default_value' => isset($settings['allowed_links']) ? $settings['allowed_links'] : array(),
    '#description' => t("Every unchecked link in this list will not be available to user to enable."),
  );

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function myepisodes_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $value = isset($items[$delta]) ? $items[$delta] : array();
  // FIXME: Generate warnings on instance settings.
  $settings = isset($instance['widget']['settings']) ? $instance['widget']['settings'] : $instance['settings'];

  /*
   * Cannot put an inoffensive fieldset: yet again Field API/Form API sadness.
   * 
  $element['#tree'] = FALSE;
  $element['mep'] = array(
    '#type' => 'fieldset',
    '#title' => $instance['label'],
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
   */
  $element/*['mep']*/['uid'] = array(
    '#type' => 'textfield',
    '#title' => t("My Episode site user identifier"),
    '#default_value' => isset($value['uid']) ? $value['uid'] : NULL,
    '#maxlength' => 128,
  );
  $element/*['mep']*/['password_hash'] = array(
    '#type' => 'textfield',
    '#title' => t("Associated password hash"),
    '#default_value' => isset($value['password_hash']) ? $value['password_hash'] : NULL,
    '#maxlength' => 64,
  );

  // Default value on database column does not work: Field API sadness.
  // But it allows us to invalidate actual data in case the user changes its
  // credentials.
  $element/*['mep']*/['last_refresh'] = array(
    '#type' => 'value',
    '#value' => 0,
  );

  if ($settings['refresh_enabled']) {
    $element/*['mep']*/['refresh_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t("Refresh my episodes list on a regular basis"),
      '#default_value' => isset($value['refresh_enabled']) ? $value['refresh_enabled'] : NULL,
    );
  } else {
    $element/*['mep']*/['refresh_enabled'] = array(
      '#type' => 'value',
      '#value' => isset($value['refresh_enabled']) ? $value['refresh_enabled'] : 0,
    );
  }

  if ($settings['public_display']) {
    $element/*['mep']*/['public_display'] = array(
      '#type' => 'checkbox',
      '#title' => t("Allow other users to see my episodes list"),
      '#default_value' => isset($value['public_display']) ? $value['public_display'] : NULL,
    );
  } else {
    $element/*['mep']*/['public_display'] = array(
      '#type' => 'value',
      '#default_value' => isset($value['public_display']) ? $value['public_display'] : 0,
    );
  }

  /*
  $allowed_links = array();
  $enabled_links = myepisodes_get_links();
  foreach ($settings['allowed_links'] as $key => $value) {
    if (isset($enabled_links[$key]) && $key === $value) {
      $allowed_links[$key] = $enabled_links[$key];
    }
  }

  if (empty($allowed_links)) {
    $element['enabled_links'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
  } else {
    $element['enabled_links'] = array(
      '#title' => t("Enabled links"),
      '#type' => 'checkboxes',
      '#options' => $allowed_links,
      '#default_value' => isset($settings['enabled_links']) ? $settings['enabled_links'] : array(),
      '#description' => t("Select here links that you want to appear on your episodes list."),
    );
  }
   */

  return $element;
}


/**
 * Build the render array for given my list field item.
 */
function myepisodes_mylist_render($item) {
  $build = array('#theme' => 'myepisodes_mylist');

  if (!empty($item['cache_mylist'])) {
    foreach (unserialize($item['cache_mylist']) as $list_item) {
      $build['#items'][] = myepisodes_mylist_item_render($list_item);
    }
  }
  else {
    $build['#items'] = array();
  }

  $build['#links'] = array(
    '#theme' => 'links',
    '#pre_render' => array('drupal_pre_render_links'),
    '#attributes' => array('class' => array('links', 'inline')),
  );

  // Allow other modules to alter links.
  drupal_alter('myepisode_mylist_links', $build['#links']['#links']);

  return $build;
}

/**
 * Build the render array for given my list field item.
 */
function myepisodes_mylist_item_render($item) {
  $build = array(
    '#theme' => 'myepisodes_mylist_item',
    '#item'  => $item,
  );

  $build['#links'] = array(
    '#theme' => 'links',
    '#pre_render' => array('drupal_pre_render_links'),
    '#attributes' => array('class' => array('links', 'inline')),
  );
  $build['#links']['#links']['myepisodes'] = array(
    'title' => t("See on My Episodes"),
    'href' => 'http://myepisodes.com', // FIXME: Find real link.
    'attributes' => array('rel' => 'tag', 'title' => t("Show on My Episodes"), 'target' => '_blank'),
  );
  if (isset($item['season'])) {
    $search_string = $item['title'] . ' ' . 'S' . $item['season'] . 'E' . $item['episode'];
  } else {
    $search_string = $item['title'] . ' ' . $item['episode_number'];
  }
  $build['#links']['#links']['torrentz'] = array(
    'title' => t("Torrenz"),
    'href' => 'http://torrentz.eu/search?f=' . $search_string,
    'html' => TRUE,
    'attributes' => array('rel' => 'tag', 'title' => t("Search on Torrentz"), 'target' => '_blank'),
  );
  $build['#links']['#links']['torrentz720p'] = array(
    'title' => t("Torrenz 720p"),
    'href' => 'http://torrentz.eu/search?f=' . $search_string . ' 720p',
    'html' => TRUE,
    'attributes' => array('rel' => 'tag', 'title' => t("Search on Torrentz"), 'target' => '_blank'),
  );

  // Allow other modules to alter links.
  drupal_alter('myepisode_mylist_item_links', $build['#links']['#links']);

  return $build;
}

/**
 * Implements template_preprocess_TEMPLATE().
 */
function template_preprocess_myepisodes_mylist(&$variables) {
  $variables['items'] = $variables['elements']['#items'];
  if (isset($variables['elements']['#links'])) {
    $variables['links'] = $variables['elements']['#links'];
  }
  else {
    $variables['links'] = array();
  }
}

/**
 * Implements template_preprocess_TEMPLATE().
 */
function template_preprocess_myepisodes_mylist_item(&$variables) {
  $variables += $variables['elements']['#item'];
  if (isset($variables['elements']['#links'])) {
    $variables['links'] = $variables['elements']['#links'];
  }
  else {
    $variables['links'] = array();
  }
}
