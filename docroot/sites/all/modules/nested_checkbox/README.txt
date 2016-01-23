-- SUMMARY --

This module implements jQuery tristate checkbox plugin 
(http://jlbruno.github.io/jQuery-Tristate-Checkbox-plugin/) as form element.
In your form definitions you can use $element['#type'] = 'nested_checkboxes'. 

-- REQUIREMENTS --

jQuery Update (jQuery v1.6 or later)
Libraries API

-- INSTALLATION --

To make this module work, you should download jQuery tristate checkbox plugin 
(http://jlbruno.github.io/jQuery-Tristate-Checkbox-plugin/) and extract it in 
the "libraries" folder of your site (renaming if necessary) so that the path 
is libraries/tristate/jquery.tristate.min.js
In jQuery Update settings, select jQuery v1.7 or 1.8 
Then install module as usual.

-- CONFIGURATION --

This module provides a new form element, 'nested_checkboxes'. Its most important
property is '#options'. It should come as a nested array, and for each level you 
should also indicate '#title' element.

Full example of usage:
  $form['checkboxes'] = array(
    '#type' => 'nested_checkboxes',
    '#attributes' => array(
      'class' => array('testclass')
    ),
    '#options' => array(
      'subset1' => t("Sub Set 1"),
      'subset2' => array(
        '#title' => t('Sub Set 2'),
        '#attributes' => array(
          'class' => array('testclass1')
        ),
        '#options' => array(
          'option1' => t('Option1'),
          'option2' => array(
            '#title' => t('Inner level 1'),
            '#options' => array(
              'option1' => t('Option1'),
              'option2' => t('Option2')
            )
          )
        )
      ),
      'subset3' => t('Sub Set 3'),
    )
  )

-- CUSTOMIZATION --

The list of checkboxes is implemented as HTML unordered list. 
Its element have classes "nested-checkboxes" and "level-n", where n is the depth
of current level. Custom theming may be implemented by adding attributes (id
or classes) in form definition or by redefining theme_nested_checkboxes() and
theme_nested_checkboxes_inner(). The former is responsible just for outer ul
tag, so redefining the latter will make more sense.
