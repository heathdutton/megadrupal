/**
 * @file
 * Main controller for the Getty Images UI.
 */

(function($) {
  // Alias GettyImages module.
  var g = GettyImages,
    s = getty_s,
    $templates,
    $modal;

  // Bind activation handler.
  Drupal.behaviors.gettyImages = {
    attach: function(context, settings) {
      // Only do this in the document context.
      if(context !== document) {
        return;
      }

      $(context).on('click', '.getty-images-select', function(ev) {
        var $this = $(this);

        // Open the frame, ensuring there's a controller and frame reference.
        function open() {
          var context = $this.data('getty-images-context');

          if(!g.controller) {
            g.controller = new g.model.GettyImages();
          }

          if(!g.frame) {
            g.frame = new g.view.GettyImages({
              controller: g.controller
            });
          }

          g.controller.set('context', context);

          if(context == 'image_field') {
            g.controller.$image_field = $this;
          }

          g.frame.open();
        };

        ev.preventDefault();

        // Defer loading of frame until templates have loaded.
        if(!$templates) {
          $templates = $('<div id="getty-templates"></div>');

          $('body').append($templates);

          $templates.load(g.settings.templates, open);
        }
        else {
          open();
        }
      });
    }
  }

  /* Initialize the Getty Images user session which handles authentication
     needs and maintains application tokens for anonymous searches. */
  g.user = new g.model.User();
  g.user.restore();

  // Reference to tab, which also contains the download form.
  g.$tab = $('.getty-images-tab');

  /* The Getty "Clipboard" - Fake a paste into textarea or text input by capturing
     ctrl- or cmd-v. Doesn't detect which meta key is correct, and is not the real
     clipboard. Disappears after the content is pasted. */
  g.clipboard = new g.view.Clipboard({
    model: new Backbone.Model()
  });

  g.$tab.append(g.clipboard.$el);

  // Main controller for the Getty Images Drupal module.
  g.model.GettyImages = Backbone.Model.extend({
    /* Keep track of create / render handlers to ensure that they
       are all registered / dereigstered whenever this state is
       activated or deactivated. */
    handlers: {
      'content:create:getty-images-browse': 'createBrowser',
      'title:create:getty-title-bar': 'createTitleBar',
      'content:create:getty-about-text': 'createAboutText',
      'content:activate:getty-images-browse': 'activateBrowser',
      'actionbar:create:getty-images-actionbar': 'createActionbar',
      'content:create:getty-mode-select': 'createModeSelect',
      'actionbar:create:blank-actionbar': 'createBlankActionbar',
    },

    initialize: function() {
      // Are we stupid?
      this.set('unsupported', !$.support.cors);

      this._displays = {};

      this.set('gutter', 8);
      this.set('edge', 150);

      if(!this.get('library')) {
        this.set('library', new g.model.Attachments(null, {
          controller: this,
          props: { query: true },
        }));
      }

      if(!this.get('refinements')) {
        this.set('refinements', new Backbone.Collection());
      }

      if(!this.get('categories')) {
        this.set('categories', new Backbone.Collection());
      }

      if(!this.get('selection')) {
        this.set('selection', new g.model.Selection([], {
          multiple: true
        }));
      }

      // Register my handlers.
      for(var handler in this.handlers) {
        this.on(handler, this[this.handlers[handler]], this);
      }

      g.user.settings.on('change:mode', this.setMode, this);
      g.user.on('change:loggedIn', this.setMode, this);
    },

    activate: function() {
      if(this.get('unsupported')) {
        this.frame.$el.addClass('getty-unsupported-browser');
        return;
      }

      s && s.t();

      // Unbind any clipboard handlers.
      g.clipboard.model.unset('value');

      this.setMode();
    },

    setMode: function() {
      this.set('mode', g.user.settings.get('mode'));

      var mode = this.get('mode');

      switch(mode) {
        case 'login':
          if(!g.user.get('loggedIn')) {
            this.set('content', 'getty-mode-select');
            this.set('actionbar', 'blank-actionbar');
          }
          else {
            this.set('content', 'getty-images-browse');
            this.set('actionbar', 'getty-images-actionbar');
          }

          break;

        case 'embed':
          this.set('content', 'getty-images-browse');
          this.set('actionbar', 'getty-images-actionbar');

          break;

        default:
          this.set('content', 'getty-mode-select');
          this.set('actionbar', 'blank-actionbar');
      }
    },

    createModeSelect: function(content) {
      if(this.get('unsupported')) {
        content.view = new g.view.UnsupportedBrowser();
        return;
      }

      content.view = new g.view.ModeSelect({
        controller: this
      });
    },

    createBlankActionbar: function(actionbar) {
      actionbar.view = new Backbone.View();
    },

    deactivate: function() {
      this.turnBindings('off');
    },

    createTitleBar: function(title) {
      title.view = new g.view.TitleBar({
        controller: this,
      });
    },

    createAboutText: function(content) {
      content.view = new g.view.About({
        controller: this,
      });
    },

    createBrowser: function(content) {
      if(this.get('unsupported')) {
        content.view = new g.view.UnsupportedBrowser();
        return;
      }

      content.view = this.browser = new g.view.Browser({
        controller: this,
        model:      this,
        collection: this.get('library'),
        selection:  this.get('selection'),
        refinements: this.get('refinements'),
        categories: this.get('categories'),
        sortable:   false,
        search:     true,
        filters:    true,
        display:    false,
        dragInfo:   false,
        AttachmentView: g.view.Attachment
      });
    },

    activateBrowser: function(content) {
      this.frame.$el.removeClass('hide-toolbar');
    },

    // Create the bottom actionbar.
    createActionbar: function(actionbar) {
      if(this.get('unsupported')) {
        return;
      }

      actionbar.view = new g.view.ActionBar({
        controller: this,
        collection: this.get('selection')
      });
    },

    copy: function() {
      var image = this.get('selection').single(),
          embed_code;

      if(!image) {
        return;
      }

      if(this.get('mode') == 'embed') {
        g.clipboard.model.set({
          value: "\nhttp://gty.im/" + image.get('ImageId') + "\n",
          label: g.text.embedCopied
        });
      } else {
        // Get display options from user.
        var display = this.display(image);

        var align = display.get('align') || 'none';
        var alt = display.get('alt');
        var caption = display.get('caption');

        var sizeSlug = display.get('size');
        var sizes = display.get('sizes');
        var size = sizes[sizeSlug];

        if(!size) {
          return;
        }

        // Build image tag with those options.
        var $img = $('<img>');

        $img.attr('src', size.url);

        if(alt) {
          $img.attr('alt', alt);
        }

        if(caption) {
          $img.attr('title', caption);
        }

        if(align != 'none') {
          $img.addClass('align-' + align);
        }

        $img.addClass('size-' + sizeSlug);
        $img.attr('width', size.width);
        $img.attr('height', size.height);

        var $container = $('<div>').append($img);

        g.clipboard.model.set({
          value: $container.html(),
          label: g.text.imageCopied
        });
      }
    },

    // Select this image for use as image value.
    select: function(image) {
      // TODO: Scrub away multiple files functionality.
      var file = image.get('files') && image.get('files').at(0);

      if(this.get('context') == 'image_field' && file) {
        this.$image_field.parents('.filefield-source').find('input[type="hidden"]').val(file.get('fid'));
        this.$image_field.trigger('getty-images-select');
        g.frame.close();
      }
    },

    reset: function() {
      this.get('selection').reset();
      this._displays = {};
      this.refreshContent();
    },

    display: function(attachment) {
      var displays = this._displays;

      if(!displays[attachment.cid]) {
        displays[attachment.cid] = new g.model.DisplayOptions({
          attachment: attachment
        });
        displays[attachment.cid].set('size', 'full');
      }

      return displays[attachment.cid];
    },
  });
})(jQuery);
