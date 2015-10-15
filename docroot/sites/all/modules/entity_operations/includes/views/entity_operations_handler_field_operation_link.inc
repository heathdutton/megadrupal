<?php

/**
 * Generic Views field handler for showing links to operations.
 *
 * Based on views_handler_field_node_link.
 */
class entity_operations_handler_field_operation_link extends views_handler_field_entity {

  /**
   * Define defaults for options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['operation_name'] = array('default' => NULL);
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    $options['rewrite_link_text'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Build the options form.
   */
  function options_form(&$form, &$form_state) {
    $entity_operations_info = entity_operations_get_operation_info($this->entity_type);
    $options = array();
    foreach ($entity_operations_info as $operation_name => $operation_definition) {
      if (!empty($operation_definition['provision']['views field'])) {
        $handler_class = $operation_definition['handler'];
        $operation_handler = new $handler_class($this->entity_type, $operation_name);
        $operation_info = $operation_handler->operationInfo();

        $options[$operation_name] = $operation_handler->getOperationAdminString('label');
      }
    }

    $form['operation_name'] = array(
      '#type' => 'radios',
      '#title' => t('Operation'),
      '#description' => t('The operation tab on the entity this field should link to.'),
      '#options' => $options,
      '#required' => TRUE,
      '#default_value' => $this->options['operation_name'],
    );

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    $form['rewrite_link_text'] = array(
      '#type' => 'checkbox',
      '#title' => t('Rewrite the link text of this field'),
      '#description' => t('Enable to override the link text with replacement tokens as per the "Replacement patterns" below.'),
      '#default_value' => $this->options['rewrite_link_text'],
    );

    parent::options_form($form, $form_state);

    // Copy the replacement patterns help (now our parent has made it) so the
    // user can see it without having to open up the 'Rewrite results' fieldset.
    $form['help'] = $form['alter']['help'];
    $form['help']['#dependency'] = array(
      'edit-options-rewrite-link-text' => array(1),
    );

    // The path is set by render_link function so don't allow to set it.
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
  }

  /**
   * Require the entity operation name to be set.
   *
   * This prevents an AJAX error when adding the field, before it's been
   * configured.
   */
  function validate() {
    $errors = parent::validate();

    if (empty($this->options['operation_name'])) {
      $errors[] = t('The operation name must be set.');
    }

    return $errors;
  }

  /**
   * Run before any fields are rendered.
   */
  function pre_render(&$values) {
    // This gets us the entities that correspond to our rows.
    parent::pre_render($values);

    // Get the handler for the operation.
    $operation_name = $this->options['operation_name'];

    $entity_operations_info = entity_operations_get_operation_info($this->entity_type);
    $operation_definition = $entity_operations_info[$operation_name];
    $handler_class = $entity_operations_info[$operation_name]['handler'];

    $this->operation_handler = new $handler_class($this->entity_type, $operation_name);

    // Spoofily get the menu item, in case the access callback has a $params
    // array. This is (currently) for the single case of a field that is itself
    // provided by a Views operations tab, which therefore needs parameters for
    // views_access().
    // (Yes, this is messy: we totally fake the loader position as it's
    // probably (!) not relevant.)
    // TODO: Would this be better done in hook_views_data() for performance?
    $menu_item = $this->operation_handler->menu_item($operation_name, $operation_definition, 0);
    $access_params = isset($menu_item['access arguments'][4]) ? $menu_item['access arguments'][4] : array();

    // We handle access here, because this lets us determine overall access
    // to all of the rows, and lets us remove ourselves from the view if the
    // user has access to nothing.
    // We can't use access(), which is the method we're meant to use to deny
    // access to a field wholesale, because that is called before we have any
    // results (and thus any entities) from the view.
    // @see http://drupal.org/node/1915230
    $this->row_access = array();
    // We have to check the entities array, as
    // views_handler_field_entity::pre_render() only sets it if there is a
    // result for the view.
    if (!empty($this->entities)) {
      foreach ($this->entities as $entity) {
        // Build up an array of access for each row for render() to use.
        $id = $entity->identifier();
        $this->row_access[$id] = $this->operation_handler->access($this->entity_type, $entity, $access_params);
      }
    }

    // The overall access to this field is whether the user has access to any
    // of the rows.
    $overall_access = count(array_filter($this->row_access));

    // If the user has access to this operation on none of the rows, we assume
    // they generally lack the right permissions, and remove ourselves wholesale
    // from this view.
    // This ensures, for instance, that the user doesn't get an empty (and
    // likely meaningless!) column in a table view.
    if (!$overall_access) {
      unset($this->view->field[$this->options['id']]);
      // Just bail now, no point.
      return;
    }

    // Determine whether we should use a destination on the link.
    $operation_info = $this->operation_handler->operationInfo();
    if (!empty($operation_info['uses form'])) {
      $this->options['alter']['query'] = drupal_get_destination();
    }
  }

  /**
   * Render the field.
   */
  function render($values) {
    if ($entity = $this->get_value($values)) {
      $id = $entity->identifier();
      // Check the access we got in pre_render().
      if (!$this->row_access[$id]) {
        return;
      }

      return $this->render_link($entity, $values);
    }
  }

  /**
   * Render this field as a link.
   */
  function render_link($entity, $values) {
    // Get the entity URI and append the operation path component.
    $uri = entity_uri($this->entity_type, $entity);
    $uri['path'] .= '/' . $this->options['operation_name'];

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $uri['path'];
    // The 'query' value is set in pre_render() as it's common to the whole
    // View result.

    if (!empty($this->options['text'])) {
      $link_text = $this->options['text'];

      if ($this->options['rewrite_link_text']) {
        $tokens = $this->get_render_tokens($link_text);
        $alter = array('text' => $link_text);
        $link_text = $this->render_altered($alter, $tokens);
      }
    }
    else {
      $link_text = $this->options['operation_name'];
    }

    return $link_text;
  }

}
