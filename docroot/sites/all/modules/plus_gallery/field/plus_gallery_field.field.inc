<?php

/**
 * @file
 * Field module functionality for the Plus Gallery module.
 */

// Load all constants for Plus Gallery.
require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'plus_gallery') . '/plus_gallery.const.inc';

/**
 * Implements hook_field_info().
 */
function plus_gallery_field_field_info() {
  return array(
    'plus_gallery_field' => array(
      'label' => t('Plus Gallery'),
      'description' => t('The fieldset containing all the needed fields for the Plus Gallery'),
      'settings' => array(
        'fb' => TRUE,
        'gplus' => TRUE,
        'flickr' => TRUE,
        'instagram' => TRUE,
      ),
      'default_widget' => 'plus_gallery_field_field_widget',
      'default_formatter' => 'plus_gallery_field_field_formatter',
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function plus_gallery_field_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];

  $form = array();
  if ($field['type'] == 'plus_gallery_field') {
    $form['credit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Credit'),
      '#description' => t('Add the credit footer to the galleries.'),
      '#default_value' => isset($settings['credit']) ? $settings['credit'] : NULL,
      '#disabled' => $has_data,
    );
    $form['gallery_mode'] = array(
      '#type' => 'select',
      '#title' => t('Gallery mode'),
      '#default_value' => isset($settings['gallery_mode']) ? $settings['gallery_mode'] : NULL,
      '#options' => plus_gallery_field_modes(),
      '#required' => TRUE,
      '#description' => '<strong>' . t('Gallery overview') . '</strong>: ' . t('Gives an overview of the albums in the account.') .
      '<br /><strong>' . t('Album mode') . ':</strong> ' . t('Show one specific album.'),
      '#element_validate' => array('plus_gallery_field_validate_mode'),
      '#disabled' => $has_data,
    );
    $form['fb'] = array(
      '#type' => 'checkbox',
      '#title' => t('Facebook'),
      '#default_value' => isset($settings['fb']) ? $settings['fb'] : 1,
      '#required' => FALSE,
      '#description' => t('Allow users to enter data for Facebook.'),
      '#disabled' => $has_data,
    );
    $form['gplus'] = array(
      '#type' => 'checkbox',
      '#title' => t('Google+'),
      '#default_value' => isset($settings['gplus']) ? $settings['gplus'] : 1,
      '#required' => FALSE,
      '#description' => t('Allow users to enter data for Google+.'),
      '#disabled' => $has_data,
    );
    $form['flickr'] = array(
      '#type' => 'checkbox',
      '#title' => t('Flickr'),
      '#default_value' => isset($settings['flickr']) ? $settings['flickr'] : 1,
      '#required' => FALSE,
      '#description' => t('Allow users to enter data for Flickr.'),
      '#disabled' => $has_data,
    );
    $form['instagram'] = array(
      '#type' => 'checkbox',
      '#title' => t('Instagram'),
      '#default_value' => isset($settings['instagram']) ? $settings['instagram'] : 1,
      '#required' => FALSE,
      '#description' => t('Allow users to enter data for Instagram.'),
      '#disabled' => $has_data,
    );
  }

  return $form;
}

/**
 * Implements hook_field_widget_info().
 */
