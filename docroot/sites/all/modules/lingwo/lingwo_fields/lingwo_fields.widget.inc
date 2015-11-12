<?php

/**
 * @file
 * The "widget" (form chucklet that goes on the node form)
 */

function _lingwo_fields_widget(&$form, &$form_state, $entry) {
  $node = &$entry->getNode();
  $language = $node->language;

  // make sure we have a valid POS!
  $pos = isset($node->pos) ? $node->pos : NULL;
  if (!empty($form['pos']['#options']) && (empty($pos) || !isset($form['pos']['#options'][$pos]))) {
    $pos = array_keys($form['pos']['#options']);
    $pos = $pos[0];
    $node->pos = $pos;
  }
  if (isset($form_state['input']['_lingwo_fields']['old_pair']) && ($old_pair = $form_state['input']['_lingwo_fields']['old_pair'])) {
    if ($old_pair != implode(':', array($language, $pos))) {
      // clear the fields, because we just changed the language or pos!
      $node->lingwo_fields = array();
    }
  }
  // add the from to our form list (we have to do this here because in AHAH we can't
  // do it in the submit handler)

  if (isset($form_state['clicked_button']) && $form_state['clicked_button']['#value'] == t('Add new form')) {
    $value = $form_state['input']['_lingwo_fields']['add_new_form'];
    if (!empty($value['value'])) {
      $entry->fields->addForm($value['name'], $value['value']);
    }
  }

  // record if we are supposed to add an extra value to a field
  $extra_value = isset($form_state['input']['_lingwo_fields']['extra_value']) ?
    $form_state['input']['_lingwo_fields']['extra_value'] : NULL;

  $element = array(
    '#type' => 'fieldset',
    '#title' => t('Fields'),
    '#tree' => TRUE,
  );

  $ajax = array(
    'callback' => '_lingwo_fields_ajax_callback',
    'wrapper' => 'lingwo_fields-wrapper',
    'effect' => 'none',
  );

  $element['wrapper'] = array(
    '#prefix' => '<div id="lingwo_fields-wrapper">',
    '#suffix' => '</div>',
  );
  $element['wrapper']['fields'] = array(
    '#theme' => array(
      'lingwo_fields_widget_form__' . $language . '__' . $pos,
      'lingwo_fields_widget_form__' . $language,
      'lingwo_fields_widget_form',
    ),
    '#parents' => array('lingwo_fields'),
  );

  $field_map = array(
    'classes' => array(),
    'options' => array(),
    'forms' => array(),
  );

  foreach ($entry->fields as $name => $field) {
    // skip it was marked for removal
    if (!empty($form_state['values']['lingwo_fields'][$name]['remove'])) {
      continue;
    }

    $def = isset($field['definition']) ? $field['definition'] : array();
    unset($field['definition']);

    $control_tag  = '<div ';
    $control_tag .= 'class="lingwo-fields-control" ';
    $control_tag .= 'data-type="' . $field['type'] . '" ';
    $control_tag .= 'data-name="' . $name . '" ';
    if (!empty($def)) {
      $control_tag .= 'data-has-definition="true" ';
    }
    $control_tag .= '>';

    $field_form = array(
      '#prefix' => $control_tag,
      '#suffix' => '</div>',
      '#item' => $field,
    );
    $field_form['name'] = array(
      '#type' => 'hidden',
      '#value' => $name,
    );
    $field_form['type'] = array(
      '#type' => 'hidden',
      '#value' => $field['type'],
    );

    $field_form['value'] = array(
      '#title' => !empty($def['label']) ? $def['label'] : $name,
      '#default_value' => $field['value'],
      // TODO: we need to enforce required with select boxes
      '#required' => !empty($def['required']) ? TRUE : FALSE,
      '#attributes' => array(
        'class' => array('lingwo-fields-value'),
      ),
      '#description' => isset($def['description']) ? $def['description'] : NULL,
    );

    switch ($field['type']) {
      case 'class':
        $field_form['value'] += array(
          '#type' => 'select',
          '#options' => array(
            0 => t('False'),
            1 => t('True'),
          ),
        );
        break;
      case 'option':
        $options = array('' => '-none-');
        if (!empty($def['options'])) {
          $options = $options + _lingwo_name_value_list($def['options']);
        }

        $field_form['value'] += array(
          '#type' => 'select',
          '#options' => $options,
        );
        break;
      case 'form':
        $field_form['value'] += array(
          '#type' => 'textfield',
        );
        break;
    }

    $alt = isset($field['alt']) ? $field['alt'] : array();
    if ($extra_value == $name) {
      $alt[] = '';
    }
    // since only forms can have multiple values, these are always
    // textfields.
    if (!empty($alt)) {
      foreach ($alt as $value) {
        $field_form['alt'][] = array(
          '#type' => 'textfield',
          '#default_value' => $value,
          '#attributes' => array(
            'class' => array('lingwo-fields-value'),
          ),
        );
      }
    }

    if ($name == '_noname_') {
      $field_form['value']['#type'] = 'hidden';
      $field_form['alt'][0]['#title'] = t('Un-named forms');
    }

    if (empty($def)) {
      $field_form['remove'] = array(
        '#type' => 'checkbox',
        '#title' => t('Remove'),
        '#ajax' => $ajax + array('event' => 'click'),
      );
    }
    else {
      $field_form['automatic'] = array(
        '#type' => 'checkbox',
        '#title' => t('Automatic'),
        '#default_value' => $field['automatic'],
      );
    }

    $element['wrapper']['fields'][$name] = $field_form;
  }

  if (count($element['wrapper']['fields']) == 0) {
    $element['wrapper']['placeholder'] = array(
      '#type' => 'markup',
      '#value' => '<p>' . t('No fields for this <em>Language</em> and <em>Part of speech</em>') . '</p>',
    );
  }

  $_lingwo_fields = array(
    '#parents' => array('_lingwo_fields'),
    '#tree' => TRUE,
  );

  // for detecting a language/pos change
  $_lingwo_fields['old_pair'] = array(
    '#type' => 'hidden',
    '#value' => implode(':', array($language, $pos)),
  );

  // for adding multiple forms
  $_lingwo_fields['extra_value'] = array(
    '#type' => 'hidden',
  );

  // for adding non-standard forms
  $_lingwo_fields['add_new_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra Form') . ':',
    '#attributes' => array(
      'id' => 'edit--lingwo-fields-add-new-form',
    ),
  );
  $_lingwo_fields['add_new_form']['name'] = array(
    '#type' => 'textfield',
    '#default_value' => '_noname_',
    '#attributes' => array(
      'class' => array("lingwo-fields-name"),
    ),
  );
  $_lingwo_fields['add_new_form']['value'] = array(
    '#type' => 'textfield',
  );
  $_lingwo_fields['add_new_form']['add_new_form'] = array(
    '#type' => 'button',
    '#value' => t('Add new form'),
    '#ajax' => $ajax + array('event' => 'click'),
    '#limit_validation_errors' => array(),
  );

  $element['wrapper']['_lingwo_fields'] = $_lingwo_fields;

  // for reloading the current values
  $element['refresh'] = array(
    '#type' => 'button',
    '#value' => t('Refresh fields'),
    '#ajax' => $ajax + array('event' => 'click'),
    '#parents' => array('_lingwo_fields', 'refresh'),
    '#attributes' => array('class' => array('js-hide')),
    '#limit_validation_errors' => array(),
  );

  //
  // Add JS and CSS
  //

  $js_path = dirname(drupal_get_path('module', 'lingwo_fields')) . "/js/";
  $element['#attached']['js'] = array(
    drupal_get_path('module', 'lingwo_fields') . '/lingwo_fields.js',

    // Use our requirejs stubs so that requirejs itself isn't required
    $js_path . 'require-stubs.js',

    array('type' => 'setting', 'data' => array('lingwo_fields' => array('language' => $node->language))),
  );

  // Add all the things we know are dependencies
  // TODO: it would be sweet to do this automatically somehow (of course,
  // the build does it automatically, I'm going to try and avoid that for now)
  foreach (array('util/declare.js', 'util/extendPrototype.js', 'Entry.js', 'Language.js') as $js) {
    $element['#attached']['js'][] = $js_path . $js;
  }

  $languages = language_list('enabled');
  $languages = $languages[1];
  foreach ($languages as $language) {
    $language_js_path = 'public://lingwo/languages/' . $language->language . '.js';
    if (file_exists($language_js_path)) {
      $element['#attached']['js'][] = $language_js_path;
    }
  }

  $element['#attached']['css'][] = drupal_get_path('module', 'lingwo_fields') . '/lingwo_fields.css';

  return $element;
}

