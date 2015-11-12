<?php

/**
 * @file
 * This file contains admin pages for the entity_collection module.
 */

/**
 * Form callback for the entity_collection add form.
 */
function entity_collection_add_form($form, &$form_state, $bundle) {
  $form_state['bundle'] = $bundle;
  $form = entity_collection_form($form, $form_state);
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save entity collection'),
    '#weight' => 40,
  );
  return $form;
}

/**
 * Form callback for the entity_collection edit form.
 */
function entity_collection_edit_form($form, &$form_state, $entity_collection) {
  $form_state['entity_collection'] = $entity_collection;
  $form = entity_collection_form($form, $form_state);
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );
  return $form;
}

/**
 * Form for deleting entity collections.
 */
function entity_collection_delete_form($form, &$form_state, $entity_collection) {
  $form_state['entity_collection'] = $entity_collection;
  return confirm_form($form, t('Are you sure you want to delete this entity collection?'), 'admin/structure/entity_collection');
}

/**
 * Submit function for deleting entity collections.
 */
function entity_collection_delete_form_submit($form, &$form_state) {
  entity_delete('entity_collection', $form_state['entity_collection']->eid);
  $form_state['redirect'] = 'admin/structure/entity_collection';
}

/**
 * Form for creating entity_collections.
 * @param array $form
 * @param array $form_state
 *   The form state should contain at least the type of the entity block.
 * @return array the form for the entity block.
 */
function entity_collection_form($form = array(), &$form_state = array()) {
  if (empty($form_state['entity_collection'])) {
    $form_state['entity_collection'] = new EntityCollection();
    $form_state['entity_collection']->bundle = $form_state['bundle'];
  }
  else {
    _entity_collection_context_form($form, $form_state);
  }
  $entity_info = entity_get_info();
  $entity_collection = $form_state['entity_collection'];
  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => $entity_collection->title,
    '#title' => t('Title'),
  );
  // Machine-readable name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $entity_collection->name,
    '#disabled' => isset($entity_collection->name),
    '#required' => TRUE,
    '#machine_name' => array(
      'exists' => 'entity_collection_load',
      'source' => array('title'),
    ),
    '#description' => t('A unique machine-readable name for this block. It must only contain lowercase letters, numbers, and underscores.'),
  );
  $form['style'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Style'),
    '#options' => EntityCollectionStyle::getStyleOptions(),
    '#default_value' => isset($entity_collection->style) ? $entity_collection->style : NULL,
    '#description' => t('Select the style you want to use for rendering this entity collection here.'),
    '#ajax' => array(
      'callback' => 'entity_collection_form_settings',
      'wrapper' => 'collection-settings',
    ),
  );

  $form['row'] = array(
    '#type' => 'select',
    '#title' => t('Row'),
    '#required' => TRUE,
    '#options' => EntityCollectionRow::getRowOptions(),
    '#default_value' => isset($entity_collection->row) ? $entity_collection->row : NULL,
    '#description' => t('Select the style you want to use for rendering this entity collection here.'),
    '#ajax' => array(
      'callback' => 'entity_collection_form_settings',
      'wrapper' => 'collection-settings',
    ),
  );
  $form['settings'] = array(
    '#tree' => TRUE,
    '#type' => 'vertical_tabs',
    '#prefix' => '<div id="collection-settings">',
    '#suffix' => '</div>'
  );
  // Allow for potential row settings.
  if (isset($form_state['values']['row'])) {
    $entity_collection->row = $form_state['values']['row'];
  }
  $row = EntityCollectionRow::getRow($entity_collection);
  $form['settings']['row_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Row settings'),
    '#tree' => TRUE,
  );
  if (isset($row) && method_exists($row, 'settingsForm')) {
    $row->settingsForm($form['settings']['row_settings']);
  }

  // Allow for potential style settings.
  if (isset($form_state['values']['style'])) {
    $entity_collection->style = $form_state['values']['style'];
  }
  $style = EntityCollectionStyle::getStyle($entity_collection);
  $form['settings']['style_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default style settings'),
    '#tree' => TRUE,
  );
  if (isset($style) && method_exists($style, 'settingsForm')) {
    $style->settingsForm($form['settings']['style_settings']);
  }
  // Content restriction.
  $form['settings']['allowed_bundles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Allowed bundles'),
    '#tree' => TRUE,
  );

  foreach ($entity_info as $entity_name => $info) {
    $bundle_options = array();
    foreach ($info['bundles'] as $bundle_name => $bundle) {
      $bundle_options[$bundle_name] = $bundle['label'];
    }
    $form['settings']['allowed_bundles'][$entity_name] = array(
      '#type' => 'checkboxes',
      '#title' => $info['label'],
      '#options' => $bundle_options,
      '#default_value' => isset($entity_collection->settings['allowed_bundles'][$entity_name]) ?
      drupal_map_assoc($entity_collection->settings['allowed_bundles'][$entity_name]) : array(),
    );
  }
  $storage = EntityCollectionStorage::getBundleStorage($entity_collection->bundle);
  if (!empty($storage) && method_exists($storage, 'entitySettingsForm')) {
    $storage->settingsForm($entity_collection, $form['settings'], $form_state);
  }

  // Attach the field form.
  field_attach_form('entity_collection', $entity_collection, $form, $form_state);

  $form['#validate'][] = 'entity_collection_form_validate';
  $form['#submit'][] = 'entity_collection_form_submit';
  return $form;
}

