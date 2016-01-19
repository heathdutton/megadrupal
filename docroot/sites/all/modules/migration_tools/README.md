# Migration Tools

CONTENTS OF THIS FILE
 * Introduction
 * Features
 * Requirements
 * Installation
 * Configuration
 * Bonus Features
 * Maintainers

--------------

## Introduction

Migration Tools does nothing by itself.  It simply adds classes and methods that
developers can use within their custom migration classes to make migrations
easier and more reliable.  It also contains example classes to illustrate how
the tools can be utilized.

## Features

  * CheckFor: Common checks that can be implemented in prepareRow to evaluate
    and report on the results.
  * HtmlCleanUp: Methods for sanitizing or cleaning up html pages or body
    content.
  * MigrationMessage: A messaging class to handle outputting useful information
    to the terminal when running migrations or logging in Watchdog.
  * NodeTools: Methods for processing nodes.
  * StringCleanUp: Methods for cleaning up aspects of string content.
  * TaxonomyTools: Methods for processing vocabularies and terms.
  * UrlTools: Methods for handling URLS and processing redirects.
  * Source Parsers:  A variety of parsers that can be used directly, or as an
    and example of a parser.
  * Obtainers: A collection of methods for processing html files to extract
    titles, dates, id numbers and other items from html pages with inconsistent
    structure.
  * Menu Generators: Methods and drush commands for either building a menu by
    crawling html structure on a legacy site, or by processing a CSV list of
    menu headings and old urls from the legacy site.
  * Drush: goodies related to migration.
  * Examples:  Migration classes meant to demonstrate the use of Migration Tools
    for a migration.
  * Migration Generator:  Yes, it builds migration classes.

## Installation

* Enable this module

## Configuration
* Set $conf['migration_tools_base_dir'] correctly for your local machine.  This
is the location for your migration source files. (parallel to docroot)

## Requirements
_incomplete documentation_

## Drush Goodies

### Copying unmanaged images:

_incomplete documentation_

To recursively copy image files from the current directory to a destination:

There is a drush command that will move the images to public:// from
$conf['migration_tools_base_dir'] for any given organization:

drush dmi <organization_abbreviation>

````
cd path/to/source/dir
find -E . -iregex '.*\.(jpg|png|gif)' | cpio --pass-through \
 --preserve-modification-time \
 --make-directories --verbose path/to/dest/dir
````



### Migration Generation

Since we have so much infrastructure developed around our html to node
migrations, it made sense to automate some of the process. The
`mt-generate-migration-class` drush command will automatically generate a
migration class for you. See `drush mt-generate-migration-class --help` for
command specification.

The command accepts the following arguments:
 * config: the name of a yml file containing subdirectory specific config. These
   must be stored in scripts directory
    _incomplete documentation_

The command accepts the following options:
  * type: (optional) _incomplete documentation_
    This dictate which twig template will be used to
    generate the migration.

Example commands:
> drush dgm atr.yml


### Generating and Importing Menus

_incomplete documentation_

There are 2 drush commands to generate a menu and one to import a menu.
* Generating a menu from a csv file
* Generating a menu by actually crawling menu html on the legacy site.

The generating command looks like this:
> drush mt-generate-menu menu_name --css-selector='#navbar' --local-base-uri='subpath' --menu-location-uri='subpath'

The only parameter required is the abbreviation of the migration, in this
case menu_name.

Other configuration is optional:

* css-selector should be a css selector pointing to the outer-most ul of the
menu in the live site
* local-base-uri should be the path to where the content is locally after a
migration has been run (ex. ag or usao-nm)
* menu-location-uri should be a page in the live site containing the menu that
we want to generate

After a migration is run, so the content is present locally (This is a
prerequisite of menu generation), and the menu is generated, we can then
import it.

the import command is
> drush MT-import-group-menu menu_name.txt subpath

The command takes the file where the menu is (the script assumes this file is
inside the sources directory) and the abbreviation of the migration.

_incomplete documentation_





### Debugging and Iterations
There are two settings through drupal variables that can aid in building and
debugging a migration.  The default for each is FALSE but can be overidden in
your settings.local.php

variable: migration_tools_drush_debug
  Enables output to be seen in the terminal on a file by file basis to see
  what elements are being found by the obtainers and migrated. Default is FALSE.
  Enable debug output: drush vset migration_tools_drush_debug TRUE

migration_tools_drush_stop_on_error
  When migration_tools_drush_debug is TRUE and a warning is thrown by the
  messsaging system that is of the level WATCHDOG_ERROR, WATCHDOG_CRITICAL,
  WATCHDOG_ALERT, WATCHDOG_EMERGENCY.
  Default is FALSE.
  Enable stop on error: drush vset migration_tools_drush_stop_on_error TRUE

##Migration Development

