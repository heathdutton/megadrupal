<?php

/**
 * @file
 * Implements the ces_message template functionality.
 */

/**
 * @defgroup ces_message_templates Ces Message Templates
 * @ingroup ces_message
 * @{
 * creating a core Entity API entity.
 */

/**
 * Basic information for the page.
 */
function ces_message_template_info_page() {
  $content['preface'] = array(
    '#type' => 'item',
    '#markup' => t('The entity ces message provides a template entity.'),
  );
  $content['table'] = ces_message_template_list_entities();

  return $content;
}


/**
 * Fetch a template object.
 *
 * This function ends up being a shim between the menu system and
 * ces_message_template_load_multiple().
 *
 * This function gets its name from the menu system's wildcard
 * naming conventions. For example, /path/%wildcard would end
 * up calling wildcard_load(%wildcard value). In our case defining
 * the path: examples/ces_message/template/%ces_message_template in
 * hook_menu() tells Drupal to call ces_message_template_load().
 *
 * @param int $template_id
 *   Integer specifying the ces message id.
 * @param bool $reset
 *   A boolean indicating that the internal cache should be reset.
 *
 * @return object
 *   A fully-loaded $template object or FALSE if it cannot be loaded.
 *
 * @see ces_message_template_load_multiple()
 * @see ces_message_menu()
 */
function ces_message_template_load($template_id = NULL, $reset = FALSE) {
  $template_ids = (isset($template_id) ? array($template_id) : array());
  $template = ces_message_template_load_multiple($template_ids, array(), $reset);
  return $template ? reset($template) : FALSE;
}
/**
 * Loads multiple template entities.
 *
 * We only need to pass this request along to entity_load(), which
 * will in turn call the load() method of our entity controller class.
 */
function ces_message_template_load_multiple($template_ids = FALSE, $conditions = array(), $reset = FALSE) {
  return entity_load('ces_message_template', $template_ids, $conditions, $reset);
}

/**
 * Implements the uri callback.
 */
function ces_message_template_uri($template) {
  return array(
    'path' => 'ces/admin/message/' . $template->template_id,
  );
}

/**
 * Returns a render array with all ces_message_template entities.
 *
 * In this template example we know that there won't be many entities,
 * so we'll just load them all for display. See pager_example.module
 * to implement a pager. Most implementations would probably do this
 * with the contrib Entity API module, or a view using views module,
 * but we avoid using non-core features in the Templates project.
 *
 * @see pager_example.module
 */
function ces_message_template_list_entities() {
  $content = array();
  // Load all of our entities.
  $entities = ces_message_template_load_multiple();
  if (!empty($entities)) {
    foreach ($entities as $entity) {
      // Create tabular rows for our entities.
      $ident = $entity->ces_action_id . '-' . $entity->exchange_id;

      $rows[$ident]['data']['ces_action'] = ces_message_action_load($entity->ces_action_id)->description;
      if (isset($rows[$ident]['data']['language'])) {
        $rows[$ident]['data']['language'] = $rows[$ident]['data']['language'] .
          ' ' .
          l($entity->language, 'ces/admin/message/' . $entity->template_id . '/view');
      }
      else {
        $rows[$ident]['data']['language'] = l($entity->language, 'ces/admin/message/' . $entity->template_id . '/view');
      }
      $rows[$ident]['data']['subject'] = $entity->subject;
      if ($entity->exchange_id == 0) {
        $exchange = t('Default');
      }
      else {
        $exchange_data = ces_bank_get_exchange($entity->exchange_id);
        $exchange = $exchange_data['shortname'];
      }
      $rows[$ident]['data']['exchange'] = $exchange;
    }
    // Put our entities into a themed table. See theme_table() for details.
    $content['entity_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => array(
        t('Action'),
        t('Languages'),
        t('Subject'),
        t('Exchange'),
      ),
    );
  }
  else {
    // There were no entities. Tell the user.
    $content[] = array(
      '#type' => 'item',
      '#markup' => t('No ces message template entities currently exist.'),
    );
  }
  return $content;
}

/**
 * Callback for a page title when this entity is displayed.
 */
function ces_message_template_title($entity) {
  return t('@ces_action', array('@ces_action' => ces_message_action_load($entity->ces_action_id)->name));
}

/**
 * Menu callback to display an entity.
 *
 * As we load the entity for display, we're responsible for invoking a number
 * of hooks in their proper order.
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 * @see hook_entity_view_alter()
 */
