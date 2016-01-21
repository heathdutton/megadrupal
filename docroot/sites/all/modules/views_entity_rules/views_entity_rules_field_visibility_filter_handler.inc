<?php

/**
 * @file
 * Definition of views_entity_rules_field_visibility_filter_handler.
 */

/**
 * A handler to define field visibility using entity rules.
 *
 * @ingroup views_filter_handlers
 */

class views_entity_rules_field_visibility_filter_handler extends views_handler_filter {

  var $entity_rule = NULL;

  function can_expose() {
    return FALSE;
  }

  function can_group() {
    return TRUE;
  }

  /**
   * Overrides views_handler_filter#option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['visible_field'] = array('default' => '');
    $options['visibility_rule'] = array('default' => '');
    $options['rule_argument_relationship'] = array('default' => '');

    return $options;
  }

  /**
   * Provides a list of available fields.
   */
  function field_options() {
    $options = array();

    $field_handlers = $this->view->display_handler->get_handlers('field');
    foreach ($field_handlers as $field => $handler) {
      $options[$field] = $handler->ui_name();
    }

    return $options;
  }

  /**
   * Provides a list of available rules.
   */
  function rule_options() {
    $options = array();
    $options[''] = t('None');

    $rules = entity_load('rules_config', FALSE, array('active' => TRUE));
    ksort($rules);
    foreach ($rules as $rule) {
      if (in_array('entity_rules_views_field_visibility', $rule->tags)) {
        $vars = $rule->componentVariables();
        if ((isset($vars['entity'])) && (isset($vars['show_field']))) {
          $options[$rule->name] = $rule->label;
        }
      }
    }

    return $options;
  }

  /**
   * Provides a list of available relationships.
   */
  function relationship_options() {
    $options = array('' => t('Do not use a relationship'));
    $relationships = $this->view->display_handler->get_option('relationships');

    foreach ($relationships as $relationship) {
      $relationship_handler = views_get_handler($relationship['table'], $relationship['field'], 'relationship');
      // ignore invalid/broken relationships.
      if (empty($relationship_handler)) {
        continue;
      }
      $options[$relationship['id']] = $relationship_handler->label();
    }

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['visible_field'] = array(
      '#type' => 'select',
      '#title' => t('Field'),
      '#description' => t('Field of this view to show or hide.'),
      '#options' => $this->field_options(),
      '#default_value' => $this->options['visible_field'],
    );

    $form['visibility_rule'] = array(
      '#type' => 'select',
      '#title' => t('Visibility rule'),
      '#description' => t('Entity rule of type "Views field visibility".'),
      '#options' => $this->rule_options(),
      '#default_value' => $this->options['visibility_rule'],
    );

    $form['rule_argument_relationship'] = array(
      '#type' => 'select',
      '#title' => t('Relationship'),
      '#description' => t('Relationship which defines entity to pass as rule argument.'),
      '#options' => $this->relationship_options(),
      '#default_value' => $this->options['rule_argument_relationship'],
    );

  }

  function query() {
    // Add base table field of the relationship to the query for entity loading in post_execute().
    $relationship = $this->get_real_relationship();
    if (empty($relationship)) {
      return;
    }
    if (!method_exists($this->query, 'add_field')) {
      return;
    }
    if (empty($relationship->definition['base field'])) {
      return;
    }

    $table = $relationship->alias;
    $field = $relationship->definition['base field'];

    if (!isset($this->query->field_aliases[$table][$field])) {
      $this->query->add_field($table, $field);
    }
  }

  function admin_summary() {
    $rule_str = !empty($this->options['visibility_rule']) ? ' ' . t('if') . ' ' . $this->options['visibility_rule'] : '';
    return check_plain($this->options['visible_field'] . $rule_str);
  }

  function post_execute(&$result) {
    if (empty($this->options['visible_field']) || empty($this->options['visibility_rule'])) {
      return;
    }
    $field_handlers = $this->view->display_handler->get_handlers('field');
    if (!isset($field_handlers[$this->options['visible_field']])) {
      return;
    }
    $field = $field_handlers[$this->options['visible_field']];
    if (empty($field->field_alias)) {
      return;
    }
    $alias = $field->field_alias;
    $relationship = $this->get_real_relationship();
    $broken_relationship = FALSE;
    if (!empty($this->options['rule_argument_relationship'])) {
      if (empty($relationship) || empty($relationship->definition['base field'])) {
        $broken_relationship = TRUE;
      }
      else {
        if (!isset($this->query->field_aliases[$relationship->alias][$relationship->definition['base field']])) {
          $broken_relationship = TRUE;
        }
      }
    }

    $type_entities = !$broken_relationship ? ($this->query->get_result_entities($result, !empty($relationship) ? $relationship->alias : NULL)) : FALSE;
    $entity_type = !empty($type_entities) ? $type_entities[0] : '';
    $entities = !empty($type_entities) ? $type_entities[1] : array();
    $entity_id_property = '';
    if (!empty($entity_type)) {
      $info = entity_get_info($entity_type);
      if (!empty($info['entity keys']['id'])) {
        $entity_id_property = $info['entity keys']['id'];
      }
      if (empty($entity_id_property)) {
        $entities = array();
      }
    }
    $entity_visibility = array();
    foreach ($result as $key => &$row) {
      $visibility = FALSE;
      if (!empty($entities[$key])) {
        $entity = $entities[$key];
        if (!empty($entity->$entity_id_property)) {
          $entity_id = $entity->$entity_id_property;
          if (!isset($entity_visibility[$entity_id])) {
            $visibility = $this->invoke_visibility_rule($entity_type, $entity);
            $entity_visibility[$entity_id] = $visibility;
          }
          else {
            $visibility = $entity_visibility[$entity_id];
          }
        }
      }
      if (!$visibility) {
        if (isset($row->$alias)) {
          $row->$alias = is_array($row->$alias) ? array() : NULL;
        }
      }
    }
  }

  function get_real_relationship() {
    if (!empty($this->options['rule_argument_relationship'])) {
      $relationship = $this->options['rule_argument_relationship'];
      if ((!empty($this->view->relationship[$relationship])) && (!empty($this->view->relationship[$relationship]->alias))) {
        return $this->view->relationship[$relationship];
      }
    }

    return NULL;
  }

  function invoke_visibility_rule($entity_type, $entity) {
    if (!isset($this->entity_rule)) {
      $this->entity_rule = !empty($this->options['visibility_rule']) ? rules_get_cache('comp_' . $this->options['visibility_rule']) : FALSE;
    }
    if ($this->entity_rule === FALSE) {
      return FALSE;
    }
    $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
    $args = array($entity_wrapper, TRUE);

    return _entity_rules_all_pass(array($this->entity_rule->executeByArgs($args)));
  }

}
