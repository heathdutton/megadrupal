<?php

/**
 * @file
 * Smartling translation handler for the translation module.
 */

/**
 * Class extends the EntityTranslationDefaultHandler.
 */
class SmartlingEntityTranslationDefaultHandler extends EntityTranslationDefaultHandler {

  /**
   * Smartling Entity Translation Field Attach.
   *
   * @see EntityTranslationHandlerInterface::entityFormSubmit()
   */
  public function smartlingEntityTranslationFieldAttach($values = array()) {
    $langcode = $this->getFormLanguage();
    $translations = $this->getTranslations();
    $is_translation = TRUE;

    // Ensure every key has at least a default value. Subclasses may provide use
    // entity-specific values to alter them.
    $values += array(
      'status' => TRUE,
      'retranslate' => 0,
      'name' => isset($GLOBALS['user']->name) ? $GLOBALS['user']->name : '',
    );

    if (!isset($translations->data[$langcode])) {
      // If we have a new translation the language is the original entity
      // language.
      $translation = $is_translation ? array(
        'language' => $langcode,
        'source' => $this->getSourceLanguage(),
      ) : array(
        'language' => $langcode,
        'source' => '',
      );
    }
    else {
      $translation = $translations->data[$langcode];
    }

    if (isset($values['translate'])) {
      $translation['translate'] = intval($values['translate']);
    }
    else {
      $this->setOutdated($values['retranslate']);
    }

    // Handle possible language changes for the original values.
    if (!$is_translation) {
      $this->setOriginalLanguage($langcode);
    }

    $translation['status'] = intval($values['status']);
    $translation['uid'] = $values['name'] ? user_load_by_name($values['name'])->uid : 0;

    $this->setTranslation($translation);
  }

}