// AJAX callback
function _lingwo_fields_ajax_callback($form, &$form_state) {
  return $form['lingwo_fields']['wrapper'];
}

/*
 * Theming
 */

function template_preprocess_lingwo_fields_widget_form(&$vars) {
  $vars['fields'] = LingwoFields::mapItems($vars['element']);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_lingwo_fields_widget_form($variables) {
  return drupal_render_children($variables['element']);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_lingwo_fields_formatter_default($variables) {
  $node = $variables['node'];
  $entry = LingwoEntry::fromNode($node);

  if ($source_node = $entry->getTranslationSource()) {
    // display the fields as if this were the source node (with the source nodes
    // language) and not a member of this language
    $entry = LingwoEntry::fromNode($source_node);
    $node = $source_node;
  }

  // fill element with real FAPI elements that simple display the item
  $element = array();
  foreach ($entry->fields as $name => $field) {
    $def = isset($field['definition']) ? $field['definition'] : array();
    if ($name == '_noname_') {
      $label = t('Un-named forms');
    }
    else {
      $label = isset($def['label']) ? $def['label'] : $name;
    }

    $value = $field['value'];
    if (isset($field['alt'])) {
      if ($value != '') {
        $value = array($value);
      }
      else {
        $value = array();
      }
      $value = implode(', ', array_merge($value, $field['alt']));
    }

    $element[] = array(
      '#type' => 'item',
      '#title' => check_plain($label),
      '#markup' => check_plain($value),
      '#item' => $field,
    );
  }

  return theme(array(
    'lingwo_fields_view__' . $node->language . '__' . $node->pos,
    'lingwo_fields_view__' . $node->language,
    'lingwo_fields_view',
  ), array('element' => $element));
}

function template_preprocess_lingwo_fields_view(&$vars) {
  $vars['fields'] = LingwoFields::mapItems($vars['element']);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_lingwo_fields_view($variables) {
  return drupal_render_children($variables['element']);
}

