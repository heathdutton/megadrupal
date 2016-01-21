(function ($) {

function getQueryParameterByName(query, name) {
  // Prepare the query string for searching.
  if (query.substr(0, 1) !== '?') {
    query = '?' + query;
  }

  // Setup a regex to find the value.
  name = name.replace(/[\[]/, "\\[").replace(/[\]]/, "\\]");
  var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
  results = regex.exec(query);
  return results == null ? "" : decodeURIComponent(results[1].replace(/\+/g, " "));
}

Drupal.wysiwyg.plugins.tweetable_text = {
  attributes: {
    'alt': 'Tweet text',
    'hashtag': 'Hashtag',
    'url': 'URL'/*,
    'url-query': 'Query string'*/
  },

  dialog: null,
  currentNode: null,
  currentInstance: null,

  /**
   * Return whether the passed node belongs to this plugin.
   */
  isNode: function(node) {
    return ($(node).is('span.tweetable-text-wysiwyg'));
  },

  /**
   * Setup the dialog initially.
   */
  setupDialog: function () {
    var self = this;

    this.dialog = $('<div></div>').appendTo('body');

    // Add the main attributes.
    $.each(this.attributes, function (attr, name) {
      var formItem = $('<div class="form-item"></div>');

      $('<label></label>')
        .text(Drupal.t(name))
        .attr('for', attr)
        .appendTo(formItem);

      $('<input type="text" length="140" class="form-text" />')
        .attr('name', attr)
        .css('width', '100%')
        .appendTo(formItem);

      formItem.appendTo(self.dialog);
    });

    // Add the Save button.
    $('<button></button>')
      .text(Drupal.t('Save'))
      .appendTo(this.dialog)
      .button()
      .click(function () {
        // Save the attributes.
        $.each(self.attributes, function (attr, name) {
          var val = $('input[name="' + attr + '"]').val().trim();
          if (attr === 'alt' && val == self.currentNode.text()) {
            // Skip setting 'alt' if it's the default.
            return;
          }
          if (val) {
            self.currentNode.attr(attr, val);
          }
        });

        // If this is a new node, we have to insert it.
        if (self.currentNode.get(0).parentNode.nodeType === Node.DOCUMENT_FRAGMENT_NODE) {
          // We append to a <div></div> just to work around .html()
          // returning the inner rather than outter HTML.
          Drupal.wysiwyg.instances[self.currentInstance].insert($('<div></div>').append(self.currentNode).html());
        }

        self.dialog.dialog('close');
      });
    
    // Process with behaviors.
    Drupal.attachBehaviors(this.dialog);
  },

  /**
   * Execute the button.
   */
  invoke: function(data, settings, instanceId) {
    var self = this, 
        instance = Drupal.wysiwyg.instances[instanceId];

    if (data.format == 'html') {
      // We do a hack to get the actually selected element because
      // we don't get it in data for some reason under CKEditor.
      if (data.node === null && !data.content && instance.editor === 'ckeditor') {
        var ckeditor = CKEDITOR.instances[instance.field]
            selection = ckeditor.getSelection();
        if (selection.getType() == CKEDITOR.SELECTION_ELEMENT) {
          data.node = selection.getSelectedElement();
        }
        else {
          var range = selection.getRanges(true)[0];
          range.shrink(CKEDITOR.SHRINK_TEXT);
          var root = range.getCommonAncestor();
          data.node = root.getAscendant('span', true);
          if (data.node) {
            data.node = data.node.$;
          }
        }
      }

      // Create the dialog, if it doesn't exist yet.
      if (!this.dialog) {
        this.setupDialog();
      }

      // Setup the dialog with current information.
      if ($(data.node).is('span.tweetable-text-wysiwyg')) {
        this.currentNode = $(data.node);
      }
      else {
        this.currentNode = $('<span class="tweetable-text-wysiwyg">' + data.content + '</span>');
      }
      $.each(this.attributes, function (attr, name) {
        var val = self.currentNode.attr(attr);
        if (attr == 'alt' && !val) {
          val = self.currentNode.text().trim().substring(0, 140);
        }
        $('input[name="' + attr + '"]').val(val);
      });

      // Finally, stash the Wysiwyg instance and show the dialog.
      this.currentInstance = instanceId;
      this.dialog.dialog({
        title: Drupal.t('Tweetable text'),
        modal: true,
        minHeight: 'auto'
      });
    }
  },

  /**
   * Replace all [tweetable]text[/tweetable] psuedo-tags with spans.
   */
  attach: function(content, settings, instanceId) {
    content = content.replace(/\[tweetable(.*?)\]/g, '<span class="tweetable-text-wysiwyg"$1>');
    content = content.replace(/\[\/tweetable\]/g, '</span>');
    return content;
  },

  /**
   * Replace span with the [tweetable]text[/tweetable] psuedo-tags.
   */
  detach: function(content, settings, instanceId) {
    var self = this,
        $content = $('<div>' + content + '</div>');
    $('span.tweetable-text-wysiwyg', $content).each(function (i, elem) {
      var psuedoTag = '[tweetable';
      $.each(self.attributes, function (attr, name) {
        if (elem.getAttribute(attr)) {
          psuedoTag += ' ' + attr + '="' + elem.getAttribute(attr) + '"';
        }
      });
      psuedoTag += ']';
      psuedoTag += elem.innerHTML;
      psuedoTag += '[/tweetable]';

      elem.parentNode.insertBefore(document.createTextNode(psuedoTag), elem);
      elem.parentNode.removeChild(elem);
    });
    return $content.html();
  }
};

})(jQuery);
