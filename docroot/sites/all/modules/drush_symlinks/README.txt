Drush Symlinks
--------------

Drush symlinks command creates symlinks using array with link definitions stored
in Drush alias file. Sometimes it's more convenient to store and develop custom
modules, themes or profiles in your home directory (not in sites/all directory
inside Drupal files tree).

Explanation
-----------

Real custom module and theme paths.

/var/www/example.com
└── sites
    └── all
        ├── modules
        │   └── example_module
        └── themes
            └── example_theme

Your project may be stored in your home directory.

$HOME/example.com
├── example_module -> /var/www/example.com/sites/all/modules/example_module
├── example_theme -> /var/www/example.com/sites/all/themes/example_theme
└── .git

With this directory structure you can put only your custom code to VCS, which
is very convenient. But it's too hard to create a lot of symlinks mannualy.
Drush Symlinks command will help you with this.

Installation
------------

Note: Drush Symlinks can't be used on Windows platform.

Download drush_symlinks and place it in your $HOME/.drush folder.

You may place drush_symlinks wherever you want, but if it is not in a standard
location for drush commands, you will need to add it to your drush include file
search path. See drush/examples/example.drushrc.php in the drush project for
more information.

Now let's create Drush alias file with 'drush_symlinks' key.

$aliases[example.com] = array(
  'uri' => 'example.com',
  'root' => '/var/www/example.com',
  'drush_symlinks' => array(
    array(
      'target_path' => '/var/www/example.com/sites/all/modules/example_module',
      'link_path' => '/home/user/example.com/example_module'
    ),
    array(
      'target_path' => '/var/www/example.com/sites/all/modules/example_theme',
      'link_path' => '/home/user/example.com/example_theme'
    )
  )
);

Execute this command to create symlinks automatically.

$ drush symlinks @example.com