function entity_collection_form_settings(&$form) {
  return $form['settings'];
}

/**
 * Form for selecting contexts for an entity collection.
 */
function _entity_collection_context_form(&$form, &$form_state) {
  ctools_include('context-admin');
  ctools_include('plugins-admin');
  ctools_include('cache');
  ctools_context_admin_includes();

  $entity_collection = $form_state['entity_collection'];
  if (!isset($entity_collection->requiredcontexts)) {
    $entity_collection->requiredcontexts = array();
  }
  $cached_entity_collection = ctools_cache_get('entity_collection_cache_object', $entity_collection->name);
  if ($cached_entity_collection) {
    $form_state['entity_collection'] = $entity_collection = $cached_entity_collection;
  }
  else {
    ctools_cache_set('entity_collection_cache_object', $entity_collection->name, $entity_collection);
  }
  ctools_context_add_required_context_form('entity_collection_cache_object',
    $form, $form_state, $form['contexts']['required'],
    $entity_collection, $entity_collection->name);
}

/**
 * Validate an entity colleciton submitted by entity_collection_form.
 * @param array $form
 * @param array $form_state
 */
function entity_collection_form_validate($form, &$form_state) {
  // Fetch the entity_collection and populate it.
  // Since this is very simple to do,
  // let's just do it ourselves instead of using the entity api function.
  $entity_collection = &$form_state['entity_collection'];
  $entity_collection->name = $form_state['values']['name'];
  $entity_collection->title = $form_state['values']['title'];
  $entity_collection->style = $form_state['values']['style'];
  $entity_collection->row = $form_state['values']['row'];
  field_attach_form_validate('entity_collection', $entity_collection, $form, $form_state);
}

/**
 * Save the actual entity collection.
 * @param array $form
 * @param array $form_state
 */
function entity_collection_form_submit($form, &$form_state) {
  ctools_include('cache');
  $entity_collection = $form_state['entity_collection'];
  // Let the style plugin postprocess the new style settings.
  $style = EntityCollectionStyle::getStyle($entity_collection);
  $style->formSettingsSubmit($form_state['values']['settings']['style_settings']);
  // Transfer any required context from the cached version,
  // since that is handled by CTools.
  $cached_entity_collection = ctools_cache_get('entity_collection_cache_object', $entity_collection->name);
  if (isset($cached_entity_collection)) {
     $entity_collection->requiredcontexts = $cached_entity_collection->requiredcontexts;
  }
  $entity_collection->settings['row_settings'] = isset($form_state['values']['settings']['row_settings']) ?
    $form_state['values']['settings']['row_settings'] : array();
  $entity_collection->settings['style_settings'] = isset($form_state['values']['settings']['style_settings']) ?
    $form_state['values']['settings']['style_settings'] : array();
  foreach ($form_state['values']['settings']['allowed_bundles'] as $entity_name => $allowed) {
    $entity_collection->settings['allowed_bundles'][$entity_name] = array();
    foreach ($allowed as $bundle => $allowed) {
      if ($allowed) {
        $entity_collection->settings['allowed_bundles'][$entity_name][] = $bundle;
      }
    }
  }
  ctools_cache_clear('entity_collection_cache_object', $entity_collection->name);
  field_attach_submit('entity_collection', $entity_collection, $form, $form_state);
  entity_save('entity_collection', $entity_collection);
  $form_state['redirect'] = 'admin/structure/entity_collection';
}

