/**
 * @file
 * Wysifield primary wrapper js.
 *
 * Provides the addPlugin architecture and supporting js.
 */

(function($) {
  // Don't use strict mode until the function loops are fixed.
  // 'use strict';

  Drupal.wysifield = Drupal.wysifield || {};

  Drupal.wysifield.lastClick     = 0;
  Drupal.wysifield.clickInterval = 200;
  Drupal.wysifield.modalActive   = false;
  Drupal.wysifield.plugins       = [];

  Drupal.behaviors.wysifield = {
    attach: function(context, settings) {
      /**
       * Create the modal dialog.
       */
      Drupal.wysifield.createModal = function (opts, instanceId, data) {
        // Create the modal dialog element.
        Drupal.wysifield.createModalElement()
        // Create jQuery UI Dialog.
        .dialog(Drupal.wysifield.modalOptions());
        // Remove the title bar from the modal.
        // .siblings(".ui-dialog-titlebar").hide();

        // Make the modal seem "fixed".
        $(window).bind("scroll resize", function() {
          $('#wysifield-modal').dialog('option', 'position', ['center', 50]);
        });

        // Get modal content.
        Drupal.wysifield.getForm(opts, instanceId, data);
      };

      /**
       * Create and append the modal element.
       */
      Drupal.wysifield.createModalElement = function() {
        // Create a new div and give it an ID of wysifield-modal.
        // This is the dashboard container.
        var wysifieldModal = $('<div id="wysifield-modal"></div>');

        // Create a modal div in the <body>.
        $('body').append(wysifieldModal);

        return wysifieldModal;
      };

      /**
       * Default jQuery dialog options used when creating the wysifield modal.
       */
      Drupal.wysifield.modalOptions = function() {
        return {
          dialogClass: 'wysifield-modal-wrapper',
          modal: true,
          draggable: false,
          resizable: false,
          width: 800,
          height: 600,
          maxHeight: 600,
          position: ['center', 50],
          minHeight: 0,
          zIndex: 210000,
          close: Drupal.wysifield.modalClose,
          open: function (event, ui) {
            // Change the overlay style.
            $('.ui-widget-overlay').css({
              opacity: 0.55,
              filter: 'Alpha(Opacity=55)',
              backgroundColor: '#FFFFFF'
            });
          },
          title: 'Embed Form'
        };
      };

      /**
       * Close the wysifield modal.
       */
      Drupal.wysifield.modalClose = function () {
        $('#wysifield-modal').dialog('destroy').remove();
        // Make sure the current intstance settings are removed when the modal is
        // closed.
        // Drupal.settings.wysifield.currentInstance = {};
      };

      /**
       *
       */
      Drupal.wysifield.getForm = function (opts, instanceId, data) {
        // Create the AJAX object.
        var url = '';
        if (data.eid) {
          url = '/wysifield/embed-modal/nojs/' + opts.type + '/' + instanceId + '/' + data.eid;
        }
        else {
          url = '/wysifield/embed-modal/nojs/' + opts.type + '/' + instanceId;
        }

        var ajax_settings = {};
        ajax_settings.event = 'wysifieldEmbedForm';
        ajax_settings.url = url;
        ajax_settings.progress = {
          type: 'throbber',
          message : Drupal.t('Loading embed form...')
        };

        Drupal.ajax['wysifield-modal'] = new Drupal.ajax('wysifield-modal', $('#wysifield-modal')[0], ajax_settings);

        // @TODO: Jquery 1.5 accept success setting to be an array of functions.
        // But we have to wait for jquery to get updated in Drupal core.
        // In the meantime we have to override it.
        Drupal.ajax['wysifield-modal'].options.success = function (response, status) {
          if (typeof response === 'string') {
            response = $.parseJSON(response);
          }

          // Call the ajax success method.
          Drupal.ajax['wysifield-modal'].success(response, status);
        };

        // Trigger the ajax event.
        $('#wysifield-modal').trigger('wysifieldEmbedForm');
      };

      // (Re)Apply the double click event to images in the wsywig to
      // launch the config modal.

      // FIXME cke_editable is arbitrary per wysiwyg config. This needs to be targetted via a common class on all wysiwygs.
      $('.cke_editable img', $('.cke_contents iframe').contents()).once('wysiwyg-clickable', function() {
        $(this).dblclick(function() {
          // Decode the alts on the image into the data for the wysifield.
          var alts = Drupal.wysifield.wysifield_decode_alt(decodeURIComponent($(this).attr('alt')));

          $.each(Drupal.wysifield, function(i,e) {
            if ($.isFunction(e.housekeeping)) {
              e.housekeeping();
            }
          });
          // Launch the modal for wysifield config.
          Drupal.wysifield.createModal(Drupal.wysifield.plugins[alts.type], $('.text-summary-wrapper textarea').attr('id'), alts);
        });
      });

      // When the modal config window is closed, remove highlighting of
      // content so that the next wysifield selected isn't misapplied.
      // @Todo: This may need to moved into a named functin that we can call after a modal is closed/canceld
      // or can be called within the commands array in the _wysifield_modal_callback() in wysifield.module after
      // a child modal form is submitted.
      $('.ctools-modal-content .close').once('cmcc', function(){
        $(this).bind('click.wysifieldRemoveHighlting', function() {
          // Remove highlighting of elements in body
          // http://stackoverflow.com/questions/1452871/how-can-i-access-iframe-elements-with-javascript
          // http://stackoverflow.com/questions/3169786/clear-text-selection-with-javascript
          if ($('.cke_contents iframe').length) {
            var ifr = $('.cke_contents iframe')[0];
            ifr = ifr.contentWindow ? ifr.contentWindow.document : ifr.contentDocument;
            if (ifr.getSelection) {
              if (ifr.getSelection().empty) {  // Chrome
                ifr.getSelection().empty();
              }
              else if (ifr.getSelection().removeAllRanges) {  // Firefox
                ifr.getSelection().removeAllRanges();
              }
            }
            else if (ifr.document && ifr.document.selection) {  // IE?
              ifr.document.selection.empty();
            }
          }
        });
      });
    }
  };
})(jQuery);

