How to use Drush Instance
=========================

Drush Instance needs:

1. A Drush site alias for your site. See:
   http://drush.ws/examples/example.aliases.drushrc.php
2. Some custom configuration inside the site alias. See the example aliases
   file, aliases.drushrc.php, in the tests/ folder of Drush Instance.
3. A build file to source any code from drupal.org or elsewhere into
   sites/all. This build file could be a Drush Make makefile, or a Composer
   JSON file: both are supported (but see the examples/ branch of this
   repository for examples of both: Composer support is currently limited.)
4. And any custom code for your build, to be stored in sites/default . This
   plus the build file above should be in version control, and can be in the
   same repository.
5. Any further customizations to your build, to e.g. permit the uploaded
   files folder to be preserved when you rebuild your codebase using Drush
   Instance / Drush make.

Note that an example site alias file is available in the tests/ subfolder
of Drush Instance. If you know what you're doing, you can skip 1) and 2) below
and just copy that site alias file instead.


1) Setting up a site alias
--------------------------

This first step doesn't really have anything specifically to do with Drush
Instance; but you should definitely start using site aliases anyway.

Let's say your site is called "mysite" - and for this example, let's assume it
already exists. In ~/.drush/mysite.aliases.drushrc.php , add something similar
to the following text:

<?php

$aliases['local'] = array(
  'uri' => 'mysite.local',
  'root' => '/var/www/drupal7-mysite',

  // Optional, but we'll make use of this later on!
  'path-aliases' => array(
    '%files' => 'sites/files/mysite',
  ),
);

That's enough to permit Drush to find your site. You can now run:

  $ drush @mysite.local status

And Drush will know to run the command in the folder of mysite.

Incidentally, you can also call this file just plain aliases.drushrc.php . But
then you'll have to name your alias as follows:

$aliases['mysite.local'] = ...

for Drush to know what you're talking about.


2) Custom configuration to support Drush Instance
------------------------------------------

Now expand the Drush alias as below.

a. if you're using a makefile:

$aliases['local'] = array(
  'uri' => 'mysite.local',
  'root' => '/var/www/drupal7-mysite',

  // This is the custom content for Drush instance
  'instance' => array(
    // Sources for makefile and sites/default e.g. git, bzr, tar.gz URLs
    'sources' => array(
      // Bare minimum is a repository for the makefile
      'makefile' => array(
        'type' => 'git',
        'url' => 'git@github.com:myaccount/mysite-makefile.git',
      ),
    ),
    // Your makefile can be called instance.make, and it will automatically
    // work. But if you want to be different, set this here
    'makefile_path' => 'mysitemakefile.make',
  ),
);

b. if you're using composer.json; as above, but:

  ...
  'instance' => array(
    'sources' => array(
      'composer' => array(
        'type' => 'git',
        'url' => 'git@github.com:myaccount/mysite-composer.git',
      ),
    ),
  ),
  ...

3) Setting up a makefile/composer repository
--------------------------------------------

Now you'll need to set up a repository to store your build file in (and also
any custom code that can't be built with Drush make/composer: for example,
code you've written just for this one website to go in sites/default.)

In the example above, we use git, although Drush Instance will support any of
the sources that Drush Make/Composer supports: so bzr, svn, or even a gzipped
file on some webserver somewhere.

a. Using a makefile.

With the example in 2a above, in your new git repository, add a makefile called
'mysitemakefile.make' (see the example above for how to change the name of
this). This should be a standard makefile which we don't discuss here;
more information is available at https://drupal.org/node/625094

But here's an example makefile, that makes a Drupal 7 site with Views, CTools:

; Drush make API version
api = 2
; Core Drupal major version
core = 7.x
; Drupal, CTools and Views minor versions
projects[drupal][version] = 7.23
projects[ctools][version] = 1.3
projects[views][version] = 3.7

This repository *can* also function as a sites/default folder, so you *can*
put a settings.php in if you want.

b. Using composer.json.

Composer is supported, but you should use composer/installers for modules,
themes and libraries, and just let Drupal core be installed into
vendor/drupal. Optionally in the future, the locations of all the projects
could be specified in the site alias above; at the moment this is not
supported.

In the git repository for Drush Instance, there's an examples branch, which
contains useful examples of both makefile and composer.json.

4) Optional extra repository for sites/default
----------------------------------------------

If you want to store your sites/default folder in a different repository,
you can specify it in your site alias above using:

    ...
    'sources' => array(
      ...
      'sites_default' => array(
        'type' => svn',
        'url' => 'https://somewhere.else.example.com/mysite/sitesdefault',
        'branch' => 'somebranchname',
      ),
    ),
    ...


