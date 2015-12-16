/**
 * @file
 * Drupal-specific implementation of isotope package.
 */

(function ($) {
  Drupal.behaviors.views_isotope = {
    attach:function(context, settings)
    {
      // Pre-select first option in option sets.
      $('.isotope-options > li:first-child > a').addClass('selected');

      // Use imagesLoaded if it is available.
      if (typeof imagesLoaded !== 'undefined') {
        var container = document.querySelectorAll('.isotope-container');
        imagesLoaded( container, function() {
          $('.isotope-container').isotope('layout');
        });
      }

      // Store filter for each group.
      var filters = {};
      $('.isotope-options').delegate('.filterbutton', 'click', function(e) {
        var $this = $(this);

        // Don't proceed if already selected.
        if ($this.hasClass('selected')) {
          return false;
        }

        // Identify what has been clicked.
        var $optionSet = $this.parents('.isotope-options');
        var filterGroup = $optionSet.attr('data-filter-group');
        var instanceID = $optionSet.attr('data-instance-id');

        // Set filter for group.
        filters[filterGroup] = $this.attr('data-filter');

        // Find all identical optionSets.
        if (typeof instanceID != 'undefined') {
          var $optionSets = $(".isotope-options[data-filter-group='" + filterGroup + "'][data-instance-id='" + instanceID + "']");
          var $container = $('#' + instanceID);
        }
        else {
          var $optionSets = $(".isotope-options[data-filter-group='" + filterGroup + "']");
          // If no instance is set, the filter should apply to all instances.
          var $container = $('.isotope-container');
        }

        // Apply class change to all identical optionsets.
        $optionSets.find('.selected').removeClass('selected');
        $optionSets.find("[data-filter='" + filters[filterGroup] + "']").addClass('selected');

        // Combine filters.
        var filterValue = '';
        for (var prop in filters) {
          filterValue += filters[ prop ];
        }

        // Set filter for Isotope.
        $container.isotope({ filter: filterValue });

        e.preventDefault();
      });

      // Apply Sorts.
      $('.isotope-options').delegate('.sorterbutton', 'click', function(e) {
        var $this = $(this);

        // Don't proceed if already selected.
        if ($this.hasClass('selected')) {
          return false;
        }

        // Identify what has been clicked.
        var $optionSet = $this.parents('.isotope-options');
        var instanceID = $optionSet.attr('data-instance-id');

        // Find all identical optionSets.
        if (typeof instanceID != 'undefined') {
          var $optionSets = $(".isotope-options.sorts[data-instance-id='" + instanceID + "']");
          var $container = $('#' + instanceID);
        }
        else {
          var $optionSets = $(".isotope-options.sorts");
          // If no instance is set, the filter should apply to all instances.
          var $container = $('.isotope-container');
        }

        // Apply class change to all identical optionsets.
        $optionSets.find('.selected').removeClass('selected');
        $optionSets.find("[data-sort-by='" + $this.attr('data-sort-by') + "']").addClass('selected');

        // Apply Sort.
        var sortValue = $this.attr('data-sort-by');
        // Make an array of values.
        sortValue = sortValue.split(',');
        $container.isotope({ sortBy: sortValue });

        e.preventDefault();
      });
    }
  }
}(jQuery));
