<?php
/**
 * @file
 * Plugin to render rows with mini panels.
 */

/**
 * Use a mini panel to render a row.
 */
class EntityCollectionRowPanelsMini extends EntityCollectionRow {

  /**
   * @see EntityCollectionRowInterface::getOptions()
   */
  public function getOptions($entity_type) {
    $panels = array();
    if (isset($this->settings['allowed_panels'])) {
      foreach ($this->settings['allowed_panels'] as $name => $enabled) {
        if ($enabled) {
          $panel = panels_mini_load($name);
          foreach ($panel->requiredcontexts as $context) {
            if ($context['name'] == 'entity:' . $entity_type) {
              $panels[$name] = $panel->admin_title;
              break;
            }
          }
        }
      }
    }
    return $panels;
  }

  /**
   * Mini panels cannot be used without letting the user choose per panel.
   */
  public function useStylePerRow() {
    return TRUE;
  }

  /**
   * Get all available contexts.
   */
  protected function getContexts($item, $panel) {
    ctools_include('context');
    $contexts = array();
    $available_contexts = array();
    $available_contexts['entity:' . $item->type] = ctools_context_create('entity:' . $item->type, $item->content);
    $available_contexts['entity_collection_item'] = ctools_context_create('entity_collection_item', $item);
    if (isset($this->entity_collection)) {
      $available_contexts['entity:entity_collection'] = ctools_context_create('entity:entity_collection', $this->entity_collection);
      if (isset($this->entity_collection->contexts)) {
        foreach ($this->entity_collection->contexts as $context) {
          $available_contexts[$context->plugin] = $context;
        }
      }
    }
    foreach ($panel->requiredcontexts as $required) {
      if (isset($available_contexts[$required['name']])) {
        $contexts[] = $available_contexts[$required['name']];
      }
    }
    return $contexts;
  }

  /**
   * Act when a new row is inserted.
   */
  public function itemPreAdd(EntityCollectionTreeNode $item) {
    // Add a default style if style per row is not enabled.
    if (!isset($item->style)) {
      $item->style = current(array_keys($this->getOptions($item->type)));
    }
  }

  /**
   * Get all available options.
   */
  public function getAllOptions() {
    if (isset($this->entity_collection)) {
      $available_contexts = array();
      foreach ($this->entity_collection->settings['allowed_bundles'] as $type => $bundles) {
        if (count($bundles)) {
          $available_contexts[] = 'entity:' . $type;
        }
      }
    }
    else {
      $allowed_bundles = entity_collection_load_all();
      $available_contexts = array('entity:node');
      foreach($allowed_bundles as $collection) {
        foreach ($collection->settings['allowed_bundles'] as $type => $bundles) {
          if (count($bundles)) {
            $available_contexts[] = 'entity:' . $type;
          }
        }
      }
    }
    $mini_panels = panels_mini_load_all();
    if (isset($this->entity_collection)) {
      $available_contexts[] = 'entity:entity_collection';
    }
    foreach ($this->entity_collection->requiredcontexts as $context) {
      $available_contexts[] = $context['name'];
    }
    $options = array();

    foreach ($mini_panels as $name => $mini_panel) {
      if (count($mini_panel->requiredcontexts) >= 1) {
        $applicable = TRUE;
        foreach ($mini_panel->requiredcontexts as $requiredcontext) {
          if (!in_array($requiredcontext['name'], $available_contexts)) {
            $applicable = FALSE;
            break;
          }
        }
        if ($applicable) {
          $options[$name] = $mini_panel->admin_title;
        }
      }
    }
    return $options;
  }

  /**
   * @see EntityCollectionRowInterface::build()
   */
  public function build(EntityCollection $collection, EntityCollectionTreeNode $item, $langcode = NULL) {
    if (!isset($panels[$item->style])) {
      $panels[$item->style] = panels_mini_load($item->style);
    }
    $panel = $panels[$item->style];
    if (isset($panel)) {
      $context = ctools_context_match_required_contexts($panel->requiredcontexts, $this->getContexts($item, $panel));
      $panel->context = $panel->display->context = ctools_context_load_contexts($panel, FALSE, $context);
      $panel->display->owner = $panel;
      $panel->display->owner->id = $panel->name;
      return array(
        '#markup' => panels_render_display($panel->display),
      );
    }
    return array();
  }

  /**
   * Provide a settings form form for setting which mini panels are allowed.
   */
  public function settingsForm(&$form) {
    $types = array();
    $form['allowed_panels'] = array(
      '#type' => 'checkboxes',
      '#options' => $this->getAllOptions(),
      '#default_value' => isset($this->settings['allowed_panels']) ? $this->settings['allowed_panels'] : array(),
      '#title' => t('Allowed mini panels'),
    );
  }
}