/**
 * An admin overview page.
 */
function entity_collection_admin() {
  // Get all entities.
  $entity_collections = entity_load('entity_collection');
  $entity_collection_table = array();
  $entity_collection = new EntityCollection();
  foreach ($entity_collections as $entity_collection) {
    $operations = l(t('Edit'), 'entity-collection/' . $entity_collection->name . '/edit');
    if (!$entity_collection->hasStatus(ENTITY_IN_CODE)) {
       $operations .= ' | ' . l(t('Delete'), 'entity-collection/' . $entity_collection->name . '/delete');
    }
    $operations .= ' | ' . l(t('Manage content'), 'entity-collection/' . $entity_collection->name . '/content');
    $entity_collection_table[] = array(
      $entity_collection->title,
      $entity_collection->bundle,
      $operations,
    );
  }
  if (count($entity_collection_table)) {
    return theme('table', array('rows' => $entity_collection_table, 'header' => array('Title', 'Type', 'Operations')));
  }
  return t("You haven't created any entity collections yet.");
}

/**
 * Content admin page callback.
 */
function entity_collection_content_admin($entity_collection) {
  $contexts = _entity_collection_get_contexts($entity_collection);
  if (count($contexts) != count($entity_collection->requiredcontexts)) {
    return drupal_get_form('entity_collection_context_input_form', $entity_collection);
  }
  else {
    return array(
      drupal_get_form('entity_collection_content_form', $entity_collection),
      drupal_get_form('entity_collection_content_add_form', $entity_collection),
    );
  }
}

/**
 * Manage the content of a entity_collection.
 * This is a simple and included admin-side form.
 */
function entity_collection_content_form($form, &$form_state, $entity_collection, $redirect = FALSE) {
  $form_state['entity_collection'] = $entity_collection;
  if ($redirect) {
    $form['#redirect'] = url($redirect);
  }
  $contexts = $form_state['contexts'] = _entity_collection_get_contexts($entity_collection);
  $tree = EntityCollectionStorage::getBundleStorage($entity_collection->bundle)->retrieve($entity_collection, $contexts);
  $form['#tree'] = TRUE;
  entity_collection_content_form_content($entity_collection, $form, $tree);

  $form['style_uses_breakpoint'] = array(
    '#type' => 'value',
    '#value' => EntityCollectionStyle::getStyle($entity_collection)->useBreakpoint(),
  );
  
  if (isset($form['content'])) {
    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
    );
  }
  else {
    $form['empty_content'] = array(
      '#prefix' => '<strong>',
      '#suffix' => '</strong>',
      '#markup'=> t('This Entity Collection is empty.'),
    );
  }

  return $form;
}

/**
 * Add an entity to an entity collection.
 * @param type $entity_type
 * @param type $entity
 */
function entity_collection_ajax_add($entity_collection) {
  $entity_id = check_plain($_POST['entity_id']);
  $entity_type = check_plain($_POST['entity_type']);
  $entity = entity_load_single($entity_type, $entity_id);
  if (isset($entity)) {
    EntityCollectionStorage::getBundleStorage($entity_collection->bundle)->appendItem($entity_collection, new EntityCollectionTreeNode($entity_type, $entity_id, $entity, 'default'));
  }
  return '';
}

/**
 * Get entity type options suitable for #options for element.
 * @param EntityCollection $entity_collection
 * @return
 *   An array of options for all entity types.
 */
function entity_collection_get_entity_type_options(EntityCollection $entity_collection) {
  $info = entity_get_info();
  $options = array();
  foreach ($info as $name => $info) {
    if (!empty($entity_collection->settings['allowed_bundles'][$name])) {
      $options[$name] = $info['label'];
    }
  }
  return $options;
}

/**
 * Show the content of the entity collection in a form.
 * @param EntityCollection $entity_collection
 * @param array $form
 * @param EntityCollectionTreeNode $tree
 * @param $parent
 * @param $depth
 */
