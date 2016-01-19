<?php

/**
 * @file
 * Provide views handler for Facebook Social plugins.
 */

/**
 * A handler to provide a field for Facebook Social plugins.
 *
 * @ingroup views_field_handlers
 */
class drulenium_handler_field extends views_handler_field {

  /**
   * Implements views_object#option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['drulenium_plugins'] = array('default' => '');
    return $options;
  }

  /**
   * Implements views_handler#options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $presets = drulenium_get_presets();
    foreach ($presets as $preset) {
      $preset_list[$preset->name] = $preset->name;
    }

    $form['drulenium_plugins'] = array(
      '#title' => t('Facebook Social Preset'),
      '#options' => array('' => '<select>') + $preset_list,
      '#type' => 'select',
      '#required' => TRUE,
      '#default_value' => $this->options['drulenium_plugins'],
      '#description' => t('Choose a Facebook Social Preset'),
    );

  }

  /**
   * Implements views_handler_field#render().
   */
  function render($values) {

    if (!empty($this->options['drulenium_plugins'])) {
      $presets = drulenium_get_presets();
      $preset = $presets[$this->options['drulenium_plugins']];
      $type = $presets[$this->options['drulenium_plugins']]->plugin_type;
      $url = NULL;
      // Only supply url for node if nid exists and FB preset accepts url.
      // TODO: instead of hardcoded values, get this info from plugin defaults
      if ($type == 'like' || $type == 'send' || $type == 'facepile' || $type == 'comments') {
        // TODO: with a little work this could be expanded to other entities.
        if (!empty($values->nid)) {
          $url = drulenium_url('node/' . $values->nid);
        }
        elseif (!empty($values->uid)) {
          $url = drulenium_url('user/' . $values->uid);
        }
        $preset -> drulenium_attrs['href'] = $url;
      }
      $value = drulenium_preset_view($preset);
    }

    return $value;
  }
}