5) Now you can build and rebuild your codebase!
--------------------------------------------

The above is enough to build or rebuild a codebase with Drush Instance. At
a command prompt, anywhere on your computer's folder structure, run the
following:

  $ drush instance-build @mysite.local

If you already have a build in place, you can rebuild it using

  $ drush instance-build @mysite.local --rebuild

Be warned: currently the configuration won't preserve your uploaded files
folder. We'll look at this below. But if you've already run instance-build
before reading this, then note that the old build will still be available as
/var/www/drupal7-mysite-old-XXXX, so you can recover the files from there.


6) Customizations to preserve files and folders
-----------------------------------------------

When the site is (re-)built, sites/default is repopulated with the entry
you've put in 'sources' above. But all other custom additions to your site's
codebase will be lost. This includes:

 * The uploaded files folder
 * Custom .htaccess
 * Any custom local settings in sites/default
 * Top-level files for e.g. convincing Google Webmaster you own the domain

You can preserve paths using the following fragment of configuration:

    'sources' => ...
    ...
    'paths' => array(
      'my_uploaded_files' => array(
        'preserve_on_rebuild' => 'keep',
        'source' => array(
          'type' => 'mkdir',
        ),
      ),
      ...
    ),

This fragment will ensure that the folder my_uploaded_files in the Drupal
root will be preserved on rebuild; on initial build, it will just create an
empty directory rather than doing anything more clever.

If you want to re-source a file on rebuild, rather than preserve the
(possibly edited) copy, use `'preserve_on_rebuild' => 'source'`.

If you use path aliases elsewhere in your site alias, you can also use them
in the 'paths' fragment:

  'path-aliases' => array(
    '%files' => 'sites/files/core',
  ),
  'instance' => array(
    ...
    'paths' => array(
      '%files' => array(
      ...

As you can probably see, there are two problems that Drush make needs to
be able to solve: what to do on initial build, and what to do every time
the site is rebuilt.

6a. What to do with filepaths on initial build
==============================================

On initial build, there are two problems that need solving:

i.  where do I source this file/directory from?
ii. what permissions should it have?

The first problem is solved by the source parameter that you can see in the
above fragment, just under preserve_on_rebuild. This must be an array of the
form:

    'paths' => array(
      'path_to_file' => array(
        'source' => array(
          'type' => ... /* See below */
          ... /* other options */
        ),
      ),
    ),

The source type can be set to a number of alternatives:

* mkdir - make a directory
* touch - make (touch) an empty file
* symlink - symlink from another filesystem location; the required 'from'
  option indicates that location.
* rsync - use drush rsync to get file or directory contents from a local or
  remote source; the required 'from' option indicates a location in
  site-alias format for rsyncing from. The extended "other options" are the
  same as what drush rsync requires, which in turn are the same as what the
  command-line rsync expects.

Here's an example of initially populating the files/ folder with the rsync'ed
files from a live instance, excluding any large files, and any files in the
*legacy* folders (maybe they pre-date Drupal?):

    'paths' => array(
      '%files' => array(
        'source' => array(
          'type' => 'rsync',
          'from' => '@live.sitealias:%files',
          'max-size' => '50k',
          'exclude' => "*legacy*",
        ),
      ),
    ),

The second issue is what to do with permissions once these files are set up.
Because drush instance can't rely on e.g. sudo and being able to do random
ownership changes, currently the only two options implemented are chmod'ing
the files to be owned by the current group, or by everyone on the server.

Here's the above example, with 'permissions' added:

    'paths' => array(
      '%files' => array(
        'source' => array(...),
        'permissions' => array(
          'type' => 'chmod_group',
        ),
      ),
    ),

It is possible to set up a server so that chmod-group provides sufficient
permissions for both PHP to write to the folders, and a deployment user to
deploy the codebase in the first place. In doing so, rather than relying on
sudo, you will end up with a more secure server.

6b. What to do with filepaths on rebuild
========================================

The configuration option preserve_on_rebuild is the only option that affects
rebuild at the time of writing. This should be set to a boolean.

If the filepath is not preserved on rebuild, Drush make drops back to its
initial build behaviour e.g. running rsync.

Summary
-------

Drush Instance uses site aliases, configured as per the above, to automate
the drush-making, populating and permission-giving of an initial codebase
build. It will also preserve all of the above on rebuild, when your custom
and/or contributed codebases change.

For an example site alias with all configuration options, check out

  tests/aliases.drushrc.php

in the Drush Instance codebase.