function entity_collection_content_form_content(EntityCollection $entity_collection, &$form, EntityCollectionTreeNode $tree, $parent = 0, $depth = 0) {
  $items = $tree->getChildren();
  $query = array('query' => drupal_get_destination());

  $i = 0;
  $break_at = FALSE;
  $style = EntityCollectionStyle::getStyle($entity_collection);
  $row = EntityCollectionRow::getRow($entity_collection);
  $form['#style'] = $style;
  $form['#row'] = $row;

  if ( $style->useBreakpoint() ) {
    $break_at = EntityCollectionStorage::getBundleStorage($entity_collection->bundle)->getBreakpoint($entity_collection, $entity_collection->contexts);
    if ( $break_at === FALSE ) {
      $break_at = $style->getBreakpoint($entity_collection);
    }
    if ( $break_at > count($items) ) {
      $break_at = count($items);
    }
  }
  foreach ($items as $key => $child) {
    if ( $break_at !== FALSE && $break_at == $i ) {
      $form['content']['breakpoint'] = array();
      entity_collection_content_form_get_break($form['content']['breakpoint'], $child->position, $child->style);
    }
    $i++;

    $entity_wrapper = entity_metadata_wrapper($child->type, $child->content);
    $links = array();
    if ($edit_path = entity_collection_edit_path($child->type, $child->content)) {
      $links[] = l(t('Edit'), $edit_path, $query);
    }
    $links[] = l(t('Remove from collection'), 'entity-collection/' . $entity_collection->name . '/content/delete/' . $child->type . '/' . $child->entity_id, $query);
    $form['content'][$key]['#content'] = $child;
    $form['content'][$key]['key'] = array(
      '#type' => 'hidden',
      '#value' => $child->key,
    );
    $form['content'][$key]['entity_id'] = array(
      '#type' => 'value',
      '#value' => $child->entity_id,
    );
    $form['content'][$key]['type'] = array(
      '#type' => 'value',
      '#value' => $child->type,
    );
    $form['content'][$key]['title'] = array(
      '#markup' => check_plain($entity_wrapper->label()),
    );
    $form['content'][$key]['position'] = array(
      '#type' => 'textfield',
      '#title' => t('Position'),
      '#size' => 3,
      '#default_value' => $child->position,
    );
    // Make sure the depth is always 0 if we don't allow depth.
    if (!$style->getMaxDepth()) {
      $parent = NULL;
      $depth = 0;
    }
    $form['content'][$key]['parent'] = array(
      '#type' => 'hidden',
      '#default_value' => $parent,
    );
    $form['content'][$key]['depth'] = array(
      '#type' => 'hidden',
      '#default_value' => $depth,
    );
    if ($row->useStylePerRow()) {
      $row_options = $row->getOptions($child->type);
      foreach ($row_options as $option_key => $option_form_item) {
        $form['content'][$key]['style'][$option_key] = $option_form_item;
        $form['content'][$key]['style'][$option_key]['#default_value'] = isset($child->style[$option_key]) ? $child->style[$option_key] : -1;
      }
    }
    $form['content'][$key]['operations'] = array(
      '#type' => 'markup',
      '#markup' => implode(' | ', $links),
    );
    if (count($child->getChildren())) {
      entity_collection_content_form_content($entity_collection, $form, $child, $child->key, $depth + 1);
    }
  }
}

function entity_collection_content_form_validate($form, &$form_state) {
  $values = &$form_state['values'];

  if ($values['style_uses_breakpoint']) {
    $items = &$values['content'];
    
    if ( $items['breakpoint']['depth'] != 0 ) {
      form_error($form['content']['breakpoint'], t('Breakpoint should never be nested!'));
      return;
    }

    // Calculate the real position of the breakpoint!
    $base_position = FALSE;
    array_walk($items, function(&$value, $key) use (&$base_position){
      if ( $value['depth'] == 0 ) {
        if($base_position === FALSE)
          $base_position = $value['position'];
        elseif ( $value['position'] < $base_position )
          $base_position = $value['position'];
      }
    });

    $values['breakpoint_position'] = $items['breakpoint']['position'] - $base_position;
    unset($items['breakpoint']);
  }
}
/**
 * 
 */
