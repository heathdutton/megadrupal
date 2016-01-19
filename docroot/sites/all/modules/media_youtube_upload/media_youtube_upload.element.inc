<?php

/**
 * @file
 * Element callbacks for the media_youtube_upload module.
 */

/**
 * Implements hook_element_info().
 */
function media_youtube_upload_element_info() {
  $file_path = drupal_get_path('module', 'media_youtube_upload');
  $cors_path = _media_youtube_upload_get_cors_upload();

  $types['media_youtube_upload_upload'] = array(
    '#input' => TRUE,
    '#tree' => TRUE,
    '#process' => array('media_youtube_upload_upload_process'),
    '#element_validate' => array('media_youtube_upload_upload_validate'),
    '#theme_wrappers' => array('form_element'),
    '#upload_validators' => array(
      'allowed_extensions' => MEDIA_YOUTUBE_UPLOADER_ALLOWED_EXTENSIONS,
      'max_uploadsize' => MEDIA_YOUTUBE_UPLOADER_MAX_UPLOADSIZE,
    ),
    '#youtube_fields' => array(
      'default_fid' => 0,
      'enabled_fields' => '',
    ),
    '#attached' => array(
      'library' => array(array('system', 'ui.progressbar')),
      'js' => array(
        $file_path . '/js/media_youtube_upload.js',
        // We also add the file.module's JS which does things like client side
        // validation for us!
        drupal_get_path('module', 'file') . '/file.js',
      ),
      'css' => array(
        $file_path . '/css/media_youtube_upload.css',
      ),
    ),
  );

  if ($cors_path) {
    $types['media_youtube_upload_upload']['#attached']['js'][] = $cors_path;
  }
  return $types;
}

/**
 * Element process function for media_youtube_upload_upload element.
 *
 * Expands the element to include Upload and Remove buttons, as well as support
 * for a default value.
 *
 * In order to take advantage of the work that file.module is already doing for
 * elements of type #managed_file we stick to the same naming convention here.
 */
