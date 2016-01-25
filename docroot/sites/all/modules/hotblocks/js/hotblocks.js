/**
 * Utility functions for Hotblocks
 */
Drupal.hotblocks = {
  // Define loading text
  loadingText: '<div class="hotblocks-loading">' + Drupal.t('Loading...') + '</div>',

  // A variable to store the hotblock of the last clicked item (jquery element)
  $hotblock: null,

  /**
   * Detect if an element is visible in the viewport
   * @param el
   * - DOM element or jQuery object
   * @returns {boolean}
   *
   * http://stackoverflow.com/questions/123999/how-to-tell-if-a-dom-element-is-visible-in-the-current-viewport
   */
  isElementInViewport: function (el) {
    // Detect jQuery object
    if (typeof jQuery === "function" && el instanceof jQuery) {
      el = el[0];
    }

    var rect = el.getBoundingClientRect();

    return (
      rect.top >= 0 &&
      rect.left >= 0 &&
      rect.bottom <= (window.innerHeight || document.documentElement.clientHeight) && /*or $(window).height() */
      rect.right <= (window.innerWidth || document.documentElement.clientWidth) /*or $(window).width() */
    );
  },

  scrollToElement: function (el) {
    // Wrap in jQuery in case this is a plain DOM element
    el = jQuery(el);

    // Get the offset with a little bit of buffer space at the top
    var y = el.offset().top - 50;

    // Do the actually scrolling
    jQuery('html,body').animate(
      {scrollTop: y},
      {duration: 500, queue: false /*,complete: function(){}*/}
    );
  }
};


Drupal.behaviors.hotblocks = {
  attach: function(context, settings) {
    var $ = jQuery;

    // Open the hotblocks modal when these links are clicked:
    $('.hotblocks-controls a.hb-modal, .hotblocks_item-controls a.hb-modal', context).click(function(e) {
      // Find the parent block and store it in a variable
      var $hotblock = $(this).closest('.hotblock');
      
      // Get the href of the clicked link
      var link_href = $(this).attr('href');

      // Open the Hotblocks modal and attach behaviors to the links inside
      HotblocksModal.open(link_href, function() {
        var $modal = HotblocksModal.getModal();

        // The create links within the hotblocks 'create' modal
        $('.hotblocks_item-create', $modal).click(function() {
          //We just want to make sure to close the modal before allowing the browser's
          //default behavior on this, since if overlay module is on we won't be leaving the page
          HotblocksModal.close();
        });

        // The item links for assigning content inside the modal
        $('.hotblocks-assign', $modal).click(function() {
          var $extraData = $('#hb-extra-data');

          // The href of the link that was clicked - Append the value of the radio button to the URL query
          var href = $(this).attr('href')
            + '&hotblocks_vis_type=' + $('#hotblocks-vis-type input:checked').val()
            + '&path=' + $extraData.attr('data-path')
            + '&token=' + $extraData.attr('data-token')
            + '&replace=' + $extraData.attr('data-replace');

          // Close the modal, scroll to the top of the hotblock (where new content will appear), and add a loading
          // indicator.
          HotblocksModal.close();

          // Scroll to hotblock top, if the top is not in the viewport area
          if(!Drupal.hotblocks.isElementInViewport($hotblock)) {
            Drupal.hotblocks.scrollToElement($hotblock);
          }

          // Prepend loading text to the hotblock that we're acting on
          $hotblock.prepend(Drupal.hotblocks.loadingText);

          // Replace the hotblock's content with markup retrieved from the server
          $hotblock.load(href,function() {
            // On complete, attach Drupal behaviors and remove the 'loading' modal.
            Drupal.attachBehaviors($hotblock);
          });

          return false;
        });

      });

      return false;
    });
    

    // Click handler for direct actions inside the block - finish assigning an item, directly remove an item, or show the reorder form
    $('.hotblocks-remove, .hotblocks-reorder', context).click(function() {
      // The href of the link that was clicked
      var href = $(this).attr('href');

      Drupal.hotblocks.$hotblock = $(this).closest('.hotblock');

      // When removing an item from a block - do slideUp animation and show a loading indicator
      if($(this).hasClass('hotblocks-remove')) {
        var $hbItemBeingRemoved = $(this).closest('.hotblocks_item');
        // Scroll to the point above item being removed. If the content is very tall, we can otherwise be left at the
        // bottom of the page, which is disorienting.
        if(!Drupal.hotblocks.isElementInViewport($hbItemBeingRemoved)) {
          Drupal.hotblocks.scrollToElement($hbItemBeingRemoved);
        }

        // Animate and add loading text.
        $hbItemBeingRemoved.slideUp().before(Drupal.hotblocks.loadingText);
      }

      // When clicking the 'reorder' link - show a loading indicator inside the hotblock area
      if($(this).hasClass('hotblocks-reorder')) {
        Drupal.hotblocks.$hotblock.html(Drupal.hotblocks.loadingText);

        // Scroll to hotblock top, if the top is not in the viewport area
        if(!Drupal.hotblocks.isElementInViewport(Drupal.hotblocks.$hotblock)) {
          Drupal.hotblocks.scrollToElement(Drupal.hotblocks.$hotblock);
        }
      }

      // Replace the hotblock's content with markup retrieved from the server
      Drupal.hotblocks.$hotblock.load(href,function() {
        // On complete, attach Drupal behaviors
        Drupal.attachBehaviors(Drupal.hotblocks.$hotblock);
        HotblocksModal.close();
      });
      return false;
    });
   
    // Ajaxformify the hotblocks reorder form
    $('#hotblocks-reorder-form',context).ajaxForm({
      target: null,   //We'll populate this at the time the form is submitted
      beforeSubmit:  function(arr, $form, options) {
        // Extrapolate the target object from the form element - used to update with response markup (our parent block)
        options.target = $form.closest('.hotblock');
        
        // Show loading indicator in the block
        options.target.html(Drupal.hotblocks.loadingText);
      },
      success: function(responseText, statusText, xhr, $form) {
        // Reattach Drupal behaviors to our block
        Drupal.attachBehaviors(this);
      }
    });
    
    // Toggler system - hide or unhide child items when a parent is expanded
    $(".toggle a", context).click(function(event){
      var el = $(this.parentNode.parentNode.parentNode.parentNode);
      var next;

      var depth = Number($(el).attr('rel'))+1;

      $('.toggle',this.parentNode.parentNode.parentNode).toggle();
      $('.depth-'+depth,el).toggle();
      
      return false;
    });
  }
};

