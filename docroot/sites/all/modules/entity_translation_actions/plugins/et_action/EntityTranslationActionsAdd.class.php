<?php

/**
 * @file
 * Et_action plugin class.
 */

class EntityTranslationActionsAdd extends EntityTranslationActionsBasic {

  /**
   * Function checks if plugin available for selected entity type.
   */
  public function available() {
    return entity_translation_enabled($this->entityType);
  }

  /**
   * Function function executes plugin actions.
   */
  public function action($entity, $context, $handler = NULL) {

    $handler = $handler ? $handler : entity_translation_get_handler($this->entityType, $entity);

    $lang_source = ($this->options['source'] == '-entity-') ? $this->entityLanguage($entity, $handler, FALSE) : $this->options['source'];
    $language_target = ($this->options['language'] == '-entity-') ? $this->entityLanguage($entity, $handler, FALSE) : $this->options['language'];

    if ($lang_source == $language_target) {
      return;
    }

    $translations = $handler->getTranslations();

    // No source translation;
    if ((!$lang_source || !$translations || !isset($translations->data[$lang_source])) && $lang_source != LANGUAGE_NONE) {
      return ENTITY_TRANSLATION_ACTIONS_RESULT_NO_SOURCE;
    }

    // Target translation exists;
    if (isset($translations->data[$language_target]) && $this->options['translation_exists'] == ENTITY_TRANSLATION_ACTIONS_TRANSLATION_SKIP) {
      return ENTITY_TRANSLATION_ACTIONS_RESULT_EXISTS;
    }

    list(,, $bundle_name) = entity_extract_ids($this->entityType, $entity);

    // Clone field translations.
    foreach (field_info_instances($this->entityType, $bundle_name) as $instance) {
      $field_name = $instance['field_name'];
      $field = field_info_field($field_name);

      if ($field['translatable']) {
        $original_item = isset($entity->{$field_name}[$lang_source]) ? $entity->{$field_name}[$lang_source] : NULL;

        // MERGE. Add item if not empty and no value for language.
        if ($original_item && !isset($entity->{$field_name}[$language_target])) {
          $entity->{$field_name}[$language_target] = $original_item;
        }

        // REPLACE. Replace existing value for language.
        // Or remode it if source values is empty.
        elseif ($this->options['translation_exists'] == ENTITY_TRANSLATION_ACTIONS_TRANSLATION_REPLACE) {
          if ($original_item) {
            $entity->{$field_name}[$language_target] = $original_item;
          }
          else {
            unset($entity->{$field_name}[$language_target]);
          }
        }
      }
    }

    // Create new translation.
    if ($language_target != LANGUAGE_NONE) {
      $lang_entity = $handler->getLanguage();
      $new_translation = isset($translations->data[$lang_source]) ? $translations->data[$lang_source] : array();
      $new_translation['status'] = TRUE;
      $new_translation['language'] = $language_target;
      $new_translation['source'] = ($lang_entity == $language_target) ? FALSE : $lang_entity;
      $handler->setTranslation($new_translation);
    }
    return ENTITY_TRANSLATION_ACTIONS_RESULT_CREATED;
  }

  /**
   * Function builds form elements for action.
   */
  public function formBuild(&$form, &$form_state) {

    $options = $this->languagesOptions();
    $options_keys = array_keys($options);

    $form['source'] = array(
      '#type' => 'radios',
      '#options' => $options,
      '#title' => t('Original language:'),
      '#required' => TRUE,
      '#default_value' => current($options_keys),
    );

    $form['language'] = array(
      '#type' => 'radios',
      '#options' => $options,
      '#title' => t('New language:'),
      '#required' => TRUE,
    );

    $form['translation_exists'] = array(
      '#type' => 'select',
      '#options' => $this->translationExistsOptions(),
      '#title' => t('Actions for existing translation:'),
    );
  }

  /**
   * Function returns options ways for resolving conflicts in translations.
   */
  protected function translationExistsOptions() {
    return array(
      ENTITY_TRANSLATION_ACTIONS_TRANSLATION_SKIP => t('SKIP ANY actions if translation exists'),
      ENTITY_TRANSLATION_ACTIONS_TRANSLATION_REPLACE => t('REPLACE ALL existing values with new'),
      ENTITY_TRANSLATION_ACTIONS_TRANSLATION_MERGE => t('ADD NEW values if existing translation is empty'),
    );
  }
}
