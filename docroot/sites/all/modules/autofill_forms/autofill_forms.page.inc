<?php
/**
 * @file
 * Page callback: Autofill forms configuration settings.
 */

/**
 * Check form field type.
 */
function autofill_forms_form_field_type_settings($field) {
  $input_types = array(
    'textarea' => 'text_textarea',
    'textarea_summay' => 'text_textarea_with_summary',
    'text' => 'text_textfield',
    'chekbox/radio' => 'options_buttons',
    'select' => 'options_select',
  );
  return array_search($field, $input_types);
}

/**
 * Default value generation.
 */
function autofill_forms_generate_default_field_value($field_type = NULL, $options = NULL, $word_length = 10) {
  $default_value = '';
  switch ($field_type) {

    // Text/textarea field default value.
    case AUTOFILL_FORMS_FIELD_TYPE_TEXT:
    case AUTOFILL_FORMS_FIELD_TYPE_TEXTAREA:
    case AUTOFILL_FORMS_FIELD_TYPE_TEXTAREA_SUMMAY:
      $default_value = autofill_forms_create_paragraph_text($word_length);
      break;

    // Checkbox, radio button and select option field default value.
    case AUTOFILL_FORMS_FIELD_TYPE_TEXT_CHECKBOX:
    case AUTOFILL_FORMS_FIELD_TYPE_TEXT_RADIO:
    case AUTOFILL_FORMS_FIELD_TYPE_TEXT_SELECT:
      $_option_keys = array_keys($options);
      if (in_array('_none', $_option_keys)) {

        // Unset none value.
        unset($_option_keys[0]);
      }
      $_option_value = array_rand($_option_keys, 1);

      // Return option first value.
      $default_value = $_option_keys[$_option_value];
      break;
  }
  return $default_value;
}

/**
 * Create lorem text generation paragraph.
 */
function autofill_forms_create_paragraph_text($word_count, $title = FALSE) {
  $dictionary = array('dolor', 'sit', 'amet', 'consectetur',
    'adipiscing', 'elit', 'curabitur', 'vel', 'hendrerit', 'libero', 'eleifend',
    'blandit', 'nunc', 'ornare', 'odio', 'ut', 'orci', 'gravida', 'imperdiet',
    'nullam', 'purus', 'lacinia', 'a', 'pretium', 'quis', 'congue', 'praesent',
    'sagittis', 'laoreet', 'auctor', 'mauris', 'non', 'velit', 'eros', 'dictum',
    'proin', 'accumsan', 'sapien', 'nec', 'massa', 'volutpat', 'venenatis', 'sed',
    'eu', 'molestie', 'lacus', 'quisque', 'porttitor', 'ligula', 'dui', 'mollis',
    'tempus', 'at', 'magna', 'vestibulum', 'turpis', 'ac', 'diam', 'tincidunt',
    'id', 'condimentum', 'enim', 'sodales', 'in', 'hac', 'habitasse', 'platea',
    'dictumst', 'aenean', 'neque', 'fusce', 'augue', 'leo', 'eget', 'semper', 'mattis',
    'tortor', 'scelerisque', 'nulla', 'interdum', 'tellus', 'malesuada', 'rhoncus',
    'porta', 'sem', 'aliquet', 'et', 'nam', 'suspendisse', 'potenti', 'vivamus',
    'luctus', 'fringilla', 'erat', 'donec', 'justo', 'vehicula', 'ultricies', 'varius',
    'ante', 'primis', 'faucibus', 'ultrices', 'posuere', 'cubilia', 'curae', 'etiam',
    'cursus', 'aliquam', 'quam', 'dapibus', 'nisl', 'feugiat', 'egestas', 'class',
    'aptent', 'taciti', 'sociosqu', 'ad', 'litora', 'torquent', 'per', 'conubia',
    'nostra', 'inceptos', 'himenaeos', 'phasellus', 'nibh', 'pulvinar', 'vitae',
    'urna', 'iaculis', 'lobortis', 'nisi', 'viverra', 'arcu', 'morbi', 'pellentesque',
    'metus', 'commodo', 'ut', 'facilisis', 'felis', 'tristique', 'ullamcorper',
    'placerat', 'aenean', 'convallis', 'sollicitudin', 'integer', 'rutrum', 'duis',
    'est', 'etiam', 'bibendum', 'donec', 'pharetra', 'vulputate', 'maecenas', 'mi',
    'fermentum', 'consequat', 'suscipit', 'aliquam', 'habitant', 'senectus', 'netus',
    'fames', 'quisque', 'euismod', 'curabitur', 'lectus', 'elementum', 'tempor',
    'risus', 'cras',
  );
  $dictionary_flipped = array_flip($dictionary);
  $greeking = '';

  if (!$title) {
    $words_remaining = $word_count;
    $i = 1;
    while ($words_remaining > 0) {
      $sentence_length = mt_rand(3, 10);
      $words = array_rand($dictionary_flipped, $sentence_length);
      if ($i == 1) {
        $sentence_length = $sentence_length + 2;
        $lorum_text = autofill_forms_get_lorem_words(TRUE);
        $words = array_merge($lorum_text, $words);
      }
      $sentence = implode(' ', $words);
      $greeking .= ucfirst($sentence) . '. ';
      $words_remaining -= $sentence_length;
      $i++;
    }
  }
  else {
    // Use slightly different method for titles.
    $word_count = $word_count - 2;
    $lorum_text = autofill_forms_get_lorem_words(TRUE);
    if ($word_count > 0) {
      $words = array_rand($dictionary_flipped, $word_count);
      $words = array_merge($lorum_text, $words);
      $words = is_array($words) ? implode(' ', $words) : $words;
    }
    else {
      $words = is_array($lorum_text) ? implode(' ', $lorum_text) : $lorum_text;
    }
    $greeking = ucwords($words);
  }

  // Work around possible php garbage collection bug. Without an unset(), this
  // function gets very expensive over many calls (php 5.2.11).
  unset($dictionary, $dictionary_flipped);
  return trim($greeking);
}

/**
 * Lorem words array generation.
 */
function autofill_forms_get_lorem_words($loremipsum) {
  $arr = array();
  if ($loremipsum) {
    $arr[0] = 'lorem';
    $arr[1] = 'ipsum';
  }
  return $arr;
}