function media_youtube_upload_upload_process($element, &$form_state, &$form) {

  if ($form_state['build_info']['form_id'] !== 'field_ui_field_edit_form') {

    if (isset($element['#field_name']) && isset($element['#language']) && isset($element['#delta'])) {
      $field = field_widget_field($element, $form_state);
      $instance = field_widget_instance($element, $form_state);
      $settings = $instance['widget']['settings'];
      $defaults = $instance['default_value'][0];
    }
    else {
      $element_info = element_info('media_youtube_upload_upload');
    }
    $default_element_fid = isset($element['#youtube_fields']['default_fid']) ? $element['#youtube_fields']['default_fid'] : 0;
    $default_fid = isset($defaults['fid']) ? $defaults['fid'] : $default_element_fid;

    $input = drupal_array_get_nested_value($form_state['input'], $element['#parents']);
    $values = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
    $id = $element['#id'];
    $element['#id'] = $id . '-upload';

    // Construct some attribute helpers for javascript.
    $parents = $element['#parents'];
    $name = count($parents) === 1 ? $parents[0] : array_shift($parents) . '[' . implode('][', $parents) . ']';
    $change_button = isset($input['filename']) && isset($values['youtube_fields']) ? TRUE : FALSE;

    // Add the display field if enabled.
    if (!empty($field['settings']['display_field']) && $element['#value']['fid']) {
      $element['display'] = array(
        '#type' => empty($element['#value']['fid']) ? 'hidden' : 'checkbox',
        '#title' => t('Include file in display'),
        '#value' => isset($element['#value']['display']) ? $element['#value']['display'] : $field['settings']['display_default'],
        '#attributes' => array('class' => array('file-display')),
      );
    }
    else {
      $element['display'] = array(
        '#type' => 'hidden',
        '#value' => '1',
      );
    }

    $fid = isset($element['#value']['fid']) && $element['#value']['fid'] !== $default_fid ? $element['#value']['fid'] : 0;
    $element['fid'] = array(
      '#type' => 'hidden',
      '#default_value' => $fid,
      '#name' => $name . '[fid]',
      '#tree' => FALSE,
    );

    $element['youtube_id'] = array(
      '#type' => 'hidden',
      '#default_value' => isset($element['#value']['youtube_id']) ? $element['#value']['youtube_id'] : '',
      '#name' => $name . '[youtube_id]',
      '#tree' => FALSE,
    );
    $element['filename'] = array(
      '#type' => 'hidden',
      '#default_value' => isset($element['#value']['filename']) ? $element['#value']['filename'] : '',
      '#name' => $name . '[filename]',
      '#tree' => FALSE,
    );
    $element['filesize'] = array(
      '#type' => 'hidden',
      '#default_value' => isset($element['#value']['filesize']) ? $element['#value']['filesize'] : '',
      '#name' => $name . '[filesize]',
      '#tree' => FALSE,
    );
    // If we have an fid value then display the file link..
    $link = !empty($element['#value']['fid']) && $element['#value']['fid'] !== $default_fid ? TRUE : FALSE;
    if ($link) {
      $element['#file'] = file_load($element['#value']['fid']);

      $element['file_link'] = array(
        '#type' => 'markup',
        '#markup' => theme('media_youtube_upload_file_link', array('file' => $element['#file'])) . ' ',
        '#weight' => -10,
      );
    }
    // Otherwise load the default file and remove the fid.
    elseif (!empty($default_fid) && file_load($default_fid) !== FALSE) {
      $file = file_load($default_fid);
      unset($file->fid);
      $element['#file'] = $file;
    }

    $element['upload'] = array(
      // Dunno why but when using single element for a form the ID gets f'ed up.
      '#id' => $element['#id'],
      '#name' => $name . '[upload]',
      '#title' => t('Choose a file'),
      '#title_display' => 'invisible',
      '#size' => 60,
      '#weight' => -3,
      '#theme' => 'media_youtube_upload_managed_file',
      '#attributes' => array('class' => array('youtube-cors-upload-file')),
      '#access' => !$link,
    );

    $element['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array(
        'class' => array(
          $id . '-actions',
          'media-youtube-uploader-actions',
        ),
      ),
      '#weight' => -2,
    );

    // AJAX settings for executing the field retrieval.
    $ajax_settings_upload = array(
      'callback' => 'media_youtube_upload_ajax_get_fields',
      'wrapper' => 'edit-' . $id . '-youtube-fields',
      'method' => 'replace',
      'effect' => 'fade',
    );

    // The "Retrieve fields" button.
    $element['actions']['upload_button'] = array(
      '#name' => $name . '[upload_button]',
      // Element type has to be a submit button and the submit callback has to
      // be set. Otherwise ajax callbacks will be called twice because of a form
      // rebuild.
      '#type' => 'submit',
      '#submit' => array(),
      '#value' => t('Upload to youtube'),
      '#weight' => -5,
      '#access' => !$link,
      '#limit_validation_errors' => array($element['#parents']),
      '#ajax' => $ajax_settings_upload,

    );

    // AJAX settings used for save and remove buttons.
    $ajax_settings_save_and_remove = array(
      'callback' => 'media_youtube_upload_upload_js',
      'wrapper' => $id . '-ajax-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    );

    // The "Save" button.
    $element['actions']['save_button'] = array(
      '#name' => $name . '[save_button]',
      '#type' => 'submit',
      '#value' => t('Save'),
      '#validate' => array(),
      '#limit_validation_errors' => array($element['#parents']),
      '#attributes' => array('style' => 'display: none;'),
      '#weight' => -5,
      '#submit' => array('media_youtube_upload_upload_remove_submit'),
      '#ajax' => $ajax_settings_save_and_remove,
      // '#access' => empty($element['fid']['#default_value']) ? FALSE : TRUE,
    );

    // The "Remove" button.
    $element['actions']['remove_button'] = array(
      '#name' => $name . '[remove_button]',
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#validate' => array(),
      '#limit_validation_errors' => array($element['#parents']),
      '#weight' => -5,
      '#submit' => array('media_youtube_upload_upload_remove_submit'),
      '#ajax' => $ajax_settings_save_and_remove,
      '#attributes' => $link ? array() : array('style' => 'display: none;'),
      // Think access needs to be moved to pre render.
      // '#access' => $link,
    );

    // Add a class to the <form> element so we can find it with JS later.
    $form['#attributes'] = array('class' => array('youtube-cors-upload-form'));

    $element['#description'] = theme('media_youtube_upload_file_upload_help', array('description' => isset($element['#description']) ? $element['#description'] : '', 'settings' => isset($instance['settings']) ? $instance['settings']['upload_validators'] : $element['#upload_validators']));

    $element['#prefix'] = '<div id="' . $id . '-ajax-wrapper">';
    $element['#suffix'] = '</div>';

    if (!$link) {
      // Set the container first.
      $element['youtube_fields'] = array(
        '#id' => 'edit-' . $id . '-youtube-fields',
        '#type' => 'container',
        '#weight' => -1,
      );
      // If a default file is present load our fields.
      if (!!file_load($default_fid)) {
        $element['youtube_fields'] = array(
          '#id' => 'edit-' . $id . '-youtube-fields',
          '#type' => 'container',
          '#weight' => -1,
          '#parents' => $element['#parents'],
        );
        $file = $element['#file'];
        $youtube_fields = field_info_instances('file', 'youtube');
        $youtube_fields_enabled = isset($settings['youtube_fields']) ? $settings['youtube_fields']['enabled_fields'] : $element['#youtube_fields']['enabled_fields'];
        foreach ($youtube_fields as $youtube_field) {
          if (!empty($youtube_fields_enabled[$youtube_field['field_name']])) {
            $field_form = _media_youtube_upload_get_field_form($form, $form_state, $youtube_field, $file);
            $element['youtube_fields'] += (array) $field_form;
          }
        }
      }
      elseif ((empty($default_fid) || !file_load($default_fid)) && !empty($element['#settings_type'])) {
        if (user_access('administer media youtube upload')) {
          $element['notice'] = array(
            '#markup' => '<p>' . t('You need to have the default settings applied before you can use the YouTube upload. Please enter your prefered default values at the <a href="@settings">settings page</a>.', array('@settings' => url('admin/config/media/media_youtube_upload/' . $element['#settings_type']))) . '</p>',
            '#weight' => -5,
          );
        }
        else {
          $element['notice'] = array(
            '#markup' => '<p>' . t('This widget is not properly configured. Please contact your administrator if you wish to use it.') . '</p>',
            '#weight' => -5,
          );
        }
        $element['#disabled'] = TRUE;
      }
      elseif (!user_access('upload media youtube upload files')) {
        $element['notice'] = array(
          '#markup' => '<p>' . t('You do not have sufficient privileges to upload videos to YouTube. This widget is therefore disabled.') . '</p>',
          '#weight' => -5,
        );
        $element['#disabled'] = TRUE;
      }
    }
    // Still BUGGY, DOES NOT WORK!
    if (isset($element['#settings_type']) && $element['#settings_type'] === 'media_settings') {
      $element['next'] = array(
        '#type' => 'submit',
        '#value' => t('Next'),
        '#submit' => array('media_browser_form_submit'),
      );
    }
    // Add the javascript settings to our page.
    $upload_id = '#' . $element['#id'];
    drupal_add_js(
      array(
        'media_youtube_upload' => array(
          $upload_id => array(
            'youtube_field_helper' => array(
              'name' => $name,
              'id' => $id,
            ),
            'upload_validators' => array(
              'max_uploadsize' => isset($instance['settings']) ? $instance['settings']['upload_validators']['max_uploadsize'] : $element['#upload_validators']['max_uploadsize'],
              'allowed_extensions' => isset($instance['settings']) ? implode(',', array_filter(explode(' ', $instance['settings']['upload_validators']['allowed_extensions']))) : implode(',', array_filter(explode(' ', $element['#upload_validators']['allowed_extensions']))),
            ),
          ),
        ),
      ),
      'setting'
    );
  }
  return $element;
}