function plus_gallery_field_field_widget_info() {
  return array(
    'plus_gallery_field_field_widget' => array(
      'label' => t('Plus Gallery'),
      'field types' => array('plus_gallery_field'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function plus_gallery_field_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  switch ($instance['widget']['type']) {
    case 'plus_gallery_field_field_widget':
      $options = plus_gallery_field_options($field['settings']);
      $default_type = isset($items[$delta]['data_type']) ? $items[$delta]['data_type'] : 0;

      $element['#type'] = 'fieldset';
      $element['data_type'] = array(
        '#type' => 'select',
        '#title' => t('Type'),
        '#default_value' => $default_type,
        '#options' => $options,
      );
      $element['userid'] = array(
        '#type' => 'textfield',
        '#title' => t('User ID'),
        '#default_value' => isset($items[$delta]['userid']) ? $items[$delta]['userid'] : NULL,
        '#size' => 60,
        '#maxlength' => 63,
        '#states' => array(
          'visible' => array(
            _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'data_type') => array('!value' => PLUS_GALLERY_NONE),
          ),
        ),
      );
      if (array_key_exists(PLUS_GALLERY_FACEBOOK, $options)
        || array_key_exists(PLUS_GALLERY_INSTAGRAM, $options)) {
        $element['access_token'] = array(
          '#type' => 'textfield',
          '#title' => t('Access token'),
          '#default_value' => isset($items[$delta]['access_token']) ? $items[$delta]['access_token'] : NULL,
          '#size' => 60,
          '#maxlength' => 255,
          '#states' => array(
            'visible' => array(
              _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'data_type') => array(
                array('value' => PLUS_GALLERY_FACEBOOK),
                array('value' => PLUS_GALLERY_INSTAGRAM),
              ),
            ),
          ),
        );
      }
      if (array_key_exists(PLUS_GALLERY_FLICKR, $options)) {
        $element['api_key'] = array(
          '#type' => 'textfield',
          '#title' => t('Flickr API key'),
          '#default_value' => isset($items[$delta]['api_key']) ? $items[$delta]['api_key'] : NULL,
          '#size' => 60,
          '#maxlength' => 63,
          '#states' => array(
            'visible' => array(
              _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'data_type') => array('value' => PLUS_GALLERY_FLICKR),
            ),
          ),
        );
      }

      if ($field['settings']['gallery_mode'] == PLUS_GALLERY_MODE_ALBUM_MODE) {
        $collapse = (!isset($items[$delta]['data_limit']) && !isset($items[$delta]['album_title']))
                    || ($items[$delta]['data_limit'] == NULL && !$items[$delta]['album_title']);
      }
      else {
        $collapse = (!isset($items[$delta]['album_limit']) && !isset($items[$delta]['incl_excl']))
                    || ($items[$delta]['album_limit'] == NULL && $items[$delta]['incl_excl'] == PLUS_GALLERY_BEHAVIOUR_ALL);
      }
      $element['optional'] = array(
        '#type' => 'fieldset',
        '#title' => t('Optional settings'),
        '#weight' => 10,
        '#attached' => array(
          'js' => array(
            'misc/form.js',
            'misc/collapse.js',
          ),
        ),
        '#attributes' => array(
          'class' => array('collapsible', ($collapse ? 'collapsed' : '')),
        ),
        '#states' => array(
          'visible' => array(
            _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'data_type') => array('!value' => PLUS_GALLERY_NONE),
          ),
        ),
      );

      // Fields for album mode.
      if ($field['settings']['gallery_mode'] == PLUS_GALLERY_MODE_ALBUM_MODE) {
        $element['album_id'] = array(
          '#type' => 'textfield',
          '#title' => t('Album ID'),
          '#description' => t('What is the ID of the album?'),
          '#default_value' => isset($items[$delta]['album_id']) ? $items[$delta]['album_id'] : NULL,
          '#size' => 60,
          '#maxlength' => 63,
          '#states' => array(
            'visible' => array(
              _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'data_type') => array(
                array('value' => PLUS_GALLERY_FACEBOOK),
                array('value' => PLUS_GALLERY_FLICKR),
                array('value' => PLUS_GALLERY_GOOGLEPLUS),
              ),
            ),
          ),
        );
        $element['optional']['album_title'] = array(
          '#type' => 'checkbox',
          '#title' => t('Album title'),
          '#description' => t('Does the title of the album have to be shown?'),
          '#default_value' => isset($items[$delta]['album_title']) ? $items[$delta]['album_title'] : FALSE,
          '#states' => array(
            'visible' => array(
              _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'data_type') => array(
                array('value' => PLUS_GALLERY_FACEBOOK),
                array('value' => PLUS_GALLERY_FLICKR),
                array('value' => PLUS_GALLERY_GOOGLEPLUS),
              ),
            ),
          ),
        );
        $element['optional']['data_limit'] = array(
          '#type' => 'textfield',
          '#title' => t('Image limit'),
          '#description' => t("How many images have to be loaded? Default is 30. As a best practice keep the size of the galleries around this size.
            Especially on mobile devices, as they don't have the power to load lots of large images in the zoom mode and you will just make them mad.")
          . '<br /><strong>' . t('Note') . '</strong>: ' . t('when using Flickr the maximum is 30!'),
          '#default_value' => isset($items[$delta]['data_limit']) ? $items[$delta]['data_limit'] : NULL,
          '#size' => 2,
          '#maxlength' => 2,
          '#states' => array(
            'visible' => array(
              _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'data_type') => array('!value' => PLUS_GALLERY_NONE),
            ),
          ),
        );
      }
      // Add fields for gallery overview.
      else {
        $element['optional']['album_limit'] = array(
          '#type' => 'textfield',
          '#title' => t('Album limit'),
          '#description' => t('How many albums have to be visible in the overview? Default is 20.'),
          '#default_value' => isset($items[$delta]['album_limit']) ? $items[$delta]['album_limit'] : NULL,
          '#size' => 2,
          '#maxlength' => 2,
          '#states' => array(
            'visible' => array(
              _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'data_type') => array(
                array('value' => PLUS_GALLERY_FACEBOOK),
                array('value' => PLUS_GALLERY_FLICKR),
                array('value' => PLUS_GALLERY_GOOGLEPLUS),
              ),
            ),
          ),
        );
        $element['optional']['incl_excl'] = array(
          '#type' => 'select',
          '#title' => t('Gallery behaviour'),
          '#description' => t('Which galleries do you want to show?') . '<br /><strong>' . t('All') . '</strong>: ' . t('All albums will be shown.')
          . '<br /><strong>' . t('Include') . '</strong>: ' . t('Only the albums whose ID is entered below, will be shown.') . '<br /><strong>' .
          t('Exclude') . '</strong>: ' . t('All albums will be shown, except the ones whose ID is entered below.'),
          '#options' => plus_gallery_field_behaviours(),
          '#default_value' => isset($items[$delta]['incl_excl']) ? $items[$delta]['incl_excl'] : PLUS_GALLERY_BEHAVIOUR_ALL,
          '#states' => array(
            'visible' => array(
              _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'data_type') => array('!value' => PLUS_GALLERY_NONE),
            ),
          ),
        );
        $element['optional']['data_incl_excl'] = array(
          '#type' => 'textfield',
          '#title' => t('Album IDs'),
          '#description' => t('A comma separated list (no spaces) of album IDs. They will be included or excluded based on the choice you made above.'),
          '#default_value' => isset($items[$delta]['data_incl_excl']) ? $items[$delta]['data_incl_excl'] : NULL,
          '#size' => 60,
          '#maxlength' => 16384,
          '#states' => array(
            'visible' => array(
              _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'incl_excl', 'optional') => array('!value' => PLUS_GALLERY_BEHAVIOUR_ALL),
              _plus_gallery_field_jquery_selector($instance['field_name'], $langcode, $delta, 'data_type') => array('!value' => PLUS_GALLERY_NONE),
            ),
          ),
        );
      }
      break;

  }
  return $element;
}

