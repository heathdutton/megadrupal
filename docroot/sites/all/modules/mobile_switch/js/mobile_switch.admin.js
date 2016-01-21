/**
 * @file
 * Required functions to use the Mobile Switch administration.
 */

(function ($) {
 Drupal.behaviors.mobileSwitch = {
  attach: function(context, settings) {
   // Basic settings form; Hide/show "Tablet device" and
   // Hide/show "Administration usage"
   if ($('#edit-mobile-switch-mobile-theme').val() == 'none') {
    $('.form-item-mobile-switch-tablet-usage').hide();
    $('.form-item-mobile-switch-admin-usage').hide();
   }
   else {
    $('.form-item-mobile-switch-tablet-usage').show();
    $('.form-item-mobile-switch-admin-usage').show();
   }
   // Show "Redirect URL's" message.
   if ($('#edit-mobile-switch-mobile-theme').val() == 'redirect') {
     $('.form-item-mobile-switch-redirect-url-message').show();
   }
   $('#edit-mobile-switch-mobile-theme').click(function() {
    if ($(this).val() == 'none') {
     $('.form-item-mobile-switch-tablet-usage').hide();
     $('.form-item-mobile-switch-admin-usage').hide();
    }
    else {
     $('.form-item-mobile-switch-tablet-usage').show();
     $('.form-item-mobile-switch-admin-usage').show();
    }
    // Hide/show "Redirect URL's" message.
    if ($(this).val() == 'redirect') {
      $('.form-item-mobile-switch-redirect-url-message').show();
    }
    else {
      $('.form-item-mobile-switch-redirect-url-message').hide();
    }
   });
   // Advanced form; Hide/show preventing strings textarea.
   if ($('#edit-mobile-switch-prevent-devices').val() == 0) {
    $('.form-item-mobile-switch-prevent-devices-strings').hide();
   }
   else {
    $('.form-item-mobile-switch-prevent-devices-strings').show();
   }
   $('#edit-mobile-switch-prevent-devices').click(function() {
    if ($(this).val() == '0') {
     $('.form-item-mobile-switch-prevent-devices-strings').hide();
    }
    else {
     $('.form-item-mobile-switch-prevent-devices-strings').show();
    }
   });
   // Development form; Hide/show "Advanced developer modus settings"
   // and uncheck "Display user agent".
   if ($('#edit-mobile-switch-developer').val() == 0) {
    $('#edit-advanced').hide();
   }
   else {
    $('#edit-advanced').show();
   }
   $('#edit-mobile-switch-developer').click(function() {
    if ($(this).val() == 0) {
     togggleUa();
     $('#edit-advanced').hide();
    }
    else {
     $('#edit-advanced').show();
    }
   });
   // Uncheck "Display user agent" and "Display Mobile Detect informations".
   function togggleUa() {
    if ($('#edit-mobile-switch-display-useragent').is(':checked')) {
     $('#edit-mobile-switch-display-useragent').attr('checked', false);
    }
    if ($('#edit-mobile-switch-display-mobiledetectinfo').is(':checked')) {
     $('#edit-mobile-switch-display-mobiledetectinfo').attr('checked', false);
    }
   }
  }
 };
})(jQuery);
