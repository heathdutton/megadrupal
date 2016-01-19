(function ($) {

  var textSplitterClass = 'text-splitter';
  var textSplitterActive = 'text-splitter-active';
  var textSplitterExclude = 'text-splitter-exclude';
  var textSplitterHide = 'text-splitter-hide';

  // Show a element.
  $.fn.textSplitterToShow = function () {
    this.removeClass(textSplitterHide).addClass(textSplitterActive).show();
    $(window).resize();
  };

  // Hide a element.
  $.fn.textSplitterToHide = function () {
    this.removeClass(textSplitterActive).addClass(textSplitterHide).hide();
    $(window).resize();
  };

  // Show all elements.
  function textSplitterShowAll(elements, textSplitterClass, textSplitterFilterId, textSplitterActive, textSplitterSelector) {
    $.each(elements, function () {
      var locationItem = $(this);
      var locationItemClasses = locationItem.attr('class').split(' ');

      if (!locationItem.hasClass(textSplitterClass)) {
        return;
      }

      // Finding relevant filters and show all elements.
      $.each(locationItemClasses, function (index, value) {
        if (value.match(/text-splitter-\d+-\d+/)) {
          var nextLocationItemId = '.' + textSplitterFilterId + (parseInt(value.match(/text-splitter-\d+-(\d+)/)[1], 10) + 1);
          $('.text-splitter-filter .' + value).removeClass(textSplitterActive);
          locationItem.textSplitterToShow();
          locationItem.nextUntil(textSplitterSelector + ' ' + nextLocationItemId).textSplitterToShow();
        }
      });
    });
  }

  Drupal.behaviors.text_splitter = {
    attach: function (context, settings) {
      if (!settings.text_splitter) {
        return;
      }

      $.each(settings.text_splitter, function (index, settings) {
        var textSplitterSelector = settings.selector;
        var textSplitterId = 'text-splitter-id-' + settings.id;
        var textSplitterFilterId = 'text-splitter-' + settings.id + '-';
        var location = $(textSplitterSelector, context);

        if (!location.length) {
          return;
        }

        // Found elements in the location.
        var elements = location.find(settings.tags);

        if (settings.type === 'filter') {
          var textSplitterFilter = $('.text-splitter-filter.' + textSplitterId);

          if (!textSplitterFilter.length || !elements.length) {
            return;
          }

          // An element is active.
          var hasActive = false;

          // Create filters and set classes in the location.
          var i = 0;
          $.each(elements, function (index, value) {
            var locationItem = $(value);

            // The element is empty or excluded.
            if (!locationItem.text() || locationItem.hasClass(textSplitterExclude)) {
              return;
            }

            var filterId = textSplitterFilterId + (i++);
            var filter = $('<button></button>').addClass(textSplitterClass + ' ' + textSplitterClass + '-filter-button ' + filterId).html(locationItem.text());

            // The element has active class.
            if (locationItem.hasClass(textSplitterActive)) {
              hasActive = true;
              filter.addClass(textSplitterActive);
            }

            textSplitterFilter.append(filter);
            locationItem.addClass(textSplitterClass + ' ' + textSplitterClass + '-separator ' + filterId);
          });

          // Values of the filter.
          var textSplitterFilterValues = textSplitterFilter.find('.' + textSplitterClass);

          if (!textSplitterFilterValues.length) {
            return;
          }

          // Hide elements in the location.
          $.each(textSplitterFilterValues, function (index, value) {
            var filter = $(value);
            var filterId = '.' + $(this).attr('class').match(/text-splitter-\d+-\d+/)[0];
            var nextFilterId = '.' + textSplitterFilterId + (parseInt($(this).attr('class').match(/text-splitter-\d+-(\d+)/)[1], 10) + 1);
            var locationItem = location.find(filterId);

            // A filter has active class.
            if (filter.hasClass(textSplitterActive)) {
              locationItem.nextUntil(textSplitterSelector + ' .' + nextFilterId).addClass(textSplitterActive);
            }
            // Hide an element in the location.
            else {
              locationItem.textSplitterToHide();
              locationItem.nextUntil(textSplitterSelector + ' .' + nextFilterId).textSplitterToHide();
            }
          });

          // No active elements.
          if (!hasActive) {
            // Show a first element in the location.
            textSplitterFilterValues.first().addClass(textSplitterActive).each(function () {
              var filterId = '.' + $(this).attr('class').match(/text-splitter-\d+-\d+/)[0];
              var nextFilterId = '.' + textSplitterFilterId + (parseInt($(this).attr('class').match(/text-splitter-\d+-(\d+)/)[1], 10) + 1);
              var locationItem = location.find(filterId);

              locationItem.textSplitterToShow();
              locationItem.nextUntil(textSplitterSelector + ' .' + nextFilterId).textSplitterToShow();
            });
          }

          // Click on the filter.
          textSplitterFilterValues.click(function () {
            var filterId = '.' + $(this).attr('class').match(/text-splitter-\d+-\d+/)[0];
            var nextFilterId = '.' + textSplitterFilterId + (parseInt($(this).attr('class').match(/text-splitter-\d+-(\d+)/)[1], 10) + 1);
            var locationItem = location.find(filterId);

            // The element is active.
            if ($(this).hasClass(textSplitterActive)) {
              return;
            }

            var locationActiveItems = location.find('.' + textSplitterActive);
            var locationItemClasses = locationItem.attr('class').split(' ');

            // Remove an active class to filters by a current element.
            $.each(locationActiveItems, function () {
              $.each($(this).attr('class').split(' '), function (index, value) {
                if (value.match(/text-splitter-\d+-\d+/)) {
                  $('.text-splitter-filter .' + value).removeClass(textSplitterActive);
                }
              });
            });

            // Add an active class to filters by a current element.
            $.each(locationItemClasses, function (index, value) {
              if (value.match(/text-splitter-\d+-\d+/)) {
                $('.text-splitter-filter .' + value).addClass(textSplitterActive);
              }
            });

            locationActiveItems.textSplitterToHide();
            locationItem.textSplitterToShow();
            locationItem.nextUntil(textSplitterSelector + ' ' + nextFilterId).textSplitterToShow();

            // @todo Show the corresponding button only.
            $('.text-splitter-button').show();
          });

          textSplitterFilter.show();
        }
        else {
          if (!elements.length) {
            return;
          }

          var textSplitterButton = $('.text-splitter-button.' + textSplitterId);

          // Click on the button.
          textSplitterButton.click(function () {
            textSplitterShowAll(elements, textSplitterClass, textSplitterFilterId, textSplitterActive, textSplitterSelector);
            textSplitterButton.hide();
          });

          textSplitterButton.show();
        }
      });
    }
  };

})(jQuery);
