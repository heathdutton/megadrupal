/**
 * @file
 * uniqueness.js - JavaScript for the uniqueness width
 */

(function($) {
  var uniqueness;
  Drupal.behaviors.uniqueness = {
    attach: function (context) {
      if (!uniqueness) {
        uniqueness = new Drupal.uniqueness(Drupal.settings.uniqueness['URL'], $('.uniqueness-dyn'));
      }
      // Search off title.
      $('#edit-title', context).once('uniqueness', function() {
        $(this).keyup(function() {
          input = this.value;
          if (input.length >= uniqueness.minCharacters) {
            uniqueness.search('title', input);
          }
          else if(input.length == 0 && !uniqueness.prependResults) {
            uniqueness.clear();
          }
        });
      });
      // Search off tags.
      $('#edit-taxonomy-tags-1').once('uniqueness', function() {
        $(this).blur(function() {
          input = this.value;
          // Some tags set.
          if (input.length > 0) {
            uniqueness.search('tags', input);
          }
        });
      });
    }
  };

  Drupal.uniqueness = function (uri, widget) {
    this.uri = uri;
    this.delay = 500;
    this.widget = widget;
    this.list = $('.item-list ul', widget);
    this.notifier = $('.uniqueness-search-notifier', widget);
    this.widgetCSS = {
      'background-image' : "url('" + Drupal.settings.basePath + "misc/throbber.gif" + "')",
      'background-position' : '100% -18px',
      'background-repeat' : 'no-repeat'
    };
    this.searchCache = {};
    this.listCache = {};
    this.prependResults = Drupal.settings.uniqueness['prependResults'];
    this.nid = Drupal.settings.uniqueness['nid'];
    this.type = Drupal.settings.uniqueness['type'];
    this.minCharacters = Drupal.settings.uniqueness['minCharacters'];
    this.autoOpen = $(widget).closest('fieldset');
  }

  Drupal.uniqueness.prototype.update = function (data) {
    var expand = false;
    uniqueness.notifier.removeClass('uniqueness-dyn-searching').empty();
    uniqueness.widget.css('background-image', '');
    uniqueness = this;
    if (uniqueness.prependResults) {
      if (data == undefined && uniqueness.listCache != {}) {
        data = uniqueness.listCache;
      }
      var items = '';
      $.each(data, function(i, item) {
        // Only use what we haven't seen before.
        if (uniqueness.listCache[item.nid] == undefined) {
          items += '<li><a href="' + item.href + '" target="_blank">' + item.title + '</a> ' + (item.status == 0 ? '(' + Drupal.t('not published') + ')' : '') + '</li>';
          // Store the new item.
          uniqueness.listCache[item.nid] = item;
          expand = true;
        }
      });
      // Show list.
      this.list.prepend(items);
    }
    else { // Replace content. //@todo still use caching?
      $(".uniqueness-description", uniqueness.widget.parent()).toggle(data != undefined);
      if (data == undefined) {
        uniqueness.clear();
        if ($('#edit-title')[0].value.length) {
          uniqueness.notifier.html(Drupal.settings.uniqueness['noResultsString']);
        }
        return;
      }
      var items = '';
      $.each(data, function(i, item) {
        if (item.more) {
          items += '<li>' + Drupal.t("... and others.") + '</li>';
        }
        else {
          items += '<li><a href="' + item.href + '" target="_blank">' + item.title + '</a> ' + (item.status == 0 ? '(' + Drupal.t('not published') + ')' : '') + '</li>';
        }
      });
      this.list.html(items);
      expand = items.length;
    }
    if (expand && uniqueness.autoOpen) {
      uniqueness.autoOpen.removeClass('collapsed');
      // Only auto open the fieldset once per page load.
      uniqueness.autoOpen = null;
    }
  }

  Drupal.uniqueness.prototype.search = function (element, searchString) {
    uniqueness = this;

    // If this string has been searched for before we do nothing.
    if (uniqueness.prependResults && uniqueness.searchCache[searchString]) {
      return;
    }

    if (this.timer) {
      clearTimeout(this.timer);
    }
    this.timer = setTimeout(function () {
      // Inform user we're searching.
      if (uniqueness.notifier.hasClass('uniqueness-dyn-searching') == false) {
        uniqueness.notifier.addClass('uniqueness-dyn-searching').html(Drupal.settings.uniqueness['searchingString']);
        uniqueness.widget.css(uniqueness.widgetCSS);
      }
      var query = uniqueness.uri + '?';
      if (uniqueness.nid != undefined) {
        query += 'nid=' + uniqueness.nid + '&';
      }
      if (uniqueness.type != undefined) {
        query += 'type=' + uniqueness.type + '&';
      }
      $.getJSON(query + element + '=' + searchString, function (data) {
        if (data != undefined && data != 'false') {
          // Found results.
          uniqueness.update(data);
          // Save this string, it found results.
          uniqueness.searchCache[searchString] = searchString;
          var blockSet = true;
        }
        // Nothing new found so show existing results.
        if (blockSet == undefined) {
          uniqueness.update();
        }
      });
    }, uniqueness.delay);
  }

  Drupal.uniqueness.prototype.clear = function () {
    this.list.empty();
  }
})(jQuery);
