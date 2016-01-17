<?php
/**
 * @file
 * A hiearchy style plugin.
 */

/**
 * This style can show groups of content in hierarchies.
 */
class EntityCollectionStyleLimit extends EntityCollectionStyle {

  /**
   * Build the limited list.
   * @see EntityCollectionStyle::build().
   */
  public function build(EntityCollection $entity, EntityCollectionTreeNode &$tree, $langcode = NULL, $settings = array()) {
    $content = array();
    // Override defaults with minipanel settings.
    $settings = array_merge(array('limitation' => array(
      'offset' => 0,
      'length' => 0,
    )), $settings);

    // Render content with a mini panel.
    if ($settings['rendering']) {
      $minipanel = panels_mini_load($settings['rendering']);
      if (isset($minipanel)) {
        $tree = clone $tree;
        $tree->entity_id = $entity->name;
        $tree->splice($settings['limitation']['offset'], $settings['limitation']['length']);
        $contexts = array('entity_collection_item' => ctools_context_create('entity_collection_item', $tree));
        $context = ctools_context_match_required_contexts($minipanel->requiredcontexts, $contexts);
        $minipanel->context = $minipanel->display->context = ctools_context_load_contexts($minipanel, FALSE, $context);
        $minipanel->display->owner = $minipanel;
        $minipanel->display->owner->id = $minipanel->name;
        $content['content'] = array(
          '#markup' => panels_render_display($minipanel->display),
        );
      }
    }
    else {
      $list = $tree->getFlat($settings['limitation']['offset'], $settings['limitation']['length']);
      $i = 0;
      foreach ($list as $key => $item) {
        $data = EntityCollectionRow::getRow($entity)->build($entity, $item, $langcode);
        $content[$key] = array(
          'item' => $data,
          '#weight' => $i,
        );
        $i++;
      }
    }

    return $content;
  }

  /**
   * Return the minipanels options.
   */
  public function getOptions() {
    $mini_panels = panels_mini_load_all();
    $available_contexts = array('entity_collection_item');
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

  public function settingsForm(&$form, $settings = array()) {
    // Use default settings in case of no settings and on the entity-collection admin page.
    if (count($settings) == 0) {
      $settings = $this->settings;
    }
    $form['limit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display a specified number of items'),
      '#default_value' => $settings['limitation']['length'] + $settings['limitation']['offset'] != 0,
    );
    $form['limitation'] = array(
      '#type' => 'fieldset',
      '#dependency' => array(
        'edit-style-settings-limit' => array(TRUE),
        'edit-settings-style-settings-limit' => array(TRUE),
      ),
      '#states' => array(
        'invisible' => array(
          ':input[name="settings[style_settings][limit]"]' => array('checked' => FALSE),
        ),
      ),
    );
    $form['limitation']['length'] = array(
      '#type' => 'textfield',
      '#title' => t('Items to display'),
      '#description' => t('The number of items to display. Enter 0 for no limit.'),
      '#default_value' => $settings['limitation']['length'],
    );
    $form['limitation']['offset'] = array(
      '#type' => 'textfield',
      '#title' => t('Offset'),
      '#description' => t('The number of items to skip. For example, if this field is 3, the first 3 items will be skipped and not displayed.'),
      '#default_value' => $settings['limitation']['offset'],
    );
    $form['minipanel'] = array(
      '#type' => 'checkbox',
      '#title' => t('Render items with a mini panel'),
      '#default_value' => !empty($settings['rendering']['minipanel']),
    );
    $form['rendering'] = array(
      '#type' => 'select',
      '#options' => array_merge(array(
        0 => t(' - None - '),
      ), $this->getOptions($settings)),
      '#default_value' => isset($settings['rendering']) ? $settings['rendering'] : array(),
      '#dependency' => array(
        'edit-style-settings-minipanel' => array(TRUE),
      ),
      '#states' => array(
        'invisible' => array(
          ':input[name="settings[style_settings][minipanel]"]' => array('checked' => FALSE),
        ),
      ),
    );
  }

  public function formSettingsSubmit(&$settings) {
    // Reset limitation if limit isn't checked.
    if (empty($settings['limit'])) {
      $settings['limitation']['length'] = 0;
      $settings['limitation']['offset'] = 0;
    }
    if (empty($settings['limitation']['length'])) {
      $settings['limitation']['length'] = 0;
    }
    if (empty($settings['limitation']['offset'])) {
      $settings['limitation']['offset'] = 0;
    }
    // Reset rendering if minipanel isn't checked.
    if (empty($settings['minipanel'])) {
      $settings['rendering'] = 0;
    }
  }
}
