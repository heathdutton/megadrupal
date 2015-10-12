(function ($) {

  // Overrides autocomplete.js.
  if (Drupal.jsAC) {

    /**
     * Hides feedback notification.
     *
     * @see Drupal.jsAC.prototype.hidePopup
     * @see Drupal.jsAC.prototype.found
     */
    Drupal.jsAC.prototype.hideFeedback = function (keycode) {
      // Hide feedback.
      var feedback = this.feedback;
      if (feedback) {
        this.feedback = null;
        $(feedback).fadeOut('fast', function () { $(feedback).remove(); });
      }
      this.selected = false;
      $(this.ariaLive).empty();
    };


    /**
     * Populate feedback notification.
     *
     * @see Drupal.jsAC.prototype.populatePopup
     * @see Drupal.jsAC.prototype.found
     */
    Drupal.jsAC.prototype.populateFeedback = function (txt) {

      var $input = $(this.input);
      var position = $input.position();
      // Show feedback.
      if (this.feedback) {
        $(this.feedback).remove();
      }
      this.feedback = $('<div id="autocomplete-feedback"></div>');
      this.feedback.owner = this;
      $(this.feedback).text(txt);
      $(this.feedback).css({
         width: $input.innerWidth() + 'px'
      });
      $input.after(this.feedback);

    };

    // Overrides autocomplete.js
    Drupal.jsAC.prototype.setStatus = function (status) {
      switch (status) {
        case 'begin':
          this.hideFeedback();
          $(this.input).addClass('throbbing');
          $(this.ariaLive).html(Drupal.t('Searching for matches...'));
          break;
        case 'cancel':
        case 'error':
        case 'found':
          $(this.input).removeClass('throbbing');
          break;
      }
    };

    // Overrides autocomplete.js.
    Drupal.jsAC.prototype.found = function (matches) {
      // If no value in the textfield, do not show the popup.
      if (!this.input.value.length) {
        return false;
      }

      // Prepare matches.
      var ul = $('<ul></ul>');
      var ac = this;
      for (key in matches) {
        $('<li></li>')
          .html($('<div></div>').html(matches[key]))
          .mousedown(function () { ac.select(this); })
          .mouseover(function () { ac.highlight(this); })
          .mouseout(function () { ac.unhighlight(this); })
          .data('autocompleteValue', key)
          .appendTo(ul);
      }

      // Show popup with matches, if any.
      if (this.popup) {
        if (ul.children().length) {
          $(this.popup).empty().append(ul).show();
          $(this.ariaLive).html(Drupal.t('Autocomplete popup'));
        }
        else {
          $(this.popup).css({ visibility: 'hidden' });
          this.hidePopup();
        }
      }

      // Show feedback if no matches.
      if (matches.length < 1) {
        this.populateFeedback('Sorry, no results were found.');
      }

    };

    // Overriding autocomplete.js.
    Drupal.jsAC.prototype.onkeyup = function (input, e) {
      if (!e) {
        e = window.event;
      }
      switch (e.keyCode) {
        case 16: // Shift.
        case 17: // Ctrl.
        case 18: // Alt.
        case 20: // Caps lock.
        case 33: // Page up.
        case 34: // Page down.
        case 35: // End.
        case 36: // Home.
        case 37: // Left arrow.
        case 38: // Up arrow.
        case 39: // Right arrow.
        case 40: // Down arrow.
          return true;

        case 9:  // Tab.
        case 13: // Enter.
        case 27: // Esc.
          this.hidePopup(e.keyCode);
          return true;

        default: // All other keys.
          if (input.value.length > 0 && !input.readOnly) {
            this.populatePopup();
          }
          else {
            this.hidePopup(e.keyCode);
            this.hideFeedback();
          }
          return true;
      }
    };


  } // ends if (Drupal.jsAC)

})(jQuery);
