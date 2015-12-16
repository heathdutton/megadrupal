<?php
/**
 * @file
 * Commerce Credits Transaction field handler entity id
 */


/**
 * Display entity label for entity based on the entity type.
 */
class commerce_credits_transaction_handler_field_entity_id extends views_handler_field {

  /**
   * Option definitions.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['render_as'] = array('default' => 'label', 'translatable' => FALSE);

    return $options;
  }

  /**
   * Options form
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['render_as'] = array(
      '#type' => 'radios',
      '#title' => t('Render As'),
      '#description' => t('Choose how to render the entity id'),
      '#options' => array(
        'label' => t('Entity label'),
        'id' => t('Entity id'),
      ),
      '#default_value' => $this->options['render_as'],
    );
  }

  /**
   * Add the entity type column for respective from/to field.
   */
  public function query() {
    $this->ensure_my_table();

    if ($this->options['render_as'] == 'label') {
      $type_column = $this->real_field == 'from_entity_id' ? 'from_entity_type' : 'to_entity_type';

      $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
      $this->entity_type_alias = $this->query->add_field($this->table_alias, $type_column, NULL, $params);
    }

    parent::query();
  }

  /**
   * Load entities, blech.
   */
  public function pre_render(&$values) {
    $ids = array();

    if ($this->options['render_as'] == 'label') {
      foreach ($values as $key => $value) {
        if (empty($value->{$this->field_alias})) {
          continue;
        }

        if (!isset($ids[$value->{$this->entity_type_alias}])) {
          $ids[$value->{$this->entity_type_alias}] = array();
        }
        $ids[$value->{$this->entity_type_alias}][] = $value->{$this->field_alias};
      }

      $types = array_keys($ids);

      foreach ($types as $entity_type) {
        $this->entities[$entity_type] = entity_load($entity_type, $ids[$entity_type]);
      }
    }
    else {
      parent::pre_render($values);
    }
  }

  public function get_value($values, $field = NULL) {
    if ($this->options['render_as'] == 'label') {
      $entity_type = $values->{$this->entity_type_alias};
      $entity_id = $values->{$this->field_alias};

      if (isset($this->entities[$entity_type][$entity_id])) {
        $entity = &$this->entities[$entity_type][$entity_id];

        return entity_label($entity_type, $entity);
      }

      return FALSE;
    }

    return parent::get_value($values, $field);
  }

}
