<?php

/**
 * @file
 * Definition of views_fast_field_handler.
 */

/**
 * A field that displays fieldapi fields without entity loading.
 *
 * @ingroup views_field_handlers
 */
class views_fast_field_handler extends views_handler_field_field {

  function query($use_groupby = FALSE) {
    $this->get_base_table();
    $this->ensure_my_table();
    parent::query($use_groupby);
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['bypass_field_formatting'] = array('default' => FALSE, 'bool' => TRUE);
    $options['custom_formatter_callback'] = array('default' => '');
    $options['group_rows']['default'] = FALSE;

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['bypass_field_formatting'] = array(
      '#type' => 'checkbox',
      '#title' => t('Bypass field formatting and access checking'),
      '#default_value' => $this->options['bypass_field_formatting'],
      '#description' => t('Enable to show field as is (skip field formatter processing and access checking). This is suitable for plain text fields.'),
      '#weight' => -80,
    );

    $custom_formatters = array();
    foreach (module_implements('views_fast_field_formatter') as $module) {
      $function = $module . '_views_fast_field_formatter';
      $formatters = $function($this->field_info['type'], $this->definition['field_name']);
      if (!empty($formatters)) {
        foreach ($formatters as $formatter) {
          $custom_formatters[$formatter['callback']] = $formatter['title'];
        }
      }
    }
    if (!empty($custom_formatters)) {
      $form['custom_formatter_callback'] = array(
        '#type' => 'select',
        '#title' => t('Custom formatter'),
        '#options' => array('' => t('None')) + $custom_formatters,
        '#default_value' => $this->options['custom_formatter_callback'],
        '#weight' => -79,
      );
    }
  }

  function multiple_options_form(&$form, &$form_state) {
    // Do nothing.
  }

  /**
   * Load the entities for all fields that are about to be displayed.
   */
  function post_execute(&$values) {
    static $entities = array();

    if (!empty($values)) {
      if (!empty($this->options['bypass_field_formatting'])) {
        foreach ($values as $row_id => &$value) {
          $value->{'field_' . $this->options['id']} = $this->set_items_fast($value);
        }
        return;
      }

      $entities_by_type = array();
      foreach ($values as $key => $object) {
        if (isset($this->aliases['entity_type']) && isset($object->{$this->aliases['entity_type']}) && isset($object->{$this->field_alias}) && !isset($values[$key]->_field_data[$this->field_alias])) {
          $entity_type = $object->{$this->aliases['entity_type']};
          if (empty($this->definition['is revision'])) {
            $entity_id = $object->{$this->field_alias};
          }
          else {
            $entity_id = $object->{$this->aliases['entity_id']};
          }
          $entities_by_type[$entity_type][$key] = $entity_id;
        }
      }

      foreach ($entities_by_type as $entity_type => $entity_ids) {
        $missing = FALSE;
        foreach ($entity_ids as $entity_id) {
          if (!isset($entities[$entity_type][$entity_id])) {
            $missing = TRUE;
            break;
          }
        }

        if ($missing) {
          $entity_info = entity_get_info($entity_type);
          $query = new EntityFieldQuery();
          $query->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');
          $query->entityCondition('entity_type', $entity_type);
          $query->propertyCondition($entity_info['entity keys']['id'], $entity_ids);
          $result = $query->execute();

          foreach ($entity_ids as $entity_id) {
            if (isset($result[$entity_type][$entity_id])) {
              $entities[$entity_type][$entity_id] = $result[$entity_type][$entity_id];
            }
            else {
              $entities[$entity_type][$entity_id] = entity_create_stub_entity($entity_type, array($entity_id, $entity_id, ' '));
            }
          }
        }

        foreach ($entity_ids as $key => $entity_id) {
          $values[$key]->_field_data[$this->field_alias] = array(
            'entity_type' => $entity_type,
            'entity' => $entities[$entity_type][$entity_id],
          );
        }
      }

      foreach ($values as $row_id => &$value) {
        $value->{'field_' . $this->options['id']} = $this->set_items($value, $row_id);
      }
    }
  }

