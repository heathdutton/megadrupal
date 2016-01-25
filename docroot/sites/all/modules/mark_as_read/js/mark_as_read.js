/**
 * @file
 * Javascript for all traversal and actions.
 */

/**
 * Performs Binary Search on Sorted Javascript Array. 
 * 
 * This is actually doing the Binary Search on the sorted array. Extending the
 * Array object with 'prototype' created an TypeError. So created the global
 * function BinarySearch.
 */
window.binarySearch = function(array, searchElement) {
  var minIndex = 0;
  var maxIndex = array.length - 1;
  var currentIndex;
  var currentElement;

  while (minIndex <= maxIndex) {
    currentIndex = (minIndex + maxIndex) / 2 | 0;
    currentElement = array[currentIndex];

    if (currentElement < searchElement) {
      minIndex = currentIndex + 1;
    } else if (currentElement > searchElement) {
      maxIndex = currentIndex - 1;
    } else {
      return true;
    }
  }
  return false;
};

(function($) {
  // Namespace as MarkAsRead.
  Drupal.MarkAsRead = Drupal.MarkAsRead || {};

  Drupal.behaviors.mark_as_read = {
    attach : function(context, settings) {
      Drupal.MarkAsRead.ListsFacade().init(settings);
    }
  };

  Drupal.MarkAsRead.ListsFacade = function() {
    var getActiveLists = function(rawListsString) {
      var activeLists = [];
      var lists = Drupal.MarkAsRead.ListsAdapter().lists(rawListsString);
      for (var i in lists) {
        if ($(lists[i].getCssSelector()).size() > 0) {
          activeLists.push(lists[i]);
        }
      }
      return activeLists;
    };
    var iterateLists = function(activeLists) {
      for (var i in activeLists) {
        new Drupal.MarkAsRead.ListIterator(activeLists[i]);
      }
    };

    var init = function(drupalSettings) {
      var $lists_json = drupalSettings.MarkAsRead.allListsjson;
      var $list = getActiveLists($lists_json);
      iterateLists($list);
    };

    return {
      init : init
    };
  };

  /**
   * Iterates over the css selector of the List over the DOM and adds the class
   */
  Drupal.MarkAsRead.ListIterator = function(list) {
    var cssSelector = list.getCssSelector();
    var attributeValues = list.getAttributeValues();
    var attributeName = list.getAttributeName();

    this.iterate = (function() {
      /*
       * @see has_attribute_selector
       *  If none of the list item in the given Css
       *  selector has provided attribute name then, no need to go further.
       */
      if ($(cssSelector + "[" + attributeName + "]").size() <= 0) {
        throw new Error("mark_as_read: None of the list items '" + cssSelector + "' has attribute '" + attributeName + "'");
      }
      $(cssSelector).each(
        function() {
          var attributeValueOfCurrentSelection = $(this).attr(attributeName);
          if (attributeValues.length != 0
              && binarySearch(attributeValues,
                  attributeValueOfCurrentSelection)) {
            new Drupal.MarkAsRead.ReadElementHandler($(this), list);
          } else {
            new Drupal.MarkAsRead.UnReadElementHandler($(this), list);
          }
      });
    })();
  };

  /**
   * This object handles the each list item based on if already visited or not.
   * 
   * @param el
   *   Jquery Element Object.
   * @param list
   *   ListActivity Object.
   */
  Drupal.MarkAsRead.ElementHandler = function(el, list) {
    this.el = el;
    this.list = list;
    this.readClass = Drupal.settings.MarkAsRead.markedAsReadClass;
    this.unreadClass = Drupal.settings.MarkAsRead.markedAsUnReadClass;
  };
  /**
   * Don't assign click event on those elements which are already visited.
   *
   * @param el
   *   Jquery Element Object.
   * @param list
   *   List Object.
   * 
   */
  Drupal.MarkAsRead.ReadElementHandler = function(el, list) {
    Drupal.MarkAsRead.ElementHandler.call(this, el, list);
    el.addClass(this.readClass);
    $(document).trigger('MarkAsRead.ReadClicked', [ el ]);
  };

  /**
   * Handler for elements , which are in unread state
   * 
   * @param el
   *   JQuery Element Object.
   * @param list
   *   List Object.
   */
  Drupal.MarkAsRead.UnReadElementHandler = function(el, list) {
    this.el = el;
    // Call parents constructor.
    Drupal.MarkAsRead.ElementHandler.call(this, el, list);
    el.addClass(this.unreadClass);
    var self = this;
    // Bind click event.
    el.click(function() {
      if ($(this).hasClass(self.unreadClass)) {
        self._onClick();
      }
    });

    // Click handler when unread element is clicked.
    this._onClick = function() {
      self.setUnBold();
      self.changeClass();
      $(document).trigger('MarkAsRead.UnReadClicked', [ el ]);
      this.saveData();
      // TODO rollback if problem in ajax request.
    };

    // Adds corresponding class
    this.changeClass = function() {
      this.el.removeClass(this.unreadClass).addClass(this.readClass);
    };
    // Set the element bold if selected on the Admin.
    this.setBold = function() {
      if (Drupal.settings.MarkAsRead.setBold) {
        this.el.css('font-weight', 'bold');
      };
    };
    this.setUnBold = function() {
      if (this.el.hasClass(this.unreadClass)) {
        this.el.css('font-weight', 'normal');
      };
    };

    // Save the data and trigger the exposed events.
    this.saveData = function() {
      var attributeValue = this.el.attr(this.list.getAttributeName());
      Drupal.MarkAsRead.Ajax().saveListItemAttributeValue(
        this.list.getListId(),
        attributeValue,
        function() {
          $(document).trigger('MarkAsRead.UnReadClickedAjaxComplete',[ el ]);
        }
      );
    };
    // Set bold by default.
    this.setBold();
  };

  /**
   * Individual List Object with activity of currently Logged In User.
   * 
   * For eg. Activity on "MainMenu" of currently logged in user.
   * 
   * @param listId
   *   Unique List Id.
   * @param cssSelector
   *   HTML css selector that represents all clickable element.
   * @param attributeName
   *   Unique attribute name that identifies elements uniquely.
   * @param attributeValues
   *   Value of the attribute provided above.
   */
  Drupal.MarkAsRead.ListActivity = function(listId, cssSelector, attributeName,
      attributeValues) {
    var cssSelector = cssSelector;
    var listId = listId;
    var attributeName = attributeName;
    var attributeValues = attributeValues;

    this.getCssSelector = function() {
      return cssSelector;
    };

    this.getListId = function() {
      return listId;
    };

    this.getAttributeName = function() {
      return attributeName;
    };

    // Returns the sorted array of attributeValues;
    this.getAttributeValues = function() {
      if (!attributeValues instanceof Array) {
        throw Error('MarkAsRead: AttributeValue is not an Array');
      }
      return attributeValues.sort();
    };

    this.addAttributeValue = function(value) {
      if (!attributeValues || attributeValues.length == 0) {
        attributeValues = [];
      }
      attributeValues.push(value);
    };

  };

  // Ajax Operations.
  Drupal.MarkAsRead.Ajax = function() {
    var baseUrl = Drupal.settings.basePath;
    var token = Drupal.settings.MarkAsRead.token;
    // Save the user activity(if clicked) in the server.
    var saveListItemAttributeValue = function(listId, attributeValue, callback) {
      $.ajax({
        url : baseUrl + "mark_as_read/ajax/add/attribute",
        data : {
          listId : listId,
          attributeValue : attributeValue,
          token: token
        },
        /*
         * Stop all the browser activities while request is being made.
         * Async:false is used to make sure the ajax request is complete and we
         * have our data in the server.
         */
        async : false,
        type : 'POST',
        success : function() {
          callback();
        }
      });
    };

    return {
      saveListItemAttributeValue : saveListItemAttributeValue
    };
  };

  /**
   * This converts the raw json string into the List Object.
   */
  Drupal.MarkAsRead.ListsAdapter = function() {
    var lists = function(string) {
      // Array of individual list objects.
      var lists = [];
      var jsonData = jQuery.parseJSON(string);
      $.each(jsonData, function($key, $obj) {
        if (!$obj.attribute_values) {
          $obj.attribute_values = new Array();
        }
        lists.push(new Drupal.MarkAsRead.ListActivity($obj.list_id,
          $obj.css_selector, $obj.attribute_name, $obj.attribute_values));
      });
      return lists;
    };

    return {
      'lists' : lists
    };
  };
})(jQuery);
