VarHammer (of steel) is a tool that auto generates configuration form pages
for any desired Drupal variables. If your site have a lot of settings
stored in Drupal variables and you want to provide easy access for non-
developers but don't want to spend time to create a custom admin interface,
this module might be useful for you.

Usage:

Create a Drupal variable named "varhammer_settings" and go to admin/config
and you will see a VarHammer section with links the defined settings form.

You can set the variable programmatically but the easiest way is to do it in
the $conf array in settings.php or to use the Strongarm module (eventually
 combined with Features).
    
Configuration example (as used in settings.php):
  
$conf['varhammer_settings'] = array (
  'paying_user' => 
  array (
    'title' => 'Paying user settings',
    'permission' => 'access administration pages',
    'description' => 'some kind of description',
    'settings' => 
    array (
      'max_per_user' => 'Max connections per user',
      'api_key' => 'The API key to the gateway',
      'default_product_id' => 'the ID of the default product',
      'other_stuff' => 'other stuff here',
      'other_stuff2' => 'other stuff here2',
    ),
  ),
  'performance_settings' => 
  array (
    'title' => 'Performance settings',
    'permission' => 'access administration pages',
    'description' => 'some kind of description',
    'settings' => 
    array (
      'other_stuff3' => 'other stuff here3',
      'other_stuff4' => 'other stuff here4',
    ),
  ),
);

The permission parameter is the Drupal-permission, and the keys in the settings
arrays are used for Drupal variable names. The description parameter is
optional.

DO NOT specify any Drupal variables that contains complex data (arrays, objects
etc.) since this tool only handles string values.

The generated forms can be altered by implementing
hook_form_varhammer_settings_form_alter().

Thanks to Snipon (https://Drupal.org/user/439380) for suggesting the name.
