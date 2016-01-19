
INSTALLATION
  1. Download the module and extract the files.
  2. Upload the entire adverticum folder into your Drupal sites/all/modules/
     or sites/my.site.folder/modules/ directory if you are running a multi-site
     installation of Drupal and you want this module to be specific to a
     particular site in your installation.
  3. Enable the Adverticum module by navigating to:
     Administration > Modules
  4. Adjust settings by navigating to:
     Administration > Configuration > Adverticum

USAGE
  Every zone has a unique ID, you can copy this information from your adserver.
  You can specify which tag (GOA3 or JavaScript) to use for each zone.

  The module offers as many blocks as zones are defined. The blocks are named
  after the zones in format "Adverticum Zone 0", "Adverticum Zone 1" etc. You may
  enable or disable blocks under Adminstration > Structure > Blocks.

  However you can temporarly disable a zone by unchecking the "Active" checkbox
  on the configuration page.

  By using the zone filter, you can insert ad zones directly in any content (where input formats
  are allowed) in [adverticum id="ZONEID"] format. Later if you change the invocation code
  type for a zone, don't forget to clear the cache!

CREDITS
  - Brainsum s.r.o <http://www.brainsum.sk/>
