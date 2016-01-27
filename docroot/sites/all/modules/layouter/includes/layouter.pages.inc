<?php

/**
 * Callback for layouter/%ctools_js/% path.
 * 
 * Runs multistep wizard form.
 */
function layouter_choose_layout($textarea_id = NULL, $step = NULL) {
  ctools_include('modal');
  ctools_include('ajax');
  require_once 'layouter.inc';
  $form_info = array(
    'id' => 'layouter',
    'path' => 'layouter/' . $textarea_id . '/%step',
    'show trail' => FALSE,
    'show back' => FALSE,
    'show cancel' => TRUE,
    'show return' => FALSE,
    'next text' => t('Next'),
    'next callback' => 'layouter_wizard_next',
    'finish callback' => 'layouter_wizard_finish',
    'cancel callback' => 'layouter_wizard_cancel',
    // This controls order, as well as form labels.
    'order' => array(
      'start' => t('Choose layout template'),
    ),
    // Here we map a step to a form id.
    'forms' => array(
      // E.g. this for the step at wombat/create.
      'start' => array(
        'form id' => 'layouter_choose_layout_form',
      ),
    ),
  );
  $object_id = 1;
  if (empty($step)) {
    // We reset the form when $step is NULL
    // because that means they have for whatever reason started over.
    layouter_cache_clear($object_id);
    $step = 'start';
  }
  // This automatically gets defaults if there wasn't anything saved.
  $object = layouter_cache_get($object_id);
  $templates = layouter_templates();
  // Make sure we can't somehow accidentally go to an invalid template.
  if (empty($templates[$object->type])) {
    $object->type = 'unknown';
  }
  // Now that we have our object, dynamically add the template form.
  if ($object->type == 'unknown') {
    // If they haven't selected a type, add a form that doesn't exist yet.
    $form_info['order']['unknown'] = t('Template content');
    $form_info['forms']['unknown'] = array('form id' => 'nothing');
  }
  else {
    // Add the selected template to the order so that it shows up properly in
    // the trail.
    $form_info['order'][$object->type] = $templates[$object->type]['config title'];
  }
  // Make sure all templates forms are represented so that the next stuff can
  // work correctly.
  foreach ($templates as $id => $template) {
    $form_info['forms'][$id] = array(
      'form id' => $templates[$id]['form'],
      'form title' => t('Layout content'),
      'theme' => $template['theme'],
    );
  }
  $form_state = array(
    'ajax' => 'ajax',
    // Put our object and ID into the form state cache so we can easily find it.
    'object_id' => $object_id,
    'object' => &$object,
    'form_info' => $form_info,
    'textarea_id' => $textarea_id,
  );
  // Send this all off to our form. This is like drupal_get_form.
  ctools_include('wizard');
  $form = ctools_wizard_multistep_form($form_info, $step, $form_state);
  $form['buttons']['cancel']['#attributes']['class'][] = 'layouter-close';
  $output = drupal_render($form);
  if ($output === FALSE || !empty($form_state['complete'])) {
    // This creates a string based upon the template and its setting using
    // function indirection.
    $template = $templates[$object->type]['output']($object);
  }
  $commands = array();
  if (($output === FALSE || !empty($form_state['complete'])) || !empty($form_state['cancel'])) {
    // Dismiss the modal.
    $commands[] = ctools_modal_command_dismiss();
  }
  else {
    $commands = ctools_modal_form_render($form_state, $output);
  }
  print ajax_render($commands);
  exit;
}

/**
 * Returns form for choose a template.
 */
