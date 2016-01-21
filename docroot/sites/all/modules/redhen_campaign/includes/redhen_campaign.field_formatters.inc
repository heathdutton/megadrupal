<?php

/**
 * @file
 * Redhen Campaign custom field formatters.
 */


/**
 * Implements hook_field_formatter_info().
 */
function redhen_campaign_field_formatter_info() {
  $return = array();
  $inheritable_types = array(
    'file',
    'image',
    'media',
    'text',
    'text_long',
    'text_with_summary',
  );

  drupal_alter('redhen_campaign_inheritable_field_types', $inheritable_types);

  $return['redhen_campaign_inherit_value'] = array(
    'label' => t('Inherit Campaign Value'),
    'field types' => $inheritable_types,
    'settings' => array(
      'default' => 'page',
      'secondary' => 'campaign',
      'tertiary' => 'team',
      'view_mode' => 'default',
    ),
  );
  $return['redhen_campaign_smart_thermometer'] = array(
    'label' => t('Smart Thermometer'),
    'field types' => array('number_integer'),
    'settings'  => array(
      'thermometer' => TRUE,
      'text' => '!PROGRESS raised of !GOAL total.',
      'abbreviate' => FALSE,
      'characters' => array(
        'thousand' => 'K',
        'million' => 'M',
        'billion' => 'B',
        'trillion' => 'T',
      ),
    ),
  );

  return $return;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function redhen_campaign_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();
  switch ($display['type']) {
    case 'redhen_campaign_inherit_value':
      $options = array(
        'campaign' => 'Campaign',
        'team' => 'Team',
        'page' => 'Page',
      );
      $element['default'] = array(
        '#type' => 'select',
        '#title' => t('Default value'),
        '#default_value' => $settings['default'],
        '#options' => $options,
        '#required' => TRUE,
      );
      $options = array('' => '-- none --') + $options;
      $element['secondary'] = array(
        '#type' => 'select',
        '#title' => t('Secondary value'),
        '#default_value' => $settings['secondary'],
        '#options' => $options,
        '#required' => FALSE,
      );
      $element['tertiary'] = array(
        '#type' => 'select',
        '#title' => t('Tertiary value'),
        '#default_value' => $settings['tertiary'],
        '#options' => $options,
        '#required' => FALSE,
      );
      $element['view_mode'] = array(
        '#type' => 'textfield',
        '#title' => t('View Mode'),
        '#default_value' => $view_mode,
        '#access' => FALSE,
      );
      break;

    case 'redhen_campaign_smart_thermometer':
      $element['thermometer'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display Thermometer'),
        '#default_value' => $settings['thermometer'],
      );
      $element['text'] = array(
        '#type' => 'textfield',
        '#title' => 'Text',
        '#default_value' => $settings['text'],
        '#description' => 'Leave blank for none. Use "!PROGRESS" and "!GOAL" to include calculated numeric values.',
      );
      if (!module_exists('short_scale_formatter')) {
        $element['abbreviate'] = array(
          '#markup' => t("To use number abbreviations, install the short_scale_formatter module."),
        );
      }
      else {
        $element['abbreviate'] = array(
          '#type' => 'checkbox',
          '#title' => t('Use Abbreviations'),
          '#description' => 'Abbreviations make big numbers more readable by replacing, for example "1000" with "1k".',
          '#default_value' => $settings['abbreviate'],
        );
        $element['characters'] = array(
          '#type' => 'fieldset',
          '#tree' => TRUE,
          '#title' => t('Character Abbreviations'),
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
          '#states' => array(
            // Hide the settings when the cancel notify checkbox is disabled.
            'visible' => array(
              ':input[name="fields[' . $instance['field_name'] . '][settings_edit_form][settings][abbreviate]"]' => array('checked' => TRUE),
            ),
          ),
        );
        $element['characters']['thousand'] = array(
          '#type' => 'textfield',
          '#title' => t('Character for thousands'),
          '#default_value' => $settings['characters']['thousand'],
          '#states' => array(
            'required' => array(
              ':input[name="fields[' . $instance['field_name'] . '][settings_edit_form][settings][abbreviate]"]' => array('checked' => TRUE),
            ),
          ),
        );

        $element['characters']['million'] = array(
          '#type' => 'textfield',
          '#title' => t('Character for millions'),
          '#default_value' => $settings['characters']['million'],
          '#states' => array(
            'required' => array(
              ':input[name="fields[' . $instance['field_name'] . '][settings_edit_form][settings][abbreviate]"]' => array('checked' => TRUE),
            ),
          ),
        );

        $element['characters']['billion'] = array(
          '#type' => 'textfield',
          '#title' => t('Character for billions'),
          '#default_value' => $settings['characters']['billion'],
          '#states' => array(
            'required' => array(
              ':input[name="fields[' . $instance['field_name'] . '][settings_edit_form][settings][abbreviate]"]' => array('checked' => TRUE),
            ),
          ),
        );
        $element['characters']['trillion'] = array(
          '#type' => 'textfield',
          '#title' => t('Character for trillions'),
          '#default_value' => $settings['characters']['trillion'],
          '#states' => array(
            'required' => array(
              ':input[name="fields[' . $instance['field_name'] . '][settings_edit_form][settings][abbreviate]"]' => array('checked' => TRUE),
            ),
          ),
        );
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function redhen_campaign_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $return = array();
  switch ($display['type']) {
    case 'redhen_campaign_inherit_value':
      $values = array('default', 'secondary', 'tertiary');
      foreach ($values as $value) {
        if (!empty($settings[$value])) {
          $return[] = ucfirst($settings[$value]);
        }
      }
      return implode(' : ', $return);

    case 'redhen_campaign_smart_thermometer':
      return $settings['text'];
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function redhen_campaign_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, &$items, $display) {
  $settings = $display['settings'];

  $element = array();
  switch ($display['type']) {
    case 'redhen_campaign_inherit_value':
      $page_wrapper = entity_metadata_wrapper($entity_type, $entity);
      // Figure out where our content will come from.
      $source = $settings['default'];
      $new_items = _redhen_campaign_populate_view_items($source, $page_wrapper, $field, $langcode);
      if (empty($new_items) && $settings['secondary']) {
        $source = $settings['secondary'];
        $new_items = _redhen_campaign_populate_view_items($source, $page_wrapper, $field, $langcode);
      }
      if (empty($new_items) && $settings['tertiary']) {
        $source = $settings['tertiary'];
        $new_items = _redhen_campaign_populate_view_items($source, $page_wrapper, $field, $langcode);
      }
      if (!empty($new_items)) {
        $items = $new_items;
        // Get our rendering settings from the campaign:
        $campaign_instance = field_info_instance('redhen_campaign', $field['field_name'], $page_wrapper->campaign->type->value());
        $function = $campaign_instance['display'][$settings['view_mode']]['module'] . '_field_formatter_view';
        // Render our element using those settings:
        $element = $function('redhen_campaign', $page_wrapper->campaign->value(), $field, $campaign_instance, $langcode, $items, $campaign_instance['display'][$settings['view_mode']]);
      }
      break;

    case 'redhen_campaign_smart_thermometer':
      $display_settings = array();
      // If we want to abbreviate the numbers, build our settings for the short
      // scale module:
      if (module_exists('short_scale_formatter') && $settings['abbreviate']) {
        $format_function = 'short_scale_formatter_field_formatter_view';
        $display_settings['type'] = 'short_scale_formatter';
        $display_settings['settings'] = $settings['characters'];
        $display_settings['settings']['wrapper_element'] = 'span';
        $display_settings['settings']['wrapper_title_text'] = '';
        $markup_index = '#formatted';
      }
      else {
        // Use the regular number formatting:
        $format_function = 'number_field_formatter_view';
        $display_settings['type'] = 'number_integer';
        $display_settings['settings'] = array(
          'thousand_separator' => ',',
          'decimal_separator' => '.',
          'scale' => 0,
          'prefix_suffix' => FALSE,
        );
        $markup_index = '#markup';
      }
      $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
      $rendered_goal = $format_function($entity_type, $entity, $field, $instance, $langcode, $items, $display_settings);
      $goal_markup = $instance['settings']['prefix'] . $rendered_goal[0][$markup_index] . $instance['settings']['suffix'];
      $progress = $entity_wrapper->progress->value() / 100;
      $rendered_progress = $format_function($entity_type, $entity, $field, $instance, $langcode, array(array('value' => $progress)), $display_settings);
      $progress_markup = $instance['settings']['prefix'] . $rendered_progress[0][$markup_index] . $instance['settings']['suffix'];
      if (strlen($display['settings']['text'])) {
        $element[0]['details'] = array(
          '#markup' => t($display['settings']['text'], array(
            '@PROGRESS' => $progress_markup,
            '@GOAL' => $goal_markup,
            '%PROGRESS' => $progress_markup,
            '%GOAL' => $goal_markup,
            '!PROGRESS' => '<span class="rhc_progress">' . $progress_markup . '</span>',
            '!GOAL' => '<span class="rhc_goal">' . $goal_markup . '</span>',
          )),
        );
      }
      if ($display['settings']['thermometer']) {
        $element[0]['thermo'] = array(
          '#theme' => 'redhen_campaign_thermometer',
          '#percent' => 100 * min(1, ($progress / $items[0]['value'])),
          '#attached' => array(
            'css' => array(
              drupal_get_path('module', 'redhen_campaign') . '/css/redhen_campaign.theme.css',
            ),
          ),
        );
      }
      break;

  }

  return $element;
}

/**
 * Helper function populates the $items array as appropriate.
 *
 * Returns the entity type from which the items came.
 *
 * $items -- a field value array
 * $source -- page, campaign, or team
 * $wrapper -- a wrapper around this campaign page entity
 * $field -- the field definition
 */
function _redhen_campaign_populate_view_items($source, $wrapper, $field, $langcode) {
  $return = array();
  $source_entity = NULL;
  switch ($source) {
    case "page":
      $source_entity = $wrapper->value();
      break;

    case "campaign":
      $source_entity = $wrapper->campaign->value();
      break;

    case "team":
      if ($wrapper->team->value()) {
        $source_entity = $wrapper->team->value();
      }
      break;

  }
  if ($source_entity) {
    $return = isset($source_entity->{$field['field_name']}[$langcode]) ? $source_entity->{$field['field_name']}[$langcode] : array();
  }
  return $return;
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * If a field on a page is set to use the inherit_value field formatter by
 * default, and its own value is not a display option, the field should be
 * hidden since any value in the field will never be used.
 */
function redhen_campaign_form_redhen_campaign_campaign_page_form_alter(&$form, &$form_state, $form_id) {
  $field_instances = field_info_instances('redhen_campaign_page', $form['#bundle']);
  foreach ($field_instances as $field_config) {
    if ($field_config['display']['default']['type'] == 'redhen_campaign_inherit_value') {
      if (!in_array('page', $field_config['display']['default']['settings'])) {
        $form[$field_config['field_name']]['#access'] = FALSE;
      }
    }
  }
}
