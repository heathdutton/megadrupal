<?php

/**
 * @file
 * Implements Field module hooks for jqueryui formatters.
 */

/**
 * Implements hook_field_formatter_info().
 */
function jqueryui_formatters_field_formatter_info() {
  return array(
    'jqueryui_formatters_dialog' => array(
      'label' => t('Dialog'),
      'field types' => array('text', 'text_long', 'text_with_summary'),
      'settings' => array(
        'link_text' => t('Click me'),
        'modal' => FALSE,
        'resizable' => FALSE,
        'escape' => FALSE,
      ),
    ),
    'jqueryui_formatters_accordion' => array(
      'label' => t('Accordion'),
      'field types' => array('text', 'text_long', 'text_with_summary'),
      'settings' => array(
        'header' => 'h3',
        'autoheight' => FALSE,
      ),
    ),
    'jqueryui_formatters_tabs' => array(
      'label' => t('Tabs'),
      'field types' => array('text', 'text_long', 'text_with_summary'),
      'settings' => array(
        'collapsible' => FALSE,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function jqueryui_formatters_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {

  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $form = array();

  if ($display['type'] == 'jqueryui_formatters_dialog') {

    $form['link_text'] = array(
      '#title' => t('Write here the text linking the popup'),
      '#type' => 'textfield',
      '#default_value' => $settings['link_text'],
      '#required' => TRUE,
    );

    $form['modal'] = array(
      '#title' => t('Show dialog like modal'),
      '#type' => 'checkbox',
      '#default_value' => $settings['modal'],
    );

    $form['resizable'] = array(
      '#title' => t('Show resizable dialog'),
      '#type' => 'checkbox',
      '#default_value' => $settings['resizable'],
    );

    $form['escape'] = array(
      '#title' => t('Close on escape'),
      '#type' => 'checkbox',
      '#default_value' => $settings['escape'],
    );

  }

  if ($display['type'] == 'jqueryui_formatters_accordion') {

    $options_header = array(
      'h3' => 'h3',
      'h4' => 'h4',
      'h5' => 'h5',
      'h6' => 'h6',
    );

    $form['header'] = array(
      '#title' => t('header'),
      '#type' => 'select',
      '#default_value' => $settings['header'],
      '#options' => $options_header,
    );

    $form['autoheight'] = array(
      '#title' => t('autoheight'),
      '#type' => 'checkbox',
      '#default_value' => $settings['autoheight'],
    );
  }

  if ($display['type'] == 'jqueryui_formatters_tabs') {

    $form['collapsible'] = array(
      '#title' => t('Collapsible'),
      '#type' => 'checkbox',
      '#default_value' => $settings['collapsible'],
    );

  }

  return $form;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function jqueryui_formatters_field_formatter_settings_summary($field, $instance, $view_mode) {

  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = '';

  if ($display['type'] == 'jqueryui_formatters_dialog') {

    $summary = t('Link text: @link_text', array('@link_text' => $settings['link_text']));
    $summary .= "<br />" . t('Modal: @modal', array('@modal' => $settings['modal'] == 0 ? t('False') : t('True')));
    $summary .= "<br />" . t('Resizable: @resizable', array('@resizable' => $settings['resizable'] == 0 ? t('False') : t('True')));
    $summary .= "<br />" . t('Close on escape: @escape', array('@escape' => $settings['escape'] == 0 ? t('False') : t('True')));

  }

  if ($display['type'] == 'jqueryui_formatters_accordion') {

    $summary = t('Header: @header', array('@header' => $settings['header']));
    $summary .= "<br />" . t('AutoHeight: @autoheight', array('@autoheight' => $settings['autoheight'] == 0 ? t('False') : t('True')));

  }

  if ($display['type'] == 'jqueryui_formatters_tabs') {

    $summary = t('Collapsible: @collapsible', array('@collapsible' => $settings['collapsible'] == 0 ? t('False') : t('True')));

  }

  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function jqueryui_formatters_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();
  $settings = $display['settings'];
  $entity_id = (entity_id($entity_type, $entity));
  $field_id = $field['id'];
  $instance_id = $instance['id'];
  $unique_instance_id = $entity_id . '-' . $field_id . '-' . $instance_id;

  switch ($display['type']) {

    case 'jqueryui_formatters_dialog':

      if (!empty($items)) {

        foreach ($items as $delta => $item) {
          $sanitized_text = _text_sanitize($instance, $langcode, $item, 'value');

          $element[$delta] = array(
            '#theme' => 'jqueryui_formatters_dialog',
            '#text' => $sanitized_text,
            '#title' => $instance['label'],
            '#unique_instance_id' => $unique_instance_id,
            '#delta' => $delta,
            '#link_text' => $settings['link_text'],
          );
        }

        // Attaching js settings as arrays in case there is more
        // than one field with this formatter in the same request.
        $modal = array();
        $modal[$unique_instance_id] = $settings['modal'];

        $resizable = array();
        $resizable[$unique_instance_id] = $settings['resizable'];

        $escape = array();
        $escape[$unique_instance_id] = $settings['escape'];

        $element['#attached'] = array(
          'js' => array(
            array(
              'data' => array(
                'jqueryuiFormattersDialog' => array(
                  'modal' => $modal,
                  'resizable' => $resizable,
                  'closeOnEscape' => $escape,
                ),
              ),
              'type' => 'setting',
            ),
            array(
              'data' => drupal_get_path('module', 'jqueryui_formatters') . '/js/jqueryui-formatters-dialog.js',
              'type' => 'file',
            ),
          ),
          'library' => array(
            array('system', 'ui.dialog'),
          ),
        );

      }

      break;

    case 'jqueryui_formatters_accordion':

      if (!empty($items)) {

        $element[] = array(
          '#theme' => 'jqueryui_formatters_accordion',
          '#header' => $settings['header'],
          '#instance' => $instance,
          '#langcode' => $langcode,
          '#items' => $items,
          '#unique_instance_id' => $unique_instance_id,
        );

        // Attaching js settings as arrays in case there is more
        // than one field with this formatter in the same request.
        $header = array();
        $header[$unique_instance_id] = $settings['header'];

        $heightstyle_accordion = array();
        $heightstyle_accordion[$unique_instance_id] = $settings['autoheight'];

        $element['#attached'] = array(
          'js' => array(
            array(
              'data' => array(
                'jqueryuiFormattersAccordion' => array(
                  'header' => $header,
                  'autoHeight' => $heightstyle_accordion,
                ),
              ),
              'type' => 'setting',
            ),
            array(
              'data' => drupal_get_path('module', 'jqueryui_formatters') . '/js/jqueryui-formatters-accordion.js',
              'type' => 'file',
            ),
          ),
          'library' => array(
            array('system', 'ui.accordion'),
          ),
        );

      }

      break;

    case 'jqueryui_formatters_tabs':

      if (!empty($items)) {

        $element[] = array(
          '#theme' => 'jqueryui_formatters_tabs',
          '#instance' => $instance,
          '#langcode' => $langcode,
          '#items' => $items,
          '#unique_instance_id' => $unique_instance_id,
        );

        // Attaching js settings as arrays in case there is more
        // than one field with this formatter in the same request.
        $collapsible = array();
        $collapsible[$unique_instance_id] = $settings['collapsible'];

        $element['#attached'] = array(
          'js' => array(
            array(
              'data' => array(
                'jqueryuiFormattersTabs' => array(
                  'collapsible' => $collapsible,
                ),
              ),
              'type' => 'setting',
            ),
            array(
              'data' => drupal_get_path('module', 'jqueryui_formatters') . '/js/jqueryui-formatters-tabs.js',
              'type' => 'file',
            ),
          ),
          'library' => array(
            array('system', 'ui.tabs'),
          ),
        );

      }

      break;

    default:
      break;
  }

  return $element;
}

/**
 * Returns HTML for accordion field formatter.
 *
 * @param array $variables
 *   An associative array containing:
 *   - text: A sanitizied string withi the content to show,
 *   - title: title for the popup,
 *   - unique_instance_id: unique id for instance,
 *   - delta: item number useful for multiple values fields,
 *   - link_text: linkable text to display the dialog,
 *
 * @ingroup themeable
 */
function theme_jqueryui_formatters_dialog($variables) {

  $text = $variables['text'];
  $title = $variables['title'];
  $unique_instance_id = $variables['unique_instance_id'];
  $delta = $variables['delta'];
  $link_text = $variables['link_text'];

  $unic_item_id = $unique_instance_id . '-' . $delta;

  $link = l($link_text, '',
    array(
      'attributes' => array(
        'class' => array('jqueryui-formatters', 'opener'),
        'data-instance' => array($unique_instance_id),
        'data-delta' => array($unic_item_id),
      ),
      'fragment' => 'jquerui-formatter-dialog',
      'external' => TRUE,
    )
  );
  $output = $link . '<div class="jqueryui-formatters-text" title="' . $title . '" style="display:none;" id="jqueryui-formatters-text-' . $unic_item_id . '">' . $text . '</div>';

  return $output;
}

/**
 * Returns HTML for an dialog field formatter.
 *
 * @param array $variables
 *   An associative array containing:
 *   - header: header html tag to build the element,
 *   - instance: Associative array of instance field data,
 *   - langcode: field langcode,
 *   - items: Associative array containing the multiple
 *     values of an instance to be rendered,
 *   - unique_instance_id: unique id for instance,
 *
 * @ingroup themeable
 */
function theme_jqueryui_formatters_accordion($variables) {

  $header = $variables['header'];
  $instance = $variables['instance'];
  $langcode = $variables['langcode'];
  $items = $variables['items'];
  $unique_instance_id = $variables['unique_instance_id'];

  $output = '<div class="jqueryui-formatters accordion" data-instance="' . $unique_instance_id . '">';
  foreach ($items as $delta => $item) {
    $sanitized_text = _text_sanitize($instance, $langcode, $item, 'value');
    $link = l($instance['label'] . "\n$delta", '',
      array(
        'external' => TRUE,
        'fragment' => "jqueryui-formatters-accordion",
      )
    );
    $output .= "<$header>" . $link . "</$header><div><p>" . $sanitized_text . '</p></div>';
  }
  $output .= '</div>';

  return $output;
}

/**
 * Returns HTML for tabs field formatter.
 *
 * @param array $variables
 *   An associative array containing:
 *   - instance: Associative array of instance field data,
 *   - langcode: field langcode,
 *   - items: Associative array containing the multiple
 *     values of an instance to be rendered,
 *   - unique_instance_id: unique id for instance,
 *
 * @ingroup themeable
 */
function theme_jqueryui_formatters_tabs($variables) {

  $items = $variables['items'];
  $instance = $variables['instance'];
  $langcode = $variables['langcode'];
  $unique_instance_id = $variables['unique_instance_id'];

  $content = '';
  foreach ($items as $delta => $item) {
    $sanitized_text = _text_sanitize($instance, $langcode, $item, 'value');
    $content .= '<div id="tabs-' . $delta . '"><p>' . $sanitized_text . '</p></div>';
  }

  $items = array();
  for ($tab_item = 0; $tab_item <= $delta; $tab_item++) {
    $items[] = l($instance['label'] . "\n$tab_item", '',
      array(
        'external' => TRUE,
        'fragment' => "tabs-$tab_item",
      )
    );
  }
  $header = array(
    'type' => 'ol',
    'items' => $items,
  );

  $output = '<div class="jqueryui-formatters tabs" data-instance="' . $unique_instance_id . '">';
  $output .= theme('item_list', $header);
  $output .= $content;
  $output .= '</div>';

  return $output;
}