function layouter_choose_layout_form($form, &$form_state) {
  $form_state['title'] = t('Layouter');
  $templates = layouter_templates();
  $options = array();
  foreach ($templates as $id => $template) {
    $options[$id] = $template['title'];
  }
  $form['type'] = array(
    '#title' => t('Choose the layout'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => $form_state['object']->type,
    '#required' => 1,
    '#after_build' => array('_layouter_process_layout_type_radios'),
  );
  $form['#attributes']['class'][] = 'layouter-form';
  $module_path = drupal_get_path('module', 'layouter');
  $form['#attached']['css'] = array($module_path . '/theme/layouter.css');
  $form['#attached']['js'] = array($module_path . '/theme/layouter_inside.js');
  $form['#attached']['js'][] = array(
    'data' => array('layouter_modal_style' => variable_get('layouter_modal_style', 0)),
    'type' => 'setting',
  );
  return $form;
}

/**
 * Moves to the form of selected template.
 */
function layouter_choose_layout_form_submit($form, &$form_state) {
  $form_state['object']->type = $form_state['values']['type'];
  // Override where to go next based on the template selected.
  $form_state['clicked_button']['#next'] = $form_state['values']['type'];
}

/**
 * Returns Form for adding content for img_only layout.
 */
function layouter_img_only_form($form, &$form_state) {
  $form_state['title'] = $form_state['form_info']['forms'][$form_state['step']]['form title'];
  $form['image'] = layouter_image_content();
  $form['image_style'] = layouter_image_style();
  return $form;
}

/**
 * Proccess Layouter Form with img_only.
 */
function layouter_img_only_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $image_fid = $values['image'];
  $image_style = $values['image_style'];
  $image_file = file_load($image_fid);
  $image_file->status = FILE_STATUS_PERMANENT;
  file_save($image_file);
  $style_theme = ($image_style == 'layouter_none') ? 'image' : 'image_style';
  $image = theme($style_theme, array('style_name' => $image_style, 'path' => $image_file->uri));
  $textarea_id = $form_state['textarea_id'];
  // Apply the layout to the content.
  $content = theme('layouter_image_only', array('image' => $image));
  // Insert it into textarea field.
  layouter_insert_content_into_textarea($textarea_id, $content);
}

/**
 * Returns Form for adding content for two_columns layout.
 */
function layouter_two_columns_form($form, &$form_state) {
  $form_state['title'] = $form_state['form_info']['forms'][$form_state['step']]['form title'];
  $form['text_content'] = layouter_text_content();
  return $form;
}

/**
 * Proccess Layouter Form with two_columns.
 */
function layouter_two_columns_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $text_content = strip_tags($values['text_content']);
  $textarea_id = $form_state['textarea_id'];
  // Apply the layout to the content.
  $content = theme('layouter_two_columns', array('text' => $text_content));
  // Insert it into textarea field.
  layouter_insert_content_into_textarea($textarea_id, $content);
}

/**
 * Returns Form for adding content for two_columns_img_left_text_right layout.
 */
function layouter_img_caption_text_form($form, &$form_state) {
  $form_state['title'] = $form_state['form_info']['forms'][$form_state['step']]['form title'];
  $form['text_content'] = layouter_text_content();
  $form['image_content'] = array(
    '#type' => 'fieldset',
    '#tree' => 1,
    '#title' => t('Image'),
  );
  $form['image_content']['image'] = layouter_image_content();
  $form['image_content']['image_style'] = layouter_image_style();
  $form['image_content']['caption'] = array(
    '#type' => 'textarea',
    '#title' => t('Caption'),
  );
  return $form;
}

/**
 * Proccess Layouter Form with two_columns_img_left_text_right.
 */
function layouter_img_caption_text_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $text_content = strip_tags($values['text_content']);
  $image_fid = $values['image_content']['image'];
  $image_style = $values['image_content']['image_style'];
  $caption = strip_tags($values['image_content']['caption']);
  $image_file = file_load($image_fid);
  $image_file->status = FILE_STATUS_PERMANENT;
  file_save($image_file);
  $style_theme = ($image_style == 'layouter_none') ? 'image' : 'image_style';
  $image = theme($style_theme, array('style_name' => $image_style, 'path' => $image_file->uri));
  $textarea_id = $form_state['textarea_id'];
  // As we use the same form for different layouts we need to know which
  // template to apply.
  $theme = $form_state['form_info']['forms'][$form_state['step']]['theme'];
  // Apply the layout to the content.
  $attributes_array = array(
    'text' => $text_content,
    'image' => $image,
    'caption' => $caption,
  );
  $content = theme($theme, $attributes_array);
  // Insert it into textarea field.
  layouter_insert_content_into_textarea($textarea_id, $content);
}