_incomplete documentation_




## Get your information
In order to create a migration class for an organization you will need the following information (some from taking a peek at the source directories, some from project people)

1. Organization or District
2. Path to source files (must match legacy site path)
3. New abbreviation for district or organization
4. Term Id of vocabulary Component - Quickest way to get this is node/add/press_release then inspect the component field for the term that matches this organization / district.
5. Location of press releases within the source directory. (sometimes there are multiple, pcik one for now and the others can be added later)

## Build migration yml file (either organization or district)
In sites/all/modules/custom/mt_migration create a new yml file in scripts/  The yml file should be named according to the abbreviation of the migration.

The contents of the yml file will resemble this

    abbreviation: subdirectory
    full_name: The name of the group or section
    directory: subdirectory_name
    pr_subdirectory: news
    component_tid: 2071

After saving this file,  the appropriate drush command needs to be run to build the migration classes.  Modify it to use your yml file.

    # This is the command for a district.
    drush mt-generate-migration-class migrationname.yml


This will inspect the content of the source files looking for binary files, content pages and press releases. It will build the migration classes and add the files to mt_migration.info for inclusion.

Register the new files with drupal: `drush cc all`

Register the new migration classes: `drush migrate-register`

Get the status of the migration you just created:

    drush migrate-status --group="usao-wdtn"

If all worked correctly it will return a report of files and nodes that need to be migrated.

## Look through the source files to see if there is garbage to exclude.
There is no real science here, just poke around and look for things like /old or /original or header.html or footer.html  or test.html.   If they are in a path like /old, they can be removed from the array of source directories in the migration class.

    // Define source directories.
    $source_dirs = array(
      0 => 'subdirectory/corruption/mango',
      1 => 'subdirectory/corruption',
      2 => 'subdirectory/thing/corruption/then',
      3 => 'subdirectory/thing/corruption/now',
      4 => 'subdirectory/thing/old',  // Just delete this from this array if it should not be migrated.
    );


### Excluding pages
Sometimes there are pages that should not be migrated
They can be skipped in prepareRow  within the migration class like this:

````
 /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/subdirectory/must/must_index.html',
      '/subdirectory/ski/ski_index.html',
    );

    if (mt_migration_skip_file($row->fileid, $skip_these) || (parent::prepareRow($row) === FALSE)) {
      return FALSE;
    }
  }
````


Additionally you can make the migration stop on lack of title or date by setting the value

    drush vset mt_migration_tools_drush_stop_on_error TRUE

## Migrate the images
Images are pretty straightforward and require little effort.   The path argument is for the original location of the images in the source.

    drush mt-migrate-images subdirectory



Copy and paste the report from your terminal into a comment on the ticket.

If this is an organization then you need to open .htaccess and add the abbreviation of the group abbreviation to the re-write rule on [this line].

##  Migrate Content Type: Page
The first time you run a migration you will want to migrate only a few pages to see that the Obtainer->finders are looking in the right spot to get the title and the body.  To run just a few you can limit the migration like this:

     drush migrate-import UsaoWdtnPage --limit='2 items'

The debug output in the terminal should show you what the title looks like.  If the title does not match the title of the original page, create/adjust the plucker methods being used.  Inspect the source to see what you should be targeting with a plucker.  Finders and Pluckers run in the Obtainer.  Finders try to find items but do not remove them from the Document.  Pluckers will remove what they find from the Document if what they find passes the validation within the Obtainer.

When you have the correct titles coming in, now view the imported nodes and see if the body looks correct.  There may be extraneous markup that needs to be removed.

## Obtainer->finder and plucker Method Tuning

You can tune the stack of finder and plucker methods in the migration class or in the source parser.  Anything done in the migration class itself will override the defaults set in the sourceparser so in general it is  easiest to set them in the migration class.  Setting them in the source parser is a better option only IF you are going to reuse the sourceparser on another migration (which is unlikely).

Overriding them in the migration class looks like this:

    $title = new ObtainerInfo("title", 'ObtainTitle');
    $title->addMethod('pluckSelector', array("h1", 2));
    $title->addMethod('findH1Any');
    $arguments['obtainers_info'][] = $title;

Overriding them in the source parser looks like this:

    protected function setDefaultObtainersInfo() {
      parent::setDefaultObtainersInfo();
      $ct = new ObtainerInfo('content_type');
      $ct->addMethod("findPRClassBreadcrumbPressRelease");
      $ct->addMethod("findPRImmediateRelease");
      $this->addObtainerInfo($ct);

      $date = new ObtainerInfo('date');
      $date->addMethod("pluckDivPAlignLeft");
      $date->addMethod('pluckSelector', array(".newsRight", 1));
      $date->addMethod('pluckXpath', array("/html/body/p[6]/span[1]/span/text()[1]", 1));
      $date->addMethod("findDivClassContentSubDiv3");
      $date->addMethod("pluckProbableDate");
      $this->addObtainerInfo($date);
    }


