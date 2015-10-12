<?php

/**
 * @file
 * Definition of views_handler_field_numeric_extended.
 */

/**
 * Render a field as a numeric value with extended formatting options.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_numeric_extended extends views_handler_field_numeric {
  /**
   * Implements parent::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['percentage'] = array('default' => FALSE, 'bool' => TRUE);
    $options['abbr_under_million'] = array('default' => FALSE, 'bool' => TRUE);
    $options['abbr_under_million_suffix'] = array('default' => '', 'translatable' => TRUE);
    $options['abbr_million'] = array('default' => FALSE, 'bool' => TRUE);
    $options['abbr_million_suffix'] = array('default' => '', 'translatable' => TRUE);
    $options['abbr_under_billion'] = array('default' => FALSE, 'bool' => TRUE);
    $options['abbr_under_billion_suffix'] = array('default' => '', 'translatable' => TRUE);
    $options['abbr_billion'] = array('default' => FALSE, 'bool' => TRUE);
    $options['abbr_billion_suffix'] = array('default' => '', 'translatable' => TRUE);
    $options['abbr_under_trillion'] = array('default' => FALSE, 'bool' => TRUE);
    $options['abbr_under_trillion_suffix'] = array('default' => '', 'translatable' => TRUE);
    $options['abbr_trillion'] = array('default' => FALSE, 'bool' => TRUE);
    $options['abbr_trillion_suffix'] = array('default' => '', 'translatable' => TRUE);
    $options['add_green'] = array('default' => FALSE, 'bool' => TRUE);
    $options['add_red'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Implements parent::options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    // Normally, the original Views numeric field handler will only
    // show these three options if the field is a float (not an integer).
    // Since we are potentially dividing during abbreviation, we should
    // make these available because the number could potentially become
    // a float.
    if (empty($this->definition['float'])) {
      $form['set_precision'] = array(
        '#type' => 'checkbox',
        '#title' => t('Round'),
        '#description' => t('If checked, the number will be rounded.'),
        '#default_value' => $this->options['set_precision'],
      );
      $form['precision'] = array(
        '#type' => 'textfield',
        '#title' => t('Precision'),
        '#default_value' => $this->options['precision'],
        '#description' => t('Specify how many digits to print after the decimal point.'),
        '#dependency' => array('edit-options-set-precision' => array(TRUE)),
        '#size' => 2,
      );
      $form['decimal'] = array(
        '#type' => 'textfield',
        '#title' => t('Decimal point'),
        '#default_value' => $this->options['decimal'],
        '#description' => t('What single character to use as a decimal point.'),
        '#size' => 2,
      );
    }
    $form['abbreviations'] = array(
      '#type' => 'fieldset',
      '#title' => t('Numeric Extras'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    // Converting to a percentage only makes sense for floats.
    if (!empty($this->definition['float'])) {
      $form['percentage'] = array(
        '#type' => 'checkbox',
        '#title' => t('Convert to a percentage/rate (multiply by 100).'),
        '#default_value' => $this->options['percentage'],
        '#fieldset' => 'abbreviations',
      );
    }
    $form['abbr_under_million'] = array(
      '#type' => 'checkbox',
      '#title' => t('If less than 1 million, divide by 1 million. The same will happen if the number is more than negative 1 million.'),
      '#default_value' => $this->options['abbr_under_million'],
      '#fieldset' => 'abbreviations',
    );
    $form['abbr_under_million_suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Under a million suffix'),
      '#default_value' => $this->options['abbr_under_million_suffix'],
      '#size' => 5,
      '#description' => t('This will appear after the number, if it is divided by 1 million. (ie, "M").'),
      '#fieldset' => 'abbreviations',
      '#dependency' => array(
        'edit-options-abbr-under-million' => array(1),
      ),
    );
    $form['abbr_million'] = array(
      '#type' => 'checkbox',
      '#title' => t('If greater than or equal to 1 million, divide by 1 million. The same will happen if the number is less than or equal to negative 1 million.'),
      '#default_value' => $this->options['abbr_million'],
      '#fieldset' => 'abbreviations',
    );
    $form['abbr_million_suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Million suffix'),
      '#default_value' => $this->options['abbr_million_suffix'],
      '#size' => 5,
      '#description' => t('This will appear after the number, if it is divided by 1 million. (ie, "M").'),
      '#fieldset' => 'abbreviations',
      '#dependency' => array(
        'edit-options-abbr-million' => array(1),
      ),
    );
    $form['abbr_under_billion'] = array(
      '#type' => 'checkbox',
      '#title' => t('If less than 1 billion, divide by 1 billion. The same will happen if the number is more than negative 1 billion.'),
      '#default_value' => $this->options['abbr_under_billion'],
      '#fieldset' => 'abbreviations',
    );
    $form['abbr_under_billion_suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Under a billion suffix'),
      '#default_value' => $this->options['abbr_under_billion_suffix'],
      '#size' => 5,
      '#description' => t('This will appear after the number, if it is divided by 1 billion. (ie, "M").'),
      '#fieldset' => 'abbreviations',
      '#dependency' => array(
        'edit-options-abbr-under-billion' => array(1),
      ),
    );
    $form['abbr_billion'] = array(
      '#type' => 'checkbox',
      '#title' => t('If greater than or equal to 1 billion, divide by 1 billion. The same will happen if the number is less than or equal to negative 1 billion.'),
      '#default_value' => $this->options['abbr_billion'],
      '#fieldset' => 'abbreviations',
    );
    $form['abbr_billion_suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Billion suffix'),
      '#default_value' => $this->options['abbr_billion_suffix'],
      '#size' => 5,
      '#description' => t('This will appear after the number, if it is divided by 1 billion. (ie, "B").'),
      '#fieldset' => 'abbreviations',
      '#dependency' => array(
        'edit-options-abbr-billion' => array(1),
      ),
    );
    $form['abbr_under_trillion'] = array(
      '#type' => 'checkbox',
      '#title' => t('If less than 1 trillion, divide by 1 trillion. The same will happen if the number is more than negative 1 trillion.'),
      '#default_value' => $this->options['abbr_under_trillion'],
      '#fieldset' => 'abbreviations',
    );
    $form['abbr_under_trillion_suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Under a trillion suffix'),
      '#default_value' => $this->options['abbr_under_trillion_suffix'],
      '#size' => 5,
      '#description' => t('This will appear after the number, if it is divided by 1 trillion. (ie, "M").'),
      '#fieldset' => 'abbreviations',
      '#dependency' => array(
        'edit-options-abbr-under-trillion' => array(1),
      ),
    );
    $form['abbr_trillion'] = array(
      '#type' => 'checkbox',
      '#title' => t('If greater than or equal to 1 trillion, divide by 1 trillion. The same will happen if the number is less than or equal to negative 1 trillion.'),
      '#default_value' => $this->options['abbr_trillion'],
      '#fieldset' => 'abbreviations',
    );
    $form['abbr_trillion_suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Trillion suffix'),
      '#default_value' => $this->options['abbr_trillion_suffix'],
      '#size' => 5,
      '#description' => t('This will appear after the number, if it is divided by 1 trillion. (ie, "M").'),
      '#fieldset' => 'abbreviations',
      '#dependency' => array(
        'edit-options-abbr-trillion' => array(1),
      ),
    );
    $form['add_green'] = array(
      '#type' => 'checkbox',
      '#title' => t('Green if positive'),
      '#default_value' => $this->options['add_green'],
      '#fieldset' => 'style_settings'
    );
    $form['add_red'] = array(
      '#type' => 'checkbox',
      '#title' => t('Red if negative'),
      '#default_value' => $this->options['add_red'],
      '#fieldset' => 'style_settings'
    );
  }

  /**
   * Implements parent::render().
   */
  function render($values) {
    $abbr_suffix = NULL;
    $color_wrapper = NULL;
    $value = $this->get_value($values);
    
    // Handle abbreviations
    $abbr_map = array(
      'gte' => array(
        'trillion' => 1000000000000,
        'billion' => 1000000000,
        'million' => 1000000,
      ),
      'lt' => array(
        'under_million' => 1000000,
        'under_billion' => 1000000000,
        'under_trillion' => 1000000000000,
      ),
    );
    foreach ($abbr_map as $operator => $levels) {
      foreach ($levels as $level => $level_number) {
        // See if we should abbreviate for this level
        if ($this->options["abbr_{$level}"]) {
          // Track whether or not the value was a match for this level
          $match = FALSE;

          // Determining if this number is a match depends on the operator
          switch ($operator) {
            // Greater than or equal
            case 'gte':
              $match = ($value > 0) ? ($value >= $level_number) : ($value <= (-1 * $level_number));
              break;
              
            // Less than
            case 'lt':
              $match = ($value > 0) ? ($value < $level_number) : ($value > (-1 * $level_number));
              break;
          }
          
          // See if we have a match
          if ($match) {
            // Divide by the level number
            $value = $value / $level_number;
            // See if we need a suffix for this level
            $abbr_suffix = $this->options["abbr_{$level}_suffix"];
            // Stop here
            break 2;
          }
        }
      }
    }
    
    // See if this number should be converted to a percentage.
    if ($this->options['percentage']) {
      $value *= 100;
    }

    // See if this number should be green if positive
    if ($this->options['add_green'] && ($value > 0)) {
      $color_wrapper = '<span class="views-numeric-green">';
    }
    
    // See if this number should be red if negative
    if ($this->options['add_red'] && ($value < 0)) {
      $color_wrapper = '<span class="views-numeric-red">';
    }
    
    if (!empty($this->options['set_precision'])) {
      $value = number_format($value, $this->options['precision'], $this->options['decimal'], $this->options['separator']);
    }
    else {
      $point_position = strpos($value, '.');
      $remainder = ($point_position === FALSE) ? '' : substr($value, $point_position + 1);
      $value = $value > 0 ? floor($value) : ceil($value);
      $value = number_format($value, 0, '', $this->options['separator']);
      if ($remainder) {
        // The substr may not be locale safe.
        $value .= $this->options['decimal'] . $remainder;
      }
    }

    // Check to see if hiding should happen before adding prefix and suffix.
    if ($this->options['hide_empty'] && empty($value) && ($value !== 0 || $this->options['empty_zero'])) {
      return '';
    }

    // Should we format as a plural.
    if (!empty($this->options['format_plural'])) {
      $value = format_plural($value, $this->options['format_plural_singular'], $this->options['format_plural_plural']);
    }

    return $color_wrapper 
      . $this->sanitize_value($this->options['prefix'], 'xss')
      . $this->sanitize_value($value)
      . $this->sanitize_value($abbr_suffix, 'xss')
      . $this->sanitize_value($this->options['suffix'], 'xss')
      . ($color_wrapper ? '</span>' : '');
  }
}