function ces_message_template_view($entity, $view_mode = 'tweaky') {
  // Our entity type, for convenience.
  $entity_type = 'ces_message_template';
  // Start setting up the content.
  $entity->content = array(
    '#view_mode' => $view_mode,
  );
  // Build fields content - this is where the Field API really comes in to play.
  // The task has very little code here because it all gets taken care of by
  // field module.
  // field_attach_prepare_view() lets the fields load any data they need
  // before viewing.
  field_attach_prepare_view($entity_type, array($entity->template_id => $entity),
    $view_mode);
  // We call entity_prepare_view() so it can invoke hook_entity_prepare_view()
  // for us.
  entity_prepare_view($entity_type, array($entity->template_id => $entity));
  // Now field_attach_view() generates the content for the fields.
  $entity->content += field_attach_view($entity_type, $entity, $view_mode);

  // OK, Field API done, now we can set up some of our own data.
  $entity->content['created'] = array(
    '#type' => 'item',
    '#title' => t('Created date'),
    '#markup' => format_date($entity->created),
  );
  $entity->content['ces_action'] = array(
    '#type' => 'item',
    '#title' => t('Action'),
    '#markup' => t(ces_message_action_load($entity->ces_action_id)->description),
  );
  $entity->content['language'] = array(
    '#type' => 'item',
    '#title' => t('Language'),
    '#markup' => $entity->language,
  );
  $entity->content['subject'] = array(
    '#type' => 'item',
    '#title' => t('Subject'),
    '#markup' => $entity->subject,
  );
  $entity->content['body'] = array(
    '#type' => 'item',
    '#prefix' => '<pre>',
    '#suffix' => '</pre>',
    '#title' => t('Body'),
    '#markup' => $entity->body,
  );

  // Now to invoke some hooks. We need the language code for
  // hook_entity_view(), so let's get that.
  global $language;
  $langcode = $language->language;
  // And now invoke hook_entity_view().
  module_invoke_all('entity_view', $entity, $entity_type, $view_mode,
    $langcode);
  // Now invoke hook_entity_view_alter().
  drupal_alter(array('ces_message_template_view', 'entity_view'),
    $entity->content, $entity_type);

  // And finally return the content.
  return $entity->content;
}

/**
 * Implements hook_field_extra_fields().
 *
 * This exposes the "extra fields" (usually properties that can be configured
 * as if they were fields) of the entity as pseudo-fields
 * so that they get handled by the Entity and Field core functionality.
 * Node titles get treated in a similar manner.
 */
function ces_message_field_extra_fields() {
  $form_elements['ces_action'] = array(
    'label' => t('Action'),
    'description' => t('Action'),
    'weight' => 0,
  );
  $form_elements['language'] = array(
    'label' => t('Language'),
    'description' => t('Language'),
    'weight' => 3,
  );
  $form_elements['subject'] = array(
    'label' => t('Subject'),
    'description' => t('Subject of the message'),
    'weight' => 5,
  );
  $form_elements['body'] = array(
    'label' => t('Body'),
    'description' => t('Body of the message'),
    'weight' => 7,
  );
  $display_elements['created'] = array(
    'label' => t('Creation date'),
    'description' => t('Creation date (an extra display field)'),
    'weight' => 9,
  );
  $display_elements['ces_action'] = array(
    'label' => t('Action'),
    'description' => t('Action'),
    'weight' => 0,
  );
  $display_elements['language'] = array(
    'label' => t('Language'),
    'description' => t('Language'),
    'weight' => 3,
  );
  $display_elements['subject'] = array(
    'label' => t('Subject'),
    'description' => t('Subject of the message'),
    'weight' => 6,
  );
  $display_elements['body'] = array(
    'label' => t('Body'),
    'description' => t('Body of the message'),
    'weight' => 9,
  );

  // Since we have only one bundle type, we'll just provide the extra_fields
  // for it here.
  $extra_fields['ces_message_template']['ces_message_bundle']['form'] = $form_elements;
  $extra_fields['ces_message_template']['ces_message_bundle']['display'] = $display_elements;

  return $extra_fields;
}

/**
 * Provides a wrapper on the edit form to add a new entity.
 */
function ces_message_template_add() {
  // Create a entity structure to be used and passed to the validation
  // and submission functions.
  $entity = entity_get_controller('ces_message_template')->create();
  return drupal_get_form('ces_message_template_form', $entity);
}

