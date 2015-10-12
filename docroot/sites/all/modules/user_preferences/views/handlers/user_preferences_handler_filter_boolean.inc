<?php

/**
 * @file
 * Contains user_preferences_handler_filter_boolean
 */

class user_preferences_handler_filter_boolean extends views_handler_filter_boolean_operator {
  var $treat_null_as_default = FALSE;
  var $preference_name = '';

  function construct() {
    $this->preference_name = $this->definition['preference_name'];
    parent::construct();
  }

  // Extra options stuff found in:
  // https://www.drupal.org/files/geolocation_proximity-sort_views_handler-1580394-12.patch
  function has_extra_options() {
    return TRUE;
  }

  function extra_options(&$option) {
    $option['treat_null_as_default'] = FALSE;
  }

  function extra_options_form(&$form, &$form_state) {
    $form['treat_null_as_default'] = array(
      '#type' => 'checkbox',
      '#title' => t('Treat NULL as selected value?'),
      '#description' => t("If the user hasn't set a value for this preference yet should we treat NULL as the value you've selected? For example checking this box with the value set at true would mean if a user hadn't set the preference to true or false it would assume true."),
      '#default_value' => isset($this->options['treat_null_as_default']) ? $this->options['treat_null_as_default'] : FALSE,
    );
  }

  function query() {
    $this->ensure_my_table();
    $preference_name = $this->preference_name;

    // Allow NULL or false values.
    $where = NULL;
    if (empty($this->value)) {
      // Should we treat no value as true?
      if (isset($this->options['treat_null_as_default']) && $this->options['treat_null_as_default']) {
        $where = db_and()
          ->condition(db_or()
            ->condition("$this->table_alias.name", $preference_name, '=')
            ->isNull("$this->table_alias.name")
          )
          ->condition(db_or()
            ->condition("$this->table_alias.value", '0', '=')
            ->isNull("$this->table_alias.value")
          );
      }
      else {
        $where = db_and()
          ->condition("$this->table_alias.name", $preference_name, '=')
          ->condition("$this->table_alias.value", '0', '=');
      }
    }
    // Require true.
    else {
      if (isset($this->options['treat_null_as_default']) && $this->options['treat_null_as_default']) {
        $where = db_and()
          ->condition(db_or()
            ->condition("$this->table_alias.name", $preference_name, '=')
            ->isNull("$this->table_alias.name")
          )
          ->condition(db_or()
            ->condition("$this->table_alias.value", '1', '=')
            ->isNull("$this->table_alias.value")
          );
      }
      else {
        $where = db_and()
          ->condition("$this->table_alias.name", $preference_name, '=')
          ->condition("$this->table_alias.value", '1', '=');
      }
    }

    $this->query->add_where($this->options['group'], $where);
  }
}
