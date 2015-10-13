<?php

namespace Drupal\campaignion\CRM;

use \Drupal\campaignion\Contact;

class ExporterBase implements ExporterInterface {
  protected $map;
  protected $contact;
  protected $wrappedContact;
  public function __construct($mappings) {
    $this->map = $mappings;
    foreach ($this->map as $mapper) {
      $mapper->setExporter($this);
    }
  }

  public function getContact() {
    return $this->contact;
  }

  public function getWrappedContact() {
    return $this->wrappedContact;
  }

  public function value($key) {
    if (isset($this->map[$key])) {
      return $this->map[$key]->value();
    }
    return NULL;
  }

  public function setContact(Contact $contact) {
    $this->contact = $contact;
    $this->wrappedContact = $contact->wrap();
    return $this;
  }
}