/**
 * Form function to create an ces_message_template entity.
 *
 * The pattern is:
 * - Set up the form for the data that is specific to your
 *   entity: the columns of your base table.
 * - Call on the Field API to pull in the form elements
 *   for fields attached to the entity.
 */
function ces_message_template_form($form, &$form_state, $entity) {
  if (!$entity->exchange_id) {
    $exchange = ces_bank_get_current_exchange();
    $entity->exchange_id = $exchange['id'];
  }

  $form['language'] = array(
    '#title' => t('Language'),
    '#description' => t('The language code that will be saved with the field values. This is used to allow translation of fields.'),
  );
  $field_language = NULL;
  if (module_exists('locale')) {
    $options = array();
    foreach (language_list() as $key => $value) {
      if (!empty($value->enabled)) {
        $options[$key] = $value->name;
      }
    }
    $field_language = !empty($form_state['values']['language']) ? $form_state['values']['language'] : language_default()->language;
    $form['language'] += array(
      '#type' => 'select',
      '#options' => $options,
      '#required' => TRUE,
      '#default_value' => $field_language,
      // '#ajax' => array(
      // 'callback' => 'ces_message_fields_ajax_callback',
      // 'wrapper' => 'ces-message-wrapper',
      // ),
    );
  }
  else {
    $form['language'] += array(
      '#type' => 'textfield',
      '#markup' => t('English'),
      '#default_value' => 'en',
      '#title' => t('Language'),
    );
  }
  $options = _ces_message_get_actions_options();
  $options = array('' => t('Select a action...')) + $options;
  $form['ces_action'] = array(
    '#type' => 'select',
    '#title' => t('Action'),
    '#description' => t('Select the ces action.'),
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => $entity->ces_action_id,
    '#weight' => 0,
  );
  // $form['ces_action'] = array(
  // '#type' => 'textfield',
  // '#title' => t('Action'),
  // '#required' => TRUE,
  // '#default_value' => $entity->ces_action,
  // '#weight' => 0,
  // );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
    '#translatable' => TRUE,
    '#default_value' => $entity->subject,
    '#weight' => 6,
  );

  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#required' => TRUE,
    '#translatable' => TRUE,
    '#rows' => 10,
    '#default_value' => $entity->body,
    '#weight' => 9,
  );

  $form['template_entity'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );

  field_attach_form('ces_message_template', $entity, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('ces_message_template_edit_delete'),
    '#weight' => 200,
  );

  $token_types = array('ces_bank');
  if (module_exists('token')) {
    $form['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => $token_types,
      '#weight' => 90,
    );
  }
  else {
    $form['token_tree'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available token browser.', array('@drupal-token' => 'http://drupal.org/project/token')) . '</p>',
    );
  }
  return $form;
}

/**
 * AJAX callback to attach the message type fields to the form.
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 */
function ces_message_fields_ajax_callback(array $form, array &$form_state) {
  return $form['ces_message_fields'];
}


/**
 * Validation handler for ces_message_template_add_form form.
 *
 * We pass things straight through to the Field API to handle validation
 * of the attached fields.
 */
function ces_message_template_form_validate($form, &$form_state) {
  field_attach_form_validate('ces_message_template', $form_state['values']['template_entity'], $form, $form_state);
}


/**
 * Form submit handler: Submits template_add_form information.
 */
function ces_message_template_form_submit($form, &$form_state) {

  $entity = $form_state['values']['template_entity'];
  $entity->subject = $form_state['values']['subject'];
  $entity->body = $form_state['values']['body'];
  $entity->ces_action_id = $form_state['values']['ces_action'];
  $entity->language = $form_state['values']['language'];
  field_attach_submit('ces_message_template', $entity, $form, $form_state);
  $entity = ces_message_template_save($entity);

  $form_state['redirect'] = 'ces/admin/message/' . $entity->template_id . '/view';

}

/**
 * Form deletion handler.
 */
function ces_message_template_edit_delete($form, &$form_state) {
  $entity = $form_state['values']['template_entity'];
  ces_message_template_delete($entity);
  drupal_set_message(t('The entity %subject (ID %id) has been deleted',
    array('%subject' => $entity->subject, '%id' => $entity->template_id))
  );
  $form_state['redirect'] = 'ces/admin/message/list';
}

/**
 * We save the entity by calling the controller.
 */
