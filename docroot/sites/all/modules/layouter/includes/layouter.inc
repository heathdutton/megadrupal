<?php

/**
 * Wraps each radio button item in the 'radios' set into additional container.
 * 
 * After-build callback
 *
 * @param array $element
 *   Form element.
 */
function _layouter_process_layout_type_radios($element) {
  if ($element['#type'] == 'radios') {
    $templates = layouter_templates();
    foreach ($templates as $id => $template) {
      if (!empty($element[$id])) {
        $element[$id]['#prefix'] = '<div class="layouter-radio-wrapper ' . $id . '" title="' . $template['title'] . '">';
        $element[$id]['#suffix'] = '</div>';
      }
    }
  }
  return $element;
}

/**
 * Enables Layouter for given textarea field.
 */
function layouter_load_by_field($field, $format) {
  static $processed_ids = array();
  $cond1 = !isset($field['#id']) || isset($processed_ids[$field['#id']]) || $field['#id'] == 'edit-log';
  $cond2 = isset($field['#access']) && !$field['#access'];
  $cond3 = isset($field['#attributes']['disabled']) && $field['#attributes']['disabled'] == 'disabled';
  if ($cond1 || $cond2 || $cond3) {
    return $field;
  }
  $format_arr = FALSE;
  if (is_array($format)) {
    $format_arr = $format;
    $format = isset($format_arr['#value']) ? $format_arr['#value'] : $format_arr['#default_value'];
  }
  drupal_add_js(array('layouter' => array('default_formats' => array($field['#id'] => $format))), 'setting');
  if (!isset($processed_ids[$field['#id']])) {
    $processed_ids[$field['#id']] = array();
  }
  $textarea_id = $field['#id'];
  $class = 'layouter';
  $layouter_link_attrs = array(
    'attributes' => array(
      'id' => array('layouter-' . $textarea_id),
      'class' => array('layouter-link', 'ctools-use-modal', $textarea_id),
      'title' => t('Click to select the text template with a simplified form of layout'),
    ),
  );
  $suffix = '<div class="layouter-link-wrapper">';
  $suffix .= l(t('Select the text template'), 'layouter/' . $textarea_id, $layouter_link_attrs);
  $suffix .= '</div>';
  // Remember extra information and reuse it during "Preview"
  $processed_ids[$field['#id']]['suffix'] = $suffix;
  $processed_ids[$field['#id']]['class'][] = $class;
  $field['#suffix'] = (empty($field['#suffix'])) ? $suffix : $field['#suffix'] . $suffix;
  $field['#attributes']['class'][] = $class;
  return $field;
}

/**
 * Store our cache so that we can retain data from form to form.
 */
function layouter_cache_set($id, $object) {
  ctools_include('object-cache');
  ctools_object_cache_set('layouter', $id, $object);
}

/**
 * Get the current object from the cache, or default.
 */
function layouter_cache_get($id) {
  ctools_include('object-cache');
  $object = ctools_object_cache_get('layouter', $id);
  if (!$object) {
    // Create a default object.
    $object = new stdClass();
    $object->type = 'unknown';
    $object->name = '';
  }
  return $object;
}

/**
 * Clear the wizard cache.
 */
function layouter_cache_clear($id) {
  ctools_include('object-cache');
  ctools_object_cache_clear('layouter', $id);
}

/**
 * Handle the 'next' click on the add/edit pane form wizard.
 *
 * All we need to do is store the updated pane in the cache.
 */
function layouter_wizard_next(&$form_state) {
  layouter_cache_set($form_state['object_id'], $form_state['object']);
}

/**
 * Handle the 'finish' click on the add/edit pane form wizard.
 *
 * All we need to do is set a flag so the return can handle adding the pane.
 */
function layouter_wizard_finish(&$form_state) {
  $form_state['complete'] = TRUE;
}

/**
 * Handle the 'cancel' click on the add/edit pane form wizard.
 */
function layouter_wizard_cancel(&$form_state) {
  $form_state['cancel'] = TRUE;
}

