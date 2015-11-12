(function ($) {
  /**
   * CJK regex patterns:
   * JP - Hiragana               \u3040-\u309F
   * JP - Katakana               \u30A0-\u30FF
   * KO - Hangul alphabet        \uAC00-\uD7AF\u1100-\u11FF\u3130-\u318F\u3200-\u32FF\uA960-\uA97F\uD7B0-\uD7FF\uFF00-\uFFEF
   * CJK Unified Ideographs      \u4E00-\u9FFF
   */

  var cjkFlag = false;
  var cjkRegex = /[\u3040-\u309F\u30A0-\u30FF\uAC00-\uD7AF\u1100-\u11FF\u3130-\u318F\u3200-\u32FF\uA960-\uA97F\uD7B0-\uD7FF\uFF00-\uFFEF\u4E00-\u9FFF]+?/g;

  /**
   * Overwrites Drupal's default in misc/autocomplete.js to submit when
   * selected. This applies to clicking only.
   */
  Drupal.jsAC.prototype.select = function (node) {
    this.input.value = $(node).data('autocompleteValue');
    if($(this.input).hasClass('auto_submit')){
      this.input.form.submit();
    }
  };

  /**
   * Overwrites Drupal's default in misc/autocomplete.js to submit when a user
   * hits the enter key. Otherwise, it behaves normally.
   */
  Drupal.jsAC.prototype.onkeyup = function (input, e) {
    if (!e) {
      e = window.event;
    }

    /**
     * Check if the last two characters contain a CJK symbol, using 2 chars
     * to account for syllable input.
     */
    var lcharFlag = input.value.charAt(input.value.length-2).match(cjkRegex);

    cjkFlag = (cjkRegex.test(input.value)) ? true : false;

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

      case 13: // Enter.
        if (cjkFlag && input.value.length > 0) {
          this.populatePopup();
          return false;
        }
        else {
          this.hidePopup(e.keyCode);
        }

        this.hidePopup(e.keyCode);
        this.input.form.submit();
        return true;

      case 9:  // Tab.
      case 27: // Esc.
        this.hidePopup(e.keyCode);
        return true;

      default: // All other keys.
        if (!cjkFlag && input.value.length > 0 && lcharFlag === null){
          // Immediate populate with non-CJK characters.
          this.populatePopup();
        }
        else {
          this.hidePopup(e.keyCode);
        }
        return true;
    }
  };

  /**
   * Overwrites default AJAX autocomplete error handling to fail silently rather
   * than alert an error. This code block can be removed once the following core
   * issue is resolved.
   *
   * @see http://drupal.org/node/1232416
   */
  $(document).ready(function() {
    $.ajaxSetup({
      beforeSend: function(jqXHR, settings) {
        settings.error = function(jqXHR, textStatus, errorThrown) {
          // If no console exists (IE), fake one.
          if (typeof console === 'undefined') {
            console = {};
            'log info warn error dir clear'.replace(/\w+/g,function(f) {
              if (!(f in console)) console[f] = console.log||new Function;
            });
          }
          console.log('ajax error: ' + textStatus);
        };
      }
    });
  });
}) (jQuery);
