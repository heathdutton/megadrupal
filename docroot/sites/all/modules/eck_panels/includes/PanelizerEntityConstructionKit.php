<?php

/**
 * @file
 * Class for the Panelizer ECK entity plugin.
 */

/**
 * Panelizer entity ECK plugin class.
 */
class PanelizerEntityConstructionKit extends PanelizerEntityDefault {

  public $supports_revisions = FALSE;
  public $uses_page_manager = TRUE;

  /**
   * {@inheritdoc}
   */
  function init($plugin) {
    parent::init($plugin);
    $this->views_table = "eck_$this->entity_type";
  }

  /**
   * {@inheritdoc}
   */
  public function entity_access($op, $entity) {
    return entity_access($op, $this->entity_type, $entity);
  }

  /**
   * {@inheritdoc}
   */
  public function entity_save($entity) {
    entity_save($this->entity_type, $entity);
  }

  /**
   * {@inheritdoc}
   */
  function entity_base_url($entity, $view_mode = NULL) {
    list($entity_id, , $bundle) = entity_extract_ids($this->entity_type, $entity);
    $base_url = "$this->entity_type/$bundle/$entity_id/panelizer";
    $base_url = $view_mode ? "$base_url/$view_mode" : $base_url;
    return $base_url;
  }

  /**
   * {@inheritdoc}
   */
  function wrap_default_panelizer_pages($bundle, $output) {
    list($bundle, $view_mode) = explode('.', $bundle);
    $base_url = "admin/structure/entity-type/$this->entity_type/$bundle/panelizer/$view_mode";
    return $this->make_fake_tabs($base_url, $bundle, $view_mode, $output);
  }

  /**
   * {@inheritdoc}
   */
  public function hook_menu(&$items) {
    $entity_info = entity_get_info($this->entity_type);
    foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
      $this->plugin['entity path'] = "$this->entity_type/$bundle/%eckentity";
      parent::hook_menu($items);

      // Add admin links.
      $this->add_admin_links("admin/structure/entity-type/$this->entity_type/$bundle", $bundle, $items);
    }

