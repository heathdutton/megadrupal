(function($) {
  Drupal.behaviors.prettyphoto_formatters = {
    attach: function(context, settings) {
      $("a[rel^='prettyPhoto']", context).once('prettyphoto_formatters').prettyPhoto({
        animation_speed: Drupal.settings.prettyphoto.animation_speed,
        slideshow: Drupal.settings.prettyphoto.slideshow,
        autoplay_slideshow: Drupal.settings.prettyphoto.autoplay_slideshow,
        opacity: Drupal.settings.prettyphoto.opacity,
        show_title: Drupal.settings.prettyphoto.show_title,
        allow_resize: Drupal.settings.prettyphoto.allow_resize,
        default_width: Drupal.settings.prettyphoto.default_width,
        default_height: Drupal.settings.prettyphoto.default_height,
        counter_separator_label: Drupal.settings.prettyphoto.counter_separator_label,
        theme: Drupal.settings.prettyphoto.theme,
        horizontal_padding: Drupal.settings.prettyphoto.horizontal_padding,
        hideflash: Drupal.settings.prettyphoto.hideflash,
        wmode: Drupal.settings.prettyphoto.wmode,
        autoplay: Drupal.settings.prettyphoto.autoplay,
        modal: Drupal.settings.prettyphoto.modal,
        deeplinking: Drupal.settings.prettyphoto.deeplinking,
        overlay_gallery: Drupal.settings.prettyphoto.overlay_gallery,
        keyboard_shortcuts: Drupal.settings.prettyphoto.keyboard_shortcuts,
        ie6_fallback: Drupal.settings.prettyphoto.ie6_fallback,
        markup: Drupal.settings.prettyphoto.markup,
        gallery_markup: Drupal.settings.prettyphoto.gallery_markup,
        image_markup: Drupal.settings.prettyphoto.image_markup,
        flash_markup: Drupal.settings.prettyphoto.flash_markup,
        quicktime_markup: Drupal.settings.prettyphoto.quicktime_markup,
        iframe_markup: Drupal.settings.prettyphoto.iframe_markup,
        inline_markup: Drupal.settings.prettyphoto.inline_markup,
        custom_markup: Drupal.settings.prettyphoto.custom_markup,
        social_tools: Drupal.settings.prettyphoto.social_tools
      });
    }
  };
})(jQuery);
