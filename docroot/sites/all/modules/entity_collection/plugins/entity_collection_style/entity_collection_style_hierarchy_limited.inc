<?php
/**
 * @file
 * A hiearchy style plugin.
 */

/**
 * This style can show groups of content in hierarchies.
 */
class EntityCollectionStyleHierarchyLimited extends EntityCollectionStyleHierarchy {
  private $break_default = 10;

  /**
   * Build the hierarchy.
   * @see EntityCollectionStyle::build().
   */
  public function build(EntityCollection $entity, EntityCollectionTreeNode &$tree, $langcode = NULL, $settings = array()) {
    $this->settings = array_merge($this->settings, $settings);

    // FIXME/TODO: Should be placed someplace more generic.
    $entity->settings['style_settings'] = $this->settings;

    if ( $this->useBreakpoint() ) {
      $offset = 0;
      $tree = $tree->splice($offset, $this->getBreakpoint($entity));
    }

    return parent::build($entity, $tree, $langcode);
  }

  /**
   * Render the content of a tree as a tree using a template.
   */
  protected function renderTreeNode(&$content, $entity, EntityCollectionTreeNode $item, $langcode = NULL) {
    $element = array(
      '#collection_children' => array(),
      '#weight' => $item->position,
      '#theme' => 'entity_collection_style_hierarchy'
    );
    if (isset($item->content)) {
      $data = EntityCollectionRow::getRow($entity)->build($entity, $item, $langcode);
      $element['#collection_parent'] = $data;
    }
    foreach ($item->getChildren() as $key => $child) {
      $this->renderTreeNode($element['#collection_children'], $entity, $child, $langcode);
    }
    $content[] = $element;
  }

  /**
   * Adds the required settings
   */
  public function settingsForm(&$form, $settings = array()) {
    $settings = $this->settings + $settings;
    parent::settingsForm($form, $settings);

    $form['enable_breakpoint'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Breakpoint'),
      '#description' => t('Enable/Disable the break functionality. If disabled this will behave like a Hierarchy style.'),
      '#default_value' => !empty($settings['enable_breakpoint']),
    );

    $form['breakpoint'] = array(
      '#type' => 'textfield',
      '#title' => t('Breakpoint'),
      '#description' => t('Set the default breakpoint after a specified number of items. <em>Defaults to: @num</em>', array('@num'=>$this->break_default)),
      '#size' => 40,
      '#maxlength' => 5,
      '#default_value' => isset($settings['breakpoint']) ? $settings['breakpoint'] : $this->break_default,
    );
  }
  
  public function getBreakpoint(EntityCollection $collection) {
    $breakpoint = EntityCollectionStorage::getBundleStorage($collection->bundle)->getBreakpoint($collection, $collection->contexts);
    return (!$breakpoint OR $breakpoint < 0) ? $this->break_default : $breakpoint;
  }
  
  public function setBreakpoint($value=10) {  
    $this->settings['breakpoint'] = $value;
  }
  
  public function useBreakpoint(){
    return !empty($this->settings['enable_breakpoint']);
  }
  
  public function setBreakpointStatus($enabled = TRUE) {
    $this->settings['enable_breakpoint'] = $enabled;
  }

  public function toggleBreakpointStatus($enabled = TRUE) {
    $this->setBreakpointStatus( !$this->useBreakpoint() );
  }
}
