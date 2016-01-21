/**
 * @file
 * OpenTags AutoCombo widget.
 *
 * This widget combines auto-complete widget and combobox widget. It allows users
 * to select items from the browse list as well as gives suggestions when users
 * are typing into the textbox.
 *
 * Copyright (c) 2015 Systemik Solutions. All rights reserved.
 * http://www.systemiksolutions.com
 */

(function($){
  /**
   * Namespace openAglsUI.AutoCombo.
   *
   * Contains a set of properties and functions used by the AutoCombo widget.
   */
  OpenTagsUI.AutoCombo = {

    /**
     * Term index.
     *
     * A central object contains term objects of all AutoCombo widgets displayed
     * on the same page, which is keyed by widget's internal ID.
     *
     * Each term object tree of each widget is an 1 dimension array contains all
     * the available terms for the widget. Each item in the array is representing
     * a term, which is an object has the following properties:
     *
     *   - label: The display value of the term.
     *   - value: The value of the term.
     *   - disabled: The term is not selectable if it's true.
     *   - parent: Parent term's value.
     *   - isroot: If it's true, the term is a top level (level 1) term.
     *   - ancestor: The top level ancestor term of the term. If it is a top level.
     *     term, the ancestor would be itself.
     *   - haschild: Has child terms if it's true.
     *   - level: Integer. The term's level in the tree structure.
     */
    termIndex: {},

    /**
     * A central object contains term lookups, which is keyed by widget's internal ID.
     *
     * Each lookup of each widget is an object has the following lookups:
     *
     *   - valueToLabel: an object contains term values as properties where the
     *     property value is the term label.
     *   - labelToValue: an object contains term labels as properties where the
     *     property value is the term value.
     */
    termLookup: {},

    /**
     * Settings Object.
     *
     * A central object contains the settings of all the widgets, which is keyed
     * by widget's internal ID.
     *
     * Each setting of each widget is an object has the following keys:
     *
     *   - delimiter: the delimiter used to concatenate widgets return values.
     *   - colorfulTags: Use random colors for tags.
     *   - single: only allows single value.
     */
    settings: {},

    /**
     * Initiate the widget.
     *
     * Apply the default settings and store the widget setting into
     * 'settings' property.
     *
     * @param string key
     *   The internal ID of the widget.
     * @param object settings
     *   The settings object defined when create the widget.
     */
    init: function(key, settings) {
      var config = {};
      // Apply default settings.
      config['delimiter'] = !settings['delimiter'] ? ';' : settings['delimiter'];
      if (settings['colorfulTags']) {
        config['colorfulTags'] = true;
      }
      if (settings['single']) {
        config['single'] = true;
      }

      // Initiate settings.
      this.settings[key] = config;
    },

    /**
     * Create term index array.
     *
     * Build the term index array from the nested term structure(items) defined
     * when create the widget. Then store the term index into 'termIndex' property.
     *
     * @param array data
     *   The nested term tree structure array.
     * @param string key
     *   The internal ID of the widget.
     */
    createTermIndex: function(data, key) {
      var addChildrenTerms = function(parentTerm, termLevel, termsData, ancestorTerm) {
        $.each(termsData, function(i, value) {
          var term = {};
          term['label'] = value['label'];
          term['value'] = value['value'];
          if (value['disabled']) {
            term['disabled'] = true;
          }
          if (parentTerm) {
            term['parent'] = parentTerm['value'];
          }
          if (!ancestorTerm || termLevel == 1) {
            ancestorTerm = value['value'];
            term['isroot'] = true;
          }
          term['ancestor'] = ancestorTerm;
          if (value['items']) {
            term['haschild'] = true;
          }
          term['level'] = termLevel;
          termIndex.push(term);
          // Add children terms recursively.
          if (value['items']) {
            addChildrenTerms(value, termLevel + 1, value['items'], ancestorTerm);
          }
        });
      };
      var termIndex = [];
      addChildrenTerms(null, 1, data, null);
      this.termIndex[key] = termIndex;
    },

    /**
     * Create term value to label lookup object and store it into 'termLookup' property.
     *
     * @param string key
     *   The internal ID of the widget.
     */
    createValueToLabelLookup: function(key) {
      var termIndex = this.termIndex[key];
      var lookup = {};
      for (var i in termIndex) {
        var label = termIndex[i].label;
        var value = termIndex[i].value;
        if (!lookup[value]) {
          lookup[value] = label;
        }
      }
      if (!this.termLookup[key]) {
        this.termLookup[key] = {};
      }
      this.termLookup[key]['valueToLabel'] = lookup;
    },

    /**
     * Create term label to value lookup object and store it into 'termLookup' property.
     *
     * @param string key
     *   The internal ID of the widget.
     */
    createLabelToValueLookup: function(key) {
      var termIndex = this.termIndex[key];
      var lookup = {};
      for (var i in termIndex) {
        var label = termIndex[i].label;
        var value = termIndex[i].value;
        if (!lookup[label]) {
          lookup[label] = value;
        }
      }
      if (!this.termLookup[key]) {
        this.termLookup[key] = {};
      }
      this.termLookup[key]['labelToValue'] = lookup;
    },

    /**
     * Create and display a level of the browse list.
     *
     * @param string controlId
     *   The internal ID of the widget.
     * @param integer level
     *   The browse list level.
     * @param string parentValue
     *   The parent term value in the upper level of the list.
     */
    createTermList: function(controlId, level, parentValue) {
      var containerSeletor = '#otu-' + controlId + '-container';
      $(containerSeletor + ' .otu-autocombo-list').each(function () {
        if ($(this).data('listlevel') >= level) {
          $(this).remove();
        }
      });

      var filters = {};
      filters.level = level;
      if (parentValue) {
        filters.parent = parentValue;
      }
      var filteredIndex = this.filterTermIndex(filters, this.termIndex[controlId]);

      if (filteredIndex.length > 0) {
        var listElement = $('<div class="otu-autocombo-list"></div>');
        listElement.appendTo(containerSeletor).data('listlevel', level);

        $.each(filteredIndex, function(i, value) {
          var itemClass = "otu-autocombo-list-item";
          if (value.disabled) {
            itemClass += " otu-autocombo-list-item-disabled";
          }
          var itemElement = $('<span title="' + OpenTagsUI.Common.htmlEncode(value.label) + '" class="' + itemClass + '">' + OpenTagsUI.Common.htmlEncode(value.label) + '</span>');
          itemElement.appendTo(listElement).data('termInfo',{level:level, value:value.value});
          if (value['haschild']) {
            itemElement.addClass('otu-autocombo-list-item-group');
          }
        });
        $('#' + controlId + ' .otu-autocombo-list-item').unbind('click').click(this.event.itemClickHandler);
        $('#' + controlId + ' .otu-autocombo-list-item').unbind('dblclick').dblclick(this.event.itemDoubleClickHandler);
      }
    },

    /**
     * Filter the term index based on the filtering criteria provided.
     *
     * @param object filters
     *   The filtering criteria. It's an object keyed by the properties of term
     *   item object. The values specified for the properties are used to filter
     *   the term index array.
     * @param array termIndex
     *   The term index array.
     *
     * @returns array
     *   An array of filtered terms.
     */
    filterTermIndex: function(filters, termIndex) {
      var filterNames = Object.keys(filters);
      var filteredTerms = [];
      for (var i in termIndex) {
        var term = termIndex[i];
        var flag = true;
        for (var j in filterNames) {
          if (!term[filterNames[j]] || term[filterNames[j]] != filters[filterNames[j]]) {
            flag = false;
            break;
          }
        }
        if (flag) {
          filteredTerms.push(term);
        }
      }
      return filteredTerms;
    },

    /**
     * Search the term labels based on the keywords in the term index.
     *
     * @param string searchText
     *   The text used for the search.
     *   Note: the search text will be treated as a word. So characters appear in
     *   the middle of the word won't be matched. eg. 'cat' will match 'category',
     *   'catalog', won't match 'bobcat'.
     * @param array termIndex
     *   The term index array.
     *
     * @returns array
     *   An array of matched terms.
     */
    searchTermIndex: function(searchText, termIndex) {
      var filteredTerms = [];
      var pattern = '(^|\\s)' + searchText.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
      var regx = new RegExp(pattern, "i");
      for (var i in termIndex) {
        var term = termIndex[i];
        if (term['label'].match(regx)) {
          filteredTerms.push(term);
        }
      }
      return filteredTerms;
    },

    /**
     * Sort the term array alphabetically.
     *
     * @param array termIndex
     *   The term array to be sorted.
     *
     * @returns array
     *   The sorted term array.
     */
    sortTermAlphabetically: function(termIndex) {
      var sortAlphabet = function(a, b) {
        var aName = a.label.toLowerCase();
        var bName = b.label.toLowerCase();
        return ((aName < bName) ? -1 : ((aName > bName) ? 1 : 0));
      }
      termIndex.sort(sortAlphabet);
      return termIndex;
    },

    /**
     * Create an array of terms with ancestor terms (top level parent) in it.
     *
     * This function will add the ancestor terms into the term array to form an
     * hierarchy list of terms.
     *
     * @param array termIndex
     *   The term array used to add the ancestor terms.
     * @param string controlId
     *   The internal ID of the widget.
     *
     * @returns array
     *   The term array with ancester terms.
     */
    buildAncesterTree: function(termIndex, controlId) {
      var ancestorTree = {};
      for (var i in termIndex) {
        if (!ancestorTree[termIndex[i].ancestor]) {
          ancestorTree[termIndex[i].ancestor] = [];
        }
        if (!termIndex[i].isroot) {
          ancestorTree[termIndex[i].ancestor].push(termIndex[i]);
        }
      }
      // All root terms.
      var ancestorTerms = [];
      for (var i in this.termIndex[controlId]) {
        if (this.termIndex[controlId][i].isroot) {
          ancestorTerms.push(this.termIndex[controlId][i]);
        }
      }
      var ancestorTreeTermIndex = [];
      for (var i in Object.keys(ancestorTree)) {
        var key = Object.keys(ancestorTree)[i];
        if (ancestorTree[key].length > 0) {
          for (var j in ancestorTerms) {
            if (ancestorTerms[j].value == key) {
              ancestorTreeTermIndex.push(ancestorTerms[j]);
              break;
            }
          }
          $.merge(ancestorTreeTermIndex, this.sortTermAlphabetically(ancestorTree[key]));
        }
        else {
          var ancestorTerm;
          for (var j in ancestorTerms) {
            if (ancestorTerms[j].value == key) {
              ancestorTerm = ancestorTerms[j];
              break;
            }
          }
          if (ancestorTerm && !ancestorTerm.disabled) {
            ancestorTreeTermIndex.push(ancestorTerm);
          }
        }
      }
      return ancestorTreeTermIndex;
    },

    /**
     * Create and display the suggest list (auto-complete).
     *
     * @param jQuery suggestListElement
     *   The suggest list containter div element.
     * @param string value
     *   The value typed into the textbox.
     */
    createSuggestList: function(suggestListElement, value) {
      suggestListElement.children().remove();
      var suggestTerms = this.searchTermIndex(value, this.termIndex[suggestListElement.parents().eq(2).attr('id')]);
      suggestTerms = this.buildAncesterTree(suggestTerms, suggestListElement.parents().eq(2).attr('id'));
      if (suggestTerms.length > 0) {
        for (var i in suggestTerms) {
          var itemClass = "otu-autocombo-suggest-list-item";
          if (suggestTerms[i].disabled) {
            itemClass += " otu-autocombo-suggest-list-item-disabled";
          }
          var itemElement = $('<span title="' + OpenTagsUI.Common.htmlEncode(suggestTerms[i].label) + '" class="' + itemClass + '">' + OpenTagsUI.Common.htmlEncode(suggestTerms[i].label) + '</span>');
          // Add term value.
          itemElement.data('termInfo', {value: suggestTerms[i].value});
          itemElement.appendTo(suggestListElement);
        }
        suggestListElement.children().click(this.event.suggestListItemClickHandler);
      }
    },

    /**
     * Check if the term is already entered.
     *
     * @param jQuery inputElement
     *   The input textbox element.
     * @param string termValue
     *   The value of the term to check.
     *
     * @returns boolean
     *   Return true if the term is already entered; otherwise, false.
     */
    tagExist: function(inputElement, termValue) {
      var existingTags = [];
      inputElement.parent().parent().children('.otu-autocombo-input-item').each(function(index, element) {
        existingTags.push($(element).data('value'));
      });
      if ($.inArray(termValue, existingTags) > -1) {
        return true;
      }
      else {
        return false;
      }
    },

    /**
     * Check if a provided term label is a valid term in the term index array.
     *
     * @param jQuery inputElement
     *   The input textbox element.
     * @param string termLabel
     *   The term label to be checked.
     *
     * @returns object
     *   Retruns corresponding term object from term index array if the label is
     *   valid; otherwise, returns null.
     */
    tagValid: function(inputElement, termLabel) {
      var termIndexKey = this.getInternalID(inputElement);
      for (var i = 0; i < this.termIndex[termIndexKey].length; i++) {
        if (termLabel.toLowerCase() == this.termIndex[termIndexKey][i]['label'].toLowerCase()) {
          return this.termIndex[termIndexKey][i];
        }
      }
    },

    /**
     * Create and display a tag into the input area.
     *
     * @param jQuery inputElement
     *   The input textbox element.
     * @param string termLabel
     *   The label to be displayed for the tag.
     * @param string termValue
     *   The value to be stored in the tag.
     */
    createDisplayTag: function(inputElement, termLabel, termValue) {
      if (!this.tagExist(inputElement, termValue)) {
        // If it's a single value field clear all existing tags.
        var controlId = this.getInternalID(inputElement);
        if (this.settings[controlId]['single']) {
          this.clearAllTags(controlId);
        }

        var closeIconElement = $('<span class="otu-autocombo-input-item-close" title="Delete"></span>');
        var inputItemElement = $('<span class="otu-autocombo-input-item">' + termLabel + '</span>');
        // Add data value.
        inputItemElement.data('value', termValue);
        inputItemElement.data('label', termLabel);
        // Drop random color for now, will discuss later.
        var iid = this.getInternalID(inputElement);
        if (this.settings[iid]['colorfulTags']) {
          this.randomApplyBackgroundColor(inputItemElement, inputElement.parent().prev().css('background-color'));
        }
        inputItemElement.insertBefore(inputElement.parent()).append(closeIconElement);
        // Add click handler.
        closeIconElement.click(function() {
          $(this).parent().remove();
        });
      }
    },

    /**
     * Apply an random color for a tag element.
     *
     * @param jQuery element
     *   The tag element.
     * @param string excludeColorCode
     *   The color which will be excluded when generate the random color. The color
     *   code should be in the rgb format, eg. rgb(0, 0, 0).
     */
    randomApplyBackgroundColor: function(element, excludeColorCode) {
      var tagColors = ['rgb(96, 73, 122)', 'rgb(54, 96, 146)', 'rgb(118, 147, 60)', 'rgb(150, 54, 52)', 'rgb(148, 138, 84)'];
      var delta = Math.floor((Math.random() * 5) + 1) - 1;
      if (excludeColorCode) {
        while (tagColors[delta] == excludeColorCode) {
          delta = Math.floor((Math.random() * 5) + 1) - 1;
        }
      }
      element.css('color', '#ffffff');
      element.css('background-color', tagColors[delta]);
    },

    /**
     * Get widget internal ID by providing any jQuery element inside the widget container.
     *
     * @param jQuery element
     *   An element inside the widget container.
     *
     * @returns string
     *   The internal ID of the widget
     */
    getInternalID: function(element) {
      var topElement = element.closest('.otu-autocombo');
      return topElement.attr('id');
    },

    /**
     * Clear all tags in the input area.
     *
     * @param string controlId
     *   The internal ID of the widget.
     */
    clearAllTags: function(controlId) {
      $('#' + controlId + ' .otu-autocombo-input-item').remove();
    },

    /**
     * Namespace openAglsUI.AutoCombo.event.
     *
     * Contains event handlers of the widget.
     */
    event: {

      /**
       * Handler when click an item in the browse list.
       */
      itemClickHandler: function() {
        $(this).siblings().removeClass('otu-autocombo-list-item-highlighted');
        $(this).addClass('otu-autocombo-list-item-highlighted');
        OpenTagsUI.AutoCombo.createTermList($(this).parents().eq(2).attr('id'), $(this).data('termInfo')['level'] + 1, $(this).data('termInfo')['value']);
      },

      /**
       * Handler when double click an item in the browse list.
       */
      itemDoubleClickHandler: function() {
        if (!$(this).hasClass('otu-autocombo-list-item-disabled')) {
          var inputElement = $(this).parent().parent().parent().find('input');
          OpenTagsUI.AutoCombo.createDisplayTag(inputElement, $(this).html(), $(this).data('termInfo').value);
        }
      },

      /**
       * Handler for auto completion.
       *
       * Add listeners for the input textbox to respond different types of key
       * stroks.
       */
      autoCompleteListener: function(e) {
        var listContainerElement = $(this).next();
        // Press Esc or Backspace.
        if (e.keyCode == 27 || e.keyCode == 8) {
          listContainerElement.hide();
        }
        // Press Enter.
        else if (e.keyCode == 13) {
          var term = OpenTagsUI.AutoCombo.tagValid($(this), $(this).val());
          if (term) {
            OpenTagsUI.AutoCombo.createDisplayTag($(this), OpenTagsUI.Common.htmlEncode(term.label), term.value);
            $(this).val('');
            $(this).focus();
            listContainerElement.hide();
          }
        }
        else if ((e.keyCode > 47 && e.keyCode < 91) || (e.keyCode > 95 && e.keyCode < 112) || (e.keyCode > 185 && e.keyCode < 222)) {
          // Hide full list.
          var browseButtonElement = $(this).parents().eq(1).next();
          if (browseButtonElement.html() == 'Hide') {
            browseButtonElement.click();
          }

          $(this).parents().eq(3).css('overflow', '');

          OpenTagsUI.AutoCombo.createSuggestList(listContainerElement, $(this).val());
          if (listContainerElement.children().length > 0) {
            listContainerElement.show();
          }
          else {
            listContainerElement.hide();
          }
        }
      },

      /**
       * Handler when click an item in the auto completion list.
       */
      suggestListItemClickHandler: function() {
        if (!$(this).hasClass('otu-autocombo-suggest-list-item-disabled')) {
          var inputElement = $(this).parent().parent().find('input');
          inputElement.val('');
          OpenTagsUI.AutoCombo.createDisplayTag(inputElement, $(this).html(), $(this).data('termInfo').value);
          $(this).parent().hide();
          inputElement.focus();
        }
      },

      /**
       * Handler when click the browse button.
       */
      browseClickHandler: function() {
        if ($(this).html() == 'Browse') {
          $(this).html('Hide');
        }
        else {
          $(this).html('Browse');
        }
        $(this).next().toggle();
      },

      /**
       * Handler when click the text box area.
       */
      inputBoxClickHandler: function() {
        $(this).find('input').focus();
      }
    }
  };

  /**
   * Implement Auto Combo jQuery plugin.
   *
   * @param string option
   *   The function name to excute.
   * @param mix param
   *   Parameter to pass to the function.
   *
   * Available functions:
   *
   * - init: Create the Auto Combo widget.
   *
   *   Prameter: an object contains configure information:
   *     * name: Required. The name of the widget, displayed as labels.
   *     * id: Required. An unique identifier assigned to the widget.
   *     * items: Required. An array contains a list of items to be displayed
   *       in the widget. Each item is an object contains following keys:
   *         - label: Required. The display value of the item.
   *         - value: Required. Value of the item.
   *         - disabled: true or false. If an item is disabled, then it's only
   *           for dispalying and user can't select it.
   *         - items: An array of children items.
   *     * delimiter: used for concatenate the result values. Default is semi-colon(;)
   *     * colorfulTags: true or false. Use random colors for tags.
   *     * default: An array of default values. Each item should be a valid default
   *       term value(not label).
   *     * width: String. The width of the widget with the unit. eg. 200px. The
   *       default is 600px;
   *     * single: true or false. Only allows a single value if set to true. Default
   *       is false.
   *
   * - value: Get Auto Combo widget value.
   */

  $.fn.openTagsAutoCombo = function(option, param) {
    switch (option) {
      // Function .openTagsAutoCombo('init', settings).
      case 'init':
        if (param != undefined) {
          var config = param;
          var controlId = 'otu-autocombo-id-' + config.id;
          // Initiate settings.
          OpenTagsUI.AutoCombo.init(controlId, config);
          // Organise term table.
          OpenTagsUI.AutoCombo.createTermIndex(config['items'], controlId);
          // Create term lookups.
          OpenTagsUI.AutoCombo.createValueToLabelLookup(controlId);
          OpenTagsUI.AutoCombo.createLabelToValueLookup(controlId);
          // Create widget wrapper.
          this.wrap('<div id="' + controlId + '" class="otu-autocombo"></div>');
          // Create browse list.
          var listContainerElement = $('<div id="otu-' + controlId + '-container"></div>');
          this.parent().append(listContainerElement);
          listContainerElement.hide();
          OpenTagsUI.AutoCombo.createTermList(controlId, 1, null);
          // Add autocomplete listener.
          this.keyup(OpenTagsUI.AutoCombo.event.autoCompleteListener);
          // Add browse button.
          var browseButtonElement = $('<div class="otu-autocombo-input-browse">Browse</div>');
          this.after(browseButtonElement);
          browseButtonElement.click(OpenTagsUI.AutoCombo.event.browseClickHandler);
          // Create text box area.
          this.wrap('<div class="otu-autocombo-input"></div>');
          this.parent().click(OpenTagsUI.AutoCombo.event.inputBoxClickHandler);
          this.addClass('otu-autocombo-input-box');
          // Add autocomplete list.
          this.wrap('<div class="otu-autocombo-input-wrapper"></div>');
          var suggestListElement = $('<div class="otu-autocombo-suggest-list"></div>');
          this.parent().append(suggestListElement);
          suggestListElement.hide();
          // Create name label.
          $('#' + controlId).prepend('<span class="otu-autocombo-title">' + config.name + '</span>');
          // Turn off autocompletion from the browser.
          this.attr('autocomplete', 'false');
          // Add name to the input element to avoid browser autofill.
          this.attr('name', 'otu-autocombo-name-' + config.id);
          // Add default value.
          if (config['default']) {
            for (var i in config['default']) {
              var item = config['default'][i];
              if (OpenTagsUI.AutoCombo.termLookup[controlId]['valueToLabel'][item]) {
                var labelHtml = OpenTagsUI.Common.htmlEncode(OpenTagsUI.AutoCombo.termLookup[controlId]['valueToLabel'][item]);
                OpenTagsUI.AutoCombo.createDisplayTag(this, labelHtml, item);
              }
            }
          }
          // Adjust widget width.
          if (config['width']) {
            $('#' + controlId + ' .otu-autocombo-input').css('width', config['width']);
          }

          // Hide browse list when click outside the area.
          $('html').click(function(e) {
            var rootSelector = '#' + controlId;
            var target = $(e.target);
            if (!target.is(rootSelector + ' .otu-autocombo-input-browse') && !target.is(rootSelector + ' .otu-autocombo-list') && !target.is(rootSelector + ' .otu-autocombo-list-item')) {
              var browseButtonElement = $(rootSelector + ' .otu-autocombo-input-browse');
              if (browseButtonElement.html() == 'Hide') {
                browseButtonElement.click();
              }
            }
          });
        }
        break;

      // Function .openTagsAutoCombo('value').
      case 'value':
        var internalID = OpenTagsUI.AutoCombo.getInternalID(this);
        var values = [];
        this.parent().parent().children('.otu-autocombo-input-item').each(function(index, element) {
          values.push($(element).data('value'));
        });
        return values.join(OpenTagsUI.AutoCombo.settings[internalID]['delimiter']);

      default:
        console.log('Unkown command.');
    }
  };

}(jQuery));
