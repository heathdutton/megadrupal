<?php

/**
 * @file
 * Contains Drupal\smartling\Processors\NodeProcessor.
 */

namespace Drupal\smartling\Processors;

class TaxonomyTermProcessor extends GenericEntityProcessor {

  protected $i18n_wrapper;

  public function __construct($smartling_submission, $field_processor_factory, $smartling_settings, $log, $entity_api_wrapper, $smartling_utils, $i18n_wrapper) {
    parent::__construct($smartling_submission, $field_processor_factory, $smartling_settings, $log, $entity_api_wrapper, $smartling_utils);

    $this->i18n_wrapper = $i18n_wrapper;
  }


  /**
   * {inheritdoc}
   */
  protected function prepareDrupalEntity() {
    $this->contentEntity = $this->entity_api_wrapper->entityLoadSingle('taxonomy_term', $this->smartling_submission->getRID());
    /* @var $source_drupal_entity \stdClass */
    $source_drupal_entity = $this->contentEntity;
    $term = $this->i18n_wrapper->i18nTaxonomyTermGetTranslation($this->contentEntity, $this->drupalTargetLocale);
    if (!is_null($term) && ($term->language != $this->contentEntity->language)) {
      $this->smartling_submission->setRID($term->tid);
      $this->contentEntity = $term;
    }
    else {
      // If term not exist, need create new term.
      $vocabulary = $this->entity_api_wrapper->taxonomyVocabularyMachineNameLoad($this->smartling_submission->getBundle());

      // Add language field or not depending on taxonomy mode.
      $vocabulary_mode = $this->i18n_wrapper->i18nTaxonomyVocabularyMode($vocabulary);
      switch ($vocabulary_mode) {
        case I18N_MODE_TRANSLATE:
          $this->ifFieldMethod = FALSE;
          // If the term to be added will be a translation of a source term,
          // set the default value of the option list
          // to the target language and
          // create a form element for storing
          // the translation set of the source term.
          $this->contentEntity = clone $source_drupal_entity;
          unset($this->contentEntity->tid);

          $target_language = $this->i18n_wrapper->i18nLanguageObject($this->drupalTargetLocale);
          // Set context language to target language.
          $this->i18n_wrapper->i18nLanguageContext($target_language);

          $this->contentEntity->language = $target_language->language;

          // Add the translation set to the form so we know the new term
          // needs to be added to that set.
          if (!empty($source_drupal_entity->i18n_tsid)) {
            $translation_set = $this->i18n_wrapper->i18nTaxonomyTranslationSetLoad($source_drupal_entity->i18n_tsid);
          }
          else {
            // No translation set yet, build a new one with the source term.
            $translation_set = $this->i18n_wrapper->i18nTranslationSetCreate('taxonomy_term', $vocabulary->machine_name)
              ->add_item($source_drupal_entity);
            $this->entity_api_wrapper->entitySave('taxonomy_term', $source_drupal_entity);
          }
          $this->contentEntity->i18n_tsid = $translation_set->tsid;

          break;

        case I18N_MODE_LOCALIZE:
          break;

        case I18N_MODE_LANGUAGE:
        case I18N_MODE_NONE:
          $this->log->error('Translatable @entity_type with id - @rid FAIL. Vocabulary mode - @vocabulary_mode',
            array(
              '@entity_type' => $this->drupalEntityType,
              '@rid' => $this->smartling_submission->getRID(),
              '@vocabulary_mode' => $vocabulary_mode
            ));
          break;

        default:
          $this->log->error('Translatable @entity_type with id - @rid FAIL', array(
            '@entity_type' => $this->drupalEntityType,
            '@rid' => $this->smartling_submission->getRID()
          ));
          break;
      }

      $this->entity_api_wrapper->entitySave('taxonomy_term', $this->contentEntity);
      $this->smartling_submission->setRID($this->contentEntity->tid);
    }
  }

  public static function supportedType($bundle) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($bundle);
    $vocabulary_mode = i18n_taxonomy_vocabulary_mode($vocabulary);
    return in_array($vocabulary_mode, array(
      I18N_MODE_TRANSLATE,
      I18N_MODE_LOCALIZE
    ));
  }

  protected function getOriginalEntity($entity) {
    return smartling_get_original_taxonomy_term($entity);
  }
}