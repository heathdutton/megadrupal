/**
 * @file
 * Fast Autocomplete jQuery plugin.
 */

(function ($) {
  var facJsonRequest = null;

  /**
   * Apply the fastAutocomplete plugin to given input:text elements.
   */
  $.fn.fastAutocomplete = function(options) {
    // Default settings.
    var settings = $.extend({
      jsonFilesPath: undefined,
      keyMinLength: 1,
      keyMaxLength: 5,
      breakpoint: 0,
      enabled: false,
      resize_timer: null,
      emptyResult: '',
      highlightEnabled: false
    }, options );

    // Check if a jsonFilesPath is set before doing anything.
    if (settings.jsonFilesPath !== undefined) {
      var facResult = undefined;

      toggleResponsiveBehavior(facResult, settings);
      $(window).resize(function(e) {
        clearTimeout(settings.resize_timer);
        settings.resize_timer = setTimeout(toggleResponsiveBehavior(facResult, settings), 250);
      });

      // Only apply the plugin on input fields of type text.
      this.filter('input:text').each(function() {
        $(this).attr('autocomplete', 'OFF')
        $(this).attr('aria-autocomplete', 'list');

        // Create the hidden result div that will contain the suggestions.
        facResult = $('<div class="fac-result"><ul class="result-list"></ul></div>').hide();
        var emptyResults = $('<div class="empty-result"></div>');
        emptyResults.html(settings.emptyResult);
        emptyResults.find('a').each(function() {
          $(this).mousedown(function (e) {
            switch (e.which) {
              case 1: // Left mouse click.
                window.location = $(this).attr('href');
                e.preventDefault();
                break;
            }
          });
        });
        facResult.append(emptyResults);
        $(this).closest('form').css('position', 'relative').append(facResult);

        // When a character is entered perform the necessary ajax call. Don't
        // respond to any special keys.
        $(this).keyup(function(e) {
          if (settings.enabled) {
            if (!e) {
              e = window.event;
            }
            switch (e.keyCode) {
              case 9:  // Tab.
              case 16: // Shift.
              case 17: // Ctrl.
              case 18: // Alt.
              case 20: // Caps lock.
              case 27: // Esc.
              case 33: // Page up.
              case 34: // Page down.
              case 35: // End.
              case 36: // Home.
              case 37: // Left arrow.
              case 38: // Up arrow.
              case 39: // Right arrow.
              case 40: // Down arrow.
              case 13: // Enter.
              case 27: // Esc.
                return true;

              default: // All other keys.
                populateResults(this, facResult, settings);
                return true;
            }
          }
        });

        // Handle special keys (Enter, up, down).
        $(this).keydown(function(e) {
          if (settings.enabled) {
            if (!e) {
              e = window.event;
            }
            switch (e.keyCode) {
              case 40: // Down arrow.
                selectDown(facResult);
                return false;

              case 38: // Up arrow.
                selectUp(facResult);
                return false;

              case 13: // Enter.
                var selected = facResult.find('li.selected');
                if (selected.hasClass('see-all-link')) {
                  facResult.closest('form').submit();
                }
                else {
                  if (selected.length) {
                    window.location = selected.find('a').attr('href');
                    return false;
                  }
                  else {
                    return true;
                  }
                }

              case 27: // Esc.
                if(facJsonRequest != null) {
                  facJsonRequest.abort();
                }
                facResult.hide();
                return false;

              default: // All other keys.
                return true;
            }
          }
        });

        // Hide the result div when the input element loses focus.
        $(this).blur(function(e) {
          if (settings.enabled) {
            if(facJsonRequest != null) {
              facJsonRequest.abort();
            }
            facResult.hide();
          }
        });

        // When the input element gains focus, show the result.
        $(this).focus(function(e) {
          if (settings.enabled) {
            facResult.show();
          }
        });
      });
    }

    // Return the original object to make the plugin chainable.
    return this;
  }

  // Enable or disable the Fast Autocomplete behavior based on a breakpoint.
  function toggleResponsiveBehavior(facResult, settings) {
    var browserWidth = window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth;
    if (browserWidth >= settings.breakpoint) {
      settings.enabled = true;
    }
    else {
      settings.enabled = false;
      if (facResult !== undefined) {
        facResult.hide();
      }
    }
  }

  // Populates the results div with suggestions from an AJAX response.
  function populateResults(element, facResult, settings) {
    var value = $(element).val();
    // Make sure the value is lowercase for case-insensitive search.
    value.toLowerCase();

    var emptyResult = facResult.find('> div.empty-result');

    // Only perform the ajax call if the key is a certain length and not
    // empty. Append a timestamp to the url to prevent the browser caching
    // the json response.
    if (value.length >= settings.keyMinLength && value.length <= settings.keyMaxLength) {
      $(element).addClass('throbbing');
      facJsonRequest = $.ajax({
        url: '/' + settings.jsonFilesPath + value + '.json?nocache=' + (new Date()).getTime(),
        dataType: 'json',
        type: 'GET',
        processData: false,
        beforeSend : function() {
          if(facJsonRequest != null) {
            facJsonRequest.abort();
          }
        },
        success: function(data, status, xhr) {
          var items = data.items.reverse();
          if (data.items.length) {
            // First we add the "see all" link to the result. Later on we prepend all the
            // results if there are any.
            facResult.find('> ul.result-list > li.see-all-link').remove();
            seeAllLink = $('<li class="see-all-link"><div><a href="#">' + Drupal.t('See all results for "%key"', {'%key' : $(element).val()}) + '</a></div></li>');
            seeAllLink.css('cursor', 'pointer').mousedown(function(e) {
              facResult.closest('form').submit();
              e.preventDefault();
            }).hover(function(e) {
              facResult.find('> ul.result-list > li.selected').removeClass('selected');
              $(this).addClass('selected');
            });
            facResult.find('> ul.result-list').append(seeAllLink);

            facResult.find('> ul.result-list > li.result').remove();
            emptyResult.hide();
            $.each(items, function (key, dataValue) {
              var item = $('<li class="result">' + dataValue + '</li>');
              item.css('cursor', 'pointer').mousedown(function(e) {
                switch (e.which) {
                  case 1: // Left mouse click.
                    window.location = item.find('a').attr('href');
                    e.preventDefault();
                    break;
                }
              }).hover(function(e) {
                facResult.find('> ul.result-list > li.selected').removeClass('selected');
                $(this).addClass('selected');
              });
              facResult.find('> ul.result-list').prepend(item);
            });
            if (settings.highlightEnabled) {
              facResult.find('> ul.result-list > li.result').highlight(value);
            }
            facResult.show();
          }
        },
        complete: function() {
          facResult.find('> ul.result-list > li.result').unhighlight();
          facResult.find('> ul.result-list > li.result').highlight(value);
        }
      });
      $(element).removeClass('throbbing');
    }
    else {
      if (settings.highlightEnabled) {
        facResult.find('> ul.result-list > li.result').unhighlight();
        facResult.find('> ul.result-list > li.result').highlight(value);
      }
      // If the key is empty, clear the result div and show the empty result content.
      if (value.length < 1) {
        facResult.find('> ul.result-list > li').remove();
        emptyResult.show();
      }
    }
  }

  // Select the next suggestion.
  function selectDown(facResult) {
    var selector = '> div.empty-result ul';
    if (facResult.find('> ul.result-list > li').length) {
      var selector = '> ul.result-list';
    }
    var selected = facResult.find(selector + ' > li.selected');
    if (selected.length) {
      selected.removeClass('selected');
      selected.next('li').addClass('selected');
    }
    else {
      facResult.find(selector + ' > li:first').addClass('selected');
    }
  }

  // Select the previous suggestion.
  function selectUp(facResult) {
    var selector = '> div.empty-result ul';
    if (facResult.find('> ul.result-list > li').length) {
      var selector = '> ul.result-list';
    }
    var selected = facResult.find(selector + ' > li.selected');
    if (selected.length) {
      selected.removeClass('selected');
      selected.prev('li').addClass('selected');
    }
    else {
      facResult.find(selector + ' > li:last').addClass('selected');
    }
  }

}(jQuery));
