/**
 * @file
 * A JavaScript file for the theme.
 * This file should be used as a template for your other js files.
 * It defines a drupal behavior the "Drupal way".
 *
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth

(function ($, Drupal, window, document, undefined) {
  'use strict';

  // To understand behaviors, see https://drupal.org/node/756722#behaviors
  Drupal.behaviors.hideSubmitBlockit = {
    attach: function(context) {
      var timeoutId = null;
      $('form', context).once('hideSubmitButton', function () {
        var $form = $(this);

        // Bind to input elements.
        $('input.form-submit, button.form-submit', $form).click(function (e) {
          var el = $(this);
          el.after('<input type="hidden" name="' + el.attr('name') + '" value="' + el.attr('value') + '" />');
          return true;
        });

        // Bind to form submit.
        $('form', context).submit(function (e) {
          var $inp;
          if (!e.isPropagationStopped()) {
            if (Drupal.settings.hide_submit.hide_submit_method === 'disable') {
              $('input.form-submit, button.form-submit', $form).attr('disabled', 'disabled').each(function (i) {
                var $button = $(this);
                if (Drupal.settings.hide_submit.hide_submit_css) {
                  $button.addClass(Drupal.settings.hide_submit.hide_submit_css);
                }
                if (Drupal.settings.hide_submit.hide_submit_abtext) {
                  $button.val($button.val() + ' ' + Drupal.settings.hide_submit.hide_submit_abtext);
                }
                $inp = $button;
              });

              if ($inp && Drupal.settings.hide_submit.hide_submit_atext) {
                $inp.after('<span class="hide-submit-text">' + Drupal.checkPlain(Drupal.settings.hide_submit.hide_submit_atext) + '</span>');
              }
            }
            else {
              var pdiv = '<div class="hide-submit-text' + (Drupal.settings.hide_submit.hide_submit_hide_css ? ' ' + Drupal.checkPlain(Drupal.settings.hide_submit.hide_submit_hide_css) + '"' : '') + '>' + Drupal.checkPlain(Drupal.settings.hide_submit.hide_submit_hide_text) + '</div>';
              if (Drupal.settings.hide_submit.hide_submit_hide_fx) {
                $('input.form-submit, button.form-submit', $form).addClass(Drupal.settings.hide_submit.hide_submit_css).fadeOut(100).eq(0).after(pdiv);
                $('input.form-submit, button.form-submit', $form).next().fadeIn(100);
              }
              else {
                $('input.form-submit, button.form-submit', $form).addClass(Drupal.settings.hide_submit.hide_submit_css).hide().eq(0).after(pdiv);
              }
            }
            // Add a timeout to rerset the buttons (if needed).
            if (Drupal.settings.hide_submit.hide_submit_reset_time) {
              timeoutId = window.setTimeout(function() {
                hideSubmitResetButtons(null, $form);
              }, Drupal.settings.hide_submit.hide_submit_reset_time);
            }
          }
          return true;
        });
      });

      // Bind to clientsideValidationFormHasErrors to support clientside validation.
      // $(document).bind('clientsideValidationFormHasErrors', function(event, form) {
        //hideSubmitResetButtons(event, form.form);
      // });

      // Reset all buttons.
      function hideSubmitResetButtons(event, form) {
        // Clear timer.
        window.clearTimeout(timeoutId);
        timeoutId = null;
        if (Drupal.settings.hide_submit.hide_submit_method === 'disable') {
          $('input.' + Drupal.checkPlain(Drupal.settings.hide_submit.hide_submit_css) + ', button.' + Drupal.checkPlain(Drupal.settings.hide_submit.hide_submit_css), form)
            .removeClass(Drupal.checkPlain(Drupal.settings.hide_submit.hide_submit_hide_css))
            .removeAttr('disabled');
          $('.hide-submit-text', form).remove();
        }
        else {
          $('input.' + Drupal.checkPlain(Drupal.settings.hide_submit.hide_submit_css) + ', button.' + Drupal.checkPlain(Drupal.settings.hide_submit.hide_submit_css), form)
            .stop()
            .removeClass(Drupal.checkPlain(Drupal.settings.hide_submit.hide_submit_hide_css))
            .show();
          $('.hide-submit-text', form).remove();
        }
      }
    }
  };

})(jQuery, Drupal, window, this.document);
