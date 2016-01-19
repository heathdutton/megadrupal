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
      resizeTimer: null,
      emptyResult: '',
      highlightEnabled: false
    }, options);

    // Check if a jsonFilesPath is set before doing anything.
    if (settings.jsonFilesPath !== undefined) {
      var facResult = undefined;

      toggleResponsiveBehavior(facResult, settings);
      $(window).resize(function(e) {
        clearTimeout(settings.resizeTimer);
        settings.resizeTimer = setTimeout(toggleResponsiveBehavior(facResult, settings), 250);
      });

      // Only apply the plugin on input fields of type text.
      this.filter('input:text').each(function() {
        $(this).attr('autocomplete', 'OFF');
        $(this).attr('aria-autocomplete', 'list');

        // Create the hidden result div that will contain the suggestions.
        facResult = $('<div class="fac-result"></div>').hide();
        var resultList = $('<ul class="result-list"></ul>').appendTo(facResult).hide();

        // Add the see all link to the result div.
        var seeAllLink = $('<li class="see-all-link"><div><a href="#"></a></div></li>');
        // The mousedown and click events are to prevent the default link behavior.
        seeAllLink.find('> div > a').mousedown(function(e) {
          e.preventDefault();
        }).click(function(e) {
          e.preventDefault();
        });
        seeAllLink.css('cursor', 'pointer').mousedown(function(e) {
          switch (e.which) {
            // Left mouse click.
            case 1:
              facResult.closest('form').submit();
              break;
          }
          e.preventDefault();
        }).hover(function(e) {
          facResult.find('> ul.result-list > li.selected').removeClass('selected');
          $(this).addClass('selected');
        }).hide();
        seeAllLink.appendTo(resultList);

        // Add the empty result text.
        var emptyResults = $('<div class="empty-result"></div>');
        emptyResults.html(settings.emptyResult);
        emptyResults.find('a').each(function() {
          $(this).mousedown(function (e) {
            e.preventDefault();
            switch (e.which) {
              // Left mouse click.
              case 1:
                window.location = $(this).attr('href');
                break;
            }
          });
        });
        emptyResults.appendTo(facResult);
        var form = $(this).closest('form');
        form.css('position', 'relative');
        facResult.appendTo(form);

        // When a character is entered perform the necessary ajax call. Don't
        // respond to any special keys.
        $(this).keyup(function(e) {
          if (settings.enabled) {
            if (!e) {
              e = window.event;
            }
            switch (e.keyCode) {
              case 9:
              case 16:
              case 17:
              case 18:
              case 20:
              case 33:
              case 34:
              case 35:
              case 36:
              case 37:
              case 38:
              case 39:
              case 40:
              case 13:
              case 27:
                return true;

              default:
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
              // Down arrow.
              case 40:
                selectDown(facResult);
                return false;

              // Up arrow.
              case 38:
                selectUp(facResult);
                return false;

              // Enter.
              case 13:
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
                break;

              // Esc.
              case 27:
                if (facJsonRequest != null) {
                  facJsonRequest.abort();
                }
                facResult.hide();
                return false;

              // All other keys.
              default:
                return true;

            }
          }
        });

        // Hide the result div when the input element loses focus.
        $(this).blur(function(e) {
          if (settings.enabled) {
            if (facJsonRequest != null) {
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
  };

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

    var resultList = facResult.find('> ul.result-list');
    var emptyResult = facResult.find('> div.empty-result');
    var seeAllLink = resultList.find('> li.see-all-link');

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
          if (facJsonRequest != null) {
            facJsonRequest.abort();
          }
        },
        success: function(data, status, xhr) {
          if (data.items.length) {
            seeAllLink.find('> div > a').html(Drupal.t('See all results for "%key"', {'%key' : $(element).val()}));
            resultList.find('> li.result').remove();
            emptyResult.hide();
            seeAllLink.show();
            $.each(data.items, function(key, dataValue) {
              var item = $('<li class="result">' + dataValue + '</li>');
              item.css('cursor', 'pointer').mousedown(function(e) {
                e.preventDefault();
                switch (e.which) {
                  // Left mouse click.
                  case 1:
                    window.location = item.find('a:not(.contextual-links a)').attr('href');
                    break;
                }
              }).hover(function(e) {
                resultList.find('> li.selected').removeClass('selected');
                $(this).addClass('selected');
              });
              item.insertBefore(seeAllLink);
            });
            if (settings.highlightEnabled && $.highlight) {
              resultList.find('> li.result').highlight(value.split(' '));
            }
            resultList.show();
            facResult.show();
          }
          else {
            resultList.hide();
            resultList.find('li.result').remove();
            seeAllLink.hide();
            facResult.hide();
            emptyResult.show();
          }
        },
        complete: function() {
          if (settings.highlightEnabled && $.highlight) {
            resultList.find('> li.result').unhighlight();
            resultList.find('> li.result').highlight(value.split(' '));
          }
        }
      });
      $(element).removeClass('throbbing');
    }
    else {
      if (settings.highlightEnabled && $.highlight) {
        resultList.find('> li.result').unhighlight();
        resultList.find('> li.result').highlight(value.split(' '));
      }
      // If the key is empty, clear the result div and show the empty result content.
      if (value.length < 1) {
        resultList.hide();
        resultList.find('> li.result').remove();
        seeAllLink.hide();
        facResult.hide();
        emptyResult.show();
      }
    }
  }

  // Select the next suggestion.
  function selectDown(facResult) {
    var selector = '> div.empty-result ul';
    if (facResult.find('> ul.result-list > li').length) {
      selector = '> ul.result-list';
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
      selector = '> ul.result-list';
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
