<?php

if (!empty($actions)) {
  foreach ($actions as $action) {
    $title = $action->Title ? $action->Title : t('(No Title)');
    $description = $action->Description ? '<p>' . $action->Description . '</p>' : '';
    echo '<h3>' . l($title, 'salsa/action/' . $action->action_KEY) . '</h3>';
    echo $description;
  }
}
