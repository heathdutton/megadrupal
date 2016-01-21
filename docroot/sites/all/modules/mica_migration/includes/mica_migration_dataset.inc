<?php
/**
 * @file
 * mica_migration_dataset.inc
 */

/**
 * Construct the Dto of Study/Harmonization Datasets.
 *
 * @param object $node
 *   The Dataset node to export.
 * @param array $context
 *   An array collecting values var during the batch operation.
 */
function _mica_migration_dataset_dto($node, array &$context) {
  $dataset_gen_dto = NULL;
  $dataset_type_parse = NULL;
  $wrapper_study = entity_metadata_wrapper('node', $node);

  $dto_util = new MicaMigrationDtoHelpers();
  $datasets_to_file_save = array();

  foreach ($wrapper_study->mica_dataset->getIterator() as $dataset) {
    $dataset_name = $dataset->title->value();
    $dce_population_find_param = array(
      'study' => $wrapper_study,
      'datasetId' => $dataset->uuid->value(),
    );
    $connector = mica_connector_query($dataset->getIdentifier(), $node->nid);
    $connector_option = !empty($connector->options) ? $connector->options : array();
    if ($dataset->field_dataset_type->value() == 'harmonization') {
      $dataset_gen_dto = mica_migration_dataset_harmonized($wrapper_study,
        $connector_option, $dataset_name, $dce_population_find_param);
      $dataset_type_parse = "obiba.mica.HarmonizationDatasetDto.type";
    }

    elseif ($dataset->field_dataset_type->value() == 'study') {
      $dataset_gen_dto = mica_migration_dataset_study($wrapper_study,
        $connector_option, $dataset_name, $dce_population_find_param);
      $dataset_type_parse = "obiba.mica.StudyDatasetDto.type";
    }

    $dataset_dto = new \obiba\mica\DatasetDto();
    $dataset_dto->setPublished(FALSE);

    $dto_util->addTranslatedField($dataset_dto, 'addName', $dataset, 'title');
    $dto_util->addTranslatedField($dataset_dto, 'setDescription', $dataset, 'body', 'value');
    $dataset_dto->setEntityType('Participant');
    $dataset_dto[$dataset_type_parse] = $dataset_gen_dto;

    $raw_response = $dataset->uuid->value();
    $dataset_dto->setId($raw_response);
    // Create  dataset.
    $codec = new \DrSlump\Protobuf\Codec\Json();
    $dataset_dto_json = $dataset_dto->serialize($codec);

    $datasets_to_file_save[] = array(
      'uuid' => $raw_response,
      'title_dataset' => $dataset->title->value(),
      'dataset_json' => $dataset_dto_json,
    );
  }
  $context['results']['datasets'] = $datasets_to_file_save;
}

/**
 * Get the DCE/Population Id's linked to the Dataset.
 *
 * @param object $study_wrapper
 *   The Entity wrapper node to export (Study, network, ...).
 * @param int $dataset_id
 *   The Id of the Dataset to export.
 *
 * @return array|bool
 *   The array of Population/Dce linked to the Dataset.
 */
function _mica_migration_get_dce_populations_id($study_wrapper, $dataset_id) {
  foreach ($study_wrapper->field_study_populations->getIterator() as $population) {
    foreach ($population->field_pop_dce->getIterator() as $dce) {
      $dce_id = $dce->uuid->value();
      $population_id = $population->uuid->value();

      $array_id = array(
        'populationId' => $population_id,
        'dceId' => $dce_id,
      );
      foreach ($dce->field_dce_dataset->getIterator() as $datasets_dce) {
        if ($datasets_dce->uuid->value() == $dataset_id) {
          return $array_id;
        }
      }

    }
  }
  return FALSE;
}

/**
 * Construct the Harmonization dataset Dto.
 *
 * @param object $study_wrapper
 *   The Study Entity wrapper node.
 * @param array $connector_options
 *   The array of connector option to opal.
 * @param string $dataset_name
 *   The name of the dataset.
 * @param array $dce_population_find_param
 *   An array of the current study wrapper + Dataset Id.
 *
 * @return \obiba\mica\HarmonizationDatasetDto
 *   The Dto Object.
 */
function mica_migration_dataset_harmonized($study_wrapper, array $connector_options, $dataset_name, array $dce_population_find_param) {
  $acronym_study = $study_wrapper->field_acroym->value();
  $project = !empty($connector_options) ? $connector_options['datasource'] :
    !empty($acronym_study) ? $acronym_study : $study_wrapper->title_field->value();
  $table = !empty($connector_options) ? $connector_options['table'] : $dataset_name;
  $datset_harmo_st_dto = new \obiba\mica\DatasetDto\StudyTableDto();
  $datset_harmo_dto = new \obiba\mica\HarmonizationDatasetDto();

  $datset_harmo_st_dto->setPopulationId('novalue');
  $datset_harmo_st_dto->setDataCollectionEventId('novalue');
  if (!empty($dce_population_find_param)) {
    $ids = _mica_migration_get_dce_populations_id($dce_population_find_param['study'], $dce_population_find_param['datasetId']);
    if (!empty($ids['dceId'])) {
      $datset_harmo_st_dto->setPopulationId($ids['populationId']);
      $datset_harmo_st_dto->setDataCollectionEventId($ids['dceId']);
    }
  }
  $datset_harmo_st_dto->setStudyId($study_wrapper->uuid->value());
  $datset_harmo_st_dto->setProject($project);
  $datset_harmo_st_dto->setTable($table);

  $datset_harmo_dto->addStudyTables($datset_harmo_st_dto);

  $datset_harmo_dto->setProject('mica');
  $datset_harmo_dto->setTable($table);
  return $datset_harmo_dto;
}

/**
 * Construct the Study dataset Dto.
 *
 * @param object $study_wrapper
 *   The Study Entity wrapper node.
 * @param array $connector_options
 *   The array of connector option to opal.
 * @param string $dataset_name
 *   The name of the dataset.
 * @param array $dce_population_find_param
 *   An array of the current study wrapper + Dataset Id.
 *
 * @return \obiba\mica\StudyDatasetDto
 *   The Dto Object.
 */
function mica_migration_dataset_study($study_wrapper, array $connector_options, $dataset_name, array $dce_population_find_param) {
  $acronym_study = $study_wrapper->field_acroym->value();
  $project = !empty($connector_options) ? $connector_options['datasource'] :
    !empty($acronym_study) ? $acronym_study : $study_wrapper->title_field->value();
  $table = !empty($connector_options) ? $connector_options['table'] : $dataset_name;
  $datset_study_st_dto = new \obiba\mica\DatasetDto\StudyTableDto();
  $datset_study_st_dto->setPopulationId('novalue');
  $datset_study_st_dto->setDataCollectionEventId('novalue');
  if (!empty($dce_population_find_param)) {
    $ids = _mica_migration_get_dce_populations_id($dce_population_find_param['study'], $dce_population_find_param['datasetId']);
    if (!empty($ids['dceId'])) {
      $datset_study_st_dto->setPopulationId($ids['populationId']);
      $datset_study_st_dto->setDataCollectionEventId($ids['dceId']);
    }
  }
  $datset_study_st_dto->setStudyId($study_wrapper->uuid->value());
  $datset_study_st_dto->setProject($project);
  $datset_study_st_dto->setTable($table);

  $datset_study_dto = new \obiba\mica\StudyDatasetDto();
  $datset_study_dto->setStudyTable($datset_study_st_dto);

  return $datset_study_dto;
}
