(function($) {

/**
 * @file
 * The transformation class
 *
 * @author Glenn De Backer <glenn at coworks dot be>
 *
 */

Drupal.behaviors.transformer_webform = {
  attach : function(context, settings) {

    var listWatchers = {};

    /*******************************************************************
     *
     * Transfomer class
     *
     ******************************************************************/
    function Transformer(elem) {
      this.elem = elem;

      this.inputChars = {};

      // process pattern
      this.processPattern();
    }

    /**
     * Process the pattern
     */
    Transformer.prototype.processPattern = function() {
      // retrieve pattern
      this.pattern = $(this.elem).attr("data-transformer");

      // split pattern
      this.patternChars = this.pattern.split('');

      // check if this pattern starts with non repleacable characters
      if (this.patternChars[0] != '#') {
        this.startWithUnrepleable = true;
      }
      else {
        this.startWithUnrepleable = false;
      }
    };

    /**
     * Gets non replaceable part at position
     * example : #)(# => getNonReplacePart(1) will return ')('
     */
    Transformer.prototype.getNonReplacePart = function(startPos) {
      var part = '';
      var partIndex = startPos;

      if (this.patternChars[partIndex] != '#') {
        while (this.patternChars[partIndex] != '#' && partIndex < this.pattern.length ) {
          part += this.patternChars[partIndex];
          partIndex++;
        }
      }

      return part;
    };

    /**
     * process keyUp
     */
    Transformer.prototype.processKeyUp = function(event) {
      // retrieve carret position (position starts at 1)
      caretPosition = this.getCursorPosition();

      // check if value is numeric value
      if (this.isNumeric(String.fromCharCode(this.lastKeyCode))) {

        // when the first key was entered
        if ($(this.elem).val().length == 1 && this.getCharacterAt(0) !=  this.patternChars[0]) {
          // check if pattern starts with non repleacable character
          if (this.startWithUnrepleable) {
            // insert non repleacable part to the beginning of the field
            var part = this.getNonReplacePart(0);
            $(this.elem).val(part+$(this.elem).val());
          }
        }

        // when a key is entered
        // check if the next pattern in the array is an character that can't be replaced !(#)
        if (!this.isSpecialKey(event.keyCode)) {
          if (caretPosition < this.patternChars.length) {

            // retrieve part at position
            var part = this.getNonReplacePart(caretPosition);

            if(this.patternChars[caretPosition] != '#' ) {
              // be sure that pattern wasn't inserted on previous occasions
              if(this.getCharacterAt(caretPosition) != this.patternChars[caretPosition]) {
                // insert following character
                $(this.elem).val(this.insertAt(part,caretPosition));
              }
            }
          }
        }
      }
    };

    /**
     * Process keypress
     */
    Transformer.prototype.processKeypress = function(event) {
      // retrieve carret position (position starts at 1)
      caretPosition = this.getCursorPosition();

      // store last keycode because in keypress it isn't possible
      // to know which key was pressed see http://stackoverflow.com/questions/3977642/how-to-know-if-keyup-is-a-character-key-jquery
      this.lastKeyCode = event.which;

      // prevent of input becomming longer then the defined pattern
      if ($(this.elem).val().length > --this.pattern.length) {
        // if input will become longer
        // and the key that was pressed isn't a special key (space,
        // backspace, ...)
        // supress the whole key event
        if (!this.isSpecialKey(event.keyCode)) {
          event.preventDefault();
        }
        // When the keypress is a 'special' character there doesn't
        // need to be any processing
      }
      else if (this.isSpecialKey(event.keyCode)) {
    	  return false;
      }
      else if (!this.isNumeric(String.fromCharCode(event.which))) {
        // when it is not a number
        // don't proceess key event further
        event.preventDefault();
      }
    };

    /**
     * Get character at position
     */
    Transformer.prototype.getCharacterAt = function(pos) {
      return $(this.elem).val().charAt(pos);
    };

    /**
     * Insert string at position
     */
    Transformer.prototype.insertAt = function(strInsert, pos) {
      return [$(this.elem).val().slice(0, pos), strInsert,$(this.elem).val().slice(pos)].join('');
    };

    /**
     * Remove string at position
     */
    Transformer.prototype.removeAt = function(pos) {
      return $(this.elem).val().charAt(pos - 1)
          + $(this.elem).val().substring(0, pos - 1)
          + $(this.elem).val().substring(pos);
    };

    /**
     * Checks if key is a 'special' key
     */
    Transformer.prototype.isSpecialKey = function(keycode) {
      switch (keycode) {
      case 8: // backspace
        return true;
      case 9: // tab
        return true;
      case 46: // delete
        return true;
      case 35: // end
        return true;
      case 36: // home
        return true;
      case 37: // right
        return true;
      case 39: // left
        return true;
      default:
        return false;
      }
    };

    /**
     * Returns if object is numeric
     */
    Transformer.prototype.isNumeric = function(n) {
      return !isNaN(parseFloat(n)) && isFinite(n);
    };

    /**
     * Gets current cursor position
     */
    Transformer.prototype.getCursorPosition = function(event) {
      var el = $(this.elem).get(0);
      var pos = 0;
      if ('selectionStart' in el) {
        pos = el.selectionStart;
      }
      else if ('selection' in document) {
        el.focus();
        var Sel = document.selection.createRange();
        var SelLength = document.selection.createRange().text.length;
        Sel.moveStart('character', -el.value.length);
        pos = Sel.text.length - SelLength;
      }
      return pos;
    };


    /*******************************************************************
     * General
     ******************************************************************/

    // look for all textfields that has a data-transform attribute
    var textfields = $("input[data-transformer]");

    // iterate through textfield
    for (var i=0, l=textfields.length; i<l; i++) {
      // add list watchers list
      listWatchers[$(textfields[i]).attr('id')] = new Transformer(textfields[i]);

      // bind on keypress to textfield
      $(textfields[i]).bind('keypress', function(evt) {
        listWatchers[$(this).attr('id')].processKeypress(evt);
      });

      // bind on keyup to textfield
      $(textfields[i]).bind('keyup', function(evt) {
        listWatchers[$(this).attr('id')].processKeyUp(evt);
      });
    }
  }
};

})(jQuery);