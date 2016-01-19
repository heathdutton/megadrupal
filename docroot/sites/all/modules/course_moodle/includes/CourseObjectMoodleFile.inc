<?php

class CourseObjectMoodleFile extends CourseObjectMoodleResource {

  public function __construct($config = array(), $account = NULL, \Course $course = NULL) {
    parent::__construct($config, $account, $course);

    $this->moodleModuleType = ($config->object_type == 'folder') ? 'folder' : 'resource';
  }

}
