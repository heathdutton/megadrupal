(function ($) {

/**
 * jQuery Scroll Follow init
 */
Drupal.behaviors.jqueryscrollfollow = {

  attach: function(context) {

    $(Drupal.settings.jquery_scroll_follow.jquery_scroll_follow_css_target).scrollFollow({

    speed: Drupal.settings.jquery_scroll_follow.jquery_scroll_follow_settings_speed,
    offset: parseInt(Drupal.settings.jquery_scroll_follow.jquery_scroll_follow_settings_offset),
    delay: parseInt(Drupal.settings.jquery_scroll_follow.jquery_scroll_follow_settings_delay)

    });

  }

};

})(jQuery);
