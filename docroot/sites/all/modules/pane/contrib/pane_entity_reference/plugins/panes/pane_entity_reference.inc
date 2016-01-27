<?php
class pane_entity_reference extends pane_base {
  const BY_BUNDLE = 'bundle';
  const BY_VIEW = 'view';
  const OUTPUT_DISPLAY_MODE = 'display_mode';
  const OUTPUT_VIEW = 'view';

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
      '#theme' => 'pane_entity_reference_reorder_form',
    );

    $ids = (TRUE === isset($this->data['ids'])) ? $this->data['ids'] : array();

    // If we have entities from the form status use those instead of the data
    if (TRUE === isset($form_state['values']['entity_ids'])) {
      $ids = $form_state['values']['entity_ids'];
    }

    // Sort the ids by their weight
    usort($ids, 'drupal_sort_weight');

    $values = array();

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

        $value = (NULL !== $entity) ? entity_label($entity_type, $entity) . " [id: $id]" : '';
        $values[$i] = array('reference' => $value, 'weight' => (NULL !== $entity) ? $ids[$i]['weight'] : $i);

        $form['references'][$i]['reference'] = array(
          '#type' => 'textfield',
          '#title' => t('Item ' . $i),
          '#title_display' => 'invisible',
          '#autocomplete_path' => 'entityreference-autocomplete/' . $this->container,
          '#attached' => array(
            'js' => array('misc/autocomplete.js'),
          ),
          '#multiple' => TRUE,
          '#default_value' => $value,
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
      $blanks = (TRUE === isset($form_state['blanks'])) ? $form_state['blanks'] : sizeof($ids) + 1;
      $form_state['blanks'] = $blanks;

      for ($i = 0; $i < $blanks; $i++) {
        $entity = NULL;

        if (TRUE === isset($ids[$i])) {
          $entities = $this->find_entities_by_id($ids[$i]['reference']);
          if (0 < sizeof($entities)) {
            $entity = array_pop($entities);
            list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
          }
        }

        $value = (NULL !== $entity) ? entity_label($entity_type, $entity) . " [id: $id]" : '';
        $values[$i] = array('reference' => $value, 'weight' => (NULL !== $entity) ? $ids[$i]['weight'] : $i);

        $form['references'][$i]['reference'] = array(
          '#type' => 'textfield',
          '#title' => t('Item ' . $i),
          '#title_display' => 'invisible',
          '#autocomplete_path' => 'entityreference-autocomplete/' . $this->container,
          '#attached' => array(
            'js' => array('misc/autocomplete.js'),
          ),
          '#multiple' => TRUE,
          '#default_value' => $value,
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
        '#submit' => array('pane_entity_reference_add_submit'),
      );
    }

    $form_state['input']['references'] = $values;

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
    $this->data = isset($form_state['values']['entity_ids']) ? array('ids' => $form_state['values']['entity_ids']) : array();
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
    $entities = array();
    $output_mode = (TRUE === isset($this->configuration['output_mode'])) ? $this->configuration['output_mode'] : self::OUTPUT_DISPLAY_MODE;

    $ids = array();

    if (TRUE === isset($this->data['ids']) && FALSE === empty($this->data['ids'])) {
      $ids = $this->data['ids'];
      usort($ids, 'drupal_sort_weight');
    }


    // Build array of entities from user-entered data
    foreach ($ids as $entity_data) {
      $matching_entities = $this->find_entities_by_id($entity_data['reference']);
      if (0 < sizeof($matching_entities)) {
        $entity = array_pop($matching_entities);
        $entities[$entity_data['reference']] = $entity;
      }
    }

    // Pad out the number of entities, if appropriate
    if (!empty($this->configuration['view']['min_results'])) {
      while (count($entities) < $this->configuration['view']['min_results']){

        // ensure we have all eligible entities
        if (empty($eligible_entities)) {
          if ($results = $this->executeView()) {
            $entity_type = $this->configuration['entity'];
            $eligible_entities = entity_load($entity_type, array_keys($results));
          }
          else {
            break;
          }
        }

        // add the next non-duplicate eligible entity to the array
        if ($item = each($eligible_entities)) {
          if (empty($entities[$item['key']])) {
            $entities[$item['key']] = $item['value'];
          }
        }
        else {
          break;
        }
      }
    }

    // Render using the appropriate output mode.
    $render = parent::render();
    switch ($output_mode) {
      case self::OUTPUT_DISPLAY_MODE:
        $render += $this->render_display_mode($entities);
        break;

      case self::OUTPUT_VIEW:
        $render['#markup'] = $this->render_view($entities);
        break;
    }

    // Add more link, if appropriate
    if (count($entities) > 0 && !empty($this->configuration['more_link'])) {
      $url = parse_url($this->configuration['more_link']);

      // Support query params, get them ready for l()
      $query = array();
      if (!empty($url['query'])) {
        parse_str($url['query'], $query);
      }

      $more_link_text = $this->configuration['more_link_text'];
      if (empty($more_link_text)) $more_link_text = 'View more';
      $render['more_link'] = array(
        '#link_text' => $more_link_text,
        '#accessible_text' => (!empty($this->configuration['view']['args'])) ? $this->configuration['view']['args'] : $more_link_text,
        '#path' => $url['path'],
        '#query' => $query,
        '#fragment' => (!empty($url['fragment']) ? $url['fragment'] : ''),
        '#theme' => 'pane_entity_reference_more_link',
      );
    }

    return $render;
  }

  /**
   * Renders the output using the selected display mode
   *
   * @param array $entities
   *
   * @return string
   */
  protected function render_display_mode(&$entities) {
    return entity_view($this->configuration['entity'], $entities, $this->configuration['view_mode']);
  }

  /**
   * Renders the output using the selected view
   *
   * @param array $entities
   *
   * @return string
   */
  protected function render_view(&$entities) {
    $ids = implode('+', array_keys($entities));
    list($view, $display) = explode(':', $this->configuration['display_view']);

    return views_embed_view($view, $display, $ids);
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
    $more_link = (TRUE === isset($this->configuration['more_link'])) ? $this->configuration['more_link'] : '';
    $more_link_text = (TRUE === isset($this->configuration['more_link_text'])) ? $this->configuration['more_link_text'] : '';
    $output_mode = (TRUE === isset($this->configuration['output_mode'])) ? $this->configuration['output_mode'] : self::OUTPUT_DISPLAY_MODE;

    $values = range(1, 9);
    $multiplicity_values = array_combine($values, $values);
    $multiplicity_values = array(0 => t('Unlimited')) + $multiplicity_values;


    $entities = array();
    $display_modes = array();

    foreach (entity_get_info() as $name => $info) {
      $entities[$name] = t($info['label']);

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
      '#ajax' => array(
        'callback' => 'pane_plugin_change_ajax',
        'wrapper' => 'pane-config',
      ),
      '#options' => array(
        self::BY_BUNDLE => t('Filter by bundle'),
        self::BY_VIEW => t('Filter by view')
      ),
    );

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

    switch ($mode) {
      case self::BY_BUNDLE:
        $this->get_bundle_form($form, $form_state, $entity);
        break;

      case self::BY_VIEW:
        $this->get_view_form($form, $form_state);
        break;
    }

    $form['output_mode'] = array(
      '#type' => 'select',
      '#title' => t('Output mode'),
      '#default_value' => $output_mode,
      '#ajax' => array(
        'callback' => 'pane_plugin_change_ajax',
        'wrapper' => 'pane-config',
      ),
      '#options' => array(
        self::OUTPUT_DISPLAY_MODE => t('Display mode'),
        self::OUTPUT_VIEW => t('View')
      ),
    );

    switch ($output_mode) {
      case self::OUTPUT_DISPLAY_MODE:
        $this->get_output_display_form($form, $form_state, $entity);
        break;

      case self::OUTPUT_VIEW:
        $this->get_output_view_form($form, $form_state);
        break;
    }

    $form['more_link'] = array(
      '#type' => 'textfield',
      '#title' => t('More link'),
      '#default_value' => $more_link,
      '#required' => FALSE,
      '#description' => t('Add a more link to the bottom of the pane.  Leave empty for no "more" link.'),
    );

    $form['more_link_text'] = array(
      '#type' => 'textfield',
      '#title' => t('More link text'),
      '#default_value' => $more_link_text,
      '#required' => FALSE,
      '#description' => t('By default the link text is "View more".  Override text used for "more" link here.'),
    );

    return $form;
  }

  /**
   * Provides form for selecting a display mode to render the referenced entities
   *
   * @param array $form
   * @param array $form_state
   * @param string $entity
   */
  protected function get_output_display_form(&$form, &$form_state, $entity) {
    $display_modes = array();

    foreach (entity_get_info() as $name => $info) {
      foreach ($info['view modes'] as $machine_name => $mode_info) {
        $display_modes[$name][$machine_name] = $mode_info['label'];
      }
    }

    $form['view_mode'] = array(
      '#type' => 'select',
      '#title' => t('View Mode'),
      '#options' => $display_modes[$entity],
      '#description' => t('Display mode used to render selections'),
      '#default_value' => (TRUE === isset($this->configuration['view_mode'])) ? $this->configuration['view_mode'] : '',
      '#required' => TRUE,
    );
  }

  /**
   * Provides form for selecting a view to render the referenced entities
   *
   * @param array $form
   * @param array $form_state
   */
  protected function get_output_view_form(&$form, &$form_state) {
    $view_settings = empty($this->configuration['display_view']) ? '' : $this->configuration['display_view'];
    $views = views_get_enabled_views();

    $options = array();
    foreach ($views as $view) {
      foreach (array_keys($view->display) as $id) {
        $options[$view->name . ':' . $id] = $view->name . ' - ' . $view->display[$id]->display_title;
      }
    }

     if ($options) {
       $form['display_view'] = array(
         '#type' => 'select',
         '#title' => t('View used to display the entities'),
         '#required' => TRUE,
         '#options' => $options,
         '#default_value' => $view_settings,
         '#description' => '<p>' . t('Choose the view and display that outputs the selected entities') . '</p>',
       );
     }
  }

  /**
   * Provides the form for selecting by entity / bundle
   *
   * @param array $form_state
   *
   * @return array
   */
  protected function get_bundle_form(&$form, &$form_state) {
    $bundles = array();
    $entity = $form['entity']['#default_value'];

    foreach (entity_get_info() as $name => $info) {
      foreach ($info['bundles'] as $bundle => $bundle_info) {
        $bundles[$name][$bundle] = t($bundle_info['label']);
      }
    }

    $form['target_bundles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Target bundles'),
      '#options' => $bundles[$entity],
      '#description' => t('The bundles of the entity type that can be referenced. Optional, leave empty for all bundles.'),
      '#default_value' => (TRUE === isset($this->configuration['target_bundles'])) ? $this->configuration['target_bundles'] : array(),
    );
  }

  /**
   * Provides the form for selecting by view
   *
   * @param array $form_state
   *
   * @return array
   */
  protected function get_view_form(&$form, &$form_state) {
    $view_settings = empty($this->configuration['view']) ? '' : $this->configuration['view'];
    $displays = views_get_applicable_views('entityreference display');
    // Filter views that list the entity type we want, and group the separate
    // displays by view.
    $entity = $form['entity']['#default_value'];
    $entity_info = entity_get_info($entity);
    $options = array();
    foreach ($displays as $data) {
      list($view, $display_id) = $data;
      if ($view->base_table == $entity_info['base table']) {
        $options[$view->name . ':' . $display_id] = $view->name . ' - ' . $view->display[$display_id]->display_title;
      }
    }

    if ($options) {
      $default = !empty($view_settings['view_and_display']) ? $view_settings['view_and_display'] : NULL;
      $form['view']['view_and_display'] = array(
        '#type' => 'select',
        '#title' => t('View used to select the entities'),
        '#required' => TRUE,
        '#options' => $options,
        '#default_value' => $default,
        '#description' => '<p>' . t('Choose the view and display that select the entities that can be referenced.<br />Only views with a display of type "Entity Reference" are eligible.') . '</p>',
      );

      $default = !empty($view_settings['args']) ? $view_settings['args'] : '';
      $form['view']['args'] = array(
        '#type' => 'textfield',
        '#title' => t('View arguments'),
        '#default_value' => $default,
        '#required' => FALSE,
        '#description' => t('Provide a comma separated list of arguments to pass to the view.'),
      );

      $default = !empty($view_settings['min_results']) ? $view_settings['min_results'] : 0;
      $form['view']['min_results'] = array(
        '#type' => 'textfield',
        '#title' => t('Minimum items'),
        '#default_value' => $default,
        '#required' => FALSE,
        '#description' => t('If the number of items featured in the pane is fewer than the number set here, the difference will be made up by pulling non-duplicate items from the view. Sorts will be obeyed. Enter 0 for no minimum.'),
      );
    }
    else {
      $form['view']['no_view_help'] = array(
        '#markup' => '<p>' . t('No eligible views were found. <a href="@create">Create a view</a> with an <em>Entity Reference</em> display, or add such a display to an <a href="@existing">existing view</a>.', array(
          '@create' => url('admin/structure/views/add'),
          '@existing' => url('admin/structure/views'),
        )) . '</p>',
      );
    }
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

    if (TRUE === isset($form_state['values']['configuration']['more_link'])) {
      $configuration['more_link'] = $form_state['values']['configuration']['more_link'];
    }

    if (TRUE === isset($form_state['values']['configuration']['more_link_text'])) {
      $configuration['more_link_text'] = $form_state['values']['configuration']['more_link_text'];
    }

    if (TRUE === isset($form_state['values']['configuration']['view_mode'])) {
      $configuration['view_mode'] = (string) $form_state['values']['configuration']['view_mode'];
    }

    if (TRUE === isset($form_state['values']['configuration']['view'])){
      $configuration['view'] = $form_state['values']['configuration']['view'];
    }

    if (TRUE === isset($form_state['values']['configuration']['output_mode'])) {
      $configuration['output_mode'] = $form_state['values']['configuration']['output_mode'];
    }

    if (TRUE === isset($form_state['values']['configuration']['display_view'])) {
      $configuration['display_view'] = $form_state['values']['configuration']['display_view'];
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

    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', $entity_type)
    ->range(0, 10);

    if (FALSE === empty($this->configuration['target_bundles'])) {
      $query->entityCondition('bundle', $this->configuration['target_bundles'], 'IN');
    }

    // Add a generic entity access tag to the query.
    $query->addTag($entity_type . '_access');
    $query->addTag('pane_entity_reference');
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

    $callback = $this->configuration['mode'] .'_find_entities_by_id';
    $results = $this->$callback($ids);

    $entities = array();

    if (!empty($results)) {
      $entities = entity_load($entity_type, array_keys($results));
    }

    return $entities;
  }

  public function bundle_find_entities_by_id($ids){
    $entity_type = $this->configuration['entity'];
    $entity_info = entity_get_info($entity_type);
    $id_field = $entity_info['entity keys']['id'];

    if (FALSE === is_array($ids)) {
      $ids = array($ids);
    }

    $query = $this->build_entity_query();

    $query->propertyCondition($id_field, $ids, 'IN');

    $results = $query->execute();

    // Now that we have the matches let's compare them to the IDs we have in configuration
    $keys = array_keys($results[$entity_type]);
    $ids = array_intersect($ids, $keys);

    $output = array();

    foreach ($ids as $id) {
      $output[$id] = $results[$entity_type][$id];
    }

    return $output;
  }

  public function view_find_entities_by_id($ids){
    return $this->executeView(NULL, 'CONTAINS', 0, $ids);
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
    $entities = array();

    $string = trim($string);

    // If our title is empty never return values
    if (FALSE === empty($string)) {
      $callback = $this->configuration['mode'] . '_find_entities_by_title';
      $results = $this->$callback($string);

      if (!empty($results)) {
        $entities = entity_load($entity_type, array_keys($results));
      }
    }

    return $entities;
  }
  
  /**
   * Returns the name of the field to use as the title
   * 
   * @param string $entity_type
   */
  protected function get_title_field($entity_type) {
    $entity_info = entity_get_info($entity_type);
    $title_field = NULL;
    
    // If the label is defined for the entity
    if (TRUE === isset($entity_info['entity keys']['label'])) {
      $title_field = $entity_info['entity keys']['label'];
    }
    // Otherwise check to see if it's integrated with the Title module
    else if (TRUE === isset($entity_info['field replacement'])) {
      $title_field = array_shift(array_keys($entity_info['field replacement']));
    }
    
    return $title_field;
  }

  public function bundle_find_entities_by_title($match = NULL, $match_operator = 'CONTAINS', $limit = 0) {
    $entity_type = $this->configuration['entity'];
    $entity_info = entity_get_info($entity_type);
    $title_field = $this->get_title_field($entity_type);
    
    // If we have a title field use it
    if ($title_field) {

      $query = $this->build_entity_query();
  
      $query->propertyCondition($title_field, $match, $match_operator);
  
      $results = $query->execute();
  
      return $results[$entity_type];
    }
    // Otherwise log an error since we can't find by title
    else {
      watchdog(WATCHDOG_ERROR, 'Unable to determine title field to use for %entity_type', array('%entity_type' => $entity_type));
      return array();
    }
  }

  public function view_find_entities_by_title($match = NULL, $match_operator = 'CONTAINS', $limit = 0) {
    return $this->executeView($match, $match_operator, $limit);
  }


  public function executeView($match = NULL, $match_operator = 'CONTAINS', $limit = 0, $ids = NULL) {
    // Split view name and display name from the 'view_and_display' value.
    if (!empty($this->configuration['view']['view_and_display'])) {
      list($view_name, $display_name) = explode(':', $this->configuration['view']['view_and_display']);
    }

    // Explode the 'args' string into an actual array. Beware, explode() turns an
    // empty string into an array with one empty string. We'll need an empty array
    // instead.
    $args_string = trim($this->configuration['view']['args']);
    if ($args_string === '') {
      $args = array();
    }
    else {
      // array_map is called to trim whitespaces from the arguments.
      $args = array_map('trim', str_getcsv($args_string, ',', '"'));
    }

    // Check that the view is valid and the display still exists.
    $this->view = views_get_view($view_name);
    if (!$this->view || !isset($this->view->display[$display_name]) || !$this->view->access($display_name)) {
      watchdog('pane_entity_reference', 'The view %view_name is no longer eligible for the %pane_name pane.', array('%view_name' => $view_name, '%pane_name' => $this->getTitle()), WATCHDOG_WARNING);
      return FALSE;
    }
    $this->view->set_display($display_name);

    // Make sure the query is not cached.
    $this->view->is_cacheable = FALSE;

    // Pass options to the display handler to make them available later.
    $entityreference_options = array(
      'match' => $match,
      'match_operator' => $match_operator,
      'limit' => $limit,
      'ids' => $ids,
    );
    $this->view->display_handler->set_option('entityreference_options', $entityreference_options);
    return $this->view->execute_display($display_name, $args);
  }
}