function entity_collection_content_form_get_break(&$destination, $position, $style_items) {
  $destination = array (
    '#content' => NULL,
    'title' => array (
      '#markup' => 'Breakpoint',
    ),
    'position' => array (
      '#type' => 'textfield',
      '#title' => 'Position',
      '#default_value' => $position,
    ),
    'parent' => array (
      '#type' => 'hidden',
      '#default_value' => 0,
    ),
    'depth' => array (
      '#type' => 'hidden',
      '#default_value' => 0,
    ),
    'style' => array(),
    'operations' => array('#markup' => ''),
  );
  
  foreach ($style_items as $key => $value) {
    $destination['style'][$key] = array(
      '#type' => 'hidden',
      '#title' => $key,
    );
  }
  return $destination;
}

/**
 * TODO: For now store the breakpoint as variable, but there has to be a better solution!
 */
function entity_collection_content_save_breakpoint($collection, $value='') {
  
}
/**
 * Submit function for the content form.
 */
function entity_collection_content_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  if ( EntityCollectionStyle::getStyle($form_state['entity_collection'])->useBreakpoint() ) {
    EntityCollectionStorage::getBundleStorage($form_state['entity_collection']->bundle)->saveBreakpoint($form_state['entity_collection'], $values['breakpoint_position'], $form_state['contexts']);
  }

  $tree = entity_collection_content_build_tree($form_state['entity_collection'], $values['content']);
  if (isset($form['#redirect'])) {
    $form_state['redirect'] = $form['#redirect'];
  }
  elseif (isset($_GET['contexts'])) {
    $form_state['redirect'] = array(current_path(), array('query' => array('contexts' => $_GET['contexts'])));
  }
  EntityCollectionStorage::getBundleStorage($form_state['entity_collection']->bundle)->save($form_state['entity_collection'], $tree, $form_state['contexts']);
}

/**
 * Build a tree from from a submitted form.
 */
function entity_collection_content_build_tree($entity_collection, $values) {
  $tree = new EntityCollectionTreeNode();
  uasort($values, 'entity_collection_content_compare');
  while (!empty($values)) {
    $value = array_shift($values);
    $entity = current(entity_load($value['type'], array($value['entity_id'])));
    if (!isset($value['style'])) {
      $row_plugin = EntityCollectionRow::getRow($entity_collection);
      $value['style'] = $row_plugin->getDefaultStyle($value['type'], $entity);
    }
    // If the we have no parent, we can insert the item on the root.
    if (empty($value['parent'])) {
      $tree->addChild($value['type'], $value['entity_id'], $entity, $value['style'], $value['key'], $value['position']);
    }
    // If we have a parent and that parent is already inserted,
    // add it to that node,
    elseif (!empty($value['parent']) && $parent = $tree->getChild($value['parent'])) {
      $parent->addChild($value['type'], $value['entity_id'], $entity, $value['style'], $value['key'], $value['position']);
    }
    // Push the value onto our array again and try again.
    else {
      array_push($values, $value);
    }
  }
  return $tree;
}

/**
 * Order all items in the order of position.
 */
function entity_collection_content_compare($a, $b) {
  return $a['position'] > $b['position'];
}

/**
 * Theme function for the content form.
 * @param type $variables
 *   Variables containing the form to render.
 */
