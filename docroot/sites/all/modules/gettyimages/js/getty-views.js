/**
 * @file
 * Views for the Getty Images plugin.
 */

(function($) {
  var g = GettyImages, s = getty_s;

  /**
   * Views and Subviews management: Completely stolen from WordPress's wp-backbone.js.
   */

  // g.view.Subviews
  // --------------------
  //
  // A subview manager.
  g.view.Subviews = function(view, views) {
    this.view = view;
    this._views = _.isArray(views) ? { '' : views } : views || {};
  };

  g.view.Subviews.extend = Backbone.Model.extend;

  _.extend(g.view.Subviews.prototype, {
    // ### Fetch all of the subviews
    //
    // Returns an array of all subviews.
    all: function() {
      return _.flatten(this._views);
    },

    // ### Get a selector's subviews
    //
    // Fetches all subviews that match a given `selector`.
    //
    // If no `selector` is provided, it will grab all subviews attached
    // to the view's root.
    get: function(selector) {
      selector = selector || '';
      return this._views[selector];
    },

    // ### Get a selector's first subview
    //
    // Fetches the first subview that matches a given `selector`.
    //
    // If no `selector` is provided, it will grab the first subview
    // attached to the view's root.
    //
    // Useful when a selector only has one subview at a time.
    first: function(selector) {
      var views = this.get(selector);
      return views && views.length ? views[0] : null;
    },

    // ### Register subview(s)
    //
    // Registers any number of `views` to a `selector`.
    //
    // When no `selector` is provided, the root selector (the empty string)
    // is used. `views` accepts a `Backbone.View` instance or an array of
    // `Backbone.View` instances.
    //
    // ---
    //
    // Accepts an `options` object, which has a significant effect on the
    // resulting behavior.
    //
    // `options.silent` &ndash; *boolean, `false`*
    // > If `options.silent` is true, no DOM modifications will be made.
    //
    // `options.add` &ndash; *boolean, `false`*
    // > Use `Views.add()` as a shortcut for setting `options.add` to true.
    //
    // > By default, the provided `views` will replace
    // any existing views associated with the selector. If `options.add`
    // is true, the provided `views` will be added to the existing views.
    //
    // `options.at` &ndash; *integer, `undefined`*
    // > When adding, to insert `views` at a specific index, use
    // `options.at`. By default, `views` are added to the end of the array.
    set: function(selector, views, options) {
      var existing, next;

      if (!_.isString(selector)) {
        options  = views;
        views    = selector;
        selector = '';
      }

      options  = options || {};
      views    = _.isArray(views) ? views : [ views ];
      existing = this.get(selector);
      next     = views;

      if(existing) {
        if(options.add) {
          if(_.isUndefined(options.at)) {
            next = existing.concat(views);
          } else {
            next = existing;
            next.splice.apply(next, [ options.at, 0 ].concat(views));
          }
        } else {
          _.each(next, function(view) {
            view.__detach = true;
          });

          _.each(existing, function(view) {
            if(view.__detach) {
              view.$el.detach();
            } else {
              view.remove();
            }
          });

          _.each(next, function(view) {
            delete view.__detach;
          });
        }
      }

      this._views[selector] = next;

      _.each(views, function(subview) {
        var constructor = subview.Views || g.view.Subviews,
          subviews = subview.views = subview.views || new constructor(subview);
        subviews.parent   = this.view;
        subviews.selector = selector;
      }, this);

      if(!options.silent)
        this._attach(selector, views, _.extend({ ready: this._isReady() }, options));

      return this;
    },

    // ### Add subview(s) to existing subviews
    //
    // An alias to `Views.set()`, which defaults `options.add` to true.
    //
    // Adds any number of `views` to a `selector`.
    //
    // When no `selector` is provided, the root selector (the empty string)
    // is used. `views` accepts a `Backbone.View` instance or an array of
    // `Backbone.View` instances.
    //
    // Use `Views.set()` when setting `options.add` to `false`.
    //
    // Accepts an `options` object. By default, provided `views` will be
    // inserted at the end of the array of existing views. To insert
    // `views` at a specific index, use `options.at`. If `options.silent`
    // is true, no DOM modifications will be made.
    //
    // For more information on the `options` object, see `Views.set()`.
    add: function(selector, views, options) {
      if(!_.isString(selector)) {
        options  = views;
        views    = selector;
        selector = '';
      }

      return this.set(selector, views, _.extend({ add: true }, options));
    },

    // ### Stop tracking subviews
    //
    // Stops tracking `views` registered to a `selector`. If no `views` are
    // set, then all of the `selector`'s subviews will be unregistered and
    // removed.
    //
    // Accepts an `options` object. If `options.silent` is set, `remove`
    // will *not* be triggered on the unregistered views.
    unset: function(selector, views, options) {
      var existing;

      if(!_.isString(selector)) {
        options = views;
        views = selector;
        selector = '';
      }

      views = views || [];

      if(existing = this.get(selector)) {
        views = _.isArray(views) ? views : [ views ];
        this._views[ selector ] = views.length ? _.difference(existing, views) : [];
      }

      if(!options || !options.silent) {
        _.invoke(views, 'remove');
      }

      return this;
    },

    // ### Detach all subviews
    //
    // Detaches all subviews from the DOM.
    //
    // Helps to preserve all subview events when re-rendering the master
    // view. Used in conjunction with `Views.render()`.
    detach: function() {
      $(_.pluck(this.all(), 'el')).detach();
      return this;
    },

    // ### Render all subviews
    //
    // Renders all subviews. Used in conjunction with `Views.detach()`.
    render: function() {
      var options = {
        ready: this._isReady()
      };

      _.each(this._views, function(views, selector) {
        this._attach(selector, views, options);
      }, this);

      this.rendered = true;
      return this;
    },

    // ### Remove all subviews
    //
    // Triggers the `remove()` method on all subviews. Detaches the master
    // view from its parent. Resets the internals of the views manager.
    //
    // Accepts an `options` object. If `options.silent` is set, `unset`
    // will *not* be triggered on the master view's parent.
    remove: function(options) {
      if(!options || !options.silent) {
        if(this.parent && this.parent.views)
          this.parent.views.unset(this.selector, this.view, { silent: true });
        delete this.parent;
        delete this.selector;
      }

      _.invoke(this.all(), 'remove');
      this._views = [];
      return this;
    },

    // ### Replace a selector's subviews
    //
    // By default, sets the `$target` selector's html to the subview `els`.
    //
    // Can be overridden in subclasses.
    replace: function($target, els) {
      $target.html(els);
      return this;
    },

    // ### Insert subviews into a selector
    //
    // By default, appends the subview `els` to the end of the `$target`
    // selector. If `options.at` is set, inserts the subview `els` at the
    // provided index.
    //
    // Can be overridden in subclasses.
    insert: function($target, els, options) {
      var at = options && options.at,
        $children;

      if(_.isNumber(at) && ($children = $target.children()).length > at) {
        $children.eq(at).before(els);
      } else {
        $target.append(els);
      }

      return this;
    },

    // ### Trigger the ready event
    //
    // **Only use this method if you know what you're doing.**
    // For performance reasons, this method does not check if the view is
    // actually attached to the DOM. It's taking your word for it.
    //
    // Fires the ready event on the current view and all attached subviews.
    ready: function() {
      this.view.trigger('ready');

      // Find all attached subviews, and call ready on them.
      _.chain(this.all()).map(function(view) {
        return view.views;
      }).flatten().where({ attached: true }).invoke('ready');
    },

    // #### Internal. Attaches a series of views to a selector.
    //
    // Checks to see if a matching selector exists, renders the views,
    // performs the proper DOM operation, and then checks if the view is
    // attached to the document.
    _attach: function(selector, views, options) {
      var $selector = selector ? this.view.$(selector) : this.view.$el,
        managers;

      // Check if we found a location to attach the views.
      if(!$selector.length) {
        return this;
      }

      managers = _.chain(views).pluck('views').flatten().value();

      // Render the views if necessary.
      _.each(managers, function(manager) {
        if(manager.rendered) {
          return;
        }

        manager.view.render();
        manager.rendered = true;
      }, this);

      // Insert or replace the views.
      this[options.add ? 'insert' : 'replace']($selector, _.pluck(views, 'el'), options);

      // Set attached and trigger ready if the current view is already
      // attached to the DOM.
      _.each(managers, function(manager) {
        manager.attached = true;

        if(options.ready) {
          manager.ready();
        }
      }, this);

      return this;
    },

    // #### Internal. Checks if the current view is in the DOM.
    _isReady: function() {
      var node = this.view.el;
      while(node) {
        if(node === document.body) {
          return true;
        }
        node = node.parentNode;
      }

      return false;
    }
  });

  // g.View
  // ----------------
  //
  // The base view class.
  g.View = Backbone.View.extend({
    // The constructor for the `Views` manager.
    Subviews: g.view.Subviews,

    constructor: function(options) {
      this.views = new this.Subviews(this, this.views);
      this.on('ready', this.ready, this);

      // Pull up properties.
      this.options = options || {};
      this.controller = this.options.controller;

      if(!this.className) {
        this.className = this.template;
      }

      Backbone.View.apply(this, arguments);
    },

    dispose: function() {
      // Undelegating events, removing events from the model, and
      // removing events from the controller mirror the code for
      // `Backbone.View.dispose` in Backbone 0.9.8 development.
      this.undelegateEvents();

      if (this.model && this.model.off) {
        this.model.off(null, null, this);
      }

      if (this.collection && this.collection.off) {
        this.collection.off(null, null, this);
      }

      // Unbind controller events.
      if (this.controller && this.controller.off) {
        this.controller.off(null, null, this);
      }

      return this;
    },

    remove: function() {
      this.dispose();

      var result = Backbone.View.prototype.remove.apply(this, arguments);

      // Recursively remove child views.
      if(this.views) {
        this.views.remove();
      }

      return result;
    },

    render: function() {
      var options;

      if(this.prepare) {
        options = this.prepare();
      }

      this.views.detach();

      if(this.template) {
        options = options || {};
        this.trigger('prepare', options);
        this.$el.html(g.template(this.template)(options));
      }

      this.views.render();
      return this;
    },

    prepare: function() {
      return this.options;
    },

    ready: function() {}
  });

  // g.view.PriorityList: sorts subviews by their priority.
  g.view.PriorityList = g.View.extend({
    initialize: function() {
      this._views = {};

      this.set(_.extend({}, this._views, this.options.views), { silent: true });

      delete this.options.views;

      if(!this.options.silent) {
        this.render();
      }
    },

    // Set a subview.
    set: function(id, view, options) {
      var priority, views, index;

      options = options || {};

      // Accept an object with an `id` : `view` mapping.
      if(_.isObject(id)) {
        _.each(id, function(view, id) {
          this.set(id, view);
        }, this);
        return this;
      }

      if(!(view instanceof Backbone.View)) {
        view = this.toView(view, id, options);
      }

      view.controller = view.controller || this.controller;

      this.unset(id);

      priority = view.options.priority || 10;
      views = this.views.get() || [];

      _.find(views, function(existing, i) {
        if(existing.options.priority > priority) {
          index = i;
          return true;
        }
      });

      this._views[id] = view;
      this.views.add(view, {
        at: _.isNumber(index) ? index : views.length || 0
      });

      return this;
    },

    // Get a subview by its id.
    get: function(id) {
      return this._views[id];
    },

    // Remove a subview by its id.
    unset: function(id) {
      var view = this.get(id);

      if(view) {
        view.remove();
      }

      delete this._views[id];
      return this;
    },

    // Make a g.View out of something that isn't that.
    toView: function(options) {
      return new g.View(options);
    }
  });

  // Extend both top-level media frames with an additional mode for fetching
  // and downloading images from Getty Images.
  g.view.GettyImages = g.View.extend({
    template: 'getty-images-frame',
    id: 'getty-images',

    events: {
      'click .getty-close-link': 'close'
    },

    handleKeyboard: function(ev) {
      // 'ESC'.
      if(ev.keyCode == 27) {
        ev.data.frame.close();
      }
    },

    initialize: function() {
      this.panels = {};
    },

    open: function() {
      if(!this.$el.parent().length) {
        $('body').append(this.$el);
      }

      this.views.set('.getty-images-title', new g.view.TitleBar({
        controller: this.controller
      }));

      this.controller.on('change:content change:title change:actionbar', this.changePanel, this);
      this.controller.activate();

      this.render();

      this.$el.show();
      this.$el.find('.getty-toolbar-primary .getty-text-input').focus();

      // Pass frame into jQuery event handler data since .on can't bind to something else.
      $('body').on('keydown.getty-images', { frame: this }, this.handleKeyboard);
    },

    // Possible panels are 'title', 'content', 'actionbar'. When a panel attribute
    // is changed in the controller model, replace that panel with one that is
    // created and activated here.
    changePanel: function(model, value, options) {
      var self = this;

      _.each([ 'title', 'content', 'actionbar' ], function(panel) {
        var content = { view: self.panels[panel] };

        if(!model.changed[panel]) {
          return;
        }

        if(!content.view) {
          self.controller.trigger(panel + ':create:' + model.changed[panel], content);
        }

        self.views.set('.getty-images-' + panel, content.view);

        content.view.trigger(panel + ':activate:' + content.view);
      });
    },

    close: function() {
      // Unregister any and all .getty-images jQuery handlers.
      $('body').off('.getty-images');
      this.$el.hide();
    },
  })

  // The Getty Images Browser view.
  g.view.Browser = g.View.extend({
    template: 'getty-attachments-browser',

    events: {
      'keyup .search': 'searchOnEnter',
      'click .getty-refine-toggle': 'toggleRefinementOptions',
      'click .getty-download .button-primary': 'download',
      'click .getty-images-more': 'more',
      'change .getty-filter-asset-type input': 'modeFlag',
      'click .getty-keyword': 'searchKeyword'
    },

    initialize: function() {
      // The possible refinement categories for a search.
      this.refinementCategories = this.options.categories;

      // The set of refinements added to a search.
      this.refinements = this.options.refinements;

      this.createSearchForm();
      this.createSidebar();
      this.updateContent();

      // Update browser content based on changes to library.
      this.collection.on('add remove reset', this.updateContent, this);

      // Update possible refinement categories.
      this.collection.results.on('change:refinements', this.updateRefinementCategories, this);

      // Handle changes to refinements list.
      this.refinements.on('add remove', this.updateSearchRefinements, this);
      this.refinements.on('reset', this.clearSearchRefinements, this);

      // Set various flags whenever the results description model  changes.
      this.collection.results.on('change:searched change:total change:refinements', this.updateFlags, this);
      this.collection.results.on('change:searching', this.updateSearching, this);

      // Set search mode flag when ImageFamilies value changes.
      this.collection.props.on('change:ImageFamilies', this.updateMode);

      // Set .have-more flag when there're more results to load.
      this.collection.on('add remove reset', this.updateHaveMore, this);

      // Update total.
      this.collection.results.on('change:total', this.updateTotal, this);

      this.controller.on('activate', this.ready, this);

      this.controller.on('change:refining', this.addRefiningClass, this);

      this.addRefiningClass();
    },

    // Create the Search Toolbar, containing filters.
    createSearchForm: function() {
      var self = this;

      // Create the toolbar.
      this.toolbar = new g.view.Toolbar({
        controller: this.controller,
        collection: this.collection
      });

      // Make primary come first.
      this.toolbar.views.set([ this.toolbar.primary, this.toolbar.secondary ]);

      // Where total results are displayed.
      this.total = new g.View({
        tagName: 'span',
        className: 'getty-search-results-total',
        priority: -20
      });

      this.button = new g.view.Button({
        text: g.text.search,
        click: function() {
          self.search()
        },
        priority: 10,
      });

      var fields = {
        search: null,

        searchButton: this.button,

        assetTypeFilters: new g.view.AssetTypeFilter({
          controller: this.controller,
          model: this.collection.propsQueue,
          priority: 25,
        }),

        imageTypeFilters: new g.view.ImageTypeFilter({
          controller: this.controller,
          model: this.collection.propsQueue,
          priority: 30,
        }),

        editorialSortOrderFilter: new g.view.EditorialSortOrderFilter({
          controller: this.controller,
          model: this.collection.propsQueue,
          priority: 35
        }),

        creativeSortOrderFilter: new g.view.CreativeSortOrderFilter({
          controller: this.controller,
          model: this.collection.propsQueue,
          priority: 35
        }),

        orientationFilter: new g.view.OrientationFilter({
          controller: this.controller,
          model: this.collection.propsQueue,
          priority: 45
        }),

        nudityFilter: new g.view.NudityFilter({
          controller: this.controller,
          model: this.collection.propsQueue,
          priority: 50
        }),

        refineExpand: new g.View({
          tagName: 'a',
          className: 'getty-refine-toggle',
          priority: -40,
        }),

        total: this.total
      };

      // Wrap search input in container because IE10 ignores left/right absolute
      // positioning on input[type="text"]... because.
      this.searchContainer = fields.search = new g.View({ className: 'getty-search-input-container' });

      this.searchContainer.views.add(new g.view.Search({
        controller: this,
        model:      this.collection.propsQueue,
        button:     fields.searchButton,
        priority:   0,
        attributes: {
          type: 'search',
          placeholder: g.text.searchPlaceholder,
          tabindex: 10
        }
      }));

      // Views with negative priority get put in secondary toolbar area.
      this.toolbar.set(fields);

      // Create collapsable "refine" box for additional filters
      // which vary depending on results.
      this.toolbar.secondary.get('refineExpand').$el.text(g.text.refine).prepend('<span class="getty-refine-toggle-arrow">&nbsp;</span>');

      this.views.set('.getty-search-toolbar', this.toolbar);

      // Refinements are singular and can be stacked.
      this.views.set('.getty-refine', new g.view.Refinements({
        tagName: 'div',
        className: 'search-results-refine',
        categories: this.refinementCategories,
        refinements: this.refinements,
        controller: this.controller
      }));

      this.modeFlag();
    },

    // Create sidebar and register hooks on selection.
    createSidebar: function() {
      var options = this.options,
        selection = options.selection,
        sidebar = this.sidebar = new g.view.Sidebar({
          controller: this.controller
        });

        this.views.set('.getty-sidebar', sidebar);

        selection.on('selection:single', this.createSingle, this);
        selection.on('selection:unsingle', this.disposeSingle, this);

        if(selection.single()) {
          this.createSingle();
        }
    },

    // Create single attachment detail view for sidebar.
    createSingle: function() {
      var sidebar = this.sidebar,
        single = this.options.selection.single();

      var attachment = single.id ? g.model.Attachments.all.get(single.id) : false;

      if(attachment) {
        var details = new g.view.Details({
          controller: this.controller,
          model: attachment,
          priority:   80
        });

        sidebar.set('details', details);

        var displayOptions = {
          model: this.model.display(attachment),
          priority: 200,
          userSettings: true
        };

        sidebar.set('display', new g.view.DisplaySettings(displayOptions));

        if(s) {
          s.events = 'event3';
          s.prop1 = s.eVar1 = s.prop2 = s.eVar2 = s.prop6 = s.eVar6 = s.prop7 = s.eVar7 = '';
          s.prop3 = s.eVar3 = single.id;
          s.tl();
        }
      }
    },

    // Refinement changes will always update the query.
    updateSearchRefinements: function(model, collection, op) {
      var searchWithin = '';
      var categoryRefinements = [];

      if(s) {
        var opString = op.add ? "Add" : (typeof op.index === 'number') ? "Remove" : '';

        if(opString) {
          s.prop7 = s.eVar7 = opString + '|' + model.get('category') + '|' + model.get('text');
        }
      }

      // Concatenate free-form search within fields to search query.
      this.refinements.each(function(refinement) {
        if(refinement.get('text') && !refinement.get('id')) {
          searchWithin += refinement.get('text') + ' ';
        }
        else {
          categoryRefinements.push({
            Category: refinement.get('category'),
            Id: refinement.id
          });
        }
      }, this);

      if(searchWithin) {
        this.collection.propsQueue.set('SearchWithin', searchWithin);
      }
      else {
        this.collection.propsQueue.unset('SearchWithin');
      }

      this.collection.propsQueue.set('Refinements', categoryRefinements);

      this.collection.search();
    },

    clearSearchRefinements: function() {
      if(s) {
        s.prop7 = s.eVar7 = '';
      }
      this.collection.propsQueue.set('Refinements', []);
    },

    /**
     * Download the selected image to the Drupal install.
     */
    download: function() {
      var attachment = g.model.Attachments.all.get(this.options.selection.single());

      // Start the download auth, get the proper URL, then fill out the
      // Drupal form with the proper information for better use of AJAX.
      attachment.download().done(function(response) {
        var meta = attachment.toJSON();

        delete meta.attachment;
        delete meta[''];

        g.$tab.find('input[name="url"]').val(response.DownloadUrls[0].UrlAttachment);
        g.$tab.find('input[name="getty-image-meta"]').val(JSON.stringify(meta));

        g.$tab.find('input[name="getty_images_getty_images_download_transfer"]').trigger('getty-images-download');

        // After the download finishes, gettyImagesDownloadComplete
        // ajax action is called.
        Drupal.ajax.prototype.commands.gettyImagesDownloadComplete = function(ajax, response, status) {
          if(status == 'success') {
            attachment.downloaded(response.data);
          }
          attachment.unset('downloading');
          attachment.unset('DownloadToken');
        };
      });
    },

    /**
     * Build the set of possible category refinements.
     *
     * Based on return set from API.
     */
    updateRefinementCategories: function() {
      var refinements = this.collection.results.get('refinements');

      // May have gotten here via a reset.
      if(!refinements) {
        return;
      }

      // Add / update possible refinement categories.
      refinements.each(function(category) {
        var refinementCategory = this.refinementCategories.get(category.id);

        if(!refinementCategory) {
          // Category exists in search refinements, but not in view. Pop into view.
          refinementCategory = new Backbone.Model({
            id: category.id,
            options: new Backbone.Collection()
          });

          this.refinementCategories.add(refinementCategory);
        }

        var options = category.get('options').filter(function(option) {
          return option.get('text') != '(future event)';
        });

        refinementCategory.get('options').set(options);

        // Kill any events that eventually resolve to "(future event)" as text.
        refinementCategory.get('options').on('change:text', function(model) {
          if(model.get('text') == "(future event)") {
            refinementCategory.get('options').remove(model);
          }
        });
      }, this);

      // Remove refinement categories that are no longer applicable.
      this.refinementCategories.each(function(refinementCategory) {
        if(!refinements.get(refinementCategory.id) || (refinementCategory.get('options').filter(function(model) { return !model.get('active'); })).length == 0) {
          this.refinementCategories.remove(refinementCategory.id);
        }
      }, this);

      this.refinementCategories.trigger('sort');
    },

    /**
     * When activated, restore any injected content, focus on search field.
     */
    ready: function() {
      this.updateFlags(this.collection.results);
      this.updateTotal();
      this.$el.find('.search-primary').focus();
    },

    // Create attachments view.
    updateContent: function() {
      var view = this;

      if(!this.attachments) {
        this.createAttachments();
      }
    },

    // Start a new search for a keyword.
    searchKeyword: function(ev) {
      this.collection.props.set('search', _.unescape($(ev.target).text()));
      this.search();
    },

    // Update total count.
    updateTotal: function() {
      var total = 0;

      if(this.model) {
        total = this.collection.results.get('total') || 0;
      }

      var separated = new Number(total).commaString();

      if(total > 0) {
        if(total == 1) {
          this.total.$el.text(g.text.oneResult.replace('%d', separated));
        }
        else {
          this.total.$el.text(g.text.results.replace('%d', separated));
        }
      }
      else {
        this.total.$el.text(g.text.noResults);
      }
    },

    // Handle return key in primary search or click on "Search" button
    // This starts a new top-level search.
    searchOnEnter: function(ev) {
      if(ev.keyCode == 13) {
        this.search();
      }
    },

    // Start a new search.
    search: function() {
      this.refinements.reset();
      this.refinementCategories.reset();

      this.collection.propsQueue.unset('SearchWithin');
      this.collection.propsQueue.unset('Refinements');

      this.collection.search();
    },

    createAttachments: function() {
      this.attachments = new g.view.Attachments({
        collection: this.collection,
        selection:  this.options.selection,
        model:      this.model,
        sortable:   this.options.sortable,
        refreshThreshold: 2,

        resize: true,
        // The single `Attachment` view to be used in the `Attachments` view.
        AttachmentView: this.options.AttachmentView
      });

      this.views.set('.getty-results', this.attachments);

      // Create "More" button, because automatic paging is just too slow with
      // this API.
      this.moreButton = new g.view.More();
      // Load in the template, just once.
      this.moreButton.render();

      // Keep a handy reference to the field to update.
      this.moreButton.$remaining = this.moreButton.$el.find('.getty-number-remaining');

      this.collection.on('add reset', this.updateMore, this);
      this.collection.results.on('change', this.updateMore, this);

      this.updateMore();
    },

    // Calculate remainder and update "More..." link.
    updateMore: function(model, collection, options) {
      // Always keep add more link at the end of the list.
      if(this.attachments) {
        this.attachments.$el.append(this.moreButton.$el);
      }

      // Update the total.
      var total = this.collection.results.get('total');

      if(typeof total == "number") {
        this.moreButton.$remaining.text(new Number(total - this.collection.length).commaString());
      }

      this.updateHaveMore();
    },

    // Load more results.
    more: function() {
      this.collection.more();
    },

    /**
     * Toggle refining state.
     */
    toggleRefinementOptions: function() {
      this.controller.set('refining', !this.controller.get('refining'));
    },

    /**
     * Toggle refining class when refining state changes.
     */
    addRefiningClass: function() {
      this.$el.toggleClass('refining', !!this.controller.get('refining'));
    },

    /**
     * Show loading state.
     */
    updateSearching: function(model) {
      this.$el.toggleClass('search-loading', model.get('searching'));

      if(model.get('searching')) {
        if(model.spinner) {
          model.spinner.stop();
        }

        var opts = { className: 'getty-spinner', color: '#888' };
        var $el;

        if(this.haveMore()) {
          opts.color = '#ddd';
          $el = this.moreButton.$el.find('.getty-more-spinner');
        }
        else {
          opts.color = '#888';
          opts.lines = 11;
          opts.length = 21;
          opts.width = 9;
          opts.radius = 38;
          $el = this.$el.find('.getty-search-spinner')
        }

        model.spinner = new Spinner(opts);
        model.spinner.spin($el[0]);
      }
      else if(model.spinner) {
        model.spinner.stop();
      }
    },

    /**
     * Add flags to container for various states.
     */
    updateFlags: function(model) {
      this.$el.toggleClass('have-searched', !!model.get('searched'));
      this.$el.toggleClass('have-results', model.get('total') > 0);

      var refinements = model.get('refinements');

      this.$el.toggleClass('have-refinements', refinements && refinements.length > 0);
    },

    /**
     * Add .search-editorial or .search-creative classes to top-level container.
     */
    modeFlag: function() {
      this.$el.removeClass('search-editorial').removeClass('search-creative')
        .addClass('search-' + this.collection.propsQueue.get('ImageFamilies'));
    },

    /**
     * Check for if there are more images to be loaded.
     */
    haveMore: function() {
      return this.collection.length > 0 && this.collection.length < this.collection.results.get('total');
    },

    /**
     * Add .have-more flag to container when there are more results.
     */
    updateHaveMore: function(model, collection) {
      this.$el.toggleClass('have-more', this.haveMore());
      this.$el.toggleClass('no-more', !this.haveMore());
    }
  });

  // Attachments.
  g.view.Attachments = g.View.extend({
    tagName: 'ul',
    className: 'getty-attachments',

    cssTemplate: g.template('getty-attachments-css'),

    initialize: function() {
      this.el.id = _.uniqueId('__attachments-view-');

      _.defaults(this.options, {
        AttachmentView:     g.view.Attachment,
        resize:             true
      });

      // Track views by their unique ID.
      this._viewsByCid = {};

      // When an item is added to the collection, insert it at the
      // same spot in the index.
      this.collection.on('add', function(attachment) {
        this.views.add(this.createThumbnailView(attachment), {
          at: this.collection.indexOf(attachment)
        });
      }, this);

      this.collection.on('remove', function(attachment) {
        var view = this._viewsByCid[attachment.cid];
        delete this._viewsByCid[attachment.cid];

        if(view) {
          view.remove();
        }
      }, this);

      // Update when collection is reset.
      this.collection.on('reset', this.render, this);

      // Bind CSS resizing.
      _.bindAll(this, 'css');
      this.model.on('change:edge change:gutter', this.css, this);
      this._resizeCss = _.debounce(_.bind(this.css, this), this.refreshSensitivity);
      if(this.options.resize) {
        $(window).on('resize.attachments', this._resizeCss);
      }

      // Call this.css() after this view has been rendered in the DOM so
      // attachments get proper width applied.
      _.defer(this.css, this);
    },

    // Begone... me!
    dispose: function() {
      this.collection.props.off(null, null, this);
      $(window).off('resize.attachments', this._resizeCss);
      // Call 'dispose' directly on the parent class.
      g.View.prototype.dispose.apply(this, arguments);
    },

    // Resize attachments.
    css: function() {
      var $css = $('#' + this.el.id + '-css');

      g.view.Attachments.$head().append(this.cssTemplate({
        id:     this.el.id,
        edge:   this.edge(),
        gutter: this.model.get('gutter')
      }));

      $css.remove();
    },

    edge: function() {
      var edge = this.model.get('edge'),
        gutter, width, columns;

      if (! this.$el.is(':visible') || this.model.get('fixed')) {
        return edge;
      }

      gutter  = this.model.get('gutter') * 2;
      width   = this.$el.width() - gutter;
      columns = Math.ceil(width / (edge + gutter));
      edge = Math.floor((width - (columns * gutter)) / columns);
      return edge;
    },

    prepare: function() {
      // Create all of the Attachment views, and replace
      // the list in a single DOM operation.
      if (this.collection.length) {
        this.views.set(this.collection.map(this.createThumbnailView, this));
      } else {
        this.views.unset();
      }
    },

    /**
     * Create an attachment view for an attachment model.
     */
    createThumbnailView: function(attachment) {
      var view = new this.options.AttachmentView({
        controller:           this.controller,
        model:                attachment,
        collection:           this.collection,
        selection:            this.options.selection
      });

      return this._viewsByCid[ attachment.cid ] = view;
    },
  }, {
    $head: (function() {
      var $head;
      return function() {
        return $head = $head || $('head');
      };
    }())
  });

  // Selected images.
  g.view.Attachments.Selection = g.view.Attachments.extend({
  });

  // View used to display the "More" button.
  g.view.More = g.View.extend({
    template: 'getty-images-more',
    className: 'getty-images-more getty-attachment',
    tagName: 'li',
  });

  // Comp Display Options.
  g.view.DisplaySettings = g.View.extend({
    template: 'getty-display-settings',

    events: {
      'change [name="size"]': 'updateSize'
    },

    initialize: function() {
      g.user.on('change:loggedIn', this.render, this);
      this.model.on('change:sizes', this.render, this);
    },

    render: function() {
      g.View.prototype.render.apply(this,arguments);
    },

    updateSize: function(ev) {
      this.model.set('size', ev.target.value);
    }
  });

  // The title bar, containing the logo, "privacy" and "About" links.
  g.view.TitleBar = g.View.extend({
    template: 'getty-title-bar',

    events: {
      'click .getty-about-link': 'showAbout',
      'click .getty-login-toggle': 'toggleUserPanel',
      'click .getty-mode-change': 'unsetMode'
    },

    initialize: function() {
      g.View.prototype.initialize.apply(this, arguments);

      if(this.controller.get('unsupported')) {
        return;
      }

      this.controller.on('change:mode', this.render, this);
      this.controller.on('change:mode', this.updateMode, this);

      g.user.on('change:loggedIn', this.render, this);

      $(document).off('.getty-user-panel-close');
      $(document).on('click.getty-user-panel-close', function(ev) {
        var $target = $(ev.target);

        if($target.closest('.getty-user-session').length == 0 && $target.closest('.getty-login-toggle').length == 0) {
          g.user.set('promptLogin', false);
        }
      });

      if(this.controller.get('mode') == 'login') {
        this.addUserPanel();
      }
    },

    addUserPanel: function(mode) {
      if(!this.user) {
        this.user = new g.view.User({
          controller: this,
          model: g.user
        });
      }

      this.views.set('.getty-user-session', this.user);
    },

    updateMode: function(model, mode) {
      if(!mode) {
        this.views.unset('.getty-user-session');
        delete(this.user);
      }

      else if(mode == 'login') {
        this.addUserPanel();
      }
    },

    prepare: function() {
      return this.controller.attributes;
    },

    unsetMode: function() {
      g.user.settings.unset('mode');
    },

    toggleUserPanel: function() {
      g.user.set('promptLogin', !g.user.get('promptLogin'))
    },

    showAbout: function() {
      this.controller.set('content', 'getty-about-text');
    }
  });

  // The primary search field. Refreshes when model changes.
  g.view.Search = g.View.extend({
    tagName: 'input',
    className: 'getty-text-input',

    events: {
      'keyup': 'update'
    },

    initialize: function(options) {
      this.button = options.button;

      // Enable or disable search button based on text presence in search box.
      this.model.on('change:search', this.changeSearch, this);

      // Disable button while searching.
      if(this.controller) {
        this.controller.on('change:searching', function(model) {
          this.toggleButtonState(!model.get('searching'));
        }, this);
      }

      this.changeSearch();
    },

    ready: function() {
      this.render();
    },

    update: function(ev) {
      var term = this.$el.val();

      this.model.set('search', this.$el.val());

      ev.stopPropagation();

      if(ev.keyCode === 13) {
        this.controller.search();
      }
    },

    // Disable or enable the button based on search text.
    changeSearch: function() {
      var term = this.$el.val();

      this.toggleButtonState(typeof term == 'string' && term.match(/\S+/));
    },

    // Enable or disable the search button based on state.
    toggleButtonState: function(enabled) {
      if(this.button) {
        if(enabled) {
          this.button.$el.removeAttr('disabled');
        }
        else {
          this.button.$el.attr('disabled', 'disabled');
        }
      }
    }
  });

  // The "About" screen, containing text and a close button.
  g.view.About = g.View.extend({
    template: 'getty-about-text',

    events: {
      'click .getty-about-close': 'close',
    },

    close: function() {
      this.controller.set('content', 'getty-images-browse');
    },

    closeAboutOnEscape: function(ev) {
      if(ev.keyCode == 27) {
        this.close();
        ev.stopPropagation();
      }
    }
  });

  // Your browser is unsupported. Wah wah.
  g.view.UnsupportedBrowser = g.View.extend({
    template: 'getty-unsupported-browser'
  });

  // View used to display a single Getty Image thumbnail in the results grid.
  g.view.Attachment = g.View.extend({
    tagName: 'li',
    template: 'getty-attachment',

    events: {
      'click': 'toggleSelection'
    },

    initialize: function() {
      this.model.on('change:Status', function() {
        this.$el.addClass('status-' + this.model.get('Status'));
      }, this);

      this.model.on('selection:single selection:unsingle', this.details, this);
    },

    details: function(model, collection) {
      var selection = this.options.selection,
        details;

      if (selection !== collection) {
        return;
      }

      details = selection.single();
      this.$el.toggleClass('getty-details', details === this.model);
    },

    // Only add to selection. Hold infinitely many.
    toggleSelection: function() {
      var collection = this.collection,
        selection = this.options.selection;

      if(!selection) {
        return;
      }

      selection.unshift(this.model);
      selection.single(this.model);
    },

    prepare: function() {
      return this.model.attributes;
    }
  });

   // Settings / Info for a particular selected getty image
   // shown in the sidebar.
  g.view.Details = g.View.extend({
    template: 'getty-image-details',

    events: {
      // Propagate changes to Getty Form.
      'change .getty-attachment-details input': 'updateModel',
    },

    initialize: function() {
      g.user.on('change:loggedIn', this.render, this);

      this.views.set('.getty-image-thumbnail', new g.view.DetailImage({
        model: this.model
      }));

      this.views.set('.getty-download-authorizations', new g.view.DownloadAuthorizations({
        model: this.model
      }));

      this.views.set('.getty-image-details-list', new g.view.DetailList({
        model: this.model
      }));

      // Load up additional details.
      this.model.fetch();
    },

    /**
     * Propagate changes to form elements to attachment model.
     */
    updateModel: function() {
      var model = this.model;

      this.$el.find('input, textarea, select').each(function() {
        if(this.type == "radio") {
          if(this.checked) {
            model.set(this.name, this.value);
          }
        }
        else if(this.type == "checkbox") {
          if(this.checked) {
            model.set(this.name, this.value);
          }
          else {
            model.unset(this.name);
          }
        }
        else {
          model.set(this.name, this.value);
        }
      });
    },

    prepare: function() {
      return this.model.attributes;
    },
  });

  // The thumbnail and possible image details for an image.
  g.view.DetailImage = g.View.extend({
    template: 'getty-detail-image',

    initialize: function() {
      this.model.on('change:files', this.render, this);
    },

    render: function() {
      g.View.prototype.render.apply(this, arguments);
    },

    prepare: function() {
      return this.model.attributes;
    },
  });

  // The list of image details and possible image details for an image.
  g.view.DetailList = g.View.extend({
    template: 'getty-image-details-list',

    initialize: function() {
      // Attach changes to login state to this view.
      this.model.on('change', this.render, this);
    },

    prepare: function() {
      return this.model.attributes;
    },
  });

  // The set of image sizes available, and the download button.
  g.view.DetailSizes = g.View.extend({
    template: 'getty-images-detail-sizes',
    initialize: function() {
      // Attach changes to login state to this view.
      g.user.on('change:loggedIn', this.changeLoginState, this);
      this.model.on('change:files change:SizesDownloadableImages', this.render, this);
      this.model.on('change:downloading', this.updateDownloading, this);
    },

    /**
     * Re-fetch image details (with downloadable sizes).
     *
     * When user login state changes.
     */
    changeLoginState: function(model, loggedIn) {
      if(loggedIn && !this.model.get('SizesDownloadableImages')) {
        this.model.fetch();
      }
      else {
        this.model.unset('LargestDownloadAuthorizations');
        this.model.unset('DownloadAuthorizations');
        this.model.unset('SizesDownloadableImage');
        this.model.unset('DownloadSizeKey');
      }
    },

    prepare: function() {
      return this.model.attributes;
    },
  });

  // Download authorizations.
  g.view.DownloadAuthorizations = g.View.extend({
    template: 'getty-download-authorizations',

    events: {
      'change': 'updateModel',
    },

    initialize: function() {
      this.model.on('change:files change:downloading', this.render, this);

      g.user.on('change:loggedIn', this.render, this);
      g.user.on('change:loggedIn', this.model.fetch, this.model);

      this.model.on('change:files change:DownloadAuthorizations change:LargestDownloadAuthorizations change:DownloadSizeKey', this.updateAttachment, this);
      this.updateAttachment();
    },

    updateAttachment: function() {
      this.render();

      if(!this.model.get('DownloadToken')) {
        var $selected = this.$el.find('[data-downloaded="true"]').parents('.getty-download-auth');

        if($selected.length) {
          $selected.find('select').trigger('change');
        }
        else {
          this.$el.find('select').trigger('change');
        }
      }
    },

    /**
     * Show / hide download spinner while attachment is downloading.
     */
    render: function() {
      g.View.prototype.render.apply(this, arguments);

      if(this.model.get('downloading')) {
        new Spinner({
          className: 'getty-spinner',
          color: '#888',
          lines: 7,
          length: 4,
          radius: 5,
        }).spin(this.$el.find('.getty-download-spinner')[0]);
      }
    },

    prepare: function() {
      return this.model.attributes;
    },

    updateModel: function(ev) {
      var $auth = $(ev.target).parents('.getty-download-auth');
      var $size = $auth.find('select[name="DownloadSizeKey"]');

      this.model.set('DownloadSizeKey', $size.val());
      this.model.set('DownloadToken', $size.find('option:selected').data('downloadtoken'));
    },
  });

  // View representing login session.
  g.view.User = g.View.extend({
    template: 'getty-images-user',
    prepare: function() {
      return this.model.attributes;
    },

    initialize: function() {
      this.listenTo(this.model, 'change', this.render);
    },

    events: {
      'click .getty-images-logout': 'logout',
      'change [name="getty-login-username"]': 'updateUsername',
      'click .button-primary': 'login',
      'keyup input': 'loginOnEnter',
      'keydown input': 'tabSwitch'
    },

    loginOnEnter: function(ev) {
      if(ev.keyCode == 13) {
        ev.preventDefault();
        this.login();
      }
    },

    tabSwitch: function(ev) {
      if(ev.keyCode == 9) {
        // Backwards.
        if(ev.shiftKey) {
          if(ev.target.name == 'getty-login-password') {
            this.$el.find('.getty-login-username input').focus();
          }
          else if(ev.target.name == 'getty-login-username') {
            this.$el.find('.getty-login-button').focus();
          }
          else if($(ev.target).hasClass('getty-login-button')) {
            this.$el.find('.getty-login-password input').focus();
          }
        }
        else {
          if(ev.target.name == 'getty-login-password') {
            this.$el.find('.getty-login-button').focus();
          }
          else if(ev.target.name == 'getty-login-username') {
            this.$el.find('.getty-login-password input').focus();
          }
          else if($(ev.target).hasClass('getty-login-button')) {
            this.$el.find('.getty-login-username input').focus();
          }
        }

        ev.preventDefault();
      }
    },

    updateUsername: function(ev) {
      this.model.set('username', ev.currentTarget.value);
    },

    /**
     * Log in the user, using the password in the password field.
     */
    login: function() {
      var self = this;

      this.model.login(this.$el.find('[name="getty-login-password"]').val());
    },

    /**
     * Log out the user.
     */
    logout: function(ev) {
      this.model.logout();
    },

    /**
     * Add spinner when logging in.
     */
    render: function() {
      g.View.prototype.render.apply(this, arguments);

      if(this.model.get('loggingIn')) {
        new Spinner({
          className: 'getty-spinner',
          width: 2,
          lines: 7,
          width: 4,
          length: 3,
          color: '#888',
          radius: 3,
        }).spin(this.$el.find('.getty-login-spinner')[0]);
      }

    }
  });

  // Comp licensing agreement view.
  g.view.CompAgreement = g.View.extend({
    template: 'getty-comp-license-agreement',
    className: 'getty-comp-license-container'
  });

  // Simple button.
  g.view.Button = g.View.extend({
    tagName: 'button',
    className: 'getty-images-button',

    events: {
      'click': 'click'
    },

    click: function() {
      this.options.click && this.options.click()
    },

    initialize: function() {
      this.$el.text(this.options.text || "Click Me");

      if(this.model) {
        this.model.on('change', this.update, this);
        this.update.call(this, this.model);
      }
    },

    update: function(model) {
      var attr = model.attributes;

      if(attr.text) {
        this.$el.text(attr.text);
      }

      if(attr.disabled) {
        this.$el.attr('disabled', 'disabled');
      }
      else {
        this.$el.removeAttr('disabled');
      }
    }
  });

  // Sidebar with details for selected image.
  g.view.Sidebar = g.view.PriorityList.extend({
    className: 'getty-details-sidebar'
  });

  // The bottom toolbar: contains selection + button.
  g.view.Toolbar = g.View.extend({
    tagName:   'div',
    className: 'getty-images-toolbar',

    initialize: function() {
      var
        selection = this.selection = this.controller.get('selection'),
        library = this.library = this.controller.get('library');

      this._views = {};

      // The toolbar is composed of two `PriorityList` views.
      this.primary   = new g.view.PriorityList();
      this.secondary = new g.view.PriorityList();
      this.primary.$el.addClass('getty-toolbar-primary');
      this.secondary.$el.addClass('getty-toolbar-secondary');

      this.views.set([ this.secondary, this.primary ]);

      if (this.options.items) {
        this.set(this.options.items, { silent: true });
      }

      if (! this.options.silent) {
        this.render();
      }

      if (selection) {
        selection.on('add remove reset', this.refresh, this);
      }

      if (library) {
        library.on('add remove reset', this.refresh, this);
      }
    },

    dispose: function() {
      if(this.selection) {
        this.selection.off(null, null, this);
      }

      if(this.library) {
        this.library.off(null, null, this);
      }

      // Call 'dispose' directly on the parent class.
      return g.View.prototype.dispose.apply(this, arguments);
    },

    ready: function() {
      this.refresh();
    },

    set: function(id, view, options) {
      var list;
      options = options || {};

      // Accept an object with an `id` : `view` mapping.
      if(_.isObject(id)) {
        _.each(id, function(view, id) {
          this.set(id, view, { silent: true });
        }, this);

      } else {
        if (! (view instanceof Backbone.View)) {
          view.classes = [ 'media-button-' + id ].concat(view.classes || []);
          view = new g.view.Button(view).render();
        }

        view.controller = view.controller || this.controller;

        this._views[ id ] = view;

        list = view.options.priority < 0 ? 'secondary' : 'primary';
        this[ list ].set(id, view, options);
      }

      if (! options.silent) {
        this.refresh();
      }

      return this;
    },

    get: function(id) {
      return this._views[id];
    },

    unset: function(id, options) {
      delete this._views[id];
      this.primary.unset(id, options);
      this.secondary.unset(id, options);

      if(!options || !options.silent) {
        this.refresh();
      }

      return this;
    },

    refresh: function() {
      var
        library = this.controller.get('library'),
        selection = this.controller.get('selection');

      _.each(this._views, function(button) {
        if(!button.model || !button.options || !button.options.requires) {
          return;
        }

        var requires = button.options.requires,
          disabled = false;

        // Prevent insertion of attachments if any of them are still uploading.
        disabled = _.some(selection.models, function(attachment) {
          return attachment.get('uploading') === true;
        });

        if (requires.selection && selection && ! selection.length) {
          disabled = true;
        } else if (requires.library && library && ! library.length) {
          disabled = true;
        }
        button.model.set('disabled', disabled);
      });
    }
  });

  // The bottom action bar.
  g.view.ActionBar = g.view.Toolbar.extend({
    className: 'getty-images-toolbar getty-frame-toolbar',

    events: {
      'click .getty-comp-buttons .button-primary': 'copyComp',
      'click .getty-comp-buttons .getty-cancel-link': 'cancel'
    },

    initialize: function(options) {
      g.view.Toolbar.prototype.initialize.apply(this, arguments);

      this._views = {};

      this.selection = this.controller.get('selection');
      this.library   = this.controller.get('library');

      this.selection = new g.view.Selection({
        controller: this.controller,
        collection: options.collection
      });

      this.collection.on('selection:single change:files', this.updateButtons, this);

      this.secondary.set('selection', this.selection);

      this.controller.on('change:mode', this.createButtons, this);
      this.createButtons();

      if(this.selection) {
        this.selection.on('add remove reset', this.refresh, this);
      }

      if(this.library) {
        this.library.on('add remove reset', this.refresh, this);
      }
    },

    /**
     * Insert appropriate buttons for the current mode.
     */
    createButtons: function() {
      var self = this;
      var copyOptions, copyModel = {
        disabled: true,
        text: g.text.copyComp,
      };

      if(this.controller.get('mode') == 'embed') {
        copyModel.text = g.text.embedImage;
      }

      this.copyButton && this.copyButton.remove();
      this.selectButton && this.selectButton.remove();

      copyOptions = {
        model: new Backbone.Model({
          text: copyModel.text,
          disabled: true
        }),
        className: 'button-secondary getty-images-button',
        requires: { selection: true },
        priority: 9,
        click: function() {
          self.copyImage();
        }
      }

      // Embed mode only gets "copy to clipboard" option.
      if(this.controller.get('mode') == 'embed') {
        copyOptions.className = 'button-primary getty-images-button';
      }

      this.copyButton = new g.view.Button(copyOptions);
      this.set('copyButton', this.copyButton);

      // Allow selection of image when done in image field.
      if(this.controller.get('context') == 'image_field') {
        this.selectButton = new g.view.Button({
          model: new Backbone.Model({
            text: g.text.useImage,
            disabled: true
          }),
          className: 'button-primary getty-images-button',
          requires: { selection: true },
          priority: 10,
          click: function() {
            self.selectImage()
          }
        });
        this.set('selectButton', this.selectButton);
      }

      this.updateButtons();
    },

    /**
     * Update button state and text.
     */
    updateButtons: function() {
      if(this.collection.length == 0) {
        this.copyButton.model.set({
          disabled: true,
          text: g.text.selectImage
        });
        this.selectButton && this.selectButton.$el.hide();
      }
      else {
        this.copyButton.model.set('disabled', false);

        var image = this.collection.single();

        if(this.controller.get('mode') == 'embed') {
          this.copyButton.model.set('text', g.text.embedImage);
        }
        else {
          if(image.hasDownloads()) {
            this.selectButton && this.selectButton.$el.show();
            this.copyButton.model.set('text', g.text.insertImage);
          }
          else {
            this.selectButton && this.selectButton.$el.hide();
            this.copyButton.model.set('text', g.text.copyComp);
          }
        }
      }
    },

    /**
     * Throw out this toolbar.
     */
    dispose: function() {
      if(this.selection) {
        this.selection.off(null, null, this);
      }

      if(this.library) {
        this.library.off(null, null, this);
      }

      return g.View.prototype.dispose.apply(this, arguments);
    },

    /**
     * Refresh when ready.
     */
    ready: function() {
      this.refresh();
    },

    /**
     * Set views within this toolbar. If the view is not a Backbone.View, create a button.
     */
    set: function(id, view, options) {
      var list;
      options = options || {};

      // Accept an object with an `id` : `view` mapping.
      if(_.isObject(id)) {
        _.each(id, function(view, id) {
          this.set(id, view, { silent: true });
        }, this);
      } else {
        if(!(view instanceof Backbone.View)) {
          view.classes = [ 'media-button-' + id ].concat(view.classes || []);
          view = new g.view.Button(view).render();
        }

        view.controller = view.controller || this.controller;

        this._views[id] = view;

        list = view.options.priority < 0 ? 'secondary' : 'primary';
        this[list].set(id, view, options);
      }

      if (!options.silent) {
        this.refresh();
      }

      return this;
    },

    /**
     * Get a view based on ID.
     */
    get: function(id) {
      return this._views[ id ];
    },

    /**
     * Remove a view entirely based on an id.
     */
    unset: function(id, options) {
      delete this._views[id];

      this.primary.unset(id, options);
      this.secondary.unset(id, options);

      if(!options || !options.silent) {
        this.refresh();
      }

      return this;
    },

    /**
     * Refresh button states based on requirements.
     */
    refresh: function() {
      var
        library = this.controller.get('library'),
        selection = this.controller.get('selection');

      _.each(this._views, function(button) {
        if(!button.model || !button.options || !button.options.requires) {
          return;
        }

        var requires = button.options.requires,
          disabled = false;

        // Prevent insertion of attachments if any of them are still uploading.
        disabled = _.some(selection.models, function(attachment) {
          return attachment.get('uploading') === true;
        });

        if(requires.selection && selection && !selection.length) {
          disabled = true;
        } else if(requires.library && library && !library.length) {
          disabled = true;
        }

        button.model.set('disabled', disabled);
      });
    },

    /**
     * Select the selected image as the image value.
     */
    selectImage: function() {
      var image = this.collection.single();

      if(!image || !image.hasDownloads()) {
        return;
      }

      if(s && image) {
        s.events = 'event5';
        s.prop1 = s.eVar1 = s.prop2 = s.eVar2 = '';
        s.prop3 = s.eVar3 = image.get('id');
        s.prop5 = s.eVar5 = this.controller.get('mode') === 'embed' ? "Embed" : "License";
        s.tl();
      }

      this.controller.select(image);
    },

    /**
     * Select the selected image as the image value.
     */
    copyImage: function() {
      var image = this.collection.single();

      if(this.controller.get('mode') != 'embed' && !image.hasDownloads()) {
        // Show license agreement for inserting comp.
        if(!this.agreement) {

          this.agreement = new g.view.CompAgreement({
            controller: this
          });
        }

        this.agreement.$el.hide();
        this.primary.set('agreement', this.agreement);
        this.agreement.$el.fadeIn();
      }
      else {
        if(s && image) {
          s.events = 'event5';
          s.prop1 = s.eVar1 = s.prop2 = s.eVar2 = '';
          s.prop3 = s.eVar3 = image.get('id');
          s.tl();
        }

        this.copy();
      }
    },

    /**
     * Insert the selected image as a comp into.
     *
     * TODO: Have this function copy the proper markup to the clipboard.
     */
    copyComp: function() {
      var image = this.controller.get('selection').single();

      if(s && image) {
        s.events = 'event4';
        s.prop1 = s.eVar1 = s.prop2 = s.eVar2 = '';
        s.prop3 = s.eVar3 = image.get('id');
        s.tl();
      }

      this.copy();
    },

    copy: function() {
      if(this.agreement) {
        this.agreement.$el.remove();
        delete this.agreement;
      }

      this.controller.copy();
    },

    cancel: function() {
      if(this.agreement) {
        this.agreement.$el.remove();
        delete this.agreement;
      }
    },

  });

  // Selection thumbnail.
  g.view.Attachment.Selection = g.view.Attachment.extend({
    className: 'getty-attachment getty-selection',

    // On click, just select the model, instead of removing the model from
    // the selection.
    toggleSelection: function() {
      this.options.selection.single(this.model);
    }
  });

  // * Selection.
  g.view.Selection = g.View.extend({
    template:  'getty-images-selection',

    initialize: function() {
      this.attachments = new g.view.Attachments.Selection({
        controller: this.controller,
        collection: this.collection,
        selection:  this.collection,
        AttachmentView: g.view.Attachment.Selection,
        model:      new Backbone.Model({
          edge:   40,
          fixed: true
        })
      });

      this.views.set('.getty-selection-view', this.attachments);
      this.collection.on('add remove reset', this.refresh, this);
    },

    ready: function() {
      this.refresh();
    },

    refresh: function() {
      // If the selection hasn't been rendered, bail.
      if(!this.$el.children().length) {
        return;
      }

      var collection = this.collection;

      // If nothing is selected, display nothing.
      this.$el.toggleClass('empty', !collection.length);
      this.$el.toggleClass('one', 1 === collection.length);

      this.$('.count').text(g.text.recentlyViewed);
    },

    edit: function(event) {
      event.preventDefault();

      if(this.options.editable) {
        this.options.editable.call(this, this.collection);
      }
    },

    clear: function(event) {
      event.preventDefault();
      this.collection.reset();
    }
  });

  // Mode selection.
  g.view.ModeSelect = g.View.extend({
    tagName:   'div',
    template:  'getty-choose-mode',

    events: {
      'click .getty-embedded-mode': 'chooseEmbeddedMode',
      'click .getty-login-mode': 'chooseLoginMode'
    },

    initialize: function() {
      this.controller.on('change:mode', this.setMode, this);

      this.setMode();
    },

    setMode: function() {
      var self = this;

      if(this.controller.get('mode') == 'login') {
        this.views.set('.getty-login-panel', new g.view.User({
          controller: this,
          model: g.user
        }));

        setTimeout(function() {
          var username = self.$el.find('.getty-login-username input').val();

          if(!username) {
            self.$el.find('.getty-login-username input').focus();
          }
          else {
            self.$el.find('.getty-login-password input').focus();
          }
        }, 20);
      }
      else {
        this.views.unset('.getty-login-panel');
      }

      this.render();
    },

    ready: function() {
      this.$el.find('.getty-login-username').focus();
    },

    prepare: function() {
      return this.controller.attributes;
    },

    chooseEmbeddedMode: function() {
      g.user.settings.set('mode', 'embed');
    },

    chooseLoginMode: function() {
      g.user.settings.set('mode', 'login');
    }
  });

  // The Getty "Clipboard" notification region.
  g.view.Clipboard = g.View.extend({
    template: 'getty-clipboard',

    initialize: function() {
      this.model.on('change:value', this.bind, this);
      this.model.on('change', this.render, this);
    },

    // Bind or unbind paste key binding from textareas and text inputs.
    bind: function(model, value) {
      if(value) {
        g.frame && g.frame.close();
        $('textarea, input[type="text"]').on('keydown', this.keyHandler());

        this.$el.parent().addClass('getty-clipboard-active');
      }
      else {
        $('textarea, input[type="text"]').off('keydown', this.keyHandler());
        this.$el.parent().removeClass('getty-clipboard-active');
      }
    },

    prepare: function() {
      return this.model.attributes;
    },

    // Return a cached function (so we can bind 'this' properly) which listens
    // for cmd- or ctrl-v on input elements and simulates a paste from the fake
    // clipboard.
    keyHandler: function() {
      var self = this;

      return self._keyHandler || (self._keyHandler = function(ev) {
        var val = this.value, text = self.model.get('value'), endIndex, range;

        if((ev.metaKey || ev.ctrlKey) && ev.keyCode == 86) {
          if (typeof this.selectionStart != "undefined" && typeof this.selectionEnd != "undefined") {
            endIndex = this.selectionEnd;
            this.value = val.slice(0, this.selectionStart) + text + val.slice(endIndex);
            this.selectionStart = this.selectionEnd = endIndex + text.length;
          } else if (typeof document.selection != "undefined" && typeof document.selection.createRange != "undefined") {
            this.focus();
            range = document.selection.createRange();
            range.collapse(false);
            range.text = text;
            range.select();
          }

          self.model.unset('value');

          ev.preventDefault();
        }
      });
    }
  });

})(jQuery);