/**
 * Gets a list of our templates and associated forms.
 *
 * What we're doing is making it easy to add more templates in just one place,
 * which is often how it will work in the real world. If using CTools, what
 * you would probably really have here, is a set of plugins for each template.
 */
function layouter_templates() {
  $items = module_invoke_all('layouter_templates');
  foreach ($items as &$item) {
    $item['config title'] = t('Layout content');
  }
  return $items;
}

/**
 * Implements hook_layouter_templates().
 */
function layouter_layouter_templates() {
  $items = array(
    'image_only' => array(
      'title' => t('One big image only'),
      'form' => 'layouter_img_only_form',
      'theme' => 'layouter_image_only',
    ),
    'two_columns' => array(
      'title' => t('Two columns of continuous text'),
      'form' => 'layouter_two_columns_form',
      'theme' => 'layouter_two_column',
    ),
    'two_columns_img_left_text_right' => array(
      'title' => t('Two columns with an image (with an optional description) on the left side and a text on the right'),
      'form' => 'layouter_img_caption_text_form',
      'theme' => 'layouter_two_columns_img_left_text_right',
    ),
    'two_columns_img_right_text_left' => array(
      'title' => t('Two columns with an image (with an optional description) on the right side and a text on the left'),
      'form' => 'layouter_img_caption_text_form',
      'theme' => 'layouter_two_columns_img_right_text_left',
    ),
    'two_columns_text_img_left' => array(
      'title' => t('Two columns of continuous text with an image on top left'),
      'form' => 'layouter_text_img_form',
      'theme' => 'layouter_two_columns_text_img_left',
    ),
    'big_img_text_below' => array(
      'title' => t('Big image on top with a text below'),
      'form' => 'layouter_text_img_form',
      'theme' => 'layouter_big_img_text_below',
    ),
    'big_img_text_above' => array(
      'title' => t('Big image at bottom with a text above'),
      'form' => 'layouter_text_img_form',
      'theme' => 'layouter_big_img_text_above',
    ),
    'big_img_two_column_text_below' => array(
      'title' => t('Big image on top with a two columns of text below'),
      'form' => 'layouter_text_img_form',
      'theme' => 'layouter_big_img_two_column_text_below',
    ),
    'big_img_two_column_text_above' => array(
      'title' => t('Big image at bottom with a two columns of text above'),
      'form' => 'layouter_text_img_form',
      'theme' => 'layouter_big_img_two_column_text_above',
    ),
  );
  return $items;
}

/**
 * Gets the list of text formats for which Layouter is enabled.
 * 
 * @return array
 *   Array of allowed formats.
 */
function layouter_active_text_formats() {
  $layouter_text_formats = variable_get('layouter_text_formats', array());
  $text_formats_enabled = array();
  if ($layouter_text_formats) {
    foreach ($layouter_text_formats as $text_format) {
      if ($text_format) {
        $text_formats_enabled[] = $text_format;
      }
    }
  }
  return $text_formats_enabled;
}

/**
 * Gets the list of content types for which Layouter is enabled.
 * 
 * @return array
 *   Array of allowed types.
 */
function layouter_active_content_types() {
  $layouter_content_types = variable_get('layouter_content_types', array());
  $content_types_enabled = array();
  if ($layouter_content_types) {
    foreach ($layouter_content_types as $content_type) {
      if ($content_type) {
        $content_types_enabled[] = $content_type;
      }
    }
  }
  return $content_types_enabled;
}

/**
 * Inserts content, which was templated with Layouter, into textarea.
 */
function layouter_insert_content_into_textarea($textarea_id, $content) {
  // Extract the id.
  $id = str_replace('layouter-', '', $textarea_id);
  $commands = array();
  // Command to insert into textarea itself.
  $commands[] = ajax_command_invoke(NULL, 'addContent', array($content, $id));
  // Command to insert into CKEditor.
  $commands[] = ajax_command_invoke(NULL, 'layouter', array($textarea_id, $content));
  // Close the pop-up.
  $commands[] = ctools_modal_command_dismiss();
  print ajax_render($commands);
  exit;
}
