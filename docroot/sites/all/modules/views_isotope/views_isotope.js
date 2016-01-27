/**
 * @file
 * Drupal-specific implementation of isotope package.
 */
(function ($) {
  var isotopeActiveFilters = {};

  function params_unserialize(p) {
    if (typeof p == 'undefined') {
      return {};
    }
    var ret = {},
        seg = p.replace(/^\?/,'').split('&'),
        len = seg.length, i = 0, s;
    for (;i<len;i++) {
      if (!seg[i]) { continue; }
      s = seg[i].split('=');
      ret[s[0]] = s[1];
    }
    return ret;
  }

  function isotope_add_hash(group, value, instanceID) {
    var hash = window.location.href.split('#')[1];
    var params = params_unserialize(hash);
    if (typeof instanceID != 'undefined') {
      params[group + '.' + instanceID] = value;
    }
    else {
      params[group] = value;
    }
    location.hash = $.param(params);
  }

  function isotope_hash() {
    var hash = window.location.href.split('#')[1];
    var params = params_unserialize(hash);
    $.each(params, function(key, value) {
      var filterGroup = key.split('.')[0];
      var instanceID = key.split('.')[1];
      if (filterGroup == 'sort') {
        var $selector = $(".isotope-options.sorts");
        if (instanceID) {
          $selector = $selector.filter("[data-instance-id='" + instanceID + "']");
        }
        if($selector.length) {
          // This sort option exists.
          isotope_apply('sort', value, instanceID);
        }
      }
      else {
        var $selector = $(".isotope-options[data-filter-group='" + filterGroup + "']");
        if (instanceID) {
          var $selector = $selector.filter("[data-instance-id='" + instanceID + "']");
        }
        if($selector.length) {
          // This filter option exists.
          isotope_apply('filter', value, instanceID, filterGroup);
        }
      }
    });
  }

  function isotope_apply(type, value, instanceID, filterGroup) {
    // Find all identical optionSets.
    var $optionSets = $(".isotope-options");
    if (type == 'filter') {
      $optionSets = $optionSets.filter("[data-filter-group='" + filterGroup + "']");
    }
    if (type == 'sort') {
      $optionSets = $optionSets.filter(".sorts");
    }
    var $container = $('.isotope-container');
    if (typeof instanceID != 'undefined') {
      var $optionSets = $optionSets.filter("[data-instance-id='" + instanceID + "']");
      var $container = $('#' + instanceID);
    }

    // Apply class change to all identical optionsets.
    $optionSets.find('.selected').removeClass('selected');
    if (type == 'filter') {
      $optionSets.find("[data-filter='" + value + "']").addClass('selected');
      // Combine filters.
      var applyValue = '';
      isotopeActiveFilters[filterGroup] = value;
      for (var prop in isotopeActiveFilters) {
        applyValue += isotopeActiveFilters[ prop ];
      }
      $container.isotope({ filter: applyValue });
    }
    if (type == 'sort') {
      $optionSets.find("[data-sort-by='" + value + "']").addClass('selected');
      applyValue = value.split(',');
      $container.isotope({ sortBy: applyValue });
    }
  }

  Drupal.behaviors.views_isotope = {
    attach:function(context, settings)
    {
      if ($('body', context).length == 0) {
        // This is an ajax call, likely a prepend.
        var $new_items = $(context).filter('.isotope-element');
        if ($new_items.length) {
          $('.isotope-container').isotope('appended', $new_items);
        };
      }

      var $container = $('.isotope-container', context);
      if ($container.length) {
        // Pre-select first option in option sets.
        $('.isotope-options > li:first-child > a', context).addClass('selected');

        // Use imagesLoaded if it is available.
        if (typeof imagesLoaded !== 'undefined') {
          var container = $container.get();
          imagesLoaded( container, function() {
            if($container.data('isotope')) {
              $container.isotope('layout');
            }
          });
        }

        // Config options that are not instance-specific.
        var config = $container.data('isotope-options');

        // Check for new hash values.
        if (config.urlFilters == 1) {
          window.onhashchange = isotope_hash;
          isotope_hash();
        }

        if (config.stamp) {
          $container.isotope('stamp', $(config.stamp));
        }

        // Filter button click.
        $('.isotope-options', context).delegate('.filterbutton:not(.selected)', 'click', function(e) {
          var $this = $(this);

          // Identify what has been clicked.
          var $optionSet = $this.parents('.isotope-options');
          var filterGroup = $optionSet.attr('data-filter-group');
          var instanceID = $optionSet.attr('data-instance-id');
          var filterValue = $this.attr('data-filter');

          if (config.urlFilters == 1) {
            isotope_add_hash(filterGroup, filterValue, instanceID);
          }
          else {
            isotope_apply('filter', filterValue, instanceID, filterGroup);
          }

          e.preventDefault();
        });

        // Sort button click.
        $('.isotope-options', context).delegate('.sorterbutton:not(.selected)', 'click', function(e) {
          var $this = $(this);

          // Identify what has been clicked.
          var $optionSet = $this.parents('.isotope-options');
          var instanceID = $optionSet.attr('data-instance-id');
          var sortValue = $this.attr('data-sort-by');

          if (config.urlFilters == 1) {
            isotope_add_hash('sort', sortValue, instanceID);
          }
          else {
            isotope_apply('sort', sortValue, instanceID);
          }

          e.preventDefault();
        });
      }
    }
  }
}(jQuery));
