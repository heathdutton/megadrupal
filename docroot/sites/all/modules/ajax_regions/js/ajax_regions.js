(function ($) {
  Drupal.ajax_regions = {};
  Drupal.behaviors.ajax_regions = {
    detach: function(context) {
      $('a', context).each(function(index) {
        if ($(this).attr('ajax-regions-index')) {
          $(this).removeAttr('ajax-regions-index');
          $(this).unbind('click');
        }
      });
    },
    attach: function(context, settings) {
      for (var i in settings.ajax_regions.array) {
        var ar = settings.ajax_regions.array[i];
        $(ar.link).each(function(index) {
          if ($(this).parents(context).length > 0) {
            if (!$(this).is("[ajax-regions-index]")) {
              $(this).attr('ajax-regions-index', i);
            }
          }
        });
      }
      $('a', context).each(function(index) {
        if ($(this).attr('ajax-regions-index')) {
          $(this).bind('click', {set: settings}, ajaxregion_click);
        }
      });

      function ajaxregion_click(event) {
        // Store current link for ajax call while detecting home.
        var link = this;
        var settings = event.data.set;

        // Unfortunally, we need to override default explorer 7.0 behaviour.
        // See:
        // @link: http://stackoverflow.com/questions/7793728/get-a-relative-path-with-jquery-attr-property-with-ie7
        if (($.browser.msie) && ($.browser.version == '7.0')) {
          var url = $(link).attr('href').replace('http://' + window.location.hostname,'');
        }
        else {
          var url = $(link).attr('href');
        }

        // Load region.
        ajaxregion_load(link, url, settings);

        // We don't really want default click.
        return false;
      }

      // Function to load region.
      function ajaxregion_load(link, url, settings) {
        var i = $(link).attr('ajax-regions-index');
        var options = settings.ajax_regions.array[i];
        var regions = $.parseJSON(options.regions);
        Drupal.ajax_regions.load(link, url, options, regions);
      }
    }
  };

  /**
   * Regionloader function version 1.0
   */
  Drupal.ajaxRegions = {
    load: function(url, data, regions) {
      Drupal.ajax_regions.load({}, url, data, regions);
    }
  }

  /**
   * Regionloader function version 2.0
   */
  Drupal.ajax_regions.load = function(link, url, options, regions) {
    // Browser loading indicator (start).
    if (options.set_indicator) {
      $loadingIndicator = $('<iframe src="/ajax_regions/sleep/100" style="display: none;">');
      $loadingIndicator.appendTo('body');
    }

    // Check custom function ajax_regions_before().
    if (typeof Drupal.ajax_regions.before == 'function') {
      Drupal.ajax_regions.before(link);
    }

    // Make ajax call to module.
    var element_settings = {
      'submit': {'ajax_regions': regions},
      'url': url,
      'link': link,
      'options': options
    };
    var ajax = new Drupal.ajax(false, false, element_settings);
    ajax.eventResponse(ajax, {});
  }

  /**
   * Ajax delivery command to reload regions.
   */
  Drupal.ajax.prototype.commands.ajax_regions = function(ajax, response, status) {
    var settings;
    // Settings (1 receive step).
    for (var i in response.data) {
      obj = response.data[i];
      switch (obj.id) {
        case 'settings':
          settings = obj;
          break;

        case 'views':
          if (Drupal.settings.views === undefined) {
            Drupal.settings.views = obj.views;
          }
          else {
            for (var av in obj.views.ajaxViews) {
              Drupal.settings.views.ajaxViews[av] = obj.views.ajaxViews[av];
            }
            break;
          }
      }
    }

    // Set browser window title using format "Node title | Sitename".
    if (ajax.element_settings.options.set_document_title) {
      document.title = settings.node_title + ' | ' + Drupal.settings.ajax_regions.site_name;
    }

    // Set up current page address.
    if (ajax.element_settings.options.update_current_address) {
      history.pushState({path: ajax.url}, '', ajax.url);
    }

    // Regions (2 receive step).
    for (var i in response.data) {
      obj = response.data[i];
      if (obj.id == 'region') {
        Drupal.detachBehaviors($(obj.selector));
        $(obj.selector).html(obj.html);
        Drupal.attachBehaviors($(obj.selector));
      }
    }

    // Set up .active class for current links.
    if (ajax.element_settings.options.update_active_links) {
      $('a').removeClass('active');
      $('a[href="' + ajax.url + '"]').addClass('active');
    }

    // Check custom function ajax_regions_after().
    if (typeof Drupal.ajax_regions.after == 'function') {
      Drupal.ajax_regions.after(ajax.element_settings.link, settings);
    }

    // Browser loading indicator (stop).
    if (ajax.element_settings.options.set_indicator) {
      $loadingIndicator.remove();
    }
  };
})(jQuery);
