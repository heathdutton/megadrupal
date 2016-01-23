<?php
/**
 * @file
 * Showoff Feeds field info.
 */

/**
 * Implements hook_field_widget_info().
 */
function showoff_feeds_field_widget_info() {
  return array(
    'showoff_feeds_layout_widget_default' => array(
      'label' => t('Feed Layout'),
      'description' => 'Default widget for selecting a layout and associated feeds.',
      'field types' => array('showoff_feeds_layout'),
    ),
  );
}

/**
 * Implements hook_field_formatter_info().
 */
function showoff_feeds_field_formatter_info() {
  return array(
    'showoff_feeds_layout_format_default' => array(
      'label' => t('Feed Layout'),
      'description' => 'Default layout formatter.',
      'field types' => array('showoff_feeds_layout'),
    ),
  );
}

/**
 * Implements hook_field_info().
 */
function showoff_feeds_field_info() {
  return array(
    'showoff_feeds_layout' => array(
      'label' => t('Feed Layout'),
      'description' => t('Allows the selection of a specific layout to display feeds.'),
      'settings' => array(),
      'instance_settings' => array(
        'allowed_views' => array(),
      ),
      'default_widget' => 'showoff_feeds_layout_widget_default',
      'default_formatter' => 'showoff_feeds_layout_format_default',
    ),
  );
}

/**
 * Implements hook_field_instance_settings_form().
 */
function showoff_feeds_field_instance_settings_form($field, $instance) {
  $form['allowed_views'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allowed Views'),
    '#options' => showoff_feeds_get_enabled_view_options(),
    '#default_value' => isset($instance['settings']['allowed_views']) && is_array($instance['settings']['allowed_views']) ? $instance['settings']['allowed_views'] : array(),
    '#description' => t('Only views selected here will show up as available feed options.'),
  );
  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function showoff_feeds_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  if ($field['type'] == 'showoff_feeds_layout') {
    $element['layout'] = array(
      '#instance' => $instance,
      '#type' => 'select',
      '#options' => array(0 => '- Select -') + showoff_feeds_map_layouts(),
      '#title' => t('Layout'),
      '#required' => $instance['required'],
      '#default_value' => isset($items[$delta]['layout']) ? $items[$delta]['layout'] : array(),
    );
    return $element;
  }
}

/**
 * Implements hook_field_validate().
 */
function showoff_feeds_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  if ($instance['required'] && isset($items['0']['layout']) && $items['0']['layout'] === 0) {
    $errors[$field['field_name']][$langcode][0][] = array(
      'error' => 'required',
      'message' => t('<em>!field</em> is required.', array('!field' => $instance['field_name'])),
    );
  }
}

/**
 * Implements hook_field_presave().
 */
function showoff_feeds_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => $item) {
    $items[$delta]['settings'] = serialize($items[$delta]['settings']);
  }
}

/**
 * Implements hook_field_widget_error().
 */
function showoff_feeds_field_widget_error($element, $error, $form, &$form_state) {
  form_error($element, $error['message']);
}

/**
 * AJAX callback for showing region+feed form fields.
 */
function showoff_feeds_layout_js($form, $form_state) {
  return drupal_rebuild_form($form['#form_id'], $form_state, $form);
}

/**
 * Get a list of available view displays for feed options.
 */
function showoff_feeds_get_allowed_view_options($instance) {
  $allowed = $instance['settings']['allowed_views'];
  $options = array();
  foreach ($allowed as $view) {
    $view = views_get_view($view);
    $key = (!empty($view->human_name) ? $view->human_name : $view->name);
    $options[$key] = array();
    foreach ($view->display as $display => $info) {
      $options[$key][$view->name . ':' . $display] = t('!title (!key)', array('!title' => $info->display_title, '!key' => $display));
    }
  }
  return $options;
}

/**
 * Implements hook_field_is_empty().
 */
function showoff_feeds_field_is_empty($item, $field) {
  if ($field['type'] == 'showoff_feeds_layout') {
    return empty($item['layout']);
  }
}

/**
 * Returns an array of available regions for the specified layout.
 */
function showoff_feeds_load_regions($layout) {
  if (is_array($layout)) {
    $layout = $layout['type'];
  }
  $layout = showoff_feeds_get_layout($layout);
  return $layout['settings'];
}

