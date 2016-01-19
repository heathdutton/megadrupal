<?php

class views_handler_field_regex_rewrite extends views_handler_field_custom {
  function option_definition() {
    $options = parent::option_definition();

    $options['alter']['contains']['text_patterns'] = array('default' => '', 'translatable' => FALSE);
    $options['alter']['contains']['text_replacements'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  /**
   * Validate if every pattern have a matching replacement.
   */
  function options_validate(&$form, &$form_state) {

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

    $form['alter']['text_patterns'] = array(
      '#title' => t('Patterns'),
      '#type' => 'textarea',
      '#default_value' => $this->options['alter']['text_patterns'],
      '#description' => t('Regular expression patterns (see <a href="http://www.php.net/manual/en/pcre.pattern.php">PHP manual</a>) for matching parts of this field to be replaced by the "Replacements" below. Specify each pattern on a new line.'),
      '#dependency' => array(
        'edit-options-alter-alter-text' => array(1),
      ),
    );

    $form['alter']['text_replacements'] = array(
      '#title' => t('Replacements'),
      '#type' => 'textarea',
      '#default_value' => $this->options['alter']['text_replacements'],
      '#description' => t('Text replacements corresponding line by line to the "Patterns" above. You may include matches from parenthesized patterns (see <a href="http://www.php.net/manual/en/function.preg-replace.php">PHP manual</a>). You may include HTML. You may enter data from this view as per the "Replacement patterns" below.'),
      '#dependency' => array(
        'edit-options-alter-alter-text' => array(1),
      ),
    );

    $form['#pre_render'][] = 'views_handler_field_regex_rewrite_pre_render_move_text';

  }

  /**
   * Render pattern replace before the token replace.
   */
  function render_text($alter) {

    $value = parent::render_text($alter);

    if ($this->options['hide_empty'] && empty($value) && ($value !== 0 || $this->options['empty_zero'])) {
      return '';
    }

    if (!empty($alter['alter_text']) && $alter['text_patterns'] !== '' && $alter['text_replacements'] !== '') {
      $patterns = preg_split('/(\r\n?|\n)/', $alter['text_patterns']);
      $replacements = preg_split('/(\r\n?|\n)/', $alter['text_replacements']);
      // Set pattern replaced value to be token replaced.
      $alter['text'] = preg_replace($patterns, $replacements, $value);
    }

    return $alter['text'];
  }
}


function views_handler_field_regex_rewrite_pre_render_move_text($form) {

  $form['text_patterns'] = $form['alter']['text_patterns'];
  $form['text_replacements'] = $form['alter']['text_replacements'];

  unset($form['alter']['text_patterns']);
  unset($form['alter']['text_replacements']);

  return $form;
}