var HotblocksModal = (function ($) {
  // locally scoped Object
  var self = {};

  var $dialog;

  /**
   * Open the hotblocks modal
   * @param href
   * @param completeCallback
   */
  self.open = function (href, completeCallback) {
    // Open the dialog
    $dialog = $('<div id="hotblocks-dialog" title="Add content">' + Drupal.hotblocks.loadingText + '</div>').dialog(
      {
        autoOpen: true,
        height: Math.floor(window.innerHeight * .75), // 75% of window height
        width: Math.floor(window.innerWidth * .75), // 75% of window width
        modal: true,
        dialogClass: 'hotblocks-dialog',
        close: function (event, ui) {
          $(this).remove();
        }
      }
    );

    // Load hotblocks href
    $.get(href, function (data) { // todo - handle error in case $.get cannot complete
      // Fill the modal with the response HTML
      $dialog.html(data);

      // Attach our own modal behaviors
      self.attachModalBehaviors();

      // Call a user supplied callback
      if (completeCallback) {
        completeCallback();
      }
    });
  };

  /**
   * Close the hotblocks modal
   */
  self.close = function() {
    try { $dialog.dialog('close'); }
    catch(e) {}
  };

  /**
   * Returns a jQuery element which is the modal wrapper
   */
  self.getModal = function() {
    return $dialog;
  };

  /**
   * Attach modal-specific javascript events
   */
  self.attachModalBehaviors = function() {
    var context = $('#hbmodal-wrapper');
    quickFilters(context);
    quickSearch(context);
    quickGroup(context);
    context.tabs();
  };


  /**
   * Setup quick filters
   */
  function quickFilters(context) {
    // Bind the click event of the filter links
    $('#hotblocks-filter-select', context).change(function (e) {
      // Define the container
      var $container = $('#hotblocks-item-list-wrapper table', context);
      // Get the filter value from the clicked link
      var filterValue = $(this).val();

      // Show all
      if (filterValue == 'all') {
        $('tbody tr', $container).show();
      }
      // Show selected
      else {
        // Can't use 'group by' with 'filter by', so set the group by to default (no grouping)
        $('#hotblocks-groupby-select').val('alpha').trigger('change');

        // Hide all items, then reveal the filtered item
        $('tbody tr', $container).hide();
        $('tbody tr.' + filterValue, $container).show();
      }
    });
  }

  /**
   * Initialize/setup the quicksearch plugin
   */
  function quickSearch(context) {
    // Quick search plugin
    $('#hotblocks-filters-container #quicksearch', context).quicksearch('.hotblocks-assign-table tbody tr a', {
      delay : 30,
      onBefore: function() {},
      hide: function () {
        $(this).closest('tr').addClass('quicksearch-hidden');
        //this.style.display = "none";
      },
      show: function () {
        $(this).closest('tr').removeClass('quicksearch-hidden');
        //this.style.display = "";
      },
      onAfter: function () {}
    });
  }

  function quickGroup(context) {
    $('#hotblocks-groupby-select').change(function (e) {
      // Define the container
      var $container = $('#hotblocks-item-list-wrapper', context);

      // Get the filter value from the clicked link
      var filterValue = $(this).val();

      // Hide all the wrapping divs in our container
      $container.find('div').hide();

      // Show the alpha div (aka 'None' or no grouping)
      if(filterValue == 'alpha') {
        $('#hotblocks-item-list-alpha', $container).show();
      }

      // Show the type div
      if(filterValue == 'type') {
        $('#hotblocks-item-list-type', $container).show();

        // Can't use 'group by' with 'filter by', so set the 'filter by' to default (no filtering)
        $('#hotblocks-filter-select').val('all').trigger('change');
      }
    });
  }

  return self;
})(jQuery);
