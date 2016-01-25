CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Usability


INTRODUCTION
------------

Current Maintainer: Rafael Pedrosanto - http://drupal.org/user/309356

Defines a new form element of type "radiogrid" to be used with survey and
questionnaire types of forms.


INSTALLATION
------------

Just enable the modules in the Modules page and the new element type will
become available.


USABILITY
-------------

To use RadioGrid just add an element of type "radiogrid" to your form like
this:

$form['radiogrid'] = array(
  '#type' => 'radiogrid',
  '#title' => 'Lorem ipsum',
  '#required' => TRUE,
  '#intro_text' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. '
      . 'Pellentesque sagittis quam at sapien ultrices adipiscing. '
      . 'Maecenas turpis dui, feugiat quis volutpat a, blandit non ipsum. '
      . 'Vivamus iaculis metus vel neque consequat ac egestas leo eleifend. '
      . 'Morbi tempor, elit et scelerisque vulputate, eros leo molestie '
      . 'sapien, eu bibendum quam erat id dui. Donec ut sapien nec urna '
      . 'scelerisque posuere.',
  '#questions_head' => 'Aenean ac euismod mauris.',
  '#questions' => array(
    '1st' => 'Sed sollicitudin, metus vel pharetra ornare, metus magna mollis '
      . 'velit, ut accumsan lacus diam sit amet turpis.',
    '2nd' => 'Vestibulum dui nisl, viverra eget iaculis ac, ullamcorper '
      . 'bibendum turpis.',
    '3rd' => 'Nam nec sem mi, id rutrum tellus.',
    '4th' => 'Quisque blandit, lorem id egestas euismod, erat orci rutrum '
      . 'orci, in mollis metus mauris nec metus.'
    ),
  '#options' => array('lorem', 'ipsum', 'dolor', 'sit', 'amet'),
  '#numbered' => TRUE,
  '#outro_text' => 'Aenean non tortor velit. Praesent tellus sem, '
      . 'lobortis cursus suscipit vitae, suscipit id sem. '
      . 'Aliquam felis nisl, facilisis tincidunt imperdiet non, '
      . 'rhoncus quis libero.',
  '#default_value' => array('1st' => 1, '2nd' => 0, '3rd' => 2, '4th' => 4),
);

The above code will generate an output similar to the screenshot available
in the project's page.

In the future, I plan to add CCK (Drupal 6.x) and Field (Drupal 7.x) support
to RadioGrid.
