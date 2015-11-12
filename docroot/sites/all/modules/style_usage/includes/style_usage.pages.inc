<?php

/**
 * @file
 *   Page generation functions.
 *
 *  @developers:
 *    Gabor Szanto <hello@szantogabor.com>
 *    http://szantogabor.com
 *
 */

/**
 * The style usage main page rendering function.
 */
function style_usage_page() {
  $styles = image_styles();
  $output = '';
  $img_portrait = variable_get('style_usage_portrait');
  $img_landscape = variable_get('style_usage_landscape');
  $collapsed = variable_get('style_usage_collapsed_default', 0) ? 'collapsed' : '';

  foreach ($styles as $style) {
    // Need to pass this html id to another build functions.
    $style['html_id'] = 'fieldset-' . drupal_html_id($style['name']);
    $element = '';
    $descr = t('This style is defined by the <strong>@module</strong> module.', array('@module' => $style['module']));

    $descr .= ' ' . l(t('Edit the @style image style.', array('@style' => $style['name'])), "admin/config/media/image-styles/edit/{$style['name']}");
    $element .= _style_usage_get_effects_table($style);

    if (module_exists('views')) {
      $element .= _style_usage_get_views_table($style);
    }
    $element .= _style_usage_get_entity_table($style);

    if (module_exists('file_entity')) {
      $element .= _style_usage_get_file_table($style);
    }

    $element .= '<h3>' . t('Landscape image:') . '</h3>';
    $element .= is_null($img_landscape) ? theme('image_style_preview', array('style' => $style)) :
      theme('image_style', array('path' => $img_landscape, 'style_name' => $style['name']));

    $element .= '<h3>' . t('Portrait image:') . '</h3>';
    $element .= is_null($img_portrait) ? theme('image_style_preview', array('style' => $style)) :
      theme('image_style', array('path' => $img_portrait, 'style_name' => $style['name']));


    $fieldset = array(
      '#children' => $element,
      '#type' => 'fieldset',
      '#title' => t('Image style name: <strong>@style</strong>', array('@style' => $style['name'])),
      '#description' => $descr,
      '#attributes' => array(
        'class' => array('collapsible'),
        'id' => $style['html_id'],
      ),
    );
    if ($collapsed) {
      $fieldset['#attributes']['class'][] = $collapsed;
    }
    $output .= render($fieldset);
  }
  if ($output) {
    // Allow collapsible form elements.
    drupal_add_library('system', 'drupal.collapse');
  }
  return $output;
}

/**
 * Form builder for preview page
 */
function style_usage_preview_form($form, &$form_state) {
  $uri = isset($_GET['uri']) ? $_GET['uri'] : NULL;
  $styles = image_styles();
  $form = array(
    'uri' => array(
      '#type' => 'textfield',
      '#title' => t('Select a file'),
      '#description' => t('Select a file to preview'),
      '#maxlength' => 200,
      '#autocomplete_path' => 'style_usage_preview/autocomplete',
      '#default_value' => $uri,
      '#weight' => -1,
      '#required' => TRUE,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Preview image'),
    ),
  );

  if (file_valid_uri($uri)) {
    $form['preview'] = array(
      '#markup' => theme('style_usage_preview_images', array('styles' => image_styles(), 'uri' => $uri)) ,
    );
  }
  elseif (!is_null($uri)) {
    drupal_set_message(t('The uri is not valid'), 'error');
  }
  return $form;
}

/**
 * Form submit callback on preview page.
 */
function style_usage_preview_form_submit($form, &$form_state) {
  $form_state['redirect'] = url('admin/config/media/image-styles/usage/preview', array(
    'absolute' => TRUE,
    'query' => array(
      'uri' => $form_state['values']['uri'],
    ),
  ));
}

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions
 * for existing files.
 */
function style_usage_preview_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $result = db_select('file_managed')->fields('file_managed', array('filename', 'uri'))->condition('filename', db_like($string) . '%', 'LIKE')->range(0, 10)->execute();
    foreach ($result as $file) {
      $matches[$file->uri] = check_plain($file->filename);
    }
  }
  drupal_json_output($matches);
}

/**
 * Helper function to create html for $style effects table
 */
function _style_usage_get_effects_table($style) {
  $output = '<h3>' . t('Effects used') . ':</h3>';

  $header = array(t('Label'), t('Module'), t('Parameters'), t('Edit'));
  $rows = array();
  foreach ($style['effects'] as $effect) {
    $list = array();
    foreach ($effect['data'] as $effect_key => $value) {
      if (is_array($value)) {
        $value_sub_items = array();
        foreach ($value as $sub_item_key => $item) {
          $value_sub_items[$effect_key][] = $sub_item_key . ': ' . $item;
        }
        $list[] = $effect_key . ': ' . theme('item_list', array('items' => $value_sub_items[$effect_key]));
      }
      else {
        $list[] = $effect_key . ': ' . $value;
      }
    };

    if (isset($effect['ieid'])) {
      $edit_text = l(t('Edit effect'), "admin/config/media/image-styles/edit/{$style['name']}/effects/{$effect['ieid']}", array(
        'query' => array(
          'destination' => $_GET['q'] . '#' . $style['html_id'],
        ),
      ));
    }
    else {
      $edit_text = t('This style is not overridden yet, you should edit the style first.');
    }
    $rows[] = array(
      $effect['label'],
      $effect['module'],
      theme('item_list', array('items' => $list)),
      $edit_text,
    );
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Helper function to create html for $style views usage table
 */
function _style_usage_get_views_table($style) {
  $usage = style_usage_get_views_usage($style['name']);
  $output = '<h3>' . t('Style used in views') . ':</h3>';

  if (!empty($usage)) {
    $header = array(t('View name'), t('Display'), t('Field'));
    $rows = array();
    foreach ($usage as $value) {
      $rows[] = array($value['view_name'], $value['display'], $value['field']);
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= '<p>' . t('There are no views usage in this style.') . '</p>';
  }
  return $output;
}

/**
 * Helper function to create html for $style entity usage table
 */
function _style_usage_get_entity_table($style) {
  $usage = style_usage_get_entity_usage($style['name']);
  $output = '<h3>' . t('Style used in entities') . ':</h3>';

  if (!empty($usage)) {
    $header = array(t('Entity name'), t('Bundle'), t('Field'), t('Display name'));
    $rows = array();
    foreach ($usage as $value) {
      $rows[] = array($value['entity'], $value['bundle'], $value['field'], $value['display']);
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= '<p>' . t('There are no entity usage in this style.') . '</p>';
  }

  return $output;
}

/**
 * Helper function to create html for $style file usage table
 */
function _style_usage_get_file_table($style) {
  $usage = style_usage_get_file_usage($style['name']);
  $output = '<h3>' . t('Style used in file styles') . ':</h3>';

  if (!empty($usage)) {
    $header = array(t('File type'), t('View mode'), t('Display name'));
    $rows = array();
    foreach ($usage as $value) {
      $rows[] = array($value['file_type'], $value['view_mode'], $value['display']);
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= '<p>' . t('There are no file usage in this style.') . '</p>';
  }

  return $output;
}
