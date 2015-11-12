(function ($) {

  // Defined keyCodes we need.
  var KEY_UP = 38;
  var KEY_DOWN = 40;
  var KEY_ENTER = 13;
  var KEY_TAB = 9;

  Drupal.qui = {
    ctx: '', // Current context to be working in.
    settings: {} // Current context settings.
  };

  /**
   * Converts a set of elements into a text field with autocomplete-like functionality.
   */
  Drupal.qui.convertToAutocompleteWidget = function ($elements) {
    $elements.each(function (i, select) {
      // Make sure we don't double attach autocomplete inputs.
      if ($(select).data('autocomplete-init')) {
        return;
      }
      var $input = $('<input type="text" placeholder="' + Drupal.t("Start typing...") + '" size="40" class="form-text qui-autocomplete" autocomplete="off" />');
      $input.attr('id', 'input-' + $(select).attr('id'));

      // Create a results div.
      var $results = $('<div class="qui-autocomplete-results"><ul class="qui-autocomplete-results-list"></ul></div>');
      var $wrapped = $('<div class="form-item form-type-textfield"></div>');
      $wrapped.append($input);
      $wrapped.append($results);

      // Copy the select label for the input.
      var $label = $('<label/>');
      $label.attr('for', 'input-' + $(select).attr('id'));
      $label.html($('label[for="' + $(select).attr('id') + '"]').html());
      $input.before($label);

      // Set the default text if there is some.
      $input.val($(select).find('option[selected]').val());
      if ($(select).hasClass('error')) {
        $input.addClass('error');
      }

      $(select).closest('.form-item').before($wrapped);
      $(select).closest('.form-item').hide();
      $(select).attr('data-autocomplete-init', 'true');
      // Attach events to the text fields.
      Drupal.qui.attachEventsTextfieldAutocomplete($input, $(select));
    });
  };

  /**
   * Attach keydown events for displaying autocomplete results.
   */
  Drupal.qui.attachEventsTextfieldAutocomplete = function ($input, $select) {
    var $output = $input.siblings('.qui-autocomplete-results');
    $input.keydown(function (e) {
      if (e.keyCode == KEY_UP || e.keyCode == KEY_DOWN) {
        // Pressing up/down arrows.
        Drupal.qui.navigateResults($(this), e.keyCode);

      } else if (e.keyCode == KEY_ENTER) {
        e.preventDefault();
        if ($output.find('li.active').length) {
          $output.find('li.active').trigger('click');
          $input.blur();
        } else {
          Drupal.qui.chooseSelectOption($select, $input.val());
        }
      } else if (e.keyCode == KEY_TAB) {
        // No prevent default.
        if ($output.find('li.active').length) {
          $output.find('li.active').trigger('click');
        } else {
          Drupal.qui.chooseSelectOption($select, $input.val());
        }
      }
    });
    $input.keyup(function (e) {
      if ($input.val() == "") {
        // When the input is empty, the select value should be too.
        $select.val("");
      }
      // If the input val is equal to something in the select list, select that.
      if ($select.find('option[value="' + $input.val() + '"]').length) {
        $select.val($input.val());
      }
      if ($select.find('option[value="' + $input.val() + '"]').length === 0) {
        $input.addClass('input-error');
      } else {
        $input.removeClass('input-error');
      }
      if (e.keyCode == KEY_UP || e.keyCode == KEY_DOWN || e.keyCode == KEY_ENTER || e.keyCode == KEY_TAB) {
        return true;
      }
      Drupal.qui.updateAutocompleteResults($input, $output, $select);
    });
    $input.focus(function (e) {
      Drupal.qui.updateAutocompleteResults($input, $output, $select);
      $(this).siblings('.qui-autocomplete-results').slideDown(250);
    });
    $input.blur(function (e) {
      setTimeout(function () {
        $input.siblings('.qui-autocomplete-results').slideUp(250);
      }, 100); // Need this timeout to not break click events.
    });

    $input.siblings('.qui-autocomplete-results').find('ul').click(function (e) {
      e.stopPropagation();
      var $item = $(e.target);
      if (!$item.data('value')) {
        return;
      }
      $input.val($item.data('value'));
      Drupal.qui.chooseSelectOption($select, $item.data('value'));
    });

  };

  /**
   * Navigate up/down through the results list.
   */
  Drupal.qui.navigateResults = function ($input, keyCode) {
    var $output = $input.siblings('.qui-autocomplete-results');
    var selected = $output.data('selected');
    if (!selected) {
      selected = 0;
    }
    var listLen = $output.find('li').length;
    switch (keyCode) {
      case KEY_UP:
        if (selected == 1) {
          // Can't go any further up.
          break;
        }
        selected--;
        break;
      case KEY_DOWN:
        if (selected >= listLen) {
          // Can't go any further down.
          break;
        }
        selected++;
        break;
    }
    $output.find('li').removeClass('active');
    $output.data('selected', selected);

    var $selected_element = $output.find('li').eq(selected - 1);
    $selected_element.addClass('active');

    // Put the value into the box in real time.
    $input.val($selected_element.data('value'));

    if (keyCode == KEY_DOWN && (selected % 8 == 0 || selected % 8 == 1)) {
      // Scroll new listing into view.
      $output.stop().animate({
        scrollTop: $selected_element.position().top
      }, 200);
    }
    else if (keyCode == KEY_UP && selected !== 1 && (selected % 8 == 0 || selected % 8 == 1)) {
      $output.stop().animate({
        scrollTop: $output.find('li').eq($output.data('selected') - 8).position().top
      }, 200);
    }
  }

  /**
   * Updates autocomplete results based on an input's value.
   */
  Drupal.qui.updateAutocompleteResults = function ($input, $output, $select) {
    var search_string = $input.val();
    // Clear out existing results.
    $output.find('ul > li').remove();
    $output.data('selected', 0);
    var results = Drupal.qui.getSearchResults(search_string, $select);
    for (var i = 0; i < results.length; i++) {
      var $item = $('<li></li>');
      $item.text(results[i].text);
      $item.attr('data-value', results[i].value);
      // Add item to the results list.
      $output.find('ul').append($item);
    }
  };

  /**
   * Change the select value.
   */
  Drupal.qui.chooseSelectOption = function ($select, value) {
    $select.val(value).trigger('change');
  }

  /**
   * Gets matching options from the select list.
   */
  Drupal.qui.getSearchResults = function (search_string, $select) {
    var results = [];
    var lower_search_string = search_string.toLowerCase();
    // Search through all select options for matching text.
    $select.find('option').each(function (i, option) {
      var compare = $(option).val();
      if (compare !== "" && compare.toLowerCase().indexOf(lower_search_string) !== -1) {
        // Value is in the option.
        results.push({
          'value': compare,
          'text': $(option).text()
        });
      }
    });

    return results;
  };

  /**
   * Handle toggling query previews on/off
   */
  Drupal.qui.toggleQueryPreview = function () {
    var preview = $(".qui-query-preview", Drupal.qui.ctx);
    var toggle = $("#qui-query-preview-toggle", Drupal.qui.ctx);
    if (Drupal.qui.settings.qui.show_query) {
      toggle.text(Drupal.t('Hide Query'));
      preview.css("display", "block");
    }
    else {
      preview.css("display", "none");
    }
    // If there's an error override everything and show the "Query Attempted"
    if ($('.qui-error', Drupal.qui.ctx).length) {
      preview.css("display", "block");
    }
    // Toggle and adjust labels as needed
    toggle.unbind('click').click(function (e) {
      e.preventDefault();
      var label = Drupal.t(toggle.text());
      preview.slideToggle();
      if (label == Drupal.t('Hide Query')) {
        toggle.text(Drupal.t('Show Query'));
      }
      if (label == Drupal.t('Show Query')) {
        toggle.text(Drupal.t('Hide Query'));
      }
    });
  };

  /**
   * Displays the report description when clicking on the link.
   */
  Drupal.qui.viewDescription = function ($el) {
    var text = $el.data('description');
    // Format the escaped HTML so it displays properly.
    var formatted = $('<span>' + text + '</span>').text();
    alert(formatted);
  };

  /**
   * React on Drupal.attachBehaviors().
   */
  Drupal.behaviors.qui = {
    attach: function (ctx, settings) {
      Drupal.qui.ctx = ctx;
      Drupal.qui.settings = settings;

      // Make fancy autocomplete select lists happen.
      if ($('select.qui-autocomplete', ctx).length && settings.qui.use_autocomplete) {
        Drupal.qui.convertToAutocompleteWidget($('select.qui-autocomplete', ctx));
      }

      // Show/Hide the query string.
      if ($(".qui-query-preview", ctx).length) {
        Drupal.qui.toggleQueryPreview();
      }

      // Attach events to view descriptions.
      if ($('#qui-report-list-form', ctx).length) {
        $('#qui-report-list-form table td a[data-description]', ctx).click(function (e) {
          e.preventDefault();
          Drupal.qui.viewDescription($(this));
        });
      }
    }
  };

}(jQuery));
