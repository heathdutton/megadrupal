<?php
class pane_entity_host extends pane_base {
  const BY_BUNDLE = 'bundle';
  const BY_VIEW = 'view';

  /**
   * Provides the data edit form
   *
   * @see pane_base::data_edit_form()
   *
   * @return array
   */
  public function data_edit_form($form = array(), &$form_state = array()) {
    $form = parent::data_edit_form($form, $form_state);

    $entity_type = $this->configuration['entity'];

    $form['references'] = array(
      '#type' => 'fieldset',
      '#title' => t('References'),
      '#theme' => 'pane_entity_host_reorder_form',
    );

    $ids = (TRUE === isset($this->data['ids'])) ? $this->data['ids'] : array();

    // Sort the ids by their weight
    usort($ids, 'drupal_sort_weight');

    // If we have a defined multiplicity

    if (0 < $this->configuration['multiplicity']) {
      for ($i = 0; $i < $this->configuration['multiplicity']; $i++) {
        $entity = NULL;
        if (TRUE === isset($ids[$i])) {
          $entities = $this->find_entities_by_id($ids[$i]['reference']);
          if (0 < sizeof($entities)) {
            $entity = array_pop($entities);
            list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
          }
        }

        $form['references'][$i]['reference'] = array(
          '#type' => 'textfield',
          '#title' => t('Item ' . $i),
          '#title_display' => 'invisible',
          '#autocomplete_path' => 'entityreference-autocomplete/' . $this->container,
          '#attached' => array(
            'js' => array('misc/autocomplete.js'),
          ),
          '#multiple' => TRUE,
          '#default_value' => (NULL !== $entity) ? entity_label($entity_type, $entity) . " [id: $id]" : '',
        );

        $form['references'][$i]['weight'] = array(
          '#type' => 'weight',
          '#delta' => $this->configuration['multiplicity'],
          '#default_value' => (NULL !== $entity) ? $ids[$i]['weight'] : $i,
          '#multiple' => TRUE,
        );
      }
    }
    // Unlimited multiplicity
    else {
      $blanks = (TRUE === isset($form_state['blanks'])) ? $form_state['blanks'] : 1;
      $form_state['blanks'] = $blanks;

      for ($i = 0; $i < (sizeof($ids) + $blanks); $i++) {
        $entity = NULL;

        if (TRUE === isset($ids[$i])) {
          $entities = $this->find_entities_by_id($ids[$i]['reference']);
          if (0 < sizeof($entities)) {
            $entity = array_pop($entities);
            list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
          }
        }

        $form['references'][$i]['reference'] = array(
          '#type' => 'textfield',
          '#title' => t('Item ' . $i),
          '#title_display' => 'invisible',
          '#autocomplete_path' => 'entityreference-autocomplete/' . $this->container,
          '#attached' => array(
            'js' => array('misc/autocomplete.js'),
          ),
          '#multiple' => TRUE,
          '#default_value' => (NULL !== $entity) ? entity_label($entity_type, $entity) . " [id: $id]" : '',
        );

        $form['references'][$i]['weight'] = array(
          '#type' => 'weight',
          '#delta' => 99,
          '#default_value' => (NULL !== $entity) ? $ids[$i]['weight'] : $i,
          '#multiple' => TRUE,
        );
      }

      $form['add_reference'] = array(
        '#type' => 'submit',
        '#value' => t('Add another item'),
        '#submit' => array('pane_entity_host_add_submit'),
      );
    }

    return $form;
  }

  /**
   * Validates the form data
   *
   * @see pane_base::data_edit_form_validate()
   */
  public function data_edit_form_validate(&$form, &$form_state) {
    if (TRUE == $form_state['process_input']) {
      $entity_type = $this->configuration['entity'];
      $entity_info = entity_get_info($entity_type);
      $id_field = $entity_info['entity keys']['id'];

      $values = array();

      foreach ($form_state['values']['references'] as $reference) {
        $value = $reference['reference'];

        $match = preg_match('/\[id: (\d+)\]/', $value, $preg_matches);
        if (!$match) {
          $match = preg_match('/^id: (\d+)/', $value, $preg_matches);
        }

        if ($match) {
          $value = $preg_matches[1];
        }

        $entities = array();

        if (TRUE === is_numeric($value)) {
          $entities = $this->find_entities_by_id($value);
        }
        else {
          $entities = $this->find_entities_by_title($value, TRUE);
        }

        if (0 < sizeof($entities)) {
          $entity = array_pop($entities);
          $values[] = array('weight' => $reference['weight'], 'reference' => $entity->{$id_field});
        }
      }

      usort($values, 'drupal_sort_weight');

      $form_state['values']['entity_ids'] = $values;
    }
  }

