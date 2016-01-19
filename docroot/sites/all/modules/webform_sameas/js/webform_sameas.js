/**
 * @file
 * Javascript behaviors for the webform_sameas component.
 */
(function ($) {
  Drupal.webform_sameas = Drupal.webform_sameas || {}

  /**
   * Handles copying data from the source field to the target field.
   */
  Drupal.webform_sameas.copy_single = function(event, source_field) {
    if (typeof source_field === 'undefined') {
      source_field = $(this);
    }
    var source_classes = source_field.attr('class').split(/\s+/);

    var sameas_checkbox_selector = '.webform-sameas';
    var sameas_checkbox = $(sameas_checkbox_selector);
    var sameas_enabled = sameas_checkbox.is(':checked');
    var sameas_target_field_selector = false;
    var is_address_field = false;
    var addressfield_subfield = false;
    var display_mode = 'invisible';

    for (var index in source_classes) {
      var class_array = source_classes[index].split('--');
      if (class_array[0] == 'webform-sameas-controller-cid') {
        sameas_checkbox_selector = '.webform-sameas-cid--' + class_array[1];
      }
      if (class_array[0] == 'webform-sameas-target-cid') {
        sameas_target_field_selector = '.webform-sameas-cid--' + class_array[1];
      }
      if (class_array[0] == 'webform-sameas-type' && class_array[1] == 'addressfield') {
        is_address_field = true;
        addressfield_subfield = source_classes[0];
      }
      if (class_array[0] == 'webform-sameas-controller-form-key') {
        display_mode = Drupal.settings.webform_sameas.controllers['webform-sameas--' + class_array[1]]['display_mode'];
      }
    }

    if (sameas_enabled && display_mode != 'invisible') {
      var source_value = source_field.val();
      if (sameas_target_field_selector) {
        // Copy the value from the source field to the target field.
        if (is_address_field) {
          sameas_target_field_selector += '.' + addressfield_subfield;
        }
        $(sameas_target_field_selector).val(source_value);

        // If we're dealing with the address field country select, trigger a change event.
        if (is_address_field && source_field.is('.country')) {
          // Re-enable all of the fields before triggering the ajax call.
          $(sameas_target_field_selector).removeAttr('disabled');
          var container = $(sameas_target_field_selector).parents('.webform-component-fieldset');
          container.find('.webform-sameas-target').removeAttr('disabled');

          // Trigger the AJAX callback for the address field.
          $(sameas_target_field_selector).change();
        }
      }
    }
  };

  /**
   * Copies all source fields to the target fields.
   */
  Drupal.webform_sameas.copy_all = function() {
    var sameas_enabled = $(this).is(':checked');

    if (sameas_enabled) {
      var controller_key = $(this).attr('class').match(/webform-sameas--[\w]+/)[0];
      var controller = Drupal.settings.webform_sameas.controllers[controller_key];
      var display_mode = controller.display_mode;
      if (display_mode != 'invisible') {
        // Get list of components to sync.
        for (var source_cid in controller.field_map) {
          var source_field = $('.webform-sameas-cid--' + source_cid);
          // Is field an addressfield?
          var source_classes = source_field.attr('class');
          var is_address_field = source_classes.match('webform-sameas-type--addressfield');
          if (is_address_field) {
          var source_country_field = $('.webform-sameas-cid--' + source_cid + '.country');;
            Drupal.webform_sameas.copy_single(null, source_country_field);
          }
          else {
            Drupal.webform_sameas.copy_single(null, source_field);
          }
        }
      }
    }
  }

  /**
   * Handles copying addressfield values after an ajax update.
   *
   * @param object event
   * @param object xhr
   * @param object settings
   */
  Drupal.webform_sameas.ajax_complete = function(event, xhr, settings) {
    var triggering_element = $(event.currentTarget.activeElement);
    var is_target_field = triggering_element.is('.webform-sameas-target');
    var is_addressfield = triggering_element.attr('class').match('webform-sameas-type--addressfield');

    // Make sure this ajax callback was for an addressfield and a target field.
    if (is_target_field && is_addressfield) {
      var controller_cid = triggering_element.attr('class').match(/(?:webform-sameas-controller-cid--)(\d+)/)[1];
      var controller = $('.webform-sameas-cid--' + controller_cid);
      var is_checked = controller.is(':checked');
      // Make sure the sameas checkbox for this field is checked.
      if (is_checked) {
        // Everything checks out, now we can do the copying.
        var source_fields = $('.webform-sameas-controller-cid--' + controller_cid + '.webform-sameas-source');
        source_fields.each(function() {
          // Don't copy the country field, but copy all others.
          if (!$(this).is('.country') && ($(this).is('input') || ($(this).is('select')))) {
            Drupal.webform_sameas.copy_single(null, $(this));
          }
        });

        // Rebind the state monitoring on the fields.
        controller.trigger('state:checked');
      }
    }
  }

  /**
   * Initialization function to bind form events to selected elements.
   *
   * @type {{attach: Function}}
   */
  Drupal.behaviors.webform_sameas = {
    attach: function (context, settings) {
      // Bind a click event to the sameas checkbox.
      $('.webform-sameas-controller').bind('click', Drupal.webform_sameas.copy_all);

      // Bind all source fields to copy their values to target fields.
      $('input.webform-sameas-source').blur(Drupal.webform_sameas.copy_single);
      $('select.webform-sameas-source ').change(Drupal.webform_sameas.copy_single);
      $( document ).ajaxSuccess(Drupal.webform_sameas.ajax_complete);
    }
  }

})(jQuery);
