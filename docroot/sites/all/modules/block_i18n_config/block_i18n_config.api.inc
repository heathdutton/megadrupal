<?php

/**
 * @file
 * API documentation.
 */

/**
 * hook_block_i18n_config_info().
 *
 * Here we are registering our custom properties from our custom configuration.
 * After registering them we can get them translated by using
 * block_i18n_config_get_properties() and block_i18n_config_get_property()
 * functions with proper parameters.
 *
 * How does it work:
 * Your property needs to be registered properly to be visible on "translate
 * page", and also its value in default language needs to be saved in
 * i18n_string table as a source translation. The source translation needs to
 * be changed every time original value changes. This module does exactly that.
 *
 * @see block_i18n_config_get_properties()
 * @see block_i18n_config_get_property()
 * @see block_i18n_config_source_translation_update()
 */
function hook_block_i18n_config_info() {
  return array(

    // Name of our property
    'my_property' => array(

      // Name of our module, we actually need this info per property because
      // of how i18n_block saves translations. Required.
      'module' => 'mymodule',

      // Blocks our property belongs to, obviously required.
      'blocks' => array('myblock1', 'myblock2'),

      // We need to provide a callback function that would provide our property
      // value to be translated. Callback will get two variables - property name,
      // and delta. It should return a value or array with keys "value"
      // and "format" if property have associated format. Required.
      'property getter' => '',

      // If set, this allows for automatic saving of translation source.
      // This basically should be #parents array of property form element.
      // If you have simple form with all values on the same flat level,
      // you can simply use here only a key of form element that handles
      // our value.
      // For example, if you have simple form like this:
      //
      // hook_block_configure($delta == '') {
      //   $form = array();
      //   if ($delta == "myblock") {
      //     $form['myproperty'] = array(
      //       '#title' => t('Some title'),
      //       '#type' => 'textfield',
      //       '#default_value' => 'some value',
      //     }
      //   }
      // return $form;
      // }
      //
      // Then you can use here 'property parents' => array('myproperty'),
      // and it should suffice for automatic source translation update.
      //
      // This key is optional. You can as well use
      // block_i18n_config_source_translation_update() function to update
      // source translation manually on hook_block_save() .
      'property parents' => array('parent1', 'parent2'),

      // This is actual info about our property that will be used to register
      // it in i18n_object_info registry on block object. At least title
      // needs to be given. If property is using specific format, you need also
      // to specify "format" key here with the string as a value. This value of
      // "format" will be used as name of the variable in i18n_string object
      // which means that it should be unique. Most of the time name of the
      // variable + "_format" suffix should suffice.
      'info' => array(
        'title' => t('Some title!'),
        'format' => 'my_property_format',
      ),
    ),
  );
}
