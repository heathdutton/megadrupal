## Style (CSS) Settings module ##

The content of this file is based on the online documentation that can be found
at https://www.drupal.org/node/2527412. It is recommended to read it there as it
is improved, contains links and images, and code examples are syntax
highlighted.

https://drupal.org/project/style_settings is intended for theme and module
maintainers but can be used for customisations by anyone with basic coding
skills (for example to provide a patch for projects that could use some CSS to
be configurable). It might simply be needed because a theme or module declares
it as a (soft-)dependency.

Allows any module or theme to have their CSS attributes configurable from the UI
just by wrapping default CSS values (plus unit) in a code comment. These are
then substituted in a separate rewritten CSS file by Drupal variables or theme
settings. The CSS is functional even if this module is not installed.

All is cached so the performance impact is neglectable. The rewritten CSS files
are included in the CSS aggregation mechanism.


### How to add configurable CSS to your module or theme? ###

- For modules and themes add to your '.info' file:
    soft_dependencies[] = style_settings
  OR if you want to make the Style (CSS) Settings module required:
    dependencies[] = style_settings
  For a site's custom CSS just put it in the existing dedicated file at
  'sites/all/modules/style_settings/css/custom.css'. This is in fact an ideal
  way to give a webmaster limited and validated control over a site's styling
  through a UI.
- Wrap CSS values you want to be configurable in comments, including its unit
  (px, em, %) if present.
  An example for a module:
    font-size: 80%;
  becomes:
    font-size: /*variable:FOO_fontsize*/80%/*variable*/;
  An example for a theme:
    font-size: 16px;
  becomes:
    font-size: /*setting:YOURTHEME_base_fontsize*/16px/*setting*/;
  The wrapped values are used as the default. Because comments cannot reside
  within url(...), you need to add the comments around it.
- Provide form elements on your settings page to configure module variables or
  theme settings (see below).

#### Note 1 ####
No code comments are allowed between a CSS value and its unit.

Something like:
  border-width: /*variable:FOO_thickness*/2/*variable*/px;
results in invalid CSS and is therefore ignored by browsers. Use instead:
  border-width: /*variable:FOO_thickness*/2px/*variable*/;

The Style Settings form element 'Number' (see below) takes care of putting a
measurement unit behind the value when storing it in a variable.

#### Note 2 ####
For readability it is recommended not to set the values of several CSS
properties simultaneously if it contains inline code comments.
Instead of:
  border: solid /*variable:FOO_borderwidth*/2px/*variable*/ /*variable:FOO_bordercolor*/Red/*variable*/;
use:
  border-style: solid;
  border-width: /*variable:FOO_borderwidth*/2px/*variable*/;
  border-color: /*variable:FOO_bordercolor*/Red/*variable*/;


### How to provide form elements on your settings page? ###

#### Add a conditional ####

Settings can be added to your settings page the usual way. If you declare the
Style (CSS) Settings module only as a soft-dependency (thus not required), it
makes sense to show settings only if the Style Settings module is enabled by
wrapping all Style Settings specific form elements inside a conditional:

  if (module_exists('style_settings')) { ... }

#### Add a form fields ####

Developers can use any form API element on their settings page to capture a
theme setting or variable. The Style Settings module offers also a few custom
form API element to make building CSS settings easier. Cool UI widgets and
built-in validation of user input. For example a color picker or a slider.

For project maintainers adding a Style Setting field to their configuration is
as easy adding a few lines of code. Examples how to provide different types of
Style Settings can be found in the 'code snippets' section below.

##### Number #####
'#type' => 'style_settings_number',
- Takes care of the validation just by providing a min/max and step value.
- Adds a unit if valid. Input: '2', field_suffix: 'px' => stored variable: '2px'
- Aligns field input to the right to stay close to the unit (field_suffix).

Accepted attributes, the same as a 'textfield' plus:
'#min' => [minimum, defaults to 0],
'#max' => [maximum, optional],
'#step' => [multiple of, optional, if set to 1 accept only integers],
'#input_help' => [show above values as field help text, defaults to TRUE],

##### ColorPicker #####
A lot of CSS attributes contain a color value. Although a simple text field
could hold a color hex value, having a colorpicker is more convenient.

'#type' =>  'style_settings_colorpicker',
- Uses Drupal core's Farbtastic ColorPicker plugin.
- Makes the current choosen color visible in the settings field itself.
- Only accepts a valid color, a hex value but also a color name.
- Does not depend on any contrib or the Color module.
- Does not need an additional jQuery library.
- Stores the variable as a hex value or color name that is valid in CSS files.

