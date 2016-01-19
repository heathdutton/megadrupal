(function($) {

  Drupal.popup_field_group = {};

  /**
   * We maintain a public stack of 'active' (i.e. popped-up) popups.
   */
  Drupal.popup_field_group.activePopups = [];

  /**
   * Convenience method for returning the top-most popped-up Popup.
   *
   * @return mixed
   *   {}   - with data about top popped-up popup.
   *   null - if no popup is popped-up.
   */
  Drupal.popup_field_group.getActivePopup = function() {
    if (Drupal.popup_field_group.activePopups.length) {
      return Drupal.popup_field_group.activePopups[Drupal.popup_field_group.activePopups.length-1];
    }
  }

  /**
   * Apply all the right classes to open the given group.
   *
   * @param $group
   */
  Drupal.popup_field_group.openPopup = function($group) {
    // Toggle group to popped up state.
    $group.addClass('popup-field-group-open');
    $group.removeClass('popup-field-group-closed');
    // Also pop a class with the stacking position.
    $group.addClass('popup-field-group-level-' + Drupal.popup_field_group.activePopups.length);
    // Add to our global variable.
    Drupal.popup_field_group.activePopups.push({
      "group": $group.get(0),
      "stack_position": Drupal.popup_field_group.activePopups.length
    });
    // Update our <html> classes.
    update_html_classes();
    // This fixes issues with ckeditor being the wrong width in our popups.
    // It's probably because we have CSS transitions going on?
    setTimeout(function() {
      // Need to trigger low-level window resize event.
      if (document.createEvent) { // W3C
        var ev = document.createEvent('Event');
        ev.initEvent('resize', true, true);
        window.dispatchEvent(ev);
      }
      else if (document.body.fireEvent) { // IE
        document.body.fireEvent('onresize');
      }
    }, 600);
  }

  /**
   * Apply all the right classes to close the given group.
   *
   * @param $group jQuery collection
   *   (Optional) Close a specific popup. If omitted the top-most is closed.
   *   This should be a jQuery collection containing the group to close.
   *
   * @param keepChildrenOpen bool
   *   (Optional) By default all children groups will be closed too. Pass true
   *   here to keep em open.
   *
   * @return mixed
   *   $group if a popup was closed
   *   false if there was no popup to close (i.e. already closed/non-existent)
   */
  Drupal.popup_field_group.closePopup = function($group, keepChildrenOpen) {
    if (!$group) {
      var top_popup = Drupal.popup_field_group.getActivePopup();
      if (top_popup && top_popup.group) {
        $group = $(top_popup.group);
      }
    }
    if ($group && $group.length && !$group.hasClass('popup-field-group-closed')) {
      // Start by recursively closing any open children groups.
      if (!keepChildrenOpen) {
        $group.find('.popup-field-group-open').each(function() {
          Drupal.popup_field_group.closePopup($(this), true);
        });
      }
      // Toggle classes.
      $group.removeClass('popup-field-group-open');
      $group.addClass('popup-field-group-closed');
      // Remove any stacking level class.
      $group.removeClass(function() {
        return this.className.match(/popup-field-group-level-\d+/);
      });
      // Update global variable & <html> classes.
      refresh_active_popup_stack();
      update_html_classes();

      return $group;
    }
    return false;
  }

  /**
   * Refresh our active popups array, removing any which are now closed.
   */
  var refresh_active_popup_stack = function() {
    var l = Drupal.popup_field_group.activePopups.length;
    var newstack = [];
    for (var i = 0; i < l; i++) {
      if ($(Drupal.popup_field_group.activePopups[i].group).hasClass('popup-field-group-open')) {
        newstack.push(Drupal.popup_field_group.activePopups[i]);
      }
    }
    Drupal.popup_field_group.activePopups = newstack;
  }

  /**
   * Helper function which updates the <html> tag with handy classes indicating
   * whether or not a popup is active.
   */
  var update_html_classes = function() {
    var $html = $('html');

    if (Drupal.popup_field_group.activePopups.length) {
      $html.addClass('popup-field-group-active');
    }

    else {
      $html.removeClass('popup-field-group-active');
    }
  }

  /**
   * Close any open popup if ESC key is pressed.
   */
  $(document).keyup(function(e) {
    // Escape key
    if (e.keyCode == 27) {
      Drupal.popup_field_group.closePopup();
    }
  });

  /**
   * Popup field groups Drupal behaviours.
   *
   * @type {Object}
   */
  Drupal.behaviors.popupFieldGroup = {

    /**
     * Attach our behaviors to the DOM.
     *
     * @param context
     * @param settings
     */
    attach: function(context, settings) {
      $(context).find('.popup-field-group').not('.popup-field-group-attached').each(function() {
        var $group = $(this);

        // If we contain a form element with an error on it, do not attach.
        // This displays it in plain sight for total clarity.
        if ($group.find('.error').length) {
          return;
        }
        $group.addClass('popup-field-group-attached');
        var $overlay = $group.find('.overlay').first();

        // Create / set up link.
        var $link = $(Drupal.theme('popupFieldGroupLink', $group.data('popup-link-title'), $group.data('popup-link-classes')));
        $link.bind('click.popupFieldGroup', function() {
          Drupal.popup_field_group.openPopup($group);
        }).insertBefore($group);

        // Set up overlay.
        $overlay.click(function() {
          Drupal.popup_field_group.closePopup($group);
        });

        // Set up top-right close link.
        $group.find('.popup-field-group-close').first().click(function() {
          Drupal.popup_field_group.closePopup($group);
        });

        // Create / set up close button at the bottom of the popup.
        var $close_button = $(Drupal.theme('popupFieldGroupSaveButton', $group.data('popup-close-button-title'), $group.data('popup-close-button-classes')));
        $close_button.bind('click.popupFieldGroup', function() {
          Drupal.popup_field_group.closePopup($group);
        }).appendTo($group.find('.popup-inner').first());

        // Initialize.
        Drupal.popup_field_group.closePopup($group);
      });
    },

    /**
     * Detach our behaviors from the DOM.
     *
     * @param context
     * @param settings
     * @param trigger
     */
    detach: function(context, settings, trigger) {

      // We don't need to do anything when ajax handlers are searching for
      // valid field values.
      if (trigger === 'serialize') {
        return;
      }

      $(context).find('.popup-field-group-attached').each(function() {
        var $group = $(this);
        if ($group.hasClass('popup-field-group-open')) {
          Drupal.popup_field_group.closePopup($group);
        }
        // In general there's no need to cause visual chaos by suddenly showing
        // all of our popups contents, so we leave it as it is. However, if
        // we're being moved we should cleanly and completely remove everything
        // because our attachBehaviors are going to be run on us in a bit.
        if (trigger === 'move') {
          $group.removeClass('popup-field-group-attached popup-field-group-open popup-field-group-closed');
        }
      });
      // In general there's no need to cause visual chaos by removing the link.
      $(context).find('.popup-field-group-link').unbind('click.popupFieldGroup');
      $(context).find('.popup-field-group-close-button').unbind('click.popupFieldGroup');
      // If we're being moved we should cleanly and completely remove every-
      // thing because our attachBehaviors are going to be run on us in a bit.
      if (trigger === 'move') {
        $(context).find('.popup-field-group-link').remove();
        $(context).find('.popup-field-group-close-button').remove();
      }
    }
  };

  /**
   * Markup for our little popup link we generate.
   *
   * @param label
   * @param classes
   * @return {String}
   */
  Drupal.theme.prototype.popupFieldGroupLink = function(label, classes) {
    classes = classes || '';
    return '<a class="popup-field-group-link ' + classes + '" href="javascript:void(0)" title="' + label + '">' + label + '</a>';
  }

  /**
   * Markup for the close button we pop at the bottom of the popup.
   *
   * @param label
   * @param classes
   * @return {String}
   */
  Drupal.theme.prototype.popupFieldGroupSaveButton = function(label, classes) {
    classes = classes || '';
    return '<a class="popup-field-group-close-button ' + classes + '" href="javascript:void(0)" title="' + label + '">' + label + '</a>';
  }

})(jQuery);
