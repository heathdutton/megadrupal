/**
 * @file
 *  Helper JS to set dynamically generated styles for CKEditor.
 */


var CKEDITOR = CKEDITOR || false;

if (CKEDITOR) {
  // Add each styleset (defined by the module) to CKEDITOR.
  var undpaul = Drupal.settings.ckeditor_styles;
  jQuery.each(undpaul, function(styleset, specs) {
    CKEDITOR.stylesSet.add(styleset, specs);
  });
}
