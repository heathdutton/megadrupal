<?php
/**
 * @file
 * Mica_network_export_dto_dataset.inc.
 */

/**
 * Construct the Network Dto to export.
 *
 * @param object $node
 *   The network node to export.
 * @param array $context
 *    An array collecting values var during the batch operation.
 *
 * @return bool|\obiba\mica\NetworkDto
 *   The Dto Object.
 */
function _mica_migration_network_dto($node, array &$context) {
  $network_to_file_save = NULL;
  $attachments_files = array();
  $wrapper_study = entity_metadata_wrapper('node', $node);
  $dto_util = new MicaMigrationDtoHelpers();
  try {
    foreach ($wrapper_study->field_networks->getIterator() as $network_wrapper) {
      $network_dto = new obiba\mica\NetworkDto();
      $network_dto->addStudyIds($wrapper_study->uuid->value());
      $network_title = $network_wrapper->title_field->value();
      if (!empty($network_title)) {
        $dto_util->addTranslatedField($network_dto, 'addName',
          $network_wrapper, 'title_field');
      }

      $network_acronym = $network_wrapper->field_acroym->value();
      if (!empty($network_acronym)) {
        $dto_util->addTranslatedField($network_dto, 'addAcronym', $network_wrapper, 'field_acroym');
      }

      $dto_util->addTranslatedField($network_dto, 'addDescription', $network_wrapper, 'body', 'value');

      $url_website = $network_wrapper->field_website->value();
      if (!empty($url_website)) {
        $network_dto->setWebsite($url_website['url']);
      };

      $authorization_maelstrom_dto = new \obiba\mica\AuthorizationDto();
      $authorization_maelstrom_dto->setAuthorized($network_wrapper->field_authorization_maelstrom->value());
      $authorization_maelstrom_dto->setAuthorizer($network_wrapper->field_authorising_person_name_m->value());
      $authorization_maelstrom_dto->setDate(date("c", $network_wrapper->field_authorising_date_m->value()));
      $network_dto->setMaelstromAuthorization($authorization_maelstrom_dto);

      foreach ($network_wrapper->field_investigators->getIterator() as $investigator_wrapper) {
        if (!empty($investigator_wrapper)) {
          $investigator = _mica_migration_contact_dto($dto_util, $investigator_wrapper);
          $network_dto->addInvestigators($investigator);
        }
      }

      foreach ($network_wrapper->field_contacts_ref->getIterator() as $contact_wrapper) {
        if (!empty($contact_wrapper)) {
          $contact = _mica_migration_contact_dto($dto_util, $contact_wrapper);
          $network_dto->addContacts($contact);
        }
      }

      $supl_info = $network_wrapper->field_supp_infos->value();
      if (!empty($supl_info)) {
        $dto_util->addTranslatedField($network_dto, 'addInfo', $network_wrapper,
          'field_supp_infos');
      }

      // Attached Documents.
      $logo = $network_wrapper->field_logo->value();
      if (!empty($logo['filename'])) {
        $attachment_logo_dto = $dto_util->attachmentConstruct($logo, 'logo');
        $attachments_files[] = array(
          'uuid' => $logo['uuid'],
          'filename' => $logo['filename'],
          'uri' => $logo['uri'],
        );
        $network_dto->addAttachments($attachment_logo_dto);
      }

      $raw_response = $network_wrapper->uuid->value();
      $network_dto->setPublished(FALSE);
      $network_dto->setId($raw_response);

      // Create  dataset.
      $codec = new \DrSlump\Protobuf\Codec\Json();
      $network_dto_json = $network_dto->serialize($codec);
      $network_to_file_save[] = array(
        'uuid' => $raw_response,
        'title_network' => $network_wrapper->title_field->value(),
        'network_json' => $network_dto_json,
      );
    }
  }
  catch (Exception $e) {
    watchdog('Mica Export', '(Exception in mica_migration_network.inc) Error entity wrapper  : @code, message: @message',
      array(
        '@code' => $e->getCode(),
        '@message' => $e->getMessage(),
      ), WATCHDOG_WARNING);
  }
  if (!empty($network_to_file_save)) {
    $context['results']['networks'] = $network_to_file_save;
  }
  if (!empty($attachments_files)) {
    $context['results']['networks']['attachments'] = $attachments_files;
  }

  if (!empty($network_dto)) {
    return $network_dto;
  }
  return FALSE;
}
