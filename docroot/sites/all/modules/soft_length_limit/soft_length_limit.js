(function ($) {
  Drupal.behaviors.softLengthLimit = {
    attach: function (context, settings) {

      // Preparing the input elements by adding a tooltip container.
      $('.soft-length-limit:not(.soft-length-limit-processed)').each(function(index){
        $(this).addClass('soft-length-limit-processed');

        var $parent = $(this).parent();
        $parent.css('position','relative');
        $parent.append('<div class="soft-length-limit-tooltip description"></div>');
        $element = $(this);

        // Used for automatically moving the tooltip when resizing the
        // text area.
        $parent.find('.grippie').mousedown({
          element: $(this)
        }, function(event){
          var endDrag = function(event) {
            event.data.element.focus();
            $(document).unbind('mouseup',endDrag);
          };
          $(document).mouseup(event.data,endDrag);
        });
      });

      // Calculates the correct position of the tooltip and shows it.
      $('.soft-length-limit').focus(function(event){
        var $tooltip = $(this).parent().find('.soft-length-limit-tooltip');
        var left = $(this).position().left;
        var top = $(this).position().top;
        var bottom = top + $(this).outerHeight(true);
        var right = left + $(this).outerWidth(true);
        $(this).trigger('textchange', $(this).val());

        if ($(this).get(0).tagName == "TEXTAREA") {
          $tooltip.css('left', 10).css('top', bottom + 5);
        }
        else {
          $tooltip.css('left', 10).css('top', bottom - 4);
        }

        $tooltip.fadeIn('fast');
      });

      // Hides the tooltip.
      $('.soft-length-limit').blur(function(event){
        var $tooltip = $(this).parent().find('.soft-length-limit-tooltip');
        $tooltip.fadeOut('fast');
      });

      // Shows the relevant info to the user in the tooltip.
      $('.soft-length-limit').bind('textchange', function(event, prevText){
        var limit = $(this).attr('data-soft-length-limit');
        var minimum = $(this).attr('data-soft-length-minimum');
        var val = $(this).val();
        var remaining = limit - val.length;
        var $tooltip = $(this).parent().find('.soft-length-limit-tooltip');
        var styleSelect = $(this).attr('data-soft-length-style-select');

        // Removes the "exceeded" class if length is not exceeded
        // anymore.
        if (prevText.length > limit && val.length <= limit) {
          $tooltip.removeClass('exceeded');
          $(this).removeClass('exceeded');
        }

        // Adds the "exceeded" class if length is exceeded.
        if (val.length > limit && !$tooltip.hasClass('exceeded')) {
          $tooltip.addClass('exceeded');
          $(this).addClass('exceeded');
        }

        // Adds the "exceeded" class if the length is less than the minimum, in the case where the minimum is set.
        // Adds the "under-min" class, for under-character-minimum specific behavior. Used for min/enhanced character count.
        if (minimum > 0) {
            if (val.length < minimum) {
                $tooltip.addClass('under-min');
                $(this).addClass('under-min');
            }
            if (val.length >= minimum) {
                $tooltip.removeClass('under-min');
                $(this).removeClass('under-min');
            }
            if (val.length > limit) {
                $tooltip.addClass('exceeded');
                $(this).addClass('exceeded');
            }
            if (val.length <= limit) {
                $tooltip.removeClass('exceeded');
                $(this).removeClass('exceeded');
            }
        }

        // The minimal/enhanced version of character limits
        if (styleSelect == '1') {
            // No minimum treatment
            $tooltip.html(Drupal.t('@val/@limit', {
                '@val': val.length,
                '@limit': limit
            }));
            // Add class to tooltip for different CSS treatment (icons, text alignment, etc )
            $tooltip.parent().children('.soft-length-limit-tooltip').addClass('min-style-tooltip');
        }

        // Original character limit treatment
        if ((styleSelect == '0') || (styleSelect == undefined)) {
            // No minimum value is set.
            if (minimum < 1) {
              if (val.length === 0) {
                $tooltip.html(Drupal.t('Content limited to @limit characters',{
                  '@limit': limit
                }));
              }
              else if (remaining < 0) {
                $tooltip.html(Drupal.t('@limit character limit exceeded by @exceed characters.',{
                  '@limit': limit,
                  '@exceed': -remaining
                }));
              }
              else {
                $tooltip.html(Drupal.t('Content limited to @limit characters. Remaining: @remaining',{
                  '@limit': limit,
                  '@remaining': remaining
                }));
              }
            }
            // There is a minimum length set.
            else {
              if (val.length === 0) {
                $tooltip.html(Drupal.t('Suggested minimum number of characters is @minimum, current count is @val.  Content limited to @limit characters. ',{
                  '@limit': limit,
                  '@minimum': minimum,
                  '@val': val.length
                }));
              }
              else if (remaining < 0) {
                $tooltip.html(Drupal.t('Suggested minimum number of characters is @minimum, current count is @val.  @limit character limit exceeded by @exceed characters.',{
                  '@limit': limit,
                  '@exceed': -remaining,
                  '@minimum': minimum,
                  '@val': val.length
                }));
              }
              else {
                $tooltip.html(Drupal.t('Suggested minimum number of characters is @minimum, current count is @val.  Content limited to @limit characters. Remaining: @remaining.',{
                  '@limit': limit,
                  '@remaining': remaining,
                  '@minimum': minimum,
                  '@val': val.length
                }));
              }
            }
        }
      });
    }
  };

})(jQuery);
