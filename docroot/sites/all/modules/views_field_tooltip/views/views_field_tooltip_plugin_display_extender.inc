<?php

class views_field_tooltip_plugin_display_extender extends views_plugin_display_extender {
  function options_definition_alter(&$options) {
    $options['tooltips'] = array('default' => array(), 'unpack_translatable' => 'unpack_tooltips');
    $options['qtip'] = array('default' => array());
  }

  function unpack_tooltips(&$translatable, $storage, $option, $definition, $parents, $keys = array()) {
    $tooltips = $storage[$option];
    if (!empty($tooltips)) foreach ($tooltips as $field => $info) {
      // Left-over from previous version that only had label tooltips.
      if (is_string($info)) {
        $info = array('label_tooltip' => $info);
      }

      if (!empty($info['field_tooltip']['text'])) {
        $translation_keys = array_merge($keys, array($field, 'field_tooltip', 'text'));
        $translatable[] = array(
          'value' => $info['field_tooltip']['text'],
          'keys' => $translation_keys,
          'format' => NULL,
        );
      }
      if (!empty($info['label_tooltip']['text'])) {
        $translation_keys = array_merge($keys, array($field, 'label_tooltip', 'text'));
        $translatable[] = array(
          'value' => $info['label_tooltip']['text'],
          'keys' => $translation_keys,
          'format' => NULL,
        );
      }
    }
  }

  function options_form(&$form, &$form_state) {
    if ($form_state['section'] != 'qtip') return;

    $library = variable_get('views_field_tooltip_library', 'qtip2');
    $info = views_field_tooltip_get_library_info($library);
    if (!empty($info)) {
      $form['qtip'] = array(
        '#type' => 'textarea',
        '#title' => t('Default settings'),
        '#description' => t('These settings apply to all label tooltips, and to field tooltips by default. Field tooltip settings can be overridden in the field settings UI.') . ' ' . call_user_func($info['help callback']),
        '#default_value' => jsonpp($this->display->get_option('qtip')),
      );
    }
    else {
      $form['qtip'] = array(
        '#type' => 'value',
        '#value' => jsonpp($this->display->get_option('qtip')),
      );
      drupal_set_message(t('Tooltip library variable <code>views_field_tooltip_library</code> holds unknown value %library. Supported libraries are <em>qtip</em> and <em>qtip2</em>.', array('%library' => $library)), 'warning');
    }
    $form['#title'] = t('Tooltip style');
  }

  function options_validate(&$form, &$form_state) {
    if ($form_state['section'] != 'qtip') return;

    $settings = json_decode($form_state['values']['qtip']);
    if (json_last_error() != JSON_ERROR_NONE) {
      form_set_error('qtip', t(json_last_error_msg()));
    }
  }

  function options_submit(&$form, &$form_state) {
    if ($form_state['section'] != 'qtip') return;

    $settings = json_decode($form_state['values']['qtip']);
    $this->display->set_option('qtip', $settings);
  }

  function options_summary(&$categories, &$options) {
    $options['qtip'] = array(
      'category' => 'other',
      'title' => t('Tooltip style'),
      'value' => t('Settings'),
      'desc' => t('Change the default tooltip style settings for this display.'),
    );
  }
}
