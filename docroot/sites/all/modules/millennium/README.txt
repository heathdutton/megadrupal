
Millennium Integration module for Drupal
http://drupal.org/project/millennium

NOTE: The HTML version of this file (with graphics) is located in the
      docs/ directory, or can be read by going to Administer > Help > Millennium
      after module installation.

This module is oriented towards libraries with Innovative Interfaces Inc.'s 
Millennium who want to import or expose some of its information in a Drupal 
site. 

It can:
* Import bibliographic information into Drupal nodes which can display real-time 
  item availability, cover images, tables of contents and book previews  
  obtained from the Library of Congress, Open Library and Google Books.
* Embed book record information in existing nodes (for instance, to build
  bibliography or highlighted item lists).

Features:
  * auto-crawl or manual import of items from the WebOPAC.
  * support for external book jacket image services.
  * choose between real-time holdings information or just a link to the 
    original item in your WebOpac.
  * maps MARC fields to taxonomy vocabularies for navigation and RSS.
  Via the optional (included) modules:
    * imports Library of Congress book description and Table of Contents along 
      with item, available for searching.
    * Google Books links for items available online.
    * Allows embedding one or more records in the body of existing nodes using
      the syntax {{millennium|[recordnumber]}} or {{millennium|[url]}}

This module works best when used with some other modules:
  * HILCC Taoxonomy Autotag: will automatically build a hierarchical subject
    taxonomy using the items' LCC call numbers.
  * Faceted Search or Apache Solr Search: let users drill down into your 
    catalog.
  * Similar by Terms (or Apache Solr Search's recommendation blocks): get 
    recommendations for similar items.
  * Fivestar: let users rate items.
  * Flag: let users "favorite" their items.

REQUIREMENTS
=============
* PHP5
  
INSTALLATION
============
  * Install this module as usual, see http://drupal.org/node/70151 for further 
    information.
  * (Required for Auto-crawl) Enable Cron for your site: 
    see http://drupal.org/cron
  * Read the instructions below for more setup instructions.

USAGE
=============
Note: If you just want to embed book information in nodes, you can skip to the 
Millennium Input Filter module's documentation.

Basically, the module works as follows:
  * You must first configure module settings, to tell the module where your 
    WebOPAC is, and how to import records.
  * You can import records manually (see below) or automatically (during cron).
  * Records are imported into nodes, with links back to the original item and, 
    if you configured it to do so, you will see the realtime holdings 
    information fetched from the WebOPAC.
  * If you want to see the module�s progress, go to to admin/reports/dblog or 
    go to admin/reports/millennium for a detailed view.

CONFIGURATION: BEFORE YOU START IMPORTING
=========================================
To get a "real" library catalog, you should follow all of these steps.

(OPTIONAL) Create a new Drupal Content Type to put your items into.
  * Go to Administer -> Content Types 
  * Click on the "Add Content Type" tab
  * Fill in "Name" and "Type"; Name could be something like "Library Items" and 
    Type is a lowercase word like "libraryitem".
    
(OPTIONAL) Set up taxonomy categories for your items, so that each imported item 
can then be properly tagged:
  * Create taxonomy free-tagging vocabularies for each one of these: 
      * Subjects
      * Document type
      * Language
      * Author
    If you haven't done this before, here is a quick step-by-step:
      * Go to: Administer -> Content Management -> Categories
      * Click on the "Add Vocabulary" tab.
      * Enter the Vocabulary's name (e.g.: Subject: keyword).
      * Below enable "tags" for the vocabulary.
      * Enale the vocabulary for the node type you will be importing items into.
      * Save changes.
      * Repeat for another vocabulary.

Now, go to the administration section at admin/settings/millennium and enter 
the required settings:

  * Settings tab:
    - Content type to import records into: 
      If you created one in the above optional step, choose it now.
    - WebOpac Base URL:
      This is the base URL for your webopac. For example: http://library.org/
  * Mappings tab:
    - Taxonomy: MARC import mappings:
      If you set up Drupal taxonomy with the above optional steps, you can now 
      select which vocabularies will hold imported data like Subjects, Authors, 
      Language and more.
  * "Display" tab:
    - Display real-time holdings information:
      Choose when users will see holdings information.

You have several ways to import records:
    
  * Manual import: 
    You can specify a specific list or range of bibliographic or item numbers to 
    import. You can specify to import them inmediately (using Drupal's 
    Batch API) or to queue them to be processed during cron runs. Doing a small
    batch import is a good way to test your setup.
    
  * Auto-crawl:
    Specify a beginning and (estimated) ending record number and the module will 
    try to fetch records one by one automatically during cron runs. See "IMPORT
    VIA AUTO-CRAWL" section for more information.
    
  * One-by-one import:
    You can go to the URL millennium/preview/[recordnumber] and you will be 
    shown a preview showing how that record would be imported using your current
    settings. To import, click on the "Import now" link.

IMPORT VIA AUTO-CRAWL
======================
Go to the administration section at admin/settings/millennium, and enter the 
required settings:

  * Automatic crawl enabled: 
    mark it as "enabled"
  * Starting record number for crawl: 
    the bib record number, MINUS the starting .b or .i
    For example,  if you wanted to import from record .b120000 you would type 
    "120000"
  * Ending record number:
    Your estimated ending record number. This number grows on its own when 
    the module reaches it and still finds records after it (so don't worry) =)
  * Restart crawl from above starting record?
    Check this box whenever you want to restart the crawl from the beginning.
    (Only necessary if you're testing your setup).

Now, each time the Drupal Cron process is run, a few records will be imported. 
How many depends on the option "Items to import per Cron run". When the module 
finds a large gap in consecutive item numbers, it will restart from the 
beginning.

Remember, you can always run cron manually (see http://drupal.org/node/158922) 
or set up Cron to run automatically (see http://drupal.org/cron)

DEVELOPERS
================
Exposed hooks:

  hook_millennium_biblio_data_alter(&$biblio, $marc)
  
  Lets other modules add/change custom bibliographic data when storing biblio 
  data in the database.
  
    where:
      $biblio is the biblio information array
      $marc is the parsed MARC record
      
  hook_millennium_load_biblio_data_alter($node, &$biblio)
   
  Lets other modules add/change custom bibliographic data when fetching it from
  the database. This hook is invoked from within hook_nodeapi($op=load)
    
    where:
      $node is the node object 
      $biblio_data is the biblio data array

  hook_millennium_continue_process_record($node, $data, $force_update)

  Allows other modules to continue or skip importing of a record by returning
  TRUE (allow) or FALSE (skip this record).

    where:
      $node is the node object generated from a Millennium record
      $data is the incoming Millennium record array
      $force_update is the current active setting to force updating existing
        records.

MORE INFORMATION
================
See the project's page at http://drupal.org/project/millennium