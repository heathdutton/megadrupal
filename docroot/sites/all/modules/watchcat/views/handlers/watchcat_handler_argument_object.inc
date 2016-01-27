<?php

class views_handler_argument_object extends views_handler_argument_numeric {

  function option_definition() {
    $options = parent::option_definition();

    $options['entity_type'] = array(
      'default' => NULL,
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $types = array();
    foreach (_watchcat_verb_info() as $module => $verbs) {
      foreach ($verbs as $verb => $info) {
        if (isset($info['object'])) {
          $entity_info = entity_get_info($info['object']);

          $types[$info['object']] = $entity_info['label'];
        }
      }
    }

    $form['entity_type'] = array(
      '#type' => 'select',
      '#title' => t("Entity type"),
      '#description' => t("Type of blaha blaha blaha..."),
      '#options' => $types,
      '#default_value' => $this->options['entity_type'],
    );
  }

  function query($group_by = FALSE) {
    parent::query($group_by);

    $placeholder = $this->placeholder();
    $valid_verbs = array();

    $type = $this->options['entity_type'];

    foreach (_watchcat_verb_info() as $module => $verbs) {
      foreach ($verbs as $verb => $info) {
        if (isset($info['object']) && $info['object'] === $type) {
          $valid_verbs[] = "{$module}:{$verb}";
        }
      }
    }

    if (empty($valid_verbs)) {
      $valid_verbs[] = 'no-valid-verb';
    }

    if (count($valid_verbs) > 1) {
      $this->query->add_where_expression(0, "$this->table_alias.verb IN($placeholder)", array($placeholder => $valid_verbs));
    }
    else {
      $this->query->add_where_expression(0, "$this->table_alias.verb = $placeholder", array($placeholder => $valid_verbs));
    }
  }

}
