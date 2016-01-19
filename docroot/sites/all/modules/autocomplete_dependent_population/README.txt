CONTENTS OF THIS FILE
---------------------
* Introduction
* Installation
* Configuration
* Maintainers

Introduction
------------
This module provides ability to populate multiple dependent form elements of an 
autocomplete form element with proper values. This module introduces 
"autocomplete_dependee" type form element which allows to create an 
autocomplete field. You can define form elements which will be populated when 
the value from autocomplete suggestion is selected. The dependent form elements 
can be of following type:

Textfield
Textarea
Dropdown
Radio button
Checkboxes

A form can have multiple "autocomplete_dependee" type form elements. 

Installation
--------------
Copy autocomplete_dependent_population.module to your module directory and 
then enable on the admin modules page. 
Enable both the modules 'Autocomplete Dependent Population' and 
'Autocomplete Dependent Population Example'.

Click on the 'help' link of 'Autocomplete Dependent Population Example'. It will
redirect you a help page containing links of different examples.

Configuration
-------------
1] Create a form element of type "autocomplete_dependee"

$form['field1'] = array(
  '#type' => 'autocomplete_dependee',
  '#title' => t('Field1'),
  '#path' => 'autocomplete_dependent_test_one_callback', 
);

Suppose it's dependent form elements are

$form['field2'] = array(
  '#type' => 'textfield',
  '#title' => t('Field2'),
  '#default_value' => '',  
 );
 
 $form['field3'] = array(
  '#type' => 'textarea',
  '#title' => t('Field3'),
  '#default_value' => '',   
 );
 
 $form['field4'] = array(
  '#type' => 'select',
  '#title' => t('Field4'),
  '#options' => array('' => 'select', 
                      'select1' => 'select1', 
                      'select2' => 'select2', 
                      'select3' => 'select3')
   );
 
 $form['field5'] = array(
  '#type' => 'radios',
  '#title' => t('Field5'),
  '#options' => array(0 => 'radio1', 1 => 'radio2', 2 => 'radio3')
 );
 
 $form['field6'] = array(
  '#type' => 'checkboxes',
  '#title' => t('Field6'),
  '#options' => drupal_map_assoc(array(t('checkbox1'), 
                                 t('checkbox2'), 
                                 t('checkbox3'))),
  );
  
Here 'autocomplete_dependent_test_one_callback' is Drupal path which invoke a 
callback function which will provide the actual results depending on the 
business logic of your application. Create a Drupal path using hook_menu and
create a callback function to provide the output relevant to your application. 
Now within your callback function implement the following line of codes to 
return output which will actually provide the autocomplete suggestion list and 
it's dependent fields values.

$matches['data'][value of 'field1']['textfield']['field2'] = 'product 1';
$matches['data'][value of 'field1']['textarea']['field3'] = 'desc for type 1'; 
$matches['data'][value of 'field1']['select']['field4'] = 'select2';
$matches['data'][value of 'field1']['radio']['field5'] = 1;
$matches['data'][value of 'field1']['checkbox']['field6'] = 'checkbox1, 
                                                             checkbox2';

drupal_json_output($matches);

'data' is a placeholder which is needed to generate the output. Keep it same.
value of 'field1' is the value of autocomplete field itself which will appear as 
suggestion list. 'textfield', 'textarea' etc are the types of dependent form 
elements.'field2', 'field3' etc are the name of the dependent form elements.

For checkboxes, in case you want multiple checkboxes to be selected/populated 
concatenate the values like 'checkbox1, checkbox2'.

The $matches array will be within loop to provide multiple values in the 
suggestion list.(Same as Drupal's default autocomplete suggestion)

Check autocomplete_dependendet.test.inc for further help.

Maintainers
-------------
Debarya Das
debarya2009@gmail.com
