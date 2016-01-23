<?php
/**
 * @file
 *   Function related to the Building of the Piecemaker XML file
 */
/**
 * Builds a Piecemaker XML file
 * 
 */
function piecemaker_xml_build($settings) {
  drupal_add_http_header('Content-Type', 'text/xml; charset=utf-8');
  print theme('piecemaker_xml', $settings);
  exit();
}
 
/**
 * Preprocess Function for the Settings XML file
 * @param $vars
 *   An associative array of the needed items to generate an XML file
 *     - 'Settings': The settings array of to be used. Most likely this comes from a piecemaker profile
 *     - 'Transitions': The transitions array to be used. Once again this is most likely comming from a profile
 *     - 'Contents': An item array containing the following keys
 *       - '#type': Either 'Image', 'Video', or 'Flash'
 *       - '#attributes': An array of attributes to add to the main node
 *         - 'Source': The Source of the file. This should be generated using $base_url to ensure it points correctly. REQUIRED for all file types.
 *         - 'Title': The title of the file. Will be shown in the tooltip on the menu. Avaialable to all file types.
 *         - 'Width': Only used in Video files. The width of the video file.
 *         - 'Height': Only used in Video files. The height of the video file.
 *         - 'Autoplay': Only used in Video files. Autoplay defines whether or not the video will start playing as soon as it‘s ready.
 *       - 'Text': Used in Image nodes. The Text node can hold a description text, which can be formatted with simple HTML tags.
 *         Which HTML tags are supported, is determined by the piecemaker.css file. Per default you can use <h1>, <p>, <p-italic>, <a>.
 *         But you can add and specify as many tags as you want, for example different headline types. If you want to use different fonts,
 *         you will have to make sure that you embed these fonts in the Flash file. If you don‘t add a Text node, no info text appears.
 *       - 'Hyperlink': An array that contains an #attributes key which holds the attributes URL and Target.
 *       - 'Image': The Flash and Video nodes must have a child array Image, which will also have a Source #attribute key. This will specify the path to a preview image to be loaded before the SWF/Video file is shown.
 *          This preview image might show the first image of the SWF/Video file though, which would cause a smooth transition from the image to the actual SWF/Video.
 *          Please note that this image will still be shown left and right of the video, in case that the video is not as wide as the image.
 */
function template_preprocess_piecemaker_xml(&$vars) {
  foreach($vars['Contents'] as $item) {
    $Contents[] = theme('piecemaker_xml_node', array('item' => $item));
  }
  $vars['ContentNodes'] = $Contents;
}

/**
 * Generates the individual nodes for a piecemaker XML file.
 */
function theme_piecemaker_xml_node(&$vars) {
  $hook = 'piecemaker_xml_node_' . $vars['item']['#type'];
  return theme($hook, $vars);
}
