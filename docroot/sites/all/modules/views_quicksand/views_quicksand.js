(function ($) {

  // If we have no quicksand settings, return immediately.
  if(!Drupal.settings.views_quicksand) {
    return;
  }

  // A library of any functions that we need during our processing.
  Drupal.settings.views_quicksand_library = {
    filter_class : function($element, class_name) {
      var regex = class_name + '[\\w\\d]*';
      var matching_class = $element.attr('class').match(new RegExp(regex))[0];
      $element.removeClass(matching_class);
    }
  };

  var library = Drupal.settings.views_quicksand_library;

  // Create some events we can trigger for end users to hook into. Since in this
  // implementation the view elements are created and destroyed, it's best to
  // bind on to document: $(document).bind('beforeQuicksandAnimation', ...);
  var
    before = jQuery.Event('beforeQuicksandAnimation'),
    after = jQuery.Event('afterQuicksandAnimation'),
    enhancement = jQuery.Event('enhancementQuicksandAnimation');

  // Loop through all of the defined quicksand views.
  $.each(Drupal.settings.views_quicksand, function(id) {
    var config = this;

    // The container that will hold all of our quicksand content.
    var $quicksand_container = false;

    // Create a behaviour for each of the views we will be processing.
    Drupal.behaviors['views_quicksand-' + config.viewname + '-' + config.display] = {
      attach : function(context) {

        var main_pageload = context == document;
        var view_refresh = $(context).hasClass('view');

        if (!main_pageload && !view_refresh) {
          return;
        }

        var $view_container = $(
          '.view-id-' + config.viewname
          + '.view-display-id-' + config.display
          + ':not(.views-quicksand-container)'
        );

        // A function to setup a clone view.
        function setup_quicksand_clone() {
          // Remove any existing views containers.
          $view_container.siblings('.views-quicksand-container').remove();
          // Clone the entire view container to ensure that the styles and
          // pager have come across. Deep copy for bound events.
          $quicksand_container = $view_container.clone(true);
          // If it has the dom-id class it will be removed by the AJAX system.
          library.filter_class($quicksand_container, 'view-dom-id-');
          // Insert our clone after it's counterpart.
          $quicksand_container
            .addClass('views-quicksand-container')
            .insertAfter($view_container)
            .show();
          // Hide the original view.
          $view_container.hide();

          // There can only be one form or the ajax layer gets the post value
          // from the wrong form.
          $quicksand_container.find('form').replaceWith($view_container.find('form'));
        }

        if (main_pageload) {
          setup_quicksand_clone();
        } else if (view_refresh) {

          // At this point $quicksand_container contains the old set of elements
          // and $view_container has the new set of elements.
          var $old_elements = $quicksand_container.find('.view-content');
          // Update the state of our clone for things like our pager and form state.
          setup_quicksand_clone();
          // Ensure our clone contains the old set of elements.
          $quicksand_container.find('.view-content').replaceWith($old_elements);

          // Not having content in our quicksand view causes an error.
          if ($quicksand_container.find('.view-content').length == 0) {
            $quicksand_container.append('<div class="view-content"></div>')
          }

          $quicksand_container.trigger(before);

          // Animate our old elements to our new elements.
          $quicksand_container.find('.view-content').quicksand(
            $view_container.find('.views-row'),
            {
              adjustHeight: config.adjustHeight,
              attribute: function(v) {
                return $(v).find(config.element).attr(config.attribute);
              },
              duration: parseInt(config.duration, 10),
              easing: config.easing,
              useScaling: true,
              enhancement: function() {
                $quicksand_container.trigger(enhancement);
              }
            },
            function() {
              $quicksand_container.trigger(after);
            });
        }
      }
    };
  });

})(jQuery);