    unset($this->plugin['entity path']);
  }


  // Entity specific Drupal hooks
  public function hook_entity_load(&$entities) {
    ctools_include('export');
    $ids = array();
    $vids = array();
    $bundles = array();

    foreach ($entities as $entity) {
      // Don't bother if somehow we've already loaded and are asked to
      // load again.
      if (!empty($entity->panelizer)) {
        continue;
      }

      list($entity_id, $revision_id, $bundle) = entity_extract_ids($this->entity_type, $entity);
      if ($this->is_panelized($bundle)) {
        $ids[] = $entity_id;
        if ($this->supports_revisions) {
          $vids[] = $revision_id;
        }
        $bundles[$entity_id] = $bundle;
      }
    }

    if (!$ids) {
      return;
    }

    // Load all the panelizers associated with the list of entities.
    if ($this->supports_revisions) {
      $result = db_query("SELECT * FROM {panelizer_entity} WHERE entity_type = :entity_type AND entity_id IN (:ids) AND revision_id IN (:vids)", array(':entity_type' => $this->entity_type, ':ids' => $ids, ':vids' => $vids));
    }
    else {
      $result = db_query("SELECT * FROM {panelizer_entity} WHERE entity_type = :entity_type AND entity_id IN (:ids)", array(':entity_type' => $this->entity_type, ':ids' => $ids));
    }

    $panelizers = array();
    while ($panelizer = $result->fetchObject()) {
      $panelizers[$panelizer->entity_id][$panelizer->view_mode] = $panelizer;
    }

    $defaults = array();
    $dids = array();
    // Go through our entity list and generate a list of defaults and displays
    foreach ($entities as $entity_id => $entity) {
      // Don't bother if somehow we've already loaded and are asked to
      // load again.
      if (!empty($entity->panelizer)) {
        continue;
      }

      // Skip not panelized bundles.
      if (empty($bundles[$entity_id])) {
        continue;
      }

      // Check for each view mode.
      foreach ($this->plugin['view modes'] as $view_mode => $view_mode_info) {
        // Load the default display for this entity bundle / view_mode.
        $name = $this->get_default_display_name($bundles[$entity_id], $view_mode);

        // If no panelizer was loaded for the view mode, queue up defaults.
        if (empty($panelizers[$entity_id][$view_mode]) && $this->has_default_panel($bundles[$entity_id] . '.' . $view_mode)) {
          $defaults[$name] = $name;
        }
        // Otherwise unpack the loaded panelizer.
        else if (!empty($panelizers[$entity_id][$view_mode])) {
          $entity->panelizer[$view_mode] = ctools_export_unpack_object('panelizer_entity', $panelizers[$entity_id][$view_mode]);
          // If somehow we have no name AND no did, fill in the default.
          // This can happen if use of defaults has switched around maybe?
          if (empty($entity->panelizer[$view_mode]->did) && empty($entity->panelizer[$view_mode]->name)) {
            if ($this->has_default_panel($bundles[$entity_id] . '.' . $view_mode)) {
              $entity->panelizer[$view_mode]->name = $name;
            }
            else {
              // With no default, did or name, this doesn't actually exist.
              unset($entity->panelizer[$view_mode]);
              list($entity_id, $revision_id, $bundle) = entity_extract_ids($this->entity_type, $entity);

              db_delete('panelizer_entity')
                ->condition('entity_type', $this->entity_type)
                ->condition('entity_id', $entity_id)
                ->condition('revision_id', $revision_id)
                ->condition('view_mode', $view_mode)
                ->execute();
              continue;
            }
          }

          // Panelizers that do not have dids are just a selection of defaults
          // that has never actually been modified.
          if (empty($entity->panelizer[$view_mode]->did) && !empty($entity->panelizer[$view_mode]->name)) {
            $defaults[$entity->panelizer[$view_mode]->name] = $entity->panelizer[$view_mode]->name;
          }
          else {
            $dids[$entity->panelizer[$view_mode]->did] = $entity->panelizer[$view_mode]->did;
          }
        }

        list(, , $bundle) = entity_extract_ids($this->entity_type, $entity);
        $path = "$this->entity_type/$bundle/%eckentity";
        $items[$path . '/panelizer/' . $view_mode . '/content']['type'] = MENU_DEFAULT_LOCAL_TASK;
        if ($this->supports_revisions) {
          $items[$path . '/revisions/%/panelizer/' . $view_mode . '/content']['type'] = MENU_DEFAULT_LOCAL_TASK;
        }
      }
    }
    ksort($items);

    // Load any defaults we collected.
    if ($defaults) {
      $panelizer_defaults = $this->load_default_panelizer_objects($defaults);
    }

    // if any panelizers were loaded, get their attached displays.
    if ($dids) {
      $displays = panels_load_displays($dids);
    }

    // Now, go back through our entities and assign dids and defaults
    // accordingly.
    foreach ($entities as $entity_id => $entity) {
      // Skip not panelized bundles.
      if (empty($bundles[$entity_id])) {
        continue;
      }

      // Reload these.
      list(, $revision_id, $bundle) = entity_extract_ids($this->entity_type, $entity);

      // Check for each view mode.
      foreach ($this->plugin['view modes'] as $view_mode => $view_mode_info) {
        if (empty($entity->panelizer[$view_mode])) {
          // Load the configured default display.
          $default_value = $this->get_default_display_name($bundle, $view_mode);

          if (!empty($panelizer_defaults[$default_value])) {
            $entity->panelizer[$view_mode] = clone $panelizer_defaults[$default_value];
            // Make sure this entity can't write to the default display.
            $entity->panelizer[$view_mode]->did = NULL;
            $entity->panelizer[$view_mode]->entity_id = 0;
            $entity->panelizer[$view_mode]->revision_id = 0;
          }
        }
        elseif (empty($entity->panelizer[$view_mode]->display) || empty($entity->panelizer[$view_mode]->did)) {
          if (!empty($entity->panelizer[$view_mode]->did)) {
            if (empty($displays[$entity->panelizer[$view_mode]->did])) {
              // Somehow the display for this entity has gotten lost?
              $entity->panelizer[$view_mode]->did = NULL;
              $entity->panelizer[$view_mode]->display = $this->get_default_display($bundles[$entity_id], $view_mode);
            }
            else {
              $entity->panelizer[$view_mode]->display = $displays[$entity->panelizer[$view_mode]->did];
            }
          }
          else {
            if (!empty($panelizer_defaults[$entity->panelizer[$view_mode]->name])) {
              // Reload the settings from the default configuration.
              $entity->panelizer[$view_mode] = clone $panelizer_defaults[$entity->panelizer[$view_mode]->name];
              $entity->panelizer[$view_mode]->did = NULL;
              $entity->panelizer[$view_mode]->entity_id = $entity_id;
              $entity->panelizer[$view_mode]->revision_id = $revision_id;
            }
          }
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function settings_form(&$form, &$form_state) {
    parent::settings_form($form, $form_state);

    foreach ($this->plugin['bundles'] as $info) {
      if (!empty($info['status']) && !empty($info['view modes']['page_manager']['status'])) {
        $entity_info = entity_get_info($this->entity_type);
        $task = page_manager_get_task('eck_view');
        $subtask = page_manager_get_task_subtask($task, $this->entity_type);

        if (!empty($subtask['disabled'])) {
          drupal_set_message(t('The @entity_type template page is currently not enabled in page manager. You must enable this for Panelizer to be able to panelize @entity_type using the "Full page override" view mode.', array(
            '@entity_type' => $entity_info['label'],
          )), 'warning');
        }

        $handler = page_manager_load_task_handler($task, '', 'nainuwa_metadata_view_panelizer');
        if (!empty($handler->disabled)) {
          drupal_set_message(t('The panelizer variant on the @entity_type template page is currently not enabled in page manager. You must enable this for Panelizer to be able to panelize @entity_type using the "Full page override" view mode.', array(
            '@entity_type' => $entity_info['label'],
          )), 'warning');
        }

        break;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function hook_default_page_manager_handlers(&$handlers) {
    $handler = new stdClass;
    $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
    $handler->api_version = 1;
    $handler->name = "eck_view_{$this->entity_type}_panelizer";
    $handler->task = 'eck_view';
    $handler->subtask = $this->entity_type;
    $handler->handler = 'panelizer_node';
    $handler->weight = -100;
    $handler->conf = array(
      'title' => t('Entity Construction Kit Panelizer'),
      'context' => "argument_entity_id:{$this->entity_type}_1",
      'access' => array(),
    );
    $handlers["eck_view_{$this->entity_type}_panelizer"] = $handler;

    return $handlers;
  }

  /**
   * {@inheritdoc}
   */
  public function hook_form_alter(&$form, &$form_state, $form_id) {
    if ($form_id == 'eck__bundle__edit_form' && $this->entity_type == $form_state['build_info']['args'][0]){
      $bundle = $form_state['build_info']['args'][1];
      $this->add_bundle_setting_form($form, $form_state, $bundle, array('type'));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function get_entity_view_entity($build) {
    $element = '#entity';
    if (isset($build[$element])) {
      return $build[$element];
    }
  }

  /**
   * {@inheritdoc}
   */
  public function preprocess_panelizer_view_mode(&$vars, $entity, $element, $panelizer, $info) {
    list($entity_id, $revision_id, $bundle) = entity_extract_ids($this->entity_type, $entity);
    $this->plugin['entity path'] = "$this->entity_type/$bundle/%eckentity";
    parent::preprocess_panelizer_view_mode($vars, $entity, $element, $panelizer, $info);
    unset($this->plugin['entity path']);
  }
}
