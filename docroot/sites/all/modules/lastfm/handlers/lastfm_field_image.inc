<?php
// $Id;

/**
 * @file
 *   Views field handler for last.fm image fields.
 */

/**
 * Views field handler for last.fm image fields.
 *
 * Image URLs are typically returned in one of four sizes - small, medium,
 * large or extra large. These are represented in our data array as 
 * <field>_<size>. We offer the user the ability to choose which size
 * to use, grab that URL, and drop it into a seprate field for the field
 * handler to pick up at render time.
 * 
 * Some images may offer more size options, in which case
 * you can override this class and implement get_image_sizes() to add 
 * or modify the sizes available. 
 */
class lastfm_field_image extends views_handler_field {
  
  /**
   * Called to add the field to a query.
   */
  function query() {  }

  function option_definition() {
    $options = parent::option_definition();
    $options['image_size'] = array('' => '-');
    return $options;
  }

  function pre_render($values) {
    // Find out which image size the user chose, grab the URL and drop it into the
    // appropriate field. 
    foreach ($values as $key => $event) {
      $chosen_image = $this->definition['lastfm']['image_type'] . '_' . $this->options['image_size'];
      $values[$key]->{$this->field} = $values[$key]->{$chosen_image};
    }    
  }

  function options_form(&$form, &$form_state) {
    // Offer the appropriate sizes to the user to choose from.
    // I suppose I could just offer these in the argument definition
    // instead of making a function and forcing people to subclass?
    parent::options_form($form, $form_state);
    $form['image_size'] = array(
      '#title' => t('Image size'),
      '#type' => 'select',
      '#default_value' => $this->options['image_size'],
      '#options' => $this->get_image_sizes(),
    );
  }
  
  /**
   * Render the field.
   *
   * @param $values
   *   The values retrieved from the database.
   */
  function render($values) {
    // Just run the URL through theme_image() and return it.
    $variables = array(
      'path' => $values->{$this->field},
    );
    return theme('image', $variables);
  }

  /**
   * Get appropriate image sizes for the images in question.
   * Classes with additional / differing image types should 
   * override this and provide their own values.
   */
  function get_image_sizes() {
    return array(
      'small' => 'Small',
      'medium' => 'Medium',
      'large' => 'Large',
      'extralarge' => 'Extra Large',
    );
  }
}