/**
 * Validation callback for media_youtube_upload element type.
 *
 * @param array $element
 *   The element for which this validation function runs.
 * @param array $form_state
 *   The form state array that contains the elements values.
 *
 * @throws Exception
 *   If the database call to change file size, fails.
 */
function media_youtube_upload_upload_validate(&$element, &$form_state) {

  if ($form_state['build_info']['form_id'] === 'field_ui_field_edit_form' && $form_state['build_info']['args'][0]['widget']['type'] === 'media_youtube_upload') {
    if (isset($element['#default_file']->fid)) {
      $file = $element['#default_file'];
    }
    else {
      $file = file_save($element['#default_file']);
      form_set_value($element['fid'], $file->fid, $form_state);
    }
    field_attach_form_validate('file', $file, $element, $form_state);
    field_attach_submit('file', $file, $element, $form_state);
    field_attach_presave('file', $file, $element, $form_state);
    file_entity_file_update($file);
  }
  else {
    // Set needed variables.
    $triggering_element_parents = isset($form_state['triggering_element']['#parents']) ? $form_state['triggering_element']['#parents'] : array();
    $button_key = array_pop($triggering_element_parents);
    array_pop($triggering_element_parents);
    $parents = $triggering_element_parents;
    $delta = count($triggering_element_parents) > 0 ? array_pop($triggering_element_parents) : 0;

    // Only update or save if we are not removing the file from our field.
    if ($button_key !== 'remove_button' && isset($element['youtube_fields'])) {

      // Update the file element so we have the correct values.
      $file = $element['#file'];
      field_attach_form_validate('file', $file, $element['youtube_fields'], $form_state);
      field_attach_submit('file', $file, $element['youtube_fields'], $form_state);
      field_attach_presave('file', $file, $element['youtube_fields'], $form_state);
      $element['#file'] = $file;

      if (isset($element['#delta']) && $delta === $element['#delta']) {
        $delta_check = TRUE;
      }
      elseif (count($triggering_element_parents) === 0) {
        $delta_check = TRUE;
      }
      else {
        $delta_check = FALSE;
      }

      // If we have a file object, there are no errors, element has the right
      // delta and we pressed the save button: then we can step in to the saving
      // of our YouTube upload file.
      if (is_object($element['#file']) && !form_get_errors() && $delta_check && $button_key === 'save_button') {
   
        // Unset any translations from the default file if necessary.
        if (module_exists('entity_translation')) {
          module_load_include('inc', 'entity_translation', 'translation.handler');
          $handler = entity_translation_get_handler('file', $file);
          $language = $handler->getDefaultLanguage();
          $file->translations->original = $language;
          foreach (entity_translation_languages('file') as $langcode) {
            if ($language != $langcode->language) {
              $handler->removeTranslation($langcode->language);
            }
          }
        }

        $file->bundle = 'youtube';
        $file->uri = 'youtube://v/' . REQUEST_TIME;
        $file->filemime = 'youtube/video';
        $file->filename = !empty($element['filename']['#value']) ? $element['filename']['#value'] : $element['filename']['#default_value'];
        $file->uri = !empty($element['youtube_id']['#value']) ? 'youtube://v/' . $element['youtube_id']['#value'] : $file->uri;
        if ($file = file_save($file)) {
          // Carry over the new file.
          $element['#file'] = $file;
          $form_state['file'] = $file;
          // Set values for the form rebuild.
          form_set_value($element['fid'], $file->fid, $form_state);
          $element['fid']['#value'] = $file->fid;
          // Add file to queue to get thumbnail on cron run.
          $queue = DrupalQueue::get('media_youtube_upload_set_thumbnail');
          $queue->createItem($file);
          // Alter the filesize to represent the file uploaded to youtube.
          $transaction = db_transaction();
          try {
            $num_updated = db_update('file_managed') // Table name no longer needs {}
              ->fields(array(
                'filesize' => $element['filesize']['#value'],
                'uri' => $file->uri,
              ))
              ->condition('fid', $file->fid, '=')
              ->execute();
          }
          catch (Exception $e) {
            $transaction->rollback();
            // Log the exception to watchdog.
            watchdog_exception('type', $e);
            throw $e;
          }
        }
      }
    }
  }
}

