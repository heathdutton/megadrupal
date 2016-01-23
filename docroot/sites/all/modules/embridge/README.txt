-- SUMMARY --

The EMBridge module (located here), created and maintained by staff at DPCI, 
extends the image management functionalities of Drupal by connecting it to 
EnterMedia, an open-source digital asset management system distributed under 
the GNU General Public License, used to search, manage, reuse, and track all
digital files.

See http://drupal.org/node/1211132 for more details.

-- REQUIREMENTS --

* EnterMedia Server (DAM) must be installed and configured.


* The embridge_field requires content (CCK) module.


-- INSTALLATION --

* Install as usual, install and enable both embridge and embridge_field.


-- EMBRIDGE CONFIGURATION --

* Go to Administer > EnterMedia > EnterMedia Settings and

  - enter host name and port number for EnterMedia DAM server.
  - enter media store path for EnterMedia server, where the mounted folders for
  catalogs resize. Refer details to EnterMedia configuration.
  - leave the asset path pattern by default ([yyyy]/[mm]), which will create 
  subfolders based on current year and month when upload.
  - enter login and password for EnterMedia server.
  - click "Test Connection" to verify if we can connect and login to EnterMedia
  server. If we are getting "success" message, then our basic server 
  configuration for EnterMedia are complete.
  
* Continue settings for each catalog (audio, interactive, photo and video).
  - Renditions: selected renditions will be included in embridge_field
  for nodes.
  - Search Fields: selected fields will be displayed in EnterMedia Search form
  as search conditions.
  - Search Result Fields: selected fields will be included in EnterMedia Search
  result.

-- EMBRIDGE_FIELD CONFIGURATION --

* Install as usual CCK fields.
  
* Configure widget settings.
  - EnterMedia Catalog: This is required field which indicates the asset type
  and catalog in EnterMedia the current field will go.
  - Asset Path: leave the asset path pattern by default ([yyyy]/[mm]), which
  will create subfolders based on current year and month when upload.
  - File extensions and File size are similar as CCK file and image.
