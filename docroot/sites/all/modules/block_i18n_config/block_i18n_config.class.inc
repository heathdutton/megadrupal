<?php


class block_i18n_config_object extends i18n_block_object {


  /**
   * Get object strings for translation
   */
  protected function build_properties() {

    $properties = parent::build_properties();
    $info = block_i18n_config_info();

    foreach ($info as $property_name => $data) {

      if (in_array($this->object->delta, $data['blocks'])) {
        $property = $data['property getter']($property_name, $this->object->delta);

        if (is_array($property)) {
          $this->object->$property_name = $property['value'];
          $this->object->$data['info']['format'] = $property['format'];
        }
        else {
          $this->object->$property_name = $property;
        }

      }
      else {
        unset($properties[$this->get_textgroup()][$this->object->module][$this->object->delta][$property_name]);
      }

    }

    return $properties;
  }
}