function ces_message_template_save(&$entity) {
  return entity_get_controller('ces_message_template')->save($entity);
}


/**
 * Use the controller to delete the entity.
 */
function ces_message_template_delete($entity) {
  entity_get_controller('ces_message_template')->delete($entity);
}

/**
 * CesMessageTemplateControllerInterface definition.
 *
 * We create an interface here because anyone could come along and
 * use hook_entity_info_alter() to change our controller class.
 * We want to let them know what methods our class needs in order
 * to function with the rest of the module, so here's a handy list.
 *
 * @see hook_entity_info_alter()
 */
interface CesMessageTemplateControllerInterface
  extends DrupalEntityControllerInterface {

  /**
   * Create an entity.
   */
  public function create();

  /**
   * Save an entity.
   *
   * @param object $entity
   *   The entity to save.
   */
  public function save($entity);

  /**
   * Delete an entity.
   *
   * @param object $entity
   *   The entity to delete.
   */
  public function delete($entity);

}

/**
 * CesMessageTemplateController extends DrupalDefaultEntityController.
 *
 * Our subclass of DrupalDefaultEntityController lets us add a few
 * important create, update, and delete methods.
 */
class CesMessageTemplateController
  extends DrupalDefaultEntityController
  implements CesMessageTemplateControllerInterface {

  /**
   * Create and return a new ces_message_template entity.
   */
  public function create() {
    global $language;
    $entity = new stdClass();
    $entity->type = 'ces_message_template';
    $entity->template_id = 0;
    $entity->exchange_id = 0;
    $entity->bundle_type = 'ces_message_bundle';
    $entity->subject = '';
    $entity->body = '';
    $entity->ces_action_id = '';
    $entity->language = $language->language;
    return $entity;
  }

  /**
   * Saves the custom fields using drupal_write_record().
   */
  public function save($entity) {
    // If our entity has no template_id, then we need to give it a
    // time of creation.
    if (empty($entity->template_id)) {
      $entity->template_id = NULL;
      $entity->created = time();
    }
    // Invoke hook_entity_presave().
    module_invoke_all('entity_presave', $entity, 'ces_message_template');
    // The 'primary_keys' argument determines whether this will be an insert
    // or an update. So if the entity already has an ID, we'll specify
    // template_id as the key.
    $primary_keys = $entity->template_id ? 'template_id' : array();
    // Write out the entity record.
    drupal_write_record('ces_message_template', $entity, $primary_keys);
    // We're going to invoke either hook_entity_update() or
    // hook_entity_insert(), depending on whether or not this is a
    // new entity. We'll just store the name of hook_entity_insert()
    // and change it if we need to.
    $invocation = 'entity_insert';
    // Now we need to either insert or update the fields which are
    // attached to this entity. We use the same primary_keys logic
    // to determine whether to update or insert, and which hook we
    // need to invoke.
    if (empty($primary_keys)) {
      field_attach_insert('ces_message_template', $entity);
      $message = t('Inserted template %action.', array('%action' => ces_message_action_load($entity->ces_action_id)->description));
      drupal_set_message($message);
    }
    else {
      field_attach_update('ces_message_template', $entity);
      $invocation = 'entity_update';
    }
    // Invoke either hook_entity_update() or hook_entity_insert().
    module_invoke_all($invocation, $entity, 'ces_message_template');
    return $entity;
  }

  /**
   * Delete a single entity.
   *
   * Really a convenience function for deleteMultiple().
   */
  public function delete($entity) {
    $this->deleteMultiple(array($entity));
  }

  /**
   * Delete one or more ces_message_template entities.
   *
   * Deletion is unfortunately not supported in the base
   * DrupalDefaultEntityController class.
   *
   * @param array $entities
   *   An array of entity IDs or a single numeric ID.
   */
  public function deleteMultiple($entities) {
    $template_ids = array();
    if (!empty($entities)) {
      $transaction = db_transaction();
      try {
        foreach ($entities as $entity) {
          // Invoke hook_entity_delete().
          module_invoke_all('entity_delete', $entity, 'ces_message_template');
          field_attach_delete('ces_message_template', $entity);
          $template_ids[] = $entity->template_id;
        }
        db_delete('ces_message_template')
          ->condition('template_id', $template_ids, 'IN')
          ->execute();
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('ces_message', $e);
        throw $e;
      }
    }
  }
}

/**
 * @} End of "defgroup ces_message_template".
 */
