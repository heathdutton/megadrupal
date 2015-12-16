<?php

/**
 * @file
 * Contains the style plugin.
 */

/**
 * Style plugin to render each item in a circleslider.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_image_circleslider extends views_plugin_style {
  /**
   * Defines format definitions.
   */
  public function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  /**
   * Defines format options.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  /**
   * Make sure the display and all associated handlers are valid.
   *
   * @return array
   *   Empty array if the display is valid; an array of error strings if it not.
   */
  public function validate() {
    $row_handler = $this->row_plugin->definition['handler'];
    switch ($row_handler) {
      case 'views_plugin_row_image_circleslider':
        $errors = array();
        $row_fields = $this->row_plugin->options;
        // Check to make sure image fields aren't empty.
        foreach ($row_fields as $title => $result) {
          if (empty($result)) {
            $errors[] = t('Image Circleslider requires you to assign a field for "@field". Currently the "@field" is empty. Click \'Row Style: Image Circleslider Fields\' to set.', array('@field' => $title));
          }
        }
        // Check to make sure that two of the same image fields aren't selected.
        $results = array();
        foreach ($row_fields as $title => $result) {
          if ($result && in_array($result, $results)) {
            $errors[] = t("Image Circleslider does not allow you to use the same field instance twice. You can use the same field twice, and are encouraged to for images. You just need to make two instances of the same field.  To do add the same field twice under 'Fields'. Give each a different label. Then select each instance of that field once under 'Row style: image circleslider Fields'.");
          }
          $results[] = $result;
        }
        return $errors;
    }
  }

}
