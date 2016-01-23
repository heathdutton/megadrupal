<?php

/**
 * @file
 * Contains the lightgallery style plugin.
 */

/**
 * Style plugin to render views as lightgallery instance.
 *
 * @ingroup views_style_plugins
 */
class lightgallery_style_plugin extends views_plugin_style {

  /**
   * Provide default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['lightgallery'] = array('default' => array());

    return $options;
  }

  /**
   * Adds Lightgallery configuration form options.
   */
  function options_form(&$form, &$form_state) {
    // Check if fields have been added.  Table style plugin will set
    // error_markup if fields are not added.
    // @see views_plugin_style::options_form()
    if (isset($form['error_markup'])) {
      return;
    }

    $fields = $this->conf_get_field_sources();
    $missing_field_warning = '';
    if (empty($fields['field_options_images'])) {
      $missing_field_warning = t('<strong>You must add a field of type image, file or file ID to your view display before this value can be set.</strong><br/>');
    }

    // CORE OPTIONS.
    $form['lightgallery_core'] = array(
      '#type' => 'fieldset',
      '#title' => t('Lightgallery core settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['lightgallery_core']['thumb_field'] = array(
      '#title' => t('Thumbnail field'),
      '#type' => 'select',
      '#options' => $fields['field_options_images'],
      '#default_value' => isset($this->options['lightgallery']['thumb_field']) ? $this->options['lightgallery']['thumb_field'] : NULL,
      '#description' => t('Select the field you want to use to display the thumbnails on page load.'),
      '#suffix' => $missing_field_warning,
      '#required' => TRUE,
    );

    $form['lightgallery_core']['image_field'] = array(
      '#title' => t('Image field'),
      '#type' => 'select',
      '#options' => $fields['field_options_images'],
      '#default_value' => isset($this->options['lightgallery']['image_field']) ? $this->options['lightgallery']['image_field'] : NULL,
      '#description' => t('Select the field you want to use to display the images in the Lightgallery.'),
      '#suffix' => $missing_field_warning,
      '#required' => TRUE,
    );

    $form['lightgallery_core']['title'] = array(
      '#title' => t('Title field'),
      '#type' => 'select',
      '#options' => array('' => t('None')) + $fields['field_options'],
      '#default_value' => isset($this->options['lightgallery']['title']) ? $this->options['lightgallery']['title'] : NULL,
      '#description' => t('Select the field you want to use as title in the Lightgallery. Leave empty to omit titles.'),
      '#required' => FALSE,
    );

    $form['lightgallery_core']['mode'] = array(
      '#type' => 'select',
      '#title' => t('Transition'),
      '#options' => _lightgallery_get_modes(),
      '#default_value' => isset($this->options['lightgallery']['mode']) ? $this->options['lightgallery']['mode'] : NULL,
      '#description' => t('Type of transition between images.'),
    );

    $form['lightgallery_core']['preload'] = array(
      '#type' => 'select',
      '#title' => t('Preload'),
      '#options' => drupal_map_assoc(array(1, 2, 3, 4)),
      '#default_value' => isset($this->options['lightgallery']['preload']) ? $this->options['lightgallery']['preload'] : 1,
      '#description' => t('number of preload slides. will exicute only after the current slide is fully loaded. '
          . 'ex:// you clicked on 4th image and if preload = 1 then 3rd slide and 5th slide will be loaded in the background after the 4th slide is fully loaded. '
          . 'If preload is 2 then 2nd 3rd 5th 6th slides will be preloaded'),
    );

    $form['lightgallery_core']['closable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Closable'),
      '#default_value' => isset($this->options['lightgallery']['closable']) ? $this->options['lightgallery']['closable'] : TRUE,
      '#description' => t('Allows clicks on dimmer to close gallery.')
    );

    $form['lightgallery_core']['loop'] = array(
      '#type' => 'checkbox',
      '#title' => t('Loop'),
      '#default_value' => isset($this->options['lightgallery']['loop']) ? $this->options['lightgallery']['loop'] : TRUE,
      '#description' => t('If not selected, the ability to loop back to the beginning of the gallery when on the last element, will be disabled.')
    );

    $form['lightgallery_core']['esc_key'] = array(
      '#type' => 'checkbox',
      '#title' => t('Escape key'),
      '#default_value' => isset($this->options['lightgallery']['esc_key']) ? $this->options['lightgallery']['esc_key'] : TRUE,
      '#description' => t('Whether the LightGallery could be closed by pressing the "Esc" key.')
    );

    $form['lightgallery_core']['key_press'] = array(
      '#type' => 'checkbox',
      '#title' => t('Keyboard'),
      '#default_value' => isset($this->options['lightgallery']['key_press']) ? $this->options['lightgallery']['key_press'] : TRUE,
      '#description' => t('Enable keyboard navigation.')
    );

    $form['lightgallery_core']['controls'] = array(
      '#type' => 'checkbox',
      '#title' => t('Controls'),
      '#default_value' => isset($this->options['lightgallery']['controls']) ? $this->options['lightgallery']['controls'] : TRUE,
      '#description' => t('If not checked, prev/next buttons will not be displayed.')
    );

    $form['lightgallery_core']['mouse_wheel'] = array(
      '#type' => 'checkbox',
      '#title' => t('Mouse wheel'),
      '#default_value' => isset($this->options['lightgallery']['mouse_wheel']) ? $this->options['lightgallery']['mouse_wheel'] : TRUE,
      '#description' => t('Chane slide on mousewheel')
    );

    $form['lightgallery_core']['download'] = array(
      '#type' => 'checkbox',
      '#title' => t('Download'),
      '#default_value' => isset($this->options['lightgallery']['download']) ? $this->options['lightgallery']['download'] : TRUE,
      '#description' => t('Enable download button. '
          . 'By default download url will be taken from data-src/href attribute but it supports only for modern browsers.'),
    );

    $form['lightgallery_core']['counter'] = array(
      '#type' => 'checkbox',
      '#title' => t('Counter'),
      '#default_value' => isset($this->options['lightgallery']['counter']) ? $this->options['lightgallery']['counter'] : TRUE,
      '#description' => t('Whether to show total number of images and index number of currently displayed image.'),
    );

    $form['lightgallery_core']['drag'] = array(
      '#type' => 'checkbox',
      '#title' => t('Drag'),
      '#default_value' => isset($this->options['lightgallery']['drag']) ? $this->options['lightgallery']['drag'] : TRUE,
      '#description' => t('Enables desktop mouse drag support.'),
    );

    $form['lightgallery_core']['touch'] = array(
      '#type' => 'checkbox',
      '#title' => t('Touch'),
      '#default_value' => isset($this->options['lightgallery']['touch']) ? $this->options['lightgallery']['touch'] : TRUE,
      '#description' => t('Enables touch support.'),
    );

    // THUMB OPTIONS.
    $form['lightgallery_thumbs'] = array(
      '#type' => 'fieldset',
      '#title' => t('Lightgallery thumbnail settings'),
      '#collapsible' => TRUE,
      '#collapsed' => isset($this->options['lightgallery']['thumbnails']) ? !$this->options['lightgallery']['thumbnails'] : FALSE,
    );

    $form['lightgallery_thumbs']['thumbnails'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use thumbnails'),
      '#default_value' => isset($this->options['lightgallery']['thumbnails']) ? $this->options['lightgallery']['thumbnails'] : TRUE,
      '#description' => t('Indicate if you want to use thumbnails in the LightGallery.')
    );

    $form['lightgallery_thumbs']['animate_thumb'] = array(
      '#type' => 'checkbox',
      '#title' => t('Animate thumbnails'),
      '#default_value' => isset($this->options['lightgallery']['animate_thumb']) ? $this->options['lightgallery']['animate_thumb'] : TRUE,
      '#description' => t('Enable thumbnail animation.')
    );

    $form['lightgallery_thumbs']['current_pager_position'] = array(
      '#type' => 'select',
      '#title' => t('Position'),
      '#options' => array('left' => t('Left'), 'middle' => t('Middle'), 'right' => t('Right')),
      '#default_value' => isset($this->options['lightgallery']['current_pager_position']) ? $this->options['lightgallery']['current_pager_position'] : 'middle',
      '#description' => t('Position of selected thumbnail.')
    );

    $form['lightgallery_thumbs']['thumb_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => isset($this->options['lightgallery']['thumb_width']) ? $this->options['lightgallery']['thumb_width'] : 100,
      '#description' => t('Width of each thumbnails.')
    );

    $form['lightgallery_thumbs']['thumb_cont_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#default_value' => isset($this->options['lightgallery']['thumb_cont_height']) ? $this->options['lightgallery']['thumb_cont_height'] : 100,
      '#description' => t('Height of the thumbnail container including padding and border.')
    );

    // AUTOPLAY OPTIONS.
    $form['lightgallery_autoplay'] = array(
      '#type' => 'fieldset',
      '#title' => t('Lightgallery autoplay settings'),
      '#collapsible' => TRUE,
      '#collapsed' => isset($this->options['lightgallery']['autoplay']) ? !$this->options['lightgallery']['autoplay'] : FALSE,
    );

    $form['lightgallery_autoplay']['autoplay'] = array(
      '#type' => 'checkbox',
      '#title' => t('Autoplay'),
      '#default_value' => isset($this->options['lightgallery']['autoplay']) ? $this->options['lightgallery']['autoplay'] : TRUE,
      '#description' => t('Enable/Disable gallery autoplay.'),
    );

    $form['lightgallery_autoplay']['pause'] = array(
      '#type' => 'textfield',
      '#title' => t('Pause'),
      '#default_value' => isset($this->options['lightgallery']['pause']) ? $this->options['lightgallery']['pause'] : 5000,
      '#description' => t('The time (in ms) between each auto transition.'),
    );

    $form['lightgallery_autoplay']['progress_bar'] = array(
      '#type' => 'checkbox',
      '#title' => t('Progress bar'),
      '#default_value' => isset($this->options['lightgallery']['progress_bar']) ? $this->options['lightgallery']['progress_bar'] : TRUE,
      '#description' => t('Enable/Disable autoplay progress bar.'),
    );

    $form['lightgallery_autoplay']['autoplay_controls'] = array(
      '#type' => 'checkbox',
      '#title' => t('Autoplay controls'),
      '#default_value' => isset($this->options['lightgallery']['autoplay_controls']) ? $this->options['lightgallery']['autoplay_controls'] : TRUE,
      '#description' => t('Show/hide autoplay controls.'),
    );

    // FULL SCREEN OPTIONS.
    $form['lightgallery_full_screen'] = array(
      '#type' => 'fieldset',
      '#title' => t('Lightgallery fullscreen settings'),
      '#collapsible' => TRUE,
      '#collapsed' => isset($this->options['lightgallery']['full_screen']) ? !$this->options['lightgallery']['full_screen'] : FALSE,
    );

    $form['lightgallery_full_screen']['full_screen'] = array(
      '#type' => 'checkbox',
      '#title' => t('Full screen'),
      '#default_value' => isset($this->options['lightgallery']['full_screen']) ? $this->options['lightgallery']['full_screen'] : TRUE,
      '#description' => t('Enable/Disable fullscreen mode.'),
    );

    // PAGER OPTIONS.
    $form['lightgallery_pager'] = array(
      '#type' => 'fieldset',
      '#title' => t('Lightgallery pager settings'),
      '#collapsible' => TRUE,
      '#collapsed' => isset($this->options['lightgallery']['pager']) ? !$this->options['lightgallery']['pager'] : FALSE,
    );

    $form['lightgallery_pager']['pager'] = array(
      '#type' => 'checkbox',
      '#title' => t('Pager'),
      '#default_value' => isset($this->options['lightgallery']['pager']) ? $this->options['lightgallery']['pager'] : TRUE,
      '#description' => t('Enable/Disable pager.'),
    );

    // ZOOM OPTIONS.
    $form['lightgallery_zoom'] = array(
      '#type' => 'fieldset',
      '#title' => t('Lightgallery zoom settings'),
      '#collapsible' => TRUE,
      '#collapsed' => isset($this->options['lightgallery']['zoom']) ? !$this->options['lightgallery']['zoom'] : FALSE,
    );

    $form['lightgallery_zoom']['zoom'] = array(
      '#type' => 'checkbox',
      '#title' => t('Zoom'),
      '#default_value' => isset($this->options['lightgallery']['zoom']) ? $this->options['lightgallery']['zoom'] : TRUE,
      '#description' => t('Enable/Disable zoom option.'),
    );

    $form['lightgallery_zoom']['scale'] = array(
      '#type' => 'select',
      '#title' => t('Scale'),
      '#options' => drupal_map_assoc(array(1, 2, 3, 4)),
      '#default_value' => isset($this->options['lightgallery']['scale']) ? $this->options['lightgallery']['scale'] : 1,
      '#description' => t('Value of zoom that should be incremented/decremented.'),
    );

    // HASH OPTIONS.
    $form['lightgallery_hash'] = array(
      '#type' => 'fieldset',
      '#title' => t('Lightgallery hash settings'),
      '#collapsible' => TRUE,
      '#collapsed' => isset($this->options['lightgallery']['hash']) ? !$this->options['lightgallery']['hash'] : FALSE,
    );

    $form['lightgallery_hash']['hash'] = array(
      '#type' => 'checkbox',
      '#title' => t('Update hash in url'),
      '#default_value' => isset($this->options['lightgallery']['hash']) ? $this->options['lightgallery']['hash'] : TRUE,
      '#description' => t('Enable/Disable hash option.'),
    );

    $form['lightgallery_hash']['gallery_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Gallery ID'),
      '#default_value' => isset($this->options['lightgallery']['gallery_id']) ? $this->options['lightgallery']['gallery_id'] : 1,
      '#description' => t('Unique id for each gallery. It is mandatory when you use hash plugin for multiple galleries on the same page.'),
    );
  }

  /**
   * Add form validation for options.
   */
  function options_validate(&$form, &$form_state) {
    $style_options = $form_state['values']['style_options'];
    // Flatten style options array.
    $form_state['values']['style_options']['lightgallery'] = $this->flatten_array($style_options);

    parent::options_validate($form, $form_state);
  }

  function validate() {
    parent::validate();
    $errors = array();
    // Make sure the pager is not enabled.
    if ($this->display->handler->use_pager()) {
      $errors[] = t('The Lighgallery style cannot be used with a pager. Please disable the "Use pager" option for this display.');
    }

    return $errors;
  }

  /**
   * Renders the view fields.
   * @param type $result
   * @return type
   */
  function render_fields($result) {
    $this->rendered_fields = array();
    $this->view->row_index = 0;
    $keys = array_keys($this->view->field);

    // If all fields have a field::access FALSE there might be no fields, so
    // there is no reason to execute this code.
    if (!empty($keys)) {
      $fields = $this->conf_get_field_sources();
      $image_fields = array_keys($fields['field_options_images']);
      foreach ($result as $count => $row) {
        $this->view->row_index = $count;
        foreach ($keys as $id) {
          if (in_array($id, $image_fields)) {
            // This is an image/thumb field.
            // Create URI for selected image style.
            $image_style = $this->view->field[$id]->options['settings']['image_style'];
            $uri = !empty($result[$count]->{'field_' . $id}[0]['raw']['uri']) ? $result[$count]->{'field_' . $id}[0]['raw']['uri'] : FALSE;
            if ($uri) {
              if (!empty($image_style)) {
                $this->rendered_fields[$count][$id] = file_create_url(image_style_url($image_style,
                        $uri));
              }
              else {
                $this->rendered_fields[$count][$id] = file_create_url($uri);
              }
            }
          }
          else {
            // Just render the field as views would do.
            $this->rendered_fields[$count][$id] = $this->view->field[$id]->theme($row);
          }
        }

        $this->row_tokens[$count] = $this->view->field[$id]->get_render_tokens(array());
      }
    }
    unset($this->view->row_index);


    return $this->rendered_fields;
  }

  /**
   * Fetches field options for all fields definied in view.
   * @return type
   */
  private function conf_get_field_sources() {
    $view = $this->view;
    $options = array(
      'field_options_images' => array(),
      'field_options_images_type' => array(),
      'field_options' => array(),
    );
    $field_handlers = $this->display->handler->get_handlers('field');
    if (!empty($view->base_table) && $view->base_table == 'file_managed') {
      $options['field_options_images_type']['file_base'] = 'file_base';
    }
    $field_labels = $this->display->handler->get_field_labels();
    // Get the label for each field.
    foreach ($field_handlers as $field => $handler) {
      $name = $field_labels[$field];
      // Separate image fields from non-image fields. For image fields we can
      // work with fids and fields of type image or file.
      if (isset($handler->field) && $handler->field == 'fid') {
        $options['field_options_images'][$field] = $name;
        $options['field_options_images_type'][$field] = 'file_id_field';
      }
      elseif (isset($handler->field_info['type']) && ($handler->field_info['type'] == 'image' || $handler->field_info['type'] == 'file')) {
        $options['field_options_images'][$field] = $name;
        $options['field_options_images_type'][$field] = 'file_field';
      }
      else {
        $options['field_options'][$field] = $name;
      }
    }
    return $options;
  }

  /**
   * Flatten array and preserve keys.
   * @param array $array
   * @return array
   */
  private function flatten_array(array $array) {
    $flattened_array = array();
    array_walk_recursive($array,
        function($a, $key) use (&$flattened_array) {
      $flattened_array[$key] = $a;
    });
    return $flattened_array;
  }

}
