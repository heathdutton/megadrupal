/** 
 * JavaScript Document
 * 
 * Fills in certain add contact form fields. 
 */

(function ($) {

//Module namespace:
var NamecardsImport = NamecardsImportFillForm || {};

Drupal.behaviors.NamecardsImportFillForm = {
  attach: function (context) {
    try {
      if (Drupal.settings.namecardsImport.organizationName) {
        $('#edit-namecards-namecard-organization-und-0-nid:not(.namecardsImport-processed)', context).addClass('.namecardsImport-processed').val(Drupal.settings.namecardsImport.organizationName);
      }
      if (Drupal.settings.namecardsImport.positionName) {
        $('#edit-namecards-namecard-position-und-0-nid:not(.namecardsImport-processed)', context).addClass('.namecardsImport-processed').val(Drupal.settings.namecardsImport.positionName);
      }
      if (Drupal.settings.namecardsImport.departmentName) {
        $('#edit-namecards-namecard-department-und-0-nid:not(.namecardsImport-processed)', context).addClass('.namecardsImport-processed').val(Drupal.settings.namecardsImport.departmentName);
      }
    }
    catch(e) {
      // Use of try-catch used to contain errors if Drupal.settings.namecardsImport has not been set. 
    } 
  }
};

}(jQuery));