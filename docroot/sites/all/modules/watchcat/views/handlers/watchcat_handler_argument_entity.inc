<?php
/**
 * @file
 */


/**
 * watchcat_handler_argument_entity.
 */
class watchcat_handler_argument_entity extends views_handler_argument_numeric {

  function init(&$view, &$options) {
    parent::init($view, $options);

    $this->additional_fields['actor_id'] = array('table' => 'watchcat', 'field' => 'actor_id');
    $this->additional_fields['object_id'] = array('table' => 'watchcat', 'field' => 'object_id');
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['entity_type'] = array(
      'default' => NULL,
    );
    $options['watchcat_type'] = array(
      'default' => array(),
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $types = array();
    foreach (_watchcat_verb_info() as $module => $verbs) {
      foreach ($verbs as $verb => $info) {
        foreach (array('actor', 'object', 'target') as $type) {
          $entity_info = entity_get_info($info[$type]);
          $types[$info[$type]] = $entity_info['label'];
        }
      }
    }

    $form['entity_type'] = array(
      '#type' => 'select',
      '#title' => t('Entity type'),
      '#description' => t('Select entity type.'),
      '#options' => $types,
      '#default_value' => $this->options['entity_type'],
    );

    $form['watchcat_type'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Watchcat type'),
      '#description' => t('Select watchcat type.'),
      '#options' => array(
        'actor' => t('Actor'),
        'object' => t('Object'),
        'target' => t('Target'),
      ),
      '#default_value' => $this->options['watchcat_type'],
    );
  }

  function query($group_by = FALSE) {
    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    $this->ensure_my_table();

    $or = db_or();
    $and = NULL;

    $types = array_filter($this->options['watchcat_type']);
    foreach ($types as $type) {
      $and = db_and();

      $placeholder = $this->placeholder();
      //$null_check = empty($this->options['not']) ? '' : "OR $this->table_alias.$type IS NULL";

      if (count($this->value) > 1) {
        $operator = empty($this->options['not']) ? 'IN' : 'NOT IN';
        $and->condition("$this->table_alias.{$type}_id", $this->value, $operator);
      }
      else {
        $operator = empty($this->options['not']) ? '=' : '!=';
        $and->condition("$this->table_alias.{$type}_id", $this->argument, $operator);
      }

      $placeholder = $this->placeholder();

      $valid_verbs = array();
      foreach (_watchcat_verb_info() as $module => $verbs) {
        foreach ($verbs as $verb => $info) {
          if (isset($info[$type]) && $info[$type] === $this->options['entity_type']) {
            $valid_verbs[] = "{$module}:{$verb}";
          }
        }
      }

      if (empty($valid_verbs)) {
        $valid_verbs[] = 'no-valid-verb';
      }

      if (count($valid_verbs) > 1) {
        $and->condition("$this->table_alias.verb", $valid_verbs, 'IN');
      }
      else {
        $and->condition("$this->table_alias.verb", $valid_verbs, '=');
      }

      $or->condition($and);
    }

    if (count($types) > 1) {
      $this->query->add_where(0, $or);
    }
    elseif (count($types) == 1) {
      $this->query->add_where(0, $and);
    }
  }

}