function theme_entity_collection_content_form(&$variables) {
  $form = &$variables['form'];
  $rows = array();
  $output = '';

  if (isset($form['content'])) {
    // TODO: Render the entity using a custom view mode!
    foreach (element_children($form['content']) as $key) {
      $this_row = array();
      $current =& $form['content'][$key];

      $current['parent']['#attributes']['class'] = array('entity_collection-parent');
      $current['key']['#attributes']['class'] = array('key');
      $current['depth']['#attributes']['class'] = array('depth');
      $current['position']['#attributes']['class'] = array('position');

      $title = drupal_render($current['title']);
      $operations = drupal_render($current['operations']);
      $position = drupal_render($current['position']);

      $entity_info_themed = theme('entity_collection_entity_info', array('entity' => $current['#content']));

      $style = array();
      if ( !empty($current['style']) ) {
        $style_opts = element_children($current['style']);
        foreach ($current['style'] as $option => $item) {
          if (in_array($option, $style_opts)) {
            $fname = $item['#title'];
            unset($item['#title']);
            if($item['#type'] != 'value') {
              $style[$fname] = drupal_render($item);  
            }
          
          }
        }
        $current['style']['#printed'] = TRUE;
      }

      $rendered_key = drupal_render($current['key']);
      $parent       = drupal_render($current['parent']);
      $depth        = drupal_render($current['depth']);
      $indentation  = !empty($current['depth']['#default_value']) ? theme('indentation', array('size' => $current['depth']['#default_value'])) : '';

      $this_row['data'] = array(
        $indentation . $title . $parent . $depth . $rendered_key,
        $position,
        $entity_info_themed,
      );
      if ($form['#row']->useStylePerRow()) {
        foreach ($style as $key => $value) {
          $this_row['data'][] = $value;
        }
      }

      $this_row['data'][] = $operations;
      $this_row['class'][] = 'draggable';
      if ( !empty($current['#content']->type) ) {
        $this_row['class'][] = $current['#content']->type.'-entity';
      }

      // Add a class if the entity is published (if the entity does support it)
      if ( isset($current['#content']->content->status) ) {
        $this_row['class'][] = ($current['#content']->content->status) ? 'published' : 'not-published';
      }
      else if ( isset($current['#content']->content->published) ) {
        $this_row['class'][] = ($current['#content']->content->published) ? 'published' : 'not-published';
      }

      $rows[] = $this_row;
    }
    $header = array(t('Title'), t('Position'), t('Info'));

    if ($form['#row']->useStylePerRow()) {
      foreach ($style as $name => $value) {
        $header[] = $name;
      }
    }
    $header[] = t('Operations');
    $output = theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'attributes' => array('id' => 'entity_collection-table'),
      ));
    if ($form['#style']->getMaxDepth()) {

      drupal_add_tabledrag('entity_collection-table', 'match', 'parent', 'entity_collection-parent', 'entity_collection-parent', 'key', FALSE, $form['#style']->getMaxDepth());
      drupal_add_tabledrag('entity_collection-table', 'depth', 'group', 'depth', NULL, NULL, FALSE, $form['#style']->getMaxDepth());
    }
    drupal_add_tabledrag('entity_collection-table', 'order', 'sibling', 'position');
  }
  return $output . drupal_render_children($variables['form']);
}

/**
 * Delete a particular entity from a collection.
 */
function entity_collection_content_delete_form($form, &$form_state, $entity_collection, $entity_type, $eid) {
  $form_state['entity_collection'] = $entity_collection;
  $form_state['entity_type'] = $entity_type;
  $form_state['eid'] = $eid;
  return confirm_form($form, t('Are you sure you want to delete this content from the entity collection?'), 'admin/structure/entity_collection');
}

/**
 * Submit handler for entity_collection_content_delete_form.
 */
function entity_collection_content_delete_form_submit($form, &$form_state) {
  entity_collection_content_delete($form_state['entity_collection'], $form_state['entity_type'], array($form_state['eid']));
}

/**
 * A form for adding content to an entity collection.
 */
function entity_collection_content_add_form($form, &$form_state, $entity_collection, $collapsed = FALSE) {
  $form_state['entity_collection'] = $entity_collection;
  $form['#attributes'] = array('class' => array($entity_collection->name));
  if (module_exists('inline_entity_form')) {
    module_load_include('ief.inc', 'entity_collection');
    $form = entity_collection_ief_create_form($form, $entity_collection);
  }
  $form['add_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add existing content'),
  );
  if ($collapsed) {
    $form['add_content'] += array(
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
  }
  $form['add_content']['entity_type'] = array(
    '#type' => 'select',
    '#options' => entity_collection_get_entity_type_options($entity_collection),
    '#title' => t('Type'),
  );
  // We can't use the regular autocomplete,
  // because we need to send the selected entity type and the search.
  $form['add_content']['content_select'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#maxlength' => 60,
    '#description' => t('Type in the name of the content you want to add here.'),
    '#attributes' => array('class' => array('form-autocomplete')),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'entity_collection') . '/js/entity_autocomplete.js' => array(
          'type' => 'file',
        ),
        array(
          'data' => array(
            'entity_collection' => array(
              $entity_collection->name => array(
                'contexts' => _entity_collection_serialize_contexts($entity_collection),
                'path' => url('entity_collection/autocomplete/' . $entity_collection->name, array('absolute' => TRUE)),
              ),
            ),
          ),
          'type' => 'setting',
        ),
      ),
      'library' => array(
        array('system', 'drupal.autocomplete'),
      ),
    ),
  );
  $form['add_content']['reference'] = array(
    '#type' => 'references_dialog',
    '#format' => '$entity_type:$entity_id',
    '#attachable' => $entity_collection->name,
    '#target' => 'edit-content-select',
    '#operations' => array(),
  );
  $form['add_content']['add_content'] = array(
    '#type' => 'submit',
    '#value' => t('Add content'),
  );
  return $form;
}

