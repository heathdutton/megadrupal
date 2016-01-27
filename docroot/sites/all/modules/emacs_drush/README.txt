This project consists of Drush utilities for making the daily life of
Emacs users easier.

Currently the project provides two new Drush commands:

 * `drush etags`
 * `drush gtags`

They will run etags/gtags in your `DRUPAL_ROOT` and generate tag files
for use in Emacs and it will help keep them up to date.


## Why not just run etags or gtags directly without the help from Drush? ##

Well, with etags you would normally have to use
[find(1)](http://linux.die.net/man/1/find) to find your files and then
pipe them to Emacs. Like this:

    find -type f -name \*.module -or -name \*.inc -or -name \*.php -or -name \*.profile -or -name \*.install | etags --language=php -

and you have to remember to do that from your `DRUPAL_ROOT`.

`drush etags` takes care of all that for you.

With gtags there are other caveats. A default installation will
recognize .php (and .php3 and .phtml) as PHP files but not .module
etc. And due to some
[limitation](http://comments.gmane.org/gmane.comp.gnu.global.bugs/1439)
gtags will fail on parsing modules/system/system.api.php in Drupal 7
(core/modules/system/system.api.php in Drupal 8).

`drush gtags` will run gtags with a configuration suitable for
generating tag files for Drupal (as well as skip
modules/system/system.api.php) and remember to run the command from
your `DRUPAL_ROOT`.


## I don't remember to update my tag files after installing new modules! ##

That is why `drush etags/gtags` will hook into `drush pm-download` and
`drush pm-update` and run `drush etags` or `drush gtags` for you if
you already have tag files in your `DRUPAL_ROOT`.

So just run `drush etags/gtags` once on your project and then `drush
pm-download` will keep the tag files up to date after each download.


## Options for `drush etags` ##

`--emacs-etags-etags-program`
:   Name of the etags executable. Include path if executable is not in $PATH.

`--emacs-etags-extensions`
:   On which file extensions to run etags. Defaults to
    "module,inc,php,profile,install".

`--emacs-etags-find-program`
:   Name of the find executable. Include path if executable is not in $PATH.

## Options for `drush gtags` ##

`--emacs-gtags-gtags-options`
:   Options to add to gtags. Defaults to none.                                                                                  

`--emacs-gtags-gtags-program`
:   Name of the gtags executable. Include path if executable is not in $PATH.

`--emacs-gtags-gtagsconf`
:   Gtags configuration file to use. Defaults to
    `~/.drush/emacs_drush/globalrc`. Use `0` for gtags default
    configuration.
    
`--emacs-gtags-gtagslabel`
:   Gtags label to use. Defaults to `drupalX` where X is your Drupal
    major version number. Use `0` for gtags default label.


## Dependencies ##
For `drush etags`:

 * `etags` - part of your Emacs installation.
 * `find` - part of a standard Unix system (including Mac OS X).

For `drush gtags`:

 * `gtags` - part of your GNU GLOBAL installation. I have tested successfully with version 6.2.x.

Tested with Drush 5.x and Drush 4.5.

## I think it's rude to run your script after drush pm-download!!§! ##

We will only run etags if you already have a file named TAGS in
your `DRUPAL_ROOT` and we will only run gtags if you already have a
GTAGS file in your `DRUPAL_ROOT`.

You can disable this by setting either:

 * `$options['emacs-pm-post-download-etags'] = FALSE;`
 * `$options['emacs-pm-post-download-gtags'] = FALSE;`

in your `drushrc.php` or to your `@my-site-alias` as:

    $aliases['my-site-alias'] = array(
      'emacs-pm-post-download-etags' => FALSE,
      'emacs-pm-post-download-gtags' => FALSE,
      ...
    );

## Installation ##

The releases 7.x-1.1 and 6.x-1.1 are identical. This will allow you to install this extension by issuing:

    drush dl emacs_drush

while located in a Drupal 7 project, a Drupal 6 project, or outside a Drupal project.

## Huh? This is not a drupal-mode for Emacs? ##

No.

But I have another project which is just that:
[drupal-mode](https://github.com/arnested/drupal-mode). Or look at
some of that projects
["competitors"](https://github.com/arnested/drupal-mode#other-takes-on-a-drupal-mode).
