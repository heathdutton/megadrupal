<?php

/**
 * @file
 * Provide the views Ooyala shared player object with default options and form.
 */

/**
 * Style plugin to render each embed code in a single Ooyala player.
 *
 * @ingroup views_style_plugins
 */
class ooyala_style_plugin extends views_plugin_style {
  function option_definition() {
    $options = parent::option_definition();

    $options['embed_code_field'] = array('default' => NULL);
    $options['title_field'] = array('default' => NULL);
    $options['width'] = array('default' => 400);
    $options['height'] = array('default' => 300);

    return $options;
  }

  /**
   * Return the form for configuring the style plugin options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#markup' => '<div class="error messages">' . t('You need at least one field before you can configure the shared player settings.') . '</div>',
      );
      return;
    }

    $form['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Player width'),
      '#default_value' => $this->options['width'],
    );

    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Player height'),
      '#default_value' => $this->options['height'],
    );

    // We need to determine what fields are possible Ooyala fields for the
    // style to use for embed codes. Unfortunately, the field info in the view
    // object doesn't contain field types. Instead, generate a list of all
    // Ooyala fields, and compare that with the fields that are actually in the
    // view.
    $possible_ooyala_fields = array();
    $fields_info = field_info_instances();
    foreach ($fields_info as $entity_type => $bundles) {
      foreach ($bundles as $bundle_name => $bundle) {
        foreach ($bundle as $field_name => $field) {
          $field_info = field_info_field($field_name);
          if ($field_info['type'] == 'ooyala') {
            $possible_ooyala_fields[$field['field_name']] = $field['field_name'];
          }
        }
      }
    }

    // Create an array of allowed columns from the data we know:
    $field_names = array_intersect_key($this->display->handler->get_field_labels(), $possible_ooyala_fields);

    $form['embed_code_field'] = array(
      '#type' => 'select',
      '#title' => t('Embed code field'),
      '#description' => t('Select the field that contains the embed code value for the video'),
      '#options' => $field_names,
      '#default_value' => isset($this->options['embed_code_field']) ? $this->options['embed_code_field'] : array_shift(array_keys($field_names)),
      '#required' => TRUE,
    );

    // Also let the user select what field to use as the title of each video.
    $form['title_field'] = array(
      '#type' => 'select',
      '#title' => t('Title field'),
      '#description' => t('Select the field that contains the title for each video'),
      '#options' => $this->display->handler->get_field_labels(),
      '#default_value' => isset($this->options['title_field']) ? $this->options['title_field'] : NULL,
      '#requred' => TRUE,
    );
  }

  /**
   * Ensure that the pager option is disabled as this makes no sense with the
   * shared player style.
   */
  function validate() {
    $errors = parent::validate();
    if ($this->display->handler->use_pager()) {
      $errors[] = t('The Ooyala shared player style cannot be used with a pager. Disable the "Use pager" option for this display.');
    }
    return $errors;
  }
}