jQuery(document).ready(function($) {
  'use strict';

  Drupal.wysifield.addPlugin = function (opts) {

    // Save settings for each plugin
    Drupal.wysifield.plugins[opts.type] = opts;

    var wysifield = {
      /**
       * Return whether the passed node belongs to this plugin.
       *
       * @param node
       *   The currently focused DOM element in the editor content.
       */
      isNode: function(node) {
        return $(node).is('img.wysifield-' + opts.type);
      },

      /**
       * Invoke is called when the toolbar button is clicked
       *
       * @param data
       *   An object containing data about the current selection:
       *   - format:  'html' when the passed data is HTML content, 'text' when the
       *     passed data is plain-text content.
       *   - node:    When 'format' is 'html', the focused DOM element in the editor.
       *   - content: The textual representation of the focused/selected editor
       *     content.
       * @param settings
       *   The plugin settings, as provided in the plugin's PHP include file.
       * @param instanceId
       *   The ID of the current editor instance.
       */
      invoke: function(data, settings, instanceId) {
        var uniqid = (new Date().getTime()).toString(16);
        var content;
        var options = {};

        // Call plugin specific invoke handler
        $(document).trigger('wysifield-' + opts.type + '-invoke', settings);

        // If we're in WYSIWYG mode
        if (data.format === 'html') {
          // If we're dealing with an existing wysifield
          if ($(data.node).is('img.wysifield-' + opts.type)) {
            // Expand inline tag in alt attribute
            data.node.alt = decodeURIComponent(data.node.alt);
            options = Drupal.wysifield.wysifield_decode_alt(data.node.alt);
            options.action = 'update';
          }
        }
        /*
        else {
          // We're in raw text mode.
          //content = this._getRawText(settings, uniqid);
        }
        */
        Drupal.wysifield.createModal(opts, instanceId, opts);
      },

      /**
       * Prepare all plain-text contents of this plugin with HTML representations.
       *
       * Optional; only required for "inline macro tag-processing" plugins.
       *
       * @param content
       *   The plain-text contents of a textarea.
       * @param settings
       *   The plugin settings, as provided in the plugin's PHP include file.
       * @param instanceId
       *   The ID of the current editor instance.
       */
      attach: function(content, settings, instanceId) {
        var re = new RegExp('\\[wysifield-' + opts.type + '\\|([^\\[\\]]+)\\]','g');
        content = content.replace(re, function(orig, match) {
          var node = {};
          node = Drupal.wysifield.wysifield_decode_alt(match);
          var tokenInfo = {
            'eid' : node.eid,
            'type' : opts.type,
            'view_mode' : node.view_mode,
          };

          // LOOP Through the opts and then pass that over.
          var img = {};
          img.name      = 'wysifield_'+opts.type;
          // 'class' is a predefined token in JavaScript.
          img['class']  = 'wysifield-'+opts.type;
          img.src       = '/' + opts.rte_icon;
          img.width     = 200;
          img.height    = 30;
          img.alt = encodeURIComponent(Drupal.wysifield.wysifield_encode_alt(tokenInfo, node));

          var element = '<img ';
          for (var property in img) {
            element += property + '="' + img[property] + '" ';
          }
          element += '/>';
          return element;
        });


        return content;
      },

      /**
       * Process all HTML placeholders of this plugin with plain-text contents.
       *
       * Optional; only required for "inline macro tag-processing" plugins.
       *
       * @param content
       *   The HTML content string of the editor.
       * @param settings
       *   The plugin settings, as provided in the plugin's PHP include file.
       * @param instanceId
       *   The ID of the current editor instance.
       */
      detach: function(content, settings, instanceId) {
        var $content = $('<div>' + content + '</div>');

        var tmp = this;
        $.each($('img.wysifield-' + opts.type, $content), function (node) {
          // [wysifield-embedded_photo|eid="1"|type="embedded_photo"|viewmode="default"]
          var inlineTag = '[wysifield-' + opts.type + '|' + decodeURIComponent(this.alt) + ']';
          $(this).replaceWith(inlineTag);
        });

        return $content.html();
      },
    };

    return wysifield;
  };

  /**
   * Perform the AJAX request for the modal config.
   */
  Drupal.wysifield.wysifield_ajax = function (opts, instanceId, data) {

    // Create a pseudo element, attach the Drupal ajax functionality
    // execute the callbacks.
    var base    = 'magic-pseudo';
    var uri = '';
    if (data.eid) {
      uri = '/wysifield/embed-modal/nojs/' + opts.type + '/' + instanceId + '/' + data.eid;
    }
    else {
      uri = '/wysifield/embed-modal/nojs/' + opts.type + '/' + instanceId;
    }

    var element = $('<a href="'+uri+'" id="magic-pseudo" class="ctools-use-modal">Loading...</a>');

    var myAjax  = new Drupal.ajax(
      base,
      element,
      {
        'url': uri,
        'event': 'click',
        'submit': {
          'params': JSON.stringify(data)
        },
        'progress': { 'type': 'throbber' }
      }
    );
    Drupal.ajax[base] = myAjax;

    // While the modal is open, we add a class to body
    $('body').addClass('modal-open');

    Drupal.wysifield.modalActive = true;
    myAjax.eventResponse(element, 'click');
  };

  /**
   * Convert alt string to array
   */
  Drupal.wysifield.wysifield_decode_alt = function (alt) {
    var data = {};
    // Cycle through the params present in the alt
    $.each(alt.match(/(\|?[^=]+="[^"]*")/g), function(i, e) {
      // Parse out the param key/values
      e.replace(/(?:\|)?([^=]+)(?:=")([^"]*)(?:")/, function(o, key, value) {
        data[key] = value;
      });
    });
    return data;
  };

  /**
   * Convert array to alt string
   */
  Drupal.wysifield.wysifield_encode_alt = function (opts, data) {
    var alts = [];
    $.each(opts, function (i,e) {
      if (typeof data[i] !== undefined) {
        alts.push(i+'="'+data[i]+'"');
      }
      else {
        alts.push(i+'="'+e+'"');
      }
    });
    alts = alts.join('|');
    return alts;
  };

  Drupal.wysifield.entity_insert = function (ajax, response, status) {
    var t = response.data;
    var node = {};
    var opts = Drupal.settings.wysifield.plugins[t.params.type];
    // Modded for FPP does this still work for nodes?
    node.name     = 'wysifield_'+t.params.type;
    // 'class' is a predefined token in JavaScript.
    node['class'] = 'wysifield-'+t.params.type;
    node.src      = '/' + opts.rte_icon;

    // Embed the entity ID (eid) as well.
    // POSSIBLE ISSUE AS WELL. DONT ASSIGN DIRCT TO OPTS?
    // Just commented remove all comments after thorough testing
    // opts.eid = t.eid;
    var tokenInfo = {
      'eid' : t.params.eid,
      'type' : t.params.type,
      'view_mode' : t.params.view_mode,
    };
    node.alt = encodeURIComponent(Drupal.wysifield.wysifield_encode_alt(tokenInfo, t.params));

    var element = '<img ';
    for (var property in node) {
      element += property + '="' + node[property] + '" ';
    }
    element += '/>';

    // Add the wysifield to the active instance.
    Drupal.wysiwyg.instances[t.instance_id].insert(element);

    // While the modal is open, we add a class to body to prevent scrolling.
    // This removes it
    $('body').removeClass('modal-open');
    Drupal.wysifield.modalActive = false;
  };

  Drupal.wysifield.showLoading = function() {
    if ($('#loading-indicator').length) {
      $('#loading-indicator').fadeIn(200);
    }
    else {
      $('body').append('<div id="loading-indicator"><img src="/sites/all/themes/twc_admin/images/ajax-loader.gif" /></div>');
    }
  };

  Drupal.wysifield.hideLoading = function() {
    $('#loading-indicator').fadeOut(200);
  };

  Drupal.ajax.prototype.commands.wysifield_close_modal = Drupal.wysifield.modalClose;

  // Add our custom commands to Drupal.ajax object.
  Drupal.ajax.prototype.commands.wysifield_insert = Drupal.wysifield.entity_insert;

  // Drupal.ajax.prototype.commands.wysifieldOpenParentModal = Drupal.wysifield.open_parent_modal {
  // }

  // Add each of the plugins to the ckeditor model.
  $.each(Drupal.settings.wysifield.plugins, function (i, e) {
      Drupal.wysiwyg.plugins[i] = Drupal.wysifield.addPlugin(e);
  });

  // Apply page state during modal status changes
  $(document).bind('CToolsDetachBehaviors', function(e, s) {
    // While the modal is open, we add a class to body to prevent scrolling.
    // This removes it
    $('body').removeClass('modal-open');
    Drupal.wysifield.modalActive = false;

  });

  // Handle tab keypresses in browser when modal is active.
  $(document).keydown(function(e) {
    if (Drupal.wysifield.modalActive) {
      if (parseInt(e.which) === 9) {
        var elements = $('input,select,textarea,radio,button', '#modalContent');
        var first = elements.first();
        var last  = elements.last();

        // shift-tab
        if (e.shiftKey) {
          // if we're looking at the first item in the list, cycle to last.
          if ($(e.srcElement).attr('name') === first.attr('name')) {
            last.focus();
            e.preventDefault();
          }
        }
        else {
          // if we're looking at the last item in the list, cycle to first.
          if ($(e.srcElement).attr('name') === last.attr('name')) {
            first.focus();
            e.preventDefault();
          }
        }
      }
    }
  });


  // When (if) CKEDITOR is ready, add event handlers.
  if (CKEDITOR) {
    //CKEDITOR.on('loaded', function(){ Drupal.attachBehaviors(); })
    // Searches through all CK Editor intances and attaches a paste
    // event to re-attach the clickable event on the wysifield entry
    // in the wysiwyg. Timeout works around the paste event firing
    // before the item is paste into the WYSIWYG.
    for (var i in CKEDITOR.instances) {
      CKEDITOR.instances[i].on('paste', function(ev) {
      //  Drupal.wysifield.dropped = ev;
        setTimeout(Drupal.wysifield.reattach_clickable, 500);
      });
    }

    CKEDITOR.on('instanceReady', function (ev) {
      // There is no event for "editor ready" so this will have to do.
      // http://drupal.org/node/641900
      setTimeout(Drupal.behaviors.wysifield.attach, 1000);

      // Prevent drag-and-drop.
      ev.editor.document.on('drop', function (ev) {
      // Drupal.wysifield.dropped = ev;
      // Depreciated event element mapping asnow we scan all imgs and remove
      // the processed class. Attach will add it back in for all of them
      // anyways, and this resolves an issue with the event detecting on the <p>
      // wrapper.
        setTimeout(Drupal.wysifield.reattach_clickable, 500);
      });
    });
  }

  /**
   * Reattaches the doubleclick handler that is lost during drag/drop.
   */
  Drupal.wysifield.reattach_clickable = function() {
    var ev = Drupal.wysifield.dropped;
    // Remove the class added by jquery .once in the attach() function.
    $('.wysiwygeditor img', $('.cke_contents iframe').contents()).removeClass('wysiwyg-clickable-processed');
    // Rerun attach and add clickabilty to all buttons.
    Drupal.behaviors.wysifield.attach();
  };

});
