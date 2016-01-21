<?php
/**
 * @file
 * Mica_export_contact.inc.
 */

/**
 * Construct the Dto of Contact/investigators.
 *
 * @param object $dto_util
 *   The instantiated Dto util class MicaMigrationDtoHelpers.
 * @param object $wrapper
 *   The Entity wrapper node to export (Study, network, ...).
 *
 * @return \obiba\mica\ContactDto
 *   The Dto Object.
 */
function _mica_migration_contact_dto($dto_util, $wrapper) {
  $contact = new obiba\mica\ContactDto();
  try {
    $contact_name_wrapper = $wrapper->field_contact_name->value();
  }
  catch (Exception $e) {
    $contact_name_wrapper = NULL;
  }

  $contact->setTitle(!empty($contact_name_wrapper['title']) ? $contact_name_wrapper['title'] : NULL);
  $contact->setFirstName(!empty($contact_name_wrapper['given']) ? $contact_name_wrapper['given'] : NULL);
  $last_name = !empty($contact_name_wrapper['family']) ? $contact_name_wrapper['family'] : NULL;
  $title_name = !empty($wrapper->title) ? $wrapper->title : NULL;
  $contact->setLastName(!empty($last_name) ? $last_name : $title_name);
  $contact->setEmail(!empty($wrapper->field_contact_email) ? $wrapper->field_contact_email->value() : NULL);
  $contact->setPhone(!empty($wrapper->field_telephone) ? $wrapper->field_telephone->value() : NULL);
  $contact->setDataAccessCommitteeMember(!empty($wrapper->field_daco) ? $wrapper->field_daco->value() : NULL);

  $institution = new obiba\mica\ContactDto\InstitutionDto();

  $institution_name = !empty($wrapper->field_institution_name) ? $wrapper->field_institution_name->value() : NULL;
  if (!empty($institution_name)) {
    $dto_util->addTranslatedField($institution, 'addName', $wrapper, 'field_institution_name');
  }

  $departement_name = !empty($wrapper->field_department_unit) ? $wrapper->field_department_unit->value() : NULL;
  if (!empty($departement_name)) {
    $dto_util->addTranslatedField($institution, 'addDepartment', $wrapper, 'field_department_unit');
  }

  $address = new obiba\mica\AddressDto();
  $address_street = !empty($wrapper->field_address) ? $wrapper->field_address->value() : NULL;
  if (!empty($address_street)) {
    $dto_util->addTranslatedField($address, 'addStreet', $wrapper, 'field_address');
  }

  $address_city = !empty($wrapper->field_city) ? $wrapper->field_city->value() : NULL;

  if (!empty($address_city)) {
    $dto_util->addTranslatedField($address, 'addCity', $wrapper, 'field_city');
  }

  $address->setZip(!empty($wrapper->field_postal_code) ? $wrapper->field_postal_code->value() : NULL);
  $address->setState(!empty($wrapper->field_state) ? $wrapper->field_state->value() : NULL);

  $contry = !empty($wrapper->field_contact_country) ? $wrapper->field_contact_country->value() : NULL;
  if (!empty($contry)) {
    $country_iso = new \obiba\mica\CountryDto();
    $country_iso->setIso($wrapper->field_contact_country->value());

    $country_name = $wrapper->field_contact_country->label();

    if (!empty($country_name)) {
      $dto_util->addTranslatedField($country_iso, 'addName', $wrapper, 'field_contact_country', NULL, 'label');
    }

    $address->setCountry($country_iso);
  }
  $institution->setAddress($address);
  $contact->setInstitution($institution);

  return $contact;
}