##### Slider (HTML5 range input) #####
'#type' => 'style_settings_slider',
- Exposes a slider widget (a range) in HTML5 capable browsers (all but <= IE9).
- Shows the numeric value that corresponds with the handle position.
- Validation just by providing a min/max and step value.
- Adds a unit if valid. Input: '1', field_suffix: 'em' => stored variable: '1em'

Accepted attributes, the same as a 'textfield' plus:
'#min' => [minimum, defaults to 0],
'#max' => [maximum, defaults to 1],
'#step' => [multiple of, defaults to 0.01],
The defaults are preset for the CSS attribute 'opacity' but can be overriden.

##### Image URL #####
'#type' => 'style_settings_imgurl',
- Accepts an absolute or relative (from base) URL.
- If not empty, validates the URL syntax, if it is an image and exists (no 404).

#### More info ####

For more information on custom theme settings, read:
  https://www.drupal.org/node/177868
A simple example of theme settings is Drupal core's
'/themes/garland/theme-settings.php' and '/themes/garland/garland.info'.

For more information on custom module settings, read:
  https://www.drupal.org/node/1111260
A simple but complete example of module settings is Drupal core's
'/modules/update/update.settings.inc'.

##### A selectable measurement unit (e.g. px, em, %) #####
To have a unit select widget after a number field :
- Put both in a fieldset with the class 'container-inline'.
- Make sure the number field has NO '#field_suffix'.
- The unit '#type' => 'select' should be '#required' => TRUE.
- In the form submit handler concatenate the value and unit variables in a new
  one.

Examples how to provide different types of Style Settings can be found in the
'code snippets' section below. Alternatively you can take a look at how things
are done in the module's demo at 'sites/all/modules/style_settings/demo'. It
includes also an example of a 'preview area' to make style changes visible in
the settings form itself.


### Code snippet examples ###

Put the following comment at the top of CSS files that contain style variables:

/**
 * The CSS values that are wrapped in '/*variable' comments are intended for use
 * by https://www.drupal.org/project/style_settings. Enable that module to
 * have those CSS variables exposed in the settings UI.
 */

An example for FOO.admin.inc:

Replace FOO with your module's machine name.

<?php
/**
 * @file
 * The admin settings for the FOO module.
 */
/**
 * Implements hook_settings().
 */
