<?php

namespace Drupal\campaignion\CRM;

use \Drupal\campaignion\CRM\Import\Source\SourceInterface;
use \Drupal\campaignion\Contact;

class ImporterBase {
  protected $mappings;
  protected $contactType;

  public function __construct(array $mappings, $type = NULL) {
    $this->mappings = $mappings;
    $this->contactType = $type;
  }

  public function import(SourceInterface $source, Contact $contact) {
    $w = $contact->wrap();
    $isNewOrUpdated = empty($contact->contact_id);
    foreach ($this->mappings as $mapper) {
      $isNewOrUpdated = $mapper->import($source, $w) || $isNewOrUpdated;
    }
    return $isNewOrUpdated;
  }

  public function findOrCreateContact(SourceInterface $source) {
    return Contact::fromEmail($source->value('email'), $this->contactType);
  }
}
