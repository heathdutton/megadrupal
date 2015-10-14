/**
 * @file Behaviors for the front-end display of Salesforce-driven picklists.
 */

(function ($) {
  'use strict';

  /**
   * Defines the keys function if not provided by the browser.
   *
   * From https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/keys.
   */
  if (!Object.keys) {
    Object.keys = (function () {
      'use strict';
      var hasOwnProperty = Object.prototype.hasOwnProperty,
          hasDontEnumBug = !({toString: null}).propertyIsEnumerable('toString'),
          dontEnums = [
            'toString',
            'toLocaleString',
            'valueOf',
            'hasOwnProperty',
            'isPrototypeOf',
            'propertyIsEnumerable',
            'constructor'
          ],
          dontEnumsLength = dontEnums.length;

      return function (obj) {
        if (typeof obj !== 'object' && (typeof obj !== 'function' || obj === null)) {
          throw new TypeError('Object.keys called on non-object');
        }

        var result = [], prop, i;

        for (prop in obj) {
          if (hasOwnProperty.call(obj, prop)) {
            result.push(prop);
          }
        }

        if (hasDontEnumBug) {
          for (i = 0; i < dontEnumsLength; i++) {
            if (hasOwnProperty.call(obj, dontEnums[i])) {
              result.push(dontEnums[i]);
            }
          }
        }
        return result;
      };
    }());
  }

  /**
   * Attaches the show/hide logic for dependent piclists in webforms.
   *
   * @property {function} attach
   *   Adds handler to the forms to update all dependent picklists whenever
   *   a controlling picklist changes values.
   */
  Drupal.behaviors.salesforce_webforms = Drupal.behaviors.salesforce_webforms || {};

  Drupal.behaviors.salesforce_webforms.attach = function(context, settings) {
    // Dependent picklist logic.
    if (Drupal.settings.salesforce_webforms.salesforceMap) {
      Drupal.salesforce_webforms.salesforce(context, Drupal.settings.salesforce_webforms);
    }
  };

  Drupal.salesforce_webforms = Drupal.salesforce_webforms || {};

  /**
   * Attaches our handler to each form on the page.
   */
  Drupal.salesforce_webforms.salesforce = function(context, settings) {
    // Add the bindings to each webform on the page.
    $.each(settings.salesforceMap, function(formId, settings) {
      var $form = $('#' + formId + ':not(.webform-salesforce-processed)');
      if ($form.length) {
        $form.addClass('webform-salesforce-processed');

        $form.bind('change', { 'settings': settings }, Drupal.salesforce_webforms.salesforceCheck);

        // See if we have any dependent fields.
        for(var elementId in settings) {
          var children = [];
          var parent = null;
          var fieldname = settings[elementId].fieldname;
          var controlField = settings[elementId].control;
          for(var child in settings) {
            if(settings[child].control == fieldname) {
              children[children.length] = child;
            }

            if(settings[child].fieldname == controlField) {
              parent = child;
            }
          }

          settings[elementId].parent = parent;
          settings[elementId].children = children;
        }

        // Trigger all the elements that are driven by Salesforce picklists on
        // this form which have children, but no parents.
        $.each(settings, function(elementId) {
          if(settings[elementId].parent == null && settings[elementId].children.length > 0) {
            $('#' + elementId).find('select').filter(':first').trigger('change');
          }
        });
      }
    });
  };

  /**
   * Event handler to respond to field changes in a form.
   *
   * This event is bound to the entire form, not individual fields.
   */
  Drupal.salesforce_webforms.salesforceCheck = function(e) {
    var $div = $(e.target).parents('.sf-picklist-wrapper:first');
    var formId = $div.attr('id');
    var settings = e.data.settings;

    for (var i = 0; i < settings[formId].children.length; i++) {
      Drupal.salesforce_webforms.show_hide(settings[formId].children[i], settings);
    }
  };

  /**
   * Filters the list of valid choices in a dependent picklist.
   *
   * If the number of valid choices is zero, then the entire control is hidden.
   *
   * @param {string} idx
   *   The ID of the dependent item.
   * @param {object} settings
   *   The settings for this page.
   */
  Drupal.salesforce_webforms.show_hide = function(idx, settings){
    // Get current value.
    var $component = $('#' + idx);
    var $selectElement = $component.find("select");

    var curval = $selectElement.val();

    var options = Drupal.salesforce_webforms.salesforceGetPickList(idx, settings);
    var showComponent;

    if(Object.keys(options).length == 0) {
      // Hide this component.
      showComponent = false;
      curval = "";
    }
    else {
      // Show this component.
      showComponent = true;
    }

    // Flag variable.
    var $flag = $component.find('input[type="hidden"]');
    if (showComponent) {
      $flag.val('1');
      var $options = $component.find('select');

      // Keep the first option if this is not a multi-select.
      var multi = $options.attr("multiple");
      if (!multi) {
        // Get the first option to add back in.
        var emptyOpt = $options.find("option").first();
      }
      $options.find("option").remove();
      if (!multi) {
        $options.append(emptyOpt);
      }

      $.each(options, function (key, lbl) {
        $options.append($('<option>', { lbl : key })
            .text(lbl));
      });
      $component.find('select').removeAttr('disabled').removeClass('salesforce-webform-disabled').end().show();
      $component.find('select').focus();
    }
    else {
      $flag.val('0');
      $component.find('select').attr('disabled', true).val(null).addClass('salesforce-webform-disabled').end().hide();
    }

    // Select the old value if possible.
    $selectElement.val(curval);

    // And process any children of the newly updated field.
    var children = settings[idx].children;
    for(var i = 0; i < children.length; i++) {
      Drupal.salesforce_webforms.show_hide(children[i], settings);
    }
  }

  /**
   * Gets the list of currently valid options for a given select list.
   *
   * @param {string} element
   *   The ID of the dependent item.
   * @param {object} map
   *   The settings for this item.
   *
   * @return
   *   The options array of currently valid choices.
   */
  Drupal.salesforce_webforms.salesforceGetPickList = function(element, map) {
    var options = {};
    var controlFieldName = map[element].control;

    // First, figure out what DOM ID houses this field.
    var controlFieldId = null;
    for(var fieldId in map) {
      if(map[fieldId].fieldname == controlFieldName) {
        controlFieldId = fieldId;
      }
    }

    // Did we find a match?
    if(controlFieldId == null) {
      // Nope.
      return map[element].options;
    }

    var controlIndex = Drupal.salesforce_webforms.salesforceGetSelectedIndex(controlFieldId, map);

    // And determine which of our options apply to that position.
    var optionmap = [];
    optionmap[0] = -1;
    controlIndex--;
    for (var i = 0; controlIndex >= 0 && i < map[element]['full'].length; i++) {
      if(map[element].full[i].map[controlIndex]) {
        options[map[element].full[i].value] = map[element].full[i].label;
        // Store the map.
        optionmap[optionmap.length] = map[element]['full'][i].position;
      }
      else {
        map[element]['full'][i]['displayPosition'] = -1;
      }
    }

    map[element].options = options;
    map[element].optionmap = optionmap;

    return options;
  }

  /**
   * Gets the current value of the identified control.
   *
   * @param {string} element
   *   The ID of the dependent item.
   * @param {object} map
   *   The settings for this item.
   *
   * @return
   *   The current value.
   */
  Drupal.salesforce_webforms.salesforceGetSelectedIndex = function(el, map) {
    // Get the controlling container.
    var $elem = $('#' + el);

    // Now find the select control within that container.
    var $find = $elem.find('select');

    // Get the selected option.
    var idx = $find.prop ? $find.prop('selectedIndex') : $find.attr('selectedIndex');

    // Now map that back to the original order.
    var ret = idx == 0 ? -1 : map[el].optionmap[idx];
    return ret;
  }
})(jQuery);
