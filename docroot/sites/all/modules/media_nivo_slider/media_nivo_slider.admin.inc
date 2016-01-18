<?php

/**
 * @file
 *  Provides admin page callbacks for adding/editing/deleting Media Nive Slider configurations.
 */

/*
 * Admin overview of Nivo Slider configurations
 */
function media_nivo_slider_admin() {
  // Initialize the output.
  $output = '';
  
  // Table header.
  $header = array(
    t('Name'),
    array(
      'data' => t('Operations'),
      'colspan' => 3,
    )
  );
  
  // Pull the available configurations from the db and list them in the overview table.
  $rows = array();
  $result = db_select('media_nivo_slider', 'ns')
    ->fields('ns', array('cid', 'name', 'settings'))
    ->execute();
  
  // Iterate through the results and build out the table data.
  foreach ($result as $config) {
    $name = l($config->name, 'admin/config/media/media-nivo-slider/edit/' . $config->cid);
    $ops = l(t('Edit'), 'admin/config/media/media-nivo-slider/edit/' . $config->cid) . ' | ' .
                l(t('Delete'), 'admin/config/media/media-nivo-slider/delete/' . $config->cid);
    $rows[] = array($name, $ops);
  }
  
  // Theme variables for building the overview table.
  $vars = array(
    'header' => $header, 
    'rows' => $rows, 
    'empty' => t('There are currently no Nivo Slider configurations. Add a new nuni gallery using the link above.'),
  );
  
  // Create the overview table.
  $output = theme('table', $vars);
  
  // Return the formatted table.
  return $output;
}

/*
 * Create/Edit form for Nivo Slider configurations.
 */
