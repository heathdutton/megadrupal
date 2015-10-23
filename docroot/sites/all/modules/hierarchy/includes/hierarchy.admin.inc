<?php

/**
 * @file
 * Hierarchy type and hierarchy editing UI.
 */

/**
 * Hierarchy type UI controller.
 */
class HierarchyTypeUIController extends EntityDefaultUIController {
}

/**
 * Hierarchy UI controller.
 */
class HierarchyUIController extends EntityDefaultUIController {

  public function hook_menu() {
    $items = parent::hook_menu();

    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';

    $file_path = isset($this->entityInfo['admin ui']['file path']) ? $this->entityInfo['admin ui']['file path'] : drupal_get_path('module', $this->entityInfo['module']);

    $defaults = array(
      'file' => $this->entityInfo['admin ui']['file'],
      'file path' => $file_path,
    );

    // Add "view hierarchy" menu item.
    $items[$this->path . '/' . $wildcard] = array(
      'title callback' => 'entity_ui_get_page_title',
      'title arguments' => array('view', $this->entityType, $this->id_count),
      'page callback' => 'entity_ui_entity_page_view',
      'page arguments' => array($this->id_count),
      'load arguments' => array($this->entityType),
      'access callback' => 'entity_access',
      'access arguments' => array('view', $this->entityType, $this->id_count),
    ) + $defaults;

    $items[$this->path . '/' . $wildcard . '/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'load arguments' => array($this->entityType),
      'weight' => -10,
    ) + $defaults;

    // Hierarchies are created automatically or in code.
    unset($items[$this->path . '/add']);

    // Hierarchies can not be cloned.
    unset($items[$this->path . '/manage/' . $wildcard . '/clone']);

    return $items;
  }

}

/**
 * Generates the hierarchy type editing form.
 */
