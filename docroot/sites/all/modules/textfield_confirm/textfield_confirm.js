/**
 * @file
 * Defines Javascript behaviors for the textfield_confirm module.
 */

(function ($, Drupal) {

  "use strict";

  /**
   * Attaches the behavior to each input element.
   */
  Drupal.behaviors.textfieldConfirm = {
    attach: function (context, settings) {
      $(context).find('input.textfield-confirm-field').once('textfield-confirm', function () {
        Drupal.textfieldConfirm.processInput(this, settings.textfield_confirm);
      });
    }
  };

  Drupal.textfieldConfirm = Drupal.textfieldConfirm || {};

  /**
   * Adds checking behavior to textfield input elements.
   */
  Drupal.textfieldConfirm.processInput = function (element, settings) {
    var $primary = $(element);

    var $secondary = $primary.parent().parent().find('input.textfield-confirm-confirm');
    var $helpText = $('<span class="textfield-confirm-help-text"></span>');
    $secondary.after($helpText);

    // Checks that the text fields are equal.
    var textfieldConfirmCheck = function () {
      // Remove timers for a delayed check if they exist.
      if (this.timer) {
        clearTimeout(this.timer);
      }

      // Verify that there is a value to check.
      if (!$primary.val()) {
        $helpText.hide();
        return;
      }

      var message = $primary.val() === $secondary.val() ? settings.success : settings.error;

      if ($helpText.html() != message) {
        $helpText.html(message);
      }

      $helpText.show();
    };

    var textfieldConfirmDelayedCheck = function () {
      // Remove any existing timers.
      if (this.timer) {
        clearTimeout(this.timer);
      }

      // When the user clears the field, hide the tips immediately.
      if (!$primary.val()) {
        $helpText.hide();
        return;
      }

      // Schedule the actual check.
      this.timer = setTimeout(textfieldConfirmCheck, 100);
    };

    // Monitor keyup and blur events. Blur must be used because a mouse paste
    // does not trigger keyup.
    $primary.keyup(textfieldConfirmDelayedCheck).blur(textfieldConfirmCheck);
    $secondary.keyup(textfieldConfirmDelayedCheck).blur(textfieldConfirmCheck);
  };

})(jQuery, Drupal);
