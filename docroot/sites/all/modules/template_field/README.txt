### Components of a Template

## The Template Itself

The template will take the form of a ctools exportable.  Theses can be defined in hook_template_api_templates()

<?php
$template['awesome'] =
  array(
    'renderer' = "the_renderer_plugin"
    'inputs' => array(
      'var1' => array(
        'label' => t('label')
        'type' => 'the_input_type_plugin'
        'other depending on type' => input types can define custom properties for the input,
      ),
    ),
    ‘content’ => ‘filename or template content’,
    ‘attached’ => array( // Anything to be attached to the render array
      ‘css’ => '', // filename or css content
      ‘js’ => '', // filename or js content
      ‘libraries’ => array( // library name or jquery_ui library name
        array(
          'name' => '',
          'module' => '' // 'name of the module that the library is defined in.  @see http://api.drupal.org/api/drupal/modules--system--system.api.php/function/hook_library/7
        ),
      ),
    ),
    ‘label’ => t(‘My awesome template’),
    ‘tags’ => array(‘page-template', 'block-template’),
  );
?>

Any module using the $template has the use to the following actions on the controller.
It is recommend that you implement each of these actions.
@see template_field for an example
@see TemplateApiTemplateController for method definitions

view - Render the template
getForm - Get the FormAPI form array to use for content input used with the template
validate - validate the form for the content use with the template
isEmpty - is the template empty
update - content being used with the template is being updated
insert - new content is being inserted that is going to be used with this template
delete - content used with the template is being delete



## Renderer

The renderer is in charge of rendering the content from the html template file.
Each renderer is defined by hook_template_api_renderers().
Each renderer must implement the interface TemplateApiRendererInterface in order to be valid.
The renderer will be passed an array of the defined variable along with their values.

<?php

function hook_template_api_renderers() {
  $renderers = array();
  $renderers['query_path'] = array(
    'class' => 'TemplateQueryPath',
    'label' => 'Query Path',
  );
  return $renderers;
}

?>



## Input Type

The input type defines the type of the input that will be displayed to the user when they are entering content.
Each input type is defined by hook_template_api_input_types()
Each renderer must implement the interface TemplateApiInputTypeInterface in order to be valid.
The input type class will take a template object and return a form api compliant array.

<?php

function hook_template_api_input_types() {
  $input_types = array();
  $input_types['text'] = array(
    'class' => 'TemplateTextInput',
    'label' => t('Text'),
  );

  return $input_types;
}
?>

Available Input Types

* String - single line text field.
* Text - textarea w/o input formats.
* text_format - textarea w/input formats
  - Adds a "format" attribute on the template definition that can be used to set the input format.
  - The format attirbute is optional and defaults to filter_default_format()
* Select - simple select w/ options
  - Add an "options" attribute on the template definition that expect an array for the select options.
* file - A file uploader
* Vertical Tabs
* Hidden - Add a hard-coded value in your template definition with no user interface for changing it.
