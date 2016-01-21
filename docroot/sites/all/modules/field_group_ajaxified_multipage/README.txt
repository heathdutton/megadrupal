DESCRIPTION
============

By using Field Group it's possible to easily turn an entity form into a multipage form, however
The paging is can only be done on client side which has several disadvantages
1. Validation of form fields is very basic, real validation is done after the form is submitted
   and often user will have to return to previous pages to correct the entered values
2. More complex multi page forms often dynamically change what user see in next steps depending on the
   data entered in previous steps, now that the form pages are ajaxified there is no limitation
3. When the form is complex and has many pages with different fields it can become considerably heavy
   to load since by using only javascript for hiding / showing related fields to each page, the whole
   form needs to be loaded

This module addresses these two issues by making it possible to enable ajax for multipage groups
It also provides Developers with special variables attached to form array regarding page status
to allow them to further customize the form depending on active page

Also it <strong>works with any forms even custom ones!</strong>

INSTALLATION
============

- Install and enable field_group module
- Enable the module at Administer >> Modules.

API
============
There is a similar hook like hook_form_alter that works the same way, it allows other modules to alter
the forms that are altered by this module. note that it gets called only when the form is mulipaged using this module

  hook_field_group_ajaxified_multipage_form_alter();
  hook_field_group_ajaxified_multipage_form_BASE_FORM_ID_alter();
  hook_field_group_ajaxified_multipage_form_FORM_ID_alter();

/**
 * Implementation of hook field_group_ajaxified_multipage_form
 */
function fgam_example_field_group_ajaxified_multipage_form_alter(&$form, &$form_state, $form_id) {
}

USAGE
============

- Setup your multipage field groups as usual for your entity/node
- In your entity/node fields list , edit your multipage field group properties and check Ajaxify option
- You can also enable only "Non Javascript Multistep" option which does not require javascript to work.
  It's very useful for debugging purposes or very complex multistep forms.
  remember that the ajaxify should be disabled for this to work
- Skip button : You can add skip button for any of the steps you like, simply clone the next button added by this module and change it's title to t('Skip this step')

If you're a developer and like to implement more complex multi page forms you can use the following
variables
 - $form_state['field_group_ajaxified_multipage_enabled']
 - $form_state['field_group_ajaxified_multipage_group']

Sample code for using these variables :
<?php
function hook_form_alter(&$form, &$form_state, $form_id) {
  if (isset($form_state['field_group_ajaxified_multipage_enabled']))
  if ($form_state['field_group_ajaxified_multipage_enabled']) {
    $step = empty($form_state['storage']['field_group_ajaxified_multipage_step']) ? 1 : $form_state['storage']['field_group_ajaxified_multipage_step'];
    $page_group = $form_state['field_group_ajaxified_multipage_group'];

    //Best practive for accessing variables, it works even when this ajax grouping is disabled
    if (isset($form_state['values'])) {
      $values = $form_state['values'];
      if (isset($form_state['field_group_ajaxified_multipage_enabled']) && isset($form_state['storage']['all']['values'])) {
        $values = $form_state['storage']['all']['values'];
      }
    }

    if ($page_group['children'][$step-1] == 'group_two') {
      $form['actions']['skip'] = $form['actions']['next'];
      $form['actions']['skip']['#value'] = t('Skip this step');
      $form['actions']['skip']['#limit_validation_errors'] = array();
    }

    if ($step == count($page_group['children'])) {
      unset($form['actions']['preview']);
    }
  }
}
?>

Sample code for enabling for custom developed forms :
<?php
function myform() {
    $form['#groups_custom'] = array (
      'group_measurements' => array(
        'group_name' => 'group_measurements',
        'label' => 'Measurements',
        'format_type' => 'multipage',
        'children' => array (
          'gender',
          'birthday',
        ),
      ),
      'group_goal' => array(
        'group_name' => 'group_goal',
        'label' => 'Goal',
        'format_type' => 'multipage',
        'children' => array (
          'overall_objectiv',
          'duration',
        ),
      ),
      'group_steps' => array(
        'group_name' => 'group_steps',
        'label' => 'Steps',
        'format_type' => 'multipage-group',
        'children' => array (
          'group_measurements',
          'group_goal',
        ),
        'format_settings' => array (
          'label' => 'Steps',
          'instance_settings' => array (
            'ajaxify' => '1',
            'nonjs_multistep' => '0',
            'classes' => ' group-steps field-group-multipage-group',
            'page_header' => '3',
            'page_counter' => '1',
            'move_button' => '1',
            'move_additional' => '1',
          ),
        ),
      ),
    );
}
?>

USAGE
============
FORM API image buttons are not supported by diffault.

DEVELOPERS
===========

Sina Salek (http://sina.salek.ws)
Sebastian Gurlt (https://www.drupal.org/u/sg88)