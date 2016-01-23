<?php

/**
 * @file
 * Offers functionality for creating nodes based on results of search query.
 */

/**
 * Implements hook_form().
 */
function open_platform_preview_form() {
  $form = array();
  $options = array();
  $header = array();
  $empty_text = t('This data is unavailable');
  $no_content_text = t('No content available or your API key may not be valid check the <a href="@url"> status report </a> page for more info', array('@url' => url('admin/reports/status')));
  $api_key = variable_get('open_platform_api_key');
  $op_data = open_platform_get_data();
  $data = open_platform_format_data('preview', $op_data);

  // These fields are used for display purposes in the preview form.
  if (!isset($fields['thumbnail'])) {
    $fields['thumbnail'] = 'thumbnail';
  }
  if (!isset($fields['trailText'])) {
    $fields['trailText'] = 'trailText';
  }

  // Using a for instead of a foreach means we do not have to declare variables
  // we are not going to use i.e $key and $value
  // e.g foreach(array as $key =>value).
  if (isset($data['content_array'])) {
    for ($x = 0; $x < count($data); ++$x) {

      $content_id = $data['content_array'][$x]['id'];
      $headline = ($data['headline'][$x] != NULL) ? $data['headline'][$x] : $data['content_array'][$x]['webTitle'];
      $body = ($data['body'][$x] != NULL) ? $data['body'][$x] : $empty_text;
      $trailtext = ($data['trail_text'][$x] != NULL) ? $data['trail_text'][$x] : $empty_text;
      $thumbnail = ($data['thumbnail'][$x] != NULL) ? $data['thumbnail'][$x] : $empty_text;
      $wordcount = ($data['wordcount'][$x] != NULL) ? $data['wordcount'][$x] : $empty_text;
      $url = $data['content_array'][$x]['webUrl'];
      $logo = $data['logo'];

      // Check if content already exist on site.
      $publish_check = open_platform_check_published_node($content_id);

      // Set the status messsage.
      $status = open_platform_check_status_of_content($publish_check, $body);

      // Set image variables.
      $img_variables = array(
        'path' => $thumbnail,
        'alt' => 'Article thumbnail image',
        'attributes' => NULL,
      );

      $image = theme('image', $img_variables);
      $options[$content_id] = array(
        'headline' => $headline,
        'trailText' => $trailtext,
        'thumbnail' => $image,
        'wordcount' => $wordcount,
        'status' => $status,
        'body' => array('#type' => 'hidden', '#value' => $body),
        'url' => array('#type' => 'hidden', '#value' => $url),
        'logo' => array('#type' => 'hidden', '#value' => $logo, 'html' => TRUE),
      );
    }
  }
  // Check user chosen options.
  foreach ($fields as $field_key => $field) {
    if ($field == '0') {
      // Remove fields that have not been set from array.
      unset($fields[$field_key]);
    }

    if ($field_key == 'headline') {
      $header[$field_key] = t('Title');
    }
    elseif ($field_key == 'trailText') {
      $header[$field_key] = t('Trail Text');
    }
    elseif ($field_key == 'thumbnail') {
      $header[$field_key] = t('Thumbnail');
    }
    elseif ($field_key == 'wordCount') {
      $header[$field_key] = t('Word Count');
    }
  }

  $header['status'] = t('Status');

  // Build the 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => array('publish' => 'publish'),
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('open_platform_preview_submit'),
  );

  $form['open_platform_preview'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => $no_content_text,
  );

  // Display error message if user does not have an API Key.
  if (empty($api_key)) {
    drupal_set_message(t('You need an API key to publish content on your site you can obtain an API key here') .
        l(t('http://guardian.mashery.com/'), 'http://guardian.mashery.com/' . t('once you have an API key visit the') . l(t('config page'), 'admin/config/services/open-platform')
            . t('for this module and add your API key')), 'warning');
  }
  else {
    return $form;
  }
}

/**
 * Implements of hook_submit().
 */
function open_platform_preview_submit($form, &$form_state) {
  $content = $form_state['complete form']['open_platform_preview']['#options'];
  $options = $form_state['values']['open_platform_preview'];
  $advanced_fields = variable_get('open_platform_advance_fields');

  // Remove non selected items.
  foreach ($options as $choice_item => $choice_value) {
    // Only act on user chosen options.
    if ($choice_item === $choice_value) {
      $wordcount = (open_platform_user_selected_fields('wordcount', $advanced_fields)) ? $content[$choice_item]['wordcount'] : NULL;
      $body = ($content[$choice_item]['body']) ? $content[$choice_item]['body']['#value'] : NULL;
      $content_id = $choice_value;
      $url = $content[$choice_item]['url']['#value'];
      $headline = $content[$choice_item]['headline'];
      $logo = $content[$choice_item]['logo']['#value'];

      if ($form_state['values']['operation'] == 'publish') {
        open_platform_create_node($logo, $headline, $body, $wordcount, $content_id, $url);
      }
    }
  }
}