function hierarchy_type_form($form, &$form_state, HierarchyType $hierarchy_type, $op = 'edit') {
  if ($op === 'clone') {
    $hierarchy_type->name .= ' (cloned)';
    $form_state['original_hierarchy_type'] = menu_get_object('entity_object', 5);
  }

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $hierarchy_type->name,
    '#description' => t('The human-readable name of this hierarchy type.'),
    '#required' => TRUE,
    '#weight' => -5,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($hierarchy_type->type) ? $hierarchy_type->type : '',
    '#disabled' => $hierarchy_type->hasStatus(ENTITY_IN_CODE),
    '#machine_name' => array(
      'exists' => 'hierarchy_type_load',
      'source' => array('name'),
    ),
    '#description' => t('A unique machine-readable name for this hierarchy type. It must only contain lowercase letters, numbers, and underscores.'),
    '#weight' => -4,
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => $hierarchy_type->description,
    '#description' => t('The description for this hierarchy type.'),
    '#required' => FALSE,
    '#weight' => -3,
  );

  if (($op === 'add') || ($op === 'clone')) {
    $entity_type_options = array();
    $entity_type_options[''] = t('Any entity type');

    foreach (entity_get_info() as $entity_type => $entity_info) {
      if (!empty($entity_info['bundle of'])) {
        continue;
      }
      // @todo Filter entities with numeric identifiers only.
      $entity_type_options[$entity_type] = $entity_info['label'];
    }

    $form['entity_type'] = array(
      '#title' => t('Entity type of hierarchy items'),
      '#type' => 'select',
      '#options' => $entity_type_options,
      '#default_value' => $hierarchy_type->entity_type,
      '#description' => t('Select the entity type of hierarchy items or chose "Any entity type" for unrestricted hierarchies.'),
      '#required' => FALSE,
      '#weight' => -2,
    );
  }
  else {
    $value = t('Any entity type');
    if (!empty($hierarchy_type->entity_type)) {
      $info = entity_get_info($hierarchy_type->entity_type);
      $value = !empty($info['label']) ? check_plain($info['label']) : check_plain($hierarchy_type->entity_type);
    }

    $form['entity_type'] = array(
      '#title' => t('Entity type of hierarchy items'),
      '#type' => 'item',
      '#markup' => $value,
      '#weight' => -2,
    );
  }

  $form['is_unique'] = array(
    '#title' => t('Hierarchy is unique (exactly one hierarchy of this type exists)'),
    '#type' => 'checkbox',
    '#default_value' => $hierarchy_type->is_unique,
    '#description' => t('You should keep this checked unless you need an advanced hierarchy system, e.g. one hierarchy per every node of some type.'),
    '#disabled' => TRUE,
    '#weight' => -1,
  );

  $form['is_ordered'] = array(
    '#title' => t('Hierarchy is ordered'),
    '#type' => 'checkbox',
    '#default_value' => $hierarchy_type->is_ordered,
    '#description' => t('In ordered hierarchy, sibling items have weight value (0, 1, 2, ...) which allows to sort them.'),
    '#disabled' => TRUE,
    '#weight' => 0,
  );

  if ($op === 'add') {
    $form['is_unique']['#default_value'] = TRUE;
    $form['is_unique']['#disabled'] = FALSE;

    $form['is_ordered']['#default_value'] = TRUE;
    $form['is_ordered']['#disabled'] = FALSE;
  }

  if ($op === 'clone') {
    $form['is_unique']['#disabled'] = FALSE;
    $form['is_ordered']['#disabled'] = FALSE;
  }

  if ($op === 'edit') {
    $form['type']['#disabled'] = TRUE;
  }

  // Placeholder for other module to add their settings,
  // that should be added to the data column.
  $form['data'] = array('#tree' => TRUE);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save hierarchy type'),
    '#weight' => 40,
  );

  if (!$hierarchy_type->hasStatus(ENTITY_IN_CODE) && $op === 'edit') {
    // @todo Check hierarchy existence.
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete hierarchy type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('hierarchy_type_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Form API submit callback for the hierarchy type form.
 */
function hierarchy_type_form_submit(&$form, &$form_state) {
  $op = $form_state['build_info']['args'][1];
  $hierarchy_type = entity_ui_form_submit_build_entity($form, $form_state);
  if ($op === 'clone') {
    // Regenerate UUID for the cloned entity.
    $hierarchy_type->uuid = NULL;
  }
  $hierarchy_type->save();

  $form_state['redirect'] = 'admin/structure/hierarchy/type';
}

/**
 * Form API submit callback for the delete button.
 */
function hierarchy_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/hierarchy/type/manage/' . $form_state['hierarchy_type']->type . '/delete';
}

/**
 * Generates the hierarchy editing form.
 */
function hierarchy_form($form, &$form_state, $hierarchy, $op = 'edit') {
  if ($op === 'clone') {
    throw new Exception('Hierarchy can not be cloned.');
  }

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $hierarchy->name,
    '#description' => t('The human-readable name of this hierarchy.'),
    '#required' => TRUE,
    '#weight' => -5,
  );

  field_attach_form('hierarchy', $hierarchy, $form, $form_state);

  // Placeholder for other module to add their settings,
  // that should be added to the data column.
  $form['data'] = array('#tree' => TRUE);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save hierarchy'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API validation callback for the hierarchy form.
 */
function hierarchy_form_validate($form, $form_state) {
  field_attach_form_validate('hierarchy', $form_state['hierarchy'], $form, $form_state);
}

/**
 * Form API submit callback for the hierarchy form.
 */
function hierarchy_form_submit(&$form, &$form_state) {
  $hierarchy = entity_ui_form_submit_build_entity($form, $form_state);
  $hierarchy->save();

  $form_state['redirect'] = 'admin/structure/hierarchy';
}

/**
 * Menu callback for hierarchy relation types administration page.
 */
function hierarchy_relations_overview() {
  $instances = field_info_instances();
  $bundles = field_info_bundles();
  $header = array(
    t('Field name'),
    t('Used in'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );
  $rows = array();

  foreach ($instances as $entity_type => $type_bundles) {
    foreach ($type_bundles as $bundle => $bundle_instances) {
      foreach ($bundle_instances as $field_name => $instance) {
        $field = field_info_field($field_name);
        if ($field['type'] == 'hierarchy_position') {
          $admin_path = _field_ui_bundle_admin_path($entity_type, $bundle);
          $rows[$field_name]['class'] = $field['locked'] ? array('menu-disabled') : array('');

          $rows[$field_name]['data'][0] = $field['locked'] ? t('@field_name (Locked)', array('@field_name' => $field_name)) : $field_name;
          $rows[$field_name]['data'][1][] = l($bundles[$entity_type][$bundle]['label'], $admin_path . '/fields');
        }
      }
    }
  }

  foreach ($rows as $field_name => $cell) {
    $rows[$field_name]['data'][1] = implode(', ', $cell['data'][1]);

    $field_name_url_str = strtr($field_name, array('_' => '-'));
    $rows[$field_name]['data'][2] = l(t('manage fields'), 'admin/structure/hierarchy/relation/manage/' . $field_name_url_str . '/fields');
    $rows[$field_name]['data'][3] = l(t('manage display'), 'admin/structure/hierarchy/relation/manage/' . $field_name_url_str . '/display');
  }

  if (empty($rows)) {
    $output = t('No hierarchy relation fields have been defined yet.');
  }
  else {
    ksort($rows);
    $output = theme('table', array('header' => $header, 'rows' => $rows));
  }

  return $output;
}

function hierarchy_build_item_tree_content($hierarchy, $langcode) {
  // Add "hierarchy-items" class.
  return array();
}

function hierarchy_reorder_items_build_form(&$form, &$form_state, $hierarchy, $langcode = NULL) {
  $tree = hierarchy_get_item_tree($hierarchy, $langcode);
  $tree = hierarchy_flatten_item_tree($tree);

  $form['#tree'] = TRUE;
  $form['#hierarchy'] = $hierarchy->hid;
  $form['#language'] = $langcode;
  $form['#prefix'] = '<div class="hierarchy-items">';
  $form['#suffix'] = '</div>';

  $delta = intval(count($tree) / 2) + 1;
  $weight = -$delta;

  foreach ($tree as $item) {
    $form[$item->entity_type . '-' . $item->entity_id] = array(
      '#item' => (array) $item,
      'title' => array(
        '#markup' => !empty($item->entity_uri) ? l($item->entity_label, $item->entity_uri) : check_plain($item->entity_label),
      ),
      'entity_key' => array(
        '#type' => 'hidden',
        '#value' => $item->entity_key,
      ),
      'parent_key' => array(
        '#type' => 'hidden',
        '#default_value' => isset($item->parent_key) ? $item->parent_key : 0,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#delta' => $delta,
        '#default_value' => $weight,
        '#title_display' => 'invisible',
        '#title' => t('Weight for @title', array('@title' => $item->entity_label)),
      ),
    );
    $weight++;
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save hierarchy item order'),
  );
}

function hierarchy_reorder_items_form_submit($form, &$form_state) {
  $hierarchy = hierarchy_load($form['#hierarchy']);
  $langcode = $form['#language'];
  $hierarchy_items = hierarchy_get_items($hierarchy, $langcode);
  $items_by_parent = array();
  $values = $form_state['values'];

  foreach ($hierarchy_items as $item) {
    $item->old_weight = intval($item->weight);
    $item->old_parent_key = $item->parent_key;
    $form_key = $item->entity_type . '-' . $item->entity_id;
    if (isset($values[$form_key])) {
      $parent_key = $values[$form_key]['parent_key'];
      if (empty($parent_key)) {
        $parent_key = NULL;
      }
      if (is_null($parent_key) || isset($values[str_replace(':', '-', $parent_key)])) {
        $item->parent_key = $parent_key;
        $item->weight = $values[$form_key]['weight'];
      }
    }
    $items_by_parent[$item->parent_key][] = $item;
  }

  foreach ($items_by_parent as $items) {
    usort($items, 'hierarchy_item_tree_sort_by_weight_and_label');
    $weight = 1;
    foreach ($items as $item) {
      $item->weight = $weight;
      $weight++;
    }
  }

  foreach ($hierarchy_items as $item) {
    $parent_entity_type = NULL;
    $parent_entity_id = NULL;
    if (!empty($item->parent_key)) {
      list($parent_entity_type, $parent_entity_id) = explode(':', $item->parent_key);
    }
    if ($item->parent_key !== $item->old_parent_key) {
      hierarchy_reorder_items_update_entity($item->entity_type, $item->entity_id, $hierarchy, $item->field_name, $parent_entity_type, $parent_entity_id, $langcode);
    }
    if (($item->parent_key !== $item->old_parent_key) || ($item->weight !== $item->old_weight)) {
      hierarchy_reorder_items_update_entity_weight($item->entity_type, $item->entity_id, $hierarchy, $item->field_name, $parent_entity_type, $parent_entity_id,
        $item->weight, $langcode);
    }
  }

  drupal_set_message(t('Hierarchy is saved.'));
}

function hierarchy_reorder_items_update_entity($entity_type, $entity_id, $hierarchy, $field_name, $parent_entity_type, $parent_entity_id, $langcode) {
  $entity = entity_load_single($entity_type, $entity_id);
  if (empty($entity)) {
    return;
  }

  $langcode = field_language($entity_type, $entity, $field_name, $langcode);
  if ($langcode === FALSE) {
    $langcode = LANGUAGE_NONE;
  }

  if (!empty($entity->{$field_name}[$langcode])) {
    foreach ($entity->{$field_name}[$langcode] as &$value) {
      if ($value['hid'] == $hierarchy->hid) {
        $value['parent_type'] = $parent_entity_type;
        $value['parent_id'] = $parent_entity_id;
        entity_save($entity_type, $entity);
        return;
      }
    }

    $entity->{$field_name}[$langcode][] = array(
      'hid' => $hierarchy->hid,
      'parent_type' => $parent_entity_type,
      'parent_id' => $parent_entity_id,
    );
    entity_save($entity_type, $entity);
    return;
  }

  $entity->{$field_name} = array(
    $langcode => array(
      0 => array(
        'hid' => $hierarchy->hid,
        'parent_type' => $parent_entity_type,
        'parent_id' => $parent_entity_id,
      ),
    ),
  );
  entity_save($entity_type, $entity);
}

function hierarchy_reorder_items_update_entity_weight($entity_type, $entity_id, $hierarchy, $field_name, $parent_entity_type, $parent_entity_id, $weight, $langcode) {
  $entity = entity_load_single($entity_type, $entity_id);
  if (empty($entity)) {
    return;
  }

  $langcode = field_language($entity_type, $entity, $field_name, $langcode);
  if ($langcode === FALSE) {
    $langcode = LANGUAGE_NONE;
  }

  if (!empty($entity->{$field_name}[$langcode])) {
    foreach ($entity->{$field_name}[$langcode] as &$value) {
      if (($value['hid'] == $hierarchy->hid) && ($value['parent_type'] == $parent_entity_type) && ($value['parent_id'] == $parent_entity_id)) {
        $relation = hierarchy_relation_load($value['relation_id']);
        if ($relation) {
          $relation->weight = $weight;
          $relation->save();
        }
        return;
      }
    }
  }
}

function hierarchy_flatten_item_tree($tree, $level = 1) {
  $result = array();

  foreach ($tree as $item) {
    $item->level = $level;
    $children = array();
    if (!empty($item->children)) {
      $children = hierarchy_flatten_item_tree($item->children, $level + 1);
    }

    $result[] = $item;
    if (!empty($children)) {
      foreach ($children as $child) {
        $result[] = $child;
      }
    }
  }

  return $result;
}

function hierarchy_get_items($hierarchy, $langcode = NULL) {
  $hierarchy_items = array();
  $relation_ids = array();

  foreach (field_read_fields(array('type' => 'hierarchy_position')) as $hierarchy_position_field_name => $field) {
    $query = new EntityFieldQuery();
    $query->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');
    // @todo Add language conditions.
    $query->fieldCondition($hierarchy_position_field_name, 'hid', $hierarchy->hid);
    $result = $query->execute();

    // @todo Add multiple parent check.
    if (!empty($result)) {
      foreach ($result as $entity_type => $type_entities) {
        $entities = entity_load($entity_type, array_keys($type_entities));
        foreach ($entities as $entity_id => $entity) {
          $entity_key = $entity_type . ':' . $entity_id;
          $entity_uri = entity_uri($entity_type, $entity);
          $entity_label = entity_label($entity_type, $entity);
          $parent_type = NULL;
          $parent_id = NULL;

          $items = field_get_items($entity_type, $entity, $hierarchy_position_field_name);
          if (!empty($items)) {
            foreach ($items as $item) {
              if ($item['hid'] == $hierarchy->hid) {
                $relation_ids[$entity_key] = $item['relation_id'];
                $parent_type = $item['parent_type'];
                $parent_id = $item['parent_id'];
              }
            }
          }

          $hierarchy_items[$entity_key] = (object) array(
            'entity_type' => $entity_type,
            'entity_id' => $entity_id,
            'entity_key' => $entity_key,
            'entity_label' => $entity_label,
            'entity_label_upper' => drupal_strtoupper($entity_label),
            'entity_uri' => (!empty($entity_uri)) ? $entity_uri['path'] : '',
            'field_name' => $hierarchy_position_field_name,
            'parent_type' => $parent_type,
            'parent_id' => $parent_id,
            'parent_key' => (isset($parent_type) && isset($parent_id)) ? ($parent_type . ':' . $parent_id) : NULL,
            'weight' => 0,
            'children' => array(),
          );
        }
      }
    }
  }

  $relations = entity_load('hierarchy_relation', $relation_ids);
  foreach ($relation_ids as $key => $relation_id) {
    if (isset($relations[$relation_id])) {
      $hierarchy_items[$key]->weight = $relations[$relation_id]->weight;
    }
  }

  return $hierarchy_items;
}

function hierarchy_get_item_tree($hierarchy, $langcode = NULL) {
  $hierarchy_items = hierarchy_get_items($hierarchy, $langcode);
  $root_items = array();

  foreach ($hierarchy_items as $entity_key => $item) {
    $parent_item = NULL;
    if ((!empty($item->parent_type)) && (!empty($item->parent_type))) {
      $parent_key = $item->parent_type . ':' . $item->parent_id;
      if (isset($hierarchy_items[$parent_key])) {
        $parent_item = $hierarchy_items[$parent_key];
      }
    }
    if (empty($parent_item)) {
      $root_items[] = $item;
    }
    else {
      $parent_item->children[] = $item;
    }
  }

  hierarchy_item_tree_sort_one_level($root_items);

  return $root_items;
}

function hierarchy_item_tree_sort_one_level(&$items) {
  usort($items, 'hierarchy_item_tree_sort_by_weight_and_label');
  foreach ($items as &$item) {
    if (!empty($item->children)) {
      hierarchy_item_tree_sort_one_level($item->children);
    }
  }
}

function hierarchy_item_tree_sort_by_weight_and_label($a, $b) {
  if ($a->weight > $b->weight) {
    return 1;
  }
  if ($a->weight < $b->weight) {
    return -1;
  }
  $label_compared = strcmp($a->entity_label_upper, $b->entity_label_upper);
  if ($label_compared !== 0) {
    return $label_compared;
  }
  if ($a->entity_key > $b->entity_key) {
    return 1;
  }
  if ($a->entity_key < $b->entity_key) {
    return -1;
  }
  return 0;
}

function theme_hierarchy_reorder_items_form($variables) {
  $form = $variables['form'];
  drupal_add_tabledrag('hierarchy-reorder-items', 'match', 'parent', 'hierarchy-parent', 'hierarchy-parent', 'hierarchy-key', TRUE);
  drupal_add_tabledrag('hierarchy-reorder-items', 'order', 'sibling', 'hierarchy-weight');

  $header = array(
    t('Title'),
    t('Weight'),
  );

  $rows = array();

  foreach (element_children($form) as $item) {
    if (isset($form[$item]['entity_key'])) {
      $element = &$form[$item];
      $element['entity_key']['#attributes']['class'] = array('hierarchy-key');
      $element['parent_key']['#attributes']['class'] = array('hierarchy-parent');
      $element['weight']['#attributes']['class'] = array('hierarchy-weight');
      $rows[] = array(
        'data' => array(
          theme('indentation', array('size' => $element['#item']['level'] - 1)) . drupal_render($element['title']),
          drupal_render($element['weight']) . drupal_render($element['parent_key']) . drupal_render($element['entity_key']),
        ),
        'class' => array('draggable'),
      );
    }
  }

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'hierarchy-reorder-items',
    ),
  ));

  $output .= drupal_render_children($form);

  return $output;
}
