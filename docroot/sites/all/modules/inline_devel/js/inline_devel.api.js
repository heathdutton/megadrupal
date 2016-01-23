/**
 * @file
 * The inline devel plugin core area.
 *
 * @description
 * Inline devel suggester plugin designed to apply IDE functionality on any
 * text area on your dome.
 *
 * How to use it: on a basic HTML structure like this:
 *  <form>
 *    <input type='foo' />
 *    <textarea id='example_textarea'>bar</textarea>
 *  </form>
 *
 *  you can use the next jQuery code:
 *    $("#example_textarea").inline_devel();
 *
 *  Simple as that, enjoy.
 */

/**
 * An alias to the console log function.
 * @param word
 *  The word for debug. If you'll pass a function - her content will be
 *  displayed as well - a small FYI ;-)
 */
function log(word) {
  console.log(word);
}

/**
 * This part will be defined as the "Inline Devel plugin".
 */
(function ($) {

  // Special character which determine if the line need to break.
  $.specialChars = new Array(
    " ", '(', ')', ';', "\n", "\r"
  );

  // A variable which determine if the plugin need to work. This is good when
  // the user want to stop the detection of functions.
  $.applyInlineDevel = true;

  /**
   * Attach this method to any text area for apply the inline devel
   * functionality.
   */
  jQuery.fn.inline_devel = function () {

    // The address of menu item item which return the json of the functions.
    var address;

    // The object that will get the inline devel functionality.
    var textarea = $(this);

    // Determine if the JSON output return any function.
    var haveFunction = false;

    // Determine of the value of the inline devel element changed from the last
    // key press.
    var prevSearch;

    // Holds the delta that the user selected from the function suggester.
    var currentFunction;

    // Wrapping the text area so the suggester will be relative the text area
    // and not to the page.
    $(this).wrap("<div id='inline_devel_wrapper'></div>");

    // Attaching the suggester divs to the element.
    $(this).after("<div class='suggestion-wrapper'><div class='suggestion' id='suggestion'></div></div>");

    // The element that will contain the suggested function.
    var functionsName = $("#suggestion");

    // Load the Drupal settings.
    var settings = Drupal.settings.inline_devel;

    // 1. Keyup detect immediately keypress but not handle with preventDefault
    // 2. Functions that worked well under keydown not working as well under
    //    keyup event.
    $(this).keyup(function () {
      if ($.applyInlineDevel == false) {
        // The user ask to stop the detection.
        return;
      }

      // When browsing in function, don't continue to the next steps.
      if (prevSearch == textarea.val()) {
        return;
      }

      // Start checking from the server the available functions name.
      var keyword = $(this).getLastWord();
      address = '?q=devel/php/inline_devel/' + keyword;

      // Check if we need to search in classes.
      if (keyword.indexOf('->') != -1) {
        var variable = keyword.split('->')[0].replace(/^\s+|\s+$/g,"");
        $.each(jQuery.returnClasses($(this)), function(key, value) {
          if (value[0] == variable) {
            var class_name = value[1].replace(';',"").replace('()',"");

            address = '?q=devel/php/inline_devel/get_classes/' + keyword.split('->')[1].replace(/^\s+|\s+$/g,"") + '/' + class_name;
            return true;
          }
        });
      }

      $.getJSON(address, function(data) {
        var items = [];
        haveFunction = true;
        currentFunction = 0;

        // Show the functions name area and add class.
        functionsName.show().addClass('bordered');
        $(".suggestion-wrapper").addClass('border-up');

        $.each(data, function(key, val) {
          items.push(jQuery.itemPush(val));
        });

        // Change the location of the suggester.
        if (Drupal.settings.inline_devel.caret_position != '---') {
          textarea.floatingDetector($("DIV.suggestion-wrapper.border-up"));
        }

        // Insert the html.
        functionsName.html(items.join(''));

        // Let's save the last search the user did.
        prevSearch = textarea.val();
      });
    }).keydown(function(keyPressed) {
      if ($.applyInlineDevel == false) {
        // The user ask to stop the detection.
        return;
      }

      var selectedDiv = $("#suggestion .selected-function");
      var availableFunctionNumber = $("#suggestion").find("div.function").length;

      // ESC button for closing the suggestor at any time.
      if (settings.esc_enable == true && keyPressed.which == 27) {
        functionsName.closeSuggester();
      }

      // Ctrl + s handling
      if (settings.ctrl_s_enable == true && keyPressed.ctrlKey && keyPressed.which == 83) {
        $(this).parents('form').submit();
        event.preventDefault();
      }

      // When clicking on the tab we need to replace the spaces for tabs.
      if (settings.spaces_instead_of_tabs && keyPressed.which == 9) {
        keyPressed.preventDefault();

        var white_space = '';
        var data = textarea.elementData();
        var cursor = data.cursor;
        var last_word = textarea.getLastWord();
        var start = cursor - last_word.length;
        var end = cursor;

        // Building the whitespaces.
        for (var i = 0; i < settings.number_of_spaces; i++) {
          white_space += ' ';
        }

        textarea.val(data.value.slice(0, start) + last_word + white_space + data.value.slice(end));

        // Change the position of the cursor.
        textarea.setCursorPosition(end + settings.number_of_spaces, end + parseInt(settings.number_of_spaces));
      }

      if ($(this).breakOnReserved() == 0 || jQuery.inArray(keyPressed.which, new Array(8, 9, 48, 57, 219, 221)) > -1) {
        $("#suggestion").closeSuggester();
        return;
      }

      // The functions is revealed to the user. When scrolling down with the
      // keyboard need to keep the the courser in the same place for replacing
      // words properly.
      if ((keyPressed.which == 38 || keyPressed.which == 40 || keyPressed.which == 9) && availableFunctionNumber > 0) {
        keyPressed.preventDefault();
        $("#suggestion").getPositionOverflow(keyPressed.which);
      }

        if ((keyPressed.which >= 38 || keyPressed.which <= 40)) {
          currentFunction = $("#suggestion").find(".selected-function").index();
          if (keyPressed.which == 38) {
            $("#suggestion .function").removeClass('selected-function');
            $("#suggestion").find(".function:nth-child(" + (currentFunction) + ")").addClass('selected-function');

            // Boundaries of the scope.
            if (currentFunction <= 2) {
              currentFunction = 0;
            }
            else {
              currentFunction--;
            }
          }
          else {
            $("#suggestion").find(".function").removeClass('selected-function');
            $("#suggestion").find(".function:nth-child(" + (currentFunction + 2) + ")").addClass('selected-function');

            // Boundaries of the scope.
            if (currentFunction > availableFunctionNumber) {
              currentFunction = 0;
            }
            else {
              currentFunction++;
            }
          }
        }

        // Check if we have only one function - if so, when clicking enter the
        // function will throw to the function.
        var divElement;
        if (availableFunctionNumber == 1) {
          divElement = $("#suggestion").find("div");
        }
        else {
          divElement = selectedDiv;
        }

        if ((keyPressed.which == 13 || keyPressed.which == 32 && keyPressed.ctrlKey) && divElement.html()) {
          // Insert data properly.
          var arguments = false;

          if (keyPressed.which == 32 && keyPressed.ctrlKey) {
            arguments = true;
          }

          var last_word = $(this).getLastWord();

          $(this).insertElementProperly(last_word, divElement, arguments);

          // Don't break row.
          keyPressed.preventDefault();
          $("#suggestion").find(".function").removeClass('selected-function');

          functionsName.html('');
          functionsName.hide();
          return;
        }

        // Hide the functions name area because there is no text.
        if (textarea.val().length == 0) {
          functionsName.removeClass('bordered');
          functionsName.hide();
        }
    });

    // Check for every second if the we have any function in the suggester.
    setInterval(function() {
      if ($("#suggestion div.function").length == 0) {
        $("#suggestion").hide();
        $(".suggestion-wrapper").removeClass('border-up');
      }
    }, 1);

    // Handle when the user click with the mouse on the function from the
    // suggester.
    $("#suggestion .function").live("click", function() {
      // Insert data properly.
      var last_word = textarea.getLastWord();
      textarea.insertElementProperly(last_word, $(this), false);

      // Don't break row.
      $("#suggestion .function").removeClass('selected-function');

      // Close the suggester.
      functionsName.closeSuggester();
    });

    // Color the function that the user hover above.
    $("#suggestion .function").live("hover", function() {
      $("#suggestion .function").removeClass('selected-function');
      $(this).addClass('selected-function');
    });
  };

  /**
   * Get the word that the cursor is closer to.
   */
  jQuery.fn.getLastWord = function() {
    var data = $(this).elementData();

    var cursor = data.cursor;
    var value = $(this).getCurrentLine();
    var key_start, key_end;

    for (var i = cursor; i >= 0; i--) {
      if (jQuery.IsSpecialChars(value.charAt(i))) {
        key_start = i;
        break;
      }
    }

    if (key_start == undefined) {
      key_start = 0;
    }

    for (var i = cursor; i <= value.length; i++) {
      if (jQuery.IsSpecialChars(value.charAt(i))) {
        key_end = i;
        break;
      }
    }

    if (key_end == undefined) {
      key_end = value.length;
    }

    // In case we got on of the special chars in the word - remove it.
    var word = value.substr(key_start, key_end);

    for (i = 0; i < $.specialChars.length; i++) {
      word = word.replace($.specialChars[i], '');
    }

    return word;
  };

  /**
   * Get current the line.
   */
  jQuery.fn.getCurrentLine = function() {
    var data = $(this).elementData();
    var cursor = data.cursor;
    var key_start, key_end;

    for (var i = cursor; i >= 0; i--) {
      if (data.value.charCodeAt(i) == NaN) {
        key_start = i;
        break;
      }
    }

    if (key_start == undefined) {
      key_start = 0;
    }

    for (var i = cursor; i <= data.value.length; i++) {
      if (jQuery.inArray(data.value.charCodeAt(i), new Array(NaN, "", "\n"))) {
        key_end = i;
        break;
      }
    }

    if (key_end == undefined) {
      key_end = data.value.length;
    }

    return data.value.substr(key_start, key_end);
  };

  /**
   * Insert element properly.
   */
  jQuery.fn.insertElementProperly = function(last_word, element_source, arguments) {
    var data, start, end, hard_location, chr, bck, word, variable, startPosition, endPosition;

    data = $(this).elementData();

    start = data.cursor - last_word.length;
    end = data.cursor;

    // The location of caret after inserting the word.
    hard_location = 0;

    if (jQuery.inArray(element_source.attr('type'), new Array('interface', 'variable')) != -1) {
      chr = '';
      bck = 0;
      word = element_source.attr('name');
    }
    else if (element_source.attr('type') == 'class_function' ) {
      variable = last_word.split('->')[0];
      word = variable + '->' + element_source.attr('name');
      chr = arguments ? '(' + element_source.attr('arguments') + ');' : '();';

      hard_location = start + word.length;
    }
    else {
      word = element_source.attr('name');
      chr = arguments ? '(' + element_source.attr('arguments') + ')' : '()';
      bck = 1;

      if (chr == '(null)') {
        chr = '()';
      }
    }

    $(this).val(data.value.slice(0, start) + word + chr + data.value.slice(end));

    // Change the position of the cursor.
    startPosition = endPosition = hard_location == 0 ? start + element_source.attr('name').length + 2 - bck : hard_location;
    $(this).setCursorPosition(startPosition, endPosition);
  };

  /**
   * Set the cursor position.
   * @param selectionStart
   *  The start position of the cursor.
   * @param selectionEnd
   *  The end position of the cursor.
   * @returns {*}
   */
  $.fn.setCursorPosition = function(selectionStart, selectionEnd) {
    if (this.length == 0) {
      return this;
    }
    var input = this[0];

    if (input.createTextRange) {
      var range = input.createTextRange();
      range.collapse(true);
      range.moveEnd('character', selectionEnd);
      range.moveStart('character', selectionStart);
      range.select();
    } else if (input.setSelectionRange) {
      input.focus();
      input.setSelectionRange(selectionStart, selectionEnd);
    }

    return this;
  };

  /**
   * Close the suggester.
   */
  jQuery.fn.closeSuggester = function () {
    $(this).html('');
    $(this).hide();
  };

  /**
   * Get information about the text area.
   */
  jQuery.fn.elementData = function() {
    var data;
    data = {
      cursor: $(this).getCursorPosition(),
      value: $(this).val()
    };

    return data;
  };

  /**
   * Returning the current position by the caret.
   */
  jQuery.fn.getCursorPosition = function() {
    var el = $(this).get(0);
    var pos = 0;
    if('selectionStart' in el) {
      pos = el.selectionStart;
    }
    else if('selection' in document) {
      el.focus();
      var Sel = document.selection.createRange();
      var SelLength = document.selection.createRange().text.length;
      Sel.moveStart('character', -el.value.length);
      pos = Sel.text.length - SelLength;
    }

    return pos;
  };

  /**
   * Check for special chars that can split sentence for words.
   */
  jQuery.IsSpecialChars = function(charecter) {
    return jQuery.inArray(charecter, $.specialChars) >= 0;
  };

  /**
   * Return an item push that will be used when displaying the suggester.
   */
  jQuery.itemPush = function(val) {
    return "<div class='function' arguments='" + val.arguments + "' name='" + val.name + "' id='" + val.id + "' type='" + val.type + "'>" + val.name + " (" + val.type + ")</div>";
  };

  /**
   * Don't auto complete when there are reserved words.
   */
  jQuery.fn.breakOnReserved = function() {
    var line = $(this).getCurrentLine();

    // Reserved words - after them auto complete will no be available.
    var reserved = new Array(
      'abstract', 'and', 'as', 'break', 'case', 'catch', 'clone',
      'const', 'continue', 'declare', 'default', 'do', 'else', 'enddeclare',
      'endfor', 'endforeach', 'endif', 'endswitch', 'endwhile', 'final',
      'global', 'goto', 'implements', 'include', 'include_once',
      'instanceof', 'insteadof', 'interface', 'namespace', 'new', 'or',
      'private', 'protected', 'public', 'require', 'require_once', 'static',
      'throw', 'trait', 'try', 'unset', 'use', 'xor'
    );

    var smart_split = line.split(/\s/g);

    $.each(smart_split, function(key, value) {
      if(jQuery.inArray(value, reserved) > -1) {
        return 0;
      }
    });

    for (var i = 0; i < reserved.length; i++) {
      if (line.indexOf(reserved[i], 0) == 0) {
        return 0;
      }
    }
  };

  /**
   * Get the position of a selected function inside the overflow div.
   */
  jQuery.fn.getPositionOverflow = function(keyNumber) {
    var functionHeight = $(this).attr("scrollHeight");
    var elementsNumber = $(this).find("div.function").length;
    var currentLocation = $(this).find(".selected-function").index();
    var ratio = functionHeight/elementsNumber;

    // Reset.
    if (currentLocation <= 0) {
      currentLocation = 1;
    }

    // How much padding we need for the scroller when scrolling up and down.
    if (keyNumber == 38) {
      var scrollerMarginTop = 150;
    }
    else {
      var scrollerMarginTop = 75;
    }

    // Set the scroller location near to the selected function.
    $("#suggestion").scrollTop((ratio * currentLocation) - scrollerMarginTop);
  };

  /**
   * Return list of classes.
   *
   * @param textArea
   *  A wrapped textarea.
   * @returns {Array}
   *  Array of classes in the available scope.
   */
  jQuery.returnClasses = function(textArea) {
    // Breaking the rows.
    var rows = textArea.val().split("\n");
    var classes = [];

    $.each(rows, function(key, value) {
      if (value.indexOf('new') != -1) {
        var split = value.split(/\s/g);
        classes[key] = new Array(split[0], split[3].replace('()', ''));
      }
    });

    return classes;
  };

  /**
   * Set on an off the inline devel plugin functionality.
   * @param status
   *  Set as true or false for turning off and on the plugin.
   */
  jQuery.detectionStatus = function(status) {
    $.applyInlineDevel = status;

    if (!status) {
      // The detection has stopped by the user request. We need to close the
      // suggester as well.
      $("#suggestion").closeSuggester();
    }
  };

  /**
   * Calculating the floating position of the x and y for the detector.
   */
  jQuery.fn.floatingDetector = function(functions) {
    // Get the x position of the textarea.
    var x = Math.abs($(this).position().left);

    // Get the y position of the textarea.
    var y = Math.abs($(this).position().top);

    // Set the floating position of the suggester.
    var pos = $(this).getCaretPosition();

    // Different browser needed different additional height.
    var additional_height = 0;
    if ($.browser.mozilla) {
      additional_height = 7;
    }
    else {
      additional_height = 15;
    }

    functions
      .css({
        position: "absolute",
        left: x + pos.left,
        top: y + pos.top + additional_height
      });
  };

})(jQuery);
