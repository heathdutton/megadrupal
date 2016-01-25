<?php

/**
 * @file
 * Field formatter hooks to implement a readmore.
 */

/**
 * Implements hook_field_formatter_info().
 */
function readmore_field_formatter_info() {
  return array(
    'readmore' => array(
      'label' => t('Read more'),
      'field types' => array(
        'text',
        'text_long',
        'text_with_summary',
      ),
      'settings' => array(
        'readmore_trim_length' => 500,
        'readmore_trim_on_break' => TRUE,
        'readmore_show_readless' => FALSE,
        'readmore_ellipsis' => TRUE,
        'readmore_wordsafe' => FALSE,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function readmore_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  if ($display['type'] == 'readmore') {
    $element['readmore_trim_length'] = array(
      '#title' => t('Trim length'),
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => $settings['readmore_trim_length'],
      '#element_validate' => array('element_validate_integer_positive'),
      '#required' => TRUE,
    );
    $element['readmore_trim_on_break'] = array(
      '#type' => 'checkbox',
      '#title' => t('Trim on @break', array('@break' => '<!--break-->')),
      '#description' => t('If @break not found in the text then trim length used.', array('@break' => '<!--break-->')),
      '#default_value' => $settings['readmore_trim_on_break'],
    );
    $element['readmore_show_readless'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show read less'),
      '#default_value' => $settings['readmore_show_readless'],
    );
    $element['readmore_ellipsis'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add ellipsis'),
      '#default_value' => $settings['readmore_ellipsis'],
    );
    $element['readmore_wordsafe'] = array(
      '#type' => 'checkbox',
      '#title' => t('Truncate on a word boundary'),
      '#default_value' => $settings['readmore_wordsafe'],
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function readmore_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = '';

  if ($display['type'] == 'readmore') {
    $summary[] = t('Trim length: @length', array('@length' => $settings['readmore_trim_length']));
    $summary[] = $settings['readmore_trim_on_break'] ? t('Trim on @break', array('@break' => '<!--break-->')) : t('Do not trim on @break', array('@break' => '<!--break-->'));
    $summary[] = $settings['readmore_show_readless'] ? t('Show read less') : t('Do not show read less');
    $summary[] = $settings['readmore_ellipsis'] ? t('Add ellipsis') : t('Do not add ellipsis');
    $summary[] = $settings['readmore_wordsafe'] ? t('Safe words') : t('Do not safe words');
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function readmore_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  switch ($display['type']) {
    case 'readmore':
      drupal_add_js(READMORE_MODULE_PATH . '/js/readmore.js');
      foreach ($items as $delta => $item) {
        $text = _text_sanitize($instance, $langcode, $item, 'value');
        $text_length = drupal_strlen($item['value']);
        $trim_length = $display['settings']['readmore_trim_length'];

        // Don't do anything if text length less than defined.
        if ($text_length > $trim_length) {
          // Add Read less if need.
          if ($display['settings']['readmore_show_readless']) {
            $text .= ' ' . l(t('Read less'), current_path(), array('attributes' => array('class' => array('readless-link'))));
          }

          // Get trimmed string.
          $summary = readmore_truncate_string(
            $text,
            isset($item['format']) ? $item['format'] : NULL,
            $trim_length,
            $display['settings']['readmore_wordsafe'],
            $display['settings']['readmore_trim_on_break']
          );

          // Add readmore link.
          $summary .= '<span>';
          $summary .= $display['settings']['readmore_ellipsis'] ? t('...') : NULL;
          $summary .= l(t('read more'), current_path(), array('attributes' => array('class' => array('readmore-link'))));
          $summary .= '</span>';

          // Close all HTML tags.
          $summary = _filter_htmlcorrector($summary);

          $element[$delta] = array(
            '#markup' => theme(
              'readmore',
              array(
                'text' => $text,
                'summary' => $summary,
              )
            ),
          );
        }
        else {
          $element[$delta] = array(
            '#markup' => $text,
          );
        }
      }
      break;
  }

  return $element;
}