/**
 * Implements hook_field_validate().
 */
function plus_gallery_field_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    $flickr = FALSE;
    switch ($item['data_type']) {
      case PLUS_GALLERY_NONE:
        break;

      case PLUS_GALLERY_FLICKR:
        plus_gallery_field_validate_general($item, $errors, $field, $langcode, $delta, TRUE);
        plus_gallery_field_validate_string($item, NULL, 'api_key', t('API Key'), $errors, $field['field_name'], $langcode, $delta, TRUE);
        break;

      case PLUS_GALLERY_FACEBOOK:
        plus_gallery_field_validate_general($item, $errors, $field, $langcode, $delta);
        plus_gallery_field_validate_access_token($item, $errors, $field['field_name'], $langcode, $delta);
        break;

      case PLUS_GALLERY_GOOGLEPLUS:
        plus_gallery_field_validate_general($item, $errors, $field, $langcode, $delta);
        break;

      case PLUS_GALLERY_INSTAGRAM:
        plus_gallery_field_validate_userid($item, $errors, $field['field_name'], $langcode, $delta);
        plus_gallery_field_validate_access_token($item, $errors, $field['field_name'], $langcode, $delta);
        plus_gallery_field_validate_data_limit($item, $errors, $field['field_name'], $langcode, $delta);
        break;

    }
  }
}

/**
 * Implements hook_field_presave().
 */
function plus_gallery_field_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => $item) {
    if (!isset($item['api_key']) || $item['data_type'] != PLUS_GALLERY_FLICKR) {
      $items[$delta]['api_key'] = NULL;
    }
    if (!isset($item['access_token']) ||
        ($item['data_type'] != PLUS_GALLERY_FACEBOOK && $item['data_type'] != PLUS_GALLERY_INSTAGRAM)) {
      $items[$delta]['access_token'] = NULL;
    }
    // Album mode.
    if (!isset($item['album_id']) || empty($item['album_id'])) {
      $items[$delta]['album_id'] = NULL;
    }
    if (!isset($item['optional']['data_limit']) || empty($item['optional']['data_limit'])) {
      $items[$delta]['data_limit'] = NULL;
    }
    else {
      $items[$delta]['data_limit'] = $item['optional']['data_limit'];
    }
    if (!isset($item['optional']['album_title'])) {
      $items[$delta]['album_title'] = NULL;
    }
    else {
      $items[$delta]['album_title'] = $item['optional']['album_title'];
    }
    // Gallery overview.
    if (!isset($item['optional']['album_limit']) || empty($item['optional']['album_limit'])) {
      $items[$delta]['album_limit'] = NULL;
    }
    else {
      $items[$delta]['album_limit'] = $item['optional']['album_limit'];
    }
    if (!isset($item['optional']['incl_excl']) || empty($item['optional']['incl_excl'])) {
      $items[$delta]['incl_excl'] = NULL;
    }
    else {
      $items[$delta]['incl_excl'] = $item['optional']['incl_excl'];
    }
    if (!isset($item['optional']['data_incl_excl']) || $item['optional']['incl_excl'] == PLUS_GALLERY_BEHAVIOUR_ALL) {
      $items[$delta]['data_incl_excl'] = NULL;
    }
    else {
      $items[$delta]['data_incl_excl'] = $item['optional']['data_incl_excl'];
    }
  }
}

/**
 * Implements hook_field_widget_error().
 */
function plus_gallery_field_field_widget_error($element, $error, $form, &$form_state) {
  if ($error['fieldset'] != NULL) {
    form_error($element[$error['fieldset']][$error['name']], $error['message']);
  }
  else {
    form_error($element[$error['name']], $error['message']);
  }
}

/**
 * Implements hook_field_is_empty().
 */
function plus_gallery_field_field_is_empty($item, $field) {
  return empty($item['data_type']) || intval($item['data_type']) === 0;
}
