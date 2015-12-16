<?php

/**
 * OzPostcode class.
 */
class OzPostcodes extends Entity {
  protected function defaultLabel() {
    // Weird that I have to check for this, I thought Entity API would do it.
    if(!isset($this->is_new) || !$this->is_new){
      $label = $this->postcode . ' ' . $this->suburb . ' ' . $this->state;
      drupal_alter('oz_postcodes_default_label', $label, $this);
      return $label;
    }
  }
}

/**
 * OzPostcodeController class.
 */
class OzPostcodesController extends EntityAPIController {
  // Maybe something here in the future.
}

/**
 * OzPostcodeViewsController class.
 * Entity API automagically implements HOOK_views_data() for you,
 * but you can override the defaults here.
 */
class OzPostcodesViewsController extends EntityDefaultViewsController {
  public function views_data() {
    $data = parent::views_data();
    $data['oz_postcodes']['state']['filter']['handler'] = 'oz_postcodes_views_handler_filter_state';
    return $data;
  }
}

/**
 * OzPostcodeUIController class.
 */
class OzPostcodesUIController extends EntityDefaultUIController {
  // Maybe something here in the future.
}