  /**
   * Saves the form data
   *
   * @see pane_base::data_edit_form_submit()
   */
  public function data_edit_form_submit(&$form, &$form_state) {
    $this->data = (TRUE === isset($form_state['values']['entity_ids'])) ? array('ids' => $form_state['values']['entity_ids']) : array();
  }

  /**
   * Renders the pane text
   *
   * @see pane_base::render()
   *
   * @return string
   */
  public function render() {
    $render = array();

    if (TRUE === isset($this->data['ids'])) {
      $entity_ids = array();

      foreach ($this->data['ids'] as $entity_data) {
        $entity_ids[] = $entity_data['reference'];
      }

      // Get the appropriate entities
      $entities = $this->find_entities_by_id($entity_ids);

      $render = parent::render();
      // And render them.
      $render += entity_view($this->configuration['entity'], $entities, $this->configuration['view_mode']);
    }

    return $render;
  }

  /**
   * Provides the container edit form
   *
   * @see pane_base::container_edit_form()
   *
   * @return array
   */
  public function container_edit_form(&$form_state = array()) {
    $mode = (TRUE === isset($this->configuration['mode'])) ? $this->configuration['mode'] : self::BY_BUNDLE;
    $multiplicity = (TRUE === isset($this->configuration['multiplicity'])) ? $this->configuration['multiplicity'] : 1;

    $values = range(1, 9);
    $multiplicity_values = array_combine($values, $values);
    $multiplicity_values = array(0 => t('Unlimited')) + $multiplicity_values;

    $form['multiplicity'] = array(
      '#type' => 'select',
      '#title' => t('Number of values'),
      '#options' => $multiplicity_values,
      '#default_value' => $multiplicity,
    );

    $form['mode'] = array(
      '#type' => 'select',
      '#title' => t('Mode'),
      '#default_value' => $mode,
      '#options' => array(
        self::BY_BUNDLE => t('Filter by bundle'),
        self::BY_VIEW => t('Filter by view')
      ),
    );

    switch ($mode) {
      case self::BY_BUNDLE:
        $form += $this->get_bundle_form($form_state);
        break;

      case self::BY_VIEW:
        $form += $this->get_view_form($form_state);
        break;
    }

    return $form;
  }

  /**
   * Provides the form for selecting by entity / bundle
   *
   * @param array $form_state
   *
   * @return array
   */
  protected function get_bundle_form(&$form_state) {
    $form = array();

    $entities = array();
    $bundles = array();
    $display_modes = array();

    foreach (entity_get_info() as $name => $info) {
      $entities[$name] = t($info['label']);

      foreach ($info['bundles'] as $bundle => $bundle_info) {
        $bundles[$name][$bundle] = t($bundle_info['label']);
      }

      foreach ($info['view modes'] as $machine_name => $mode_info) {
        $display_modes[$name][$machine_name] = $mode_info['label'];
      }

    }

    if (TRUE === isset($this->configuration['entity'])) {
      $entity = $this->configuration['entity'];
    }
    else {
      $entity_types = array_keys($entities);
      $entity = array_shift($entity_types);
    }

    $form['entity'] = array(
      '#type' => 'select',
      '#title' => t('Entity'),
      '#default_value' => $entity,
      '#options' => $entities,
      '#ajax' => array(
        'callback' => 'pane_plugin_change_ajax',
        'wrapper' => 'pane-config',
      ),
    );

    $form['target_bundles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Target bundles'),
      '#options' => $bundles[$entity],
      '#description' => t('The bundles of the entity type that can be referenced. Optional, leave empty for all bundles.'),
      '#default_value' => (TRUE === isset($this->configuration['target_bundles'])) ? $this->configuration['target_bundles'] : array(),
    );