Iterate through the migration a few at a time checking them as you go.  You will need to rollback as you iterate.  Continue tuning the Obtainer finder method stack and excluding pages if they should not be migrated.

    drush migrate-rollback OurPageMigration

 After you have fine tuned it enough to get several to work fine.  Run the migration without the --limit option.

Examine the report at the end of the migration to see if pages came in with duplicate titles or missing titles.  Investigate the reason.  Sometimes titles are actually repeated in the source files, sometimes you end up grabbing a page element that is not the title.  Fix if needed.  rollback -> migrate -> repeat.

When satisfied with the migration, copy the command and report and put it in as a comment in the ticket.

**Iterating specific pages** (rather than migrating and rollback the entire migration) can be done by targeting that page file id.

    drush migrate-import OurPageMigration --idlist="/musto_index.html"
    # And
    drush migrate-rollback OurPageMigration --idlist="/musto_index.html"


## Migrating Press Releases
These are similar to migrating pages, except that there are more elements to try and obtain from the page.  Press releases often have

* date  (release dates)
* id numbers
* subtitles

As a result, additional work is often needed to tune the finder method stacks for each Obtainer to obtain the elements if they exist.

   drush migrate-import OurPressReleases

## Migrate Menu
When you have all the content migrated on your machine, run one of these commands to build the menu.


    drush mt-generate-menu subdirectory --menu-location-uri='subdirectory/index.htm' --local-base-uri='subdirectory' --css-selector='ul#navbar' --recurse='FALSE'

The first argument is the abbreviation of organization or district.
The options:
* menu-location-uri is the original path to the page where the menu exists.
* local-base-uri is the new location of the menu.
* selector is the css selector to use to find the menu on the page.
* recurse Whether or not load and crawl the primary pages to obtain menu items from those pages.

This will crawl the old menu and generate a text file for importing the menu.  Scan the menu for odd urls that may have gotten caught in the crawler.  If a full url appears, it is an indication that the page has not been migrated yet, so it links to the live site and will redirect accordingly when the page is eventually migrated. An http://  makes drupal treat it like an external link.  example: http://www.legacysite.com/foo/bar/somepage.html

The following command will import the menu.

    drush digm my-menu.txt subdirectory

### Advanced Methods
Sometimes you need some unusual methods:
* Restricting by Content Type
* Restricting by a subpath
* Restricting by Date


### Restricting by Content Type
There are times when you have multiple content types residing in the same directory and we need to limit them so that a press release migration brings in only press releases and skips non press releases.
Here are the basic steps:

1. Add the Obtainer for content_type to your migration.

        // Put this in your migration _construct
        $type = new ObtainerInfo('content_type', "ObtainContentType");
        // Add any finders you need.  Each one will take a shot at identifying the content type.
        $type->addMethod('findPRImmediateRelease');

    $arguments['obtainers_info'][] = $type;

1.  Add an 'isType' check within prepareRow that will cause it to skip the document if it is not a 'press_release'. Using isType will cause it to give proper drush feedback.

        if (mt_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE || (!self::isType('press_release', $row) ) {
          return FALSE;
         }

1.  If you need a new finder (a way to identify the kind of document you are looking for) add them to `mt_migration/includes/obtainers/ObtainContentType.php`.  The only difference is that each finder returns the machine name of the content type it thinks it found, or '' if it turned out to be something else.  So any logic checks for the type, occur within the finder rather than on validate as is done in typical finders.

### Restricting by a subpath
It might be necessary (due to the recursive method of file discovery) to exclude certain subpaths within the file system.  This can be done by creating an array of subpaths to exclude, then placing a call to isInPath() within prepareRow.

    isInPath(array $paths, $row)

Returns true if the file being processed is within one of the paths specified.

### Restricting by Date
If you are faced with press releases from multiple years all mixed in one directory, and you are trying to restrict them to only things after 2012. It can be done using `mt_migration_date_after($date, $date_cutoff, $default = TRUE)` in prepareRow.
Example:

       // Skip any press releases prior to 2013.
       if (!mt_migration_date_after($row->field_pr_date, '12/31/2012', FALSE)) {
         $message = '@fileid ------> Dated prior to 2013. Skipped: intentionally.';
         MigrationMessage::makeMessage($message, array('@fileid' => $row->fileid), WATCHDOG_WARNING);
         return FALSE;
       }

## BONUS
--------
The following modules are not required, but if you have them enabled they will
improve the experience:

  * Markdown - When the Markdown filter is enabled, display of the module help
    will be rendered with markdown.


## MAINTAINERS
--------------

* Steve Wirt (swirt) - https://drupal.org/user/138230