function media_nivo_slider_config($form, &$form_state, $cid = NULL) { 
  if (isset($cid)) {
    $config = db_select('media_nivo_slider', 'ns')
      ->fields('ns')
      ->condition('cid', $cid)
      ->execute()
      ->fetchObject();

    $settings = unserialize($config->settings);
  }

  $form = array();

  // Configuration name.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $config->name : ''),
  );
  
  // Vertical tabs setup.
  $form['vtabs'] = array(
    '#type' => 'vertical_tabs',
  );
  
  // Transition settings fieldset.
  $form['transition_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transition'),
    '#group' => 'vtabs',
  );
  
  // Slide transition effect.
  $form['transition_settings']['transition'] = array(
    '#type' => 'select',
    '#title' => t('Slide Transition Effect'),
    '#required' => TRUE,
    '#options' => _media_nivo_slider_get_effect_values(),
    '#default_value' => (isset($config) ? $settings['transition'] : 'random'),
  );
  
  // Slide transition speed.
  $form['transition_settings']['transition_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Slide Transition Speed'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['transition_speed'] : 500),
  );
  
  // Slide pause length.
  $form['transition_settings']['pause_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Slide Pause Length'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['pause_length'] : 3000),
  );

  // Transition slices.
  $form['transition_settings']['slices'] = array(
    '#type' => 'textfield',
    '#title' => t('Slices'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['slices'] : 15),
  );
  
  // Box Columns.
  $form['transition_settings']['box_columns'] = array(
    '#type' => 'textfield',
    '#title' => t('Box Columns'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['box_columns'] : 8),
  );
  
  // Box Rows.
  $form['transition_settings']['box_rows'] = array(
    '#type' => 'textfield',
    '#title' => t('Box Rows'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['box_rows'] : 4),
  );
  
  // Start slide.
  $form['transition_settings']['start_slide'] = array(
    '#type' => 'textfield',
    '#title' => t('Start Slide'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['start_slide'] : 0),
  );
  
  // Start on a random slide.
  $form['transition_settings']['random_start'] = array(
    '#type' => 'checkbox',
    '#title' => t('Start on a random slide.'),
    '#default_value' => (isset($config) ? $settings['random_start'] : FALSE),
  );
  
  // Pause slideshow on hover.
  $form['transition_settings']['pause_slideshow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause slideshow on hover.'),
    '#default_value' => (isset($config) ? $settings['pause_slideshow'] : FALSE),
  );
  
  // Name & theme settings fieldset.
  $form['theme'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#group' => 'vtabs',
  );
  
  // Image style to use.
  $form['theme']['image_style'] = array(
    '#type' => 'select',
    '#title' => t('Image Style'),
    '#options' => _media_nivo_slider_get_image_style_values(),
    '#default_value' => (isset($config) ? $settings['image_style'] : 'na'),
  );

  // Slider theme.
  $form['theme']['slider_theme'] = array(
    '#type' => 'select',
    '#title' => t('Nivo Slider theme'),
    '#required' => TRUE,
    '#options' => _media_nivo_slider_get_theme_values(),
    '#default_value' => (isset($config) ? $settings['slider_theme'] : 'default'),
  );
  
  // Display ribbon theme ribbon.
  $form['theme']['ribbon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display theme ribbon.'),
    '#default_value' => (isset($config) ? $settings['ribbon'] : FALSE),
  );
  
  // Caption opacity.
  $form['theme']['caption_opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Caption Opacity'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['caption_opacity'] : 0.80),
  );
  
  // Navigation fieldset.
  $form['navigation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navigation'),
    '#group' => 'vtabs',
  );
  
  // Enable Next & Prev navigation.
  $form['navigation']['direction_nav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable next & prev navigation.'),
    '#default_value' => (isset($config) ? $settings['direction_nav'] : TRUE),
  );

  // Direction nav hide.
  $form['navigation']['direction_nav_hide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only show next & prev on hover.'),
    '#default_value' => (isset($config) ? $settings['direction_nav_hide'] : TRUE),
  );
  
  // Prev direction navigation text.
  $form['navigation']['direction_nav_prev_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Navigation Text (Prev)'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['direction_nav_prev_txt'] : 'Prev'),
  );
  
  // Replace string for thumbnail filename search and replace processing.
  $form['navigation']['direction_nav_next_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Navigation Text (Next)'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['direction_nav_next_txt'] : 'Next'),
  );
  
  // Enable control nav (1,2,3).
  $form['navigation']['control_nav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable control navigation.'),
    '#default_value' => (isset($config) ? $settings['control_nav'] : TRUE),
  );
  
  // Use thumbnails for Control Nav.
  $form['navigation']['control_nav_thumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use thumbnails for control nav.'),
    '#default_value' => (isset($config) ? $settings['control_nav_thumbs'] : FALSE),
  );
  
  // Use image rel for thumbs.
  $form['navigation']['control_nav_rel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use image rel for thumbs.'),
    '#default_value' => (isset($config) ? $settings['control_nav_rel'] : FALSE),
  );

  // Search string for thumbnail filename search and replace processing.
  $form['navigation']['control_nav_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail search string'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['control_nav_search'] : '.jpg'),
  );
  
  // Replace string for thumbnail filename search and replace processing.
  $form['navigation']['control_nav_replace'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail replace string'),
    '#required' => TRUE,
    '#default_value' => (isset($config) ? $settings['control_nav_replace'] : '_thumb.jpg'),
  );
  
  // Enable keyboard nav.
  $form['navigation']['keyboard_nav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable keyboard navigation.'),
    '#default_value' => (isset($config) ? $settings['keyboard_nav'] : TRUE),
  );
  
  // Manual advance.
  $form['navigation']['manual_advance'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force manual transitions.'),
    '#default_value' => (isset($config) ? $settings['manual_advance'] : FALSE),
  );
  
  // Callbacks fieldset.
  $form['callbacks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Callbacks'),
    '#group' => 'vtabs',
  );
  
  // beforeChange callback.
  $form['callbacks']['before_change'] = array(
    '#type' => 'textarea',
    '#title' => t('beforeChange Callback'),
    '#description' => t('Triggers before a slide transition. Only include the callback body. Do not include the callback function definition (function() {..}), or @script tags.', array('@script' => '<script>')),
    '#default_value' => (isset($config) ? $settings['before_change'] : ''),
  );
  
  // afterChange callback.
  $form['callbacks']['after_change'] = array(
    '#type' => 'textarea',
    '#title' => t('afterChange Callback'),
    '#description' => t('Triggers after a slide transition. Only include the callback body. Do not include the callback function definition (function() {..}), or @script tags.', array('@script' => '<script>')),
    '#default_value' => (isset($config) ? $settings['after_change'] : ''),
  );
  
  // slideshowEnd callback.
  $form['callbacks']['slideshow_end'] = array(
    '#type' => 'textarea',
    '#title' => t('slideshowEnd Callback'),
    '#description' => t('Triggers after all slides have been shown. Only include the callback body. Do not include the callback function definition (function() {..}), or @script tags.', array('@script' => '<script>')),
    '#default_value' => (isset($config) ? $settings['slideshow_end'] : ''),
  );
  
  // lastSlide callback.
  $form['callbacks']['last_slide'] = array(
    '#type' => 'textarea',
    '#title' => t('lastSlide Callback'),
    '#description' => t('Triggers when last slide is shown. Only include the callback body. Do not include the callback function definition (function() {..}), or @script tags.', array('@script' => '<script>')),
    '#default_value' => (isset($config) ? $settings['last_slide'] : ''),
  );
  
  // afterLoad callback.
  $form['callbacks']['after_load'] = array(
    '#type' => 'textarea',
    '#title' => t('afterLoad Callback'),
    '#description' => t('Triggers when slider has loaded. Only include the callback body. Do not include the callback function definition (function() {..}), or @script tags.', array('@script' => '<script>')),
    '#default_value' => (isset($config) ? $settings['after_load'] : ''),
  );
  
  $form['cid'] = array(
    '#type' => 'hidden',
    '#value' => $cid,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  return $form;
}

