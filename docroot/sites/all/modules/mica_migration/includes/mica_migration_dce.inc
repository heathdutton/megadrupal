<?php

/**
 * @file
 * Mica_export_dce.inc.
 */

/**
 * Construct the DCE Dto to export.
 *
 * @param object $dto_util
 *   The instantiated Dto util class MicaMigrationDtoHelpers.
 * @param object $wrapper
 *   The Entity wrapper node to export (Study, network, ...).
 * @param array $context
 *   An array collecting values var during the batch operation.
 *
 * @return \obiba\mica\StudyDto\PopulationDto\DataCollectionEventDto
 *   The Dto Object.
 */
function _mica_migration_dce_dto($dto_util, $wrapper, array &$context) {
  $dce_dto = new obiba\mica\StudyDto\PopulationDto\DataCollectionEventDto();
  $attachments_files = array();
  $dce_title = $wrapper->title_field->value();
  $dce_dto->setId($wrapper->uuid->value());
  if (!empty($dce_title)) {
    $dto_util->addTranslatedField($dce_dto, 'addName',
      $wrapper, 'title_field');
  }

  $dce_description = $wrapper->body->value();
  if (!empty($dce_description)) {
    $dto_util->addTranslatedField($dce_dto, 'addDescription',
      $wrapper, 'body', 'value');
  }
  $start_year = $wrapper->field_dce_start_year->value();
  if (empty($start_year)) {
    $wrapper->field_dce_start_year->set(0);
  }
  $dce_dto->setStartYear($wrapper->field_dce_start_year->value());
  $dce_dto->setStartMonth($wrapper->field_dce_start_month->value());
  $dce_dto->setEndYear($wrapper->field_dce_end_year->value());
  $dce_dto->setEndMonth($wrapper->field_dce_end_month->value());

  foreach ($wrapper->field_dce_data_sources->getIterator() as $sources) {
    if (!empty($sources)) {
      $dce_dto->addDataSources($sources->value());
      if ($sources->value() == 'others') {
        $dce_datasource_sp = $wrapper->field_dce_data_sources_sp->value();
        if (!empty($dce_datasource_sp)) {
          $dto_util->addTranslatedField($dce_dto, 'addOtherDataSources',
            $wrapper, 'field_dce_data_sources_sp');
        }
      }
      if ($sources->value() == 'biological_samples') {
        foreach ($wrapper->field_dce_bio_samples_management->getIterator() as $biosample) {
          if (!empty($biosample)) {
            $dce_dto->addBioSamples($biosample->value());
          }
          if ($biosample->value() == 'others') {
            $other_tissue = $wrapper->field_dce_samples_man_other_sp->value();
            if (!empty($other_tissue)) {
              $dto_util->addTranslatedField($dce_dto, 'addOtherBioSamples',
                $wrapper, 'field_dce_samples_man_other_sp', 'value');
            }
          }
          if ($biosample->value() == 'tissues') {
            $dce_tissue_sp = $wrapper->field_dce_tissues_sp->value();
            if (!empty($dce_tissue_sp)) {
              $dto_util->addTranslatedField($dce_dto, 'addTissueTypes',
                $wrapper, 'field_dce_tissues_sp');
            }
          }
        }
      }
      if ($sources->value() == 'administratives_databases') {
        foreach ($wrapper->field_dce_data_sources_admin_db->getIterator() as $admindata) {
          if (!empty($admindata)) {
            $dce_dto->addAdministrativeDatabases($admindata->value());
          }
        }
      }
    }
  }

  foreach ($wrapper->field_dce_data_dictionaries->value() as $attachment) {
    if (!empty($attachment['filename'])) {
      $attachment_dto = $dto_util->attachmentConstruct($attachment, 'dictionaries');
      $attachments_files[] = array(
        'uuid' => $attachment['uuid'],
        'filename' => $attachment['filename'],
        'uri' => $attachment['uri'],
      );
      $dce_dto->addAttachments($attachment_dto);
    }
  }

  foreach ($wrapper->field_dce_sops->value() as $attachment) {
    if (!empty($attachment['filename'])) {
      $attachment_dto = $dto_util->attachmentConstruct($attachment, 'sop');
      $attachments_files[] = array(
        'uuid' => $attachment['uuid'],
        'filename' => $attachment['filename'],
        'uri' => $attachment['uri'],
      );
      $dce_dto->addAttachments($attachment_dto);
    }
  }

  foreach ($wrapper->field_dce_questionnaires->value() as $attachment) {
    if (!empty($attachment['filename'])) {
      $attachment_dto = $dto_util->attachmentConstruct($attachment, 'questionnaire');
      $attachments_files[] = array(
        'uuid' => $attachment['uuid'],
        'filename' => $attachment['filename'],
        'uri' => $attachment['uri'],
      );
      $dce_dto->addAttachments($attachment_dto);
    }
  }

  foreach ($wrapper->field_dce_others->value() as $attachment) {
    if (!empty($attachment['filename'])) {
      $attachment_dto = $dto_util->attachmentConstruct($attachment, 'others_document');
      $attachments_files[] = array(
        'uuid' => $attachment['uuid'],
        'filename' => $attachment['filename'],
        'uri' => $attachment['uri'],
      );
      $dce_dto->addAttachments($attachment_dto);
    }
  }

  if (!empty($attachments_files)) {
    $context['results']['dce']['attachments'][$wrapper->uuid->value()] = $attachments_files;
  }
  return $dce_dto;
}
