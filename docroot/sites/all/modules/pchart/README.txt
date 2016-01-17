About pChart module
-------------------
pChart module is an API for developers. It does nothing by itself. It should only be installed if some other module requires it or you are building module on top of it. This module provides a basic wrapper to integrate pChart with Drupal.

About pChart http://www.pchart.net/
-----------------------------------
pChart is a PHP library that will help you to create anti-aliased charts or pictures directly from your web server. You can then display the result in the client browser, sent it by mail or insert it into PDFs.

Requirements
------------
pChart require the GD and FreeType PHP extensions to be installed on your web server.

Installation
------------
- Install this module like any other Drupal module
- Download pChart and extract it into a subfolder called pchart under
    sites/all/libraries/
  - OR -
    The module folder
    sites/*/modules/pchart/

  So you will end up with sites/all/libraries/pchart/ or sites/*/modules/pchart/pchart/

APIs
----

/**
 * Load needed files.
 *
 * @param $additional
 *   Additional class name or an array of classes to load.
 *   e.g. pchart_add('pScatter') or pchart_add(array('pRadar', 'pScatter', 'pSplit'))
 *
 * @see http://wiki.pchart.net/doc.introduction.html
 */
function pchart_add($additional=NULL) {
  ...
}

Add the pchart library in your Drupal module. By default this adds pData.class.php, pDraw.class.php and pImage.class.php from pChart to be used. Additional pChart classes can be loaded by passing a class name (1 class only) or an array of classes names, e.g. pchart_add('pScatter') or pchart_add(array('pRadar', 'pScatter', 'pSplit')).


/**
 * Get font properties for pChart setFontProperties().
 *
 * @param $font
 *   The font name with extension, default to 'GeosansLight.ttf'
 * @param $size
 *   The font size, default to 12
 * @param $format
 *   Additional properties to pass to setFontProperties()
 *
 * @return
 *   The array of font properties.
 */
function pchart_get_font_properties($font='GeosansLight.ttf', $size=12, $format=NULL) {
  ...
}

E.g.
  $picture->setFontProperties(pchart_get_font_properties('GeosansLight.ttf', 14, array('R' => 0, 'G' => 0, 'B' => 0)));

E.g.
  $text_settings = pchart_get_font_properties('GeosansLight.ttf', 14, array('R' => 0, 'G' => 0, 'B' => 0, 'Align' => TEXT_ALIGN_MIDDLEMIDDLE)));


/**
 * Get the location of the pChart library.
 *
 * @param $type
 *   The path type to get, default to 'class'.
 *   Other valid types are 'fonts', 'data', 'palettes'.
 *
 * @return
 *   The location of the library, or FALSE if the library isn't installed.
 */
function pchart_get_path($type='class') {
  ...
}

E.g.
  $data->loadPalette(pchart_get_path('palettes') . 'autumn.color', TRUE);

E.g.
  pchart_get_font_properties('GeosansLight.ttf', 12, array(
    'ValueFontName' => pchart_get_path('fonts') . 'calibri.ttf',
    ...
  ));