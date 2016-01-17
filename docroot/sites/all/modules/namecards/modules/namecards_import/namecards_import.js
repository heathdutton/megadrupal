/** 
 * JavaScript Document
 * 
 * Add behaviours for CSV file download elements. 
 */

(function ($) {

//Module namespace:
var NamecardsImport = NamecardsImport || {};

Drupal.behaviors.NamecardsImport = {
  attach: function (context) {
    // Add hide CSV file download link when close button is pressed. 
    $('#namecards-download-file-close-button-link').each(function(index, ele) {
      $(ele).click(function() {
        $('#namecards-import-export-csv-file').fadeOut();
        return false;
      });
    });
  }
};

}(jQuery));