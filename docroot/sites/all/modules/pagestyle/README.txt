$Id: README.txt,v 1.4 2009/12/10 23:36:28 christianzwahlen Exp $

Drupal Page Style Module
------------------------
Author - Christian Zwahlen info@zwahlendesign.ch
Requires - Drupal 7
License - GPL (see LICENSE.txt)

Description
-----------
This module display a style changer on the page and in the browser menue for a better web accessibility.

Advantages
----------
1. No building of extra themes required! Black/White, White/Black and Yellow/Blue.
2. Style changer in the browser available.
3. One click function. No tutorial required.
4. Same handling in any browser (shortcuts).
5. Adjustable.
6. Select menu availabe.
7. Display current page style.
8. Browser independent.

Features
--------
* WCAG and BITV conform.
* No Javascript and Cookies required. jQuery support.
* No CSS required.
* Style display settings.
* Caching mode compatible (Block cache and Page cache for anonymous users).
* Bandwidth optimization compatible.
* Uniform markin of menu, based on the open source browser Firefox 2.0 (www.mozilla.com/en-US/firefox/) (Page Style).
* Themable. See: pagestyle-text.tpl.php, pagestyle-image.tpl.php or pagestyle-form.tpl.php.
* No JavaScript support available.

Installation
------------
1. Move the pagestyle modul to the Drupal directory "sites/all/modules/".
2. Go to: "Configuration and modules", "Modules" (admin/config/modules/) and enable the module "Page Style".
3. Run the database update script "update.php" if required.

Settings
--------
1. Go to: "Configuration and modules", "People and permissions", "Permissions" ("admin/config/people/permissions#module-pagestyle) and set the different roles.
2. Go to: "Structure", "Blocks" ("admin/structure/block") and make the block "Page Style" visibile.
3. Go to "Configuration and modules", "Content authoring", "Page Style configuration" ("admin/config/content/pagestyle") and change the settings if you want.
4. Go to "Configuration and modules", "Regional and language", "Translate interface", "Import" ("admin/config/regional/translate/import") and import your language file (for German "/modules/pagestyle/translations/de.po") if it is necessary.

Theming
-------
Create Themes:
1.  Copy the files/directorys in your theme:
  1. pagestyle-text.tpl.php
  2. pagestyle-image.tpl.php
  3. pagestyle-form.tpl.php
  4. pagestyle.css
  5. images
2. Edit the PHP and CSS code in the files. Example: Image links, not in a list: Change the following elements: <ul>/</ul> to: <p>/</p>
  <li></li> to: <span class="display_hidden"> | </span>.
  WCAG/BITV 10.5: Adjacent hyperlinks have to be separated by printable characters surrounded by spaces. Warning: Not for: WCAG/BITV 13.6! Links are not in a list.
3. Edit the PHP templates (tpl.php) and pagestyle.css file. Example: "Image links, not in a list: Change the following elements: <ul>/</ul> to: <p>/</p>
  <li></li> to: <span class="display_hidden"> | </span>.
  WCAG/BITV 10.5: Adjacent hyperlinks have to be separated by printable characters surrounded by spaces. Warning: Not for: WCAG/BITV 13.6! Links are not in a list.
4. Edit the images. Create your own icons. The source file is in the directory: "/images/source/pagestyle.xcf". PNG images are for new browsers and GIF images for old browsers (Internet Explorer 6).

Download Themes: Edited templates are available from the developers website: http://www.zwahlendesign.ch/pagestyle.

Translations
------------
German
Importieren Sie die Datei "sites/all/modules/pagestyle/translations/de.po". unter "admin/build/translate/import" in die Deutsche Sprache, wenn es nötig ist.

Other languages
Open the file "sites/all/modules/pagestyle/translations/pagestyle.pot". in poEdit (http://www.poedit.net) or KBabel (http://kbabel.kde.org), translate the module in your language and save the file in "sites/all/modules/pagestyle/translations/" with a file suffix ".po".
