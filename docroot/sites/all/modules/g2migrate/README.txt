
CONTENTS OF THIS FILE
---------------------

* Introduction
* Dependencies
* Installation
* Configuration
* Import
* Migration
* A View for your albums
* A little (taxonomy) action
* Redirects (bleeding edge)

INTRODUCTION
------------

Current maintainer: Benji Fisher <http://drupal.org/user/683300>

The Gallery2 migrate module is designed to import photos and related data from
the Gallery package: http://gallery.menalto.com/. This module works with
Version 2 of Gallery (G2) and Drupal 7.

Some Drupal sites use the Gallery module (http://drupal.org/project/gallery)
to integrate G2 with Drupal. This module gives an upgrade path to Drupal 7.

This module works for me, but it is still in development. It may not (yet)
import all the data you want from your Gallery installation. It may have bugs.
Before using this module, you should back up your Drupal database and practice
restoring from your backup.

Security warning:

This module includes an option to import SQL dumps of your Gallery database
tables. This option is vulnerable to nasty SQL attacks.

* This vulnerability is mitigated by the fact that the user needs the
  'administer nodes' permission to execute the code.
* The only SQL code you import should be dumps you have made yourself.
* You should look at the SQL files before you import them, to make sure that
  they do not have any unusual lines.
* There are other methods for importing tables, so you do not have to use the
  insecure code in this module.

DEPENDENCIES
------------

This module requires Drupal 7.14 or later. (Why? Because we copy the mixed-case
field names from the Gallery database tables. See http://drupal.org/drupal-7.14
and http://drupal.org/node/1171866.)

This module is based on the Migrate API. It depends on the following
contributed modules:

* Migrate and Migrate UI:
    http://drupal.org/project/migrate

If you want to use the Media module (7.x-1.x), then you will also need

* Migrate Extras and Migrate Extras Media:
    http://drupal.org/project/migrate_extras

If you want to redirect legacy pages to your new site, then you will also need

* Redirect module and one of these patches:
    http://drupal.org/project/redirect
    https://drupal.org/comment/7603833#comment-7603833
    https://drupal.org/comment/6057284#comment-6057284


as well as the Gallery migrate media submodule (included with this one) and
Media and related modules. If you want to use core Image fields instead of
Media, and you do not want to install extra modules, then you do not need these
modules.

INSTALLATION
------------

Install like any other Drupal module. Installation will create a content type
and two vocabularies as well as adding several (initially empty) tables to
your Drupal database.

Installing the optional Gallery migrate media (g2migrate_media1) sub-module only
adds migration options.

Once you have finished importing your Gallery photos, you should disable this
module. You may also want to disable Migrate. If you uninstall this module, it
will remove the database tables but it will not remove the content type nor the
vocabularies. If you have imported data into your own content type and/or
vocabularies, then you can remove the ones created when you installed this
module.

CONFIGURATION
-------------

Configure the module from Administration > Configuration > Gallery2 Migrate
(admin/config/development/gallery2). There are three tabs:

* Paths: Enter
  - which vocabulary to use for the album structure
  - which content type to use for importing photos
  - the paths to g2data/ (where Gallery stores the photos)
  - the path to the directory containing your SQL dumps (if you want this module
    to handle importing tables from your Gallery installation)
  - the prefix for your Gallery database tables (ditto)

The paths should be on your web server. Use UNIX-style paths even if the server
is running Windows. (It works for me. It may depend on PHP configuration.)

* Fields: Choose which fields you will use to store imported data. There is
one section for the vocabulary you chose on the Paths tab and one section for
the content type you chose.

If you choose the content type provided by this module, then it is safe to use
the defaults on the Fields tab. In this case, you should still submit the
form. If you change the content type selected on the Paths tab, then you
should revisit the Fields tab.

The module does not currently check that the taxonomy reference field that you
choose on the Fields tab actually refers to the vocabulary you chose on the
Paths tab. If it does not, you will get error messages later.

The default content type uses the core Image module for its photo field. If
you want to use the Media module, then use the "File" field type (not the
deprecated "Multimedia asset" field type) with the "Media file" selector
widget. I have tested with Media 7.x-1.2.

* Import: Just click the buttons and do not argue about whether this is part
of the configuration stage or the import stage. ;)

I recommend that you do not use the "Import tables" form on this page.
Instead, use one of the methods described in the Import section below. Either
way, you will have to use the "Process tables" form on this page.

IMPORT
------

If you run into trouble with the first step (importing database tables) on the
Import tab, it is probably easier to use another method than to fix this
module. You may have timeout issues if your Gallery tables are much larger
than the ones I used. Try using a command-line tool (drush or mysql) or
phpMyAdmin. The structure of the tables (those in your Gallery installation and
those created by this module) should be the same, so it is just a question of
importing the data and using the right table names.

The Migration step is more robust, so you should not have to worry about
timeout issues.

In development, others have reported no troubles with up to 5000 photos. If you
use this module with larger installations, please report your experience.

The tables you need to import are listed on the Paths tab of the configuration
page. This module creates tables that should have the same structure as your
tables, replacing the prefix with "g2migrate_". For example, if you chose the
prefix "g2_" when installing Gallery2, then you should have a table g2_Entity
in your Gallery database with the same structure as this module's
g2migrate_Entity.

In the following examples, I assume that you installed Gallery2 with the
prefix "g2_". Adjust the commands as needed.

The examples show the *_Entity tables, but you have to do the same thing for
ALL the tables listed on the Paths tab (/admin/config/development/gallery2).

* Import from the command line:

  If your Gallery2 and Drupal databases use the same database server, and you
  have a mysql user who can read Gallery2 and write Drupal, then you should be
  able to copy the tables as follows:
  $ mysql -u myuser -p
  > INSERT INTO Drupal.g2migrate_Entity (SELECT * FROM Gallery2.g2_Entity);

* Import using drush:

  This is a variant of the preceding method, which works if you use drush and
  if the database credentials in your settings.php file also give you access
  to your Gallery2 database:
  $ drush sqlq "INSERT INTO g2migrate_Entity (SELECT * FROM Gallery2.g2_Entity)"

* Import using phpMyAdmin or a similar GUI tool

  Using phpMyAdmin, you can find the tables in your Gallery2 database. For
  each table, select the Operations tab. Choose the Copy operation; select the
  corresponding Drupal table as the destination; select the "Data only"
  option.

  If you have to import your Gallery2 database from another database server,
  then you may have trouble if the database dump is too large. If you can find
  your phpMyAdmin config file, then it is easy to set it to look for dump
  files on the server instead of uploading them from your local computer via
  HTTP. See the instructions at
  http://docs.phpmyadmin.net/en/latest/faq.html#i-cannot-upload-big-dump-files-memory-http-or-timeout-problems .

* Be creative

  Maybe you can use some combination of the other methods.

* Import tables using this module's configuration page.

  If all else fails, create separate dump files (backups) for each table you
  need to import. Un-check the "Skip import" option on the Paths tab on this
  module's configuration page, and use the Import tab. The module will read
  your file, look for INSERT lines, replace your prefix with "g2migrate_", and
  execute the SQL command. I do not know what I was thinking ...

  For this to work, you have to make sure that each INSERT command is on a
  single line of your file. It will not work if you have, for example,
    INSERT INTO `g2_Entity` VALUES
  on one line and then the data on the next line(s). With my version of mysql,
  the following works from the command line:
  $ mysqldump -e --skip-opt -t -r g2_Entity.sql Gallery2 g2_Entity
  (Here, -e is the short form of --extended-insert; -t is the short form of
  --no-create-info; and -r is the short form of --result-file= . The last two
  arguments are the database and the table.)

MIGRATION
---------

The Migrate module is designed for large-scale migrations, so it should be able
to handle even large Gallery installations. The Migrate UI submodule
(migrate_ui) provides an admin interface at Administration > Content > Migrate
(admin/content/migrate). You can also administer your migration using drush from
the command line. The Migrate module has extensive documentation:

* Main documentation page: http://drupal.org/node/415260
* Drush instructions: http://drupal.org/node/1561820
* External docs (blogs, videos, etc.): http://drupal.org/node/1132428
* Caveats: http://drupal.org/node/1555278

(The Caveats page includes some simple advice, probably based on hard
experience.)

The Migrate module gives you the option of importing a limited number of
items. It is a good idea to run some tests using this option before converting
your entire Gallery installation.

This module provides two migration options, each of which can be used through
the UI or drush:

* Gallery2Album: Run this first.
* Gallery2Node: Use this to create nodes with core Image fields.

The Gallery migrate media sub-module provides two more:

* Gallery2Media1Image: Use this to create Media FileField entities.
* Gallery2Media1Node: Use this to create nodes with Media File fields.

The Gallery2Album migration will create taxonomy terms to represent the album
structure of your Gallery installation. The other migrations depend on this
one so that the nodes or entities they create can be tagged with the
appropriate terms.

The module should be able to choose between the two node migrations based on
the configuration, but it does not. There is also a lot of code duplication
between these two migrations. Patches are welcome.

A VIEW FOR YOUR ALBUMS
----------------------

This module includes a view to create an album out of your migrated photos. It
should work as is if you use the content type supplied by the module. You can
tweak the view, assuming that you have installed the Views and Views UI
modules. 

The view is styled by g2migrate.css. As usual, you can override this by adding
a file of the same name to your theme folder.

There is some custom code in this module to display parent albums in the
breadcrumb trail, with a select list for child albums. This will only work
with the provided view.

A LITTLE (TAXONOMY) ACTION
--------------------------

If you have albums that contain no photos, just sub-albums, then they will
look pretty barren if you use the view. This module provides an action that
will attach to a node one or more parent terms of each term already attached.
You can apply the action using the Views bulk operations
(views_bulk_operations, or VBO) module. (You could also use the action with
the Rules module or with the core Trigger module. For example, you could set
it up so that every time a node is created or updated, ancestor terms are
added along with any taxonomy terms.)

REDIRECTS
---------

If you are keeping the domain name as you upgrade your site to Drupal 7, and
you want to redirect legacy Gallery pages to the new pages you are creating,
then install the Redirect module (http://drupal.org/project/redirect). As of
this writing, you will also have to patch the redirect module. Check the
issues queue:  there may be more recent versions of the relevant patches than
the ones I have tested, and they may have been committed to the Redirect
module.

If you would like to redirect legacy Gallery photo pages to the new pages on
your site, then apply the patch from
https://drupal.org/node/1116408#comment-7603833 to the Redirect module. Then,
fill out the "Legacy base path" text box on Administration > Configuration >
Gallery2 Migrate (admin/config/development/gallery2). When you run the
Gallery2Node migration, the Redirect entities should be created automatically.

If you would like to redirect legacy Gallery album pages to the new pages on
your site, then apply the patch from
https://drupal.org/comment/6057284#comment-6057284 to the Redirect module. Then,
fill out the "Legacy base path" text box as in the previous paragraph. On the
Migrate page (admin/content/migrate) there should now be a Gallery2Redirect
migration. Run this, and the Redirect entities should be created.

After applying either patch, you may have to clear caches in order to register
the newly created migration classes.

As of this writing, the two Redirect patches are incompatible. You will have
to undo one before applying the other if you want to use both.
