<?php

namespace Drupal\smartling\Forms;

class NodeSettingsForm extends GenericEntitySettingsForm {

  public function __construct() {
    $this->entity_name_translated = t('Node');
    $this->entity_key = '#entity';
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'smartling_get_node_settings_form';
  }

  protected function targetLangFormElem($id, $entity_type, $entity, $default_language) {
    // This is node for fields method translate or original for nodes method.
    if (($entity->tnid == '0') || ($entity->tnid == $entity->nid)) {
      $languages = smartling_language_list();
    }
    elseif ($entity->tnid != $entity->nid) {
      // This is node for nodes method translate | not original.
      $languages = smartling_language_list();
      $node_original = node_load($entity->tnid);
      unset($languages[$node_original->language]);
    }

    if (!is_null($id)) {
      $check = array();

      if (($entity->tnid != '0') && ($entity->tnid != $entity->nid)) {
        // For not original node in nodes translate method.
        $translations = translation_node_get_translations($entity->tnid);
        $original_nid = FALSE;
        // Get original.
        foreach ($translations as $langcode => $value) {
          if ($translations[$langcode]->nid == $entity->tnid) {
            $original_nid = $translations[$langcode]->nid;
            break;
          }
        }

        foreach ($languages as $d_locale => $language) {
          //if ($language->enabled != '0') {

            $entity_data = smartling_entity_load_by_conditions(array(
              'rid' => $original_nid,
              'entity_type' => $entity_type,
              'target_language' => $d_locale,
            ));
            $language_name = check_plain($language->name);

            if ($entity_data !== FALSE) {
              $options[$d_locale] = smartling_entity_status_message(t('Node'), $entity_data->status, $language_name, $entity_data->progress);
            }
            else {
              $options[$d_locale] = $language_name;
            }

            $check[] = ($entity_data) ? $d_locale : FALSE;
          //}
        }
      }
      elseif (($entity->tnid != '0') && ($entity->tnid == $entity->nid)) {
        // For original node in nodes translate method.
        $translations = translation_node_get_translations($entity->tnid);
        $original_nid = FALSE;
        // Get original.
        foreach ($translations as $langcode => $value) {
          if ($translations[$langcode]->nid == $entity->tnid) {
            $original_nid = $translations[$langcode]->nid;
            break;
          }
        }

        foreach ($languages as $d_locale => $language) {

          if ($default_language != $d_locale ) {//&& $language->enabled != '0') {

            $entity_data = smartling_entity_load_by_conditions(array(
              'rid' => $original_nid,
              'entity_type' => $entity_type,
              'target_language' => $d_locale,
            ));
            $language_name = check_plain($language->name);

            if ($entity_data !== FALSE) {
              $options[$d_locale] = smartling_entity_status_message(t('Node'), $entity_data->status, $language_name, $entity_data->progress);
            }
            else {
              $options[$d_locale] = $language_name;
            }

            $check[] = ($entity_data) ? $d_locale : FALSE;
          }
        }
      }
      else {
        // For fieds method.
        foreach ($languages as $d_locale => $language) {
          if ($default_language != $d_locale ) {//&& $language->enabled != '0') {

            $entity_data = smartling_entity_load_by_conditions(array(
              'rid' => $id,
              'entity_type' => $entity_type,
              'target_language' => $d_locale,
            ));
            $language_name = check_plain($language->name);

            if ($entity_data !== FALSE) {
              $options[$d_locale] = smartling_entity_status_message(t('Node'), $entity_data->status, $language_name, $entity_data->progress);
            }
            else {
              $options[$d_locale] = $language_name;
            }
            $check[] = ($entity_data) ? $d_locale : FALSE;
          }
        }
      }

      $elem = array(
        '#type' => 'checkboxes',
        '#title' => 'Target Locales',
        '#options' => $options,
        '#default_value' => $check,
      );
    }
    else {
      foreach ($languages as $langcode => $language) {
        $options[$langcode] = check_plain($language->name);
      }

      $elem = array(
        '#type' => 'checkboxes',
        '#title' => 'Target Locales',
        '#options' => $options,
      );
    }
    return $elem;
  }
}