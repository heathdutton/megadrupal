
Biblio Advanced Import
======================

Name: biblio_advanced_import
Authors: Markus Kalkbrenner | bio.logis
         Christian Spitzlay | bio.logis
Drupal: 7.x
Sponsors: Cocomore AG - http://www.cocomore.com
          bio.logis   - http://www.biologis.com

About
=====

"Biblio Advanced Import" is an add-on for the Bibliography Module,
which can be found at http://drupal.org/project/biblio

Instead of creating duplicate biblio records during imports,
existing ones can be updated, or the import can be skipped
depending on a configurable duplicate detection strategy.



Installation
============

1. Install Bibliography Module itself from
   http://drupal.org/project/biblio

2. Place whole biblio_advanced_import folder into your Drupal
   modules/ directory, or better, your sites/x/modules directory.

3. Enable the "Biblio Advanced Import" module at /admin/build/modules



Usage
=====

Configure your duplicate detection strategy at
/admin/config/content/biblio/advanced_import
