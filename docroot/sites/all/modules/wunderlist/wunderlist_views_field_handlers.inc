<?php
/**
 * @file
 * Views handlers for Wunderlist module.
 */

/**
 * Class wunderlist_views_handler_field_avatar.
 */
class WunderlistViewsHandlerFieldAvatar extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    $value = $values->{$this->field_alias};
    return '<img src="data:image/png;base64,' . $value . '" />';
  }

}
