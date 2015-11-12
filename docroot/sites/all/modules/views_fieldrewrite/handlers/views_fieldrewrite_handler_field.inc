<?php

/**
 * @file
 * Contains the 'fieldrewrite' field handler.
 */

/**
 * Field handler to rewrite a Views field using regular expressions.
 */
class views_fieldrewrite_handler_field extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $items['text_patterns'] = array('default' => '', 'translatable' => FALSE);
    $items['text_replacements'] = array('default' => '', 'translatable' => TRUE);

    // Replace 'text' form element with new items.
    $this->array_item_replace($options['alter']['contains'], 'text', $items);

    return $options;
  }

  /**
   * Replace an array item preserving its position.
   */
  function array_item_replace(&$array, $key, $items) {
    $new_array = array();
    foreach ($array as $array_key => $array_value) {
      if ($array_key == $key) {
        foreach ($items as $item_key => $item_value) {
          $new_array[$item_key] = $item_value;
        }
      }
      else {
        $new_array[$array_key] = $array_value;
      }
    }
    $array = $new_array;
  }

  /**
   * Validate if every pattern have a matching replacement.
   */
  function options_validate($form, &$form_state) {
    // Count patterns and replacements.
    $patterns_count = count(preg_split('/(\r\n?|\n)/', $form_state['values']['options']['alter']['text_patterns']));
    $replacements_count = count(preg_split('/(\r\n?|\n)/', $form_state['values']['options']['alter']['text_replacements']));

    if ($patterns_count != $replacements_count) {
      $error_field = ($patterns_count < $replacements_count) ? 'text_patterns' : 'text_replacements';
      form_set_error($error_field, t('Every pattern must have a matching replacement. There are !patterns patterns, and !replacements replacements.', array('!patterns' => $patterns_count, '!replacements' => $replacements_count)));
    }
  }

  /**
   * Provide field rewrite form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    if ($this->allow_advanced_render()) {
      $form['alter']['alter_text']['#description'] = t('If checked, you can alter the output of this field by specifying regular expression patterns and replacement strings, which may contain replacement tokens that can use any existing field output.');

      $items = array(
        'text_patterns' => array(
          '#title' => t('Patterns'),
          '#type' => 'textarea',
          '#default_value' => $this->options['alter']['text_patterns'],
          '#description' => t('Regular expression patterns (see <a href="http://www.php.net/manual/en/pcre.pattern.php">PHP manual</a>) for matching parts of this field to be replaced by the "Replacements" below. Specify each pattern on a new line.'),
          '#dependency' => array(
            'edit-options-alter-alter-text' => array(1),
          ),
        ),
        'text_replacements' => array(
          '#title' => t('Replacements'),
          '#type' => 'textarea',
          '#default_value' => $this->options['alter']['text_replacements'],
          '#description' => t('Text replacements corresponding line by line to the "Patterns" above. You may include matches from parenthesized patterns (see <a href="http://www.php.net/manual/en/function.preg-replace.php">PHP manual</a>). You may include HTML. You may enter data from this view as per the "Replacement patterns" below.'),
          '#dependency' => array(
            'edit-options-alter-alter-text' => array(1),
          ),
        ),
      );

      // Replace 'text' form element with new items.
      $this->array_item_replace($form['alter'], 'text', $items);
    }
  }

  /**
   * Render pattern replace before the token replace.
   */
  function render_text($alter) {
    $value = trim($this->last_render);
    if ($this->options['hide_empty'] && empty($value) && ($value !== 0 || $this->options['empty_zero'])) {
      return '';
    }

    if (!empty($alter['alter_text']) && $alter['text_patterns'] !== '' && $alter['text_replacements'] !== '') {
      $patterns = preg_split('/(\r\n?|\n)/', $alter['text_patterns']);
      $replacements = preg_split('/(\r\n?|\n)/', $alter['text_replacements']);
      // Set pattern replaced value to be token replaced.
      $alter['text'] = preg_replace($patterns, $replacements, $value);
    }

    return parent::render_text($alter);
  }
}
