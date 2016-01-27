<?php
/**
 * @file
 */

/**
 * WebcutateAPI - Commerce settings form.
 */
function webducateapi_commerce_settings($form, $form_state) {
  $settings = variable_get('webducateapi_commerce', webducateapi_commerce_defaults());

  $form['webducateapi_commerce'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $form['webducateapi_commerce']['enabled'] = array(
    '#title' => t('Enabled'),
    '#type' => 'checkbox',
    '#default_value' => $settings['enabled'],
  );

  $map_fields = array(
    'body' => array(
      'types' => array('text', 'text_long', 'text_with_summary'),
    ),
    'product_reference' => array(
      'types' => array('commerce_product_reference'),
    ),
  );

  // Determine which content types can be used for field mapping.
  $content_types = array();
  foreach (node_type_get_names() as $bundle_name => $title) {
    $content_types[$bundle_name] = array(
      'title' => $title,
    );

    $instances = field_info_instances('node', $bundle_name);
    foreach ($instances as $instance) {
      $field = field_info_field_by_id($instance['field_id']);
      foreach ($map_fields as $key => $data) {
        if (in_array($field['type'], $data['types'])) {
          $content_types[$bundle_name]["{$key}_fields"][] = $instance;
        }
      }
    }

    if (empty($content_types[$bundle_name]['product_reference_fields'])) {
      unset($content_types[$bundle_name]);
    }
  }

  if (!empty($content_types)) {
    $options = array();
    foreach ($content_types as $key => $data) {
      $options[$key] = $data['title'];
    }
    $default_value = !empty($settings['content_type']) ? $settings['content_type'] : key($options);
    $default_value = isset($options[$default_value]) ? $default_value : key($options);
    $form['webducateapi_commerce']['content_type'] = array(
      '#title' => t('Content type'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $default_value,
      '#ajax' => array(
        'callback' => 'webducateapi_commerce_settings_ajax_callback',
        'wrapper' => 'map-wrapper',
      ),
      '#states' => array(
        'invisible' => array(
          ':input[name="webducateapi_commerce[enabled]"]' => array('checked' => FALSE),
        ),
      ),
    );

    $form['webducateapi_commerce']['map'] = array(
      '#title' => t('Field map'),
      '#type' => 'fieldset',
      '#prefix' => '<div id="map-wrapper">',
      '#suffix' => '</div>',
      '#states' => array(
        'invisible' => array(
          ':input[name="webducateapi_commerce[enabled]"]' => array('checked' => FALSE),
        ),
      ),
    );

    // Get currently selected content type.
    $content_type = isset($form_state['values']['webducateapi_commerce']['content_type'])
      ? $form_state['values']['webducateapi_commerce']['content_type']
      : $form['webducateapi_commerce']['content_type']['#default_value'];

    // Product reference field.
    $options = array();
    foreach ($content_types[$content_type]['product_reference_fields'] as $instance) {
      $options[$instance['field_name']] = $instance['label'];
    }
    $default_value = $settings['content_type'] == $content_type ? $settings['map']['product_reference'] : key($options);
    $default_value = !empty($default_value) ? $default_value : key($options);
    $default_value = isset($options[$default_value]) ? $default_value : key($options);
    $form['webducateapi_commerce']['map']['product_reference'] = array(
      '#title' => t('Product reference'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $default_value,
      '#required' => TRUE,
    );

    // Body field.
    $options = array();
    foreach ($content_types[$content_type]['body_fields'] as $instance) {
      $options[$instance['field_name']] = $instance['label'];
    }
    $default_value = $settings['content_type'] == $content_type ? $settings['map']['body'] : '';
    $default_value = !empty($default_value) || isset($options[$default_value]) ? $default_value : $default_value;
    $form['webducateapi_commerce']['map']['body'] = array(
      '#title' => t('Body'),
      '#type' => 'select',
      '#options' => $options,
      '#empty_value' => '',
      '#default_value' => $default_value,
    );
  }

  else {
    $form['webducateapi_commerce']['enabled']['#value'] = FALSE;
    $form['webducateapi_commerce']['enabled']['#disabled'] = TRUE;

    // @TODO - Add better help message here.
    $form['help'] = array(
      '#markup' => t('There are no Content types with the required fields to allow automated WebducateAPI product referencing.'),
    );
  }

  return system_settings_form($form);
}

/**
 * AJAX callback for  WebcutateAPI - Commerce settings form.
 */
function webducateapi_commerce_settings_ajax_callback($form, $form_state) {
  return $form['webducateapi_commerce']['map'];
}