/**
 * Create/Edit Media Nivo Slider configuration validation handler.
 */
function media_nivo_slider_config_validate($form, &$form_state) {
  // Build list of the callback fields that need to be validated.
  $callbacks = array(
    'before_change',
    'after_change',
    'slideshow_end',
    'last_slide',
    'after_load',
  );
  
  // Check the callback functions for script tags or function definitions.
  foreach ($callbacks as $callback) {
    // Check for <script> tags.
    if (preg_match('~<\s*\/?\s*script\s*.*?>~i', $form_state['values'][$callback])) {
      form_set_error($callback, t('Do not include @script tags in the callbacks.', array('@script' => '<script>')));
    }
  }
  
}

/**
 * Create/Edit Media Nivo Slider configuration submit handler.
 */
function media_nivo_slider_config_submit($form, &$form_state) {
  // Fields to ignore in the form_state values array when extracting the configuration settings.
  $ignore_fields = array(
    'vtabs__active_tab' => 'vtabs__active_tab',
    'submit' => 'submit',
    'form_build_id' => 'form_build_id',
    'form_token' => 'form_token',
    'form_id' => 'form_id',
    'op' => 'op',
    'name' => 'name',
    'cid' => 'cid',
  );
  
  // Create the settings array.
  $settings = array_diff_key($form_state['values'], $ignore_fields);
  
  // Build the DB record to be saved/updated.
  $record = array(
    'name' => $form_state['values']['name'],
    'settings' => $settings,
  );
  
  // Save or update the DB record.
  if (isset($form_state['values']['cid'])) {
    $record['cid'] = $form_state['values']['cid'];
    drupal_write_record('media_nivo_slider', $record, 'cid');
  }
  else {
    drupal_write_record('media_nivo_slider', $record);
  }
  
  // Send user back to the overview page.
  $form_state['redirect'] = 'admin/config/media/media-nivo-slider';
}

/**
 * Delete confirmation form for Nivo Slider configurations.
 */
function media_nivo_slider_config_delete($form, &$form_state, $cid) {
  // Retrieve the gallery so we can display the name.
  $config = db_select('media_nivo_slider', 'ns')
    ->fields('ns')
    ->condition('cid', $cid)
    ->execute()
    ->fetchObject();

  // Display a delete confirmation page.
  return confirm_form(
    array(
      'cid' => array(
        '#type' => 'value', 
        '#value' => $cid,
      ),
    ), 
    t('Are you sure you want to delete the %config Nivo Slider configuration?', array('%config' => $config->name)), 
    'admin/config/media/media-nivo-slider', 
    t('This action cannot be undone.'), 
    t('Delete configuraiton'), 
    t('Cancel')
  );
}

/**
 * Delete form validation handler.
 */
function media_nivo_slider_config_delete_validate($form, &$form_state) {
  // Check to make sure no sliders are still using the configuration being deleted.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'media_gallery');
  $query->fieldCondition('media_nivo_slider_config', 'value', $form_state['values']['cid']);
  $result = $query->execute();

  if (!empty($result['node'])) {
    form_set_error('cid', t('The configuration can not be deleted. There are still sliders using this configuration.'));
  }
}

/**
 * Delete form submission handler.
 */
function media_nivo_slider_config_delete_submit($form, &$form_state) {
  // Delete the gallery.
  db_delete('media_nivo_slider')
  ->condition('cid', $form_state['values']['cid'])
  ->execute();
  
  // Display confirmation message.
  drupal_set_message(t('Configuration deleted successfully.'));

  // Send user back to the overview page.
  $form_state['redirect'] = 'admin/config/media/media-nivo-slider';
}