    $form['view_mode'] = array(
      '#type' => 'select',
      '#title' => t('View Mode'),
      '#options' => $display_modes[$entity],
      '#description' => t('Display mode used to render selections'),
      '#default_value' => (TRUE === isset($this->configuration['view_mode'])) ? $this->configuration['view_mode'] : '',
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Provides the form for selecting by view
   *
   * @param array $form_state
   *
   * @return array
   */
  protected function get_view_form(&$form_state) {
    $form = array();

    return $form;
  }

  /**
   * Submit handler for container edit form
   *
   * @see pane_base::container_edit_form_submit()
   */
  public function container_edit_form_submit(&$form_state) {
    $configuration = array();

    if (TRUE === isset($form_state['values']['configuration']['mode'])) {
      $configuration['mode'] = $form_state['values']['configuration']['mode'];
    }

    if (TRUE === isset($form_state['values']['configuration']['entity'])) {
      $configuration['entity'] = $form_state['values']['configuration']['entity'];
    }

    if (TRUE === isset($form_state['values']['configuration']['target_bundles'])) {
      $configuration['target_bundles'] = array_filter($form_state['values']['configuration']['target_bundles']);
    }

    if (TRUE === isset($form_state['values']['configuration']['multiplicity'])) {
      $configuration['multiplicity'] = (int) $form_state['values']['configuration']['multiplicity'];
    }

    if (TRUE === isset($form_state['values']['configuration']['view_mode'])) {
      $configuration['view_mode'] = (string) $form_state['values']['configuration']['view_mode'];
    }

    if (FALSE === empty($configuration)) {
      $this->configuration = $configuration;
    }
  }

  /**
   * Builds the EntityFieldQuery to find an entity based on the current container
   * configuration
   *
   * @return EntityFieldQuery
   */
  protected function build_entity_query() {
    $entity_type = $this->configuration['entity'];
    $entity_info = entity_get_info($entity_type);
    $title_field = $entity_info['entity keys']['label'];

    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', $entity_type)
    ->range(0, 10);

    if (FALSE === empty($this->configuration['target_bundles'])) {
      $query->entityCondition('bundle', $this->configuration['target_bundles'], 'IN');
    }

    // Add a generic entity access tag to the query.
    $query->addTag($entity_type . '_access');
    $query->addTag('pane_entity_host');
    $query->addMetaData('container', $this->container);
    $query->addMetaData('pane_plugin', $this);

    return $query;
  }

  /**
   * Returns entities for the provided IDs and the current container configuration
   *
   * @param mixed array|integer $ids
   *
   * @return array
   */
  public function find_entities_by_id($ids) {
    $entity_type = $this->configuration['entity'];
    $entity_info = entity_get_info($entity_type);
    $id_field = $entity_info['entity keys']['id'];

    if (FALSE === is_array($ids)) {
      $ids = array($ids);
    }

    $entities = array();

    $query = $this->build_entity_query();

    $query->propertyCondition($id_field, $ids, 'IN');

    $results = $query->execute();

    if (!empty($results[$entity_type])) {
      $entities = entity_load($entity_type, array_keys($results[$entity_type]));
    }

    return $entities;
  }

  /**
   * Returns entities matching by title and the current container configuration
   *
   * @param string $string
   * @param boolean $exact
   *
   * @return array
   */
  public function find_entities_by_title($string, $exact = FALSE) {
    $entity_type = $this->configuration['entity'];
    $entity_info = entity_get_info($entity_type);
    $title_field = $entity_info['entity keys']['label'];

    $entities = array();

    $query = $this->build_entity_query();

    if (TRUE == $exact) {
      $query->propertyCondition($title_field, $string, '=');
    }
    else {
      $query->propertyCondition($title_field, '%' . $string . '%', 'LIKE');
    }

    $results = $query->execute();

    if (!empty($results[$entity_type])) {
      $entities = entity_load($entity_type, array_keys($results[$entity_type]));
    }

    return $entities;
  }
}
