<?php

/**
 * @file
 * Defines a Node translatable compatible with the core Translation module.
 */

namespace EntityXliff\Drupal\Translatable\Content;

use EntityXliff\Drupal\Factories\EntityTranslatableFactory;
use EntityXliff\Drupal\Mediator\FieldMediator;
use EntityXliff\Drupal\Translatable\EntityTranslatableBase;
use EntityXliff\Drupal\Utils\DrupalHandler;


/**
 * Class NodeContentTranslatable
 * @package EntityXliff\Drupal\Translatable\Content
 */
class NodeTranslatable extends EntityTranslatableBase {

  /**
   * An array of partial nodes in the translation set represented by this node,
   * keyed by site (used for content translation).
   *
   * @var array
   */
  protected $tset = array();

  /**
   * {@inheritdoc}
   */
  public function __construct(\EntityDrupalWrapper $entityWrapper, DrupalHandler $handler = NULL, EntityTranslatableFactory $factory = NULL, FieldMediator $fieldMediator = NULL) {
    parent::__construct($entityWrapper, $handler, $factory, $fieldMediator);

    $this->drupal->staticReset('translation_node_get_translations');
    $raw = $entityWrapper->raw();
    if (!isset($raw->tnid) || empty($raw->tnid)) {
      $this->tset = $this->nodeGetTranslations((int) $entityWrapper->getIdentifier());
    }
    else {
      $this->tset = $this->nodeGetTranslations((int) $raw->tnid);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getTranslatableFields() {
    $fields = parent::getTranslatableFields();
    if ($this->isTranslatable()) {
      $fields[] = 'title';
    }
    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function isTranslatable() {
    return $this->drupal->translationSupportedType($this->entity->getBundle());
  }

  /**
   * {@inheritdoc}
   */
  public function getTargetEntity($targetLanguage) {
    if (!isset($this->targetEntities[$targetLanguage]) || empty($this->targetEntities[$targetLanguage])) {
      // If a translation already exists, use it!
      if (isset($this->tset[$targetLanguage]->nid)) {
        $target = $this->drupal->nodeLoad($this->tset[$targetLanguage]->nid, NULL, TRUE);

        // Do not mark this node as a new revision. This is necessary in
        // cases where this node happens to reference a field collection...
        $target->revision = FALSE;
      }
      // Otherwise, "clone" the original and mark it as new.
      else {
        $this->initializeTranslation();
        $target = $this->getRawEntity($this->entity);
        $target->translation_source = clone $target;

        unset($target->nid, $target->vid);
        $target->is_new = TRUE;
        $target->tnid = (int) $this->entity->getIdentifier();
        $target->language = $targetLanguage;
      }

      $this->targetEntities[$targetLanguage] = $this->drupal->entityMetadataWrapper('node', $target);
    }

    return $this->targetEntities[$targetLanguage];
  }

  /**
   * {@inheritdoc}
   *
   * This converts a language neutral node to an English node that is part of a
   * translation set.
   */
  public function initializeTranslation() {
    $nid = (int) $this->entity->getIdentifier();
    $source = $this->drupal->nodeLoad($nid, NULL, TRUE);
    if ($source->language === DrupalHandler::LANGUAGE_NONE || empty($source->tnid)) {
      $source->tnid = $nid;
      $source->language = $this->getSourceLanguage();
      $this->drupal->nodeSave($source);
      $this->entity = $this->drupal->entityMetadataWrapper('node', $source);
      $this->drupal->staticReset('translation_node_get_translations');
      $this->tset = $this->nodeGetTranslations((int) $source->tnid);
    }
  }

  /**
   * OO wrapper around translation_node_get_translations().
   *
   * @param int $tnid
   * @return array
   */
  protected function nodeGetTranslations($tnid) {
    // We have to temporarily pretend to have access to all nodes. If a node in
    // a translation set happens to be unpublished, we still want to know about
    // it so we can update/override it (rather than creating a new one).
    $tempUser = clone $GLOBALS['user'];
    $this->drupal->saveSession(FALSE);
    $GLOBALS['user'] = $this->drupal->userLoad(1);
    $tset = $this->drupal->translationNodeGetTranslations($tnid);
    $GLOBALS['user'] = $tempUser;
    $this->drupal->saveSession(TRUE);
    return $tset;
  }

}
