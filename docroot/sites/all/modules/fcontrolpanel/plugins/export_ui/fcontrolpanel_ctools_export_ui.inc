<?php
/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'fcontrolpanel_fcontrol', // As defined in hook_schema(), mymodule_myobj.
  'access' => 'administer fcontrolpanel', // Define a permission users must have to access these pages.
  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/structure',
    'menu item' => 'fcontrolpanel',
    'menu title' => 'Features Control Panel',
    'menu description' => 'Administer Features Control Panel Controls.',
  ),
  // Define user interface texts.
  'title singular' => t('control'),
  'title plural' => t('controls'),
  'title singular proper' => t('Features Control Panel Control'),
  'title plural proper' => t('Features Control Panel Controls'),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'fcontrolpanel_ctools_export_ui_form',
    // 'submit' and 'validate' are also valid callbacks.
  ),
  // Use this to modify the default export UI interface.
  'handler' => array(
    'class' => 'fcontrolpanel_export_ui',
    'parent' => 'ctools_export_ui',
  ),
);

/**
 * Define the preset add/edit form.
 */
function fcontrolpanel_ctools_export_ui_form(&$form, &$form_state) {
  $fcontrol = $form_state['item'];

  // fcontrolpanel's configuration interface.
  /* Apparently Ctools includes this field automatically on its own.
   $form['name'] = array(
   '#type' => 'textfield',
   '#title' => t('Machine Name'),
   '#description' => t('lowercase, no spaces or punctuation'),
   '#default_value' => $fcontrol->name,
   '#required' => true,
   );
   // */
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Human readable control name.'),
    '#default_value' => $fcontrol->title,
    '#required' => true,
  );
  $form['package'] = array(
    '#type' => 'textfield',
    '#title' => t('Feature Set (Package)'),
    '#description' => t('Determine which feature set the control will '
                       . 'be included in.'),
    '#default_value' => $fcontrol->package,
    '#required' => true,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('Describe what this control does.'),
    '#default_value' => $fcontrol->description,
    '#required' => true,
  );
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('Send users to this path, e.g. admin/example'),
    '#default_value' => $fcontrol->path,
    '#required' => true,
  );
  $form['query'] = array(
    '#type' => 'textfield',
    '#title' => t('Query'),
    '#description' => t('Include this query string in the URL, e.g. x=y&a=b '
                       . 'will be added to the URL like this: '
                       . 'http://example.com/admin/example?x=y&a=b'),
    '#default_value' => $fcontrol->query,
    '#required' => false,
  );
  /*  // TODO Figure out how to display this stuff on admin/fcontrolpanel page
   $form['video_title'] = array(
   '#type' => 'textfield',
   '#title' => t('Instructional Video Title'),
   '#description' => t('Title of instructional video'),
   '#default_value' => $fcontrol->video_title,
   '#required' => false,
   );
   $form['video'] = array(
   '#type' => 'textarea',
   '#title' => t('Video'),
   '#description' => t('e.g. <embed>HTML for YouTube, Vimeo, etc., video goes here</embed>'),
   '#default_value' => $fcontrol->video,
   '#required' => false,
   );
   $form['instructions_title'] = array(
   '#type' => 'textfield',
   '#title' => t('Instructions Title'),
   '#description' => t('Written instructions'),
   '#default_value' => $fcontrol->video_title,
   '#required' => false,
   );
   $form['instructions'] = array(
   '#type' => 'textarea',
   '#title' => t('Instructions'),
   '#description' => t('Include written instructions for the end-user here'),
   '#default_value' => $fcontrol->video,
   '#required' => false,
   );
   // */
  $form['permission'] = array(
    '#type' => 'textfield',
    '#title' => t('Required Permission'),
    '#description' => t(''),
    '#default_value' => $fcontrol->permission,
    '#required' => false,
  );
  $form['weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#description' => t('Sort order of controls'),
    '#default_value' => $fcontrol->weight,
    '#required' => false,
  );

}
