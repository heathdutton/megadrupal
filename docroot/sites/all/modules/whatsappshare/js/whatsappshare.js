/**
 * Javascript for the whatsappshare module.
 */

(function ($) {
    Drupal.behaviors.whatsappshare = {
      attach: function(context) {
        $(Drupal.settings.whatsappshare.whatsappshare_sharing_location).append('<a href="whatsapp://send" data-text="' + Drupal.settings.whatsappshare.whatsappshare_sharing_text + '" data-href="' + Drupal.settings.whatsappshare.base_path + '" class="wa_btn ' + Drupal.settings.whatsappshare.whatsappshare_button_size + '" style="display:none; margin:1px;">' + Drupal.settings.whatsappshare.whatsappshare_button_text + '</a>');
      }
    }
})(jQuery);
