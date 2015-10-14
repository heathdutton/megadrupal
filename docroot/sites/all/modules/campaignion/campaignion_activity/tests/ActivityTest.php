<?php

namespace Drupal\campaignion;

class ActivityTest extends \DrupalWebTestCase {
  function testCRUD_allData() {
    $activity = new Activity();
    $activity->created = time();
    $activity->type = 'activity_test_type';
    $activity->contact_id = 21;
    $activity->save();
    
    $loaded_activity = Activity::load($activity->activity_id);
    $this->assertEqual($activity, $loaded_activity);
    
    $activity->delete();
    $loaded_activity = Activity::load($activity->activity_id);
    $this->assertFalse($loaded_activity);
  }
}
