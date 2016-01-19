<?php

function extern_server_course($course) {
  global $USER;
  if ($USER->id != 2) {
    return $_COOKIE['course_moodle_course_url'];
  }
}
