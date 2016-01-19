<?php

use \Drupal\campaignion\Contact;

class ContactTest extends \DrupalUnitTestCase {
  public function testWrapContact() {
    $contact = Contact::fromBasicData('my@email.com', 'first', 'last');
    $contact->wrap();
  }
}