  function get_value($values, $field = NULL) {
    // Go ahead and render and store in $this->items.
    $entity = clone $values->_field_data[$this->field_alias]['entity'];

    $entity_type = $values->_field_data[$this->field_alias]['entity_type'];
    $langcode = $this->field_language($entity_type, $entity);

    $base_value = array();
    foreach (array_keys($this->field_info['columns']) as $column_name) {
      $attr_name = _field_sql_storage_columnname($this->field_info['field_name'], $column_name);
      if (!isset($this->aliases[$attr_name])) {
        continue;
      }
      $alias = $this->aliases[$attr_name];
      if (!isset($values->{$alias})) {
        continue;
      }
      $base_value[$column_name] = $values->{$alias};
    }
    $entity->{$this->definition['field_name']}[$langcode] = !empty($base_value) ? array($base_value) : array();

    // If we are grouping, copy our group fields into the cloned entity.
    // It's possible this will cause some weirdness, but there's only
    // so much we can hope to do.
    if (!empty($this->group_fields)) {
      // first, test to see if we have a base value.
      $base_value = array();
      // Note: We would copy original values here, but it can cause problems.
      // For example, text fields store cached filtered values as
      // 'safe_value' which doesn't appear anywhere in the field definition
      // so we can't affect it. Other side effects could happen similarly.
      $data = FALSE;
      foreach ($this->group_fields as $field_name => $column) {
        if (property_exists($values, $this->aliases[$column])) {
          $base_value[$field_name] = $values->{$this->aliases[$column]};
          if (isset($base_value[$field_name])) {
            $data = TRUE;
          }
        }
      }

      // If any of our aggregated fields have data, fake it:
      if ($data) {
        // Now, overwrite the original value with our aggregated value.
        // This overwrites it so there is always just one entry.
        $entity->{$this->definition['field_name']}[$langcode] = array($base_value);
      }
      else {
        $entity->{$this->definition['field_name']}[$langcode] = array();
      }
    }

    // The field we are trying to display doesn't exist on this entity.
    if (!isset($entity->{$this->definition['field_name']})) {
      return array();
    }

    if ($field == 'entity') {
      return $entity;
    }
    else {
      return !empty($entity->{$this->definition['field_name']}[$langcode]) ? $entity->{$this->definition['field_name']}[$langcode] : array();
    }
  }

  /**
   * Return an array of items for the field.
   */
  function set_items($values, $row_id) {
    // In some cases the instance on the entity might be easy, see
    // https://drupal.org/node/1161708 and https://drupal.org/node/1461536 for
    // more information.
    if (empty($values->_field_data[$this->field_alias]) || empty($values->_field_data[$this->field_alias]['entity'])) {
      return array();
    }

    $display = array(
      'type' => $this->options['type'],
      'settings' => $this->options['settings'],
      'label' => 'hidden',
      // Pass the View object in the display so that fields can act on it.
      'views_view' => $this->view,
      'views_field' => $this,
      'views_row_id' => $row_id,
    );


    $entity_type = $values->_field_data[$this->field_alias]['entity_type'];
    $entity = $this->get_value($values, 'entity');
    if (!$entity) {
      return array();
    }

    $langcode = $this->field_language($entity_type, $entity);
    $render_array = field_view_field($entity_type, $entity, $this->definition['field_name'], $display, $langcode);

    $items = array();
    if ($this->options['field_api_classes']) {
      return array(array('rendered' => drupal_render($render_array)));
    }

    foreach (element_children($render_array) as $count) {
      $items[$count]['rendered'] = $render_array[$count];
      // field_view_field() adds an #access property to the render array that
      // determines whether or not the current user is allowed to view the
      // field in the context of the current entity. We need to respect this
      // parameter when we pull out the children of the field array for
      // rendering.
      if (isset($render_array['#access'])) {
        $items[$count]['rendered']['#access'] = $render_array['#access'];
      }
      // Only add the raw field items (for use in tokens) if the current user
      // has access to view the field content.
      if ((!isset($items[$count]['rendered']['#access']) || $items[$count]['rendered']['#access']) && !empty($render_array['#items'][$count])) {
        $items[$count]['raw'] = $render_array['#items'][$count];
      }
    }

    return $items;
  }

  /**
   * Return an array of items for the field without using field formatters.
   */
  function set_items_fast($values) {
    $items = array();

    $base_value = array();
    foreach (array_keys($this->field_info['columns']) as $column_name) {
      $attr_name = _field_sql_storage_columnname($this->field_info['field_name'], $column_name);
      if (!isset($this->aliases[$attr_name])) {
        continue;
      }
      $alias = $this->aliases[$attr_name];
      if (!isset($values->{$alias})) {
        continue;
      }
      $base_value[$column_name] = $values->{$alias};
    }

    if (empty($base_value)) {
      return $items;
    }

    $items[0] = array();
    $items[0]['raw'] = $base_value;

    $rendered = '';

    if (!empty($this->options['custom_formatter_callback'])) {
      $function = $this->options['custom_formatter_callback'];
      if (function_exists($function)) {
        $rendered = $function($base_value, $this->field_info['type'], $this->definition['field_name'], $this->options['type'], $this->options['settings']);
      }
    }
    elseif (isset($base_value['value'])) {
      if (empty($base_value['format']) || ($base_value['format'] === 'plain_text')) {
        $rendered = check_plain($base_value['value']);
      }
      else {
        $rendered = check_markup($base_value['value'], $base_value['format'], '', TRUE);
      }
    }

    $items[0]['rendered'] = $rendered;

    return $items;
  }

  function field_language($entity_type, $entity) {
    return LANGUAGE_NONE;
  }

  function ui_name($short = FALSE) {
    return parent::ui_name($short) . ' (fast)';
  }
}
