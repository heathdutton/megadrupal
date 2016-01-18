<?php

/**
 * @file
 * The "widget" (form chucklet that goes on the node form)
 */

function _lingwo_pron_widget(&$form, &$form_state, $entry) {
  // Prepare our pron list
  $pron_list = $entry->pron->getItems();
  // handle an upload if there was one
  _lingwo_pron_handle_upload($form, $form_state, $pron_list);
  foreach ($pron_list as $index => $item) {
    // remove the items marked for removal
    if (!empty($form_state['input']['lingwo_pron'][$index]['remove'])) {
      unset($pron_list[$index]);
    }
  }
  if (count($pron_list) == 0) {
    $pron_list = array(array());
  }
  if (!empty($form_state['clicked_button']['#value']) && $form_state['clicked_button']['#value'] == t('Add another pronunciation')) {
    $pron_list[] = array();
  }

  $element = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  $element['wrapper'] = array(
    '#prefix' => '<div id="edit-lingwo_pron-wrapper">',
    '#suffix' => '</div>',
  );

  $element['wrapper']['pron_list'] = array(
    '#theme' => 'lingwo_pron_form',
    '#parents' => array('lingwo_pron'),
  );

  $enabled_fields = array();
  foreach (LingwoPron::$settings->enabled_fields as $name) {
    $enabled_fields[$name] = TRUE;
  }

  if ($enabled_fields['audio']) {
    $form['#attributes']['enctype'] = 'multipart/form-data';
  }

  $ajax = array(
    'callback' => '_lingwo_pron_ajax_callback',
    'wrapper' => 'edit-lingwo_pron-wrapper',
    'effect' => 'none',
  );

  foreach ($pron_list as $index => $pron) {
    $item = array();
    if (isset($enabled_fields['ipa'])) {
      $item['ipa'] = array(
        '#type' => 'textfield',
        '#title' => t('IPA (International Phonetic Alphabet)'),
        '#default_value' => isset($pron['ipa']) ? $pron['ipa'] : '',
      );
    }
    if (isset($enabled_fields['tag'])) {
      $tag_options = LingwoPron::getTagOptions($entry->language, TRUE);
      if (!empty($tag_options)) {
        $item['tag'] = array(
          '#type' => 'select',
          '#title' => t('Tag'),
          '#options' => array('' => t('None')) + $tag_options,
          '#default_value' => isset($pron['tag']) ? $pron['tag'] : '',
        );
      }
    }
    if (isset($enabled_fields['audio'])) {
      if (empty($pron['audio'])) {
        $upload_name = implode('_', array('lingwo_pron', 'audio', $index));
        $item['audio'] = array(
          '#type' => 'file',
          '#title' => t('Audio file'),
          '#name' => "files[$upload_name]",
          '#size' => 22,
        );
        $item['upload'] = array(
          '#type' => 'button',
          '#value' => t('Upload'),
          '#delta' => $index,
          '#upload_name' => $upload_name,
          '#attributes' => array('class' => array('lingwo-pron-upload')),
        );
      }
      else {
        $item['audio'] = array(
          '#type' => 'value',
          '#value' => $pron['audio'],
        );
        $item['audio_widget'] = array(
          '#type' => 'item',
          '#title' => t('Audio'),
          '#markup' => theme('lingwo_pron_audio_widget', array('file' => $pron['audio'])),
        );
      }
    }

    $item['weight'] = array(
      '#title' => t('Weight'),
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $index,
    );
    $item['remove'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove'),
      '#attributes' => array('class' => array('lingwo-pron-remove')),
    );

    $element['wrapper']['pron_list'][] = $item;
    $index++;
  }

  $_lingwo_pron = array(
    '#parents' => array('_lingwo_pron'),
    '#tree' => TRUE,
  );
  $_lingwo_pron['add'] = array(
    '#type' => 'button',
    '#value' => t('Add another pronunciation'),
    '#ajax' => $ajax,
    '#limit_validation_errors' => array(),
  );
  $_lingwo_pron['refresh'] = array(
    '#type' => 'button',
    '#value' => t('Refresh pronunciation'),
    '#ajax' => $ajax,
    '#attributes' => array('class' => array('js-hide')),
    '#limit_validation_errors' => array(),
  );

  $element['_lingwo_pron'] = $_lingwo_pron;

  $element['#attached']['js'][] = drupal_get_path('module', 'lingwo_pron') . '/lingwo_pron.js';

  return $element;
}

function _lingwo_pron_ajax_callback($form, &$form_state) {
  return $form['lingwo_pron']['wrapper'];
}

function _lingwo_pron_handle_upload($form, &$form_state, &$pron_list) {
  if (empty($_FILES['files']['error'])) {
    return;
  }

  $upload_name = NULL;
  $delta = NULL;
  foreach ((array) $_FILES['files']['error'] as $source => $code) {
    if ($code != UPLOAD_ERR_NO_FILE) {
      $upload_name = $source;
      if (preg_match('/^lingwo_pron_audio_(\d+)$/', $upload_name, $matches)) {
        $delta = intval($matches[1]);
      }
      break;
    }
  }

  if (!is_null($upload_name) && !is_null($delta)) {
    if ($dest = _lingwo_pron_file_path()) {
      $max_filesize = parse_size(file_upload_max_size());
      if ($local_max_filesize = parse_size(LingwoPron::$settings->audio_file_size_max)) {
        $max_filesize = min($max_filesize, $local_max_filesize);
      }

      $validators = array(
        'file_validate_extensions' => array(LingwoPron::$settings->audio_file_extensions),
        'file_validate_size' => array($max_filesize, 0),
      );

      // make the upload into a temporary file
      $file = file_save_upload($upload_name, $validators, $dest, FILE_EXISTS_RENAME);

      // save the file to the node
      $pron_list[$delta]['audio'] = $file;
    }
  }
}

