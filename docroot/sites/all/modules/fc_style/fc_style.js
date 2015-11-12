(function ($) {
  /* ********************************
   EQUAL HEIGHTS
   ******************************** */

  // Make all Field Collection items of each row equal in height.
  Drupal.behaviors.fc_styleChanges = {
    attach: function (context, settings) {
      field_collections = settings.fc_style.fc_styleStylise;

      // Change equal heights when window is resized.
      $(window).resize(function() {
        processFields();
      });

      // Set equal heights on page load.
      $(window).load(function() {
        var minimumWindowWidth = 740; // Skip screens smaller than this.

        if ($(window).width() >= minimumWindowWidth) {
          processFields();
        }
      });

      // Set equal heights of Field Collections.
      function processFields() {

        // Find all Field Collections and iterate through them.
        $(function() {

          var minimumWindowWidth = 740; // Skip screens smaller than this.

          // Loop through each type of Field Collection.
          for (var i in field_collections) {
            if (isFinite(String(i))) {
              // Process Field Collections of this type.
              $(field_collections[i]).each(function () {
                // Process a specific Field Collection instance.
                setAutoHeights(this);

                if ($(window).width() >= minimumWindowWidth) {
                  setEqualHeights(this);
                }

              });
            }

          }
        });
      }

      // Set auto calculated heights of items in one specific type of Field Collection.
      function setAutoHeights(fieldCollection) {
        // Loop through each item in a Field Collection.
        var items = $(fieldCollection).find(".fc-style-item");

        // Loop through each item in a Field Collection.
        for (var key in items) {

          // Check key is numeric.
          if (isFinite(String(key))) {
            $(items[key]).height('auto');
          }
        }

      }

      function setRowHeights(rowItems, startItem, endItem, rowHeight) {
        for (var delta = startItem; delta <= endItem; delta++) {
          $(rowItems[delta]).height(rowHeight);
        }
      }

      // Set equal heights of items in one specific Field Collection.
      function setEqualHeights(fieldCollection) {
        // Loop through each item in a Field Collection.
        var items = $(fieldCollection).find(".fc-style-item");
        var rowTop = 0;
        var top = 0;
        var rowMaximum = 0;
        var rowCount = 0;
        var rowStart = 0;
        var setHeights = 0;
        var currentTallest = 0;
        var totalItems = items.length;

        // Get number of items in a row.
        for (var key in items) {

          // Check key is numeric.
          if (isFinite(String(key))) {

            top = $(items[key]).offset().top;

            // Get top of row
            if (rowTop == 0) {
              rowTop = top;
            }

            // Add to count of items in first row.
            if (rowTop == top) {
              rowMaximum = rowMaximum + 1;
            }
          }
        }

        // Loop through each item in a Field Collection.
        for (var key in items) {

          // Check key is numeric.
          if (isFinite(String(key))) {

            rowCount = rowCount + 1;

            // Get height of tallest item in this row.
            if ($(items[key]).height() > currentTallest) {
              currentTallest = $(items[key]).height();
            }

            // First item of a row.
            if (rowCount == 1) {
              $(items[key]).addClass("fc-style-item-alpha");
              rowStart = key;
            }
            // Last item in a row.
            else if (rowCount == rowMaximum) {
              $(items[key]).addClass("fc-style-item-omega");
              setHeights = 1;
            }
            // Middle item of a row.
            else {
              $(items[key]).addClass("fc-style-item-middle");
            }

            // Last item of all rows.
            if (key == (totalItems - 1)) {
              setHeights = 1;
            }

            // End of a row or last item, set heights and reset variables.
            if (setHeights == 1) {
              // Set tallest height on all items in this row.
              setRowHeights(items, rowStart, key, currentTallest);

              // Clear variables for next row.
              rowCount = 0;
              currentTallest = 0;
              setHeights = 0;
            }
          }
        }
      }
    }
  };

})(jQuery);
