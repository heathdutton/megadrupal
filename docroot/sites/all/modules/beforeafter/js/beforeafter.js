(function ($) {
  Drupal.behaviors.beforeafter = {
    attach: function (context, settings) {
      if (settings.beforeafter) {
        // Looping through all image fields with before/after formatting enabled on a page.
        $.each(settings.beforeafter, function (index, before_after_instances) {
          // Looping through each image pair of that field.
          $.each(before_after_instances, function (index, before_after_settings) {
            $('#' + before_after_settings.container).beforeAfter({
              imagePath: before_after_settings.image_path + '/',
              animateIntro : before_after_settings.animate_intro,
              introDelay : before_after_settings.intro_delay,
              introDuration : before_after_settings.intro_duration,
              showFullLinks : before_after_settings.show_full_links,
              dividerColor: '#' + before_after_settings.divider_color,
              introPosition : before_after_settings.intro_position,
              beforeLinkText: before_after_settings.before_link_text,
              afterLinkText: before_after_settings.after_link_text,
              cursor: before_after_settings.cursor,
              clickSpeed: before_after_settings.click_speed,
              linkDisplaySpeed: before_after_settings.link_display_speed,
            });
          })
        })
      }
    }
  }
})(jQuery);
