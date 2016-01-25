<?php

/**
 * Implements hook_views_data().
 * This will override EntityAPI's Views handler. Use hook_views_data_alter() instead.
 */
//function computing_views_data() {
//  $views_data = array(
//    'computing_record' => array(
//      'input' => array(
//        'title' => 'Input',
//        'help' => t('Computing record "input" field, in JSON.'),
//        'field' => array(
//          'handler' => 'views_handler_field_computing_json',
//        ),
//      ),
//      'output' => array(
//        'title' => 'Output',
//        'help' => t('Computing record "output" field, in JSON.'),
//        'field' => array(
//          'handler' => 'views_handler_field_computing_json',
//        ),
//      ),
//    ),
//  );
//  return $views_data;
//}


/**
 * Implements hook_views_data_alter().
 */
function computing_views_data_alter(&$data) {
  // TODO: might use EntityAPI's "options list" for "status" property instead of directly alter views.
  // see entity.module > 'views' > entity.views.inc > EntityDefaultViewsController.
  if (isset($data['computing_record']['status'])) {
    $data['computing_record']['status']['field']['handler'] = 'views_handler_field_computing_status';
    $data['computing_record']['status']['filter']['handler'] = 'views_handler_filter_computing_status';
  }

  if (isset($data['computing_record']['uid'])) {
    $data['computing_record']['uid']['field']['handler'] = 'views_handler_field_user';
    $data['computing_record']['uid']['filter']['handler'] = 'views_handler_filter_user_name';
    $data['computing_record']['uid']['argument']['handler'] = 'views_handler_argument_user_uid';
  }

  $data['computing_record']['input'] = array(
    'title' => 'Input',
    'help' => t('Computing record "input" field, in JSON.'),
    'field' => array(
      'handler' => 'views_handler_field_computing_json',
    ),
  );

  $data['computing_record']['output'] = array(
    'title' => 'Output',
    'help' => t('Computing record "output" field, in JSON.'),
    'field' => array(
      'handler' => 'views_handler_field_computing_json',
    ),
  );
}


class views_handler_field_computing_status extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    return computing_status_readable($value) ." ({$value})";
  }
}


class views_handler_filter_computing_status extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Status');
      $this->value_options = computing_status_readable();
    }
  }
}


class views_handler_field_computing_json extends views_handler_field_serialized {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['format']['#type'] = 'radios';
    unset($form['format']['#description']);
    $form['format']['#options']['unserialized'] = t('Pretty JSON object');
    $form['format']['#options']['serialized'] = t('Raw JSON string (Full)');
    $form['format']['#options']['trimmed'] = t('Raw JSON string (Trimmed)');
    $form['format']['unserialized']['#description'] = t('Display JSON object in Krumo if installed, or show data in array.');
    $form['format']['serialized']['#description'] = t('Display the raw encoded JSON string in full length.');
    $form['format']['trimmed']['#description'] = t('Display the first 100 characters of the raw JSON encoded string.');
    //$form['format']['#attached']['css'] = array('.views-field .krumo-footnote { display: none; }', array('type' => 'inline'));

    // we won't allow 'key' options.
    unset($form['format']['#options']['key']);
  }

  function render($values) {
    $value = $values->{$this->field_alias};

    if ($this->options['format'] == 'unserialized') {
      if (function_exists('kprint_r')) {
        //drupal_add_css('.views-field .krumo-footnote { display: none; }', array('type' => 'inline'));
        return kprint_r((array) drupal_json_decode($value), TRUE);
      }
      elseif (function_exists('dprint_r')) {
        return dprint_r((array) drupal_json_decode($value), TRUE);
      }
      else {
        return check_plain(print_r((array) drupal_json_decode($value), TRUE));
      }
    }
//    elseif ($this->options['format'] == 'key' && !empty($this->options['key'])) {
//      $value = (array) drupal_json_decode($value);
//      return check_plain($value[$this->options['key']]);
//    }
    elseif ($this->options['format'] == 'trimmed') {
      return strlen($value) > 100 ? substr($value, 0, 100) .'...' : $value;
    }

    return $value;
  }
}