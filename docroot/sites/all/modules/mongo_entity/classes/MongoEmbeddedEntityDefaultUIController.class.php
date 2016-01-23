<?php

class MongoEmbeddedEntityDefaultUIController extends EntityDefaultUIController {

  public function hook_menu() {

    $plural_label = isset($this->entityInfo['plural label']) ? $this->entityInfo['plural label'] : $this->entityInfo['label'] . 's';

    $items = array();
    $items[$this->path . '/manage'] = array(
      'title' => $plural_label,
      'page callback' => 'drupal_goto',
      'page arguments' => array($this->path . '/manage/fields'),
      // drupal_goto will provide access control. might as well make sure the user isn't anonymous
      'access callback' => 'user_is_logged_in',
      // Use an unlike menu name to prevent this from appearing in the navigation menu
      'menu_name' => 'null',
    );

    return $items;
  }
}
