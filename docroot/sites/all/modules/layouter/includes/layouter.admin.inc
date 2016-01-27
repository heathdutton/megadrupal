<?php

/**
 * Module settings form.
 */
function layouter_config_main_form() {
  // The list of all text formats in the system.
  $filter_formats = filter_formats();
  // The list of all content types in the system.
  $node_types = node_type_get_types();
  $image_styles = image_styles();
  $text_formats = array();
  foreach ($filter_formats as $format) {
    $text_formats[$format->format] = $format->name;
  }
  $content_types = array();
  foreach ($node_types as $node_type) {
    $content_types[$node_type->type] = $node_type->name;
  }
  $image_styles_options = array();
  foreach ($image_styles as $k => $v) {
    $image_styles_options[$k] = $k;
  }
  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $form = array();
  $form['layouter_text_formats'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Text formats'),
    '#description' => t('Choose input formats for which you want to enable Layouter<br />Attention : Make sure that the choosed formats allowed HTML tags: &lt;img&gt; &lt;div&gt; &lt;p&gt; and also tags used in layouter-extension modules.'),
    '#options' => $text_formats,
    '#default_value' => variable_get('layouter_text_formats', $text_formats),
  );
  $form['layouter_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#description' => t('Choose content types for which you want to enable Layouter'),
    '#options' => $content_types,
    '#default_value' => variable_get('layouter_content_types', $content_types),
  );
  $form['layouter_image_styles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Image styles'),
    '#description' => t('Choose image styles for which you want to enable alter the original image in Layouter'),
    '#options' => $image_styles_options,
    '#default_value' => variable_get('layouter_image_styles', $image_styles_options),
  );
  $form['layouter_modal_style'] = array(
    '#type' => 'checkbox',
    '#title' => t('Layouter modal style'),
    '#description' => t("Use Layouter's style for Chaostools - modal"),
    '#default_value' => variable_get('layouter_modal_style', 'checked'),
  );
  /**
   * @todo Drupal system_settings_form() doesn't save a radios or a selects
   *   values to variable. So we should upload form throught submit(). Find the
   *   beauty way to change it.
   */ 
  $form['uri_scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => $scheme_options,
    '#default_value' => variable_get('layouter_uri_scheme', key($scheme_options)),
    '#description' => t('Select where the final files should be stored. Private file storage has significantly more overhead than public files, but allows restricted access to files within this field.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Module settings form submit.
 */
function layouter_config_main_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  variable_set('layouter_text_formats', $values['layouter_text_formats']);
  variable_set('layouter_content_types', $values['layouter_content_types']);
  variable_set('layouter_image_styles', $values['layouter_image_styles']);
  variable_set('layouter_modal_style', $values['layouter_modal_style']);
  variable_set('layouter_uri_scheme', $values['uri_scheme']);
}