function FOO_admin_settings() {
  // Put CSS variables together in a fieldset. Remove if only one is given.
  $form['css_variables'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS variables'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  if (module_exists('style_settings')) {
    // When we have a list of CSS variables it is better to collapse it.
    $form['css_variables']['#collapsed'] = TRUE;
    //
    // Any normal form API type can be used 
    // A normal TEXTFIELD. It does not offer any validation by itself.
    $form['css_variables']['FOO_caption_align'] = array(
      '#type' => 'textfield',
      '#title' => t('Caption text align'),
      '#default_value' => variable_get('FOO_caption_align', 'center'),
      '#size' => 12,
    );
    //
    // The Style Settings module offers several form API elements to help
    // developers build the settings page. Cool UI widgets and built-in
    // validation of user input especially designed for CSS attributes.
    //
    // NUMBER example in this case with an appended measurement unit (optional).
    // E.g. user input: '2', field_suffix: 'px' => stored variable: '2px'.
    $form['css_variables']['FOO_borderwidth'] = array(
      '#type' => 'style_settings_number',
      '#title' => t('Border width'),
      '#step' => 1, // In this case if forces an integer as input.
      '#min' => 0, // Could be omitted. Defaults to 0. Set to NULL to ignore.
      '#max' => 10, // Defaults to 1 if omitted.
      // The variable default should include a measurement unit if applicable.
      // Wrapped in floatval() to turn it into a number. E.g. '2px' => '2'.
      '#default_value' => floatval(variable_get('FOO_borderwidth', '2px')),
      // The suffix gets added to the input on submit if valid measurement unit.
      '#field_suffix' => 'px',
      // Uncomment the line below to NOT align the field input on the right.
//      '#attributes' => NULL,
      // Uncomment the line below to NOT show min, max and step values as help.
//      '#input_help' => NULL,
    );
    //
    // COLOR PICKER example.
    $form['css_variables']['FOO_bordercolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Border color'),
      // Besides hex color value also color names are accepted.
      '#default_value' => variable_get('FOO_bordercolor', 'IndianRed'),
    );
    //
    // SLIDER WIDGET example.
    $form['css_variables']['FOO_magnifier_icon_opacity'] = array(
      '#type' => 'style_settings_slider',
      '#title' => t('Magnifier icon opacity'),
      '#description' => t('0 = transparent. 1 = opaque.'),
      // The variable default should include a measurement unit if applicable.
      // Wrapped in floatval() to turn it into a number. E.g. '2px' => '2'.
      '#default_value' => floatval(variable_get('FOO_magnifier_icon_opacity', 0.85)),
      // Parameters below could be omitted. It is already preset for opacity.
      '#step' => 0.01,
      '#min' => 0,
      '#max' => 1,
      // The suffix gets added to the input on submit if valid measurement unit.
      // Added for demonstration purpose only.
      'field_suffix' => NULL,
    );
    //
    // IMAGE URL example.
    $form['css_variables']['FOO_bgimage'] = array(
      '#type' => 'style_settings_imgurl',
      '#title' => t('Background image'),
      // If you use this for a 'theme', replace 'module' below.
      '#default_value' => variable_get('FOO_bgimage', '/' . drupal_get_path('module', 'image') . '/sample.png'),
      // In the submit handler below we reset an empty field to the default URL.
      // This way the user isn't required to know the URL of the default image.
      '#description' => t('An absolute (external) or relative (local) image URL. A relative URL must be given from the base URL (<em>/sites/..</em>). Leave empty to reset to the default image.'),
    );
    //
    // A SELECTABLE MEASUREMENT UNIT (e.g. px, em, %) example. It goes together
    // with a submit handler inside the function FOO_admin_settings_submit().
    $form['css_variables']['FOO_fontsize'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Caption font-size'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      // Add optionally a field description in the fieldset. NOT in the elements below.
      // Number field input help (min, max, step) will optionally be appended.
      '#description' => t('Note: A minimum font-size setting of your browser might interfere.'),
    );
    // Number field without a '#field_suffix'.
    $form['css_variables']['FOO_fontsize']['FOO_fontsize_value'] = array(
      '#type' => 'style_settings_number',
      '#default_value' => variable_get('FOO_fontsize_value', '85'),
      // Uncomment the line below to NOT show min, max and step values as help.
//      '#input_help' => NULL,
    );
    // A measurement unit select field.
    $form['css_variables']['FOO_fontsize']['FOO_fontsize_unit'] = array(
      '#type' => 'select',
      '#options' => array(
        'px' => t('px'),
        'em' => t('em'),
        '%' => t('%'),
      ),
      '#default_value' => variable_get('FOO_fontsize_unit', '%'),
      '#required' => TRUE,
    );
  }
  //
  // If the Style Settings module is not enabled, provide some instructions.
  else {
    $style_settings_module = l(t('Style (CSS) Settings module'), 'https://drupal.org/project/style_settings', array(
        'attributes' => array(
          'title' => t('Style (CSS) Settings | Drupal.org'),
          'target' => '_blank',
        ),
    ));
    $form['css_variables']['FOO_note'] = array(
      '#markup' => t("Enable the !style_settings_module to get style options exposed here. They consist of:<ul>
          <li> A caption font-size.</li>
          <li> ... </li>
          <li> ... </li>
        </ul>", array('!style_settings_module' => $style_settings_module)),
    );
  }
  //
  // Call submit_function() on form submission.
  $form['#submit'][] = 'FOO_admin_settings_submit';
  return system_settings_form($form);
}
/**
 * Submit form data.
 */
function FOO_admin_settings_submit($form, &$form_state) {
  if (module_exists('style_settings')) {
    // IMAGE URL: Reset to default if empty. Does not work after a hook_form_FORM_ID_alter().
    // In that case move it to the submit handler after hook_settings() in the 'parent' form.
    if (trim($form_state['values']['FOO_bgimage']) == '') {
      $form_state['values']['FOO_bgimage'] = '/' . drupal_get_path('module', 'image') . '/sample.png';
      drupal_set_message(t('The image URL has been reset to the default.'), 'warning', FALSE);
    }
    //
    // SELECTABLE MEASUREMENT UNIT: concatenate the value and unit in a new
    // variable (the one that will be used in the CSS).
    variable_set('FOO_fontsize', $form_state['values']['FOO_fontsize_value'] . $form_state['values']['FOO_fontsize_unit']);
    //
    // Make sure changes are visible right after saving the settings.
    _drupal_flush_css_js();
   }
}

// END OF CODE

Note in the form submit handler:

    _drupal_flush_css_js();

This way changes are visible right away after saving the form. Otherwise it is
necessary to clear the cache after changing CSS variables at:
'/admin/config/development/performance'


### About the "cache" ###

Unnecessary rewriting of CSS files is avoided through a two step process for
each monitored CSS file.

The first step generates a unique checksum key that will be used by the second
step but only if the css/js cache has been flushed.

The second step rewrites the CSS file using the checksum key if it contains
inline setting or variable code comments AND:
- theme settings have changed
OR
- variables have changed
OR
- the original CSS file has changed.
