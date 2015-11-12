/**
 * @file
 * Homebox packery javascipt file.
 */

(function($) {

  Drupal.homebox_packery = {
    config: {}
  };

  Drupal.behaviors.homebox_packery = {
    attach: function(context, settings) {
      var $homebox_packery = $('#homebox:not(.homebox-packery-processed)', context).addClass('homebox-packery-processed');

      // To launch packery only once.
      if ($homebox_packery.length > 0) {
        Drupal.homebox_packery.$columns = $homebox_packery.find('div.homebox-column');

        // Launch packery construct.
        Drupal.homebox_packery.construct();

        // Refresh packery on homebox button click.
        Drupal.homebox_packery.portlet_click();
      }
    }
  };

  /**
   * Construct function to create the active packery with the settings
   * And add the draggable on the items
   */
  Drupal.homebox_packery.construct = function () {
    for (var packery in Drupal.settings.homebox_packery.packery) {
      var $container = $(packery);
      $container.packery(Drupal.settings.homebox_packery.packery[packery].settings);

    	Drupal.homebox_packery.item_draggable($container, packery);

      // On draggable item: populate the hidden form and send the homebox ajax callback.
    	$container.packery('on', 'dragItemPositioned',
        function(pckryInstance, draggedItem) {

          var blocks = {}, regionIndex;
          regionIndex = Drupal.homebox_packery.$columns.attr('id').replace(/homebox-column-/, '');

          var itemElems = $container.packery('getItemElements');
          $(itemElems).each(function(i, itemElem) {
            var $this = $(itemElem), color = 'default';

            // Determine custom color, if any.
            $.each($this.attr('class').split(' '), function (key, a) {
              if (a.substr(0, 14) === 'homebox-color-') {
                color = a.substr(14);
              }
            });

            // Build blocks object.
            blocks[$this.attr('id').replace(/^homebox-block-/, '')] = $.param({
              region: regionIndex,
              status: $this.is(':visible') ? 1 : 0,
              color: color,
              open: $this.find('.portlet-content').is(':visible') ? 1 : 0
            });

            // Rearrange the DOM element to fit the new order (like sortable).
            // To keep the order list after a packery('reloadItems').
            if (i != $this.index()) {
              elem = Drupal.homebox_packery.$columns.find(Drupal.settings.homebox_packery.packery[packery].settings.itemSelector).get(i);
              $this.insertAfter(elem);
            }
          });

          // If the order of the block has change, we launch the homebox ajax callback.
          if ($('#homebox-save-form').find('[name=blocks]').attr('value') != $.param(blocks)) {
            $('#homebox-save-form').find('[name=blocks]').attr('value', $.param(blocks));
            $('#homebox-changes-made').hide();

            // Launch Homebox Ajax callback.
            Drupal.homebox.$pageSave.click();
          }
        }
      );
    }
  }

  /**
   * Build function to create the active packery with the settings
   */
  Drupal.homebox_packery.build = function () {
    for (var packery in Drupal.settings.homebox_packery.packery) {
      var $container = $(packery);
      $container.packery(Drupal.settings.homebox_packery.packery[packery].settings);
    }
  }

  /**
   * Active the draggability to the items of packery.
   */
  Drupal.homebox_packery.item_draggable = function ($container, packery) {
    // Get item elements, jQuery-ify them.
  	var $itemElems = $container.find(Drupal.settings.homebox_packery.packery[packery].settings.itemSelector);
  	// Make item elements draggable.
  	$itemElems.draggable();
  	// Bind Draggable events to Packery.
  	$container.packery('bindUIDraggableEvents', $itemElems);
  }

  /**
   * Active the refresh of packery to the homebox button on click event.
   */
  Drupal.homebox_packery.portlet_click = function () {
    $portlet = $('.homebox-portlet');
    $portletHeader = $portlet.find('.portlet-header');
    $portletSettings = $portlet.find('.portlet-settings');
    $portletConfig = $portlet.find('.portlet-config');

    $portlet.delegate('.portlet-header, .portlet-settings, .portlet-config', 'click', function() {
      Drupal.homebox_packery.build();
    });
  }

  /**
   * Rebuild function to create the active packery with the settings
   * Reload items of packery and add the draggability + the build on click
   * After on ajax callback
   *
   * TODO: simplify this and see why we have to launch packery 3 items
   * And with the reloadItems, otherwise it doesn't work
   */
  Drupal.homebox_packery.rebuild = function () {
    for (var packery in Drupal.settings.homebox_packery.packery) {
      var $container = $(packery);
      $container.packery(Drupal.settings.homebox_packery.packery[packery].settings).packery('reloadItems');
      $container.packery(Drupal.settings.homebox_packery.packery[packery].settings);

      Drupal.homebox_packery.item_draggable($container, packery);
    }

    Drupal.homebox_packery.portlet_click();
  }

  $(document).ajaxComplete(function(e, a) {
    // TODO: check if this test on the ajax url is relevant.
    // But now it preserves the rebuild to be launch when it doesn't needs it.
    if (arguments[2].url != '/system/ajax') {
      Drupal.homebox_packery.rebuild();
    }
  });

}(jQuery));
