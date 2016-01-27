// $Id$

/**
 * @file
 * The main javascript file for the users_export module
 *
 * @ingroup users_export
 * @{
 */

(function ($) {

  Drupal.usersExport = Drupal.usersExport || {};

  /**
  * Core behavior for users_export.
  */
  Drupal.behaviors.usersExport = Drupal.behaviors.usersExport || {};
  Drupal.behaviors.usersExport.attach = function (context, settings) {
    $('#edit-users-export-type').change(function(){
      var id = $(this).val();
      var extension = Drupal.settings.usersExport[id].extension;
      $('#edit-users-export-filename+.field-suffix').html(extension);
    })
  }

  /**
  * @} End of "defgroup users_export".
  */

})(jQuery);