/**
 * Ajax callback to return the youtube fields necessary for uploading the file.
 *
 * @param array $form
 *   The full form structure upon which our ajax callback can work.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 *
 * @return array
 *   Ajax commands to process when called.
 */
function media_youtube_upload_ajax_get_fields($form, &$form_state) {
  // Find the element that triggered the AJAX callback and return fields for it,
  // or remove them if necessary.
  $parents = $form_state['triggering_element']['#array_parents'];
  $button_key = array_pop($parents);
  // Also lose the actions wrapper to get to the element itself.
  array_pop($parents);
  $element = drupal_array_get_nested_value($form, $parents);

  $commands = array();
  // Get a token from Google to initialize our upload.
  $ytapi = new MediaYoutubeUploadYtapi();
  $tok = $ytapi->getFreshToken();

  // If we have a field, load it's enabled fields and default file values.
  if (isset($element['#field_name']) && isset($element['#language']) && isset($element['#delta'])) {
    $instance = field_widget_instance($element, $form_state);
    $youtube_fields_enabled_fields = array_keys($instance['widget']['settings']['youtube_fields']['enabled_fields']);
    $default_fid = $instance['default_value'][0]['fid'];
  }
  // If it's not a field but a coded element load configuration from the element
  // itself.
  else {
    $youtube_fields_enabled_fields = array_keys($element['#youtube_fields']['enabled_fields']);
    $default_fid = $element['#youtube_fields']['default_fid'];
  }

  $errors = form_get_errors();
  $youtube_fields = trim(drupal_render($element['youtube_fields']));
  $actions = trim(drupal_render($element['actions']));
  $fid = trim(drupal_render($element['fid']));
  $field_name = array_shift($parents);
  $upload = $field_name . '[' . implode('][', $parents) . '][\'upload\']';
  $id = '#' . $element['#id'];
  $fid_selector = 'input[name="' . $field_name . '[' . implode('][', array_merge($parents, array('fid'))) . ']"]';

  if ($errors) {
    $commands[] = ajax_command_replace(NULL, $youtube_fields);
    $commands[0]['effect'] = 'slide';
    $commands[0]['speed'] = 'slow';
    $commands[] = ajax_command_prepend(NULL, theme('status_messages'));
    $commands[1]['effect'] = 'slide';
    $commands[1]['speed'] = 'slow';
  }
  else {
    $commands[] = ajax_command_replace(NULL, $youtube_fields);
    $commands[0]['effect'] = 'slide';
    $commands[0]['speed'] = 'slow';
    $commands[] = ajax_command_prepend(NULL, theme('status_messages'));
    $commands[1]['effect'] = 'slide';
    $commands[1]['speed'] = 'slow';
    $commands[] = ajax_command_replace($fid_selector, $fid);

    if ($element['fid']['#value'] !== $default_fid) {

      if ($button_key === 'upload_button') {
        // Get the altered file.
        $file = $element['#file'];
        // Unset any fid if any.
        unset($file->fid);
        // Grab it's values.
        $values = array();
        // Set the language for our file upload.
        $language = LANGUAGE_NONE;
        if (module_exists('entity_translation')) {
          module_load_include('inc', 'entity_translation', 'translation.handler');
          $handler = entity_translation_get_handler('file', $file);
          $language = $handler->getDefaultLanguage();
        }
        
        foreach ($youtube_fields_enabled_fields as $field_key) {
          if ($items = field_get_items('file', $file, $field_key, $language)) {
            foreach ($items as $item) {
              if (isset($item['value'])) {
                $values[$field_key] = $item['value'];
              }
              elseif (isset($item['tid'])) {
                $term = taxonomy_term_load($item['tid']);
                $values[$field_key][] = $term->name;
              }
            }
          }
        }
        // Send out ajax command to start the upload.
        $commands[] = ajax_command_invoke(
          NULL, "startUpload", array(
            json_encode(
              array(
                'upload' => $upload,
                'token' => $tok['token'],
                'fields' => $values,
                'id' => $id,
              )
            )
          )
        );
      }
    }
    else {
      $commands[] = ajax_command_replace('div.edit-' . str_replace('_', '-', implode('-', $element['#parents'])) . '-actions', $actions);
    }
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Submit callback for the remove button on media_youtube_upload elements.
 */
function media_youtube_upload_upload_remove_submit($form, &$form_state) {
  // Determine whether it was the upload or the remove button that was clicked,
  // and set $element to the cors upload element that contains that button.
  $parents = $form_state['triggering_element']['#array_parents'];
  $button_key = array_pop($parents);
  // Also lose the actions wrapper to get to the element itself.
  array_pop($parents);
  $element = drupal_array_get_nested_value($form, $parents);

  $delta = array_pop($parents);

  // Rebuild the form to reflect updated values.
  $form_state['rebuild'] = TRUE;

  // No action is needed here for the upload button, because all file uploads on
  // the form are processed by media_youtube_upload_upload_value() regardless of which
  // button was clicked. Action is needed here for the remove button, because we
  // only remove a file in response to its remove button being clicked.
  if ($button_key == 'remove_button') {
    // If it's a temporary file we can safely remove it immediately, otherwise
    // it's up to the implementing module to clean up files that are in use.
    if ($element['#file'] && $element['#file']->status == 0) {
      file_delete($element['#file']);
    }
    // Update both $form_state['values'] and $form_state['input'] to reflect
    // that the file has been removed, so that the form is rebuilt correctly.
    // $form_state['values'] must be updated in case additional submit handlers
    // run, and for form building functions that run during the rebuild, such as
    // when the media_youtube_upload_upload element is part of a field widget.
    // $form_state['input'] must be updated so that media_youtube_upload_upload_value()
    // has correct information during the rebuild.
    form_set_value($element['fid'], 0, $form_state);
    drupal_array_set_nested_value($form_state['input'], $element['#parents'], array('fid' => 0));
  }
  elseif ($button_key == 'save_button') {

    // Set the correct fid on the form. When the node get's saved the file is
    // also correctly referenced. Otherwise we wouldn't see any file although
    // it has been saved.
    $input = drupal_array_get_nested_value($form_state['input'], $element['#parents']);
    drupal_array_set_nested_value($form_state['input'], $element['#parents'], array('fid' => $element['fid']['#value']));
    form_set_value($element['fid'], $input['fid'], $form_state);
  }
}

/**
 * Ajax callback to return the entire youtube upload field.
 */
function media_youtube_upload_upload_js($form, &$form_state) {
  // Find the element that triggered the AJAX callback and return it so that it
  // can be replaced.
  $parents = $form_state['triggering_element']['#array_parents'];
  $button_key = array_pop($parents);
  // Also lose the actions wrapper to get to the element itself.
  array_pop($parents);
  $element = drupal_array_get_nested_value($form, $parents);

  // If no redirection is set, just return the element.
  if (!isset($element['#youtube_redirect'])) {
    return $element;
  }
  // Redirect to the element set redirect url.
  elseif (!form_get_error($element) && !empty($element['fid']['#default_value'])) {
    $commands = array();
    ctools_include('ajax');
    ctools_add_js('ajax-responder');
    // If we have a token with current-page:url replace it with the form action.
    // Else we would redirect to system/ajax.
    if (strpos($element['#youtube_redirect'], '[current-page:url]') !== FALSE || empty($element['#youtube_redirect'])) {
      $element['#youtube_redirect'] = str_replace('[current-page:url]', $form['#action'], $element['#youtube_redirect']);
      // If a redirect to the current page is set it is hard to see that the
      // file has actually uploaded. Therefore we display a message that it has.
      // TODO: Make the message optional and customizable.
      drupal_set_message(t('Upload to YouTube successful. Click <a href="@url">here</a> to see your file.', array('@url' => url('/file/' . $element['fid']['#default_value']))));
    }
    // Replace the tokens.
    $url = token_replace($element['#youtube_redirect'], array('file' => file_load($element['fid']['#default_value'])));
    // Execute redirect command.
    $commands[] = ctools_ajax_command_redirect($url);
    return array('#type' => 'ajax', '#commands' => $commands);
  }
}
