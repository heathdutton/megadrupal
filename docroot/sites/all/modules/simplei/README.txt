--- Usage ---

1. Enable the module like any other. The module works with either Drupal core
   Toolbar module or Admin menu module.

2. Enter a line like the example in your settings.php file.

   $conf['simple_environment_indicator'] = 'DodgerBlue';

   or

   $conf['simple_environment_indicator'] = '#1E90FF DEV';

   The color name (or hex value) is optionally followed by indicator text.
   The text appears in the far left of the menu bar.

3. The indicator appears only when Toolbar or Admin Menu is showing.

--- Settings suggestion ---

For production
   $conf['simple_environment_indicator'] = 'FireBrick PRD';

For staging
   $conf['simple_environment_indicator'] = 'GoldenRod STG';

Fore local development
   $conf['simple_environment_indicator'] = 'DodgerBlue DEV';

