CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 

INTRODUCTION
------------
Drupal integration with Semantic UI (http://semantic-ui.com/)
Provides Icon API bundle and Integrates Semantic UI API to be used by other
modules.

Semantic UI not only provides integration with Icon API but also provides a lot
of other inbuilt components such as Popups, Form Elements, validations, Buttons,
Classic dropdowns, Navigation/Breadcrumb, menus and much more..!

About Semantic UI collections: http://semantic-ui.com/collection.html
Elements:- http://semantic-ui.com/elements/image.html
Icons:- semantic-ui.com/elements/icon.html

Github: https://github.com/semantic-org/semantic-ui/


INSTALLATION
------------
1. Download and Install dependencies.
   i.e. Libraries Module(https://drupal.org/project/libraries).

2. Download "Semantic UI API" module 
  from - https://drupal.org/project/semantic_ui_api
and place inside your module directory. ("sites/all/modules/contrib").

3. Download & extract semantic.zip file of "Semantic UI" 
  from - http://semantic-ui.com
  From archive contents copy all contents of "packaged" directory
  into -  "sites/all/libraries/semantic_ui" directory.

4. Confirm that "semantic.min.css" file is
  inside "sites/all/libraries/semantic_ui/css" directory
  and "semantic.min.js" file is inside
  "sites/all/libraries/semantic_ui/javascript" directory.

5. Enable the module at Administer >> Site building >> Modules.

6. Now Semantic UI library should be available for use. To check if API is
  available you can visit: "/admin/help/semantic_ui_api" if you are able to see
  "Flag" icon it means library is working properly.

7. You can further use "Semantic UI" in three different ways:-
    a) Stand Alone: Within HTML:-To use stand alone you can use HTML tag "i"
       with classes which are available in library. Documentation for these
       classes is available in Semantic UI website. E.g If you need "Flag"
       Icon within your Node body field (or any other HTML field) you can use
       html as: <i class="icon flag teal big"></i>

    b) With Icon API: As a Icon Bundle for Icon API: To use as a Icon set with
       Icon API you will need to download and enable Icon API module. Once
       enabled Semantic UI bundle will be available to be used with Icon API.

    c) use it in themes: (developers): Developers can use this library in their
       themes as they will need to assign proper classes for theme elements
       (e.g. buttons, textfields,tables, images etc..). e.g a Simple theme
       hook_form_alter() may look like:-

/**
 * @desc Implements hook_form_alter()
 * Providing better UI elements support for FORM elements.
 */
function nws_omega_form_alter(&$form, &$form_state, $form_id) {
  $form['#attributes']['class'][] = 'ui form';
  if (isset($form['actions'])) {
    foreach ($form['actions'] as $action_key => $action) {
      if (isset($action['#type']) && $action['#type'] == 'submit') {
        $form['actions'][$action_key]['#attributes']['class'][] = 'ui button';
      }
    }
  }
}

8. If you are getting difficulties or facing any issues while using this module
   please visit project issue page and raise an issue.