/**
 * Returns an array of enabled views for the field.
 */
function showoff_feeds_get_enabled_view_options() {
  $options = array();
  $views = views_get_enabled_views();
  foreach ($views as $view) {
    $vals = array('!name' => $view->human_name, '!key' => $view->name);
    $options[$view->name] = (!empty($view->human_name) ? t('!name (!key)', $vals) : $view->name);
  }
  return $options;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function showoff_feeds_form_node_form_alter(&$form, &$form_state) {
  if (isset($form['field_feeds'])) {
    $lng = $form['field_feeds']['#language'];
    $wrapper = $form['field_feeds'] . '-settings';
    $form['field_feeds'][$lng]['0']['layout']['#ajax'] = array(
      'callback' => 'showoff_feeds_layout_js',
      'wrapper' => $wrapper,
    );
    $form['#prefix'] = '<div id="' . $wrapper . '">';
    $form['#suffix'] = '</div>';
    if (isset($form_state['values']['field_feeds'][$lng]['0']['layout'])
      || isset($form['#node']->field_feeds[$lng]['0']['layout'])) {
      $layout = (isset($form_state['values']['field_feeds'][$lng]['0']['layout']) ? $form_state['values']['field_feeds'][$lng]['0']['layout'] : $form['#node']->field_feeds[$lng]['0']['layout']);
      $region_form = array();
      if (!empty($layout)) {
        $regions = showoff_feeds_load_regions($layout);
        $form['field_feeds'][$lng]['0']['settings'] = array(
          '#type' => 'fieldset',
          '#collapsible' => FALSE,
          '#title' => t('Select Feeds'),
          '#tree' => TRUE,
        );
        $settings = array();
        if (isset($form['#node']->field_feeds[$lng]['0']['settings'])) {
          $settings = unserialize($form['#node']->field_feeds[$lng]['0']['settings']);
        }
        foreach ($regions as $region) {
          $form['field_feeds'][$lng]['0']['settings'][$region] = array(
            '#type' => 'select',
            '#required' => $form['field_feeds'][$lng]['#required'],
            '#title' => t('<em>!region</em> Feed Selection', array('!region' => $region)),
            '#default_value' => (isset($settings[$region]) ? $settings[$region] : NULL),
            '#options' => showoff_feeds_get_allowed_view_options($form['field_feeds'][$lng]['0']['layout']['#instance']),
          );
        }
      }
    }
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function showoff_feeds_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  if ($field['type'] == 'showoff_feeds_layout') {
    global $user;
    $layout = $items['0']['layout'];
    $regions = unserialize($items['0']['settings']);
    if ($user->uid != 0) {
      // Don't do the rotator if they're logged in.
      $vars = array(
        'header' => array(
          0 => array(
            'data' => t('<em>Layout:</em> !layout', array('!layout' => $layout)),
            'colspan' => 2 * count($regions),
          ),
        ),
      );
      foreach ($regions as $region => $view_data) {
        list($view, $display) = explode(':', $view_data);
        $view = views_get_view($view);
        $view_name = (isset($view->human_name) ? $view->human_name . ' (' . $view->name . ')' : $view->name);
        $view_display = (isset($view->display[$display]->display_title) ? $view->display[$display]->display_title . ' (' . $display . ')' : $display);
        $view_display = l($view_display, $view->display[$display]->display_options['path'], array('absolute' => TRUE));
        $vars['rows'][] = array(
          0 => array(
            'width' => '25%',
            'data' => t('<em>Region:</em> !region', array('!region' => $region)),
          ),
          1 => t('<em>View:</em> !view<br /><em>Display:</em> !display', array(
            '!view' => $view_name,
            '!display' => $view_display
          )),
        );
      }
      $element[0] = array(
        '#markup' => theme('table', $vars),
        '#title' => $instance['label'],
      );
    }
    else {
      $variables = array(
        'layout' => $layout,
      );
      foreach ($regions as $region_id => $view_info) {
        list($view, $display) = explode(':', $view_info);
        $variables['regions'][$region_id] = array(
          'view' => $view,
          'display' => $display,
        );
      }
      $element[0] = array(
        '#markup' => theme('showoff_layout', $variables),
      );
    }
    return $element;
  }
}
