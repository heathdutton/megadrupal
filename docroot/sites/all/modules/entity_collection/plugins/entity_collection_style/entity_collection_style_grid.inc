<?php

/**
 * @file
 * A hiearchy style plugin.
 */

/**
 * This style can show groups of content in hierarchies.
 */
class EntityCollectionStyleGrid extends EntityCollectionStyle {

  /**
   * Build the limited list.
   * @see EntityCollectionStyle::build().
   */
  public function build(EntityCollection$entity, EntityCollectionTreeNode &$tree, $langcode = NULL, $settings = array()) {
    $content = array();

    // Override defaults with minipanel settings.
    $settings = array_merge(array('limitation' => array(
          'offset' => 0,
          'length' => 0,
        )), $settings);

    
    if(!isset($tree->access_check)) { // Pop out elements if the user has not access to view
      foreach ($tree->list as $key => $item) {
        //$item_entity = entity_load_single($item->type, array($item->entity_id));
        $wrapper = entity_metadata_wrapper($item->type, $item->content, array('langcode'=>$langcode));
        if(!$wrapper->access('view')) {
          unset($tree->list[$key]);
        }
        $tree->access_check = TRUE;
      }
    }

    // Mystical way to handle this but it works in a way
    if (!isset($tree->processed_offset)) {
      $tree->processed_offset = 0;
    }

    $tree->end_of_row = 0;

    $list = $tree->getFlat($tree->processed_offset, $settings['limit']);
    $i = 0;
    foreach ($list as $key => $item) {
      $extra_classes = array();
      $item->grid_settings = $settings['grid']['options'][$i];
      $data = EntityCollectionRow::getRow($entity)->build($entity, $item, $langcode);

      if($tree->end_of_row == 0) {
        $tree->end_of_row = 0;
        $extra_classes[] = 'first-column';
      }
      $tree->end_of_row += $settings['grid']['options'][$i];
      
      if($tree->end_of_row >= 12) {
        $tree->end_of_row = 0;
        $extra_classes[] = 'last-column';
      }

      $extra_classes[] = variable_get('grid-item-prefix', 'span') . $settings['grid']['options'][$i];

      $content[$key] = array(
        'item' => $data,
        '#weight' => $i,
        'classes' => $extra_classes,
        '#theme' => 'entity_collection_style_grid'
      );
      $i++;
    }

    $tree->processed_offset += $settings['limit'];
    return $content;
  }

  /**
   * Return the minipanels options.
   */
  public function getOptions() {
    $mini_panels        = panels_mini_load_all();
    $available_contexts = array('entity_collection_item');
    $options            = array();

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

  public function settingsForm(&$form, $settings = array()) {
    // Use default settings in case of no settings and on the entity-collection admin page.
    if (count($settings) == 0) {
      $settings = $this->settings;
    }

    $form['limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Items to display'),
      '#description' => t('The number of items to display. Enter 0 for no limit.'),
      '#default_value' => $settings['limit'],
      '#attributes' => array(
        'class' => array('grid-settings-limit-elements'),
      ),
    );

    $form['grid'] = array(
      '#type' => 'fieldset',
      '#title' => t('Grid items'),
      '#description' => t('Here you can set the width of every element to create your grid.'),
      '#attributes' => array(
        'class' => array('grid-settings-elements-settings'),
      ),
    );

    for ($i = 0; $i < $settings['limit']; $i++) {
      $form['grid']['options'][$i] = array(
        '#type' => 'select',
        '#title' => t('Grid item @i', array('@i' => $i + 1)),
        '#options' => _entity_collection_grid_item_options(),
        '#default_value' => $settings['grid']['options'][$i],
      );
    }
  }

  public function formSettingsSubmit(&$settings) {}
}

function _entity_collection_grid_item_options() {
  $options = array();
  for ($i = 1; $i <= 12; $i++) {
    $options[$i] = 'Span ' . $i;
  }
  return $options;
}

/**
 * Preprocess function for this plugin.
 */
function template_preprocess_entity_collection_style_grid(&$variables) {
  $variables['item'] = $variables['element']['item'];
  $variables['classes_array'] = $variables['element']['classes'];
}
