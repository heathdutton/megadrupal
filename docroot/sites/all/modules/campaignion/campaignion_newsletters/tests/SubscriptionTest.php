<?php

namespace Drupal\campaignion_newsletters;

class SubscriptionTest extends \Drupal\Tests\DrupalWebTestCase {
  public function test_byData_doesntDuplicate() {
    $email = 'bydataduplicate@test.com';
    $list_id = 4711;
    $s = Subscription::byData($list_id, $email);
    $s->save();
    Subscription::byData($list_id, $email);
    $s->save();
    $this->assertFalse($s->isNew());
    $this->assertEqual(1, count(Subscription::byEmail($email)));
    $s->delete();
    $this->assertTrue($s->isNew());
    $this->assertEqual(0, count(Subscription::byEmail($email)));
  }

  public function test_delete_worksForNonExisting() {
    Subscription::fromData(4711, 'this@doesnot.exist')->delete();
  }
}
