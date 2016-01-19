/**
 * @file
 * OpenTags DropDown widget.
 *
 * OpenTags DropDown provides a traditional drop down list, with flexibility of
 * custom data source and appearance.
 *
 * Copyright (c) 2015 Systemik Solutions. All rights reserved.
 * http://www.systemiksolutions.com
 */

(function($) {
  /**
   * Namespace openAglsUI.DropDown.
   *
   * Contains a set of properties and functions used by the DropDown widget.
   */
  OpenTagsUI.DropDown = {
    init: function() {
      // Initiate.
    },

    /**
     * Get the item label by providing the item value.
     *
     * @param string value
     *   The item value to be converted.
     * @param array items
     *   The array contains all items.
     *
     * @returns string
     *   The corresponding label of the item if the value is found; otherwise,
     *   returns null.
     */
    itemValueToLabel: function(value, items) {
      for (var i in items) {
        if (value == items[i].value) {
          return items[i].label;
          break;
        }
      }
    },

    /**
     * Get the item value by providing the item label.
     *
     * @param string label
     *   The item label to be converted.
     * @param array items
     *   The array contains all items.
     *
     * @returns string
     *   The corresponding value of the item if the label is found; otherwise,
     *   returns null.
     */
    itemLabelToValue: function(label, items) {
      for (var i in items) {
        if (label == items[i].label) {
          return items[i].value;
          break;
        }
      }
    },

    /**
     * Namespace openAglsUI.DropDown.event.
     *
     * Contains event handlers of the widget.
     */
    event: {

      /**
       * Handler when click drop down arrow.
       */
      dropDownClickHandler: function(event) {
        event.stopPropagation();
        $(this).parents().eq(1).css('overflow', '');
        $(this).next().toggle();
      },

      /**
       * Handler when select an item in the drop down list.
       */
      listItemClickHandler: function(event) {
        event.stopPropagation();
        $(this).parent().parent().find('.otu-dropdownlist-text').html($(this).html());
        $(this).parent().parent().find('.otu-dropdownlist-text').attr('title', $(this).html());
        $(this).parent().parent().find('select').val($(this).data('value'));
        $(this).parent().hide();
      }
    }
  };

  /**
   * Implement OpenTags Dropdown jQuery plugin.
   *
   * @param string option
   *   The function name to excute.
   * @param mix param
   *   Parameter to pass to the function.
   *
   * Available functions:
   *
   * - init: Create the Dropdown widget.
   *
   *   Parameter: an object contains configure information:
   *     * name: Required. The name of the widget, displayed as labels.
   *     * id: Required. An unique identifier assigned to the widget.
   *     * items: Required. An array contains a list of items to be displayed in
   *       the dropdown list. Each item is an object contains following keys:
   *         - label: Required. The display value of the item.
   *         - value: Required. Value of the item.
   *     * default: String. The default value of the widget (not label).
   *     * width: String. The width of the widget with the unit. eg. 200px. The
   *       default is 250px;
   *
   * - value: Get widget value.
   */
  $.fn.openTagsDropdown = function(option, param) {
    switch (option) {
      // Function .openTagsDropdown('init', param).
      case 'init':
        if (param != undefined) {
          var config = param;
          var controlId = 'otu-dropdownlist-id-' + config.id;
          this.wrap('<div id="' + controlId + '" class="otu-dropdownlist-container"></div>');
          this.append('<option value=""></option>');
          for (var i in config.items) {
            var item = config.items[i];
            this.append('<option value="' + item.value + '">' + item.label + '</option>');
          }
          this.parent().append('<span class="otu-dropdownlist-title">' + config.name + '</span>');

          // Calculate width.
          var widgetWidth = config['width'] ? config['width'] : '250px';

          this.parent().append('<span class="otu-dropdownlist" style="width:' + widgetWidth + '"></span>');
          this.parent().children().last().append('<span class="otu-dropdownlist-arrow"></span>');
          this.parent().children().last().append('<span class="otu-dropdownlist-text">Select...</span>');

          var listElement = $('<span class="otu-dropdownlist-list" style="width:' + widgetWidth + '"></span>');
          this.parent().append(listElement);
          listElement.hide();

          for (var i in config.items) {
            var listItemElement = $('<span class="otu-dropdownlist-list-item" title="' + OpenTagsUI.Common.htmlEncode(config.items[i].label) + '">' + OpenTagsUI.Common.htmlEncode(config.items[i].label) + '</span>');
            listItemElement.data('value', config.items[i].value);
            $('#' + controlId + ' .otu-dropdownlist-list').append(listItemElement);
          }

          if (config['default']) {
            var label = OpenTagsUI.DropDown.itemValueToLabel(config['default'], config.items);
            if (label) {
              this.val(config['default']);
              $('#' + controlId + ' .otu-dropdownlist-text').html(label);
            }
          }
          else {
            this.val('');
          }
          this.hide();

          $('#' + controlId + ' .otu-dropdownlist').click(OpenTagsUI.DropDown.event.dropDownClickHandler);

          $('#' + controlId + ' .otu-dropdownlist-list-item').click(OpenTagsUI.DropDown.event.listItemClickHandler);

          $('html').click(function() {
            $('#' + controlId + ' .otu-dropdownlist-list').hide();
          });
        }
        break;

      // Function .openTagsDropdown('value').
      case 'value':
        return this.parent().find('select').val();

      default:
        console.log('Unkown command.');
    }

  };

}(jQuery));
