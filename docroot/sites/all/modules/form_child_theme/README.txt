Form Child Theme Module

Features
--------

Allow form checkboxes and radios elements with '#child_theme' to theme their
child elements with the specified theming function.


Requirements
------------

None


Installation instructions
-------------------------

Enable Form Child Theme module at your site.

When declaring checkboxes or radios elements in your forms,
add the '#child_theme' variable to the array the same way
as you would with the '#theme' variable.


Usage
-----

$form['checkboxes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('My Checkboxes'),
    '#description' => t('An example declaration of checkboxes'),
    '#options' => $options,
    '#default_value' => $default_value,
    '#child_theme' => 'my_checkbox_theme',
);

Each of the expanded checkboxes will now have '#theme' set to 'my_checkbox_theme'.

$form['radios'] = array(
    '#type' => 'radios',
    '#title' => t('My Radios'),
    '#description' => t('An example declaration of radios'),
    '#options' => $options,
    '#default_value' => $default_value,
    '#child_theme' => 'my_radio_theme',
);

Each of the expanded radios will now have '#theme' set to 'my_radio_theme'.