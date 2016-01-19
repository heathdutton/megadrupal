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
        // Although a target already exists, we load the source and run it
        // through field translation preparation as if one didn't exist already.
        // This ensures all embedded entities have some structure for XLIFF
        // content to be set against.
        $actual_target = $this->drupal->nodeLoad($this->tset[$targetLanguage]->nid, NULL, TRUE);
        $sourceNid = $actual_target->tnid;
        $target = $this->drupal->nodeLoad($sourceNid);
        $source = clone $target;
        unset($target->nid, $target->vid, $target->tnid);

        // Set properties as though translation_node_prepare set them.
        $target->language = $targetLanguage;
        $target->translation_source = $source;
        $target->title = $source->title;

        // Run through field translation preparation, but be sure to reset the
        // actual nid/vid/tnid values. This ensures that we never mistakenly
        // set translations on entities embedded on the source translation.
        $this->drupal->fieldAttachPrepareTranslation('node', $target, $targetLanguage, $source, $source->language);
        $target->nid = $actual_target->nid;
        $target->vid = $actual_target->vid;
        $target->tnid = $actual_target->tnid;

        // Preserve the original target's path alias.
        $target->path = $actual_target->path;

        // Do not mark this node as a new revision. This is necessary in
        // cases where this node happens to reference a field collection...
        $target->revision = FALSE;
      }
      // Otherwise prepare the original for translation.
      else {
        $this->initializeTranslation();
        $target = $this->getRawEntity($this->entity);
        unset($target->nid, $target->vid);
        $target->is_new = TRUE;
        $this->translationNodePrepare($target, $this->entity->getIdentifier(), $targetLanguage);
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

  /**
   * OO wrapper around translation_node_prepare().
   * @param object $node
   * @param int $sourceNid
   * @param string $targetLang
   */
  protected function translationNodePrepare($node, $sourceNid, $targetLang) {
    $tempUser = clone $GLOBALS['user'];
    $this->drupal->saveSession(FALSE);
    $GLOBALS['user'] = $this->drupal->userLoad(1);
    $_GET['translation'] = $sourceNid;
    $_GET['target'] = $targetLang;
    $this->drupal->translationNodePrepare($node);
    unset($_GET['translation'], $_GET['target']);
    $GLOBALS['user'] = $tempUser;
    $this->drupal->saveSession(TRUE);
  }

}
