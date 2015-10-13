/**
 * @file
 * Provides UI improvement functionality to grab "insert" fields and display handy overlay.
 */
(function($) {

  Drupal.behaviors.inline_image_button = {

    attach: function() {

      // Discover which fields have insert module enabled on them- this should only ever be 1
      $.each($('.form-wrapper.insert-enabled'), function(index, value) {

        // The master field wrapper is 2 parents up
        var element = $(this).parent().parent();

        // Apply class to field wrapper to allow generic CSS to be applied
        $(element).addClass('insert-enabled-wrapper');

        // Once-like behaviour
        if ($(element).find('.show-toggle').size() == 0) {

          // Insert a close button on popup form
          $(element).find(' > div').prepend('<a class="show-toggle" href="#">' + Drupal.t('Close') + '</a>');

          // Slide up popup when close is clicked
          $(element).find('.show-toggle').click(function() {
            $(element).slideUp();
            return false;
          });

        }

        // Prevent image/file field ajax from calling this more than once
        $(element).once(function() {

          // Make popup draggable and re position when scrolling
          $(element).draggable();
          $(window).scroll(function() {
            $(element).css('top', $(window).scrollTop() - 50);
          });

        });

      });

    }

  };

  /**
   * WYSIWYG plugin trigger
   */
  Drupal.wysiwyg.plugins.inline_image_button = {

    invoke: function(data, settings, instanceId) {

      if ($('.form-wrapper.insert-enabled').size() == 0) {
        alert('No image or file field has been set on this content type with Insert enabled.');
      }
      else {

        // Discover which fields have insert module enabled on them- this should only ever be 1

        $.each($('.form-wrapper.insert-enabled'), function(index, value) {

          // The master field wrapper is 2 parents up
          var element = $(this).parent().parent();

          // Slide down and positioning rules
          $(element).slideDown();
          $(element).css('top', $(window).scrollTop() - 50);
          $(element).css('right', 0);
          $(element).css('left', '');

        });

      }

    },

  };

})(jQuery);
