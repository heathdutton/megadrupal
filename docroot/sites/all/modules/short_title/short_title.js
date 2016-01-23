(function ($) {
  Drupal.behaviors.short_title = {
    attach: function (context, settings) {
      // Show short title link.
      $('#show_short_title').bind('click', function() {
        shortTitle.slideDown('slow');
      });
     
      var title =  $('#edit-title', context);
      var shortTitle = $('#edit-field-short-title', context);
      var toggleShortTitle = $('#show_short_title', context);
      // Clone the required asterix element.
      var requiredEl = $('.form-required', context).clone();
      var shortTitleLabel = $('label', shortTitle);
      
      // Main title character count.
      title.bind('keyup paste cut', function() {
        // Delay until after event for paste and cut events.
        setTimeout(function() {
          var titleVal = title.val();
          if ((titleVal.length > Drupal.settings.short_title.max_length)) {
            if ((Drupal.settings.short_title.required == 1)) {
              // Add required asterix.
              shortTitleLabel.append(requiredEl[0]);
            }
            shortTitle.slideDown('slow');
          }
          else {
            // Remove required asterix.
            $('span', shortTitleLabel).remove();
            // Hide if short title is empty.
            if ($('.form-text', shortTitle).val() == '') {
              shortTitle.slideUp('slow');
              toggleShortTitle.fadeIn('slow');
            }
          }
        }, 10);
      });

      if (Drupal.settings.short_title.short_title == Drupal.settings.short_title.title || Drupal.settings.short_title.short_title == '') {
        shortTitle.hide();
      }
      else {
        // Hide change short title.
        toggleShortTitle.hide();
      }
    },
  };
}(jQuery));
