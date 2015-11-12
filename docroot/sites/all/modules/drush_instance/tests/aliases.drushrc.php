<?php

/**
 * @file aliases.drushrc.php
 *
 * Example aliases file
 *
 * This was initially used as part of the test suite. However, it's now also
 * part of the documentation, so please read README.md and HOWTO.md, then
 * look at this file for examples of how to configure Drush Instance
 */

/**
 * Makefile-based instance.
 *
 * The master instance, forked below for composer/broken builds.
 */

$aliases['instance.local'] = array(
    // Standard site aliases configuration
    'uri' => 'instance.local',
    'root' => '/var/www/drupal7-core',
    'path-aliases' => array(
      '%files' => 'sites/files/core',
    ),

    // Custom configuration for Drush Instance
    'instance' => array(
      // Sources for the makefile repository, and a repository to sit in
      // sites_default
      'sources' => array(
        'makefile' => array(
          // Each source must have a type; and typically, depending on its
          // type, it should also have a URL and other parameters
          'type' => 'git',
          'url' => 'http://git.drupal.org/project/drush_instance.git',
          'branch' => 'examples',
        ),
        // If you omit sites_default, Drush Instance assumes it's the same
        // source as the makefile. You can certainly have the makefile in
        // the same repository as sites_default, by having them on different
        // branches (or just leaving the makefile in with your sites/default
        // code: it's unlikely anyone will discover it if you give it a
        // custom name.)
        'sites_default' => array(
          'type' => 'git',
          'url' => 'http://git.drupal.org/project/drush_instance.git',
          'branch' => '7.x-1.x-dev',
        ),
      ),

      // Optional custom name for the makefile. Drush Instance assumes
      // instance.make otherwise
      'makefile_path' => 'core.make',

      // Non-make paths you might want to preserve when you completely rebuild
      // a codebase. This includes e.g. the uploaded files folder
      'paths' => array(

        // You can reference paths using path aliases, if you've already
        // defined them above...
        '%files' => array(
          // Preserve on rebuild:
          //   'keep': keep the file
          //   'discard': discard the file
          //   'source': re-source the file, overwriting it.
          // Booleans are deprecated but correspond to the first two options.
          'preserve_on_rebuild' => 'keep',
          // What to do on initial build to "source" the path. This example
          // tells Drush Instance to just create it as a directory
          'source' => array(
            'type' => 'mkdir',
          ),

          // Set permissions on this directory to by writeable by the group
          'permissions' => array(
            'type' => 'chmod_group',
          ),
        ),

        // The same configuration, but for a random empty text file
        'some_random_toplevel_file.txt' => array(
          // Re-source this on rebuild: this will be explicitly tested for
          // in ReBuildTest.php by modifying file between builds.
          'preserve_on_rebuild' => 'source',
          'source' => array(
            'type' => 'touch',
          ),
          // This text file is writeable by all, which probably isn't what
          // you want; but then in the absence of guaranteed sudo access it
          // might be a sufficient hack for you.
          'permissions' => array(
            'type' => 'chmod_all',
          ),
        ),

        // Path configuration again, this time for a symlink
        'symlinks/user' => array(
          // Keep this, as we want to test symlink integrity on rebuild.
          'preserve_on_rebuild' => 'keep',
          'source' => array(
            'from' => 'modules/user',
            'type' => 'symlink',
          ),
          // Symlinks inherit the permissions of their sources, so
          // we have no 'permissions' here.
        ),

        'sites/includes_rsynced' => array(
          'preserve_on_rebuild' => 'discard',
          'source' => array(
            'type' => 'rsync',
            // This must be a valid site-plus-path alias
            'from' => '@instance.local:includes',
            // Any other options for rsync: see drush rsync help / man rsync
            /* 'max-size' => '50k',
            'exclude' => "*legacy*", */
          ),

          // Set permissions on this directory to by writeable by the group
          'permissions' => array(
            'type' => 'chmod_group',
          ),
        ),

        // Copy: file and directory copy are the same to configure, but
        // implemented differently. So to test them both, we have two entries here.
        'CHANGELOG_copy.txt' => array(
          'preserve_on_rebuild' => 'keep',
          'source' => array(
            'type' => 'copy',
            'from' => 'CHANGELOG.txt',
          ),
        ),
        'sites/includes_copied' => array(
          // Leave preserve_on_rebuild blank: counts as a 'discard'.
          'source' => array(
            'type' => 'copy',
            'from' => 'includes',
          ),
        ),
      ),
    ),
);

/**
 * Composer-based instance.
 *
 * Fork makefile instance above, and modify sources.
 */

// Duplicate makefile instance.
$aliases['instance.composer'] = $aliases['instance.local'];

// Straight swap of makefile config to composer.json config will do.
$aliases['instance.composer']['instance']['sources']['composer']
  = $aliases['instance.composer']['instance']['sources']['makefile'];
unset($aliases['instance.composer']['instance']['sources']['makefile']);

/**
 * Broken instance.
 *
 * To test what happens when a build cannot be completed because
 * of filesystem conflicts.
 */

// Duplicate makefile instance.
$aliases['instance.fileconflict'] = $aliases['instance.local'];

// Add a file whose toplevel folder is itself a core file.
$aliases['instance.fileconflict']['instance']['paths']['CHANGELOG.txt/test_conflict.txt'] = array(
  'source' => array(
    'type' => 'touch',
  ),
);
