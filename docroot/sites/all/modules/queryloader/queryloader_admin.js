(function ($) {

  /**
   * Provide the summary information for the queryloader visibility vertical tabs.
   */
  Drupal.behaviors.blockSettingsSummary = {
    attach: function (context) {
      // Vertical tabs (adapted from Block module)
      if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
        return;
      }

      $('fieldset#edit-path', context).drupalSetSummary(function (context) {
        if (!$('textarea[name="pages"]', context).val()) {
          return Drupal.t('Not restricted');
        }
        else {
          return Drupal.t('Restricted to certain pages');
        }
      });

      $('fieldset#edit-node-type', context).drupalSetSummary(function (context) {
        var vals = [];
        $('input[type="checkbox"]:checked', context).each(function () {
          vals.push($.trim($(this).next('label').text()));
        });
        if (!vals.length) {
          vals.push(Drupal.t('Not restricted'));
        }
        return vals.join(', ');
      });

      $('fieldset#edit-role', context).drupalSetSummary(function (context) {
        var vals = [];
        $('input[type="checkbox"]:checked', context).each(function () {
          vals.push($.trim($(this).next('label').text()));
        });
        if (!vals.length) {
          vals.push(Drupal.t('Not restricted'));
        }
        return vals.join(', ');
      });

      $('fieldset#edit-user', context).drupalSetSummary(function (context) {
        var $radio = $('input[name="custom"]:checked', context);
        if ($radio.val() == 0) {
          return Drupal.t('Not customizable');
        }
        else {
          return $radio.next('label').text();
        }
      });

      //  Color fields
      $('.color-box').bind('keydown mousedown mouseup keyup change', function () {
        // Get the background and text color
        var _bgColor = $(this).val();
        var _textColor = (parseInt(_bgColor.replace('#', ''), 16) > 0xffffff / 2) ? 'black' : 'white';

        // Set the background and text Color
        $(this).css({
          'color': _textColor,
          'background-color': _bgColor
        })

      }).trigger('change');

    }
  };


})(jQuery);