/**
 * Returns Form for adding content for two_columns_text_img_left layout.
 */
function layouter_text_img_form($form, &$form_state) {
  $form_state['title'] = $form_state['form_info']['forms'][$form_state['step']]['form title'];
  $form['text_content'] = layouter_text_content();
  $form['image_content'] = array(
    '#type' => 'fieldset',
    '#tree' => 1,
    '#title' => t('Image'),
  );
  $form['image_content']['image'] = layouter_image_content();
  $form['image_content']['image_style'] = layouter_image_style();
  return $form;
}

/**
 * Proccess Layouter Form with two_columns_text_img_left.
 */
function layouter_text_img_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $text_content = strip_tags($values['text_content']);
  $image_fid = $values['image_content']['image'];
  $image_style = $values['image_content']['image_style'];
  $image_file = file_load($image_fid);
  $image_file->status = FILE_STATUS_PERMANENT;
  file_save($image_file);
  $style_theme = ($image_style == 'layouter_none') ? 'image' : 'image_style';
  $image = theme($style_theme, array('style_name' => $image_style, 'path' => $image_file->uri));
  $textarea_id = $form_state['textarea_id'];
  // As we use the same form for different layouts we need to know which
  // template to apply.
  $theme = $form_state['form_info']['forms'][$form_state['step']]['theme'];
  // Apply the layout to the content.
  $content = theme($theme, array('text' => $text_content, 'image' => $image));
  // Insert it into textarea field.
  layouter_insert_content_into_textarea($textarea_id, $content);
}

/**
 * Returns the 'textarea' form item.
 */
function layouter_text_content() {
  return array(
    '#type' => 'textarea',
    '#title' => t('Text'),
    '#rows' => 15,
    '#required' => 1,
  );
}

/**
 * Returns the form item with actual settings, to upload image.
 */
function layouter_image_content() {
  $allowed_extensions = array('png gif jpeg jpg');
  $max_upload_size_mb = (int) ini_get('upload_max_filesize');
  $max_upload_size = array($max_upload_size_mb * 1024 * 1024);
  $image_field_description = t('Files must be less than !size.', array('!size' => '<strong>' . format_size($max_upload_size[0]) . '</strong>'));
  $image_field_description .= '<br />' . t('Allowed file types: !extensions.', array('!extensions' => '<strong>' . $allowed_extensions[0] . '</strong>'));
  $location_scheme = variable_get('layouter_uri_scheme', 'public');
  return array(
    '#type' => 'managed_file',
    '#title' => t('Image'),
    '#description' => $image_field_description,
    '#required' => 1,
    '#upload_location' => $location_scheme . '://layouter_images',
    '#upload_validators' => array(
      'file_validate_extensions' => $allowed_extensions,
      'file_validate_size' => array($max_upload_size),
    ),
  );
}

/**
 * Returns image styles for prerendering image.
 */
function layouter_image_style() {
  if (user_access('administer image styles')) {
    $description = 'You can also <a href="@image_styles">'
      . 'add your own image style</a> if you need to.';
    $extra = array('@image_styles' => url('admin/config/media/image-styles'));
    $admin_image_style_description = '<br />' . t($description, $extra);
  }
  else {
    $admin_image_style_description = '';
  }
  $image_styles = variable_get('layouter_image_styles', image_styles());
  $image_styles_options = array();
  $image_styles_options['layouter_none'] = 'none';
  foreach ($image_styles as $k => $v) {
    if ($v != '0') {
      $image_styles_options[$k] = $k;
    }
  }
  return array(
    '#type' => 'select',
    '#title' => t('Image style'),
    '#options' => $image_styles_options,
    '#default_value' => 'layouter_none',
    '#description' => $admin_image_style_description,
  );
}