/**
 * Validate that the new entry is permitted.
 */
function entity_collection_content_add_form_validate($form, &$form_state) {
  $entity_collection = $form_state['entity_collection'];
  list($type, $value) = explode(':', $form_state['values']['content_select']);
  $entity = entity_load_single($type, $value);
  list($entity_id,,$bundle) = entity_extract_ids($type, $entity);
  if (!in_array($bundle, $entity_collection->settings['allowed_bundles'][$type])) {
    form_set_error('content_select', t('The selected entitiy is not allowed in this collection'));
  }
  $content = EntityCollectionStorage::getBundleStorage($entity_collection->bundle)->retrieve($entity_collection);
  if ($content->getChild($form_state['values']['content_select'])) {
    form_set_error('content_select', t('This item already exists in the collection'));
  }
}

/**
 * Add form for adding entities to collections.
 */
function entity_collection_content_add_form_submit($form, &$form_state) {
  $entity_collection = $form_state['entity_collection'];
  $contexts = _entity_collection_get_contexts($entity_collection);
  if (!empty($form_state['values']['content_select'])) {
    list($type, $value) = explode(':', $form_state['values']['content_select']);
    $entities = entity_load($type, array($value));
    if (isset($entities[$value])) {
      $entity = $entities[$value];
      $item = new EntityCollectionTreeNode($type, $value, $entity);
      if (isset($_GET['contexts'])) {
        $form_state['redirect'] = array(current_path(), array('query' => array('contexts' => $_GET['contexts'])));
      }
      entity_collection_append_item($entity_collection, $item, $contexts);
    }
  }
}

/**
 * Autocomplete for entity collection content form.
 * @param type $string
 */
function entity_collection_content_form_autocomplete(EntityCollection $entity_collection, $entity_type, $contexts = '', $string = '') {
  // Let's limit this to nodes for now.
  $info = entity_get_info($entity_type);
  $loaded_contexts = array();

  if ($contexts != 'none') {
    // Convert contexts from URL to real ones.
    $contexts = explode(':', $contexts);
    ctools_include('context');
    foreach ($contexts as $i => $context) {
      $loaded_contexts[] = ctools_context_create($entity_collection->requiredcontexts[$i]['name'], $context);
    }
  }
  if (isset($info['entity keys']['label'])) {
    $label_property = $info['entity keys']['label'];
  }
  // Let's hardcode this for now until we figure out a nice way to do it.
  if ($entity_type == 'user') {
    $label_property = 'name';
  }
  // If we don't have a lable to search into, just return an empty result.
  $matches = array();
  if ( !empty($label_property) ) {
    $types = entity_collection_get_entity_type_options($entity_collection);
    $allowed_bundles = $entity_collection->settings['allowed_bundles'];
    $bundles = array();
    foreach ($allowed_bundles[$entity_type] as $bundle => $enabled) {
      if ($enabled) {
        $bundles[] = $bundle;
      }
    }
    // Don't output any results if we get an invalid input.
    if (!isset($types[$entity_type])) {
      drupal_json_output(array());
    }
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type)
      ->propertyCondition($label_property, db_like($string) . '%', 'LIKE');

    if ( isset($info['bundle keys']) ) {
      $query->entityCondition('bundle', $allowed_bundles[$entity_type]);
    }
  
    $result = $query->range(0, 10)->execute();
  
    $content = EntityCollectionStorage::getBundleStorage($entity_collection->bundle)->retrieve($entity_collection, $loaded_contexts);
    if (isset($result[$entity_type])) {
      $entities = entity_load($entity_type, array_keys($result[$entity_type]));
      foreach ($entities as $entity) {
        list($id) = entity_extract_ids($entity_type, $entity);
        $key = $entity_type . ':' . $id;
        if (entity_access('view', $entity_type, $entity) && !$content->getChild($key)) {
          $matches[$key] = check_plain(entity_label($entity_type, $entity));
        }
      }
    }
  }
  drupal_json_output($matches);
}
