/**
 * @file
 * Js file for cloud_monitoring
 */

(function ($) {
  Drupal.behaviors.existingTemplateCheck = {
    attach: function (context, settings) {
      //check reload from validation or onload
      if ($('#edit-existing').is(':checked')) {
        Drupal.hideSystemElements();
        Drupal.showExistingElements();
      }
      else {
        Drupal.showSystemElements();
        Drupal.hideExistingElements();
      }
      
      $('#edit-existing').bind('click', function() {
        if ($('#edit-existing').is(':checked')) {
          Drupal.hideSystemElements();
          Drupal.showExistingElements();
        }
        else {
          Drupal.showSystemElements();
          Drupal.hideExistingElements();
        }
      });
    }
  };
  Drupal.hideSystemElements = function() {
    $('.form-item-system-template').hide();
    $('.form-item-cloud-type').hide();
    $('.form-item-monitor-type').hide();
    $('#ssh-wrapper').hide();
    $('.form-item-email').hide();
  };

  Drupal.showSystemElements = function() {
    $('.form-item-system-template').show();
    $('.form-item-cloud-type').show();
    $('.form-item-monitor-type').show();
    $('#ssh-wrapper').show();
    $('.form-item-email').show();
  };

  Drupal.hideExistingElements = function() {
    $('.form-item-server-template').hide();
  };

  Drupal.showExistingElements = function() {
    $('.form-item-server-template').show();
  };
})(jQuery);



